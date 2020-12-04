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
        private:
    };
}