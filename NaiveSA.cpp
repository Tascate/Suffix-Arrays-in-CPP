#include "NaiveSA.h"

bool NaiveSA::addStringFromFile(std::string fileName)
{
    if (numStrings + 1 > 1)
        return false;
    SuffixArray::addStringFromFile(fileName);
	return true;
}

bool NaiveSA::addString(std::string inputString)
{
    if (numStrings + 1 > 1)
        return false;
    SuffixArray::addString(inputString);
	return true;
}

void NaiveSA::makeSuffixArray()
{
    std::stringstream ss;
    for(auto it = string->begin(); it != string->end(); ++it )
    {
        ss << (unsigned char)*it;
    }
    std::string s = ss.str();
    s.append(" ");
    std::vector<std::string> suffixes;
    for (int i = 0 ; i < s.length(); i++)
    {
        suffixes.push_back(s.substr(i, s.length()));
    }
    std::sort(suffixes.begin(), suffixes.end());

    for (auto& suffix : suffixes)
    {
        int offset = s.length() - suffix.length();
        suffixArray->push_back(offset);
    }
}