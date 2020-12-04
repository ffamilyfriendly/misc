#include "header.h"

int main() {
    vector<vector<string>> test;
    console::Console *c = new console::Console();
    test.push_back({"index","key","value"});
    for(int i = 0; i < 20; i++) {
        test.push_back({"#"+to_string(i), "user_" + to_string(rand()),"$pass" + to_string(rand())});
    }
    
    c->log(c->styleReset( c->colourBG(c->colour("[",console::colour::black),console::colour::red) + c->colour("20",console::colour::yellow,console::mode::bold) + c->colourBG(c->colour("] rows needs checking: ",console::colour::black),console::colour::red) ));
    c->box(test);
}
