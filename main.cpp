#include <pjsua2.hpp>
#include <iostream>
#include <thread>

using namespace pj;

class MyCall : public Call {
public:
    MyCall(Account &acc, int callId = PJSUA_INVALID_ID) : Call(acc, callId) {}

    virtual void onCallState(OnCallStateParam &prm) override {
        CallInfo ci = getInfo();
        std::cout << "*** Call state changed: " << ci.stateText << std::endl;

        if (ci.state == PJSIP_INV_STATE_CONFIRMED) {
            std::cout << "Call connected!" << std::endl;

            sendDTMF("9");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            sendDTMF("014");

            AudioMedia callAudio = getAudioMedia(-1);  
            AudioMedia& playbackDev = Endpoint::instance().audDevManager().getPlaybackDevMedia();
            AudioMedia& captureDev = Endpoint::instance().audDevManager().getCaptureDevMedia();

            try {
                callAudio.startTransmit(playbackDev);
                captureDev.startTransmit(callAudio);
            } catch (Error &err) {
                std::cout << "Error connecting audio: " << err.info() << std::endl;
            }
        } 
        else if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
            std::cout << "Call disconnected!" << std::endl;
            delete this;
        }
    }

    void sendDTMF(const std::string &digits) {
        CallSendDtmfParam dtmfParam;
        dtmfParam.digits = digits;
        try {
            sendDtmf(dtmfParam);
            std::cout << "DTMF tones sent: " << digits << std::endl;
        } catch (Error &err) {
            std::cout << "Error sending DTMF: " << err.info() << std::endl;
        }
    }
};

class MyAccount : public Account {
public:
    virtual void onRegState(OnRegStateParam &prm) {
        AccountInfo ai = getInfo();
        std::cout << (ai.regIsActive ? "*** Register:" : "*** Unregister:")
                  << " code=" << prm.code << std::endl;
    }

    virtual void onIncomingCall(OnIncomingCallParam &prm) {
        std::cout << "Incoming call!" << std::endl;
        MyCall *call = new MyCall(*this, prm.callId);
        call->answer(200);
    }
};

int main() {
    Endpoint ep;
    ep.libCreate();

    EpConfig ep_cfg;
    ep_cfg.uaConfig.threadCnt = 1;
    ep_cfg.medConfig.clockRate = 16000;
    ep_cfg.medConfig.ecTailLen = 0;
    ep_cfg.medConfig.sndClockRate = 16000;
    ep_cfg.medConfig.noVad = true;
    ep_cfg.medConfig.sndRecLatency = 1;
    ep_cfg.medConfig.sndPlayLatency = 1;
    ep_cfg.medConfig.audioFramePtime = 10;
    ep.libInit(ep_cfg);

    TransportConfig tcfg;
    tcfg.port = 5062;

    try {
        ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    } catch (Error &err) {
        std::cout << err.info() << std::endl;
        return 1;
    }

    ep.libStart();
    std::cout << "*** PJSUA2 STARTED ***" << std::endl;

    AccountConfig acfg;
    acfg.idUri = "sip:*@*";
    acfg.regConfig.registrarUri = "sip:*";
    AuthCredInfo cred("digest", "*", "*", 0, "*");
    acfg.sipConfig.authCreds.push_back(cred);

    MyAccount *acc = new MyAccount;
    acc->create(acfg);

    MyCall *call = new MyCall(*acc);
    CallOpParam prm(true);
    prm.opt.audioCount = 1;
    try {
        call->makeCall("sip:*@*", prm);
    } catch (Error &err) {
        std::cout << "Error making call: " << err.info() << std::endl;
    }

    while (true) {
        ep.libHandleEvents(50);
    }

    delete call;
    delete acc;
    ep.libDestroy();

    return 0;
}