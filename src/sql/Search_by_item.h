#ifndef SEARCH_BY_ITEM_H
#define SEARCH_BY_ITEM_H

#include "sql_queries.h"

class Search_by_item {
    private:
        QSqlQuery query;
        item_types type;
        long long id;
        QString name;
        // returns just a category but can contain redundancy
        bool retrieve_internal(long &category_id);
        bool retrieve_internal(all_item_info &info);
        bool retrieve_internal(minimal_item_info &minimal);
    public:
        Search_by_item(item_types type, long long id);
        Search_by_item(item_types type, QString name);
        Search_by_item(item_types type, QString name, QVector<QString> names);
        long long ID();
        QString Name();
        // total size and not what is left
        int size();
        // returns just a category but can contain redundancy
        bool retrieve(long &category_id);
        bool retrieve(all_item_info &info);
        bool retrieve(minimal_item_info &info);
        int retrieve(long category_ids[], int size_of_array);
        int retrieve(minimal_item_info info[], int size_of_array);
        int retrieve(all_item_info info[], int size_of_array);
        // just for stems
        QString getGrammarStem();
        // just for stems
        dbitvec getGrammarStem_sources();
};

#endif

