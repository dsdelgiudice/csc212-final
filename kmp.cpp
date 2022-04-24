//kmp.cpp
#include "kmp.h"

KMP::KMP(std::string txt, std::string pat, unsigned int txt_length, unsigned int pat_length, std::vector<std::string> triggers) {
    this->text = txt;
    this->pattern = pat;
    this->text_length = txt_length;
    this->pattern_length = pat_length;
    trigger_words = triggers;
}

//Set up char vector of pattern
std::vector<char> KMP::Setup_Pattern(std::string pattern) {
    std::vector<char> pattern_chars(pattern.begin(), pattern.end());
    return pattern_chars;
}

//Set up char vector of text
std::vector<char> KMP::Setup_Text(std::string text) {
    std::vector<char> text_chars(text.begin(), text.end());
    return text_chars;
}

std::vector<int> KMP::Setup_PreSuff_Table(std::vector<char>* pat_vec, unsigned int pat_size) {

    //Vector for prefixes declared
    std::vector<int> prefix_vec(pat_size);

    //Set initial position as 0
    prefix_vec.at(0) = 0;

    //Declare indexes, i for text, j for pattern
    size_t i = 0;
    size_t j = 1;

    //While loop looks for matching prefixes in the pattern
    while (j < pat_size) {
        if (pat_vec->at(i) == pat_vec->at(j)) {
            prefix_vec.at(j) = i + 1;
            i++;
            j++;
        }
        else {
            if (i != 0) {
                i = prefix_vec.at(i - 1);
            }
            else {
                prefix_vec.at(j) = 0;
                j++;
            }
        }
    }

    return prefix_vec;
}

void KMP::Search(std::string txt, std::string pat) {

    //Declare character vector for pattern
    std::vector<char> pat_vec = Setup_Pattern(pat);

    //Declare character vector for full text
    std::vector<char> txt_vec = Setup_Text(txt);

    //Boolean for flagging if the pattern has been found
    bool is_found = false;

    //Array for prefix and suffixes
    std::vector<int> prefix = Setup_PreSuff_Table(&pat_vec, pattern_length);

    //Declare unsigned int indexes
    size_t txt_idx = 0;
    size_t pat_idx = 0;

    //While loop increments the text index for each character match
    while (txt_idx < text_length) {

        //If a character match is found
        if (txt_vec.at(txt_idx) == pat_vec.at(pat_idx)) {
            txt_idx++;
            pat_idx++;
        }
        /*If all characters match, if the pattern index has reached the end of the pattern
        as character comparisons occur (left -> right), contradicting Boyer-Moore comparing
        characters in the opposite direction (right -> left)*/
        if (pat_idx == pattern_length) {

            //If the index of the pattern equals the length of the pattern, this indicates that the pattern is found.
            is_found = true;

            //Increment the amount of times the pattern has been found; is returned to the function call.
            times_found++;

            //Following for loop executes in an attempt to match the pattern with any red flag word from the string vector
            for (size_t i = 0; i < trigger_words.size(); i++) {

                //If the pattern matches a word in the string vector
                if (trigger_words.at(i) == pattern) {

                    //Set boolean to true
                    red_flag = true;

                    //Additional notification to display if the boolean is true during execution of search
                    std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
                    std::cout << "A Red-Flag word has been detected! \nConsider this post dangerous!\nContact Local Law Enforcement!" << std::endl;
                    std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
                    std::cout << std::endl;
                }

            }

            //Set next shift
            pat_idx = prefix[pat_idx - 1];
        }
        //If the characters do not match
        if (!(txt_vec[txt_idx] == pat_vec[pat_idx])) {
            if (pat_idx != 0) {
                //Set next shift
                pat_idx = prefix[pat_idx - 1];
            }
            else {
                //Increment index of text
                txt_idx++;
            }
        }
    }

    //Has the pattern been found in the text?
    if (is_found) {
        std::cout << "Pattern has been found in the text." << std::endl;
    }
    else {
        std::cout << "Pattern is not in the text." << std::endl;
    }
}

//Getter for times_found, displays in output
unsigned int KMP::getTimesFound() {
    return times_found;
}

//Getter for length of text, displays in output
unsigned int KMP::getTextLength() {
    return text_length;
}

//Getter for length of pattern, displays in output
unsigned int KMP::getPatternLength() {
    return pattern_length;
}
