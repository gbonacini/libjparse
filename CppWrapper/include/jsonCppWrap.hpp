#ifndef __JSONCPPWRAP__
#define __JSONCPPWRAP__

#include <iostream>
#include <string>
#include <vector> 
#include <map> 

extern "C" {
#include <jsonParserTypes.h>
#include <parseJsonConfig.h>
}

namespace jsonCppWrap{

class JsonCppWrapException{
	public:
		JsonCppWrapException(int errCode);
		JsonCppWrapException(std::string errMsg);
		JsonCppWrapException(std::string* errMsg);
		JsonCppWrapException(char* errMsg);
		JsonCppWrapException(const char* errMsg);
		JsonCppWrapException(int errCode, char* errMsg);
		JsonCppWrapException(int errCode, const char* errMsg);
		JsonCppWrapException(int errCode, std::string errMsg);
		JsonCppWrapException(int errCode, std::string* errMsg);
		std::string what(void) const noexcept;
	private:
		int errorCode;
		std::string errorMessage;
};

class JsonCppWrap{
	public:
		explicit JsonCppWrap(std::string* jsonFile) ;
		explicit JsonCppWrap(std::string& jsonFile) ;
		explicit JsonCppWrap(const char* jsonFile) ;
		node*    getElementValueByPath(const char* elUrl)                const noexcept;
		node*    getElementValueByPath(std::string* elUrl)               const noexcept;
		node*    getElementValueByPath(std::string& elUrl)               const noexcept;
		node*    getElementValueByPathExc(const char* elUrl)             const;
		node*    getElementValueByPathExc(std::string* elUrl)            const;
		node*    getElementValueByPathExc(std::string& elUrl)            const;
		type     getType(node* nodeElement)                              const noexcept;
		node*    getArrayElements(node* arrayRootElement)                const noexcept;
		node*    getArrayOfObjsElements(node* arrayRootElement)          const noexcept;
		node*    getElementFromObj(node* object)                         const noexcept;
		bool     checkPath(std::string* elUrl)                           const noexcept;
		bool     checkPath(std::string& elUrl)                           const noexcept;
		bool     checkPath(char const* elUrl)                            const noexcept;
		bool     checkPathList(const std::vector<std::string>& urlList)  const noexcept;
		bool     checkPathMapValues(const std::map<std::string,
		                                           std::string>& urlMap) const noexcept;
		bool     isScalar(node* toCheck)                                 const noexcept;
		void     printNodeValueExc(node* nodeElement)                    const;
		int      printNodeValue(node* nodeElement)                       const;
	private:
		std::string    jsonFilePath;
		pathIndex*     fullIndexesList;
		node           nodeRoot;
};


}

#endif
