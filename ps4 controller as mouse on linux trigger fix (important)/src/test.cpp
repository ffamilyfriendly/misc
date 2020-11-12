#include "header.h"

int main() {

    ps4ctrl::Ds4 *c = new ps4ctrl::Ds4();
    system("clear");

    int timesRan = 0;
    for(;;) {
        ps4ctrl::input i = c->listen();

        if(i.button_x) system("clear");
        if(i.button_triangle) std::cout << "cool!";
        if(i.r_stick_y < -0.5) std::cout << std::endl;

        if(timesRan > 0 && timesRan % 1 == 0 && (1000/5) * timesRan % 50000 == 0) printf("%f seconds until game closes!\n",(float)(1000 * 5 - timesRan * 5) / 1000);
        if(timesRan > 0 && i.tpad) {
            timesRan++;
            if(timesRan > ((1000/5) * 5)) break;
        } else if(!i.tpad) timesRan = 1;
    }
    printf("exiting...\n");
    delete c;

    return 0;
}