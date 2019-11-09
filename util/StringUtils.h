#pragma once

#include <map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../Globals.h"

class StringUtils
{
public:
	//convert a string to number 
	template <class T> static bool convertStringToNumber(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
	{
		std::istringstream iss(s);
		return !(iss >> f >> t).fail();
	};

	//iterates a directory and gets all filenames from it into a vector
	static void iterateDirectory(const std::string& path, std::vector<std::string>& filenames, bool appendWildcard = true);

	//read a text file into buffer
	static bool readFile(const std::string& path, std::vector<std::string>& buffer, bool lowercase = false);
	//writes a text file from buffer
	static bool writeFile(const std::string& path, std::vector<std::string>& buffer);

	//split the string into tokens
    static void tokenize(const std::string& str, std::vector<std::string> &tokens);
    //remove all instances of a character from a string
    static void removeChar(std::string &str, char remove);

	static bool startsWith(const std::string& str, const std::string& match);

	static std::string getSubstringBetween(const std::string& str, const char match);

	static bool readBlock(const std::string& path, const std::string& blockName, std::vector<std::string>& source, std::vector<std::string>& result);
};
