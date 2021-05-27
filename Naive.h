#include "SuffixArray.h"
#include <sstream>
#include <iterator>
#include <algorithm>

class Naive : public SuffixArray
{
public:
    Naive() {};
    void makeSuffixArray();
    bool addStringFromFile(std::string);
	bool addString(std::string);
};