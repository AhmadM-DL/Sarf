#ifndef TEST_H
#define TEST_H

#include <QString>
#include "common.h"
#include "ATMProgressIFC.h"

#define REPETITIONS 1

int morphology(QString input_str,ATMProgressIFC * prg);
int hadith(QString input_str,ATMProgressIFC * prg);
int test(QString input_str,ATMProgressIFC * prg);
int verify(QString input_str,ATMProgressIFC * prg);
int breakAffix(QString input_str, ATMProgressIFC * prg);
int timeRecognize(QString input_str, ATMProgressIFC * prg);
int biographyHelper(QString input_str,ATMProgressIFC *prg);
int genealogy(QString input_str,ATMProgressIFC *prg);
int biography(QString input_str,ATMProgressIFC *prg);
int simple_annotation(QString input_str,ATMProgressIFC *prg);
int bible_annotation(QString input_str,ATMProgressIFC *prg);
int hadith_annotation(QString input_str,ATMProgressIFC *prg);
int hadith_name_annotation(QString input_str,ATMProgressIFC *prg);
int hadith_agreement(QString input_str,ATMProgressIFC *prg);
int genealogy_agreement(QString input_str,ATMProgressIFC *prg);
int narrator_annotation(QString input_str,ATMProgressIFC *prg);
int narrator_equality_comparision(QString input_str, ATMProgressIFC *prg);

#endif 
