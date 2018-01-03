#include <jsonCppWrap.hpp>

using namespace std;

namespace jsonCppWrap{

	JsonCppWrapException::JsonCppWrapException(int errCode) : \
				   errorCode(errCode), errorMessage("None"){ } 

	JsonCppWrapException::JsonCppWrapException(string errMsg) : \
				   errorCode(-1), errorMessage(errMsg){ }

	JsonCppWrapException::JsonCppWrapException(string* errMsg) : \
				   errorCode(-1), errorMessage(*errMsg){ }

	JsonCppWrapException::JsonCppWrapException(char* errMsg) : \
				   errorCode(-1), errorMessage(errMsg){ } 

	JsonCppWrapException::JsonCppWrapException(const char* errMsg) : \
				   errorCode(-1), errorMessage(errMsg){ }

	JsonCppWrapException::JsonCppWrapException(int errCode, char* errMsg) : \
				   errorCode(errCode), errorMessage(errMsg){ }

	JsonCppWrapException::JsonCppWrapException(int errCode, const char* errMsg) : \
				   errorCode(errCode), errorMessage(errMsg){ }

	JsonCppWrapException::JsonCppWrapException(int errCode, string errMsg) : \
				   errorCode(errCode), errorMessage(errMsg){ }

	JsonCppWrapException::JsonCppWrapException(int errCode, string* errMsg) : \
				   errorCode(errCode), errorMessage(*errMsg){ }

	string JsonCppWrapException::what(void) const noexcept{
			return errorMessage;
	}

JsonCppWrap::JsonCppWrap(string* jsonFile)  : JsonCppWrap(jsonFile->c_str()){

        }

JsonCppWrap::JsonCppWrap(string& jsonFile)  : JsonCppWrap(jsonFile.c_str()){

        }

	JsonCppWrap::JsonCppWrap(const char* jsonFile)  : jsonFilePath(jsonFile){
		int status=::parseJsonConfig(jsonFile, &nodeRoot, &fullIndexesList);
		if(status<=0){
			throw JsonCppWrapException(status, "Parse status: NOK Err");
		}
	}

	node* JsonCppWrap::getElementValueByPath(const char *elUrl) const noexcept{
		pathIndex* element;
		element = ::getElementValueByString(fullIndexesList, elUrl);
		if(element==nullptr){
			return nullptr;
		}

		return element->Node;
	}

	node* JsonCppWrap::getElementValueByPath(string* elUrl) const noexcept{
		return getElementValueByPath(elUrl->c_str());
	}

	node* JsonCppWrap::getElementValueByPath(string& elUrl) const noexcept{
		return getElementValueByPath(elUrl.c_str());
	}
	
	node* JsonCppWrap::getElementValueByPathExc(string* elUrl) const {
		return getElementValueByPathExc(elUrl->c_str());
	}
	
	node* JsonCppWrap::getElementValueByPathExc(string& elUrl) const {
		return getElementValueByPathExc(elUrl.c_str());
	}

	node* JsonCppWrap::getElementValueByPathExc(const char *elUrl) const {
		pathIndex* element;
		element = ::getElementValueByString(fullIndexesList, elUrl);
		if(element==nullptr){
			throw JsonCppWrapException("No matching.");
		}

		return element->Node;
	}

	node* JsonCppWrap::getArrayElements(node* arrayRootElement) const noexcept{
		return ::getArrayElements(arrayRootElement);
	}

	node* JsonCppWrap::getArrayOfObjsElements(node* arrayRootElement) const noexcept{
		return ::getArrayOfObjsElements(arrayRootElement);
	}

	node* JsonCppWrap::getElementFromObj(node* object) const noexcept{
		return ::getElementFromObj(object);
	}

	bool JsonCppWrap::isScalar(node* toCheck) const noexcept{
		return ::isScalar(toCheck);
	}

	bool JsonCppWrap::checkPath(char const *elUrl) const noexcept{
		pathIndex* element;
		element = ::getElementValueByString(fullIndexesList, elUrl);
		if(element==nullptr){
			return false;
		}
		return true;
	}

	bool JsonCppWrap::checkPath(string* elUrl) const noexcept{
		return checkPath(elUrl->c_str());
	}

	bool JsonCppWrap::checkPath(string& elUrl) const noexcept{
		return checkPath(elUrl.c_str());
	}

	bool JsonCppWrap::checkPathList(vector<string>& urlList) const noexcept{
		for (auto iter : urlList ){
			if(!checkPath(iter.c_str())){
				return false;
			}
		}
		return true;
	}

	type JsonCppWrap::getType(node* nodeElement) const noexcept{
		return nodeElement->nodeType;
	}

	void JsonCppWrap::printNodeValueExc(node* nodeElement) const{

                                switch(nodeElement->nodeType){
                                        case ROOT_T:
						cout << "Root Node." << endl;
                                        break;
                                        case EMPTY_T:
						cout << "Empty Node." << endl;
                                        break;
                                        case  NUMERIC_T:
                                                cout << "Numeric Node: " << (nodeElement->data).number << endl;
                                        break;
                                        case  TEXT_T:
                                                cout << "Text Node: " << (nodeElement->data).string << endl;
                                        break;
                                        case  BOOL_T:
                                                cout << "Bool node: " << (nodeElement->data).boolean << endl;
                                        break;
                                        case  MATRIX_T:
						cout << "Matrix Node." << endl;
                                        break;
                                        case  OBJ_T:
						cout << "Object Node." << endl;
                                        break;
                                        case UNASSIGNED_T:
						throw JsonCppWrapException(nodeElement->nodeType,\
								           "Unassigned Node Detected.");
                                }
	}

	int JsonCppWrap::printNodeValue(node* nodeElement) const{

                                switch(nodeElement->nodeType){
                                        case ROOT_T:
						cout << "Root Node." << endl;
                                        break;
                                        case EMPTY_T:
						cout << "Empty Node." << endl;
                                        break;
                                        case  NUMERIC_T:
                                                cout << "Numeric Node: " << (nodeElement->data).number << endl;
                                        break;
                                        case  TEXT_T:
                                                cout << "Text Node: " << (nodeElement->data).string << endl;
                                        break;
                                        case  BOOL_T:
                                                cout << "Bool node: " << (nodeElement->data).boolean << endl;
                                        break;
                                        case  MATRIX_T:
						cout << "Matrix Node." << endl;
                                        break;
                                        case  OBJ_T:
						cout << "Object Node." << endl;
                                        break;
                                        case UNASSIGNED_T:
						return 1;
                                }
		return 0;
	}

} // End Namespace
