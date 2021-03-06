#include <QTextBrowser>
#include <QFile>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <assert.h>

#include "bibleManualTagger.h"
#include "bibleGeneology.h"
#include "ATMProgressIFC.h"
#include "Math_functions.h"
//#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include "stemmer.h"
#include "letters.h"
#include "diacritics.h"
#include "text_handling.h"
#include "textParsing.h"
#include "common.h"
#include "Retrieve_Template.h"
#include "genealogyItem.h"

enum WordType { NEW_NAME, CORE_NAME, LEAF_NAME, DC, ENDING_PUNC, PARA_PUNC, COLON_PUNC, OTHER};
enum DescentDirection {SPOUSE, SON, FATHER, SIBLING, UNDEFINED_DIRECTION};
enum StateType { TEXT_S , NAME_S, SONS_S};

#define display(c) ;

QString preProcessedGenealogyDescriptionsFileName = ".GenealogyPreProcessedDescriptions";

class DescentConnectors;
class DescentConnectorGroup {
    private:
        friend class DescentConnectors;
        QList<long> descriptions;
        DescentDirection dir;
        bool plural;
    private:
        long operator[](int i) {
            return descriptions[i];
        }
        int size() {
            return descriptions.size();
        }
        friend QDataStream &operator << (QDataStream   &out, const DescentConnectorGroup &group) {
            out << group.descriptions;
            out << (int)group.dir;
            out << group.plural;
            return out;
        }
        friend QDataStream &operator >> (QDataStream   &in, DescentConnectorGroup &group) {
            in >> group.descriptions;
            int dir;
            in >> dir;
            group.dir = (DescentDirection)dir;
            in >> group.plural;
            return in;
        }

    public:
        DescentConnectorGroup(DescentDirection dir, bool plural = false) {
            this->dir = dir;
            this->plural = plural;
        }
        DescentConnectorGroup() {
            this->dir = UNDEFINED_DIRECTION;
        }
        void addDescription(long description_id) {
            descriptions << description_id;
        }
        bool isDescription(long description_id) {
            for (int j = 0; j < size(); j++) {
                if (descriptions[j] == description_id) {
                    return true;
                }
            }

            return false;
        }
};
class DescentConnectors {
    private:
        QList<DescentConnectorGroup> groups;

        friend QDataStream &operator << (QDataStream   &out, const DescentConnectors &conn) {
            out << conn.groups;
            return out;
        }
        friend QDataStream &operator >> (QDataStream   &in, DescentConnectors &conn) {
            in >> conn.groups;
            return in;
        }
    public:
        void addDescentGroup(DescentConnectorGroup group) {
            groups << group;
        }
        DescentDirection getDirection(long description_id, bool &plural) {
            for (int i = 0; i < groups.size(); i++) {
                if (groups[i].isDescription(description_id)) {
                    plural = groups[i].plural;
                    return groups[i].dir;
                }
            }

            plural = false;
            return UNDEFINED_DIRECTION;
        }
        DescentDirection getDirection(long description_id) {
            bool plural;
            return getDirection(description_id, plural);
        }
};


GeneologyParameters geneologyParameters;
QList<int> bits_gene_NAME;
QStringList relativeSuffixes;
int bit_VERB_PERFECT, bit_LAND, bit_FEMALE, bit_MALE;
DescentConnectors descentConnectors;
DescentConnectorGroup landDesc;

class GeneStemmer: public Stemmer {
        //private:
        //bool place;
    public:
        long finish_pos;
        bool name: 1;
        bool male: 1;
        bool gaveBirth: 1;
        bool land: 1;
        DescentDirection descentDir: 3;
        bool pluralDescent: 1;
        bool relativeSuffix;
        bool has_waw: 1;
        bool has_li: 1;
        long startStem, finishStem, wawStart, wawEnd;

        GeneStemmer(QString *word, int start)
            : Stemmer(word, start, false)  {
            setSolutionSettings(M_ALL);
            init(start);
        }
        void init(int start) {
            this->info.start = start;
            this->info.finish = start;
            name = false;
            descentDir = UNDEFINED_DIRECTION;
            pluralDescent = false;
            gaveBirth = false;
            land = false;
            finish_pos = start;
            startStem = start;
            finishStem = start;
            relativeSuffix = false;
            male = true;
            wawStart = start;
            wawEnd = start;
            has_waw = false;
            has_li = false;
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
            has_li = false;
            solution_position *p_inf = Prefix->computeFirstSolution();

            do {
                prefix_infos = &Prefix->affix_info;

                if (prefix_infos->size() > 0) {
                    int i = 0;
                    has_waw = prefix_infos->at(i).POS == "wa/CONJ+";

                    if (!has_waw) {
                        has_li = prefix_infos->at(i).POS == "li/PREP+";
                    }

                    if (has_waw || has_li) {
                        wawStart = (i == 0 ? Prefix->info.start : Prefix->getSplitPositions().at(i - 1) - 1);
                        wawEnd = Prefix->getSplitPositions().at(i) - 1;
                        break;
                    }
                }

                if (has_waw || has_li) {
                    break;
                }
            } while (Prefix->computeNextSolution(p_inf));

            delete p_inf;
        }


        bool analyze()  {
            if (info.finish > info.start) { //more than one letter to be tested for being a name
                if (equal_withoutLastDiacritics(israel, stem_info->raw_data)) {
                    return true;
                }

                if (landDesc.isDescription(stem_info->description_id())) {
                    land = true;
                    return false;
                }

                bool plural;
                descentDir = descentConnectors.getDirection(stem_info->description_id(), plural);

                if (descentDir != UNDEFINED_DIRECTION) {
                    pluralDescent = plural;
                    int suffSize = Suffix->info.finish - Suffix->info.start + 1;
                    gaveBirth = stem_info->abstract_categories.getBit(bit_VERB_PERFECT);

                    if (suffSize > 0 && !gaveBirth) {
                        QString suffix = info.text->mid(Suffix->info.start, suffSize);

                        for (int i = 0; i < relativeSuffixes.size(); i++) {
                            if (equalNames(suffix, relativeSuffixes[i])) {
                                relativeSuffix = true;

                                if (descentDir == SON) {
                                    descentDir = FATHER;
                                    pluralDescent = true;
                                } else if (descentDir == FATHER) {
                                    descentDir = SON;
                                    pluralDescent = true;
                                }

                                break;
                            }
                        }
                    }

                    return false;
                }

                int bitsNamesSize = bits_gene_NAME.size();

                for (int i = 0; i < bitsNamesSize; i++) {
                    if (stem_info->abstract_categories.getBit(bits_gene_NAME[i]))
                        //&& Suffix->info.finish-Suffix->info.start<0 && Stem->info.finish>Stem->info.start) //i.e. has no suffix and stem > a letter
                    {
                        name = true;

                        if (info.finish > finish_pos) {
                            finish_pos = info.finish;
                            finishStem = Stem->info.finish;
                            startStem = Stem->info.start;

                            if (stem_info->abstract_categories.getBit(bit_FEMALE)) {
                                male = false;
                            } else if (stem_info->abstract_categories.getBit(bit_MALE)) {
                                male = true;
                            }
                        }

                        if (info.finish == finish_pos && !has_waw && !has_li) {
                            checkForWaw();
                        }

                        return true;
                    }
                }
            }

            return true;
        }
};

void readFromDatabasePreProcessedGenealogyDescriptions() {
    DescentConnectorGroup spouse(SPOUSE);
    Retrieve_Template d_spouse("description", "id", "name='woman' OR name LIKE '%spouse%' OR name='concubine'");

    while (d_spouse.retrieve()) {
        spouse.addDescription(d_spouse.get(0).toULongLong());
    }

    DescentConnectorGroup spouses(SPOUSE, true);
    Retrieve_Template d_spouses("description", "id", "name='women' OR name='concubines'");

    while (d_spouses.retrieve()) {
        spouses.addDescription(d_spouses.get(0).toULongLong());
    }

    DescentConnectorGroup sibling(SIBLING);
    Retrieve_Template d_sibling("description", "id", "name='brother' OR name='sister' OR name='sister/counterpart'");

    while (d_sibling.retrieve()) {
        sibling.addDescription(d_sibling.get(0).toULongLong());
    }

    DescentConnectorGroup siblings(SIBLING, true);
    Retrieve_Template d_siblings("description", "id", "name='brothers' OR name='sisters'");

    while (d_siblings.retrieve()) {
        siblings.addDescription(d_siblings.get(0).toULongLong());
    }

    DescentConnectorGroup father(FATHER);
    Retrieve_Template d_father("description", "id", "name='son' OR name='daughter/girl' OR name='eldest/first-born'");

    while (d_father.retrieve()) {
        father.addDescription(d_father.get(0).toULongLong());
    }

    DescentConnectorGroup son(SON);
    Retrieve_Template d_son("description", "id", "name='father' OR name='mother/maternal' OR name='mother'");

    while (d_son.retrieve()) {
        son.addDescription(d_son.get(0).toULongLong());
    }

    DescentConnectorGroup sons(SON, true);
    Retrieve_Template d_sons("description", "id",
                             "name='give birth to' OR name='be born' OR name LIKE 'sons/children%' OR name='daughters/girls'");

    while (d_sons.retrieve()) {
        sons.addDescription(d_sons.get(0).toULongLong());
    }

    descentConnectors.addDescentGroup(spouse);
    descentConnectors.addDescentGroup(sibling);
    descentConnectors.addDescentGroup(father);
    descentConnectors.addDescentGroup(son);
    descentConnectors.addDescentGroup(sons);
    descentConnectors.addDescentGroup(siblings);
    descentConnectors.addDescentGroup(spouses);
    Retrieve_Template d_land("description", "id", "name='earth/territory' OR name LIKE 'feddan%'");

    while (d_land.retrieve()) {
        landDesc.addDescription(d_land.get(0).toULongLong());
    }

    //abou ... bani and kharaja minho/minhoma/minhom => non direct descent
    QFile file(preProcessedGenealogyDescriptionsFileName.toStdString().data());

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);   // we will serialize the data into the file
        out << descentConnectors
            << landDesc;
        file.close();
    } else {
        _error << "Unexpected Error: Unable to write PreProcessed Genealogy Descriptions to file\n";
    }
}

void readFromFilePreprocessedGenealogyDescriptions() {
    QFile file(preProcessedGenealogyDescriptionsFileName.toStdString().data());

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);    // read the data serialized from the file
        in >> descentConnectors
           >> landDesc;
        file.close();
    } else {
        readFromDatabasePreProcessedGenealogyDescriptions();
    }
}

void geneology_initialize() {
    long abstract_NAME = database_info.comp_rules->getAbstractCategoryID("Hebrew Bible Names");
    int bit_NAME = database_info.comp_rules->getAbstractCategoryBitIndex(abstract_NAME);
    bits_gene_NAME.append(bit_NAME);
    readFromFilePreprocessedGenealogyDescriptions();
    long abstract_VERB_PERFECT = database_info.comp_rules->getAbstractCategoryID("VERB_PERFECT");
    bit_VERB_PERFECT = database_info.comp_rules->getAbstractCategoryBitIndex(abstract_VERB_PERFECT);
    long abstract_FEMALE = database_info.comp_rules->getAbstractCategoryID("Female Names");
    bit_FEMALE = database_info.comp_rules->getAbstractCategoryBitIndex(abstract_FEMALE);
    long abstract_MALE = database_info.comp_rules->getAbstractCategoryID("Male Names");
    bit_MALE = database_info.comp_rules->getAbstractCategoryBitIndex(abstract_MALE);
    relativeSuffixes << QString("") + ha2 << QString("") + ha2 + alef << QString("") + ha2 + meem + alef << QString("") +
                     ta2 + ha2 << QString("") + ta2 + ha2 + alef << QString("") + ta2 + ha2 + meem + alef;
}

class GeneTagVisitor: public GeneVisitor {
    private:
        ATMProgressIFC *prg;
    public:
        GeneTagVisitor(ATMProgressIFC *prg) {
            this->prg = prg;
        }
        void visit(GeneNode *node, int) {
            const Name &name = node->getName();
            prg->tag(name.getStart(), name.getLength(), Qt::white, true);
        }
        void visit(GeneNode *, AbstractGeneNode *node2, bool isSpouse) {
            if (isSpouse) {
                const Name &name = node2->getName();
                prg->tag(name.getStart(), name.getLength(), Qt::white, true);
            }
        }
        void finish() {}
};
class GeneTree::MergeVisitor : public GeneVisitor {
    public:
        static QString getEdge(const Name &n_parent, const Name &n_child, QList<int> *delimitersStart,
                               QList<int> *delimitersEnd) {
            int start = min(n_parent.getEnd(), n_child.getEnd()),
                end   = max(n_parent.getStart(), n_child.getStart());
            //qDebug()<<n_parent.getString()<<"\t"<<n_child.getString();
            QList<int>::const_iterator lower = qLowerBound(*delimitersStart, end);
            QList<int>::const_iterator upper = qLowerBound(*delimitersEnd, end);
            start = *(lower - 1);
            end = *(upper - 1);
            Name temp(n_child.getTextPointer(), start, end);
            QString s = temp.getString();
            return s.remove("\n");
        }

        static void appendEdgeName(AbstractGeneNode *nodeToAppendTo, const Name &n_parent, const Name &n_child,
                                   QList<int> *delimitersStart, QList<int> *delimitersEnd) {
            if (delimitersStart == NULL || delimitersEnd == NULL) {
                return;
            }

            QString s = getEdge(n_parent, n_child, delimitersStart, delimitersEnd);
            nodeToAppendTo->addEdgeText(s);
        }
        static void appendEdgeName(GeneNode *n, AbstractGeneNode *n2, QList<int> *delimitersStart, QList<int> *delimitersEnd,
                                   bool appendToSecond = true) {
            if (delimitersStart == NULL || delimitersEnd == NULL) {
                return;
            }

            const Name &n_parent = n->getName(),
                        & n_child = n2->getName();
            AbstractGeneNode *node = (appendToSecond ? n2 : n);
            appendEdgeName(node, n_parent, n_child, delimitersStart, delimitersEnd);
        }

    private:
        typedef QPair< GeneNode *, AbstractGeneNode *> Edge;
        typedef QMap<Edge, bool> EdgeMap;
    private:
        GeneTree *mainTree;
        GeneNode *topMostNode;
        EdgeMap unPerformedEdges;
        QList<int> *delimitersStart, *delimitersEnd;
        bool newTree;
        void addUnPerformedEdge(GeneNode *n1, AbstractGeneNode *n2, bool isSpouse = false) {
            unPerformedEdges[Edge(n1, n2)] = isSpouse;
        }
        GeneNode *find(const Name &name, bool checkSpouses = false) {
            if (topMostNode == NULL) {
                GeneNode *node = mainTree->findTreeNode(name.getString(), checkSpouses);

                if (node != NULL) {
                    topMostNode = node;
                }

                return node;
            } else {
                GeneNode *node = topMostNode->getNodeInSubTree(name.getString(), checkSpouses);
                return node;
            }
        }
        bool notFoundWithinRadius(GeneNode *node, const Name &searchedName, bool searchSpouses = false) {
            int radius = geneologyParameters.radius;
            GeneNode *temp = node->getParent();
            int tempRadius = radius;

            for (int i = 0; i < radius; i++) {
                if (temp != NULL) {
                    tempRadius++;
                    node->ignoreInSearch = true;
                    GeneNode *temp2 = temp->getNodeInSubTree(searchedName.getString(), searchSpouses, tempRadius);
                    node->ignoreInSearch = false;

                    if (temp2 != NULL) {
                        return false;
                    }

                    if (equalNames(temp->toString(), searchedName.getString())) {
                        return false;
                    }

                    for (int j = 0; j < temp->spouses.size(); j++) {
                        if (equalNames(temp->spouses[j]->getString(), searchedName.getString())) {
                            return false;
                        }
                    }

                    temp = temp->getParent();
                }
            }

            node->ignoreInSearch = true;
            temp = node->getNodeInSubTree(searchedName.getString(), searchSpouses, radius);
            node->ignoreInSearch = false;

            if (temp != NULL) {
                return false;
            }

            return true;
        }
        void performAction(GeneNode *node1, AbstractGeneNode *node2, bool isSpouse) {
            const Name   &name1 = node1->getName(),
                          & name2 = node2->getName();
            GeneNode *n1 = find(name1, true);
            GeneNode *n2 = find(name2, true);

            if (newTree && n1 != NULL && n2 != NULL && notFoundWithinRadius(n1, name2)) {
                topMostNode = n1;
                n2 = NULL;
            }

            if (isSpouse) {
                if (n1 == n2) {
                    if (n1 != NULL) {
                        //correctly placed
                        n1->name = name1; //so that we add a new edge when calling appendEdgeName(..)

                        for (int i = 0; i < n1->spouses.size(); i++) {
                            if (n1->spouses[i]->getName() == name2) {
                                n1->spouses[i]->name = name2; //so that we add a new edge when calling appendEdgeName(..)
                                appendEdgeName(n1, n1->spouses[i], delimitersStart, delimitersEnd);
                            }
                        }
                    } else {
                        addUnPerformedEdge(node1, node2, isSpouse);
                    }
                } else {
                    if (n1 == NULL) {
                        if (notFoundWithinRadius(n2, name1, true)) {
                            n2->addSpouse(name1);
                            appendEdgeName(n2, n2->spouses.last(), delimitersStart, delimitersEnd);
                        }
                    } else if (n2 == NULL) {
                        if (notFoundWithinRadius(n1, name2, true)) {
                            n1->addSpouse(name2);
                            appendEdgeName(n1, n1->spouses.last(), delimitersStart, delimitersEnd);
                        }
                    }
                }
            } else {
                if (n1 == n2) {
                    if (n1 == NULL) {
                        addUnPerformedEdge(node1, node2, isSpouse);
                    }
                } else {
                    if (n1 == NULL) {
                        if (notFoundWithinRadius(n2, name1, true)) {
                            n2->addParent(new GeneNode((Name &)name1, NULL));
                            mainTree->updateRoot();
                            appendEdgeName(n2->getParent(), n2, delimitersStart, delimitersEnd);
                        }
                    } else if (n2 == NULL) {
                        if (notFoundWithinRadius(n1, name2)) {
                            GeneNode *child = new GeneNode((Name &)name2, NULL);
                            n1->addChild(child);
                            appendEdgeName(n1, child, delimitersStart, delimitersEnd);
                        }
                    } else {
                        if (n2->getParent() != n1) {
                        } else {
                            n1->name = name1; //so that we add a new edge when calling appendEdgeName(..)
                            n2->name = name2; //so that we add a new edge when calling appendEdgeName(..)
                            appendEdgeName(n1, n2, delimitersStart, delimitersEnd);
                        }
                    }
                }
            }
        }

    public:
        MergeVisitor(GeneTree *tree2, QList<int> &delimitersStart, QList<int> &delimitersEnd) {
            this->mainTree = tree2;
            this->delimitersStart = &delimitersStart;
            this->delimitersEnd = &delimitersEnd;
            topMostNode = NULL;
        }
        MergeVisitor(GeneTree *tree2) {
            this->mainTree = tree2;
            topMostNode = NULL;
            delimitersStart = delimitersEnd = NULL;
        }
        virtual ~MergeVisitor() {}

        void operator()(GeneTree *tree) {
            if (tree->getRoot() != NULL) {
                topMostNode = find(tree->getRoot()->getName());
                newTree = true;
                GeneVisitor::operator()(tree);
                newTree = false;
            }
        }

        virtual void visit(GeneNode *, int) {  }
        virtual void visit(GeneNode *node1, AbstractGeneNode *node2, bool isSpouse) {
            performAction(node1, node2, isSpouse);
        }
        void finish() {}
        void tryPerformingUnperformedNodes() {
            EdgeMap::iterator itr = unPerformedEdges.begin();

            for (; itr != unPerformedEdges.end(); itr++) {
                performAction(itr.key().first, itr.key().second, itr.value());
            }
        }
};
class FindAllVisitor: public GeneVisitor {
    public:
        typedef QPair<GeneNode *, Name> NodeNamePair;
    private:
        QSet<NodeNamePair> &visited;

        GeneNode *nodeToMatch;
        const Name &nameToMatch;
        GeneNode *bestMatch;
        int bestContext, bestNeighbor, bestSpouses, bestChildren;

        GeneNode *currMatch;  //temporary
        int currContext, currNeighbor, currSpouses, currChildren; //temporary
    private:
        void getNeighborNames(GeneNode *node, QList<const Name *> &list) {
            if (node == NULL) {
                return;
            }

            if (node->getParent() != NULL) {
                for (int i = 0; i < node->getParent()->spouses.size(); i++) {
                    list << &node->getParent()->spouses[i]->getName();
                }

                for (int i = 0; i < node->getParent()->children.size(); i++)
                    if (node->getParent()->children[i] != node) {
                        list << &node->getParent()->children[i]->getName();
                    }

                list << &node->getParent()->getName();
            }

            for (int i = 0; i < node->children.size(); i++) {
                list << &node->children[i]->getName();
            }

            for (int i = 0; i < node->spouses.size(); i++) {
                list << &node->spouses[i]->getName();
            }
        }

        void finalizeMatch() {
            if (currMatch == NULL) {
                return;
            }

            if (visited.contains(NodeNamePair(currMatch, nameToMatch))) {
                return;
            }

            assert(nodeToMatch != NULL);
            GeneNode *currParent = currMatch->getParent();
            GeneNode *toMatchParent = nodeToMatch->getParent();

            if (toMatchParent == NULL) {
                if (currParent == NULL) {
                    currContext++;
                }
            } else {
//                if (equalNames(toMatchParent->toString(), currParent->toString())) {
//                    currContext++;
//                }
                if (toMatchParent != NULL && currParent != NULL) {
                    if (equalNames(toMatchParent->toString(), currParent->toString())) {
                        currContext++;
                    }

                    for (int i = 0; i < toMatchParent->spouses.size(); i++) {
                        if (equalNames(toMatchParent->spouses[i]->getString(), currParent->toString())) {
                            currContext++;
                        }

                        for (int j = 0; j < currParent->spouses.size(); j++) {
                            if (j == 0 && equalNames(toMatchParent->toString(), currParent->spouses[j]->getString())) {
                                currContext++;
                            }

                            if (equalNames(toMatchParent->spouses[i]->getString(), currParent->spouses[j]->getString())) {
                                currContext++;
                            }
                        }
                    }
                }
            }

            QList<const Name *> nodeNames, matchNames;
            getNeighborNames(nodeToMatch, nodeNames);
            getNeighborNames(currMatch, matchNames);

            for (int i = 0; i < nodeNames.size(); i++) {
                for (int j = 0; j < matchNames.size(); j++) {
                    if (equalNames(nodeNames[i]->getString(), matchNames[j]->getString())) {
                        currNeighbor++;
                    }
                }
            }

            if (bestMatch == NULL || bestContext < currContext || bestNeighbor < currNeighbor) {
                bestMatch = currMatch;
                bestContext = currContext;
                bestNeighbor = currNeighbor;
                bestSpouses = currSpouses;
                bestChildren = currChildren;
            }
        }

    public:
        FindAllVisitor(GeneNode *nodeToMatch, const Name &name, QSet<NodeNamePair> &visitedNodes)
            : visited(visitedNodes), nameToMatch(name) {
            this->nodeToMatch = nodeToMatch;
            bestMatch = NULL;
            currMatch = NULL;
            bestContext = 0;
            bestNeighbor = 0;
            bestSpouses = 0;
            bestChildren = 0;
            currContext = 0;
            currNeighbor = 0;
            currSpouses = 0;
            currChildren = 0;
        }
        virtual void visit(GeneNode *node, int) {
            finalizeMatch();

            if (equalNames(node->toString(), nameToMatch.getString()) || node->hasSpouse(nameToMatch, false)) {
                currMatch = static_cast<GeneNode *>(node);
            } else {
                currMatch = NULL;
            }

            currContext = 0;
            currNeighbor = 0;
            currSpouses = 0;
            currChildren = 0;
        }
        virtual void visit(GeneNode *node1, AbstractGeneNode *node2, bool isSpouse) {
            const Name &name2 = node2->getName();

            if (node1 == currMatch) {
                //qDebug()<<"\t"<<name2.getString();
                if (isSpouse) {
                    if (nodeToMatch->hasSpouse(name2, true)) {
                        currContext++;
                        currSpouses++;
                    }
                } else {
                    if (nodeToMatch->hasChild(name2)) {
                        currContext++;
                        currChildren++;
                    }
                }
            }
        }
        virtual void finish() {
            finalizeMatch();

            if (bestMatch != NULL) {
                assert(!visited.contains(NodeNamePair(bestMatch, nameToMatch)));
                visited.insert(NodeNamePair(bestMatch, nameToMatch));
            }
        }
        bool isFound() {
            return bestMatch != NULL;
        }
        GeneNode *getFoundNode() {
            return bestMatch;
        }
        double getContextRecall() {
            int contextCount = bestMatch->spouses.size() + bestMatch->children.size() + 1; //1 is for parent even if NULL

            if (bestMatch->getParent() != NULL) {
                contextCount += bestMatch->getParent()->spouses.size();    //mothers possibly
            }

            if (contextCount > 0) {
                double score = (double)bestContext / contextCount;
                return score;
            } else {
                return 1;
            }
        }
        double getContextPrecision() {
            int contextCount = nodeToMatch->spouses.size() + nodeToMatch->children.size() + 1; //1 is for parent even if NULL

            if (nodeToMatch->getParent() != NULL) {
                contextCount += nodeToMatch->getParent()->spouses.size();    //mothers possibly
            }

            if (contextCount > 0) {
                double score = (double)bestContext / contextCount;
                return score;
            } else {
                return 1;
            }
        }
        double getNeighborRecall() {
            QList<const Name *> matchNames;
            getNeighborNames(bestMatch, matchNames); //TODO: inefficient for getting number of neighbors

            if (matchNames.size() > 0) {
                double recall = (double)bestNeighbor / matchNames.size();
                return recall;
            } else {
                return 1;
            }
        }
        int getNumberNeighbors() {
            QList<const Name *> matchNames;
            getNeighborNames(nodeToMatch, matchNames); //TODO: inefficient for getting number of neighbors
            return matchNames.size();
        }

        double getNeighborPrecision() {
            QList<const Name *> matchNames;
            getNeighborNames(nodeToMatch, matchNames); //TODO: inefficient for getting number of neighbors

            if (matchNames.size() > 0) {
                double precision = (double)bestNeighbor / matchNames.size();
                return precision;
            } else {
                return 1;
            }
        }
        double getSpousesRecall() {
            int size = bestMatch->spouses.size()/*+1*/;

            if (size > 0) {
                double recall = (double)bestSpouses / size;
                return recall;
            } else {
                return 1;
            }
        }
        double getSpousesPrecision() {
            int size = nodeToMatch->spouses.size()/*+1*/;

            if (size > 0) {
                double precision = (double)bestSpouses / size;
                return precision;
            } else {
                return 1;
            }
        }
        double getChildrenRecall() {
            int size = bestMatch->children.size();

            if (size > 0) {
                double recall = (double)bestChildren / size;
                return recall;
            } else {
                return 1;
            }
        }
        double getChildrenPrecision() {
            int size = nodeToMatch->children.size();

            if (size > 0) {
                double precision = (double)bestChildren / size;
                return precision;
            } else {
                return 1;
            }
        }
};
class CompareVisitor: public GeneVisitor {
    public:
        typedef GeneTree::GraphStatistics GraphStatistics;
    private:
        QSet<FindAllVisitor::NodeNamePair> &visitedNodes;
        GeneTree *standard;
        int foundCount, totalCount, totalDetected, totalNeighbors;
        GraphStatistics &statistics;
        QVector<double> contextRecall, contextPrecision,
                neigborhoodRecall, neigborhoodPrecision,
                spousesRecall, spousesPrecision,
                childrenRecall, childrenPrecision;
    public:
        CompareVisitor(GeneTree *standard, QSet<FindAllVisitor::NodeNamePair> &visited, GraphStatistics &stats)
            : visitedNodes(visited), statistics(stats) {
            foundCount = 0;
            totalCount = standard->getTreeNodesCount(true);
            totalDetected = 0;
            this->standard = standard;
            totalNeighbors = 0;
            //standard->updateRoot();
        }
        void searchFor(GeneNode *node, const Name &n) {
            FindAllVisitor v(node, n, visitedNodes);
            v(standard);

            if (v.isFound()) {
                //qDebug()<<n.getString()<<"\t"<<v.getContextRecall()<<"\t"<<v.getNeighborRecall();
                foundCount++;
                int numNeighbors = v.getNumberNeighbors();
                totalNeighbors += numNeighbors;
                contextRecall.append(v.getContextRecall()*numNeighbors);
                contextPrecision.append(v.getContextPrecision()*numNeighbors);
                neigborhoodRecall.append(v.getNeighborRecall()*numNeighbors);
                neigborhoodPrecision.append(v.getNeighborPrecision()*numNeighbors);
                spousesRecall.append(v.getSpousesRecall()*numNeighbors);
                spousesPrecision.append(v.getSpousesPrecision()*numNeighbors);
                childrenRecall.append(v.getChildrenRecall()*numNeighbors);
                childrenPrecision.append(v.getChildrenPrecision()*numNeighbors);
            }
        }

        virtual void visit(GeneNode *node, int) {
            if (node->getParent() == NULL) {
                totalDetected = node->getSubTreeCount(true);
            }

            searchFor(node, node->getName());
        }
        virtual void visit(GeneNode *node1, AbstractGeneNode *node2, bool isSpouse) {
            if (isSpouse) {
                searchFor(node1, node2->getName());
            }
        }
        virtual void finish() {
            statistics.foundRecall = (double)foundCount / totalCount;
            statistics.foundPrecision = (double)foundCount / totalDetected;

            if (foundCount > 0) {
                statistics.contextRecall = sum(contextRecall) / totalNeighbors;
                statistics.contextPrecision = sum(contextPrecision) / totalNeighbors;
                statistics.neigborhoodRecall = sum(neigborhoodRecall) / totalNeighbors;
                statistics.neigborhoodPrecision = sum(neigborhoodPrecision) / totalNeighbors;
                statistics.spousesRecall = sum(spousesRecall) / totalNeighbors;
                statistics.spousesPrecision = sum(spousesPrecision) / totalNeighbors;
                statistics.childrenRecall = sum(childrenRecall) / totalNeighbors;
                statistics.childrenPrecision = sum(childrenPrecision) / totalNeighbors;
            } else {
                statistics.contextRecall = 0;
                statistics.contextPrecision = 1;
                statistics.neigborhoodRecall = 0;
                statistics.neigborhoodPrecision = 1;
                statistics.spousesRecall = 0;
                statistics.spousesPrecision = 1;
                statistics.childrenRecall = 0;
                statistics.childrenPrecision = 1;
            }
        }
};
class GeneDisplayVisitor: public GeneVisitor {
    protected:
        QFile *file;
        QTextStream *dot_out;
#define d_out (*dot_out)
        QList<QStringList> ranksList;
        QSet<long> set;

        long getUniqueNodeID(const Name &n, bool isSpouse) {//if not there returns -1
            long id = (long)&n;

            if (!set.contains(id)) {
                d_out << QString("n%1 [label=\"%2\" %3]\n").arg(id).arg(n.getString()).arg((isSpouse ? ",style=filled, fillcolor=grey" :
                        ""));
            }

            return id;
        }
        void setGraphRank(int rank, QString s)  {
            while (rank >= ranksList.size()) {
                ranksList.append(QStringList());
            }

            ranksList[rank].append(s);
        }
        QString getAndInitializeDotNode(const Name &n, bool isSpouse) {
            long curr_id = getUniqueNodeID(n, isSpouse);
            return QString("n%1").arg(curr_id);
        }
        virtual void initialize() {
            ranksList.clear();
            file = new QFile("graph.dot");
            file->remove();

            if (!file->open(QIODevice::ReadWrite)) {
                theSarf->out << "Error openning file\n";
                return;
            }

            dot_out = new QTextStream(file);
            d_out.setCodec("utf-8");
            d_out << "digraph gene_graph {\n";
        }
        virtual void visit(GeneNode *n1, AbstractGeneNode *n2, bool isSpouse)  {
            const Name &name1 = n1->getName();
            const Name &name2 = n2->getName();
            QString s1 = getAndInitializeDotNode(name1, false);
            QString s2 = getAndInitializeDotNode(name2, isSpouse);
            d_out << s1 << "->" << s2 << "[label=\"" << n2->getEdgeText() << "\"] ;\n";
        }
        virtual void visit(GeneNode *n, int) {
            getAndInitializeDotNode(n->getName(), false);
        }
        virtual void finish() {
            d_out << "}\n";
            delete dot_out;
            file->close();
            delete file;
        }
    public:
        GeneDisplayVisitor() {
            initialize();
        }

        virtual ~GeneDisplayVisitor() {}
};
class TreeDuplicator {
    private:
        QList<int> *delimitersStart, *delimitersEnd;
    private:
        void duplicateChildren(GeneNode *node, GeneNode *duplicatedNode) {
            for (int i = 0; i < node->children.size(); i++) {
                const Name &childName = node->children[i]->getName();
                GeneNode *duplicatedChild = new GeneNode(childName, duplicatedNode);
                duplicatedChild->copyEdgeText(node->children[i]);

                if (duplicatedChild->getEdgeText().isEmpty()) {
                    GeneTree::MergeVisitor::appendEdgeName(duplicatedNode, duplicatedChild, delimitersStart, delimitersEnd);
                }

                duplicateChildren(node->children[i], duplicatedChild);
            }

            for (int i = 0; i < node->spouses.size(); i++) {
                const Name &spouseName = node->spouses[i]->getName();
                duplicatedNode->addSpouse(spouseName);
                SpouseGeneNode *duplicatedSpouse = duplicatedNode->spouses.last();
                duplicatedSpouse->copyEdgeText(node->spouses[i]);

                if (duplicatedSpouse->getEdgeText().isEmpty()) {
                    GeneTree::MergeVisitor::appendEdgeName(duplicatedNode, duplicatedSpouse, delimitersStart, delimitersEnd);
                }
            }
        }
    public:
        TreeDuplicator() {
            delimitersStart = delimitersEnd = NULL;
        }
        TreeDuplicator(QList<int> &delimitersStart, QList<int> &delimitersEnd) {
            this->delimitersStart = &delimitersStart;
            this->delimitersEnd = &delimitersEnd;
        }
        GeneTree *operator()(GeneTree *tree) {
            if (tree->getRoot() != NULL) {
                GeneNode *duplicatedRoot = new GeneNode(tree->getRoot()->getName(), NULL);
                GeneTree *duplicatedTree = new GeneTree(duplicatedRoot);
                duplicateChildren(tree->getRoot(), duplicatedRoot);
                return duplicatedTree;
            } else {
                return new GeneTree();
            }
        }
};
void GeneTree::init(GeneNode *root) {
    this->root = root;

    if (mergeVisitor != NULL) {
        delete mergeVisitor;
        mergeVisitor = NULL;
    }
}
GeneTree *GeneTree::duplicate() {
    TreeDuplicator v;
    return v(this);
}
GeneTree *GeneTree::duplicate(QList<int> &Start, QList<int> &End) {
    TreeDuplicator v(Start, End);
    return v(this);
}
void GeneTree::compareToStandardTree(GeneTree *standard, QSet<FindAllVisitor::NodeNamePair> &visitedNodes,
                                     GraphStatistics &stats) {
    CompareVisitor v(standard, visitedNodes, stats);
    v(this);
}
void GeneTree::compareToStandardTree(GeneTree *standard, GraphStatistics &stats) {
    QSet<FindAllVisitor::NodeNamePair> visitedNodes;
    compareToStandardTree(standard, visitedNodes, stats);
}
AbstractGraph *GeneTree::merge(AbstractGraph *t) {
    GeneTree *tree = dynamic_cast<GeneTree *>(t);

    if (mergeVisitor == NULL) {
        mergeVisitor = new GeneTree::MergeVisitor(this);
    }

    (*mergeVisitor)(tree);
    return this;
}
void GeneTree::merge(AbstractGraph *t, QList<int> &delimitersStart, QList<int> &delimitersEnd) {
    GeneTree *tree = dynamic_cast<GeneTree *>(t);

    if (mergeVisitor == NULL) {
        mergeVisitor = new GeneTree::MergeVisitor(this, delimitersStart, delimitersEnd);
    }

    (*mergeVisitor)(tree);
}
void GeneTree::mergeLeftovers() {
    if (mergeVisitor != NULL) {
        mergeVisitor->tryPerformingUnperformedNodes();
    }
}
void GeneTree::displayGraph(ATMProgressIFC *prg) {
    GeneDisplayVisitor *d = new GeneDisplayVisitor();
    (*d)(this);
    delete d;
    prg->displayGraph(this);
}
bool GeneTree::buildFromText(QString text, TwoLevelSelection *sel, QString *fileText,
                             AbstractTwoLevelAnnotator *tagger) {
    QStringList lines = text.split("\n", QString::SkipEmptyParts);
    QString s;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Error Processing Tree");
    GeneTree *newtree = new GeneTree();
    GeneNode *lastNode = NULL;
    int lastIndentation = -1;

    foreach (s, lines) {
        int indentation = 0;
        GeneNode *child = NULL;

        while (indentation < s.size() && s.at(indentation) == '>') {
            indentation++;
        }

        if (indentation == lastIndentation + 1) {
            //just keep old values for lastIndentation and lastNode
        } else if (indentation > lastIndentation + 1) {
            msgBox.setText(QString("Syntax error with indentation at line: \"%1\"").arg(s));
            msgBox.exec();
            newtree->deleteGraph();
            return false;
        } else {
            int diff = lastIndentation - indentation + 1;

            for (int i = 0; i < diff; i++) {
                if (lastNode == NULL) {
                    msgBox.setText(QString("Syntax error with indentation at line: \"%1\", parent to which to attach node not found").arg(
                                       s));
                    msgBox.exec();
                    newtree->deleteGraph();
                    return false;
                }

                lastNode = lastNode->parent;
            }

            lastIndentation = indentation - 1;
        }

        bool noSpouses = false;
        int l = s.indexOf('[') - 1;

        if (l < 0) {
            l = s.length() - 1;
            noSpouses = true;
        }

        QString nameString = Name(&s, indentation, l).getString();

        if (sel == NULL) {
            int pos = tagger->getNameIndexInAll(nameString);

            if (pos >= 0) {
                Name name = Name(fileText, pos, pos + nameString.length() - 1);
                child = new GeneNode(name, lastNode);

                if (lastNode == NULL) {
                    newtree->setRoot(child);
                    lastNode = child;
                }
            } else {
                msgBox.setText(QString("Name \"%1\" does not exist in the file at line \"%2\".").arg(nameString).arg(s));
                msgBox.exec();
                newtree->deleteGraph();
                return false;
            }
        } else {
            int i = sel->getNameIndex(nameString);

            if (i < 0) {
                msgBox.setText(QString("Name \"%1\" does not exist among the genealogical names tagged.").arg(nameString));
                msgBox.exec();
                newtree->deleteGraph();
                return false;
            } else {
                const MainSelectionList &names = sel->getNamesList();
                child = new GeneNode(Name(sel->getTextPointer(), names[i].first, names[i].second/*-1*/), lastNode);

                if (lastNode == NULL) {
                    newtree->setRoot(child);
                    lastNode = child;
                }
            }
        }

        bool finish = false;

        if (!noSpouses) {
            l++;

            do {
                int st = l + 1;
                l = s.indexOf('\t', l + 1);

                if (l < 0) {
                    l = s.indexOf(']');
                    finish = true;

                    if (l < 0) {
                        msgBox.setText(QString("Syntax error at line \"%1\" was expecting ']'").arg(s));
                        msgBox.exec();
                        newtree->deleteGraph();
                        return false;
                    }
                }

                QString nameString = Name(&s, st, l - 1).getString();

                if (sel == NULL) {
                    int pos = tagger->getNameIndexInAll(nameString);

                    if (pos >= 0) {
                        Name name = Name(fileText, pos, pos + nameString.length() - 1);
                        child->addSpouse(name);
                    } else {
                        msgBox.setText(QString("Spouse Name \"%1\" does not exist in the file at line \"%2\".").arg(nameString).arg(s));
                        msgBox.exec();
                        newtree->deleteGraph();
                        return false;
                    }
                } else {
                    int i = sel->getNameIndex(nameString);

                    if (i < 0) {
                        msgBox.setText(QString("Spouse Name \"%1\" does not exist among the genealogical names tagged.").arg(nameString));
                        msgBox.exec();
                        newtree->deleteGraph();
                        return false;
                    } else {
                        const MainSelectionList &names = sel->getNamesList();
                        Name name = Name(sel->getTextPointer(), names[i].first, names[i].second/*-1*/);
                        child->addSpouse(name);
                    }
                }
            } while (!finish);
        }

        lastIndentation = indentation;
        lastNode = child;
    }

    if (sel == NULL) {
        init(NULL);
        return true;
    } else if (newtree->getTreeNodesCount(true) == sel->getNamesList().size()) {
        sel->getGraph()->deleteGraph();
        sel->graph = newtree;
        return true;
    } else {
        msgBox.setText(
            QString("Nodes in the tree already constructed does not match those available in the genealogical tags"));
        msgBox.exec();
        newtree->deleteGraph();
        return false;
    }
}
QString GeneTree::getText() {
    QString *out_text = theSarf->out.string();
    QString s;
    theSarf->out.setString(&s);
    outputTree();
    theSarf->out.setString(out_text);
    return s;
}
void GeneTree::fillNullGraph(MainSelectionList &names, QString *text) {
    //if (this==NULL && names.size()>0) {
    GeneNode *root = new GeneNode(Name(text, names.at(0).first, names.at(0).second), NULL);
    init(root);

    for (int i = 1; i < names.size(); i++) {
        root->addChild(new GeneNode(Name(text, names.at(i).first, names.at(i).second), NULL));
    }

    //}
}
void GeneTree::fillNullGraph(Name & /*name*/) {
}

bool GeneTree::isRepresentativeOf(const MainSelectionList &list) { //very basic assumption for now
    int count = getTreeNodesCount(true);
    return count == list.size();
}
void GeneTree::fillTextPointers(QString *text) {
    FillTextVisitor v(text);
    v(this);
}
QAbstractItemModel *GeneTree::getTreeModel() {
    return new GeneItemModel(this);
}
void GeneTree::removeNameFromGraph(Name &name) {
    QString nameString = name.getString();
    GeneNode *n = findTreeNode(nameString, true);
    assert(n != NULL);

    if (n->getParent() == NULL) { //is root
        assert(getRoot() == n);

        if (n->spouses.size() > 0) {
            if (equalNames(n->toString(), nameString)) {
                n->name = n->spouses[0]->getName();
                n->spouses.removeAt(0);
            } else {
                for (int i = 0; i < n->spouses.size(); i++) {
                    if (equalNames(n->spouses[i]->getString(), n->toString())) {
                        n->spouses.removeAt(i);
                        break;
                    }
                }
            }
        } else if (n->children.size() > 0) {
            setRoot(n->children[0]);
            n->children[0]->parent = NULL;

            for (int i = 1; i < n->children.size(); i++) {
                n->children[0]->addChild(n->children[i]);
            }

            n->children[0]->parent = NULL;
            delete n;
        } else {
            deleteGraph();
            init(NULL);
            fillNullGraph(name);
        }
    } else {
        n->parent->children.removeOne(n);

        for (int i = 0; i < n->spouses.size(); i++) {
            n->parent->addChild(new GeneNode(n->spouses[i]->getName(), NULL));
        }

        for (int i = 0; i < n->children.size(); i++) {
            n->parent->addChild(n->children[i]);
        }

        delete n;
    }
}
void GeneTree::addNameToGraph(Name &name) {
    if (isEmpty()) {
        fillNullGraph(name);
    } else {
        root->addChild(new GeneNode(name, NULL));
    }
}
void GeneTree::writeToStream(QDataStream &out) {
    GeneTree *t = this;
    out << *t;
}
AbstractGraph *GeneTree::readFromStreamHelper(QDataStream &in) {
    GeneTree *t = this;
    in >> *t;
    return t;
}
GeneTree::~GeneTree() {
    if (mergeVisitor != NULL) {
        delete mergeVisitor;
    }
}
int SpouseGeneNode::getGraphHeight() const {
    if (parent == NULL) {
        return -1;
    }

    return parent->getGraphHeight();
}

class GenealogySegmentor {
    private:
        class StateInfo {
            public:
                QString *text;
                long lastEndPos;
                long startPos;
                long endPos;
                long nextPos;
                WordType lastType: 3;
                WordType currentType: 3;
                StateType currentState: 2;
                StateType nextState: 2;
                bool preceededByWaw: 1;
                bool preceededByLi: 1;
                bool preceededBygaveBirth: 1;
                bool sonsFoundName: 1;
                bool land: 1;
                bool male: 1;
                bool newNameNotProcessed: 1;
                DescentDirection descentDirection: 3;
                DescentDirection lastDescentDirection: 3;
                bool singularDescent: 1;
                //int unused:19;
                Name unProcessedName;
                PunctuationInfo previousPunctuationInfo, currentPunctuationInfo;
                void resetCurrentWordInfo() {
                    currentPunctuationInfo.reset();
                    preceededByWaw = false;
                    preceededByLi = false;
                    //descentDirection=UNDEFINED_DIRECTION;
                }
                QString getWord() {
                    return text->mid(startPos, endPos - startPos + 1);
                }
                void initialize(QString *text) {
                    resetCurrentWordInfo();
                    currentState = TEXT_S;
                    nextState = TEXT_S;
                    lastEndPos = 0;
                    startPos = 0;
                    previousPunctuationInfo.fullstop = true;
                    this->text = text;
                    preceededByWaw = false;
                    preceededByLi = false;
                    preceededBygaveBirth = false;
                    singularDescent = true;
                    lastDescentDirection = UNDEFINED_DIRECTION;
                    newNameNotProcessed = false;
                    land = false;
                    lastType = OTHER;
                }
                StateInfo(): unProcessedName(NULL, -1, -1) {
                    initialize(NULL);
                }
        };
        typedef TwoLevelSelection OutputData;
        typedef AbstractTwoLevelAnnotator::SelectionList OutputDataList;
        typedef TwoLevelSelection::MainSelection Selection;
        typedef TwoLevelSelection::MainSelectionList SelectionList;
        QList<int> delimitersStart, delimitersEnd;
        OutputDataList outputList;
        class StateData {
            public:
                int i0;
                GeneTree *globalTree;
                GeneTree *tree;
                GeneNode *last;
                QString lastName;
                QString currEdge;
                long startGene;
                OutputData *outputData;
            public:
                StateData() {
                    outputData = NULL;
                    initialize();
                    globalTree = NULL;
                }
                void initialize() {
                    if (outputData == NULL) {
                        outputData = new OutputData(new GeneTree());
                    } else {
                        outputData->clear();
                        outputData->setGraph(new GeneTree());
                    }

                    i0 = 0;
                    last = NULL;
                    tree = NULL;
                    startGene = -1;
                }
        };
    private:
        QString fileName;
        StateData currentData;
        StateInfo stateInfo;
        QString *text;
        ATMProgressIFC *prg;
        long text_size;

        inline bool conditionToOutput() {
            return currentData.tree->getTreeLevels() >= geneologyParameters.L_min ||
                   currentData.tree->getTreeNodesCount(true) >= geneologyParameters.N_min;
        }
        inline bool checkIfDisplay(bool keep = false) {
            bool ret_val = true;

            if (!keep) {
                if (conditionToOutput()) {
                    outputAndTag();
                    ret_val = false;
                } else {
                    currentData.tree->deleteGraph();
                    currentData.outputData->clear();
                    currentData.outputData->setGraph(new GeneTree());
                }

                currentData.last = NULL;
                currentData.tree = NULL;
                stateInfo.newNameNotProcessed = false;
                stateInfo.nextState = TEXT_S; //redundant
            }

            return ret_val;
        }
        inline bool doParaCheck() {
            int count = currentData.tree->getTreeNodesCount(true);
            display(QString("{keep? count %1}").arg(count));
            bool keep = count <= geneologyParameters.C_max && count > 1;
            bool ret_val = checkIfDisplay(keep);

            if (!keep) {
                if (currentData.tree != NULL) {
                    currentData.tree->deleteGraph();
                }

                currentData.tree = NULL;
                currentData.last = NULL;
                stateInfo.newNameNotProcessed = false;
                currentData.outputData->clear();
                currentData.outputData->setGraph(new GeneTree());
                stateInfo.nextState = TEXT_S;
            } else {
                if (conditionToOutput()) {
                    outputAndTag();
                    currentData.last = NULL;
                    currentData.tree = NULL;
                    stateInfo.newNameNotProcessed = false;
                    stateInfo.nextState = TEXT_S; //redundant
                    ret_val = false;
                } else {
                    currentData.i0 += geneologyParameters.step;
                    currentData.last = NULL;
                    stateInfo.nextState = NAME_S;
                }
            }

            stateInfo.descentDirection = UNDEFINED_DIRECTION;
            return ret_val;
        }
        inline bool doActionNewNameAndNullLast(Name &name) {
            bool ret_value = true;
            DescentDirection dir = stateInfo.descentDirection;
            ret_value = doParaCheck(); //nameList is cleared here, if needed

            if (currentData.outputData->getNamesList().size() > 0) {
                currentData.tree->deleteGraph();
                stateInfo.newNameNotProcessed = false;
                ;
                currentData.outputData->setGraph(new GeneTree());
            }

            currentData.outputData->addName(name);
            currentData.startGene = stateInfo.startPos;
            currentData.last = new GeneNode(name, NULL);
            currentData.tree = new GeneTree(currentData.last);
            currentData.i0 = 0;

            if (dir != UNDEFINED_DIRECTION) {
                if (dir == SON) {
                    stateInfo.nextState = SONS_S;
                } else {
                    stateInfo.nextState = NAME_S;
                }

                stateInfo.sonsFoundName = true;
                stateInfo.descentDirection = dir;
            } else {
                stateInfo.nextState = NAME_S;
            }

            return ret_value;
        }
        inline void addToTree(Name &name) {
            if (currentData.last != NULL) {
                switch (stateInfo.descentDirection) {
                    case SON:

                        //case UNDEFINED_DIRECTION:
                        if (!currentData.last->hasSpouse(name)) {
                            new GeneNode(name, currentData.last);
                        }

                        break;

                    case FATHER:
                        currentData.last->addParent(new GeneNode(name, NULL));
                        currentData.tree->updateRoot();
                        break;

                    case SPOUSE:
                        if (currentData.last != NULL && currentData.last->getName().isMarriageCompatible(name)) {
                            currentData.last->addSpouse(name);
                        } else {
                            DescentDirection temp = stateInfo.descentDirection;
                            stateInfo.descentDirection = stateInfo.lastDescentDirection;
                            addToTree(name); //TODO: check if useful, or needs more complex intervention
                            stateInfo.descentDirection = temp;
                            return;
                        }

                        break;

                    case SIBLING:
                        currentData.last->addSibling(new GeneNode(name, NULL));

                        if (currentData.last->getParent() != NULL) {
                            currentData.outputData->addName(name);
                        }

                        return;

                    default:
                        break;
                }
            }

            if (stateInfo.descentDirection != UNDEFINED_DIRECTION && currentData.last != NULL) {
                currentData.outputData->addName(name);
            }
        }
        bool getNextState(bool addCounters = true) {
            display(stateInfo.currentState);
            display(QString(" i0: %1 ").arg(currentData.i0));
            display(QString("curr: %1 ").arg(currentData.last->toString()));
            //display(QString("startGene: %1\n").arg(currentData.startGene));
            display("dir: ");
            display(stateInfo.descentDirection);
            display("\n");
            bool ret_val = true;
            Name name(stateInfo.text, stateInfo.startPos, stateInfo.endPos, stateInfo.male);

            switch (stateInfo.currentState) {
                case TEXT_S:
                    switch (stateInfo.currentType) {
                        case DC:
                            currentData.initialize();
                            stateInfo.newNameNotProcessed = false;
                            currentData.startGene = stateInfo.startPos;
                            currentData.tree = new GeneTree();
                            stateInfo.nextState = NAME_S;
                            break;

                        case NEW_NAME:
                            currentData.initialize();
                            stateInfo.newNameNotProcessed = false;
                            currentData.last = new GeneNode(name, NULL);
                            currentData.outputData->addName(name);
                            currentData.tree = new GeneTree(currentData.last);
                            currentData.startGene = stateInfo.startPos;
                            stateInfo.descentDirection = UNDEFINED_DIRECTION;
                            stateInfo.nextState = NAME_S;
                            break;

                        default:
                            stateInfo.nextState = TEXT_S;
                    }

                    break;

                case NAME_S:
                    if (!geneologyParameters.refined || !stateInfo.preceededByLi) {
                        switch (stateInfo.currentType) {
                            case DC:
                                if (stateInfo.newNameNotProcessed && currentData.last != NULL) {
                                    currentData.startGene = min(currentData.startGene, stateInfo.unProcessedName.getStart());
                                    addToTree(stateInfo.unProcessedName);
                                    stateInfo.newNameNotProcessed = false;

                                    if (stateInfo.singularDescent) {
                                        stateInfo.descentDirection = stateInfo.lastDescentDirection;
                                        stateInfo.singularDescent = false;
                                    }

                                    stateInfo.nextState = NAME_S;
                                } else {
                                    if (stateInfo.newNameNotProcessed) {
                                        stateInfo.newNameNotProcessed = false;
                                    }

                                    if (currentData.last == NULL) {
                                        currentData.last = currentData.tree->findTreeNode(currentData.lastName, stateInfo.descentDirection == SON ||
                                                                                          stateInfo.descentDirection == FATHER);
                                    }

                                    if (stateInfo.descentDirection == SON) {
                                        stateInfo.nextState = SONS_S;    //NAME_S;
                                    } else {
                                        stateInfo.nextState = NAME_S;
                                    }

                                    stateInfo.sonsFoundName = false;
                                }

                                break;

                            case ENDING_PUNC:
                                currentData.last = NULL;
                                stateInfo.nextState = NAME_S;
                                stateInfo.descentDirection = UNDEFINED_DIRECTION;
                                break;

                            case NEW_NAME: /*|| ((stateInfo.currentType==CORE_NAME || stateInfo.currentType==LEAF_NAME) && currentData.last==NULL)*/
                                if (stateInfo.preceededByWaw) {
                                    if (currentData.last != NULL && currentData.last->getParent() == NULL) {
                                        currentData.tree->deleteGraph();
                                        currentData.outputData->clear();
                                        currentData.outputData->setGraph(new GeneTree());
                                        display("{Waw resulted in deletion}\n");

                                        if (!stateInfo.previousPunctuationInfo.hasEndingPunctuation()) {
                                            currentData.last = NULL;
                                            currentData.tree = NULL;
                                            stateInfo.newNameNotProcessed = false;
                                            stateInfo.nextState = TEXT_S;
                                            //stateInfo.nextState=NAME_S;
                                        } else {
                                            currentData.last = new GeneNode(name, NULL);
                                            currentData.tree = new GeneTree(currentData.last);
                                            stateInfo.newNameNotProcessed = false;
                                            stateInfo.nextState = NAME_S;
                                        }
                                    } else {
                                        if (currentData.last != NULL) {
                                            //new GeneNode(name,currentData.last->getParent());
                                            addToTree(name);
                                            stateInfo.nextState = NAME_S;
                                        } else {
                                            ret_val = doActionNewNameAndNullLast(name); //choses also nextState
                                        }
                                    }
                                } else {
                                    if (stateInfo.descentDirection == UNDEFINED_DIRECTION) {
                                        if (!stateInfo.newNameNotProcessed) {
                                            if (currentData.tree->getTreeNodesCount() > 0) {
                                                stateInfo.newNameNotProcessed = true;
                                                stateInfo.unProcessedName = name;
                                                stateInfo.nextState = NAME_S;
                                            } else {
                                                currentData.last = new GeneNode(name, NULL);

                                                if (currentData.tree == NULL) {
                                                    currentData.tree = new GeneTree(currentData.last);
                                                } else {
                                                    currentData.tree->setRoot(currentData.last);
                                                }

                                                currentData.lastName = name.getString();
                                                currentData.outputData->addName(name);
                                                stateInfo.nextState = NAME_S;
                                            }
                                        } else {
                                            if (stateInfo.lastType == NEW_NAME) { //in cases similar to the start of 1 chronicles
                                                stateInfo.descentDirection = SON;

                                                if (currentData.tree->getTreeNodesCount() == 0) {
                                                    currentData.last = new GeneNode(stateInfo.unProcessedName, NULL);
                                                    currentData.tree->setRoot(currentData.last);
                                                    currentData.outputData->addName(stateInfo.unProcessedName);
                                                } else {
                                                    addToTree(stateInfo.unProcessedName);
                                                }

                                                addToTree(name);
                                                currentData.lastName = name.getString();
                                                currentData.last = currentData.tree->findTreeNode(currentData.lastName);
                                                stateInfo.nextState = NAME_S;
                                            } else  {
                                                doActionNewNameAndNullLast(name); //choses also nextState
                                            }
                                        }
                                    } else {
                                        if (currentData.last == NULL) {
                                            ret_val = doActionNewNameAndNullLast(name); //choses also nextState
                                        } else {
                                            if (currentData.tree == NULL) {
                                                currentData.tree = new GeneTree(currentData.last);
                                            }

                                            if (stateInfo.descentDirection != UNDEFINED_DIRECTION) {
                                                addToTree(name);
                                                currentData.last = currentData.tree->findTreeNode(name.getString(), true);
                                                currentData.i0 = 0;
                                                stateInfo.nextState = NAME_S;
                                            } else {
                                                doActionNewNameAndNullLast(name); //choses also nextState
                                            }
                                        }
                                    }
                                }

                                break;

                            /*case CORE_NAME:*/
                            case OTHER:
                                display(QString("{here threshold: %1}").arg(geneologyParameters.theta_0));

                                if (currentData.i0 >= geneologyParameters.theta_0) {
                                    ret_val = checkIfDisplay();
                                    stateInfo.nextState = TEXT_S;
                                } else {
                                    if (addCounters) {
                                        currentData.i0++;
                                    }

                                    stateInfo.nextState = NAME_S;
                                }

                                break;

                            case CORE_NAME:
                            case LEAF_NAME:
                                currentData.last = currentData.tree->findTreeNode(name.getString(), stateInfo.descentDirection == SON ||
                                                                                  stateInfo.descentDirection == FATHER);
                                stateInfo.nextState = SONS_S;
                                stateInfo.sonsFoundName = false;
                                break;

                            case PARA_PUNC:
                                ret_val = doParaCheck(); //choses also nextState
                                break;

                            case COLON_PUNC:
                                if (stateInfo.preceededBygaveBirth) {
                                    stateInfo.descentDirection = SON;
                                    stateInfo.singularDescent = false;
                                    stateInfo.nextState = SONS_S;
                                    break;
                                }

                            default:
                                stateInfo.nextState = NAME_S;
                        }

                        break;
                    } //in this way, code applies to both NAME_S and SONS_S bc we dont reach break

                case SONS_S:
                    if (!geneologyParameters.refined || !stateInfo.preceededByLi) {
                        switch (stateInfo.currentType) {
                            /*case DC:
                              if (stateInfo.preceededBygaveBirth)
                              stateInfo.descentDirection=SON;*/
                            case NEW_NAME:
                                addToTree(name);

                                if (stateInfo.descentDirection != UNDEFINED_DIRECTION) {
                                    currentData.i0 = 0;
                                    stateInfo.sonsFoundName = true;
                                }

                                stateInfo.nextState = SONS_S;
                                break;

                            case ENDING_PUNC:
                                //currentData.i0=0;
                                stateInfo.nextState = NAME_S;

                                if (stateInfo.descentDirection == SON /*&& stateInfo.precededBygaveBirth*/ &&
                                    !stateInfo.sonsFoundName) { //maybe name in next sentence
                                    stateInfo.descentDirection = SON;
                                } else {
                                    stateInfo.descentDirection = UNDEFINED_DIRECTION;
                                    currentData.last = NULL;
                                }

                                break;

                            case PARA_PUNC:
                                ret_val = doParaCheck(); //choses also nextState
                                break;

                            case CORE_NAME:
                            case LEAF_NAME:
                                currentData.last = currentData.tree->findTreeNode(name.getString(), stateInfo.descentDirection == SON ||
                                                                                  stateInfo.descentDirection == FATHER);
                                stateInfo.nextState = SONS_S;
                                break;

                            case COLON_PUNC:
                                if (stateInfo.preceededBygaveBirth) {
                                    stateInfo.descentDirection = SON;
                                    stateInfo.singularDescent = false;
                                    stateInfo.nextState = SONS_S;
                                    break;
                                }

                            default:
                                if (addCounters) {
                                    currentData.i0++;
                                }

                                stateInfo.nextState = SONS_S;
                        }

                        break;
                    } else {
                        if (stateInfo.preceededBygaveBirth) {
                            if (currentData.last != NULL) {
                                stateInfo.descentDirection = SON;

                                if (!currentData.tree->findTreeNode(name.getString(), true)) {
                                    currentData.last->addSpouse(name);//make sure no duplication occurs
                                    currentData.outputData->addName(name);
                                }

                                stateInfo.nextState = SONS_S;
                            } else {
                                stateInfo.descentDirection = UNDEFINED_DIRECTION;
                                stateInfo.nextState = NAME_S;
                            }
                        } else {
                            stateInfo.descentDirection = UNDEFINED_DIRECTION;
                            stateInfo.nextState = stateInfo.currentState;
                        }

                        break;
                    }

                default:
                    assert(false);
            }

            return ret_val;
        }
        inline bool result(WordType t, bool addCounters = true) {
            display(t);
            stateInfo.currentType = t;
            bool result = getNextState(addCounters);

            if (!currentData.currEdge.isEmpty()) {
                QString currWord = stateInfo.getWord();
                AbstractGeneNode *node = currentData.tree->findAbstractTreeNode(currWord, true);

                if (node != NULL) {
                    node->addEdgeText(currentData.currEdge);
                }

                currentData.currEdge = "";
            }

            return result;
        }
        bool proceedInStateMachine() {//does not fill stateInfo.currType
            long  finish;

            if (isNumber(stateInfo.text, stateInfo.startPos, finish)) {
                display("Number ");
                stateInfo.endPos = finish;
                stateInfo.nextPos = next_positon(stateInfo.text, finish, stateInfo.currentPunctuationInfo);
                delimitersStart.append(stateInfo.nextPos);
                //delimitersEnd.append(stateInfo.lastEndPos);
                display(stateInfo.text->mid(stateInfo.startPos, finish - stateInfo.startPos + 1) + ":");

                if (! result(OTHER, false)) {
                    return false;
                }

                if (!updateAndMoveToNewWord(false)) {
                    return true;
                }
            }

            GeneStemmer s(stateInfo.text, stateInfo.startPos);
            stateInfo.resetCurrentWordInfo();
            s();
            finish = max(s.info.finish, s.finish_pos);

            if (finish == stateInfo.startPos) {
                finish = getLastLetter_IN_currentWord(stateInfo.text, stateInfo.startPos);
            }

            stateInfo.endPos = finish;
            stateInfo.nextPos = next_positon(stateInfo.text, finish, stateInfo.currentPunctuationInfo);

            if (!s.name && stateInfo.currentPunctuationInfo.has_punctuation) {
                delimitersStart.append(stateInfo.nextPos);
                delimitersEnd.append(stateInfo.endPos);
            }

            display(stateInfo.text->mid(stateInfo.startPos, finish - stateInfo.startPos + 1) + ":");

            if (s.land) {
                stateInfo.land = true;

                if (!result(OTHER, false)) {
                    return false;
                }
            } else if (s.name) {
                long nextpos = stateInfo.nextPos;
                PunctuationInfo copyPunc = stateInfo.currentPunctuationInfo;

                if ((s.has_waw || s.has_li) && (stateInfo.currentState == NAME_S || stateInfo.currentState == SONS_S)) {
                    if (s.has_waw) {
                        display("waw ");
                    } else {
                        display("li ");
                    }

                    stateInfo.startPos = s.wawStart;
                    stateInfo.endPos = s.wawEnd;
                    stateInfo.nextPos = s.wawEnd + 1;
                    stateInfo.currentPunctuationInfo.reset();

                    if (!result(OTHER, false)) {
                        return false;
                    }

                    stateInfo.currentState = stateInfo.nextState;
                    stateInfo.lastEndPos = stateInfo.endPos;

                    if (s.has_waw) {
                        stateInfo.preceededByWaw = true;
                    } else {
                        stateInfo.preceededByLi = true;
                    }

                    stateInfo.land = false;
                }

                stateInfo.currentPunctuationInfo = copyPunc;
                stateInfo.startPos = s.startStem;
                stateInfo.endPos = s.finishStem;
                stateInfo.nextPos = nextpos;
                WordType type;
                QString word = stateInfo.getWord();
                bool searchSpouseCondition = stateInfo.descentDirection == SPOUSE ||
                                             stateInfo.descentDirection == UNDEFINED_DIRECTION || stateInfo.preceededBygaveBirth;
                GeneNode *node = NULL;

                if (currentData.tree != NULL) {
                    currentData.tree->findTreeNode(word, searchSpouseCondition);
                }

                if (node == NULL) {
                    type = NEW_NAME;
                } else if (node->isLeaf()) {
                    type = LEAF_NAME;
                } else {
                    type = CORE_NAME;
                }

                delimitersStart.append(stateInfo.nextPos);
                delimitersEnd.append(stateInfo.lastEndPos);
                stateInfo.male = s.male;

                /*if (stateInfo.currentType==NEW_NAME && type==NEW_NAME && stateInfo.descentDirection==UNDEFINED_DIRECTION)//in cases similar to the start of 1 chronicles
                  stateInfo.descentDirection=SON;*/
                if (stateInfo.land) {
                    stateInfo.land = false;
                    type = OTHER;
                }

                Name currName(text, stateInfo.startPos, stateInfo.endPos);

                if (type == LEAF_NAME || type == CORE_NAME) {
                    AbstractGeneNode *node = currentData.tree->findAbstractTreeNode(word, true);
                    assert(node != NULL);

                    if (equalNames(currentData.lastName, node->getParent()->getString())) {
                        GeneTree::MergeVisitor::appendEdgeName(node, node->getName(), currName, &delimitersStart, &delimitersEnd);
                    }
                } else {
                    if (!currentData.lastName.isEmpty()) {
                        AbstractGeneNode *lastNode = NULL;

                        if (currentData.tree != NULL) {
                            currentData.tree->findAbstractTreeNode(currentData.lastName, true);
                        }

                        if (lastNode != NULL) {
                            currentData.currEdge = GeneTree::MergeVisitor::getEdge(lastNode->getName(), currName, &delimitersStart, &delimitersEnd);
                        }
                    }
                }

                if (!(result(type))) {
                    return false;
                }

                if (type == NEW_NAME && stateInfo.singularDescent) {
                    stateInfo.singularDescent = false;
                    stateInfo.descentDirection = stateInfo.lastDescentDirection;
                }

                currentData.lastName = word;
            } else if (s.descentDir != UNDEFINED_DIRECTION) {
                stateInfo.land = false;

                if (!stateInfo.singularDescent) {
                    stateInfo.lastDescentDirection = stateInfo.descentDirection;
                } else {
                    stateInfo.lastDescentDirection = UNDEFINED_DIRECTION;
                }

                stateInfo.singularDescent = !s.pluralDescent;
                stateInfo.descentDirection = s.descentDir;

                if (s.descentDir == FATHER /*&& s.pluralDescent*/ && stateInfo.preceededBygaveBirth) {
                    stateInfo.descentDirection = SON;
                }

                /*if (stateInfo.newNameNotProcessed && s.relativeSuffix && stateInfo.currentState!=TEXT_S) {
                  addToTree(stateInfo.unProcessedName);
                  stateInfo.descentDirection=stateInfo.lastDescentDirection;
                  stateInfo.singularDescent=false;
                  stateInfo.newNameNotProcessed=false;
                  stateInfo.nextState=stateInfo.currentState;
                  } else {*/
                if (!result(DC)) {
                    return false;
                }

                //}
                if (!stateInfo.preceededBygaveBirth) {
                    stateInfo.preceededBygaveBirth = s.gaveBirth;
                }
            } else {
                stateInfo.land = false;
                stateInfo.preceededByWaw = false;
                stateInfo.preceededByLi = false;

                if (!result(OTHER)) {
                    return false;
                }
            }

            stateInfo.preceededByWaw = false;
            stateInfo.preceededByLi = false;

            if (stateInfo.currentPunctuationInfo.hasEndingPunctuation() ||
                stateInfo.currentPunctuationInfo.hasParagraphPunctuation() || stateInfo.currentPunctuationInfo.colon) {
                if (!stateInfo.currentPunctuationInfo.colon) {
                    stateInfo.land = false;
                    stateInfo.lastDescentDirection = UNDEFINED_DIRECTION;
                    stateInfo.singularDescent = false;
                    stateInfo.newNameNotProcessed = false;
                }

                stateInfo.currentState = stateInfo.nextState;
                stateInfo.startPos = stateInfo.endPos + 1;
                stateInfo.endPos = stateInfo.nextPos - 1;
                WordType type;

                if (stateInfo.currentPunctuationInfo.hasParagraphPunctuation()) {
                    type = PARA_PUNC;
                } else if (stateInfo.currentPunctuationInfo.colon) {
                    type = COLON_PUNC;
                } else {
                    type = ENDING_PUNC;
                }

                if (!result(type, false)) {
                    return false;
                }

                stateInfo.preceededBygaveBirth = false;
                return true;
            } else {
                return true;
            }
        }
        void outputAndTag() {
            long geneStart = currentData.startGene, geneEnd = stateInfo.endPos;
            prg->tag(geneStart, geneEnd - geneStart + 1, Qt::darkYellow, false);
            GeneTagVisitor v(prg);
            v(currentData.tree);

            if (currentData.globalTree == NULL) {
                currentData.tree->outputTree();
                GeneTree *duplicate = currentData.tree->duplicate(delimitersStart, delimitersEnd);
                currentData.globalTree = duplicate;
            } else {
                currentData.globalTree->merge(currentData.tree, delimitersStart, delimitersEnd);
                currentData.tree->outputTree();
            }

            currentData.outputData->setGraph(currentData.tree);
            currentData.outputData->setMainInterval(currentData.startGene, stateInfo.endPos);
            outputList.append(*currentData.outputData);
            currentData.outputData->setGraph(new GeneTree());
        }
        inline int commonNames(const SelectionList &list1, const SelectionList &list2, QSet<int> &visitedTags, int &allCommon) {
            QSet<int> visitedTags2;
            int common = 0;
            allCommon = 0;

            for (int i = 0; i < list1.size(); i++) {
                bool found = false, allCommonFound = false;
                QString s1 = Name(text, list1[i].first, list1[i].second).getString();

                for (int j = 0; j < list2.size(); j++) {
                    QString s2 = Name(text, list2[j].first, list2[j].second).getString();

                    if (equalNames(s1, s2)) {
                        if (!allCommonFound) {
                            allCommonFound = true;
                            allCommon++;
                        }

                        if (!visitedTags2.contains(j) && !visitedTags.contains(i)) {
                            found = true;
                            visitedTags.insert(i);
                            visitedTags2.insert(j);
                            break;
                        }
                    }
                }

                if (found) {
                    common++;
                }
            }

            return common;
        }
        inline void modifySizeStatistics(int newEntry, int &max, int &min, int &sum, int &count) {
            if (newEntry > max) {
                max = newEntry;
            }

            if (newEntry < min || min == 0) {
                min = newEntry;
            }

            sum += newEntry;
            count++;
        }

        int calculateStatisticsOrAnotate() {
#define MERGE_GLOBAL_TREE               if (globalTree==NULL) { \
    globalTree=tags[i].getGraph()->duplicateTree(); \
} else { \
    globalTree->mergeTrees(tags[i].getTree()); \
} /*\
    globalTree->displayTree(prg);*/
#define MERGE_LOCAL_TREES               if (localMergedGraph==NULL) { \
    localMergedGraph=dynamic_cast<GeneTree*>(outputList[j].getGraph()->duplicate());\
} else {\
    localMergedGraph->merge(outputList[j].getGraph());\
}
#define COMPARE_TO_LOCAL_MERGED_TREE    GeneTree::GraphStatistics stats; \
                    if (localMergedGraph!=NULL) { \
                        if (i>=tags.count()) \
                        i=tags.count()-1; \
                        int countCorrect=tags[i].getNamesList().size(); \
                        numNames+=countCorrect; \
                        GeneTree * tagTree=dynamic_cast<GeneTree *>(tags[i].getGraph());\
                        localMergedGraph->compareToStandardTree(tagTree,stats);\
                        graphFoundRecallList.append(stats.foundRecall*countCorrect);\
                        graphFoundPrecisionList.append(stats.foundPrecision*countCorrect);\
                        graphContextRecallList.append(stats.contextRecall*countCorrect);\
                        graphContextPrecisionList.append(stats.contextPrecision*countCorrect);\
                        graphNeigborhoodRecallList.append(stats.neigborhoodRecall*countCorrect);\
                        graphNeighborhoodPrecisionList.append(stats.neigborhoodPrecision*countCorrect);\
                        graphSpousesRecallList.append(stats.spousesRecall*countCorrect);\
                        graphSpousesPrecisionList.append(stats.spousesPrecision*countCorrect);\
                        graphChildrenRecallList.append(stats.childrenRecall*countCorrect);\
                        graphChildrenPrecisionList.append(stats.childrenPrecision*countCorrect);\
                        localMergedGraph->deleteGraph(); \
                        localMergedGraph=NULL;\
                        if (geneologyParameters.detailed_statistics) { \
                            theSarf->displayed_error <<">Graph:\t"<<stats.foundRecall<<"\t"<<stats.neigborhoodRecall<<"\t"<<stats.contextRecall<<"\t"<<stats.spousesRecall<<"\t"<<stats.childrenRecall<<"\n"; \
                        } \
                    }
            OutputDataList tags;
            QList<int> common_i, common_j;
            QVector<double> boundaryRecallList, boundaryPrecisionList,
                    graphFoundRecallList, graphFoundPrecisionList,
                    graphContextRecallList, graphContextPrecisionList,
                    graphNeigborhoodRecallList, graphNeighborhoodPrecisionList,
                    graphSpousesRecallList, graphSpousesPrecisionList,
                    graphChildrenRecallList, graphChildrenPrecisionList,
                    underGraphFoundRecallList, underGraphFoundPrecisionList,
                    underGraphContextRecallList, underGraphContextPrecisionList,
                    underGraphNeigborhoodRecallList, underGraphNeighborhoodPrecisionList,
                    underGraphSpousesRecallList, underGraphSpousesPrecisionList,
                    underGraphChildrenRecallList, underGraphChildrenPrecisionList;
            GeneTree *globalTree = new GeneTree(), * localMergedGraph = NULL;
            QFile file(QString("%1.tags").arg(fileName).toStdString().data());

            if (file.open(QIODevice::ReadOnly)) {
                QDataStream in(&file);   // we will serialize the data into the file
                tags.readFromStream(in, globalTree); //globalTree provided but will not be in use after call just duplicated
                in >> *globalTree;
                file.close();
            } else {
                _error << "Annotation File does not exist\n";

                if (file.open(QIODevice::WriteOnly)) {
                    /*for (int i=0;i<timeVector->size();i++) {
                                                                          tags.append(TimeTaggerDialog::Selection(timeVector->));
                                                                          }*/
                    QDataStream out(&file);   // we will serialize the data into the file
                    out << outputList;
                    file.close();
                    _error << "Annotation File has been written from current detected expressions, Correct it before use.\n";
                }

                return -1;
            }

            qSort(tags.begin(), tags.end());

            for (int i = 0; i < tags.size(); i++) {
                tags[i].setText(text);
                FillTextVisitor v(text);
                GeneTree *tree = dynamic_cast<GeneTree *>(tags[i].getGraph());
                v(tree);
            }

            FillTextVisitor v(text);
            v(globalTree);
            int i = 0, j = 0, numNames = 0, underNumNames = 0, maxTag = 0, minTag = 0, sumTag = 0, maxOutput = 0, minOutput = 0,
                sumOutput = 0, countTag = 0, countOutput = 0, countCorrectOutput = 0;
            QSet<FindAllVisitor::NodeNamePair> visitedNodes;
            QSet<int> visitedTags;

            while (i < tags.size() && j < outputList.size()) {
                int start1 = tags[i].getMainStart(), end1 = tags[i].getMainEnd(),
                    start2 = outputList[j].getMainStart(), end2 = outputList[j].getMainEnd();

                if (overLaps(start1, end1, start2, end2) && start1 != end2) {
                    int allCommonCount;
                    int countCommon = commonNames(tags[i].getNamesList(), outputList[j].getNamesList(), visitedTags, allCommonCount);
                    int countCorrect = tags[i].getNamesList().size();
                    int countDetected = outputList[j].getNamesList().size();

                    if (countCommon > countDetected) {
                        qDebug() << text->mid(start1, end1 - start1 + 1) << "\t" << text->mid(start2, end2 - start2 + 1) << "\t";
                        visitedTags.clear();
                        countCommon = commonNames(tags[i].getNamesList(), outputList[j].getNamesList(), visitedTags, allCommonCount);
                        assert(false);
                    }

                    bool foundI = common_i.contains(i), foundJ = common_j.contains(j);

                    if (!foundI /*&& !foundJ*/) { //so that merged parts will not be double counted
                        common_i.append(i);
                    }

                    if (!foundJ) { //common_i and common_j now are not same size, bc recall and precision need different treatment for overlap
                        common_j.append(j);
                    }

                    if (countCorrect > 0) {
                        underNumNames += countCorrect;
                        boundaryRecallList.append((double)countCommon / countCorrect * countCorrect);
                        boundaryPrecisionList.append((double)allCommonCount / countDetected * countCorrect);
                        GeneTree::GraphStatistics stats;
                        GeneTree *outputTree = dynamic_cast<GeneTree *>(outputList[j].getGraph());
                        GeneTree *tagTree = dynamic_cast<GeneTree *>(tags[i].getGraph());
                        outputTree->compareToStandardTree(tagTree, stats);
                        underGraphFoundRecallList.append(stats.foundRecall * countCorrect);
                        underGraphFoundPrecisionList.append(stats.foundPrecision * countCorrect);
                        underGraphContextRecallList.append(stats.contextRecall * countCorrect);
                        underGraphContextPrecisionList.append(stats.contextPrecision * countCorrect);
                        underGraphNeigborhoodRecallList.append(stats.neigborhoodRecall * countCorrect);
                        underGraphNeighborhoodPrecisionList.append(stats.neigborhoodPrecision * countCorrect);
                        underGraphSpousesRecallList.append(stats.spousesRecall * countCorrect);
                        underGraphSpousesPrecisionList.append(stats.spousesPrecision * countCorrect);
                        underGraphChildrenRecallList.append(stats.childrenRecall * countCorrect);
                        underGraphChildrenPrecisionList.append(stats.childrenPrecision * countCorrect);
                        countCorrectOutput += outputTree->getTreeNodesCount(true);
                        MERGE_LOCAL_TREES
                    } else {
                        boundaryRecallList.append(0);
                        boundaryPrecisionList.append(0);
                    }

                    if (geneologyParameters.detailed_statistics) {
                        theSarf->displayed_error    <</*text->mid(start1,end1-start1+1)*/i << "\t"
                                                    <</*text->mid(start2,end2-start2+1)*/j << "\t"
                                                    << countCommon << "/" << countCorrect << "\t" << allCommonCount << "/" << countDetected << "\n";
                    }

                    if (end1 <= end2) {
                        visitedNodes.clear();
                        visitedTags.clear();
                        //MERGE_GLOBAL_TREE
                        COMPARE_TO_LOCAL_MERGED_TREE
                        modifySizeStatistics(countCorrect, maxTag, minTag, sumTag, countTag);
                        i++;
                    }

                    if (end2 <= end1) {
                        modifySizeStatistics(countDetected, maxOutput, minOutput, sumOutput, countOutput);
                        j++;
                    }
                } else if (before(start1, end1, start2, end2)) {
                    if (geneologyParameters.detailed_statistics) {
                        theSarf->displayed_error    <</*text->mid(start1,end1-start1+1)*/i << "\t"
                                                    << "-----\n";
                    }

                    visitedNodes.clear();
                    visitedTags.clear();
                    //MERGE_GLOBAL_TREE
                    COMPARE_TO_LOCAL_MERGED_TREE
                    modifySizeStatistics(tags[i].getNamesList().size(), maxTag, minTag, sumTag, countTag);
                    i++;
                } else if (after(start1, end1, start2, end2)) {
                    if (geneologyParameters.detailed_statistics) {
                        theSarf->displayed_error    << "-----\t"
                                                    <</*text->mid(start2,end2-start2+1)*/j << "\n";
                    }

                    modifySizeStatistics(outputList[j].getNamesList().size(), maxOutput, minOutput, sumOutput, countOutput);
                    /*if (outputList[j].getNamesList().size()==2) {
                      Name n1(text,outputList[j].getNamesList()[0].first,outputList[j].getNamesList()[0].second);
                      qDebug()<<n1.getString();
                      Name n2(text,outputList[j].getNamesList()[1].first,outputList[j].getNamesList()[1].second);
                      qDebug()<<n2.getString();
                      }*/
                    j++;
                }
            }

            COMPARE_TO_LOCAL_MERGED_TREE

            while (i < tags.size()) {
                if (geneologyParameters.detailed_statistics) {
                    //int start1=tags[i].getMainStart(),end1=tags[i].getMainEnd();
                    theSarf->displayed_error <</*text->mid(start1,end1-start1+1)*/i << "\t"
                                             << "-----\n";
                }

                //MERGE_GLOBAL_TREE
                modifySizeStatistics(tags[i].getNamesList().size(), maxTag, minTag, sumTag, countTag);
                i++;
            }

            while (j < outputList.size()) {
                if (geneologyParameters.detailed_statistics) {
                    //int start2=outputList[j].getMainStart(),end2=outputList[j].getMainEnd();
                    theSarf->displayed_error << "-----\t"
                                             <</*text->mid(start2,end2-start2+1)*/j << "\n";
                }

                modifySizeStatistics(outputList[j].getNamesList().size(), maxOutput, minOutput, sumOutput, countOutput);
                j++;
            }

            /*int tagNamesCount=0;
              for (int i=0;i<tags.size();i++)
              tagNamesCount+=tags[i].getNamesList().size();*/
//assert(common_i.size()==common_j.size());
            int commonCount = common_i.size();
            int allCommonCount = common_j.size();
            double segmentationRecall = (double)commonCount / tags.size(),
                   segmentationPrecision = (double)allCommonCount / outputList.size(),
                   underSegmentationRatio = (double)commonCount / allCommonCount,
                   boundaryRecall = sum(boundaryRecallList) / numNames,
                   boundaryPrecision = sum(boundaryPrecisionList) / underNumNames,
                   graphFoundRecall = sum(graphFoundRecallList) / numNames,
                   graphFoundPrecision = sum(graphFoundPrecisionList) / numNames,
                   graphNeighborhoodRecall = sum(graphNeigborhoodRecallList) / numNames,
                   graphNeighborhoodPrecision = sum(graphNeighborhoodPrecisionList) / numNames,
                   graphContextRecall = sum(graphContextRecallList) / numNames,
                   graphContextPrecision = sum(graphContextPrecisionList) / numNames,
                   graphSpousesRecall = sum(graphSpousesRecallList) / numNames,
                   graphSpousesPrecision = sum(graphSpousesPrecisionList) / numNames,
                   graphChildrenRecall = sum(graphChildrenRecallList) / numNames,
                   graphChildrenPrecision = sum(graphChildrenPrecisionList) / numNames,
                   underBoundaryRecall = sum(boundaryRecallList) / underNumNames,
                   underBoundaryPrecision = sum(boundaryPrecisionList) / underNumNames,
                   underGraphFoundRecall = sum(underGraphFoundRecallList) / underNumNames,
                   underGraphFoundPrecision = sum(underGraphFoundPrecisionList) / underNumNames,
                   underGraphNeighborhoodRecall = sum(underGraphNeigborhoodRecallList) / underNumNames,
                   underGraphNeighborhoodPrecision = sum(underGraphNeighborhoodPrecisionList) / underNumNames,
                   underGraphContextRecall = sum(underGraphContextRecallList) / underNumNames,
                   underGraphContextPrecision = sum(underGraphContextPrecisionList) / underNumNames,
                   underGraphSpousesRecall = sum(underGraphSpousesRecallList) / underNumNames,
                   underGraphSpousesPrecision = sum(underGraphSpousesPrecisionList) / underNumNames,
                   underGraphChildrenRecall = sum(underGraphChildrenRecallList) / underNumNames,
                   underGraphChildrenPrecision = sum(underGraphChildrenPrecisionList) / underNumNames;
            int graphMergedSize = currentData.globalTree->getTreeNodesCount(true),
                graphTagsSize = globalTree->getTreeNodesCount(true);
            GeneTree::GraphStatistics globalStats;
            currentData.globalTree->compareToStandardTree(globalTree, globalStats);
            globalTree->displayGraph(prg);
            currentData.globalTree->deleteGraph();
            globalTree->deleteGraph();
            theSarf->displayed_error << "-------------------------\n"
                                     << "Segmentation:\n"
                                     << "\trecall=\t\t" << commonCount << "/" << tags.size() << "=\t" << segmentationRecall << "\n"
                                     << "\tprecision=\t\t" << allCommonCount << "/" << outputList.size() << "=\t" << segmentationPrecision << "\n"
                                     << "\tunder-segmentation=\t" << commonCount << "/" << allCommonCount << "=\t" << underSegmentationRatio << "\n"
                                     << "Boundary (Min-Boundaries):\n"
                                     << "\trecall=\t\t" << underBoundaryRecall << "\n"
                                     << "\tprecision=\t\t" << underBoundaryPrecision << "\n"
                                     << "Boundary (Max-Boundaries):\n"
                                     << "\trecall=\t\t" << boundaryRecall << "\n"
                                     << "\tprecision=\t\t" << boundaryPrecision << "\n"
                                     << "Local Graphs (Min-boundaries):\n"
                                     << "\tfound:\n"
                                     << "\t\trecall=\t" << underGraphFoundRecall << "\n"
                                     << "\t\tprecision=\t" << underGraphFoundPrecision << "\n"
                                     << "\tneighbors:\n"
                                     << "\t\trecall=\t" << underGraphNeighborhoodRecall << "\n"
                                     << "\t\tprecision=\t" << underGraphNeighborhoodPrecision << "\n"
                                     << "\tcontext:\n"
                                     << "\t\trecall=\t" << underGraphContextRecall << "\n"
                                     << "\t\tprecision=\t" << underGraphContextPrecision << "\n"
                                     << "\tspouses:\n"
                                     << "\t\trecall=\t" << underGraphSpousesRecall << "\n"
                                     << "\t\tprecision=\t" << underGraphSpousesPrecision << "\n"
                                     << "\tchildren:\n"
                                     << "\t\trecall=\t" << underGraphChildrenRecall << "\n"
                                     << "\t\tprecision=\t" << underGraphChildrenPrecision << "\n"
                                     << "Local Graphs (Max-boundaries):\n"
                                     << "\tfound:\n"
                                     << "\t\trecall=\t" << graphFoundRecall << "\n"
                                     << "\t\tprecision=\t" << graphFoundPrecision << "\n"
                                     << "\tneighbors:\n"
                                     << "\t\trecall=\t" << graphNeighborhoodRecall << "\n"
                                     << "\t\tprecision=\t" << graphNeighborhoodPrecision << "\n"
                                     << "\tcontext:\n"
                                     << "\t\trecall=\t" << graphContextRecall << "\n"
                                     << "\t\tprecision=\t" << graphContextPrecision << "\n"
                                     << "\tspouses:\n"
                                     << "\t\trecall=\t" << graphSpousesRecall << "\n"
                                     << "\t\tprecision=\t" << graphSpousesPrecision << "\n"
                                     << "\tchildren:\n"
                                     << "\t\trecall=\t" << graphChildrenRecall << "\n"
                                     << "\t\tprecision=\t" << graphChildrenPrecision << "\n"
                                     << "Global Graph:\n"
                                     << "\tfound:\n"
                                     << "\t\trecall=\t" << globalStats.foundRecall << "\n"
                                     << "\t\tprecision=\t" << globalStats.foundPrecision << "\n"
                                     << "\tneighbors:\n"
                                     << "\t\trecall=\t" << globalStats.neigborhoodRecall << "\n"
                                     << "\t\tprecision=\t" << globalStats.neigborhoodPrecision << "\n"
                                     << "\tcontext:\n"
                                     << "\t\trecall=\t" << globalStats.contextRecall << "\n"
                                     << "\t\tprecision=\t" << globalStats.contextPrecision << "\n"
                                     << "\tspouses:\n"
                                     << "\t\trecall=\t" << globalStats.spousesRecall << "\n"
                                     << "\t\tprecision=\t" << globalStats.spousesPrecision << "\n"
                                     << "\tchildren:\n"
                                     << "\t\trecall=\t" << globalStats.childrenRecall << "\n"
                                     << "\t\tprecision=\t" << globalStats.childrenPrecision << "\n"
                                     << " Local Genealogies Size (Annotation - Output):\n"
                                     << "\tminimum=\t\t" << minTag << "\t" << minOutput << "\n"
                                     << "\taverage=\t\t" << (double)sumTag / countTag << "\t" << (double)sumOutput / countOutput << "\n"
                                     << "\tmaximum=\t\t" << maxTag << "\t" << maxOutput << "\n"
                                     << "\ttotal size=\t\t"  << sumTag << "\t" << sumOutput << "\n"
//<< "\ttotal matched size=\t\t"<<countCorrectOutput<<"\n"
                                     << " Global Graph Size (Annotation - Output):\n"
                                     << "\tAnnotation=\t" << graphTagsSize << "\n"
                                     << "\tOutput=\t\t" << graphMergedSize << "\n";

            for (int i = 0; i < tags.size(); i++) {
                tags[i].getGraph()->deleteGraph();
            }

            for (int i = 0; i < outputList.size(); i++) {
                outputList[i].getGraph()->deleteGraph();
            }

            return 0;
#undef MERGE_GLOBAL_TREE
        }
        inline bool updateAndMoveToNewWord(bool updatePunctuation = true) {
            stateInfo.currentState = stateInfo.nextState;
            stateInfo.startPos = stateInfo.nextPos;
            stateInfo.lastEndPos = stateInfo.endPos;

            if (updatePunctuation) {
                stateInfo.previousPunctuationInfo = stateInfo.currentPunctuationInfo;
            }

            stateInfo.lastType = stateInfo.currentType;
            prg->report((double)stateInfo.startPos / text_size * 100 + 0.5);

            if (stateInfo.startPos >= text_size - 1) {
                return false;
            }

            return true;
        }
        int segmentHelper(QString *text, ATMProgressIFC *prg) {
            this->prg = prg;
            delimitersStart.clear();
            delimitersEnd.clear();

            if (text == NULL) {
                return -1;
            }

            text_size = text->size();
            currentData.initialize();
            prg->startTaggingText(*text);
            int geneCounter = 1;
            stateInfo.initialize(text);

            while (stateInfo.startPos < text->length() && isDelimiter(text->at(stateInfo.startPos))) {
                stateInfo.startPos++;
            }

            prg->setCurrentAction("Parsing Biblical Text");

            for (; stateInfo.startPos < text_size;) {
                if (!(proceedInStateMachine())) {
                    geneCounter++;
                }

                if (!updateAndMoveToNewWord()) {
                    break;
                }
            }

            if (stateInfo.currentState != TEXT_S) {
                if (!checkIfDisplay()) {
                    geneCounter++;
                }
            }

            if (currentData.outputData != NULL) {
                delete currentData.outputData;
            }

            prg->report(100);
            prg->finishTaggingText();
            currentData.globalTree->displayGraph(prg);
            calculateStatisticsOrAnotate();
            return 0;
        }

    public:
        int segment(QString input_str, ATMProgressIFC *prg)  {
            fileName = input_str;
            QFile input(input_str);

            if (!input.open(QIODevice::ReadOnly)) {
                theSarf->out << "File not found\n";
                return 1;
            }

            QTextStream file(&input);
            file.setCodec("utf-8");
            text = new QString(file.readAll());

            if (text->isNull()) {
                theSarf->out << "file error:" << input.errorString() << "\n";
                return 1;
            }

            if (text->isEmpty()) {//ignore empty files
                theSarf->out << "empty file\n";
                return 0;
            }

            return segmentHelper(text, prg);
        }
        int segment(QString *text, ATMProgressIFC *prg)  {
            fileName = "";
            return segmentHelper(text, prg);
        }
};

int genealogyHelper(QString input_str, ATMProgressIFC *prgs) {
    input_str = input_str.split("\n")[0];
    GenealogySegmentor s;
    s.segment(input_str, prgs);
    return 0;
}


