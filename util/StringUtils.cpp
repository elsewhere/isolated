#include "StringUtils.h"

namespace
{
	const static bool BLOCK_DEBUG = false;

}
//splits a string into tokens
void StringUtils::tokenize(const std::string& str, std::vector<std::string> &tokens)
{
	std::stringstream ss(str);
	std::string buffer;
    while (ss >> buffer)
    {
        tokens.push_back(buffer);
    }
}

void StringUtils::iterateDirectory(const std::string& path, std::vector<std::string>& filenames, bool appendWildcard)
{
	filenames.clear();
	//this is a bastardized MSDN example
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	//find the first file in the directory and append the mask with '*' to get all files
	if (appendWildcard)
	{
		hFind = FindFirstFile((path+"*").c_str(), &ffd);
	}
	else
	{
		hFind = FindFirstFile(path.c_str(), &ffd);
	}

	if (INVALID_HANDLE_VALUE == hFind) 
	{
 		return;
	} 

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//directory
		}
		else
		{
			//file
			std::string filename = std::string(ffd.cFileName);
			//convert to lower case
			std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
 			filenames.push_back(filename);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
}

//removes all instances of a character
void StringUtils::removeChar(std::string &str, char remove)
{
    for (auto it = str.begin(); it < str.end(); )
    {
        if (*it == remove)
        {
            it = str.erase(it);
        }
        else
        {
            it++;
        }
    }
}


bool StringUtils::readFile(const std::string& path, std::vector<std::string>& buffer, bool lowercase)
{
	std::ifstream file(path.c_str(), std::ios::in);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string line;
			getline(file, line);
			if (lowercase)
			{
				std::transform(line.begin(), line.end(), line.begin(), tolower);
			}
			buffer.push_back(line);
		}
		file.close();
		return true;
	}
	else
	{
		g_debug << "Could not open file " << path << " for reading" << std::endl;
		return false;
	}
}

bool StringUtils::writeFile(const std::string& path, std::vector<std::string>& buffer)
{
	std::ofstream writefile(path.c_str(), std::ios::out);
	if (writefile.is_open())
	{
		for (auto it = buffer.begin(); it != buffer.end(); it++)
		{
			writefile << *it << std::endl;

		}
		return true;
	}
	else
	{
		return false;
	}
}

bool StringUtils::startsWith(const std::string& str, const std::string& match)
{
	std::string s = str;
	removeChar(s, ' '); //remove spaces
	removeChar(s, '\t'); //and tabs

	size_t pos = s.find(match);
	return (pos == 0);
}

std::string StringUtils::getSubstringBetween(const std::string& str, const char match)
{
	int startIndex = str.find_first_of(match);
	int endIndex = str.find_last_of(match);

	if (startIndex == std::string::npos || endIndex == std::string::npos)
	{
		g_debug << "Cannot find two characters " << match << " from string " << str;
		return "";
	}

	int len = endIndex - startIndex - 1;
	return str.substr(startIndex + 1, len);
}



bool StringUtils::readBlock(const std::string& path, const std::string& blockName, std::vector<std::string>& source, std::vector<std::string>& result)
{
	result.clear(); //TODO: is this what we want? 

	int level = 0;
	bool readingBlock = false;
	for (auto it = source.begin(); it != source.end(); it++)
	{
		std::string s = *it;

		if (!readingBlock && s == blockName)
		{
			if (BLOCK_DEBUG)
			{
				g_debug << "found block " << blockName << std::endl;
			}
			readingBlock = true;
			continue;
		}
		s += "\n";


		if (readingBlock)
		{
			if (BLOCK_DEBUG)
			{
				g_debug << "menee blokkiin " << std::endl;
			}
			if (s.find("{") != std::string::npos)
			{
				if (level == 0)//don't include opening parenthesis
				{
					if (BLOCK_DEBUG)
					{
						g_debug << "aukeava sulku " << std::endl;
					}
				}

				if (level > 0) 				
				{
					result.push_back(s);
				}
				level++;
				if (BLOCK_DEBUG)
				{
					g_debug << "level on nyt " << level << std::endl;
				}
				continue;
			}

			if (s.find("}") != std::string::npos)
			{
				level--;

				if (level > 0)
				{
					result.push_back(s); //don't include closing parenthesis
				}
				else
				{
					if (BLOCK_DEBUG)
					{
						g_debug << "tämä oli nyt tässä " << std::endl;
					}
					//just read the closing parenthesis
					readingBlock = false;
					return true;
				}
				if (BLOCK_DEBUG)
				{
					g_debug << "level on nyt " << level << std::endl;
				}
				continue;
			}
			if (BLOCK_DEBUG)
			{
				g_debug << "normirivi" << std::endl;
			}

			//#include takes a file and inserts it here 
			if (StringUtils::startsWith(s, "#include"))
			{
				if (BLOCK_DEBUG)
				{
					g_debug << "found #include directive, string = " << s << std::endl;
				}
				std::string filename = path + getSubstringBetween(s, '\"');
				if (BLOCK_DEBUG)
				{
					g_debug << "filename = " << filename << std::endl;
				}
				std::vector<std::string> buffer;
				if (StringUtils::readFile(filename, buffer))
				{
					for (auto& s : buffer)
						result.push_back(s + "\n");
				}
				else
				{
					g_debug << "could not read file " << filename << std::
						endl;
				}
			}
			else
			{
				result.push_back(s);
			}

		}
	}
	if (!readingBlock)
	{
		if (BLOCK_DEBUG)
		{
			g_debug << "could not find requested block " << blockName << "!" << std::endl;
		}
	}
	else
	{
		if (BLOCK_DEBUG)
		{
			g_debug << "block " << blockName << " was never closed!" << std::endl;
		}
	}
	//we're still here so reading the block never stopped
	return false;
}
