//boyermoore.cpp

#include "boyermoore.h"

BoyerMoore::BoyerMoore(std::string txt, std::string pat, unsigned int txt_length, unsigned int pat_length, std::vector<std::string> triggers) {
    this->text = txt;
    this->pattern = pat;
    this->text_length = txt_length;
    this->pattern_length = pat_length;
    trigger_words = triggers;
}

std::vector<int> BoyerMoore::Preprocess_BC(std::string pattern) {
    //Vector for bad characters. 256 represents all ASCII values for all characters, from punctuation to numbers and letters (uppercase and lowercase)
    std::vector<int> bad_chars(256, -1);

    for (unsigned int i = 0; i < pattern.size(); i++) {
        //Store the last occurrences of all letters in the pattern
        bad_chars.at(pattern.at(i)) = i;
        //std::cout << badCharacterArray[pattern.at(i)];
    }
    return bad_chars;
}

std::vector<int> BoyerMoore::Preprocess_GS(std::string pattern) {

    size_t i = pattern_length;
    size_t j = i + 1;

    //All values in the shifts vector is initialized to 0 by default
    std::vector<int> boundary_positions(pattern_length + 1);
    std::vector<int> shifts(pattern_length + 1, 0);

    boundary_positions.at(i) = j;
    //While loop below tracks for the boundaries of the pattern
    while (i > 0) {
        //If the characters are not equal at i -1, continue searching right for the border
        while (j <= pattern_length && pattern.at(i - 1) != pattern.at(j - 1)) {

            //Preceding character of matched string is different than mismatched character
            if (shifts.at(j) == 0)
                shifts.at(j) = j - i;

            // update the position of next border
            j = boundary_positions.at(j);
        }

        //As the pattern at (i - 1) == pattern at (j - 1), the position of the border of the prefix starts from i -1 is j -1.
        i--;
        j--;
        boundary_positions.at(i) = j;
    }

    //Partial shifts are to be calculated from here
    j = boundary_positions.at(0);
    for (i = 0; i <= pattern_length; i++) {
        if (shifts.at(i) == 0) {
            shifts.at(i) = j;
        }

        if (i == j) {
            j = boundary_positions.at(j);
        }
    }
    return shifts;
}

//Main function to perform string-search
void BoyerMoore::Search(std::string text, std::string pattern) {

    //Index for text.
    int i = 0;

    //Index for pattern. 
    int j = 0;

    //Space remaining between the size of the text and the pattern
    size_t space = text_length - pattern_length;

    //Shift from bad characters
    int bad_char_s;

    //Shift from good suffix
    int good_shift_s;

    //Preprocess the Bad Characters
    std::vector<int> bad_char_vec = Preprocess_BC(pattern);

    //Preproces the Good Suffix
    std::vector<int> shifts_vec = Preprocess_GS(pattern);

    bool is_found = false;

    //Traverse the text and pattern for matches
    while (i <= space) {
        j = pattern_length - 1;

        //Pattern index decremented until either a character mismatch or full match occurs
        for (j; j >= 0; j--) {
            if (!(pattern.at(j) == text.at(i + j))) {
                break;
            }
        }

        //If the pattern has been found in the text
        if (j == -1) {
            //bool is true if the index j moves past the pattern, indicating the pattern is found.
            is_found = true;

            //Increment the amount of times found
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

            //Set shift from Bad Character Rule; condition is to prevent bugs if at the end of the text length
            //Shift from bad character rule is 1 if the index is already at the end of the text
            if (i + pattern_length < text_length) {
                bad_char_s = pattern_length - bad_char_vec.at(text.at(i + pattern_length));
            }
            else {
                bad_char_s = 1;
            }

            //Set shift from Good Suffix Rule
            good_shift_s = shifts_vec.at(0);

            //Shifts from both Bad Character and Good Suffix is compared, the larger one is added to index i.
            i += std::max(bad_char_s, good_shift_s);
        }

        //If a character mistmatch occurs
        else {
            //Set shift from Bad Character Rule
            bad_char_s = std::max(1, j - bad_char_vec.at(text.at(i + j)));

            //Set shif from Good Suffix Rule
            good_shift_s = shifts_vec.at(j + 1);

            //Shifts from both Bad Character and Good Suffix is compared, the larger one is added to index i.
            i += std::max(bad_char_s, good_shift_s);
        }

    }

    //Is the pattern found?
    if (is_found) {
        std::cout << "Pattern has been found in the text." << std::endl;
    }

    //Pattern not found
    else {
        std::cout << "Pattern is not in the text.";
    }
}

//Getter for times_found, displays in output
unsigned int BoyerMoore::getTimesFound() {
    return times_found;
}

//Getter for text_length, displays in output
unsigned int BoyerMoore::getTextLength() {
    return text_length;
}

//Getter for pattern_length, displays in output
unsigned int BoyerMoore::getPatternLength() {
    return pattern_length;
}
