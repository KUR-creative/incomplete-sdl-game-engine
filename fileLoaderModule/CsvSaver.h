#pragma once

#include <string>
using std::string;

class CsvSaver {
public:
	bool save(const string& folderPath, const string& onlyName, const string& data);
};