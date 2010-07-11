#ifndef TEST_H
#define TEST_H

#include <QString>
#include "common.h"
#include "ATMProgressIFC.h"

//careful just one of the below precompiler definitions should be left uncommented
//if more than one is defined than HADITH takes priority then WORD_SARF then AUGMENT_DICTIONARY
//#define AUGMENT_DICTIONARY
//#define HADITH
//#define WORD_SARF

//starting point
int start(QString input_str, QString &output_str, QString &error_str, QString & hadith_str, bool hadith,ATMProgressIFC *m_ui);

#endif // TEST_H