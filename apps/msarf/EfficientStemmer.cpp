#include "EfficientStemmer.h"


/**
 * This is the constructor of the EfficientStemmer class
 * @param text This is the input text to be processed	
 */

EfficientStemmer::EfficientStemmer(QString *text) : Stemmer(text,0)
{
    this->text = *text;
};

/**
  * This is the function called by the tool upon finding a possible solution for the input.
  * By implementing this method, we control the accepted solutions that
  * meets our functionality required.
  * @return This function returns a boolean that is processed by the tool,
  * as long as the returned value is true the tool continues to find the next possible
  * solution till all possible ones and extracted, else if the returned value is false 
  * it will stop the analysis process.
  */
bool EfficientStemmer::on_match(){
	Solution solution;

	int prefix_pos = Prefix->info.start;
	int sub_prefix_pos;

	int suffix_pos = Suffix->info.start;
	int sub_suffix_pos;

	Morphological_Info temp;

	// Get Prefix Info
	for (int i = 0; i < prefix_infos->size(); i++) {
		
		minimal_item_info &pre = (*prefix_infos)[i];
		sub_prefix_pos = Prefix->sub_positionsOFCurrentMatch[i];

		temp.type = PREFIX;
		temp.part_of_speech = pre.POS.toUtf8().constData();
		temp.text = info.text->mid(prefix_pos, sub_prefix_pos - prefix_pos + 1).toUtf8().constData();
                temp.diacritic_text = pre.raw_data.toUtf8().constData();
		temp.description = pre.description().toUtf8().constData();
	
		solution.Prefix_infos.push_back(temp);
        	prefix_pos = sub_prefix_pos + 1;

		// Reset Morphological Info Temp Block 
		temp = Morphological_Info();
	}	

	// Get Stem Info
	temp.type = STEM;
	temp.part_of_speech =  stem_info->POS.toUtf8().constData();
	temp.text = info.text->mid(Stem->info.start, Stem->info.finish - Stem->info.start + 1).toUtf8().constData();
	temp.diacritic_text = stem_info->raw_data.toUtf8().constData();
	temp.description = stem_info->description().toUtf8().constData();

	solution.Stem_info = temp;
	
	// Reset Morphological Info Temp Block 
	temp = Morphological_Info();

	//Get Suffix Info
	for(int i=0; i < suffix_infos->size(); i++){
		minimal_item_info &suff = (*suffix_infos)[i];
		sub_suffix_pos = Suffix->sub_positionsOFCurrentMatch[i];

		temp.type = SUFFIX;
		temp.part_of_speech = suff.POS.toUtf8().constData();
		temp.text = info.text->mid(suffix_pos, sub_suffix_pos - suffix_pos + 1).toUtf8().constData();
		temp.diacritic_text = suff.raw_data.toUtf8().constData();
		temp.description = suff.description().toUtf8().constData();

		solution.Suffix_infos.push_back(temp);
		suffix_pos = sub_suffix_pos + 1;

		// Reset Morphological Info Temp Block 
		temp = Morphological_Info();
	}

	assert(suffix_pos - 1 == Suffix->info.finish); //TODO Is this necessary?

	// Add the matched solution to the solutions list of Efficient Stemmer
	this->solutions.push_back(solution);

    	return true;
};
