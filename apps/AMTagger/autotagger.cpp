#include "autotagger.h"
#include "global.h"

AutoTagger::AutoTagger(QString *text, QHash<QString, QSet<QString> > *synSetHash) {
    this->text = text;
    this->synSetHash = synSetHash;
}

bool AutoTagger::operator ()() {
    int start = 0;
    int wordCount = 1;
    while(start != text->count()) {
        Word word = nextWord(*text, start);
        if(word.word.isEmpty()) {
            break;
        }
        int length = word.end - word.start + 1;
        //excludedNegationFormula
        QSet<QString> eNF;
        //includedNegationFormula
        QHash<QString, QString> iNF;

        int tagCount = _atagger->tagHash->size();
        QString diacriticEmptyWord = removeDiacritics(word.word);
        //&(word.word)
        SarfTag sarftag(word.start, length, &diacriticEmptyWord, synSetHash, &eNF, &iNF);
        sarftag();

        /** Clean tags to fix the negation **/

        QHashIterator<QString,QString> iNFIterator(iNF);
        while(iNFIterator.hasNext()) {
            iNFIterator.next();
            QString tag = iNFIterator.value();
            bool add = true;
            /*
            for( int z=0; z<(_atagger->tagVector.count()); z++) {
                const Tag * tag2 = &(_atagger->tagVector.at(z));
                if((tag2->pos == word.start) && (tag2->length == length) && (tag2->tagtype->name == tag)) {
                    add = false;
                    break;
                }
            }
            */
            QList<Tag*> values = _atagger->tagHash->values(wordCount);
            /// This is just to check if anything goes wrong with wordIndex/wordPOS
            if(!(values.isEmpty()) && values[0]->pos != word.start) {
                _atagger->tagHash->clear();
                return false;
            }
            for(int i=0; i<values.count(); i++) {
                if(values[i]->tagtype->name == tag) {
                    add = false;
                    break;
                }
            }
            if(add) {
                const TagType* type = NULL;
                for(int i=0;i<_atagger->tagTypeVector->count(); i++) {
                    if(_atagger->tagTypeVector->at(i)->name == tag) {
                        type = _atagger->tagTypeVector->at(i);
                        break;
                    }
                }
                _atagger->insertTag(type,word.start,length,wordCount,sarf,original);
            }
        }
        /** End of cleaning **/

        if(_atagger->tagHash->size() == tagCount) {
            /// word didn't have any morphological analysis
            bool ok;
            word.word.toLong(&ok);
            if(ok) {
                for(int i=0; i< (_atagger->tagTypeVector->count()); i++) {
                    if(_atagger->tagTypeVector->at(i)->source != sarf) {
                        continue;
                    }
                    SarfTagType* stt = static_cast<SarfTagType *>(_atagger->tagTypeVector->at(i));
                    for(int j=0; j < (stt->tags.count()); j++) {
                        bool add = false;
                        const Quadruple< QString , QString , QString , QString > * aTerm = &(stt->tags.at(j));

                        if(aTerm->third == "" && aTerm->second.toInt() == 578) {
                            add = true;
                            QList<Tag*> values = _atagger->tagHash->values(wordCount);
                            for(int k=0; k<values.count(); k++) {
                                Tag* t = values[k];
                                if(t->tagtype->name == stt->name) {
                                    add = false;
                                    break;
                                }
                            }

                            if(add) {
                                _atagger->insertTag(stt,word.start,length,wordCount,sarf,original);
                            }
                        }
                        else if(aTerm->third == "NOT" && aTerm->second.toInt() != 578) {
                            add = true;
                            QList<Tag*> values = _atagger->tagHash->values(wordCount);
                            for(int k=0; k<values.count(); k++) {
                                Tag* t = values[k];
                                if(t->tagtype->name == stt->name) {
                                    add = false;
                                    break;
                                }
                            }

                            if(add) {
                                _atagger->insertTag(stt,word.start,length,wordCount,sarf,original);
                            }
                        }
                    }
                }
            }
        }

        start = word.end + 1;
        wordCount = wordCount + 1;
    }

    return true;
}
