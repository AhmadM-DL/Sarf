#include "sarftag.h"
#include "global.h"
#include "amtmainwindow.h"

class AMTMainWindow;

SarfTag::SarfTag(int start, int length, QString *text,
                 QHash< QString, QSet<QString> > * synSetHash,
                 QSet<QString> *eNF,
                 QHash<QString, QString>* iNF,
                 QWidget *parent)
    : Stemmer(text,0)
{
    this->text = *text;
    this->start = start;
    this->length = length;
    this->synSetHash = synSetHash;
    this->eNF = eNF;
    this->iNF = iNF;
}

bool SarfTag::on_match() {

    for( int i=0; i< (_atagger->tagTypeVector->count()); i++) {

        QString _tag = _atagger->tagTypeVector->at(i)->tag;
        bool belong = false;

        /** Check if tag source is sarf tag types **/
        if(_atagger->tagTypeVector->at(i)->source != sarf) {
            continue;
        }

        const SarfTagType * tagtype = (SarfTagType*)(_atagger->tagTypeVector->at(i));
        for(int j=0; j < (tagtype->tags.count()); j++) {

            bool contain = false;
            const Quadruple< QString , QString , QString , QString > * tag = &(tagtype->tags.at(j));
            if(tag->first == "Prefix") {
                for(int k=0;k<prefix_infos->size();k++) {

                    minimal_item_info & pre = (*prefix_infos)[k];
                    if (pre.POS.isEmpty() && pre.raw_data.isEmpty())
                            continue;

                    if(pre.raw_data == tag->second) {
                        contain = true;
                        break;
                    }
                }
                if(contain && (tag->third.compare("NOT") != 0)) {
                    belong = true;
                    break;
                }
                else if(tag->third.compare("NOT") == 0) {
                    QString key = tagtype->tag;
                    key.append(tag->first);
                    key.append(tag->second);

                    if(!contain) {
                        if(!(eNF->contains(key))) {
                            iNF->insert(key,tagtype->tag);
                        }
                    }
                    else {
                        if(iNF->contains(key)) {
                            iNF->remove(key);
                            eNF->insert(key);
                        }
                    }
                    continue;
                }
                else {
                    continue;
                }
            }
            else if(tag->first == "Stem") {

                minimal_item_info & stem = *stem_info;

                // check if the relation is isA or contains
                bool isA = true;
                if(tag->fourth.compare("contains") == 0 ) {
                    isA = false;
                }

                if((isA && stem.raw_data == tag->second) || ((!isA) && stem.raw_data.contains(tag->second))) {
                    if(tag->third.compare("NOT") != 0) {
                        belong = true;
                        break;
                    }
                    else {
                        QString key = tagtype->tag;
                        key.append(tag->first);
                        key.append(tag->second);
                        if(iNF->contains(key)) {
                            iNF->remove(key);
                            eNF->insert(key);
                        }
                        continue;
                    }
                }
                else {
                    if(tag->third.compare("NOT") == 0) {
                        QString key = tagtype->tag;
                        key.append(tag->first);
                        key.append(tag->second);
                        if(!contain) {
                            if(!(eNF->contains(key))) {
                                iNF->insert(key,tagtype->tag);
                            }
                        }
                    }
                    continue;
                }
            }
            else if(tag->first == "Category") {

                minimal_item_info & stem = *stem_info;
                int id = tag->second.toInt();

                for(unsigned int k=0; k< stem.abstract_categories.length(); k++) {
                    if (stem.abstract_categories[k]) {
                        int abstract_id=database_info.comp_rules->getAbstractCategoryID(k);
                        if(abstract_id == id) {
                            contain = true;
                            break;
                        }
                    }
                }
                if(contain && (tag->third.compare("NOT") != 0)) {
                    belong = true;
                    break;
                }
                else if(tag->third.compare("NOT") == 0) {
                    QString key = tagtype->tag;
                    key.append(tag->first);
                    key.append(tag->second);

                    if(!contain) {
                        if(!(eNF->contains(key))) {
                            iNF->insert(key,tagtype->tag);
                        }
                    }
                    else {
                        if(iNF->contains(key)) {
                            iNF->remove(key);
                            eNF->insert(key);
                        }
                    }
                    continue;
                }
                else {
                    continue;
                }
            }
            else if(tag->first == "Suffix") {
                for(int k=0;k<suffix_infos->size();k++) {

                    minimal_item_info & suff = (*suffix_infos)[k];
                    if (suff.POS.isEmpty() && suff.raw_data.isEmpty())
                            continue;

                    if(suff.raw_data == tag->second) {
                        contain = true;
                        break;
                    }
                }
                if(contain && (tag->third.compare("NOT") != 0)) {
                    belong = true;
                    break;
                }
                else if(tag->third.compare("NOT") == 0) {
                    QString key = tagtype->tag;
                    key.append(tag->first);
                    key.append(tag->second);

                    if(!contain) {
                        if(!(eNF->contains(key))) {
                            iNF->insert(key,tagtype->tag);
                        }
                    }
                    else {
                        if(iNF->contains(key)) {
                            iNF->remove(key);
                            eNF->insert(key);
                        }
                    }
                    continue;
                }
                else {
                    continue;
                }
            }
            else if(tag->first == "Stem-POS") {

                minimal_item_info & stem = *stem_info;
                if(stem.POS.contains(tag->second,Qt::CaseSensitive)) {

                    if(tag->third.compare("NOT") != 0) {
                        belong = true;
                        break;
                    }
                    else {
                        QString key = tagtype->tag;
                        key.append(tag->first);
                        key.append(tag->second);
                        if(iNF->contains(key)) {
                            iNF->remove(key);
                            eNF->insert(key);
                        }
                        continue;
                    }
                }
                else {
                    if(tag->third.compare("NOT") == 0) {
                        QString key = tagtype->tag;
                        key.append(tag->first);
                        key.append(tag->second);
                        if(!(eNF->contains(key))) {
                            iNF->insert(key,tagtype->tag);
                        }
                    }
                    continue;
                }
            }
            else if(tag->first == "Prefix-POS") {

                for(int k=0;k<prefix_infos->size();k++) {

                    minimal_item_info & pre = (*prefix_infos)[k];
                    if (pre.POS.isEmpty() && pre.raw_data.isEmpty())
                            continue;

                    if(pre.POS.contains(tag->second,Qt::CaseSensitive)) {
                        contain = true;
                        break;
                    }
                }
                if(contain && (tag->third.compare("NOT") != 0)) {
                    belong = true;
                    break;
                }
                else if(tag->third.compare("NOT") == 0) {
                    QString key = tagtype->tag;
                    key.append(tag->first);
                    key.append(tag->second);

                    if(!contain) {
                        if(!(eNF->contains(key))) {
                            iNF->insert(key,tagtype->tag);
                        }
                    }
                    else {
                        if(iNF->contains(key)) {
                            iNF->remove(key);
                            eNF->insert(key);
                        }
                    }
                    continue;
                }
                else {
                    continue;
                }
            }
            else if(tag->first == "Suffix-POS") {

                for(int k=0;k<suffix_infos->size();k++) {

                    minimal_item_info & suff = (*suffix_infos)[k];
                    if (suff.POS.isEmpty() && suff.raw_data.isEmpty())
                            continue;

                    if(suff.POS.contains(tag->second,Qt::CaseSensitive)) {
                        contain = true;
                        break;
                    }
                }
                if(contain && (tag->third.compare("NOT") != 0)) {
                    belong = true;
                    break;
                }
                else if(tag->third.compare("NOT") == 0) {
                    QString key = tagtype->tag;
                    key.append(tag->first);
                    key.append(tag->second);

                    if(!contain) {
                        if(!(eNF->contains(key))) {
                            iNF->insert(key,tagtype->tag);
                        }
                    }
                    else {
                        if(iNF->contains(key)) {
                            iNF->remove(key);
                            eNF->insert(key);
                        }
                    }
                    continue;
                }
                else {
                    continue;
                }
            }
            else if(tag->first == "Gloss") {

                if(tag->fourth.contains("Syn")) {
                    int order = tag->fourth.mid(3).toInt();
                    QString gloss_order = tag->second;
                    gloss_order.append(QString::number(order));
                    const QSet<QString> & glossSynSet = synSetHash->value(gloss_order);

                    minimal_item_info & stem = *stem_info;
                    QStringList stem_glosses = getGloss(stem.description());
                    for(int k=0; k < stem_glosses.count(); k++) {
                        if(glossSynSet.contains(stem_glosses[k])) {
                            contain = true;
                            break;
                        }
                    }

                    if(contain && (tag->third.compare("NOT") != 0)) {
                        belong = true;
                        break;
                    }
                    else if(tag->third.compare("NOT") == 0) {
                        QString key = tagtype->tag;
                        key.append(tag->first);
                        key.append(tag->second);

                        if(!contain) {
                            if(!(eNF->contains(key))) {
                                iNF->insert(key,tagtype->tag);
                            }
                        }
                        else {
                            if(iNF->contains(key)) {
                                iNF->remove(key);
                                eNF->insert(key);
                            }
                        }
                        continue;
                    }
                    else {
                        continue;
                    }
                }
                else {
                    // Check for isA or contain relation
                    bool isA = true;
                    if(tag->fourth.compare("contains") == 0) {
                        isA = false;
                    }

                    QStringList second_glosses = getGloss(tag->second);
                    // check prefix glosses

                    for(int k=0;k<prefix_infos->size();k++) {

                        minimal_item_info & pre = (*prefix_infos)[k];
                        if (pre.POS.isEmpty() && pre.raw_data.isEmpty())
                                continue;

                        QStringList pre_glosses = getGloss(pre.description());
                        for(int n=0; n<second_glosses.count(); n++) {
                            if( isA && pre_glosses.contains(second_glosses[n])) {
                                contain = true;
                                break;
                            }
                            else if(!isA) {
                                for(int m=0; m< pre_glosses.count(); m++) {
                                    QString gloss = pre_glosses[m];
                                    if(gloss.contains(second_glosses[n])) {
                                        contain = true;
                                        break;
                                    }
                                }
                            }
                            if(contain) {
                                break;
                            }
                        }
                        if(contain) {
                            break;
                        }
                    }

                    if((contain && (tag->third.compare("NOT") != 0))) {
                        belong = true;
                        break;
                    }

                    // check stem glosses

                    minimal_item_info & stem = *stem_info;
                    QStringList stem_glosses = getGloss(stem.description());
                    //QStringList second_glosses = getGloss(tag->second);
                    for(int k=0; k<second_glosses.count(); k++) {
                        if(isA && stem_glosses.contains(second_glosses[k])) {
                            contain = true;
                            break;
                        }
                        else if(!isA) {
                            for(int m=0; m< stem_glosses.count(); m++) {
                                QString gloss = stem_glosses[m];
                                if(gloss.contains(second_glosses[k])) {
                                    contain = true;
                                    break;
                                }
                            }
                            if(contain) {
                                break;
                            }
                        }
                    }
                    if(contain && (tag->third.compare("NOT") != 0)) {
                        belong = true;
                        break;
                    }

                    // check suffix glosses

                    for(int k=0;k<suffix_infos->size();k++) {

                        minimal_item_info & suff = (*suffix_infos)[k];
                        if (suff.POS.isEmpty() && suff.raw_data.isEmpty())
                                continue;

                        QStringList suff_glosses = getGloss(suff.description());
                        for(int n=0; n<second_glosses.count(); n++) {
                            if(isA && suff_glosses.contains(second_glosses[n])) {
                                contain = true;
                                break;
                            }
                            else if(!isA) {
                                for(int m=0; m< suff_glosses.count(); m++) {
                                    QString gloss = suff_glosses[m];
                                    if(gloss.contains(second_glosses[n])) {
                                        contain = true;
                                        break;
                                    }
                                }
                            }
                            if(contain) {
                                break;
                            }
                        }
                        if(contain) {
                            break;
                        }
                    }
                    if(contain && (tag->third.compare("NOT") != 0)) {
                        belong = true;
                        break;
                    }
                    else if(tag->third.compare("NOT") == 0) {
                        QString key = tagtype->tag;
                        key.append(tag->first);
                        key.append(tag->second);

                        if(!contain) {
                            if(!(eNF->contains(key))) {
                                iNF->insert(key,tagtype->tag);
                            }
                        }
                        else {
                            if(iNF->contains(key)) {
                                iNF->remove(key);
                                eNF->insert(key);
                            }
                        }
                        continue;
                    }
                    else {
                        continue;
                    }
                }
            }
        }

        if(belong) {
            // Add tag with this tagtype
            bool add = true;
            for( int z=0; z<(_atagger->tagVector.count()); z++) {
                const Tag * tag2 = &(_atagger->tagVector.at(z));
                if((tag2->pos == start) && (tag2->length == length) && (tag2->type == _tag)) {
                    add = false;
                    break;
                }
            }
            if(add) {
                _atagger->insertTag(_tag,start,length,sarf,original);
            }
        }
    }
    return true;
}
