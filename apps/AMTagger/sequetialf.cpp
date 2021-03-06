#include "sequetialf.h"

SequentialF::SequentialF(QString name, MSF* parent): MSF(name, parent)
{
}

void SequentialF::addMSF(MSF *msf) {
    vector.append(msf);
}

bool SequentialF::isMBF() {
    return false;
}

bool SequentialF::isUnary() {
    return false;
}

bool SequentialF::isBinary() {
    return false;
}

bool SequentialF::isFormula() {
    return false;
}

bool SequentialF::isSequential() {
    return true;
}

QString SequentialF::print() {
    QString value;
    for(int i=0; i<vector.count(); i++) {
        value.append(vector.at(i)->print());
        value.append(" ");
    }
    value.chop(1);
    return value;
}

QString SequentialF::printwithNames() {
    if(parent != NULL) {
        return QString();
    }

    QString value = name;
    value.append("=(");
    for(int i=0; i<vector.count(); i++) {
        value.append(vector.at(i)->printwithNames());
        value.append(" ");
    }
    value.chop(1);
    value.append(')');
    return value;
}

void SequentialF::buildTree(QTreeWidgetItem* parent) {
    QStringList data;
    data << name << QString() << "()";
    QTreeWidgetItem* item = new QTreeWidgetItem(parent, data);
    for(int i=0; i<vector.count(); i++) {
        vector.at(i)->buildTree(item);
    }
}

void SequentialF::buildTree(QTreeWidget* parent) {

    QStringList data;
    data << name << QString() << "()";
    QTreeWidgetItem* item = new QTreeWidgetItem(parent, data);
    for(int i=0; i<vector.count(); i++) {
        vector.at(i)->buildTree(item);
    }
}

bool SequentialF::buildActionFile(QString &actionsData, QMultiMap<QString, QPair<QString,QString> > *functionParametersMap) {
    for(int i=0; i<vector.count(); i++) {
        if(!(vector.at(i)->buildActionFile(actionsData, functionParametersMap))) {
            return false;
        }
    }

    /// Adding function for preMatch actions
    QString tempInit = init;
    actionsData.append("extern \"C\" void " + name + "_preMatch(");
    if(!(tempInit.isEmpty())) {
        QSet<QString> param;
        while(true) {
            int dollarIndex = tempInit.indexOf("$", 0);
            if(dollarIndex == -1) {
                break;
            }

            QString msfName = tempInit.mid(dollarIndex+1).section('.',0,0);

            int afterDotPosition = tempInit.indexOf('.', dollarIndex) +1;
            QRegExp sep("[^a-zA-Z]");
            QString attribute = tempInit.mid(afterDotPosition).section(sep, 0, 0);
            tempInit = tempInit.remove(dollarIndex, 1);
            tempInit = tempInit.replace(afterDotPosition-2, 1, '_');

            if(param.contains(msfName + '|' + attribute)) {
                continue;
            }
            param.insert(msfName + '|' + attribute);

            if(attribute.compare("text") == 0) {
                functionParametersMap->insert(name + "_preMatch", QPair<QString,QString>(msfName,"text"));
                actionsData.append("QString " + msfName + "_text, ");
            }
            else if(attribute.compare("number") == 0) {
                functionParametersMap->insert(name + "_preMatch", QPair<QString,QString>(msfName,"number"));
                actionsData.append("int " + msfName + "_number, ");
            }
            else if(attribute.compare("position") == 0) {
                functionParametersMap->insert(name + "_preMatch", QPair<QString,QString>(msfName,"position"));
                actionsData.append("int " + msfName + "_position, ");
            }

            else if(attribute.compare("length") == 0) {
                functionParametersMap->insert(name + "_preMatch", QPair<QString,QString>(msfName,"length"));
                actionsData.append("int " + msfName + "_length, ");
            }
            else if(attribute.compare("matches") == 0) {
                functionParametersMap->insert(name + "_preMatch", QPair<QString,QString>(msfName,"matches"));
                actionsData.append("vector<Match>& " + msfName + "_matches, ");
            }
            else {
                return false;
            }
        }
        if(param.count() != 0) {
            actionsData.chop(2);
        }
    }
    actionsData.append(") {\n" + tempInit + "\n}\n\n");

    /// Adding function for onMatch actions
    QString tempMatch = actions;
    if(returns.isEmpty()) {
        actionsData.append("extern \"C\" void " + name + "_onMatch(");
    }
    else {
        actionsData.append("extern \"C\" " + returns + ' ' + name + "_onMatch(");
    }
    if(!(tempMatch.isEmpty())) {
        QSet<QString> param;
        while(true) {
            int dollarIndex = tempMatch.indexOf("$", 0);
            if(dollarIndex == -1) {
                break;
            }

            QString msfName = tempMatch.mid(dollarIndex+1).section('.',0,0);

            int afterDotPosition = tempMatch.indexOf('.', dollarIndex) +1;
            QRegExp sep("[^a-zA-Z]");
            QString attribute = tempMatch.mid(afterDotPosition).section(sep, 0, 0);
            tempMatch = tempMatch.remove(dollarIndex, 1);
            tempMatch = tempMatch.replace(afterDotPosition-2, 1, '_');

            if(param.contains(msfName + '|' + attribute)) {
                continue;
            }
            param.insert(msfName + '|' + attribute);

            if(attribute.compare("text") == 0) {
                functionParametersMap->insert(name + "_onMatch", QPair<QString,QString>(msfName,"text"));
                actionsData.append("QString " + msfName + "_text, ");
            }
            else if(attribute.compare("number") == 0) {
                functionParametersMap->insert(name + "_onMatch", QPair<QString,QString>(msfName,"number"));
                actionsData.append("int " + msfName + "_number, ");
            }
            else if(attribute.compare("position") == 0) {
                functionParametersMap->insert(name + "_onMatch", QPair<QString,QString>(msfName,"position"));
                actionsData.append("int " + msfName + "_position, ");
            }

            else if(attribute.compare("length") == 0) {
                functionParametersMap->insert(name + "_onMatch", QPair<QString,QString>(msfName,"length"));
                actionsData.append("int " + msfName + "_length, ");
            }
            else if(attribute.compare("matches") == 0) {
                functionParametersMap->insert(name + "_onMatch", QPair<QString,QString>(msfName,"matches"));
                actionsData.append("vector<Match>& " + msfName + "_matches, ");
            }
            else {
                return false;
            }
        }
        if(param.count() != 0) {
            actionsData.chop(2);
        }
    }
    actionsData.append(") {\n" + tempMatch + "\n}\n\n");

    return true;
}

QVariantMap SequentialF::getJSON() {
    QVariantMap sMap;
    sMap.insert("name", name);
    sMap.insert("type","sequential");
    sMap.insert("init", init);
    sMap.insert("actions",actions);
    //sMap.insert("after", after);
    sMap.insert("returns", returns);
    sMap.insert("parent",parent->name);
    QVariantList sList;
    for(int i=0; i<vector.count();i++) {
        sList << vector.at(i)->getJSON();
    }
    sMap.insert("MSFs",sList);
    return sMap;
}

bool SequentialF::buildNFA(NFA *nfa) {
    QString state1 = "q";
    state1.append(QString::number(nfa->i));
    (nfa->i)++;
    nfa->stateTOmsfMap.insert(state1, QPair<MSF*,QString>(this,"pre"));
    if(nfa->start.isEmpty()) {
        nfa->start = state1;
    }
    else {
        nfa->transitions.insert(nfa->last + '|' + "epsilon",state1);
    }
    nfa->last = state1;

    for(int i=0; i< vector.count(); i++) {
        if(!(vector.at(i)->buildNFA(nfa))) {
            return false;
        }
    }

    QString state2 = "q";
    state2.append(QString::number(nfa->i));
    (nfa->i)++;
    nfa->stateTOmsfMap.insert(state2, QPair<MSF*,QString>(this,"on"));
    nfa->transitions.insert(nfa->last + '|' + "epsilon",state2);
    nfa->last = state2;
    nfa->accept = state2;
    return true;
}

bool SequentialF::removeSelfFromMap(QMap<QString, MSF*> &map) {
    for(int j=0; j<vector.count(); j++) {
        if(!(vector.at(j)->removeSelfFromMap(map))) {
            return false;
        }
    }
    int count = map.remove(name);
    if(count > 0) {
        return true;
    }
    else {
        return false;
    }
}

QStringList SequentialF::getMSFNames() {
    QStringList list;
    list.append(name);
    for(int i=0; i<vector.count(); i++) {
        QStringList tempList = vector.at(i)->getMSFNames();
        list += tempList;
    }
    return list;
}

SequentialF::~SequentialF() {
    for(int i=0; i<vector.count(); i++) {
        delete vector.at(i);
    }
}
