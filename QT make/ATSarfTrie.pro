TOP = ../third/
INCLUDEPATH += $$QTDIR/include/QtSql \
    $$QTDIR/include/Qt \
    ../src/common \
    ../src/util \
    ../apps/gui \
    ../src/sarf \
    ../src/case \
    ../src/log \
    ../src/cache \
    ../src/sql \
    ../src/build

INCLUDEPATH += $$TOP/datrie/
INCLUDEPATH += $$TOP/datrie/datrie

HEADERS += ../src/util/diacritics.h \
    ../src/util/dbitvec.h \
    ../src/sql/sql_queries.h \
    ../src/sql/Search_Compatibility.h \
    ../src/sql/Search_by_item.h \
    ../src/sql/Search_by_category.h \
    ../src/sarf/tree_search.h \
    ../src/sarf/suffix_search.h \
    ../src/sarf/stemmer.h \
    ../src/sarf/stem_search.h \
    ../src/sarf/prefix_search.h \
    ../src/common/tree.h \
    ../src/common/result_node.h \
    ../src/common/node.h \
    ../src/common/letter_node.h \
    ../src/cache/database_info_block.h \
    ../src/cache/compatibility_rules.h \
    ../src/build/functions.h \
    ../src/log/logger.h \
    ../src/common/common.h \
    ../src/sql/Retrieve_Template.h \
    ../src/common/StemNode.h \
    ../src/common/atmTrie.h \
    ../src/util/letters.h \
    ../src/log/ATMProgressIFC.h \
    ../src/util/Math_functions.h \
    ../src/cache/Search_by_item_locally.h \
    ../src/util/Triplet.h \
    ../src/util/Ptr.h \
    ../src/common/node_info.h \
    ../src/common/solution_position.h \
    ../src/util/reference.h \
    ../src/util/text_handling.h \
    ../src/util/Quadruplet.h \
    ../src/util/textParsing.h \
    ../src/util/mergeLists.h \
    ../src/util/editDistance.h \
    ../src/common/inflections.h \
    ../src/util/transliteration.h \
    ../src/util/vocalizedCombinations.h \
    ../src/util/ambiguity.h \
    ../src/util/morphemes.h \
    ../src/util/combinations.h \
    ../src/common/sarf.h \
    ../src/case/twoLevelTaggerSelection.h \
    ../src/case/timeRecognizer.h \
    ../src/case/timeManualTagger.h \
    ../src/case/split_affixes.h \
    ../src/case/OneLevelAgreement.h \
    ../src/case/narratorHash.h \
    ../src/case/narratorEqualityModel.h \
    ../src/case/narratordetector.h \
    ../src/case/narrator_abstraction.h \
    ../src/case/localizeddisplay.h \
    ../src/case/hadith_utilities.h \
    ../src/case/hadithManualTagger.h \
    ../src/case/hadithInterAnnotatorAgreement.h \
    ../src/case/hadithDagItemModel.h \
    ../src/case/hadithDagGraph.h \
    ../src/case/hadithCommon.h \
    ../src/case/hadithChainItemModel.h \
    ../src/case/hadithChainGraph.h \
    ../src/case/hadith.h \
    ../src/case/graph_nodes.h \
    ../src/case/graph.h \
    ../src/case/geneMainwindow.h \
    ../src/case/genealogyItem.h \
    ../src/case/genealogyInterAnnotatorAgreement.h \
    ../src/case/diacriticsPostProcessing.h \
    ../src/case/diacriticDisambiguation.h \
    ../src/case/biographyGraphUtilities.h \
    ../src/case/biographies.h \
    ../src/case/bibleManualTagger.h \
    ../src/case/bibleGeneology.h \
    ../src/case/atbExperiment.h \
    ../src/case/atbDiacriticExperiment.h \
    ../src/case/affixTreeTraversal.h \
    ../src/case/abstractTwoLevelAnnotator.h \
    ../src/case/AbstractTwoLevelAgreement.h \
    ../src/case/abstractGraph.h \
    ../src/case/abstractAnnotator.h \
    ../apps/gui/test.h \
    ../apps/gui/mainwindow.h \
    ../src/common/TreeVisitor.h \
    ../src/common/TreeFilter.h \
    ../src/common/Tree2Dot.h \
    ../src/util/largeFileIterator.h \
    ../src/util/iterativeStandardDeviation.h \
    ../src/util/iterativeMathFunctions.h \
    ../src/util/instanceIterator.h \
    ../src/util/distinguishingLargeFileIterator.h \
    ../src/util/decisionTreeRegression.h
HEADERS += $$TOP/datrie/datrie/trie.h

SOURCES += ../src/util/text_handling.cpp \
    ../src/util/diacritics.cpp \
    ../src/util/dbitvec.cpp \
    ../src/sql/sql_queries.cpp \
    ../src/sql/Search_Compatibility.cpp \
    ../src/sql/Search_by_item.cpp \
    ../src/sql/Search_by_category.cpp \
    ../src/sarf/suffix_search.cpp \
    ../src/sarf/stemmer.cpp \
    ../src/sarf/stem_search.cpp \
    ../src/sarf/prefix_search.cpp \
    ../src/common/tree.cpp \
    ../src/common/result_node.cpp \
    ../src/common/node.cpp \
    ../src/common/letter_node.cpp \
    ../src/cache/database_info_block.cpp \
    ../src/cache/compatibility_rules.cpp \
    ../src/build/functions.cpp \
    ../src/sarf/tree_search.cpp \
    ../src/sql/Retrieve_Template.cpp \
    ../src/common/global_variables.cpp \
    ../src/common/StemNode.cpp \
    ../src/common/atmTrie.cpp \
    ../src/util/Math_functions.cpp \
    ../src/cache/Search_by_item_locally.cpp \
    ../src/common/common.cpp \
    ../src/common/solution_position.cpp \
    ../src/util/editDistance.cpp \
    ../src/util/transliteration.cpp \
    ../src/log/ATMProgressIFC.cpp \
    ../src/util/vocalizedCombinations.cpp \
    ../src/util/morphemes.cpp \
    ../src/util/ambiguity.cpp \
    ../src/util/combinations.cpp \
    ../src/common/sarf.cpp \
    ../src/case/twoLevelTaggerSelection.cpp \
    ../src/case/timeRecognizer.cpp \
    ../src/case/timeManualTagger.cpp \
    ../src/case/split_affixes.cpp \
    ../src/case/regressionDiacriticsTest.cpp \
    ../src/case/OneLevelAgreement.cpp \
    ../src/case/narratorHash.cpp \
    ../src/case/narratorEqualityModel.cpp \
    ../src/case/narratorEqualityComparision.cpp \
    ../src/case/narratordetector.cpp \
    ../src/case/narrator_abstraction.cpp \
    ../src/case/madaExperiment.cpp \
    ../src/case/localizeddisplay.cpp \
    ../src/case/hadithRefactored.cpp \
    ../src/case/hadithManualTagger.cpp \
    ../src/case/hadithInterAnnotatorAgreement.cpp \
    ../src/case/hadithDagItemModel.cpp \
    ../src/case/hadithDagGraph.cpp \
    ../src/case/hadithCommon.cpp \
    ../src/case/hadithChainItemModel.cpp \
    ../src/case/hadithChainGraph.cpp \
    ../src/case/graph_nodes.cpp \
    ../src/case/graph.cpp \
    ../src/case/geneMainwindow.cpp \
    ../src/case/genealogyItem.cpp \
    ../src/case/genealogyInterAnnotatorAgreement.cpp \
    ../src/case/diacriticsPostProcessing.cpp \
    ../src/case/diacriticDisambiguation.cpp \
    ../src/case/biographyGraphUtilities.cpp \
    ../src/case/biographies.cpp \
    ../src/case/bibleManualTagger.cpp \
    ../src/case/bibleGeneology.cpp \
    ../src/case/atbExperiment.cpp \
    ../src/case/atbDiacriticExperiment.cpp \
    ../src/case/affix_verification.cpp \
    ../src/case/abstractTwoLevelAnnotator.cpp \
    ../src/case/AbstractTwoLevelAgreement.cpp \
    ../src/case/abstractAnnotator.cpp \
    ../apps/gui/test.cpp \
    ../apps/gui/mainwindow.cpp \
    ../src/util/decisionTreeRegression.cpp \
    ../src/util/distinguishingLargeFileIterator.cpp \
    ../src/util/largeFileIterator.cpp \
    ../src/util/iterativeMathFunctions.cpp

# LIBS += -ldatrie
LIBS += -L$$TOP/datrie/datrie/.libs/ \
    -ldatrie
CONFIG += qt
CONFIG += debug_and_release

# -Q suppresses the call graph data
# QMAKE_CFLAGS += -pg
# QMAKE_CXXFLAGS += -pg
# QMAKE_LFLAGS += -pg
# QMAKE_CFLAGS += -O3
# QMAKE_CXXFLAGS += -O3
# QMAKE_LFLAGS += -O3
# debug
QT += sql
FORMS += ../apps/gui/mainwindow.ui
OTHER_FILES +=
