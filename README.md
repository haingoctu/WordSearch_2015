# WordSearch_2015 Project written for Dr. Szumlanksi's CS 1 2015. 

I. Overview:
=========

In this program, you will be given a dictionary and a word search grid, and your goal will be to print
out all the words from the dictionary that occur in the word search puzzle, along with a count of the
number of unique occurrences of those words. Words in the grid may be spelled vertically, horizontally,
or diagonally, in any direction (forward or backward, up or down, and so on). For example, given the
following dictionary and word search puzzle, the output would be as shown:

dictionary.txt
-------------
5
ab
be
he
hi
him

sample-puzzle-1.txt
-------------
3 5
abcde
efghi
jklem

sample-output-1.txt
-------------
ab (1)
be (1)
he (2)
hi (1)


(Notice that “he” occurs twice in the grid: once diagonally up and right from the 'h', and once vertically,
going down from the 'h'.)
For a large dictionary, it would be really inefficient to take each word, then search for how many times
it occurs in the word search puzzle. Instead, you will have to construct every possible string of
characters in the puzzle, one-by-one, and then search the dictionary to see if that string is an actual
word. To make this process even faster, you will have to search the dictionary using binary search.


II. Input Specifications:
=====================
You will read the dictionary from a file called dictionary.txt. You should assume the dictionary file
will be in the directory where we run your program, so open it like so:
FILE *ifp = fopen("dictionary.txt", "r");
Do NOT hard-code a file path when opening the file. For example, this is undesirable:
// WRONG! Do not hard-code a directory!
FILE *ifp = fopen("C:\Users\John\Desktop\Program2\dictionary.txt");
You should use fscanf() to read the contents of the dictionary file. The file will start with a single
integer, n, on a line by itself. Following that, there will be n lines, sorted in alphabetical order, each
containing a single word consisting entirely of lower-case letters (no spaces, digits, or other symbols).
The maximum length of any word in the dictionary is defined in the attached WordSearch.h file. You
should refer to that #defined identifier instead of hard-coding a max word length in your source file.
The wordsearch puzzle itself should be read from stdin (i.e., the keyboard); so, use scanf(), not
fscanf(), to read the puzzle. The puzzle input will start with two integers, separated by a space: the
height (3 ≤ height ≤ 300) of the puzzle grid, followed by the width (3 ≤ width ≤ 300) of the puzzle grid.
Following that will be height lines of text, each containing width number of characters.
Because the size of the dictionary and puzzle are not known ahead of time, you will have to use
dynamic memory allocation to get them into memory in your program.


III. Output Specifications:
======================
Every time you find a word in the word search puzzle that is also in the dictionary, you will increment a
counter within the dictionary. (A struct for storing the dictionary and counting these occurrences is
described in the following section.) Then, at the end of the program, you will loop through the
dictionary and print out (in alphabetical order, which is the same order in which words are stored in
dictionary.txt) each word that occurred at least once in the word search puzzle. After every such word,
you should print a single space, followed by the number of times that word occurred (in parentheses).
Although we have included some sample input files to get you started with testing the functionality of
your code, we encourage you to develop your own test cases, as well. Ours are by no means
comprehensive. We will use much more elaborate test cases when grading your submission.
