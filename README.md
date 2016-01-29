# BK String
BK String is a C implementation of a BK Tree for strings.

## Building BK String
To build everything:

1. Navigate to ``./bk-string``
2. Run ``make``

To build tests only:

1. Navigate to ``./bk-string``
2. Run ``make test``
3. Builds ``./test/test`` executable

To build benchmarks only:

1. Navigate to ``./bk-string``
2. Run ``make bench``
3. Builds ``./benchmark/bench`` executable

To build shared library only:

1. Navigate to ``./bk-string``
2. Run ``make shared``
3. Builds ``./build/libbkstring.so`` dynamic library

To build static library only:

1. Navigate to ``./bk-string``
2. Run ``make static``
3. Builds ``./build/libbkstring.a`` static library

## Usage
To use simply:

    #include "bkstring.h"

and then link the ``./src/bkstring.h``

### BK Tree Initialization
To initialize a BK Tree:

    BKTree b = init_bk_tree();


### BK Tree Building
To add a single word onto a BK Tree:

    BKTree b = init_bk_tree();
    b.Add("foo", &b);

### BK Tree Searching
To search for words within a distance:

    // Search(char*, int, BKTree*)
    char** list = b.Search("foo", 2, &b);

    // Do stuff with list.

    // When you're done, don't forget to free the memory allocated.
    free(list);

    // "list" is all words in "b" within 2 Levenshtein Distance from "foo."

In the above example the search list is allocated to the ``list`` variable.  This means when the ``list`` variable is ``free()``d, so is the allocated memory used by the ``Search()`` function.

### BK Tree Deconstruction
To free memory allocated by the BK Tree:

    clear_bktree(&b);

## Running Tests
If you ran the ``Makefile``, an executable test file was created at ``./test/test``.  Simply run that to test all libraries are working as intended.

## Running Benchmarks
If you ran the ``Makefile``, an executable benchmark file was created at ``./benchmark/bench``.  It expects a file to read from, followed by a string and a distance.

### What is measured:
1. Building a BK Tree based off of each word presented in the file.
2. Searching that tree for all strings which are within a distance of the given string.

Example Command:

    // Search for all words within 2 Levenshtein distance of "foo" from the file "/path/to/strings.txt"
    $ ./bench /path/to/strings.txt foo 2

### How to structure the file:
Simply create a file of words, with each word on a new line.

Example:

    foo
    bar
    baz
    //EOF

### Benchmark Resources
I used a pre-created list of usernames to test and benchmark the implementation.
This list can be found here:
https://github.com/maryrosecook/commonusernames

## Thank You
This implementation was adapted from a C# implementation by Jerry Bachmann, found
in an article on his blog using a BK Tree for spell checking.

Here's the link to the blog article:
https://nullwords.wordpress.com/2013/03/13/the-bk-tree-a-data-structure-for-spell-checking/

Thank You Mary Rose Cook for supplying a long list of common usernames.

## Maintainers
* Brian C. Mackintosh
