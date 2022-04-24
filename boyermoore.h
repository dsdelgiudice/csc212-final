//boyermoore.h
#include <iostream>
#include <string>
#include <vector>

class BoyerMoore {
private:

	unsigned int text_length;
	unsigned int pattern_length;
	unsigned int times_found;
	std::string text;
	std::string pattern;
	std::vector<std::string> trigger_words;
	bool red_flag;
	bool found;

public:
	BoyerMoore(std::string txt, std::string pat, unsigned int txt_length, unsigned int pat_length, std::vector<std::string> triggers);

	std::vector<int> Preprocess_BC(std::string pattern);
	std::vector<int> Preprocess_GS(std::string pattern);
	void Search(std::string txt, std::string pat);

	unsigned int getTimesFound();
	unsigned int getTextLength();
	unsigned int getPatternLength();
};

