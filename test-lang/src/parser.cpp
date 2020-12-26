#include "ff_language.h"
#include "ff_defenitions.h"

vector<string> instructions;

int ffs::lineIndex = 0;
unordered_map<string,ffs::instrReturnType(*)(vector<string>)> ffs::instructionList;

bool startsWith(string data, string target) {
	for(int i = 0; i < data.length(); i++) {
		if(data[i] == ' ' || data[i] == '	') continue;
		else return data.substr(i,target.length()) == target;
	}
	return false;
}

vector<string> argumentList(string data) {
	vector<string> args;
	string chunk = "";
	bool hasInitiatedString = false;

	for(int i = 0; i < data.length(); i++) {
		if(chunk.empty() && data[i] == ' ') continue;
		else if(data[i] == ' ' && !hasInitiatedString) {
			args.push_back(chunk);
			chunk = "";
			continue;
		}
		if(data[i] == FF_STRINGCHAR) hasInitiatedString = !hasInitiatedString;
		chunk += data[i];
	}

	args.push_back(chunk);

	return args;
}

bool evaluate(vector<string> args, int start) {
	int mode = 0;
	if(args[start+1] == ">") mode = 1;
	if(args[start+1] == "<") mode = 2;
	if(args[start+1] == "==") mode = 3;
	if(args[start+1] == "!=") mode = 4;

	float asNr1 = atof(args[start].c_str());
	float asNr2 = atof(args[start+2].c_str());

	switch(mode) {
		case 1:
			return asNr1 > asNr2;
		break;

		case 2:
			return asNr1 < asNr2;
		break;

		case 3:
			return args[start] == args[start+2];
		break;

		case 4:
			return args[start] != args[start+2];
		break;

		default:
			return false;
		break;
	}
}

string insertVariables(string data) {

	int len = data.length();
	string temp = data;

	for(int i = 0; i < len; i++) {
		if(temp[i] == FF_VARIABLECHAR) {
			size_t nSpace = temp.substr(i).find(" ");
			if(nSpace == string::npos) nSpace = temp.length() - i;
			string varName = temp.substr(i+1,nSpace-1);

			temp = temp.substr(0,i) + ffs::memory[varName] + temp.substr(i + nSpace);
			len = temp.length();
			i = 0;
		}
	}

	return temp;
}

bool fullfilledIf = true;

int parseInstructions() {
	for(ffs::lineIndex = 0; ffs::lineIndex < instructions.size(); ffs::lineIndex++) {
		string l = instructions[ffs::lineIndex];
		if(startsWith(l,FF_COMMENTCHAR)) continue;
		l = insertVariables(l);

		vector<string> args = argumentList(l);
		if(ffs::instructionList.find(args[0]) != ffs::instructionList.end() && fullfilledIf) {
			ffs::instrReturnType rt = ffs::instructionList[args[0]](args);
			if(rt.panic) {
				cout << "Error on line " << ffs::lineIndex << endl;
				cout << rt.message << "\nexecution halted!" << endl;
				return 1;
			}
		} else if(args[0] == "if") {
			fullfilledIf = evaluate(args,1);
		} else if(args[0] == "endif") fullfilledIf = true;
	}

	return 0;
}

int main(int argc, char *argv[]) {

	ffs::mountInstructions();

    if(argc < 2) {
        cout << "ERROR: please pass a path to the file to interpret." << endl;
        return 1;
    }

	fstream file;
	file.open(argv[1]);

	if(!file || !file.is_open()) {
		perror("ERROR: could not open file");
		return 1;
	}

	string line;
	while(getline(file,line))
		instructions.push_back(line);
	
	file.close();

	return parseInstructions();
}