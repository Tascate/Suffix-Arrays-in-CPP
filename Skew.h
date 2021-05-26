#include "SuffixArray.h"

class Skew: public SuffixArray
{
public:
    Skew() {};
    void makeSuffixArray();
    bool addStringFromFile(std::string);
	bool addString(std::string);
private:
    int sentinelCharacter = -1;
	void skewSuffixArray(const std::vector<int>&, std::vector<int>&, int, int, int);
	void radixSort(std::vector<int>&, std::vector<int>&, const std::vector<int>&, int, int, int, int);
};