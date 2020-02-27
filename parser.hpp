#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;


#ifndef H_PARSER


// Maximum number of tags
#define NTAGS 50

// Define the tags enumertation according to their order of precedence (eg. h2 must come before h1, since a match with ### must be found before a match with ## is, otherwise the shortest common prefixes would always match)
typedef enum {TAG_START, TAG_H6, TAG_H5, TAG_H4, TAG_H3, TAG_H2, TAG_H1, TAG_H0, TAG_BOLD_ITALIC, TAG_BOLD, TAG_ITALIC, TAG_U, TAG_CLOSE, TAG_NOTE, TAG_DEF, TAG_EXAMPLE, TAG_THEOREM, TAG_PROOF, TAG_CODE_INLINE, TAG_CODE_BLOCK, TAG_LATEX_MULTILINE, TAG_LATEX_INLINE, TAG_IMG, TAG_CENTER, TAG_UL, TAG_OL, TAG_CHECK, TAG_BLUE, TAG_END} tag;


// Initialize the tags with the corresponding string for each tag id
void init_tags();


// Check if a tag t matches at the current position pos of the line string s
bool check_tag(string s, int pos, tag t);


// Check if a line corresponds to a ul tag, ie. starts with '-' (ignoring preceding spaces/tabs)
bool check_tag_ul(string s);


// Check if a line corresponds to a ol tag, ie. starts with "1. " or any other non-negative integer (it can also be something like "0.1.7. ") (ignoring preceding spaces/tabs)
bool check_tag_ol(string s);


// Check if a string is empty or only consists of spaces, tabs, etc.
bool string_is_empty(string s);


// Print the HTML equivalent of a tag
void print_html_tag(tag t, ofstream &file_html_output);


// Parser entry point
void parse(string path_program, string path_html_template, string path_html_output);


#define H_PARSER
#endif