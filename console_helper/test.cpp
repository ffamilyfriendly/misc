#include "header.h"

int strToNumber(string input) {
    return atoi(input.c_str());
}

string numberToString(int input) {
    return to_string(input);
}

//void runTests(T func, string answerToString , vector<string> inputs, vector<T> answers) {

void runTests(string func(string), string answerToString(string) , vector<string> inputs, vector<string> answers) {}

int main() {
    vector<vector<string>> test;
    console::Console *c = new console::Console();
    test.push_back({"index","key","value"});
    for(int i = 0; i < 20; i++) {
        test.push_back({"#"+to_string(i), "user_" + to_string(rand()),"$pass" + to_string(rand())});
    }
    
    c->log(c->styleReset( c->colourBG(c->colour("[",console::colour::black),console::colour::red) + c->colour("20",console::colour::yellow,console::mode::bold) + c->colourBG(c->colour("] rows needs checking: ",console::colour::black),console::colour::red) ));
    c->box(test);

    vector<string> inputs = {"0","123","923","this is not a number","this input is wrong","too many inputs"};
    vector<int> answers = {0,123,923,0,9};
    
    c->runTests<int>(strToNumber,numberToString,inputs,answers);
}
