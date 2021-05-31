# C++ Suffix Arrays

This is a set of Suffix Array implementations using the **SA-IS algorithm** and the **Skew Algorithm**.   
LCP Array construction from a Suffix Array is implemented by the **Kasai Algorithm** and used to find the **Longest Common Substring in k-Strings**.    
Resources to learn about the algorithim, Suffix Arrays, and their applications can be found at the end of this readme.

## Compiling & Building
In order to build the project, you will need to install:
- **CMake**
- **Make**

Clone the Repository.
```bash
git clone "https://github.com/Tascate/Suffix-Array-Implementation.git"
```
Compile using CMake.
```bash
cmake .
```
Build using the Makefile.
```bash
make
```
Run to view test code.
```
./SuffixArray
```

## Example Usage
Given the following code:
```c++
#include "Skew.h"

Skew sa;
sa.addString("cabbage");
sa.makeSuffixArray();
sa.printSuffixArray();
```
This will output the computed Suffix Array using the Skew Algorithm:
> Suffix Array : 7 1 4 3 2 0 6 5


## Things to note
The abstract class **SuffixArray** represents a generic base class that any class can derive from to implement different Suffix Array Construction Algorithms. The classes: **Naive**, **Skew**, **SAIS**, all derive from the **SuffixArray** class to implement their respective algorithms.

### Skew Algorithm
**O(n) time, O(n) work**   
The algorithm is fairly simple to implement, requiring less code overall than the SA-IS implementation. Because of this, it is quite useful for quick testing and usage of Suffix Arrays.  This implementation has support for **multiple strings** which is useful for finding the LCS of **k-Strings**.

### SA-IS Algorithm
**O(n) time, O(n) work**   
This is an implementation of the SA-IS algorithm for my own understanding as well as improving my C++. The code is largely based on Screwtape's [A walk through the SA-IS Suffix Array Construction Algorithm](https://zork.net/~st/jottings/sais.html) and **may not necessarily be optimized**. Screwtape's walkthrough is a great resource for learning the SA-IS algorithm!

### Kasai Algorithm
**O(n) time, O(1) work**   
A simple algorithm to construct the LCP Array from a Suffix Array in linear time. In this code, the LCP Array can be used to find the Longest Common Substring.

### Longest Common String for k-Strings
**O(n) time, O(n) work)**   
Finding the LCS is based upon the premise that for a __sliding window__ from **i** to **j** on the LCP array. The **LCP value** for that sliding window is the minimum value from **i+1** to **j** provided that the first character for the suffix **i** and suffix **j** are the same. Also, we only have to update the current maximum value if the sliding window contains unique **k-strings**.   
We can then use bookkeeping to find the longest common substring.

#### Finding out which string a suffix belongs to:
For coding simplicity, finding out the parent string for a given suffix takes **O(logm)** time (where **m = # of strings**) and **O(1)** space using binary search. However, it is possible to optimize for **O(1)** time by tracking the parent string for a suffix in memory. (For example, a map or array)


## Good Resources I used to Learn about Suffix Arrays
### Algorithm Documentation:
- **SA-IS**: "*Linear Suffix Array Construction by Almost Pure Induced-Sorting*" by G. Nong, S. Zhang and W. H. Chan"
- **Skew**: "*Simple Linear Work Suffix Array Construction*" by Juha Kärkkäinen and Peter Sanders)
- **Kasai**: "*Linear-Time Longest Common-Prefix Computation in Suffix Arrays and Its Applications*" by Toru Kasai, Gunho Lee, Hiroki Arimura, Setsuo Arikawa and Kunsoo Park

### Learning:
- [**Stanford Slide Visuals on explaining Suffix Arrays**](http://web.stanford.edu/class/archive/cs/cs166/cs166.1196/lectures/04/Small04.pdf)
- [**Screwtape's "A walk through the SA-IS Suffix Array Construction Algorithm"**](https://zork.net/~st/jottings/sais.html)
- [**markormesher's Skew Algorithm explanation**](https://gist.github.com/markormesher/59b990fba09972b4737e7ed66912e044)
- [**Skew Algorithim Description**](http://www.mi.fu-berlin.de/wiki/pub/ABI/SS13Lecture3Materials/script.pdf)
- [**Skew Algorithim Slide Visuals**](https://www.cs.cmu.edu/~ckingsf/bioinfo-lectures/suffixarrays.pdf)
#### Videos:
- [**Longest Common Prefix (LCP Array)**](https://www.youtube.com/watch?v=53VIWj8ksyI&t=0s)
- [**Longest common substring problem suffix array part 1**](https://www.youtube.com/watch?v=Ic80xQFWevc)
- [**Longest Common Substring problem suffix array part 2**](https://www.youtube.com/watch?v=DTLjHSToxmo)
- [**Longest Repeated Substring**](https://www.youtube.com/watch?v=OptoHwC3D-Y)
