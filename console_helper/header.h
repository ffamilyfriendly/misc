#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace console {
    enum colour {black,red,green,yellow,blue,magenta,cyan,white};
    enum mode {bold,underline,inverted,none};

    class Console {
        public:
            string leftTopCorner = "┌";
            string topCorner = "─";
            string rightTopCorner = "┐";
            string leftBottomCorner = "└";
            string bottomCorner = "─";
            string rightBottomCorner = "┘";
            string leftDivider = "├";
            string rightDivider = "┤";
            string side = "│";
            string mid = "┼";
            string topDiv = "┬";
            string bottomDiv = "┴";

            void log(string msg);
            void box(vector<vector<string>> contents);
            string colourBG(string input,console::colour bg);
            string colour(string input,console::colour c, console::mode m = console::mode::none);
            string styleReset(string input);

            template <typename T>
            void runTests(T func(string), string answerToString(T), vector<string> inputs, vector<T> answers) {
                for(int i = 0; i < inputs.size(); i++) {
                    T answer = func(inputs[i]);
                    
                    string resStr = " PASSED ";
                    console::colour resCol = console::colour::green;

                    if(answer != answers[i]) {
                        resStr = " FAILED ";
                        resCol = console::colour::red;
                    }

                    if(i > answers.size()-1) {
                        resStr = " NO ANSWER DEFINED ";
                        resCol = console::colour::yellow;        
                    }

                    this->log(this->styleReset( this->colourBG( this->colour(resStr, console::colour::black),resCol)) + " " + inputs[i] + "\nexpected \"" + answerToString(answers[i]) + "\" got \"" + answerToString(answer) + "\"\n");
                }
            };
        private:
    };

}