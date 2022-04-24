//kmp.h

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>

class KMP
{
private:
	unsigned int text_length;
	unsigned int pattern_length;
	unsigned int times_found;
	std::string text;
	std::string pattern;
	std::string new_word;
	std::vector<std::string> trigger_words;
	bool red_flag;
	bool found;

public:
	KMP(std::string txt, std::string pat, unsigned int txt_length, unsigned int pat_length, std::vector<std::string> triggers);

	std::vector<char> Setup_Pattern(std::string pattern);
	std::vector<char> Setup_Text(std::string text);
	std::vector<int> Setup_PreSuff_Table(std::vector<char>* pat_vec, unsigned int pat_size);

	void Search(std::string txt, std::string pat);

	unsigned int getTimesFound();
	unsigned int getTextLength();
	unsigned int getPatternLength();
};

