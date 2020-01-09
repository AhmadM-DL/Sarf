# Sarf #

[//]: # (Automated analysis of Arabic data sets including texts, publications, records, and digital media
became essential with the huge digital Arabic content available nowadays. The current
solutions adopt data and text mining techniques that are suitable for Latin languages. The
Arabic language has unique features that differentiate it from other languages and that can
help text mining and information retrieval techniques. In this project we aim at developing
Arabic text mining tools and techniques to handle complex queries on separate but related
Arabic documents. Given a set of documents and a relation expressed in a query form, we will
use structural analysis techniques to extract graphs representing the relation and the different
entities involved in the given documents.THIS CONTENT IS NOT VISIBLE )

## Sarf Installation ##

### Prerequisites ###

  * Install the following packages using the "Synaptic Package Manager":
    * mysql-client
    * mysql-server
    * libmysqlcppconn
    * libqt4-sql-mysql
    * libmysqlcppconn-dev
    * subversion
    * qt-sdk
    * qtcreator
    * g++

The ATSarf Gui needs the following additional package for graph generation
  * graphviz

The "tagger" tools needs the following additional pacakge for json parsing
  * libqjson


### Details ###

  * Get our latest version of source code from the following page http://code.google.com/p/atmine/source/checkout
  * Download datrie from http://linux.thai.net/~thep/datrie/datrie.html#Download then open datrie/libdatrie-0.2.3 & "./configure" & "make"
  * Fix the links on the folder ATSarf/third/ to point to correctly:
> > - 'qt' to refer to the folder of your current version of QT
> > - 'datrie' to the folder of your datrie that you downloaded and installed
> > (use the following command to make symbolic links: 'ln -s')
  * Build the source code using QT for the file ATSarf/QT Make/ATSarfTrie.pro
  * Run the resulting Executable generated. **Enjoy!**

## Notes ##

### Introduction ###

The narrator order graph contains a merger of all chains of narrators.
Two narrators are merged if they have a low distance (high identity) score.
The graph should allow and maintain an entry exit correspondence, this can be done by keeping a pointer to the narrators in their chain lists and providing parent/parents, child/children as interface functions.


### Details ###

The graph is multi-rooted. We should keep a list of all rank 1 narrators. The leaves are the hadiths themselves.
We will consider a hash table record of all narrators where a merged narrator with X identified chain narrators will have X entries in the table pointing to it.

We will consider later different lookup and search mechanisms in the graph.



