#include "IniSerialBuffer.hpp"

void IniSerialBuffer::printArr(){
	for(auto item : filePaths) {
		std::cout << item.c_str() << '\n';
	}
}

vector<string>& IniSerialBuffer::getFilePathVector(const char* dir){
	auto* fileStrs = new vector<string>;

	char originalDirectory[_MAX_PATH];

	// Get the current directory so we can return to it
	_getcwd(originalDirectory, _MAX_PATH);

	if(_chdir(dir) == -1) { // Change to the working directory
		perror("Can't change directory!");
		exit(1);
	}

	_finddata_t fileinfo;

	// This will grab the first file in the directory
	// "*" can be changed if you only want to look for specific files
	intptr_t handle = _findfirst("*", &fileinfo);

	if(handle == -1) {  // No files or directories found
		perror("Error searching for file");
		exit(1);
	}

	do {
		if(strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0) {
			continue;
		}

		string* nowPath = new string(dir);
		*nowPath += "\\"; //windows only
		*nowPath += fileinfo.name;

		if(fileinfo.attrib & _A_SUBDIR) { // Use bitmask to see if this is a directory
			auto subDir = getFilePathVector(nowPath->c_str());
			fileStrs->insert(fileStrs->end(), subDir.begin(), subDir.end()); //반환된 것을 뒤에 붙인다!
			delete nowPath; //폴더의 경로이기에 필요없음
		} else {
			fileStrs->push_back(nowPath->c_str());
		}
	} while(_findnext(handle, &fileinfo) == 0);

	_findclose(handle); // Close the stream

	_chdir(originalDirectory);

	return *fileStrs;
}