# esp32-eye-s3-md with Yolo v11n

## Background
After purchasing a ["clone" ESP32 devkit](https://www.aliexpress.com/item/1005008393479463.html) with an ESP32 S3, LCD and camera; I was left with a facial recognition demo and no usable reference source code. I tried using the original board's source, but it was very outdated.

I then started on a journey to get a BSP up and running that works with this board which can display the camera image on the LCD with the YOLO detections.

The end result is yolo v11n running at 11 seconds per inference. Currently the pertained network is trained using the COCO dataset with 320x320 images. I has some trouble getting 320x320 frames from the camera, so I settle on using 240x240 RAW RGB565 frames.

This uses the [Espressif deep-learning library for AIoT applications](https://github.com/espressif/esp-dl).

## TODO
This repository is still a work in progress and the following are items that needs to be finished:
* Move the detection workload to a separate task and use mutexes between UI, camera and detection.
* Try to determine why the SPI only works in mode 3 (Different from original board which uses mode 2)
* Retrain yolo v11 for 240x240 (For the fastest option)
* Capture a 640x640 image in jpeg and convert back to RGR565 (For the more accurate version)
* Port [EfficientDet](https://github.com/google/automl/tree/master/efficientdet) networks into ESP-DL
* Train EfficientDet D0 for 240x240
* Compile some benchmarks
