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
		explicit           JsonCppWrap(const std::string* jsonFile) ;
		explicit           JsonCppWrap(const std::string& jsonFile) ;
		explicit           JsonCppWrap(const char * const jsonFile) ;
		const node*        getElementValueByPath(const char* elUrl)                    const noexcept(true);
		const node*        getElementValueByPath(const std::string * const elUrl)      const noexcept(true);
		const node*        getElementValueByPath(const std::string& elUrl)             const noexcept(true);
		const node*        getElementValueByPathExc(const char * const elUrl)          const noexcept(false);
		const node*        getElementValueByPathExc(const std::string * const elUrl)   const noexcept(false);
		const node*        getElementValueByPathExc(const std::string& elUrl)          const noexcept(false);
		const node*        getArrayElements(const node * const arrayRootElement)       const noexcept(true);
		const node*        getArrayOfObjsElements(const node * const arrayRootElement) const noexcept(true);
		const node*        getElementFromObj(const node * const object)                const noexcept(true);
		type               getType(const node * const nodeElement)                     const noexcept(true);
		bool               checkPath(const std::string * const elUrl)                  const noexcept(true);
		bool               checkPath(const std::string& elUrl)                         const noexcept(true);
		bool               checkPath(char const * const elUrl)                         const noexcept(true);
		bool               checkPathList(const std::vector<std::string>& urlList)      const noexcept(true);
		bool               checkPathMapValues(const std::map<std::string,
		                                      std::string>& urlMap)                    const noexcept(true);
		bool               isScalar(const node * const toCheck)                        const noexcept(true);
		void               printNodeValueExc(const node * const nodeElement)           const noexcept(false);
		int                printNodeValue(const node * const nodeElement)              const noexcept(false);
	private:
		std::string        jsonFilePath;
		pathIndex*         fullIndexesList;
		node               nodeRoot;
};


}

#endif
