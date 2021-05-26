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

    //Set-up LCP Array and Offsets
    std::set<int> offsets;
	std::vector<int> lcp = makeLCPArray();
	unsigned int length = lcp.size();
    
    //Skip indexes set aside for sentinel characters
	unsigned int window1 = numStrings; //sliding window, first index
	unsigned int window2 = numStrings; //sliding window, second index
	std::map<int, int> suffixSourcesMap; //Tracks # of suffixes for each string in the sliding window
	
    //Accurately track how many strings are in the sliding window
    //Delete its index if # of suffixes becomes 0
    auto decrementIndex = [](std::map<int, int> &map, int index) {
		if (map[index] - 1 <= 0)
			map.erase(index);
		else
			map[index] -= 1;
	};
	suffixSourcesMap[calcParentString((*suffixArray)[window1])] += 1;

	int max = 0;
	int currentMinIndex = 1; //index for LCP minimum value of sliding window from index1+1 to index2
	
    //This code is used twice, so let's make this a lambda expression
    auto updateMaxValue = [&](int index1, int index2) {
		//Only need to check if the first byte of the suffix is equal
		if ((*string)[index1] == (*string)[index2])
		{
			//Update offsets and max
			if (lcp[currentMinIndex] > max)
			{
				max = lcp[currentMinIndex];
				offsets.clear();
				offsets.insert(index1);
				offsets.insert(index2);
			}
			//Edge-case if the LCS actually has two or more answers
			else if (lcp[currentMinIndex] == max)
			{
				offsets.insert(index1);
				offsets.insert(index2);
			}
		}
	};

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
			/*
				Update currentMinIndex to new sliding window
				currentMinIndex may be set outside sliding window
					this will only happen when window1 == window2
				sliding window will catch up
			*/
			window1 += 1;
		}
		//Enlarge sliding window
		else if (++window2 < length) {
			suffixSourcesMap[calcParentString((*suffixArray)[window2])] += 1;
		}

        //Sliding Window has changed so find its new minimum
        if (window1 == window2) //trivial min value
            currentMinIndex = window1;
        else
        {
            currentMinIndex = window1 + 1;
            for (auto i = window1 + 2; i <= window2; i++)
					if (lcp[currentMinIndex] > lcp[window1])
						currentMinIndex = window1;
        }
	}
	
	//Look at any missed lcp values
	int suffix2 = *suffixArray->crbegin();
	while (++window1 < length && suffixSourcesMap.size() >= k)
	{
		int suffix1 = (*suffixArray)[window1];
		decrementIndex(suffixSourcesMap, calcParentString(suffix1-1));
		currentMinIndex = window1 + 1;
		for (auto i = window1 + 1; i < window2; i++)
			if (lcp[currentMinIndex] > lcp[i])
				currentMinIndex = i;
		//Update our max value if needed
		updateMaxValue(suffix1, suffix2);
	}
    
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
void printSubVector(std::vector<T> v, int offset, int length) 
{
	if (offset + length <= v.size()) {
		for (auto i = offset; i < offset + length; i++)
		{
			std::cout << v.at(i) << ' ';
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