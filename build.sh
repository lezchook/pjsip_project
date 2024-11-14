#!/bin/bash

sudo ./configure --enable-g711-codec \
            --disable-ilbc-codec \
            --disable-g722-codec \
            --disable-gsm-codec \
            --disable-speex-codec \
            --disable-opus \
            --disable-l16-codec \
            --disable-g7221-codec \
            --disable-silk \
            --disable-bcg729 \
            --disable-upnp \
            --disable-speex-aec \
            --disable-extra-checks \
            --disable-shared \
            --disable-floating-point \
            --enable-epoll \
            --disable-video \
            --enable-resample \
            --disable-ffmpeg \
            --disable-libuuid \
            --disable-ssl \
            --disable-ext-sound \
            --disable-small-filter \
            --disable-assert \
            --disable-errno \
            --disable-speex-codec \
            --enable-jitter-buffer-min-size=4 \
	    --enable-jitter-buffer-max-size=20 \
            CFLAGS="-O3 -march=native -mtune=native -DNDEBUG -s -DPJSIP_MAX_PKT_LEN=1200 -DPJMEDIA_ECHO_TAIL=50 -DPJMEDIA_ECHO_SKEW=15 -DPJMEDIA_SND_DEFAULT_PLAY_LATENCY=5 -DPJMEDIA_SND_DEFAULT_REC_LATENCY=5 -DPJ_ENABLE_EXTRA_CHECK=0 -DPJ_THREAD_SET_STACK_SIZE=0 -DPJ_THREAD_DESC_SIZE=128"
sudo make dep
sudo make
sudo make install