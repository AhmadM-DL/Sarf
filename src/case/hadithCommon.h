#ifndef HADITHCOMMON_H
#define HADITHCOMMON_H

#include <assert.h>
#include <QFile>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include "hadith.h"
#include "stemmer.h"
#include "letters.h"
#include "diacritics.h"
#include "text_handling.h"
#include "textParsing.h"
#include "common.h"
#include "Retrieve_Template.h"
#include "Search_by_item_locally.h"
#include "narrator_abstraction.h"


class BiographySegmenterAlgorithm;

/*
 * Definition of a class for the Hadith and its parameters
 */
class HadithParameters {
    public:
        unsigned int nmc_max: 5;
        unsigned int nrc_max: 5;
        unsigned int narr_min: 5;
        unsigned int equality_radius: 5;
        bool display_chain_num: 1;
        bool break_cycles: 1;
        unsigned int bio_nmc_max: 5;
        unsigned int bio_nrc_max: 15;
        unsigned int bio_narr_min: 5;
        unsigned int bio_max_reachability: 5;
        bool detailed_statistics: 1;
        unsigned int unused: 11;
        double equality_delta;
        double equality_threshold;
        double bio_threshold;
        HadithParameters() {
            nmc_max = 3;
            narr_min = 3;
            nrc_max = 5;
            bio_nmc_max = 1;
            bio_narr_min = 4;
            bio_nrc_max = 100;
            equality_radius = 3;
            equality_delta = 0.3;
            equality_threshold = 0.1;
            bio_max_reachability = 1;
            bio_threshold = 5.0;
            display_chain_num = true;
            break_cycles = true;
            detailed_statistics = true;
        }
};

extern HadithParameters hadithParameters;
extern QString chainDataStreamFileName;

extern QString chainDataStreamFileName;
extern QString preProcessedDescriptionsFileName;
extern QString PhrasesFileName;
extern QString StopwordsFileName;

enum WordType { NAME, NRC, NMC, STOP_WORD};
enum StateType { TEXT_S , NAME_S, NMC_S , NRC_S, STOP_WORD_S};
enum Structure { INITIALIZE, NAME_CONNECTOR, NARRATOR_CONNECTOR, NAME_PRIM, RASOUL_WORD, UNDETERMINED_STRUCTURE};

typedef struct  {
    long lastEndPos;
    long startPos;
    long endPos;
    long nextPos;
    Structure processedStructure: 3;
    WordType currentType: 3;
    StateType currentState: 4;
    StateType nextState: 4;
    bool familyNMC: 1;
    bool ibn: 1;
    bool _2ab: 1;
    bool _2om: 1;
    bool _3abid: 1;
    bool possessivePlace: 1;
    bool number: 1;
    bool isWaw: 1;
    bool _3an: 1;
    bool learnedName: 1;
    bool nrcIsPunctuation: 1; //if state is NRC and that is caused soley by punctuation
    bool nrcPreviousType: 1;
    int unused: 11;
    PunctuationInfo previousPunctuationInfo, currentPunctuationInfo;
    void resetCurrentWordInfo() {
        familyNMC = false;
        ibn = false;
        _2ab = false;
        _2om = false;
        _3abid = false;
        possessivePlace = false;
        number = false;
        isWaw = false;
        _3an = false;
        learnedName = false;
        currentPunctuationInfo.reset();
    }
    bool familyConnectorOr3abid() {
        return familyNMC || _3abid;
    }
    bool isFamilyConnectorOrPossessivePlace() {
        return familyNMC || possessivePlace;
    }
} StateInfo;

extern QStringList compound_words, rasoul_words, suffixNames;
extern QString alrasoul, abyi;
extern QString hadath, abid, alrasoul, abyi, _3an, _2ama, _3ama;
extern int bit_POSSESSIVE, bit_PLACE, bit_CITY, bit_COUNTRY, bit_NOUN_PROP, bit_ENARRATOR_NAMES;
extern QList<int> bits_NAME;

extern QHash<long, bool> familyNMC_descriptions;
extern QHash<long, bool> NRC_descriptions;
extern QHash<long, bool> IBN_descriptions;
extern QHash<long, bool> AB_descriptions;
extern QHash<long, bool> OM_descriptions;

extern QTextStream *myoutPtr;

typedef QList<NameConnectorPrim *> TempConnectorPrimList;

#define display_letters 30

void readFromDatabasePreProcessedHadithDescriptions();
void readFromFilePreprocessedHadithDescriptions();

void hadith_initialize();

class hadith_stemmer: public Stemmer {
        //TODO: seperate ibn from possessive from 3abid and later seperate between ibn and bin
    private:
        bool place;
    public:
        long finish_pos;
        int numSolutions;
        bool name: 1, nrc: 1, nmc, possessive: 1, familyNMC: 1, ibn: 1, _2ab: 1, _2om: 1, _3abid: 1, stopword: 1;
        bool has_waw: 1, is3an: 1;
        bool tryToLearnNames: 1, learnedName: 1;
        long startStem, finishStem, wawStart, wawEnd;
        hadith_stemmer(QString *word, int start)
            : Stemmer(word, start, false) {
            setSolutionSettings(M_ALL);
            init(start);
        }
        void init(int start) {
            this->info.start = start;
            this->info.finish = start;
            name = false;
            nmc = false;
            nrc = false;
            possessive = false;
            place = false;
            familyNMC = false;
            ibn = false;
            _2ab = false;
            _2om = false;
            _3abid = false;
            stopword = false;
            finish_pos = start;
            numSolutions = 0;
            tryToLearnNames = false;
            learnedName = false;
            is3an = false;
            startStem = start;
            finishStem = start;
            wawStart = start;
            wawEnd = start;
            has_waw = false;
        }
        bool on_match() {
            solution_position *S_inf = Stem->computeFirstSolution();

            do {
                stem_info = Stem->solution;

                if (!analyze()) {
                    return false;
                }
            } while (Stem->computeNextSolution(S_inf));

            delete S_inf;
            return true;
        }

        void checkForWaw() {
            has_waw = false;
            solution_position *p_inf = Prefix->computeFirstSolution();

            do {
                prefix_infos = &Prefix->affix_info;

                for (int i = 0; i < prefix_infos->size(); i++)
                    if (prefix_infos->at(i).POS == "wa/CONJ+") {
                        wawStart = (i == 0 ? Prefix->info.start : Prefix->getSplitPositions().at(i - 1) - 1);
                        wawEnd = Prefix->getSplitPositions().at(i) - 1;
                        has_waw = true;
                        break;
                    }
            } while (Prefix->computeNextSolution(p_inf));

            delete p_inf;
        }

        bool analyze() {
            numSolutions++;
            _3abid = (equal_ignore_diacritics(stem_info->raw_data, abid));

            if (equal_ignore_diacritics(stem_info->raw_data, hadath)) {
                nrc = true;
                finish_pos = info.finish;
                return false;
            } else if (familyNMC_descriptions.contains(stem_info->description_id())
                       && !equal_ignore_diacritics(stem_info->raw_data, _2ama) && !equal_ignore_diacritics(stem_info->raw_data, _3ama)
                      ) {
                familyNMC = true;

                if (IBN_descriptions.contains(stem_info->description_id())) {
                    ibn = true;
                }

                if (AB_descriptions.contains(stem_info->description_id())) {
                    _2ab = true;
                }

                if (OM_descriptions.contains(stem_info->description_id())) {
                    _2om = true;
                }

                finishStem = Stem->info.finish;
                startStem = Stem->info.start;
                checkForWaw();
                nmc = true;
                finish_pos = info.finish;
                return false;
            } else if (NRC_descriptions.contains(stem_info->description_id())) {
                if (equal_ignore_diacritics(stem_info->raw_data, _3an)) {
                    is3an = true;
                    startStem = Stem->info.start;
                    finishStem = Stem->info.finish;
                }

                nrc = true;
                finish_pos = info.finish;
                return false;
            }

            if (info.finish > info.start) { //more than one letter to be tested for being a name
                if (tryToLearnNames) {
                    bits_NAME.append(bit_NOUN_PROP);
                }

                int bitsNamesSize = bits_NAME.size();

                for (int i = 0; i < bitsNamesSize; i++) {
                    if (stem_info->abstract_categories.getBit(bits_NAME[i])
                        && Suffix->info.finish - Suffix->info.start < 0 &&
                        Stem->info.finish > Stem->info.start) { //i.e. has no suffix and stem > a letter
                        if (removeDiacritics(Stem->info.getString()).count() < 3) { //bit_NOUN_PROP==bits_NAME[i] &&
                            continue;
                        }

                        name = true;

                        if (info.finish > finish_pos) {
                            finish_pos = info.finish;

                            if (bit_NOUN_PROP == bits_NAME[i]) {
                                learnedName = true;
                            } else {
                                learnedName = false;
                            }

                            finishStem = Stem->info.finish;
                            startStem = Stem->info.start;
                            checkForWaw();
                        } else if (info.finish == finish_pos && bit_NOUN_PROP != bits_NAME[i]) {
                            learnedName = false;
                        }

                        if (tryToLearnNames) {
                            bits_NAME.removeLast();
                        }

                        return true;
                    }
                }

                if (tryToLearnNames) {
                    bits_NAME.removeLast();
                }

                if (stem_info->abstract_categories.getBit(bit_ENARRATOR_NAMES)) {
                    numSolutions--;
                }
            }

            if (stem_info->abstract_categories.getBit(bit_POSSESSIVE) && stem_info->abstract_categories.getBit(bit_PLACE)) {
                possessive = true;
                place = true;
                nmc = true;
                finish_pos = info.finish;
                return false;
            }

            QString c;

            foreach (c, rasoul_words) {
                int i1 = 0, i2 = Stem->info.start;

                if (checkIfSmallestIsPrefixOfLargest(c.rightRef(-1), info.text->midRef(i2), i1, i2) && i1 == c.size() - 1) {
                    int pos = i2 + Stem->info.start;

                    if (pos + 1 == info.text->size() || isDelimiter(info.text->at(pos + 1))) {
                        finish_pos = pos;
                        stopword = true;
                        return false;
                    }
                }
            }

            return true;
        }
};

inline QString choose_stem(QList<QString> stems) { //rule can be modified later
    if (stems.size() == 0) {
        return "";
    }

    QString result = stems[0];

    for (int i = 1; i < stems.size(); i++)
        if (result.length() > stems[i].length()) {
            result = stems[i];
        }

    return result;
}

inline bool isRelativeNarrator(const Narrator &n) {  //needed in equality and narrator Hash
    QString n_str = n.getString();

    if (equal(n_str, abyi)) {
        return true;
    }

    for (int i = 1; i < n.m_narrator.size(); i++) {
        QString s = n.m_narrator[i]->getString();

        for (int j = 0; j < suffixNames.size(); j++) {
            if (equal_ignore_diacritics(s, suffixNames[j])) {
                return true;
            }
        }
    }

    return false;
}

#include "bibleGeneology.h"
#include "timeManualTagger.h"
class NameLearningEvaluator {
    private:
        typedef TimeTaggerDialog::Selection Selection;
        typedef TimeTaggerDialog::SelectionList SelectionList;
    private:
        QSet<Name> nonContextNames, contextNames, annotatedNames;
        SelectionList nonContextNarrators, contextNarrators, annotatedNarrators;
        QList<Name> knownNames;
        QString fileName, * text;
        bool names: 1;
    public:
        NameLearningEvaluator(QString fileName, QString *text, bool hadith) {
            this->fileName = fileName;
            this->text = text;
            names = hadith;
            readTags();
        }
        bool equalNames(QString *, int, int, int, int); //do not use, just for use of the internal functions
        void readTags() {
            SelectionList &tags = annotatedNarrators;
            QFile file(QString("%1" + QString(names ? ".names" : ".narr")).arg(fileName).toStdString().data());

            if (file.open(QIODevice::ReadOnly)) {
                QDataStream out(&file);   // we will serialize the data into the file
                out >> tags;
                file.close();
            }

            qSort(tags.begin(), tags.end());

            if (names) {
                for (int i = 0; i < tags.size(); i++) {
                    Name n(text, tags[i].first, tags[i].second);
                    annotatedNames.insert(n);
                }

                tags.clear();
                QFile file(QString("%1.narr").arg(fileName).toStdString().data());

                if (file.open(QIODevice::ReadOnly)) {
                    QDataStream out(&file);   // we will serialize the data into the file
                    out >> tags;
                    file.close();
                }
            }
        }
        void resetLearnedNames() {
            nonContextNames.clear();
            contextNames.clear();
        }
        void addNonContextLearnedName(const Name &name) {
            if (names) {
                nonContextNames.insert(name);
            } else {
                addNonContextNarrator(name);
            }
        }
        void addNonContextNarrator(const Name &name) {
            Selection s(name.getStart(), name.getEnd());
            nonContextNarrators.append(s);
        }
        void addContextLearnedName(const Name &name) {
            if (names) {
                contextNames.insert(name);
            } else {
                addContextNarrator(name);
            }
        }
        void addContextNarrator(const Name &name) {
            Selection s(name.getStart(), name.getEnd());
            contextNarrators.append(s);
        }
        void addKnownName(const Name &name, bool learned) {
            if (names) {
                if (!learned && annotatedNames.contains(name)) {
                    annotatedNames.remove(name);
                }

                if (annotatedNames.size() == 0) { //else no need to modify the list, since will not be written
                    knownNames.append(name);
                }
            }
        }
        void displayNameLearningStatistics();
};

typedef struct StateData_ {
    long  mainStructureStartIndex;
    long narratorStartIndex, narratorEndIndex, nrcStartIndex, nrcEndIndex, nameStartIndex, nmcStartIndex;
    int  nmcCount, nrcCount, narratorCount,
         bio_nrcCount ; //bio_nrcCount needed bc dont want finding an NRC to change the tolerance breaking count

    bool nmcValid: 1;
    bool ibn_or_3abid: 1;
    bool nrcPunctuation: 1;
    int unused: 29;

    void initialize() {
        nmcCount = 0;
        narratorCount = 0;
        nrcCount = 0;
        narratorStartIndex = 0;
        narratorEndIndex = 0;
        nmcValid = false;
        ibn_or_3abid = false;
        nameStartIndex = 0;
        nmcStartIndex = 0;
        mainStructureStartIndex = 0;
        nrcStartIndex = 0;
        nrcEndIndex = 0;
        bio_nrcCount = 0;
        nrcPunctuation = false;
    }

} StateData;
class HadithData {
    public:
        QString *text;
        NarratorGraph *graph;
        bool hadith: 1, segmentNarrators: 1;
        NameLearningEvaluator learningEvaluator;

        NamePrim *namePrim;
        NameConnectorPrim *nameConnectorPrim;
        NarratorConnectorPrim *narratorConnectorPrim;
        TempConnectorPrimList temp_nameConnectors;
        Narrator *narrator;
        Chain *chain;
        Biography *biography;
        BiographySegmenterAlgorithm *segment;

        void initialize(QString *text) {
            if (namePrim != NULL) {
                delete namePrim;
                namePrim = NULL;
            }

            if (nameConnectorPrim != NULL) {
                delete nameConnectorPrim;
                nameConnectorPrim = NULL;
            }

            if (narratorConnectorPrim != NULL) {
                delete narratorConnectorPrim;
                narratorConnectorPrim = NULL;
            }

            if (narrator != NULL) {
                delete narrator;
                narrator = NULL;
            }

            for (int i = 0; i < temp_nameConnectors.size(); i++) {
                delete temp_nameConnectors[i];
            }

            temp_nameConnectors.clear();

            if (hadith) {
                if (chain != NULL) {
                    delete chain;
                }

                chain = new Chain(text);
            } else {
                int s = 0;

                if (biography != NULL) {
                    s = biography->getStart();
                    //delete biography; //TODO: check how this should be kept without resulting in memory corruption
                }

                biography = new Biography(graph, text, s);
            }
        }
        HadithData(QString *text, bool hadith, NarratorGraph *graph, QString fileName)
            : learningEvaluator(fileName, text, hadith) {
            this->text = text;
            this->hadith = hadith;
            this->graph = graph;
            namePrim = NULL;
            nameConnectorPrim = NULL;
            narrator = NULL;
            narratorConnectorPrim = NULL;
            biography = NULL;
            chain = NULL;
            segment = NULL;
        }
};


bool proceedInStateMachine(StateInfo   &stateInfo, HadithData *structures, StateData &currentData) ;

#endif
