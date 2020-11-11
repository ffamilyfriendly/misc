#include "header.h"

int main() {
    int r;
    r = libusb_init(NULL);

    //LIBUSB_LOG_LEVEL_DEBUG | LIBUSB_LOG_LEVEL_INFO | LIBUSB_LOG_LEVEL_WARNING | LIBUSB_LOG_LEVEL_ERROR | LIBUSB_LOG_LEVEL_NONE 
    libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);

    if(r < 0) {
        printf("error! %s\n",libusb_strerror((enum libusb_error)r));
        return r;
    }
    unsigned pid, vid;

    vid = 0x054c;
    pid = 0x09cc;

    libusb_device_handle *HANDLE = libusb_open_device_with_vid_pid(NULL,vid,pid);
    if(!HANDLE) {
        printf("Device with PID %d and VID %d not detected! Is it plugged in?\n",pid,vid);
        return 1;
    } else printf("Device detected and handle aqquired! Claiming interface...\n");

    r = libusb_detach_kernel_driver(HANDLE,3);
        if(r < 0) {
        printf("could not detach kernel driver! continuing anyways... %s\n",libusb_strerror((enum libusb_error)r));
    }

    r = libusb_claim_interface(HANDLE,3);
    if(r < 0) {
        printf("could not claim interface! %s\n",libusb_strerror((enum libusb_error)r));
        return r;
    }

    printf("Interface claimed!\n");

    unsigned char bytes[64];
    int len = 0;
    bool running = true;
    while(running) {
        ps4ctrl::input i;

        r = libusb_interrupt_transfer(HANDLE,0x84,bytes,sizeof(bytes),&len,1000);
        if(r < 0) {
            printf("could not read input! %s\n",libusb_strerror((enum libusb_error)r));
            break;
        }
        i.l_stick_x = (float)(bytes[1]) / (0xFF/2) - 1;
        i.l_stick_x = (float)(bytes[2]) / (0xFF/2) - 1;
        i.r_stick_x = (float)(bytes[3]) / (0xFF/2) - 1;
        i.r_stick_y = (float)(bytes[4]) / (0xFF/2) - 1;
        i.button_x = 0 < (bytes[5]&0x20);
        i.button_square = 0 < (bytes[5]&0x10);
        i.button_triangle = 0 < (bytes[5]&0x80);
        i.button_circle = 0 < (bytes[5]&0x40);
        i.l2 = (float)bytes[8] / 0xFF;
        i.r2 = (float)bytes[9] / 0xFF;
        printf("x: %d sq: %d tr: %d ci: %d HEX: %02x\n",i.button_x, i.button_square, i.button_triangle, i.button_circle,bytes[5]);


        //exit dev mode
        if(i.l2 == 1 && i.r2 == 1) break;
    }

    libusb_release_interface(HANDLE,3);
    r = libusb_attach_kernel_driver(HANDLE,3);
    if(r < 0) {
        printf("could not re-attach kernel driver! continuing anyways... %s\n",libusb_strerror((enum libusb_error)r));
    }

    libusb_close(HANDLE);
    libusb_exit(NULL);
    return r;
}