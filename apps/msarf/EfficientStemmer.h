#include<stemmer.h>
#include<QTextStream>
#include <vector>
#include <string>
#include <common.h>

/**
 * @class EfficientStemmer
 * @brief This class achieve the same thing as Stemmer but it returns 
 * a data structur that includes stemming results
 * @author Ahmad Mustapha
 */

struct Morphological_Info{
	item_types type;
	string part_of_speech;
	string text;
	string diacritic_text;
	string description;
};

struct Solution
{
	vector<Morphological_Info> Prefix_infos;
	Morphological_Info Stem_info;
	vector<Morphological_Info> Suffix_infos;
};


class EfficientStemmer : public Stemmer
{
private:

    	/// String that holds the input text
    	QString text;
	
	///
	vector<Solution> solutions;
  
public:

    /**
     * This is the constructor of the EfficientStemmer class
     * @param text This is the input text to be processed
     */
    explicit EfficientStemmer(QString * text);

    /**
     * This is the function called by the tool upon finding a possible solution for the input.
     * By implementing this method, we control the accepted solutions that meet
     * our functionality required.
     * @return This function returns a boolean that is processed by the tool, as long as the returned 
     * value is true the tool continues to find the next possible
     * solution till all possible ones and extracted, else if the returned value is false it will stop
     * the analysis process.
     */
    bool on_match();
};

