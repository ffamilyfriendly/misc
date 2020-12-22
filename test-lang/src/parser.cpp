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

	for(int i = 0; i < data.length(); i++) {
		if(chunk.empty() && data[i] == ' ') continue;
		else if(data[i] == ' ') {
			args.push_back(chunk);
			chunk = "";
			continue;
		}

		chunk += data[i];
	}

	args.push_back(chunk);

	return args;
}

string insertVariables(string data) {
	for(int i = 0; i < data.length(); i++) {
		if(data[i] == FF_VARIABLECHAR) {
			size_t nSpace = data.substr(i).find(" ");
			if(nSpace == string::npos) nSpace = data.length();
			string varName = data.substr(i+1,nSpace);
			string revamped = data.substr(0,i) + ffs::memory[varName] + data.substr(nSpace);
			return insertVariables(revamped);
		}
	}

	return data;
}

int parseInstructions() {
	for(ffs::lineIndex = 0; ffs::lineIndex < instructions.size(); ffs::lineIndex++) {
		string l = insertVariables(instructions[ffs::lineIndex]);
		if(startsWith(l,FF_COMMENTCHAR)) continue;

		vector<string> args = argumentList(l);
		if(ffs::instructionList.find(args[0]) != ffs::instructionList.end()) {
			ffs::instrReturnType rt = ffs::instructionList[args[0]](args);
			if(rt.panic) {
				cout << "Error on line " << ffs::lineIndex << endl;
				cout << rt.message << "\nexecution halted!" << endl;
				return 1;
			}
		}
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