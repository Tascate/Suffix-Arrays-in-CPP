#include "Skew.h"
#include "SAIS.h"
#include "NaiveSA.h"

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

    NaiveSA s3;
    std::cout << typeid(s3).name() << std::endl;
    s3.addString(testString);
    s3.makeSuffixArray();
    s3.printSuffixArray();
}