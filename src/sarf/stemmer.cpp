/**
 * @file   stemmer.cpp
 * @author Jad Makhlouta
 * @brief  This file implements functions of the stemmer class mainly and other classes defined in Stemmer.h
 */
#include "logger.h"
#include "Search_by_item_locally.h"
#include "text_handling.h"
#include "diacritics.h"
#include "stemmer.h"
//#include "test.h"
#include "inflections.h"

SarfParameters sarfParameters;

/**
 * This method implements the constructor of the PrefixMachine which in turn calls the PrefixSearch constructor; the class which PrefixMachine inherets from.
 * @param  controller
 * @param  start   index of the input string to be analyzed
 */
PrefixMachine::PrefixMachine(Stemmer *controller, int start): PrefixSearch(controller->info.text, start) {
    this->controller = controller;
}

/**
 * This method is called upon finding a Prefix Match
 * @return It returns true if a prefix match is found, else false
 */
bool PrefixMachine::onMatch() {
    if (controller->Stem != NULL) {
        delete controller->Stem;    //TODO: change this allocation, deallocation to clear
    }

    controller->Stem = new StemMachine(controller, position, controller->Prefix->resulting_category_idOFCurrentMatch);
    controller->Stem->setSolutionSettings(controller->multi_p);
    return (*controller->Stem)();
}

/**
 * This method implements the constructor of the StemMachine which in turn calls the StemSearch constructor; the class which StemMachine inherets from.
 * @param  controller
 * @param  start   index of the input at which the StemMachine is required to operate
 * @param  prefix_category
 */
StemMachine::StemMachine(Stemmer *controller, int start, long prefix_category): StemSearch(controller->info.text, start,
            prefix_category) {
    this->controller = controller;
}

/**
 * This method is called upon finding a Stem Match
 * @return It returns true if a prefix match is found, else false
 */
bool StemMachine::onMatch() {
    if (controller->called_everything) {
        if (controller->Suffix != NULL) {
            delete controller->Suffix;    //TODO: change this allocation, deallocation to clear
        }

        controller->Suffix = new SuffixMachine(controller, currentMatchPos + 1,
                                               controller->Prefix->resulting_category_idOFCurrentMatch, controller->Stem->category_of_currentmatch);
        controller->Suffix->setSolutionSettings(controller->multi_p);
        return (*controller->Suffix)();
    } else {
        if (!controller->on_match_helper()) {
            return false;
        }
    }

    return true;
}

/**
 * This method implements the constructor of the SuffixMachine which in turn calls the SuffixSearch constructor; the class which SuffixMachine inherets from.
 * @param  controller
 * @param  start   index of the input at which the SuffixMachine is required to operate
 * @param  prefix_category
 * @param  stem_category
 */
SuffixMachine::SuffixMachine(Stemmer *controller, int start, long prefix_category,
                             long stem_category): SuffixSearch(controller->info.text, start, prefix_category, stem_category) {
    this->controller = controller;
}

/**
 * This method is called upon finding a Suffix Match
 * @return It returns true if a prefix match is found, else false
 */
bool SuffixMachine::onMatch() {
    if (sarfParameters.enableRunonwords) {
        int index = controller->machines.size() - 1;

        if (index >= 0 && controller->machines[index].Suffix == controller->Suffix) {
            controller->Prefix = new PrefixMachine(controller, info.finish + 1);
            controller->Prefix->setSolutionSettings(controller->multi_p);
            controller->Stem = NULL;
            controller->Suffix = NULL;
            bool result = (*controller->Prefix)();
            controller->Prefix = controller->machines[index].Prefix;
            controller->Stem = controller->machines[index].Stem;
            controller->Suffix = controller->machines[index].Suffix;
            //controller->machines.removeLast();
            controller->removeLastMachines();
            return result;
        }
    }

    return controller->on_match_helper();
}


bool SuffixMachine::shouldcall_onmatch(int position) {
    //TODO:support for diacritics
    if (sarfParameters.enableRunonwords)
        if (position - controller->Prefix->info.start == 1 && controller->machines.size() > 0 &&
            (*info.text)[position - 1] != waw) {
            return false;
        }

    if (position >= info.text->length()) {
        return true;
    }

    QChar ch = info.text->at(position);

    if (isDelimiter(ch)) {
        return true;
    }

    if (sarfParameters.enableRunonwords) {
        int lastLetterIndex = getLastLetter_index(*info.text, position - 1);

        if (lastLetterIndex >= 0) {
            ch = info.text->at(lastLetterIndex);

            if (isNonConnectingLetter(ch))
                if (position - controller->Prefix->info.start > 1) {
                    controller->machines.append(SubMachines(controller->Prefix, controller->Stem, controller->Suffix));
                    return true;
                }
        }
    }

    return false;
}

void Stemmer::removeLastMachines() {
    //SubMachines last = machines.last();
    machines.removeLast();
    /*
       if (last.Prefix!=NULL)
       delete last.Prefix;
       if (last.Stem!=NULL)
       delete last.Stem;
       if (last.Suffix!=NULL)
       delete last.Suffix;
       */
}

bool Stemmer::on_match_helper() {
    // Set the finish index of the input as the finish index returned by the Suffix machine
    info.finish = Suffix->info.finish;

    // check if run on words is set
    if (sarfParameters.enableRunonwords)
        // append the solution to the SubMachines structure
    {
        machines.append(SubMachines(Prefix, Stem, Suffix));
    }

    for (int i = 0; i < machines.size(); i++) {
        runwordIndex = i;

        if (sarfParameters.enableRunonwords) {
            SubMachines &m = machines[i];
            Prefix = m.Prefix;
            Stem = m.Stem;
            Suffix = m.Suffix;
        }

        if (get_all_details) {
            solution_position *s_inf = Suffix->computeFirstSolution();

            do {
                solution_position *p_inf = Prefix->computeFirstSolution();

                do {
                    solution_position *S_inf = Stem->computeFirstSolution();

                    do {
                        suffix_infos = &Suffix->affix_info;
                        prefix_infos = &Prefix->affix_info;
                        stem_info = Stem->solution;

                        for (int j = 0; j < 2; j++) {
                            QVector<minimal_item_info> *infos = (j == 0 ? prefix_infos : suffix_infos);
                            QList<result_node *> *results = (j == 0 ? Prefix->result_nodes : Suffix->result_nodes);

                            for (int i = 0; i < results->size(); i++) {
                                QString inflectionRule = results->at(i)->getInflectionRule();
                                minimal_item_info &current = (*infos)[i];

                                if (i > 1) {
                                    minimal_item_info &previous = (*infos)[i - 1];
                                    applyInflections(inflectionRule, previous, current);
                                } else {
                                    applyInflections(inflectionRule, current);
                                }
                            }
                        }

                        int count = 0;

                        if (suffix_infos->size() > 0) { //maybe not best solution working for now
                            minimal_item_info &info = (*suffix_infos)[0];
                            QStringRef diacritics_raw_after = addlastDiacritics(-1, -1, &info.raw_data);

                            if (diacritics_raw_after.size() > 0) {
                                QStringRef diacritics_before = getDiacriticsBeforePosition(Suffix->info.start, Suffix->info.text);
                                count = 0;
                                int i = 0;
                                int s_r = diacritics_raw_after.size() - 1;
                                int s = diacritics_before.size() - 1;
                                int m = min(diacritics_raw_after.size(), diacritics_before.size());

                                while (i < m && diacritics_raw_after.at(s_r - i) == diacritics_before.at(s - i)) { //either common ending
                                    count++;
                                    i++;
                                }

                                if (count == 0) {
                                    int i = 0;

                                    while (i < m && diacritics_raw_after.at(i) == diacritics_before.at(i)) { //or common starting
                                        count++;
                                        i++;
                                    }
                                }

                                Suffix->info.start -= count;
                                Stem->info.finish -= count;
                            }
                        }

                        if (!on_match()) {
                            return false;
                        }

                        if (count > 0) {
                            Suffix->info.start += count;
                            Stem->info.finish += count;
                        }
                    } while (Stem->computeNextSolution(S_inf));

                    delete S_inf;
                } while (Prefix->computeNextSolution(p_inf));

                delete p_inf;
            } while (Suffix->computeNextSolution(s_inf));

            delete s_inf;
        } else {
            if (!on_match()) {
                return false;
            }
        }
    }

    if (sarfParameters.enableRunonwords) {
        removeLastMachines();
    }

    return true;
}

/**
 * This method is called upon finding a match for the input word
 */
bool Stemmer::on_match() {
    theSarf->out << "ALTERNATIVE:\t";
    QString word;

    for (int i = 0; i < prefix_infos->size(); i++) {
        word.append(prefix_infos->at(i).raw_data);
    }

    word.append(stem_info->raw_data);
    QString raw1 = stem_info->raw_data;
    QString POS2 = stem_info->POS;
    QString desc1 = stem_info->description();

    for (int i = 0; i < suffix_infos->size(); i++) {
        word.append(suffix_infos->at(i).raw_data);
    }

    theSarf->out << " " << word << "\n";
    //theSarf->out<<"\t("<<Prefix->info.start+1<<","<<Suffix->info.finish+1<<")\n";
    int pos = Prefix->info.start;

    for (int i = 0; i < prefix_infos->size(); i++) {
        minimal_item_info &pre = (*prefix_infos)[i];
        int pos2 = Prefix->sub_positionsOFCurrentMatch[i];

        if (pre.POS.isEmpty() && pre.raw_data.isEmpty()) {
            continue;
        }

        theSarf->out    << "PREFIX:\t"
                        << info.text->mid(pos, pos2 - pos + 1) << "\t"
                        << pre.raw_data << "\t"
                        << "\"" << pre.description() << "\"\t"
                        << pre.POS << "\n";
        pos = pos2 + 1;
    }

    pos = Stem->info.start;
    minimal_item_info &stem = *stem_info;
    int pos2 = Stem->info.finish;
    theSarf->out    << "STEM:\t"
                    << info.text->mid(pos, pos2 - pos + 1) << "\t"
                    << stem.raw_data << "\t"
                    << "\"" << stem.description() << "\"\t"
                    << stem.POS << "\n";
    pos = Suffix->info.start;

    for (int i = 0; i < suffix_infos->size(); i++) {
        minimal_item_info &suff = (*suffix_infos)[i];
        int pos2 = Suffix->sub_positionsOFCurrentMatch[i];

        if (suff.POS.isEmpty() && suff.raw_data.isEmpty()) {
            continue;
        }

        theSarf->out    << "SUFFIX:\t"
                        << info.text->mid(pos, pos2 - pos + 1) << "\t"
                        << suff.raw_data << "\t"
                        << "\"" << suff.description() << "\"\t"
                        << suff.POS << "\n";
        pos = pos2 + 1;
    }

    assert(pos - 1 == Suffix->info.finish);
    theSarf->out << "\n";
    return true;
}
