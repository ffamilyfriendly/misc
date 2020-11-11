#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <libusb-1.0/libusb.h>

namespace ps4ctrl {
    struct input {
        float l2;
        float r2;
        float r_stick_x;
        float r_stick_y;
        float l_stick_x;
        float l_stick_y;
        bool button_triangle;
        bool button_circle;
        bool button_x;
        bool button_square;
    };
}