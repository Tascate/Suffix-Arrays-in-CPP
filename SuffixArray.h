#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
class SuffixArray
{
public:
	SuffixArray();
	virtual ~SuffixArray();
	virtual void makeSuffixArray() = 0;
	virtual bool addStringFromFile(std::string);
	virtual bool addString(std::string);

	std::vector<int> makeLCPArray();
	int findLongestCommonStrand(const unsigned int);
	int calcParentString(int);

	void printSuffixArray();
	
protected:
	const int maxAlphabetSize = 256;
	int numStrings;
	std::vector<int>* suffixArray;
	std::vector<int>* string;

/*
	Print methods
*/
	template <typename T>
	void printVector(std::vector<T> v);

	template <typename T>
	void printSubVector(std::vector<T> v, int offset, int length);
};

