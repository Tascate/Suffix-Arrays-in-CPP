#include "Skew.h"

bool Skew::addStringFromFile(std::string fileName)
{
    SuffixArray::addStringFromFile(fileName);
    string->push_back(sentinelCharacter);
    sentinelCharacter -= 1;
	return true;
}

bool Skew::addString(std::string inputString)
{
    SuffixArray::addString(inputString);
    string->push_back(sentinelCharacter);
    sentinelCharacter -= 1;
	return true;
}

void Skew::makeSuffixArray()
{
	suffixArray->resize(string->size());

	//Skew Algorithim Implementation assumption
	//See function declaration comments
	//Add temp characters
	string->push_back(sentinelCharacter);
	string->push_back(sentinelCharacter);
	string->push_back(sentinelCharacter);
	skewSuffixArray(*string, *suffixArray, string->size()-3, maxAlphabetSize, numStrings+1);

	//Remove temp characters
	string->pop_back();
	string->pop_back();
	string->pop_back();
}

/*
	Skew-Algorithim for constructing Suffix Arrays of an integer alphabet given a source from s[1...n]
	Assumes s[n] = s[n+1] = s[n+2] = unused sentinelCharacter
	Supports for a source consisting of different sentinel characters (for multiple given sources)
*/
void Skew::skewSuffixArray(const std::vector<int> &s, std::vector<int> &SA, int length, int alphabetSize, int alphabetOffset)
{
	int n0 = (length + 2) / 3, n1 = (length + 1) / 3, n2 = length / 3, n02 = n0 + n2;
	std::vector<int> s12(n02 + 3); s12[n02] = s12[n02 + 1] = s12[n02 + 2] = sentinelCharacter;
	std::vector<int> SA12(n02 + 3); SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = sentinelCharacter;
	std::vector<int> s0(n0);
	std::vector<int> SA0(n0);

	//Create Triple Indexes for s12
	for (int i = 0, j = 0; i < length + (n0 - n1); i++)
		if (i % 3 != 0)
			s12[j++] = i;

	//RadixSort to construct SA12
	radixSort(s12, SA12, s, 2, n02, alphabetSize, alphabetOffset);
	radixSort(SA12, s12, s, 1, n02, alphabetSize, alphabetOffset);
	radixSort(s12, SA12, s, 0, n02, alphabetSize, alphabetOffset);

	int name = 0, c0 = -1, c1 = -1, c2 = -1;

	for (int i = 0; i < n02; i++)
	{
		if (s[SA12[i]] != c0 || s[SA12[i] + 1] != c1 || s[SA12[i] + 2] != c2)
		{
			name++;
			c0 = s[SA12[i]]; c1 = s[SA12[i] + 1]; c2 = s[SA12[i] + 2];
		}
		if (SA12[i] % 3 == 1)
			s12[SA12[i] / 3] = name;
		else
			s12[SA12[i] / 3 + n0] = name;
	}

	//Create Suffix Array for s12
	//recursion if names are not unique
	if (name < n02)
	{
		skewSuffixArray(s12, SA12, n02, name, alphabetOffset);
		for (int i = 0; i < n02; i++)
			s12[SA12[i]] = i + 1;
	}
	else
		for (int i = 0; i < n02; i++)
			SA12[s12[i] - 1] = i;
	
	//construct suffix array for s0 and sort
	for (int i = 0, j = 0; i < n02; i++)
		if (SA12[i] < n0)
			s0[j++] = 3 * SA12[i];
	radixSort(s0, SA0, s, 0, n0, alphabetSize, alphabetOffset);

	//merge SA0 and SA12 suffixes
	auto leqPair = [](int a1,int a2, int b1, int b2)->bool{
		return a1 < b1 || a1 == b1 && a2 <= b2;
	};
	auto leqTriple = [leqPair](int a1, int a2, int a3, int b1, int b2, int b3)->bool {
		return a1 < b1 || a1 == b1 && leqPair(a2,a3,b2,b3);
	};
	for (int p = 0, t = n0 - n1, k = 0; k < length; k++)
	{
		int i = SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2; //offset of 12 suffix
		int j = SA0[p]; //offset of 0 suffix
		if (SA12[t] < n0 ? leqPair(s[i],s12[SA12[t]+n0],s[j],s12[j/3]) : 
			leqTriple(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0]))
		{
			SA[k] = i;
			t++;
			if (t == n02)
				for (k++; p < n0; p++, k++) SA[k] = SA0[p];
		}
		else
		{
			SA[k] = j;
			p++;
			if (p == n0)
				for (k++; t < n02; t++, k++)
					SA[k] = SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2;
		}
	}
}

/*
	radixSort
	From Skew Algorithim Implementation
	Allows for alphabet that includes negative integers
		Assumes alphabetOffset offsets the smallest character of the alphabet to 0
*/
void Skew::radixSort(std::vector<int>& a, std::vector<int>& b, const std::vector<int> &r, int offset, int length, int alphabetSize, int alphabetOffset) 
{
	std::vector<int> count(alphabetSize + 1+alphabetOffset,0);
	for (int i = 0; i < length; i++)
		count[r[a[i]+offset]+alphabetOffset]++;
	for (int i = 0, sum = 0; i <= alphabetSize+alphabetOffset; i++)
	{
		int t = count[i];
		count[i] = sum;
		sum += t;
	}
	for (int i = 0; i < length; i++)
		b[count[r[a[i]+offset]+alphabetOffset]++] = a[i];
	
}