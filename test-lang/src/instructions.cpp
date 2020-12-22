#include "ff_language.h"
#include "ff_defenitions.h"

unordered_map<string,string> ffs::memory;

string failMessage(string cmd, string expected, string got) {
	return "[" + cmd + "]" + " expected " + expected + " got " + got;
}

string removeChar(string data, char toRemove) {
	string out = "";

	for(int i = 0; i < data.length(); i++) {
		if(data[i] != toRemove) out += data[i];
	}

	return out;
}

string doThing(vector<string> args, int _i = 0) {
	string leftHand = args[_i];
	int mode = -1;

	for(int i = _i; i < args.size(); i++) {
		if(args[i] == "+") {mode = 1; continue;}
		if(args[i] == "/") {mode = 2; continue;}
		if(args[i] == "*") {mode = 3; continue;}

		switch (mode)
		{
			case 1: {
				if(leftHand[0] == FF_STRINGCHAR && leftHand[leftHand.length() - 1] == FF_STRINGCHAR)
					leftHand = FF_STRINGCHAR + removeChar(leftHand,FF_STRINGCHAR) + removeChar(args[i],FF_STRINGCHAR) + FF_STRINGCHAR;
				else
					leftHand =  to_string(atof(leftHand.c_str()) + atof(args[i].c_str()));
				} 
			break;

			case 2:
				leftHand =  to_string(atof(leftHand.c_str()) / atof(args[i].c_str()));
			break;

			case 3:
				leftHand =  to_string(atof(leftHand.c_str()) * atof(args[i].c_str()));
			break;

		}
	}

	return leftHand;
}

// MAIN SCRIPT FUNCTIONS

ffs::instrReturnType _goto(vector<string> args) {
	ffs::instrReturnType rt;

	if(args.size() < 2) {
		rt.panic = true;
		rt.message = failMessage("goto","number","nothing");
		return rt;
	}

	ffs::lineIndex = atoi(args[1].c_str()) - 1;

	return rt;
}

ffs::instrReturnType _print(vector<string> args) {
	ffs::instrReturnType rt;

	if(args.size() < 2) {
		rt.panic = true;
		rt.message = failMessage("print","string","nothing");
		return rt;
	}

	cout << removeChar(doThing(args,1), FF_STRINGCHAR) << endl;

	return rt;
}

ffs::instrReturnType _var(vector<string> args) {
	ffs::instrReturnType rt;

	if(args.size() < 3) {
		rt.panic = true;
		rt.message = failMessage("var","string string","nothing");
		return rt;
	}

	ffs::memory[args[1]] = doThing(args,2);

	return rt;
}

void ffs::mountInstructions() {
	ffs::instructionList["goto"] = _goto;
	ffs::instructionList["print"] = _print;
	ffs::instructionList["var"] = _var;
}