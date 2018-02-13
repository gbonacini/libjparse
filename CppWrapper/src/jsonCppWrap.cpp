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

	string JsonCppWrapException::what(void) const noexcept(true){
			return errorMessage;
	}

    JsonCppWrap::JsonCppWrap(const string * const jsonFile)  : JsonCppWrap(jsonFile->c_str()){}

    JsonCppWrap::JsonCppWrap(const string& jsonFile)  : JsonCppWrap(jsonFile.c_str()){}

	JsonCppWrap::JsonCppWrap(const char * const jsonFile)  : jsonFilePath(jsonFile){
		int status=::parseJsonConfig(jsonFile, &nodeRoot, &fullIndexesList);
		if(status<=0){
			throw JsonCppWrapException(status, "Parse status: NOK Err");
		}
	}

	const node* JsonCppWrap::getElementValueByPath(const char * const elUrl) const noexcept(true){
		pathIndex* element;
		element = ::getElementValueByString(fullIndexesList, elUrl);
		if(element==nullptr){
			return nullptr;
		}

		return element->Node;
	}

	const node* JsonCppWrap::getElementValueByPath(const string * const elUrl) const noexcept(true){
		return getElementValueByPath(elUrl->c_str());
	}

	const node* JsonCppWrap::getElementValueByPath(const string& elUrl) const noexcept(true){
		return getElementValueByPath(elUrl.c_str());
	}
	
	const node* JsonCppWrap::getElementValueByPathExc(const string * const elUrl) const noexcept(false){
		return getElementValueByPathExc(elUrl->c_str());
	}
	
	const node* JsonCppWrap::getElementValueByPathExc(const string& elUrl) const noexcept(false){
		return getElementValueByPathExc(elUrl.c_str());
	}

	const node* JsonCppWrap::getElementValueByPathExc(const char * const elUrl) const noexcept(false){
		pathIndex* element;
		element = ::getElementValueByString(fullIndexesList, elUrl);
		if(element==nullptr){
			throw JsonCppWrapException("No matching.");
		}

		return element->Node;
	}

	const node* JsonCppWrap::getArrayElements(const node * const arrayRootElement) const noexcept(true){
		return ::getArrayElements(arrayRootElement);
	}

	const node* JsonCppWrap::getArrayOfObjsElements(const node * const arrayRootElement) const noexcept(true){
		return ::getArrayOfObjsElements(arrayRootElement);
	}

	const node* JsonCppWrap::getElementFromObj(const node * const object) const noexcept(true){
		return ::getElementFromObj(object);
	}

	bool JsonCppWrap::isScalar(const node * const toCheck) const noexcept(true){
		return ::isScalar(toCheck);
	}

	bool JsonCppWrap::checkPath(char const * const elUrl) const noexcept(true){
		pathIndex* element;
		element = ::getElementValueByString(fullIndexesList, elUrl);
		if(element==nullptr){
			return false;
		}
		return true;
	}

	bool JsonCppWrap::checkPath(const string * const elUrl) const noexcept(true){
		return checkPath(elUrl->c_str());
	}

	bool JsonCppWrap::checkPath(const string& elUrl) const noexcept(true){
		return checkPath(elUrl.c_str());
	}

	bool JsonCppWrap::checkPathList(const vector<string>& urlList) const noexcept(true){
		for (auto iter : urlList ){
			if(!checkPath(iter.c_str())){
				return false;
			}
		}
		return true;
	}

	bool JsonCppWrap::checkPathMapValues(const map<const string, const string>& urlMap) const noexcept(true){
		for (auto iter : urlMap ){
			if(!checkPath(iter.second.c_str())){
				return false;
			}
		}
		return true;
	}

	type JsonCppWrap::getType(const node * const nodeElement) const noexcept(true){
		return nodeElement->nodeType;
	}

	void JsonCppWrap::printNodeValueExc(const node * const nodeElement) const{

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

	int JsonCppWrap::printNodeValue(const node * const nodeElement) const noexcept(false){

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
