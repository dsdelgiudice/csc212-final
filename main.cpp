//main.cpp

/*CSC212 Data Structures and Abstractions Final Project Spring 2022. Group members:
* David DelGiudice
* David Lancellotti
* Leo Ampuero
*/

//main.cpp

#include <iostream>
#include<string>
#include <limits>
#include <fstream>
#include <ctime>
#include <vector>
#include <cctype>
#include "boyermoore.h"
#include "kmp.h"

/*Global variables*/
//Line for temporarily storing string data from .txt file
std::string line = "";

//Word data when inserting or deleting trigger word from database / vector
std::string word = "";

//String to hold all text data from a file.
std::string full_text = "";

//String to hold pattern when string-searching
std::string pattern = "";

//String to hold a completely lowercased text for string-searching
std::string lowercase_text = "";

//String to hold lowercased text for string-searching
std::string lowercase_pattern = "";

//Unsigned int to determine which algorithm to run in the Perform_Search function
size_t algorithm = 0;

//File stream for retrieving data from .txt file
std::ifstream file;

//String for file name when retrieving .txt file
std::string fname;

//Lowercase input for inserting words into database
std::string lowercase_input = "";

//Vector of strings for holding red flag / trigger words
std::vector<std::string> red_flags = { "kill", "threat", "weapon", "gun", "knife" };

//Boolean for keeping the program running
bool running = true;

//Unsigned int for length of pattern
size_t pattern_length;

//Unsigned int for length of text
size_t text_length;

std::string Lowercase_Input(std::string ipt) {

    for (size_t i = 0; i < ipt.length(); i++) {
        ipt.at(i) = tolower(ipt.at(i));
    }

    return ipt;
}

//Function to read text from a file
void Read_File() {
    system("Color 60");

    //Boolean for determining if a file is found
    bool found = false;

    std::cout << "\nEnter file name to read from. Input !cancel to abort action." << std::endl;
    std::cin >> fname;

    //File read action can be canceled if !cancel is inputted
    if (fname != "!cancel") {
        file.open(fname);

        while (!found) {
            if (fname == "!cancel") {
                std::cout << "Action cancelled.\nSelect an action to perform." << std::endl;
                fname.clear();
                break;
            }
            if (file.fail()) {
                //If the name of the specified file is non-existent, user will be prompted to enter a valid name of a txt file. Action can still be canceled
                std::cout << "File with that name not found. Please enter the name of an existing .txt file." << std::endl;
                fname.clear();
                std::cin.clear();
                std::cin >> fname;
                file.open(fname);
            }
            else {
                //This is essential if a file was previously loaded while running. This will overwrite the string data so different string searches can be performed.
                found = true;
                if (!full_text.empty()) {
                    full_text.clear();
                    lowercase_text.clear();
                }

                //Retrieves data and appends to the full_text string
                while (std::getline(file, line)) {
                    full_text += line;
                }

                //Confirmation message and closes the file
                std::cout << "File successfully opened and post successfully recorded." << std::endl;
                lowercase_text = Lowercase_Input(full_text);
                file.close();
            }
        }
    }
    //If input is !cancel
    else {
        std::cout << "File Read aborted." << std::endl;
    }
}

void Perform_Search() {
    system("Color 57");

    //Boolean for confirming if the search can be run
    bool confirmed = false;

    //Sets length of text
    text_length = full_text.length();

    //Prompt for pattern to search
    std::cout << "Enter a pattern to search for. Input !cancel to abort. " << std::endl;
    std::cin >> pattern;

    //Sets length of pattern
    pattern_length = pattern.length();

    lowercase_pattern = Lowercase_Input(pattern);

    //Below while loop is necessary to prevent exceptions if the length of the pattern is longer than the length of the full text. User will be prompted for a smaller pattern.
    while (pattern_length > text_length) {

        std::cout << "Pattern length is larger than text length. Search cannot be performed." << std::endl;
        std::cout << "Please enter a pattern with a lower length than the text." << std::endl;
        std::cin.clear();
        std::cin >> pattern;
        pattern_length = pattern.length();
    }
    //If input is not to abort action
    if (pattern != "!cancel") {
        pattern_length = pattern.length();

        //Prompt for which algorithm to run, checks for valid input.
        std::cout << "Which algorithm is desired for the search? 1 for Boyer-Moore, or 2 for Knuth-Morris-Pratt." << std::endl;
        if (std::cin >> algorithm) {
            while (!confirmed) {
                if (algorithm < 1 || algorithm > 2) {
                    std::cout << "Invalid command entered. Enter 1 or 2." << std::endl;
                    std::cin.clear();
                    std::cin >> algorithm;
                }
                else {
                    //Correct input 1 or 2
                    confirmed = true;

                    //Boyer-Moore search
                    if (algorithm == 1) {
                        
                        //Class instantiation
                        BoyerMoore bmsearch(lowercase_text, lowercase_pattern, text_length, pattern_length, red_flags);

                        //Clock time before execution
                        unsigned long c_start = std::clock();

                        //Execute string-search
                        bmsearch.Search(lowercase_text, lowercase_pattern);

                        //Clock time after execution
                        unsigned long c_end = std::clock();

                        //Total runtime
                        float elapsed_time = 1.0 * (c_end - c_start) / CLOCKS_PER_SEC;

                        //Output displays algorithm run, total runtime, times the pattern has been found, and the length of both the text and pattern
                        std::cout << "Boyer-Moore Search Statistics: " << std::endl;
                        std::cout << "Total Runtime: " << elapsed_time << " seconds" << std::endl;
                        std::cout << "Length of text: " << bmsearch.getTextLength() << std::endl;
                        std::cout << "Length of pattern: " << bmsearch.getPatternLength() << std::endl;
                        std::cout << "Times found: " << bmsearch.getTimesFound() << std::endl;
                    }

                    //Knuth-Morris-Pratt search
                    else if (algorithm == 2) {

                        //Class instantiation
                        KMP kmpsearch(lowercase_text, lowercase_pattern, text_length, pattern_length, red_flags);

                        //Clock time before execution
                        unsigned long start = std::clock();

                        //Execute string-search
                        kmpsearch.Search(lowercase_text, lowercase_pattern);

                        //Clock time after execution
                        unsigned long end = std::clock();

                        //Total runtime
                        float elapsed_time = 1.0 * (end - start) / CLOCKS_PER_SEC;

                        //Output displays algorithm run, total runtime, times the pattern has been found, and the length of both the text and pattern
                        std::cout << "Knuth-Morris-Pratt Search Statistics:" << std::endl;
                        std::cout << "Total Runtime: " << elapsed_time << " seconds" << std::endl;
                        std::cout << "Length of text: " << kmpsearch.getTextLength() << std::endl;
                        std::cout << "Length of pattern: " << kmpsearch.getPatternLength() << std::endl;
                        std::cout << "Times found: " << kmpsearch.getTimesFound() << std::endl;
                    }
                }
            }

        }
        else {
            //Incorrect input for string-search algorithm to be run
            std::cout << "Invalid input entered. Please enter an integer between 1 and 2." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    else {
        //Input is !cancel
        std::cout << "String Search aborted." << std::endl;
    }
}

//Function to insert word into string vector
void Insert() {
    system("Color 90");
    bool in_database = false;
    std::cout << "Enter word to insert into database. Input !cancel to abort action. " << std::endl;
    std::cin >> word;
    if (word != "!cancel") {
        //Traverses through the vector to check for duplicates, capital letters are omitted and treated as lowercase letters in input
        for (unsigned int i = 0; i < red_flags.size(); i++) {
            //Word via input will not be inserted if already in vector
            if (red_flags[i] == word) {
                in_database = true;
                std::cout << "Inputted word is already in database." << std::endl;
                break;
            }
        }

        //If the word is not in the string vector
        if (!in_database) {
            lowercase_input = Lowercase_Input(word);

            //Insert new word
            red_flags.push_back(lowercase_input);

            //Prompt notification indicating successful execution
            std::cout << "New Red Flag word successfully inserted into database." << std::endl;
            lowercase_input.clear();
        }
    }

    //!cancel as input
    else {
        std::cout << "Insertion aborted" << std::endl;
    }
}

//Function to delete existing word from string vector
void Delete() {
    system("Color 40");
    bool removed = false;

    //Action cannot be done if the database is already empty
    if (red_flags.empty()) {
        std::cout << "Red Flag database is empty." << std::endl;
    }

    //Prompt to remove data from string vector
    else {
        std::cout << "Enter word to remove: " << std::endl;
        std::cin >> word;
        lowercase_input = Lowercase_Input(word);

        //If input is not to abort action
        if (word != "!cancel") {

            //Traverses through the string vector, erases the word inputted if found
            for (unsigned int i = 0; i < red_flags.size(); i++) {
                if (red_flags[i] == lowercase_input) {

                    //Erases the word
                    red_flags.erase(red_flags.begin() + i);
                    removed = true;

                    //Prompt notification indicating successful execution
                    std::cout << "Word has been successfully removed." << std::endl;
                }
            }

            //If word via input is not in the string vector
            if (!removed) {
                std::cout << "Inputted word was not found in database." << std::endl;
            }
        }

        //Input is !cancel
        else {
            std::cout << "Deletion aborted." << std::endl;
        }
    }
    lowercase_input.clear();
}

//Function to execute a certain function based on input
void Select_Action(unsigned int act) {

    //Switch statement to indicate cases
    switch (act) {

    //1: Read text from input file
    case 1:
        std::cout << "Read from a file." << std::endl;
        Read_File();
        break;

    //2: Peform string-search, cannot be done if text data was not previously loaded
    case 2:
        if (full_text.empty()) {
            std::cout << "No post from a file is loaded. Please load in a .txt file first before performing a string-search." << std::endl;
            break;
        }
        else {
            std::cout << "Post is currently open." << std::endl;
            Perform_Search();
            break;
        }

    //3: Insert new word into string vector
    case 3:
        Insert();
        break;

    //4: Delete word from string vector
    case 4:
        Delete();
        break;

    //5: View all trigger words in string vector
    case 5:
        std::cout << "All Red Flag Words in Database: " << std::endl;
        for (unsigned int i = 0; i < red_flags.size(); i++) {

            std::cout << red_flags[i] + ", ";
        }
        std::cout << std::endl;
        break;

    //6: View text loaded
    case 6:
        if (!full_text.empty()) {
            std::cout << full_text << std::endl;
        }
        else {
            std::cout << "No post is currently loaded. Please load in a .txt file first." << std::endl;
            std::cout << std::endl;
        }
        break;

    //7: Exit the program
    case 7:
        system("Color 07");
        system("cls");
        std::cout << "Program has been terminated." << std::endl;
        exit(0);
    }
}

//Driver function
int main()
{
    system("Color 1F");

    //Opening prompt
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "CSC212 Final Project Spring 2022, Group members include David DelGiudice, David Lancellotti, and Leo Ampuero" << std::endl;
    std::cout << "Program: String Search Emulator that local law enforcement would utilize to detect potential school shooters from mock social media posts in the form of .txt files." << std::endl;
    std::cout << "Program is to have certain words trigger an alert in the program where otherwise would function as normal with string searching." << std::endl;
    std::cout << "Additional functionality exists for a menu-like application, allowing editing the red flag database, display the loaded post, and exiting the app via user input" << std::endl;
    std::cout << std::endl;

    //Unsigned int for action
    unsigned int action;

    //Commands prompt
    std::cout << "Select an action to perform. \n1 - Read text from a file \n2 - Perform String Search" <<
        "\n3 - Add word to Red Flag List\n4 - Remove word from Red Flag List\n5 - View all Red Flag words in list\n6 - Display Post\n7 - Exit" << std::endl;
    while (running) {
        system("Color 1F");
        std::cout << "\nAction: ";
        if (std::cin >> action) {
            //If input is of type int but not within range
            if (action < 1 || action > 7) {
                std::cout << "Invalid command entered. Please enter an integer between 1 and 7." << std::endl;
                std::cin.clear();
            }
            else {
                //std::cout << "Command accepted." << std::endl;
                Select_Action(action);
            }
        }

        //If text data is inputted
        else {
            std::cout << "Invalid input entered. Please enter an integer between 1 and 7." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}
