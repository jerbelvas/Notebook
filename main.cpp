#include <iostream>
#include "parser.hpp"
using namespace std;


int main(int argc, char** argv) {
    init_tags();

    // Argument checking
    if (argc != 3+1) {
        cerr << "Usage: " << argv[0] << " source.nb template.html output.html\n";
        exit(EXIT_FAILURE);
    }

    string path_program(argv[1]);
    string path_html_template(argv[2]);
    string path_html_output(argv[3]);
    parse(path_program, path_html_template, path_html_output);
}