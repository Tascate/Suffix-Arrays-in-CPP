#include "SuffixArray.h"
#include <sstream>
#include <iterator>
#include <algorithm>

class NaiveSA : public SuffixArray
{
public:
    NaiveSA() {};
    void makeSuffixArray();
    bool addStringFromFile(std::string);
	bool addString(std::string);
};