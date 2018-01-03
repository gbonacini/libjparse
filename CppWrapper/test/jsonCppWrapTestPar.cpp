#include <jsonCppWrap.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace jsonCppWrap;

int main(int argc, char** argv){
	vector<string> urlList;
	cerr << "Start test..." << endl;

	if(argc != 3){
		cerr << "Syntax: " << endl;
		cerr <<  argv[0] << "<jsonfile> <path>" << endl;
		exit(1);
	}

	try{
		string jsonFile = argv[1];
		JsonCppWrap test(&jsonFile);

		test.printNodeValue(test.getElementValueByPath(argv[2]));

		cerr << "Exist: " << test.checkPath(argv[2]) << endl;

		urlList.push_back(argv[2]);

		cerr << "Exist Vector: " << test.checkPathList(urlList) << endl;

	}catch(JsonCppWrapException &e){
		cerr << "Exception: " << e.what() << endl;
		return 1;
	}

	cerr << "... End test." << endl;

	exit(0);

}
