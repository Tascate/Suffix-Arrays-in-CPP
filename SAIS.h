#include "SuffixArray.h"
class SAIS : public SuffixArray
{
public:
    SAIS() {};
    void makeSuffixArray();
    bool addStringFromFile(std::string);
	bool addString(std::string);
private:
	void SAIS_SA(std::vector<int>&, std::vector<int>& target, std::vector<int>&);

	std::vector<int> fillBucket(std::vector<int>&, int alphabetSize);
	std::vector<bool> fillLTypeArray(const std::vector<int>&);
	std::vector<int> findBucketHeads(std::vector<int>&);
	std::vector<int> findBucketTails(std::vector<int>&);

	std::vector<int> guessLMSSort(const std::vector<int>&, std::vector<int> &bucketSizes, const std::vector<bool> &Ltype);
	std::vector<int> LMSsort(const std::vector<int>&, std::vector<int> &bucketSizes, const std::vector<bool> &Ltype, std::vector<int> &sumArray, std::vector<int> &sumOffsets);
	
	//Returns
	int summariseSuffixArray(const std::vector<int>&, std::vector<int>&, const std::vector<bool>&,
								std::vector<int>& targetSummarySource,
								std::vector<int>& targetSummaryOffset);
	std::vector<int> makeSummarySuffixArray(std::vector<int>&, int);

	void inducedSortL(const std::vector<int>&, std::vector<int> &, std::vector<int>&, const std::vector<bool> &);
	void inducedSortS(const std::vector<int>&, std::vector<int>&, std::vector<int>&, const std::vector<bool>&);

	bool isLMSChar(unsigned int offset, const std::vector<bool>&);
	bool isLMSSubstringsEqual(const std::vector<int>&, const std::vector<bool>&, unsigned int, unsigned int);
};