#include "Skew.h"
#include "SAIS.h"
#include "Naive.h"

int main()
{
    std::string testString("cabbage");
    std::cout << "Test String: " << testString << std::endl;

    Skew s;
    std::cout << typeid(s).name() << std::endl;
    s.addString(testString);
    s.makeSuffixArray();
    s.printSuffixArray();
    std::cout << std::endl;

    SAIS s2;
    std::cout << typeid(s2).name() << std::endl;
    s2.addString(testString);
    s2.makeSuffixArray();
    s2.printSuffixArray();
    std::cout << std::endl;

    Naive s3;
    std::cout << typeid(s3).name() << std::endl;
    s3.addString(testString);
    s3.makeSuffixArray();
    s3.printSuffixArray();
    std::cout << std::endl;

    Skew testLCS;
    std::cout << "LCS for: " << testString << " " << "baggage" << std::endl;
    std::cout << typeid(s3).name() << std::endl;
    testLCS.addString(testString);
    testLCS.addString("baggage");
    testLCS.makeSuffixArray();
    testLCS.printSuffixArray();
    testLCS.findLongestCommonStrand(2);
}