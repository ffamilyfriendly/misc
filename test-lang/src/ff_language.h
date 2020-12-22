#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

//ffs stands for family friendly script, nothing else
namespace ffs {

	class instrReturnType {
		public:
			bool succesfull = false;
			bool panic = false;
			string message = "";
	};

	extern int lineIndex;
	extern unordered_map<string,instrReturnType(*)(vector<string>)> instructionList;
	extern unordered_map<string,string> memory;
	void mountInstructions();
}