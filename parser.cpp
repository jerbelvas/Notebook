#include "parser.hpp"
using namespace std;



// Correspondence between a tag id and its string representation
string tag_string[NTAGS];

// Stack of tags for maintaining (possibly nested) open/close tags
stack<tag> st;


// Initialize the tags with the corresponding string for each tag id
void init_tags() {
    tag_string[TAG_H6] = "#######";
    tag_string[TAG_H5] = "######";
    tag_string[TAG_H4] = "#####";
    tag_string[TAG_H3] = "####";
    tag_string[TAG_H2] = "###";
    tag_string[TAG_H1] = "##";
    tag_string[TAG_H0] = "#";
    tag_string[TAG_BOLD_ITALIC] = "***";
    tag_string[TAG_BOLD] = "**";
    tag_string[TAG_ITALIC] = "*";
    tag_string[TAG_U] = "_";
    tag_string[TAG_NOTE] = "\\note{{";
    tag_string[TAG_DEF] = "\\def{{";
    tag_string[TAG_EXAMPLE] = "\\example{{";
    tag_string[TAG_THEOREM] = "\\theorem{{";
    tag_string[TAG_PROOF] = "\\proof{{";
    tag_string[TAG_CODE_INLINE] = "\\code{{";
    tag_string[TAG_CODE_BLOCK] = "\\codeblock{{";
    tag_string[TAG_CLOSE] = "}}";
    tag_string[TAG_LATEX_MULTILINE] = "$$";
    tag_string[TAG_LATEX_INLINE] = "$";
    tag_string[TAG_IMG] = "\\img{{";
    tag_string[TAG_CENTER] = "\\center{{";
    tag_string[TAG_UL] = "- ";
    tag_string[TAG_OL] = "- "; // NOTE: The ol tag will not be matched with this string, but this still has to be defined as a not empty string, otherwise it would always match and cause an infinite loop with every token; is defined as the previous one because if the previous one matches, then this gets skipped
    tag_string[TAG_CHECK] = "\\check";
    tag_string[TAG_BLUE] = "\\blue{{";
}


// Check if a tag t matches at the current position pos of the line string s
bool check_tag(string s, int pos, tag t) {
    return (pos+tag_string[t].size()-1 <= s.size() && s.substr(pos, tag_string[t].size()) == tag_string[t]);
}


// Check if a line corresponds to a ul tag, ie. starts with "- " (ignoring preceding spaces/tabs)
bool check_tag_ul(string s) {
    stringstream ss(s);
    string l;
    ss >> l;
    return l == "-";
}


// Check if a line corresponds to a ol tag, ie. starts with "1. " or any other non-negative integer (it can also be something like "0.1.7. ") (ignoring preceding spaces/tabs)
bool check_tag_ol(string s) {
    stringstream ss(s);
    string l;
    ss >> l;

    bool valid = true;
    for (size_t i = 0; i < l.size()-1; i++)
        if (!isdigit(l[i]) && l[i] != '.') valid = false;
    
    return (valid && l[l.size()-1] == '.');
}


// Check if a string is empty or only consists of spaces, tabs, etc.
bool string_is_empty(string s) {
    stringstream ss(s);
    string l;
    ss >> l;
    return l == "";
}


// Print the HTML equivalent of a tag
void print_html_tag(tag t, ofstream &file_html_output) {
    switch (t) {
        case TAG_H6: file_html_output << "<h6>"; break;
        case TAG_H5: file_html_output << "<h5>"; break;
        case TAG_H4: file_html_output << "<h4>"; break;
        case TAG_H3: file_html_output << "<h3>"; break;
        case TAG_H2: file_html_output << "<h2>"; break;
        case TAG_H1: file_html_output << "<h1>"; break;
        case TAG_H0: file_html_output << "<center><div class='h0'>"; break;
        case TAG_BOLD_ITALIC: file_html_output << "<strong><em>"; break;
        case TAG_BOLD: file_html_output << "<strong>"; break;
        case TAG_ITALIC: file_html_output << "<em>"; break;
        case TAG_U: file_html_output << "<u>"; break;
        case TAG_NOTE: file_html_output << "<div class='box annotation'>➤ &nbsp; "; break;
        case TAG_DEF: file_html_output << "<div class='box definition'><div style=\"padding-bottom: 5px; font-size: 1.05em; font-family: 'CMU Sans Serif'; color: #23ac9e;\"><b>Definição</b></div>"; break;
        case TAG_EXAMPLE: file_html_output << "<div class='box example'><div style=\"padding-bottom: 5px; font-size: 1.05em; font-family: 'CMU Sans Serif'; color: #9e654f;\"><b>Exemplo</b></div>"; break;
        case TAG_THEOREM: file_html_output << "<div class='box theorem'><div style=\"padding-bottom: 5px; font-size: 1.05em; font-family: 'CMU Sans Serif'; color: #34367e;\"><b>Teorema</b></div>"; break;
        case TAG_PROOF: file_html_output << "<div class='box proof'><div style=\"padding-bottom: 5px; font-size: 1.05em; font-family: 'CMU Sans Serif'; font-style: italic;\"><b>Prova</b></div>"; break;
        case TAG_CODE_INLINE: file_html_output << "<code class='language-cpp'>"; break;
        case TAG_CODE_BLOCK: file_html_output << "<pre><code class='language-cpp'>"; break;
        case TAG_LATEX_MULTILINE: file_html_output << "$$"; break;
        case TAG_LATEX_INLINE: file_html_output << "$"; break;
        case TAG_IMG: file_html_output << "<img "; break;
        case TAG_CENTER: file_html_output << "<center>"; break;
        case TAG_CHECK: file_html_output << "<span class='check'>✓</span>"; break;
        case TAG_BLUE: file_html_output << "<span class='blue'>"; break;
        default:;
    }
}


// Parser entry point
void parse(string path_program, string path_html_template, string path_html_output) {
    // Open file streams
    ifstream file_program(path_program);
    ifstream file_html_template(path_html_template);
    ofstream file_html_output(path_html_output);
    if (!file_program.is_open()) {
        cerr << "Error: couldn't open " << path_program << "\n";
        exit(EXIT_FAILURE);
    }
    if (!file_html_template.is_open()) {
        cerr << "Error: couldn't open " << path_html_template << "\n";
        exit(EXIT_FAILURE);
    }
    if (!file_html_output.is_open()) {
        cerr << "Error: couldn't open " << path_html_output << "\n";
        exit(EXIT_FAILURE);
    }
    
    string line;
    // Copy the HTML template line by line
    while (getline(file_html_template, line)) {
        file_html_output << line << '\n';
        
        // When the sentinel line is reached, insert the parsed program there before continuing to copy the rest of the HTML template
        if (line == "<!-- MAIN TEXT -->") {

            // Read the program line by line and output the parsed version of it
            while (getline(file_program, line)) {
                // Convert an empty line to an HTML <br> tag (it requires two blank lines for an actual break to be noticeable)
                if (string_is_empty(line)) {file_html_output << "<br>\n"; continue;}

                // Check if the line starts with the ul tag
                if (check_tag_ul(line)) {
                    // Count the number of tabs before '-' so the proper indentation can be given to the list item (number of tabs * 50px)
                    int tabs = 0;
                    for (int j = 0; j+4 <= (int)line.size() && line.substr(j, 4) == "    "; tabs++, j+=4);
                    file_html_output << "<ul style='margin-inline-start: " << tabs*50 << "px;'><li>";

                    st.push(TAG_UL);
                }
                // Check if the line starts with the ol tag
                else if (check_tag_ol(line)) {
                    // Count the number of tabs before the number part so the proper indentation can be given to the list item (number of tabs * 50px)
                    int tabs = 0;
                    for (int j = 0; j+4 <= (int)line.size() && line.substr(j, 4) == "    "; tabs++, j+=4);
                    
                    // Find the number to render on the ol tag (eg. "1. ")
                    stringstream ss(line);
                    string number_part;
                    ss >> number_part;
                    getline(ss, line);
                    
                    file_html_output << "<div class='ol'  style='margin-inline-start: " << tabs*50 << "px;'><span class='ol-number'>" << number_part << "&nbsp;</span>";

                    st.push(TAG_OL);
                }
                
                size_t i = 0;
                while (i < line.size()) {
                    // Precedence rules: check the most precedent first (eg. ### has to be matched before ##, otherwise ## will match both cases)

                    // Loop through all tags and break on the first match
                    bool matched = false;
                    for (int t_iter = TAG_START+1; t_iter != TAG_END; t_iter++) {
                        tag t = static_cast<tag>(t_iter);
                        if (check_tag(line, i, t)) {

                            // Ignore the effect of any tags inside a Latex or code body
                            if (!st.empty() && (st.top() == TAG_LATEX_MULTILINE || st.top() == TAG_LATEX_INLINE || ((st.top() == TAG_CODE_INLINE || st.top() == TAG_CODE_BLOCK) && t != TAG_CLOSE))) {
                                if (st.top() == TAG_LATEX_MULTILINE && t == TAG_LATEX_MULTILINE) st.pop();
                                else if (st.top() == TAG_LATEX_INLINE && t == TAG_LATEX_INLINE) st.pop();
                            }
                            else {
                                // When the tag has no (almost) direct equivalent in HTML (eg. **text** has to be converted to <b>text</b>)

                                // Bold & italic tag
                                if (t == TAG_BOLD_ITALIC && !st.empty() && st.top() == TAG_BOLD_ITALIC) {
                                    file_html_output << "</em></strong>";
                                    st.pop();
                                }
                                // Bold tag
                                else if (t == TAG_BOLD && !st.empty() && st.top() == TAG_BOLD) {
                                    file_html_output << "</strong>";
                                    st.pop();
                                }
                                // Italic tag
                                else if (t == TAG_ITALIC && !st.empty() && st.top() == TAG_ITALIC) {
                                    file_html_output << "</em>";
                                    st.pop();
                                }
                                // Underline tag
                                else if (t == TAG_U && !st.empty() && st.top() == TAG_U) {
                                    file_html_output << "</u>";
                                    st.pop();
                                }
                                // Close tag: use the appropriate HTML closing tag for each case
                                else if (t == TAG_CLOSE && !st.empty()) {
                                    switch (st.top()) {
                                        case TAG_NOTE: case TAG_DEF: case TAG_EXAMPLE: case TAG_THEOREM: case TAG_PROOF: file_html_output << "</div>"; break;
                                        case TAG_CODE_INLINE: file_html_output << "</code>"; break;
                                        case TAG_CODE_BLOCK: file_html_output << "</code></pre>"; break;
                                        case TAG_IMG: file_html_output << ">"; break;
                                        case TAG_CENTER: file_html_output << "</center>"; break;
                                        case TAG_BLUE: file_html_output << "</span>"; break;
                                        default:;
                                    }
                                    st.pop();
                                }
                                // When a tag has an (almost) direct equivalent in HTML (eg. \center{{text}} gets converted to <center>text</center>)
                                else {
                                    print_html_tag(t, file_html_output);
                                    if (t != TAG_CHECK) // Don't add not-enclosable tags to the stack
                                        st.push(t);
                                }
                                // Advance the parser cursor by the size of the matched tag
                                i += tag_string[t].size();
                                matched = true;

                                break;
                            }
                        }
                    }

                    // If no tag has been matched, simply output the same character
                    if (!matched) {
                        // At first, the parser would escape '<' and '>' into entities, but now the user either has to use Latex or the entities &lt; and &gt;
                        /*switch (line[i]) {
                            case '<': file_html_output << "&lt;"; break;
                            case '>': file_html_output << "&gt;"; break;
                            default: file_html_output << line[i];
                        }*/
                        file_html_output << line[i];
                        i++; // Advance the parser cursor by 1 to read the next character
                    }
                }

                // At the end of the line, check if there are header tags to close (eg. ## header has to be closed in HTML: <h1> header</h1>)
                while (!st.empty() && ((TAG_H6 <= st.top() && st.top() <= TAG_H0) || st.top() == TAG_UL || st.top() == TAG_OL)) {
                    switch (st.top()) {
                        case TAG_H6: file_html_output << "</h6>"; break;
                        case TAG_H5: file_html_output << "</h5>"; break;
                        case TAG_H4: file_html_output << "</h4>"; break;
                        case TAG_H3: file_html_output << "</h3>"; break;
                        case TAG_H2: file_html_output << "</h2>"; break;
                        case TAG_H1: file_html_output << "</h1>"; break;
                        case TAG_H0: file_html_output << "</div></center>"; break;
                        case TAG_UL: file_html_output << "</li></ul>"; break;
                        case TAG_OL: file_html_output << "</div>"; break;
                        default:;
                    }
                    st.pop();
                }

                file_html_output << "\n";
            }

            // The program has been read; close it
            file_program.close();
        }
    }
    // The HTML template has been copied; close it
    file_html_template.close();
}