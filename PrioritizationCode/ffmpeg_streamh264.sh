#!/bin/bash
ffmpeg -s cif -f rawvideo -pix_fmt yuv420p -re -i $1 -profile:v baseline -vcodec libx264  -x264-params keyint=30:min-keyint=30:bframes=1:slices=1 -f h264 udp://127.0.0.1:39082
#udp://140.154.1.30:39082
