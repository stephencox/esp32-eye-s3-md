idf_component_register(
    SRCS "yolo.c" "ui.c" "main.c" "lcd.c" "camera.c" "camera.cpp" "ui.c" "yolo.c" 
    INCLUDE_DIRS "include" 
    REQUIRES coco_detect
)