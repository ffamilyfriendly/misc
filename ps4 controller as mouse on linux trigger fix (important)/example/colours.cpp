#include "header.h"

int main() {

    ps4ctrl::Ds4 *c = new ps4ctrl::Ds4();
    c->colour.r = 0;
    c->colour.g = 0;
    c->colour.b = 0;
    c->sendUpdate();
    system("clear");

    enum currentlyEditing {
        r = 1,
        g = 2,
        b = 3
    };

    currentlyEditing _c = currentlyEditing::r;


    for(;;) {
        ps4ctrl::input i = c->listen();

        if(i.button_square) _c = currentlyEditing::r;
        if(i.button_x) _c = currentlyEditing::g;
        if(i.button_circle) _c = currentlyEditing::b;
        if(i.button_square || i.button_x || i.button_circle) c->rumble(150,255/4);

        if(i.r2 > 0.1 && i.r1) {
            switch (_c)
            {
                case currentlyEditing::r:
                    c->colour.r = i.r2*255;
                break;
                case currentlyEditing::g:
                    c->colour.g = i.r2*255;
                break;
                case currentlyEditing::b:
                    c->colour.b = i.r2*255;
                break;
            }
            c->sendUpdate();
        }

        if(i.button_triangle) {
            switch (_c)
            {
                case currentlyEditing::r:
                    c->colour.r = 0;
                break;
                case currentlyEditing::g:
                    c->colour.g = 0;
                break;
                case currentlyEditing::b:
                    c->colour.b = 0;
                break;
            }
            c->sendUpdate();
        }

        if(i.button_ps) break;
    }
    printf("exiting...\n");
    delete c;

    return 0;
}