#!/bin/bash

sudo apt update
sudo apt install build-essential gcc g++ make libasound2-dev libpulse-dev libssl-dev libcurl4-openssl-dev libasound2-dev libsdl2-dev libv4l-dev libx11-dev libavformat-dev libswscale-dev
wget https://github.com/pjsip/pjproject/archive/refs/tags/2.14.1.tar.gz -O pjsip.tar.gz
tar xvf pjsip.tar.gz