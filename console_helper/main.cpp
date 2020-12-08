#include "header.h"

string repeat(string character, int times) {
    string content;
    for(int i = 0; i < times; i++) content += character;
    return content;
}

void console::Console::timeLog::startLog() {
    this->c->log(" ⏲️ | Timer " + this->c->styleReset(this->c->colour(this->name,console::colour::magenta, console::mode::underline)) + " started!");
}

void console::Console::timeLog::end() {
    auto t2 = std::chrono::steady_clock::now();
    auto difference = chrono::duration_cast<chrono::milliseconds>(t2 - this->t1);
    string diff = "Difference: ";

    int dDif = this->goal - difference.count();

    if(dDif < this->goal) {
        diff += c->styleReset(c->colour(to_string(dDif) + "ms", console::colour::red));
    } else {
        diff += c->styleReset(c->colour(to_string(dDif) + "ms", console::colour::green));
    }

    this->c->log("\n ⏲️ | Timer " + this->c->styleReset(this->c->colour(this->name,console::colour::magenta, console::mode::underline)) + " ended!\n ⏲ | Time: " + to_string(difference.count()) + "ms Goal: " + to_string(this->goal) + "ms " + diff + "\n" );
}

void console::Console::log(string msg) {
    cout << msg << endl;
}

string console::Console::styleReset(string input) {
    return input + "\033[0m";
}

string console::Console::colourBG(string input, console::colour bg) {
    string clr = "\033[0;";

    switch (bg)
    {
        case console::colour::black:
            clr+="40";
        break;
        case console::colour::red:
            clr+="41";
        break;
        case console::colour::green:
            clr+="42";
        break;
        case console::colour::yellow:
            clr+="43";
        break;
        case console::colour::blue:
            clr+="44";
        break;
        case console::colour::magenta:
            clr+="45";
        break;
        case console::colour::cyan:
            clr+="46";
        break;
        case console::colour::white:
            clr+="47";
        break;
    }

    clr+="m";
    return clr + input;
}

string console::Console::colour(string input,console::colour c, console::mode m) {
    string clr = "\033[";

    switch(m) {
        case console::mode::bold:
            clr+="1";
        break;
        case console::mode::underline:
            clr+="4";
        break;
        case console::mode::inverted:
            clr+="7";
        break;
        case console::mode::none:
            clr+="99";
        break;
    }

    clr+=";";

    switch (c)
    {
        case console::colour::black:
            clr+="30";
        break;
        case console::colour::red:
            clr+="31";
        break;
        case console::colour::green:
            clr+="32";
        break;
        case console::colour::yellow:
            clr+="33";
        break;
        case console::colour::blue:
            clr+="34";
        break;
        case console::colour::magenta:
            clr+="35";
        break;
        case console::colour::cyan:
            clr+="36";
        break;
        case console::colour::white:
            clr+="37";
        break;
    }
    clr+="m";
    return clr + input;
};

void console::Console::box(vector<vector<string>> contents) {
    int longestCell[] = {0,0,0,0,0,0,0,0,0,0,0};
    int maxCells = 0;
    int longestCombined = 0;
    for(vector<string> row: contents) {
        int i = 0;
        for(string str: row) {
            if(i > maxCells) maxCells = i;
            if(row[i].length() > longestCell[i]) longestCell[i] = row[i].length();
            i++;
        }
    }

    cout << this->leftTopCorner;
    for(int i = 0; i < maxCells+1; i++) {
        longestCombined += longestCell[i];
        cout << repeat(this->topCorner,longestCell[i]) << (i == maxCells ? "" : this->topDiv);
    }
    cout << this->rightTopCorner << "\n";

    for(int i = 0; i < contents.size(); i++) {
        string bottomDivThing = this->leftDivider;
        for(int j = 0; j < maxCells+1; j++) {
            string thisTab = this->side + contents[i][j];
            cout << thisTab << repeat(" ",longestCell[j]+3-thisTab.size()) << (j == maxCells ? this->side : "");
            bottomDivThing += repeat(this->bottomCorner,longestCell[j]) + (j == maxCells ? "" : this->mid);
        }
        bottomDivThing += this->rightDivider;
        if(i != contents.size()-1) cout << "\n" << bottomDivThing;
        cout << "\n";
    }

    cout << this->leftBottomCorner;
    for(int i = 0; i < maxCells+1; i++) {
        longestCombined += longestCell[i];
        cout << repeat(this->bottomCorner,longestCell[i]) << (i == maxCells ? "" : this->bottomDiv);
    }
    cout << this->rightBottomCorner;
    cout << endl;
};