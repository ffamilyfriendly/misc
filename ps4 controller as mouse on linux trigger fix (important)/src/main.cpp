#include "header.h"

#define INTERFACE 3

bool getBit(unsigned char byte, int bitpos) {
    return (byte & (1 << bitpos)) != 0;
}

ps4ctrl::Ds4::Ds4() {
    libusb_init(NULL);
    this->tryReconnect(true);
}

bool ps4ctrl::Ds4::isInitialized() {
    return this->initialized;
}

std::pair<int,int> decodeTPAD(unsigned char byte1, unsigned char midbyte, unsigned char byte3) {
    std::pair <int,int> xy;
    int x = byte1 + ((midbyte & 0xF) * 255);
    int y = ((midbyte & 0xF0) >> 4) + (byte3 * 16);
    xy.first = x;
    xy.second = y;
    return xy;
}


void ps4ctrl::Ds4::tryReconnect(bool firstinit) {
    unsigned pid, pid2, vid;

    vid = 0x054c;
    pid = 0x05C4;
    pid2 = 0x09cc; 
    libusb_device_handle *HANDLE = libusb_open_device_with_vid_pid(NULL,vid,pid);
    if(!HANDLE) HANDLE = libusb_open_device_with_vid_pid(NULL,vid,pid2);
    if(!HANDLE) return;
    this->HANDLE = HANDLE;
    int r;

    r = libusb_detach_kernel_driver(this->HANDLE,INTERFACE);
        if(r < 0) {
        printf("could not detach kernel driver! continuing anyways... %s\n",libusb_strerror((enum libusb_error)r));
    }
    
    r = libusb_claim_interface(this->HANDLE,INTERFACE);
    if(r < 0) {
        printf("could not claim interface! %s\n",libusb_strerror((enum libusb_error)r));
        return;
    }

    this->initialized = true;
}

void ps4ctrl::Ds4::sendUpdate() {
    if(!this->initialized) return;
    int r;
    unsigned char _bytes[10];
    _bytes[0] = 5;
    _bytes[1] = 255;
    _bytes[3] = 4;
    _bytes[4] = this->rumbleLeft;
    _bytes[5] = this->rumbleRight;
    _bytes[6] = this->colour.r;
    _bytes[7] = this->colour.g;
    _bytes[8] = this->colour.b; 
    _bytes[9] = this->flashOn;
    _bytes[10] = this->flashOff;

    r = libusb_interrupt_transfer(this->HANDLE,0x03,_bytes,sizeof(_bytes),NULL,0);
    if(r < 0) {
        printf("could not send input! %s code: %d\n",libusb_strerror((enum libusb_error)r),r);
    }
}

ps4ctrl::input ps4ctrl::Ds4::listen() {
    //0x84
    //0x01
    int r;
    ps4ctrl::input i;
    if(!this->initialized) return i;
    unsigned char bytes[64];
    int len = 0;

    r = libusb_interrupt_transfer(this->HANDLE,0x84,bytes,sizeof(bytes),&len,0);
    if(r == -4) {
        //detatched
        libusb_close(this->HANDLE);
        this->initialized = false;
        while(!this->initialized && this->reconnect) {
            this->tryReconnect();
        }
        return i;
    }
    else if(r < 0) {
        printf("could not read input! %s code: %d\n",libusb_strerror((enum libusb_error)r),r);
        return i;
    }

    //bytes[1-4]
    i.l_stick_x = (float)(bytes[1]) / (0xFF/2) - 1;
    i.l_stick_x = (float)(bytes[2]) / (0xFF/2) - 1;
    i.r_stick_x = (float)(bytes[3]) / (0xFF/2) - 1;
    i.r_stick_y = (float)(bytes[4]) / (0xFF/2) - 1;
    
    //bytes[5]
    i.button_square = getBit(bytes[5],4);
    i.button_x = getBit(bytes[5],5);
    i.button_circle = getBit(bytes[5],6);
    i.button_triangle = getBit(bytes[5],7);

    //bytes[6]
    i.l1 = getBit(bytes[6],0);
    i.r1 = getBit(bytes[6],1);
    i.button_share = getBit(bytes[6],4);   
    i.button_options = getBit(bytes[6],5);
    i.button_l3 = getBit(bytes[6],6);
    i.button_r3 = getBit(bytes[6],7);

    //bytes[7]
    i.button_ps = getBit(bytes[7],0);
    i.tpad = getBit(bytes[7],1);

    //bytes[8-9]
    i.l2 = (float)bytes[8] / 0xFF;
    i.r2 = (float)bytes[9] / 0xFF; 

    i.audio = (ps4ctrl::dsAudio)bytes[30];

    //TPAD
    i.tpad_finger1_down = getBit(bytes[35],7);
    i.tpad_finger2_down = getBit(bytes[39],7);

    i.tpad_finger1_pos = decodeTPAD(bytes[36],bytes[37],bytes[38]); // Get curr pos finger 1
    i.tpad_finger2_pos = decodeTPAD(bytes[40],bytes[41],bytes[42]); // Get curr pos finger 2

    i.prev_tpad_finger1_pos = decodeTPAD(bytes[45],bytes[46],bytes[47]); // Get prev pos finger 1
    i.prev_tpad_finger2_pos = decodeTPAD(bytes[49],bytes[50],bytes[51]); // Get prev pos finger 2

    return i;
}

ps4ctrl::Ds4::~Ds4() {
    libusb_release_interface(this->HANDLE,INTERFACE);
    libusb_attach_kernel_driver(this->HANDLE,INTERFACE);
    libusb_close(this->HANDLE);
    libusb_exit(NULL);
}