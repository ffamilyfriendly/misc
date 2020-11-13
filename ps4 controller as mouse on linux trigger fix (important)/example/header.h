#include <iostream>
#include <libusb-1.0/libusb.h>

namespace ps4ctrl {

    enum dsAudio {
        headset_mic = 0x7B,
        headset = 0x3B,
        nothing = 0x1B,
        bluetooth = 0x08,
        unknown = 0x05
    };

    struct input {
        float l2; // left bottom trigger value as float. Values range from 0 to 1 with 1 being the button fullt pressed
        float r2; // same as above but right trigger
        bool l1; // left top trigger. Either on or off (duh)
        bool r1; // same as above but right trigger

        // nominaly the values when no manipulation is done to the stick would be exactly 0
        // but there is often a slight offset that is due to dust and such making the input
        // a bit out of wack. Can likely be fixed by setting a threshold.
        // both values range from -1 to 1 where the resting input value is 0.
        // in the case of "x" -1 would mean the stick is being pushed left
        float r_stick_x; //the right stick x direction
        float r_stick_y; //right stick y direction
        float l_stick_x; //left stick x direction
        float l_stick_y; //left stick y direction
        bool button_triangle; // if triangle button is pressed
        bool button_circle; // circle
        bool button_x; // x
        bool button_square; // square
        bool button_l3; // if the left stick is being pushed down
        bool button_r3; // if the right stick is being pushed down
        bool button_options; // if options button is clicked
        bool button_share; // if share button is clicked
        bool button_ps; // if ps logo button is clicked
        dsAudio audio; // the state of attatched audio attatchements
        bool tpad; // if the trackpad is being pushed down
        bool tpad_finger1_down; // if finger1 is touching the trackpad;
        bool tpad_finger2_down; // if finger2 is touching the trackpad;
        std::pair<int,int> tpad_finger1_pos; // finger1 pos pair<int,int>. Represents x & y;
        std::pair<int,int> tpad_finger2_pos; // same as above but finger2
        std::pair<int,int> prev_tpad_finger1_pos; // previous finger1 pos pair<int,int>. Represents x & y;
        std::pair<int,int> prev_tpad_finger2_pos; // same as above but finger2
    };

    struct RGB {
        short r = 0;
        short g = 0;
        short b = 0;
    };

    class Ds4 {
        public:
            Ds4(); //constructor. Calls private function "tryReconnect"
            ~Ds4(); //destructor. Gives back drivers to kernel and exits libusb
            bool isInitialized(); //gets value of private bool "initialized"
            bool reconnect = true; //wheter or not a previusly connected controller shall attempt to reconnect
            RGB colour; //ds4 led colour thingie
            int rumbleLeft;
            int rumbleRight;
            int flashOn = 0;
            int flashOff = 0;
            input listen(); //listens for input and returns it as a struct input
            void rumble(int ms, int intensity);
            void sendUpdate(); //updates controller with values
        private:
            bool initialized = false; //wheter or not a handle has been created succesfully
            void tryReconnect(bool firstinit = false); //function that connects the controller
            libusb_device_handle *HANDLE; //the handle of the controller
    };
}