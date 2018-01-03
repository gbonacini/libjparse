#include <jsonCppWrap.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace jsonCppWrap;

int main(){
	vector<string> urlList;
	cerr << "Start test..." << endl;

	string jsonFile = "jsons/sample.null.json";
	JsonCppWrap test(&jsonFile);

	test.printNodeValue(test.getElementValueByPath("root.menu.footer"));
	test.printNodeValue(test.getElementValueByPath("root.menu.title"));
	test.printNodeValue(test.getElementValueByPath("root.menu.items.Object_Data_17.label"));

	cerr << "Exist: " << test.checkPath("root.menu.items.Object_Data_17.label") << endl;
	cerr << "Exist: " << test.checkPath("root.menu.items.xxx.label") << endl;

	urlList.push_back("root.menu.footer");
	urlList.push_back("root.menu.title");
	urlList.push_back("root.menu.items.Object_Data_17.label");

	cerr << "Exist Vector: " << test.checkPathList(urlList) << endl;

	urlList.push_back("root.menu.items.xxx.label");
	cerr << "Exist Vector: " << test.checkPathList(urlList) << endl;

	cerr << "... End test." << endl;

	return 0;

}
