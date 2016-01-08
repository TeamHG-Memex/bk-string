# BK String
BK Word is a C implementation of a BK Tree for strings.

## Usage
To use, simply place an ``#include`` header for "bkmodule.c" in your file.

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

    // "list" is all words in "b" within 2 Levenshtein Distance from "foo."

## Running Tests
I didn't know if I should or should not have included compiled tests, but,
if you have gcc, simply compile the "test/test.c" file and run that.

## Thank You
This implementation was adapted from a C# implementation by Jerry Bachmann, found
in an article on his blog using a BK Tree for spell checking.

Here's the link to the blog article:
https://nullwords.wordpress.com/2013/03/13/the-bk-tree-a-data-structure-for-spell-checking/

## Maintainers
* Brian C. Mackintosh
