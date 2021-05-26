#include "SAIS.h"

bool SAIS::addStringFromFile(std::string fileName)
{
    if (numStrings + 1 > 1)
        return false;
    SuffixArray::addStringFromFile(fileName);
	return true;
}

bool SAIS::addString(std::string inputString)
{
    if (numStrings + 1 > 1)
        return false;
    SuffixArray::addString(inputString);
	return true;
}

void SAIS::makeSuffixArray()
{
    std::vector<int> bucket = fillBucket(*string, maxAlphabetSize);
    SAIS_SA(*string, *suffixArray, bucket);
}

std::vector<int> SAIS::fillBucket(std::vector<int>& string, int alphabetSize)
{
	std::vector<int> bucketSize(alphabetSize, 0);
	for (int i = 0; i < string.size(); i++)
	{
		bucketSize[string[i]]++;
	}
	return bucketSize;
}

std::vector<bool> SAIS::fillLTypeArray(const std::vector<int>& string)
{
	// +1 size to store empty suffix/sentinel character
	std::vector<bool> target(string.size()+1, false);

	//Empty suffix is S_Type
	*target.rbegin() = false;

	//If string is empty
	if (string.size() == 0)
	{
		return target;
	}
	//Last character is L_Type always
	*(target.rbegin() + 1) = true;
	
	for (int i = target.size()-3; i >= 0; i--)
	{
		//L-Type Suffix Criteria
		if (string[i] > string[i + 1])
			target[i] = true;
		else if (string[i] == string[i + 1] && target[i + 1])
			target[i] = true;
		else
			target[i] = false;
	}
	return target;
}

std::vector<int> SAIS::findBucketHeads(std::vector<int>& bucket)
{
	unsigned int offset = 1;
	unsigned int length = bucket.size();
	std::vector<int> result(length);
	for (auto i = 0; i < bucket.size(); ++i)
	{
		result[i] = offset;
		offset += bucket[i];
	}
	return result;
}

std::vector<int> SAIS::findBucketTails(std::vector<int>& bucket)
{
	unsigned int offset = 1; //Suffix for Sentinel Characters start at the beginning
	unsigned int length = bucket.size();
	std::vector<int> result(length);
	for (auto i = 0; i < bucket.size(); ++i)
	{
		offset += bucket[i];
		result[i] = offset - 1;
	}
	return result;
}

bool SAIS::isLMSChar(unsigned int offset, const std::vector<bool>& Ltype)
{
	if (offset == 0)
		return false;
	if (!Ltype[offset] && Ltype[offset - 1])
		return true;
	return false;
}

bool SAIS::isLMSSubstringsEqual(const std::vector<int>& string, const std::vector<bool>& Ltype, unsigned int offset1, unsigned int offset2)
{
	//Substrings not equal if equal to the empty suffix
	if (offset1 == string.size() || offset2 == string.size())
		return false;
	int i = 0;
	while (true) 
	{
		bool firstIsLMS = isLMSChar(i + offset1, Ltype);
		bool secondIsLMS = isLMSChar(i + offset2, Ltype);

		//True only if no difference is found
		if (i > 0 and firstIsLMS && secondIsLMS)
			return true;
		//Reached end of one substring
		if (firstIsLMS != secondIsLMS)
			return false;
		//If character difference
		if (string[i + offset1] != string[i + offset2])
			return false;
		i += 1;
	}
	
}

void SAIS::SAIS_SA(std::vector<int>& string, std::vector<int>& target, std::vector<int> &bucket)
{
	//Compute whether a character is LType or SType
	std::vector<bool> LType = fillLTypeArray(string);

	//Bucket-sort to insert all LMS suffixes
	std::vector<int> guess = guessLMSSort(string, bucket, LType);
	//Move all other suffixes into guess array by induced sorting
	inducedSortL(string, guess, bucket, LType);
	inducedSortS(string, guess, bucket, LType);

	//Create new string to summarize relative order of LMS suffixes 
	std::vector<int> summaryString;
	std::vector<int> summaryOffset;
	int summaryAlphabetSize = summariseSuffixArray(string, guess, LType, summaryString, summaryOffset);

	//Make sorted suffix array of summary string
	std::vector<int> summarySuffixArray = makeSummarySuffixArray(summaryString, summaryAlphabetSize);

	//LMS suffix positions are now known
	std::vector<int> finalSuffixArray = LMSsort(string, bucket, LType, summarySuffixArray, summaryOffset);
	//Finish suffix array by induced sorting
	inducedSortL(string, finalSuffixArray, bucket, LType);
	inducedSortS(string, finalSuffixArray, bucket, LType);

	//Copy into the target suffix array
    target.assign(finalSuffixArray.begin(), finalSuffixArray.end());
}

std::vector<int> SAIS::guessLMSSort(const std::vector<int>& string, std::vector<int>& bucket, const std::vector<bool>& Ltype)
{
	std::vector<int>guessSuffixArray(string.size()+1, -1);
	auto bucketTails = findBucketTails(bucket);
	//Bucket sort all LMS suffixes
	for (int i = 0; i < string.size(); i++)
	{
		if (isLMSChar(i, Ltype))
		{
			unsigned int bucketIndex = string[i];
			guessSuffixArray[bucketTails[bucketIndex]] = i;
			bucketTails[bucketIndex] -= 1;
		}
	}
	//Empty suffix goes at front
	guessSuffixArray[0] = string.size();
	return guessSuffixArray;
}

void SAIS::inducedSortL(const std::vector<int>& string, std::vector<int>& target, std::vector<int>& bucket, const std::vector<bool>& Ltype)
{
	auto bucketHeads = findBucketHeads(bucket);
	for (auto i = 0U; i < target.size(); i++)
	{
		//Looking for L-Type
		if (target[i] == -1)
			//No offset at this index
			continue;
		//Look at suffix beginning to left
		int j = target[i] - 1;
		if (j < 0)
			//J is out of bounds
			continue;
		if (!Ltype[j])
			//Offset j is a S-Type suffix
			continue;

		//Offset J is a L-Type suffix

		//Place suffix at bucket index
		unsigned int bucketIndex = string[j];
		target[bucketHeads[bucketIndex]] = j; //
		bucketHeads[bucketIndex] += 1; //increment head pointer
	}
}

void SAIS::inducedSortS(const std::vector<int>& string, std::vector<int>& target, std::vector<int>& bucket, const std::vector<bool>& Ltype)
{
	auto bucketTails = findBucketTails(bucket);
	for (int i = target.size()-1; i >= 0; i--)
	{
		int j = target[i] - 1;
		if (j < 0)
			//J is out of bounds
			continue;
		if (Ltype[j])
			//Offset j is a L-Type suffix
			continue;

		//Offset j is a S-Type suffix

		//Place suffix at bucket index
		unsigned int bucketIndex = string[j];
		target[bucketTails[bucketIndex]] = j; //
		bucketTails[bucketIndex] -= 1; //decrement tail pointer
	}
}

int SAIS::summariseSuffixArray(const std::vector<int>& string, std::vector<int>& guessArray, const std::vector<bool>& Ltype,
									  std::vector<int>& targetSummaryString,
									  std::vector<int>& targetSummaryOffset)
{
	std::vector<int> lmsNames(string.size()+1, -1);
	int currentName = 0; //Track allocated names
	//First LMS-substring represents the empty suffix
	lmsNames[guessArray[0]] = currentName;
	int lastLMSSuffixOffset = guessArray[0];

	for (int i = 1; i < guessArray.size(); i++)
	{
		int suffixOffset = guessArray[i];

		//Pass over non LMS suffixes
		if (!isLMSChar(suffixOffset, Ltype))
			continue;
		//If LMS suffix is different from last suffix 
		if (!isLMSSubstringsEqual(string, Ltype, lastLMSSuffixOffset, suffixOffset))
			currentName += 1;
		//Update last suffix looked at
		lastLMSSuffixOffset = suffixOffset;
		//Store name of LMS suffix
		lmsNames[suffixOffset] = currentName;
	}

	//Clean up lmsNames by removing unused indices
	//Create Summary String
	for (int i = 0; i < lmsNames.size(); i++)
	{
		if (lmsNames[i] != -1)
		{
			targetSummaryOffset.push_back(i);
			targetSummaryString.push_back(lmsNames[i]);
		}
	}
	//Return alphabet size for summary string
	return currentName + 1;
}

std::vector<int> SAIS::makeSummarySuffixArray(std::vector<int>& summaryString, int summaryAlphabetSize)
{
	//Every character only appears once
	//Use bucket sort to make suffix array
	if (summaryAlphabetSize == summaryString.size())
	{
		std::vector<int> summarySuffixArray(summaryString.size()+1);
		summarySuffixArray[0] = summaryString.size();

		for (int i = 0; i < summaryString.size(); i++)
		{
			summarySuffixArray[summaryString[i] + 1] = i;
		}
		return summarySuffixArray;
	}
	//Recursion to create suffix array
	else {
		std::vector<int> bucket = fillBucket(summaryString, summaryAlphabetSize);
        std::vector<int> target;
		SAIS_SA(summaryString, target, bucket);
        return target;
	}
}

std::vector<int> SAIS::LMSsort(const std::vector<int>& string, std::vector<int>& bucketSizes, const std::vector<bool>& Ltype, std::vector<int>& sumSuffixArray, std::vector<int>& sumOffsets)
{
	std::vector<int> suffixOffsets(string.size()+1, -1);
	std::vector<int> bucketTails = findBucketTails(bucketSizes);

	//Reverse order to add suffixes to respective buckets
	for (int i = sumSuffixArray.size() - 1; i > 1; --i)
	{
		int stringIndex = sumOffsets[sumSuffixArray[i]];
		int bucketIndex = string[stringIndex];
		
		//Place suffix at bucket location
		suffixOffsets[bucketTails[bucketIndex]] = stringIndex;
		bucketTails[bucketIndex] -= 1; //Decrement tail position
	}
	//Empty suffix placement
	suffixOffsets[0] = string.size();
	return suffixOffsets;
}