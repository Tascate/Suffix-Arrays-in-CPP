#include "Skew.h"
#include "SAIS.h"
#include "NaiveSA.h"

int main()
{
    std::string testString("cabbage");

    Skew s;
    s.addString(testString);
    s.makeSuffixArray();
    s.printSuffixArray();

    SAIS s2;
    s2.addString(testString);
    s2.makeSuffixArray();
    s2.printSuffixArray();

    NaiveSA s3;
    s3.addString(testString);
    s3.makeSuffixArray();
    s3.printSuffixArray();
}