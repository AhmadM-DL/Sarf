#include <QQueue>
#include "graph.h"
#include "abstractTwoLevelAnnotator.h"
#include "hadithDagGraph.h"
#include "hadithChainGraph.h"
#include "hadithInterAnnotatorAgreement.h"
#include "Math_functions.h"

void ColorIndices::unUse(unsigned int bit) { //unuse and clear color bit for all nodes in graph
    assert(bit < maxBits());
    usedBits &= (~(1 << bit));
    int max = graph->all_nodes.size();

    for (int i = 0; i < max; i++) {
        NarratorNodeIfc *n = graph->all_nodes[i];

        if (n != NULL) {
            n->resetVisited(bit);
        }
    }

    if (nextUnused > bit) {
        nextUnused = bit;
    }
}
void ColorIndices::setAllNodesVisited(unsigned int bit) {
    assert(bit < maxBits());
    int max = graph->all_nodes.size();

    for (int i = 0; i < max; i++) {
        NarratorNodeIfc *n = graph->all_nodes[i];

        if (n != NULL) {
            n->setVisited(bit);
        }
    }
}

GraphVisitorController::GraphVisitorController(NodeVisitor *visitor, NarratorGraph *graph, unsigned int visitIndex,
                                               unsigned int finishIndex, bool keep_track_of_edges, bool merged_edges_as_one) { //assumes keep_track_of_nodes by default
    construct(visitor, graph, keep_track_of_edges, true, merged_edges_as_one);
    this->visitIndex = visitIndex; //assumed already cleared
    graph->colorGuard.use(visitIndex);
    this->finishIndex = finishIndex; //assumed already cleared
    graph->colorGuard.use(finishIndex);
}
GraphVisitorController::GraphVisitorController(NodeVisitor *visitor, NarratorGraph *graph, bool keep_track_of_edges,
                                               bool merged_edges_as_one) {
    construct(visitor, graph, keep_track_of_edges, true, merged_edges_as_one);

    if (keep_track_of_nodes && graph != NULL) {
        visitIndex = graph->colorGuard.getNextUnused();
        graph->colorGuard.use(visitIndex);
        finishIndex = graph->colorGuard.getNextUnused();
        graph->colorGuard.use(finishIndex);
    }
}
void GraphVisitorController::construct(NodeVisitor *visitor, NarratorGraph *graph, bool keep_track_of_edges,
                                       bool keep_track_of_nodes, bool merged_edges_as_one) {
    assert(!(!keep_track_of_edges &&
             !keep_track_of_nodes)); //i dont want both to be false at same time=> no exponential traversal
    assert(!(keep_track_of_edges && !merged_edges_as_one && graph == NULL)); //bc we cannot clear them after traversal if so
    graphBuilt = (graph != NULL ? graph->isBuilt() :
                  true); //we are assuming if we are doing a traversal without a graph then graph has been built
    firstCall = true;
    disableFillChildren = false;
    this->visitor = visitor;
    assert(visitor != NULL);
    this->visitor->controller = this;
    this->keep_track_of_edges = keep_track_of_edges ||
                                merged_edges_as_one; //if merged_edges_as_one is needed we must keep_track_of_edges
    this->keep_track_of_nodes = keep_track_of_nodes;
    this->merged_edges_as_one = merged_edges_as_one;
    this->graph = graph;
    init();
}

void GraphVisitorController::init() {
    if (firstCall) {
        firstCall = false;
    } else {
        if (graph != NULL) {
            graph->colorGuard.unUse(visitIndex); //to clear color bits
            graph->colorGuard.unUse(finishIndex);
            graph->colorGuard.use(visitIndex);
            graph->colorGuard.use(finishIndex);
            visitedMap = NULL;
        } else {
            visitedMap = new ColorMap();
        }
    }

    if (keep_track_of_edges) {
        edgeMap.clear();
    }

    parentStack.clear();
}
void GraphVisitorController::initialize(int direction) {
    init();
    this->direction = direction > 0;
    visitor->initialize();
}
void GraphVisitorController::finish() {
    if (graph != NULL) {
        graph->colorGuard.unUse(visitIndex);
        graph->colorGuard.unUse(finishIndex);
    } else {
        delete visitedMap;
    }

    visitor->finish();
}

void LoopBreakingVisitor::reMergeNodes(NarratorNodeIfc *n) {
    if (n == NULL) {
        return;
    }

    if (n->isChainNode()) {
        return;    //unable to resolve
    }

    assert(n->isGraphNode());
    GraphNarratorNode *g = dynamic_cast<GraphNarratorNode *>(n);
    QList<GroupNode *> groups;
    int size = g->size();

    for (int i = 0; i < size; i++) {
        GroupNode &c = (*g)[i];
        assert(c.size() > 0); //otherwise means has been deleted which must not happen
        groups.append(&c);
        c.setGraphNode(NULL);
    }

    g->groupList.clear();
    NarratorGraph *graph = controller->getGraph();
    QList<NarratorNodeIfc *> new_nodes;
    QList<GroupNode *> unmatchedGroups;
    size = groups.size();

    for (int j = 0; j < size; j++) {
        unmatchedGroups.append(groups[j]);
    }

    for (int j = 0; j < size; j++) {
        assert(groups[j]->size() > 0);
        Narrator &n1 = (*groups[j])[0].getNarrator();

        for (int k = j + 1; k < size; k++) {
            assert(groups[k]->size() > 0);
            Narrator &n2 = (*groups[k])[0].getNarrator();
            double eq_val = equal(n1, n2);

            if (eq_val > threshold) {
                NarratorNodeIfc *n1 = &groups[j]->getCorrespondingNarratorNode();
                NarratorNodeIfc *n2 = &groups[k]->getCorrespondingNarratorNode();
                assert(groups[j]->getKey() != groups[k]->getKey());
                NarratorNodeIfc *n_new = controller->getGraph()->mergeNodes((*groups[j]), (*groups[k]), &toDelete);
                assert(n_new->isGraphNode());

                if (n1 != n_new && n1 != NULL && new_nodes.contains(n1)) {
                    new_nodes.removeOne(n1);
                }

                if (n2 != n_new && n2 != NULL && new_nodes.contains(n2)) {
                    new_nodes.removeOne(n2);
                }

                unmatchedGroups.removeOne(groups[j]);
                unmatchedGroups.removeOne(groups[k]);
                assert(&groups[j]->getCorrespondingNarratorNode() != NULL);
                assert(&groups[k]->getCorrespondingNarratorNode() != NULL);

                if (!new_nodes.contains(n_new)) {
                    new_nodes.append(n_new);
                }
            }
        }
    }

    for (int i = 0; i < unmatchedGroups.size(); i++) {
        if (unmatchedGroups[i]->size() > 1) {
            GraphNarratorNode *g = new GraphNarratorNode(*controller->getGraph(), *unmatchedGroups[i]);
            new_nodes.append(g);
            assert(controller->getGraph()->getNode(g->getId()) == g);
        } else {
            assert(unmatchedGroups[i]->size() > 0);
            ChainNarratorNode *c = &(*unmatchedGroups[i])[0];
            graph->removeNode(unmatchedGroups[i]);
            c->setCorrespondingNarratorNodeGroup(NULL);
            new_nodes.append(c);
        }
    }

    //to keep all_nodes consistent
    if (graph->top_nodes.contains(g)) {
        graph->top_nodes.removeOne(g);
        //check if still top both
        graph->top_nodes.append(new_nodes);
    }

    for (int i = 0; i < new_nodes.size(); i++) {
        new_nodes[i]->color = g->color;
    }

    if (!toDelete.contains(g)) {
        toDelete.append(g);
    }
}

void LoopBreakingVisitor::finish() {
    const GraphVisitorController::ParentStack &stack = controller->getParentStack();

    for (int i = 0; i < toDelete.size(); i++) {
        if (toDelete[i]->getId() == 1173) {
            qDebug() << "check";
        }

        controller->getGraph()->removeNode(toDelete[i]);
        assert(!stack.contains(toDelete[i]));
        delete toDelete[i];
    }

    toDelete.clear();
}

int deserializeGraph(QString fileName, ATMProgressIFC *prg) {
    fileName = fileName.split("\n")[0];
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        return -1;
    }

    QDataStream fileStream(&file);
    NarratorGraph *graph = new NarratorGraph(fileStream, prg);
    file.close();
    biographies(graph);
    return 0;
}

int mergeGraphs(QString fileName1, QString fileName2, ATMProgressIFC *prg) {
    fileName1 = fileName1.split("\n")[0];
    QFile file1(fileName1);

    if (!file1.open(QIODevice::ReadOnly)) {
        return -1;
    }

    QDataStream fileStream1(&file1);
    NarratorGraph *graph1 = new NarratorGraph(fileStream1, prg);
    file1.close();
    fileName2 = fileName2.split("\n")[0];
    QFile file2(fileName2);

    if (!file2.open(QIODevice::ReadOnly)) {
        return -1;
    }

    QDataStream fileStream2(&file2);
    NarratorGraph *graph2 = new NarratorGraph(fileStream2, prg);
    file2.close();
    graph1->mergeWith(graph2);
    delete graph2;
    QFile file(fileName1.remove(".por") + "Merged.por");

    if (!file.open(QIODevice::ReadWrite)) {
        return -1;
    }

    QDataStream fileStream(&file);
    graph1->serialize(fileStream);
    file.close();
    delete graph1;
    return 0;
}

typedef TwoLevelSelection OutputData;
typedef AbstractTwoLevelAnnotator::SelectionList OutputDataList;
typedef TwoLevelSelection::MainSelection Selection;
typedef TwoLevelSelection::MainSelectionList SelectionList;

int calculateStatisticsOrAnotate(ChainsContainer &generatedChains, NarratorGraph *generatedPor, QString *text,
                                 QString fileName) {
    HadithChainGraph *dummyChainGraph = new HadithChainGraph();
    OutputDataList outputList;
    HadithDagGraph *generatedGraph = new HadithDagGraph(generatedPor);

    for (int i = 0; i < generatedChains.size(); i++) {
        Chain *chain = generatedChains[i];
        int start = chain->getStart();
        int end = chain->getEnd();
        OutputData sel(dummyChainGraph->duplicate(), text, start, end);
        Chain c_narrators(text);

        for (int j = 0; j < chain->m_chain.size(); j++) {
            ChainPrim *c = chain->m_chain[j];

            if (c->isNarrator()) {
                Narrator *narr = dynamic_cast<Narrator *>(c);
                start = narr->getStart();
                end = narr->getEnd();
                Name name(text, start, end);
                sel.addName(name);
                c_narrators.m_chain.append(narr);
            }
        }

        HadithChainGraph *localGraph = new HadithChainGraph(
            c_narrators); //we should not call deleteGraph() else we corrupt 'chain'
        delete sel.getGraph();
        sel.setGraph(localGraph);
        outputList.append(sel);
    }

    HadithInterAnnotatorAgreement h(text, fileName, generatedGraph, outputList, true);

    if (h.calculateStatisticsOrAnotate() == 0) {
        h.displayStatistics();
    }

    return 0;
#undef NULL_CORRECT
#undef NULL_DETECTED
}
