#include "SuffixArray.h"

SuffixArray::SuffixArray()
{
    numStrings = 0;
    suffixArray = new std::vector<int>;
    string = new std::vector<int>;
}

SuffixArray::~SuffixArray()
{
    delete suffixArray;
    delete string;
}

/*
    Appends strings to make a Suffix Array of
*/
bool SuffixArray::addString(std::string inputString)
{
    for (auto it = inputString.cbegin(); it != inputString.cend(); it++) {
		string->push_back(*it);
	}
    numStrings += 1;
	return true;
}

bool SuffixArray::addStringFromFile(std::string fileName)
{
	std::ifstream fin(fileName, std::ios::binary);
	if (!fin)
	{
		std::cerr << "Could not read file from: " << fileName << std::endl;
		return false;
	}
	
	//Do not skip whitespace characters
	fin.unsetf(std::ios::skipws);
	
	//Read in size of file
	fin.seekg(0, std::ios::end);
	std::streampos length = fin.tellg();
	fin.seekg(0, std::ios::beg);

	//Read in all data from file
	string->reserve(length);
    
	int i = 0;
    char* temp = new char;
	while (i < length)
	{
		fin.read(temp, sizeof(*temp));
		string->push_back(reinterpret_cast<unsigned char&>(*temp));
		i++;
	}
	delete temp;

    numStrings += 1;
	return true;
}

/*
    Construct LCP Array using Kasai's Algorithim
    Assumes that SuffixArray is already built
    Assumes Sentinel Characters are Unique
*/
std::vector<int> SuffixArray::makeLCPArray()
{
	int length = suffixArray->size();
	std::vector<int> lcp(length, 0);
	std::vector<int> invertedSuffixArray(length, 0);

	int l = 0;
	//Construct Inverted Suffix Array
	for (int i = 0; i < length; i++)
		invertedSuffixArray[(*suffixArray)[i]] = i;

	//Kasai Algorithim
	for (int i = 0; i < length - 1; i++)
	{
		int k = invertedSuffixArray[i];
		int j = (*suffixArray)[k - 1];
		while ((*string)[i + l] == (*string)[j + l])
			l += 1;
		lcp[k] = l;
		if (l > 0)
			l -= 1;

	}
	return lcp;
}

/*
	Finds Longest Common Strand that appears in k substrings
	Returns length of LCS and saves offsets to a set
*/
int SuffixArray::findLongestCommonStrand(const unsigned int k)
{
	if (k < 1 || k > numStrings)
	{
		std::cout << "Given invalid k-value: " << k << std::endl;
	}

    //initialize LCP Array and Offsets
	std::vector<int> lcp = makeLCPArray();
	unsigned int length = lcp.size();
    std::set<int> offsets;
	
    
    // Can skip suffix indexes that are sentinel characters
	unsigned int window1 = numStrings; //sliding window, first index
	unsigned int window2 = numStrings; //sliding window, second index
	std::map<int, int> suffixSourcesMap; //Tracks # of suffixes for each string in the sliding window
	suffixSourcesMap[calcParentString((*suffixArray)[window1])] += 1; //first value

	int max = 0;
	std::deque<int> window; //Tracks the minimum value for the sliding window

    //Must accurately track how many strings are in the sliding window
    //Lambda expression to Decrement given index
	//And delete given index if # of suffixes for the string becomes 0
    auto decrementIndex = [](std::map<int, int> &map, int index) {
		if (map[index] - 1 <= 0)
			map.erase(index);
		else
			map[index] -= 1;
	};

    //This code is used twice, so let's make this a lambda expression
    auto updateMaxValue = [&](int index1, int index2) {
		//Only need to check if the first byte of the suffix is equal
		if ((*string)[index1] == (*string)[index2])
		{
			//Update offsets and max
			if (lcp[window.front()] > max)
			{
				max = lcp[window.front()];
				offsets.clear();
				offsets.insert(index1);
			}
			//Edge-case if the LCS actually has two or more answers
			else if (lcp[window.front()] == max)
			{
				offsets.insert(index2);
			}
		}
	};

	//Begin looping through lcp array
    while (window1 < length && window2 < length)
	{
		int suffix1 = (*suffixArray)[window1];
		int suffix2 = (*suffixArray)[window2];
		if (suffixSourcesMap.size() >= k) //We are looking for K-unqiue strings in our sliding window
		{
            //Update our max value if needed
			updateMaxValue(suffix1, suffix2);

			//Reduce sliding window
			decrementIndex(suffixSourcesMap, calcParentString(suffix1));
			window1 += 1;

			//Remove min value if outside window+1-window2
			if (window.front() <= window1)
				window.pop_front();
		}
		//Enlarge sliding window
		else if (++window2 < length) {
			suffixSourcesMap[calcParentString((*suffixArray)[window2])] += 1;
			
			//Do not need to track values larger then one being inserted
			//Therefore remove all those values
			while (!window.empty() && lcp[window.back()] >= lcp[window2])
				window.pop_back();
			window.push_back(window2);
		}
	}
	
	//Look at any missed lcp values
	int suffix2 = *suffixArray->crbegin();
	while (++window1 < length && suffixSourcesMap.size() >= k)
	{
		int suffix1 = (*suffixArray)[window1];
		decrementIndex(suffixSourcesMap, calcParentString(suffix1-1));
		if (window.front() <= window1)
				window.pop_front();
		//Update our max value if needed
		updateMaxValue(suffix1, suffix2);
	}

	//Print out results
	for (auto it = offsets.begin(); it != offsets.end(); it++)
		printSubStringVector(*string, *it, max);
    
	return max;
}

/*
    Returns which string a suffix is in
    i.e. first string, second string
*/
int SuffixArray::calcParentString(int suffixOffset)
{
	//Given offset is out of range
	if (suffixOffset > (*suffixArray)[0])
	{
		std::cerr << "Offset is out of range: " << suffixOffset << std::endl;
		return -1;
	}

	//Trivial answer
	if (numStrings <= 1)
		return numStrings;

	int high = numStrings - 1;
	int low = 0;
	int mid = 0;

	//Binary Search to find closest value
	while (low <= high)
	{
		mid = low + (high - low) / 2;
		if ((*suffixArray)[mid] == suffixOffset)
			return numStrings - mid;
		if ((*suffixArray)[mid] > suffixOffset)
			low = mid + 1;
		else
			high = mid - 1;
	}
	//Edge-case where offset is greater then the closest value
	if (suffixOffset > (*suffixArray)[mid])
		mid--;
	//Found the parent string
	return numStrings - mid;
}

template <typename T>
void SuffixArray::printVector(std::vector<T> v) 
{
	for (auto i = v.begin(); i != v.end(); i++)
	{
		std::cout << *i << ' ';
	}
	std::cout << std::endl;
}

template <typename T>
void SuffixArray::printSubStringVector(std::vector<T> v, int offset, int length) 
{
	if (offset + length <= v.size()) {
		for (auto i = offset; i < offset + length; i++)
		{
			std::cout << (char)v.at(i) << ' ';
		}
		std::cout << std::endl;
	}
	else {
		std::cerr << "Given offset & length for SubVector Print out of range" << std::endl;
		std::cerr << "Offset: " << offset << std::endl;
		std::cerr << "Length: " << length << std::endl;
		std::cerr << "Vector Size: " << v.size() << std::endl;
	}
}

void SuffixArray::printSuffixArray()
{
	std::cout << "Suffix Array : ";
	printVector(*suffixArray);
}