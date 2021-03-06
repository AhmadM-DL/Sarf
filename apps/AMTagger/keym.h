#ifndef KEYM_H
#define KEYM_H

#include <QString>
#include "match.h"

class KeyM : public Match
{
public:
    KeyM(Match* parent, QString key, int pos, int length, int id);
    QString word;
    QString key;
    int pos;
    int length;
    bool setMatch(Match* match);
    bool isUnaryM();
    bool isBinaryM();
    bool isSequentialM();
    bool isKeyM();
    bool isMERFTag();
    int getPOS();
    int getLength();
    QString getText();
    int getMatchCount();
    void buildMatchTree(Agraph_t* G,Agnode_t* node,Agedge_t* edge,QMap<Agnode_t *,Agnode_t *>* parentNodeMap,QTreeWidgetItem* parentItem, int& id);
    void executeActions(NFA* nfa);
    QString getParam(QString msfName,QString param, QString* sarfMatches=NULL);
    QVariantMap getJSON();
    void constructRelation(Relation* relation, QVector<Match*>& entity1, QVector<Match*>& entity2, QVector<Match*>& edge);
    ~KeyM();
};

#endif // KEYM_H
