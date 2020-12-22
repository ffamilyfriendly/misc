#include "ff_language.h"
#include "ff_defenitions.h"

unordered_map<string,string> ffs::memory;

string failMessage(string cmd, string expected, string got) {
	return "[" + cmd + "]" + " expected " + expected + " got " + got;
}

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

	cout << args[1] << endl;

	return rt;
}

ffs::instrReturnType _var(vector<string> args) {
	ffs::instrReturnType rt;

	if(args.size() < 3) {
		rt.panic = true;
		rt.message = failMessage("var","string string","nothing");
		return rt;
	}

	ffs::memory[args[1]] = args[2];

	return rt;
}

void ffs::mountInstructions() {
	ffs::instructionList["goto"] = _goto;
	ffs::instructionList["print"] = _print;
	ffs::instructionList["var"] = _var;
}