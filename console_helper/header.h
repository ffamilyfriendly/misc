#include <string>
#include <iostream>
#include <vector>
#include <chrono>

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

            struct timeLog {
                string name;
                int goal;
                chrono::_V2::steady_clock::time_point t1;
                console::Console *c;
                void end();
                void startLog();
                timeLog(string name, Console *c, int goal = 10) {
                    this->t1 = std::chrono::steady_clock::now();
                    this->name = name;
                    this->goal = goal;
                    this->c = c;

                    this->startLog();
                }
            };

            void log(string msg);
            void box(vector<vector<string>> contents);
            string colourBG(string input,console::colour bg);
            string colour(string input,console::colour c, console::mode m = console::mode::none);
            string styleReset(string input);
            timeLog timedLog(string name, int goat = 10) {
                timeLog t(name,this, goat);
                return t;
            }

            template <typename T>
            void runTests(T func(string), string answerToString(T), vector<string> inputs, vector<T> answers) {
                int succeededTests = answers.size();
                for(int i = 0; i < inputs.size(); i++) {
                    T answer = func(inputs[i]);
                    
                    
                    string resStr = " PASSED ";
                    console::colour resCol = console::colour::green;

                    if(answer != answers[i]) {
                        resStr = " FAILED ";
                        resCol = console::colour::red;
                        succeededTests--;
                    }

                    if(i > answers.size()-1) {
                        resStr = " NO ANSWER DEFINED ";
                        resCol = console::colour::yellow;        
                    }

                    this->log(this->styleReset( this->colourBG( this->colour(resStr, console::colour::black),resCol)) + " " + inputs[i] + "\nexpected \"" + answerToString(answers[i]) + "\" got \"" + answerToString(answer) + "\"\n");
                }
                this->log(this->styleReset(this->colourBG(this->colour(" PASSED ",console::colour::black), console::colour::green)) + " " + to_string(succeededTests) + " " + this->styleReset(this->colourBG(this->colour(" FAILED ",console::colour::white), console::colour::red)) + " " + to_string(answers.size() - succeededTests));
            };
        private:
    };

}