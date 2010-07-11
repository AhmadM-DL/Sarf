#include <QFile>
#include <QDateTime>
#include "database_info_block.h"
#include "atmTrie.h"
#include "common.h"
#include "sql_queries.h"
#include "Search_by_item.h"
#include "ATMProgressIFC.h"
#include <assert.h>
#include <QDebug>

#ifdef USE_TRIE

ATMProgressIFC *prgsIFC=NULL;

#ifdef REDUCE_THRU_DIACRITICS
inline QString cache_version()
{
	return "RD";
}
#else
inline QString cache_version()
{
	return "ND";
}
#endif

void buildfromfile()
{
	QSqlQuery query(db);
#ifdef REDUCE_THRU_DIACRITICS
	QString stmt=QString("SELECT stem.id, stem.name, stem_category.category_id, stem_category.raw_data FROM stem, stem_category WHERE stem.id=stem_category.stem_id ORDER BY stem.id ASC");
#else
	QString stmt=QString("SELECT stem.id, stem.name, stem_category.category_id FROM stem, stem_category WHERE stem.id=stem_category.stem_id ORDER BY stem.id ASC");
#endif
	QString name,raw_data;
	long category_id;
	long long  stem_id, last_id;
	if (!execute_query(stmt,query))
		return;
	//out<<QDateTime::currentDateTime().time().toString()<<"\n";
	StemNode * node = NULL;
	int index=0;
	last_id=-1;
	int total=query.size(), current=0;
	while (query.next())
	{
		stem_id=query.value(0).toULongLong();
		if (last_id!=stem_id)
		{
			if (last_id!=-1)
			{
				database_info.trie_nodes->insert(index,*node);
				delete node;
				database_info.Stem_Trie->store(name,index);
				index++;
			}
			last_id=stem_id;
			node=new StemNode();
			node->key=name;
			node->stem_id=stem_id;
		}
		name=query.value(1).toString();
		category_id=query.value(2).toLongLong();
#ifdef REDUCE_THRU_DIACRITICS
		raw_data=query.value(3).toString();
		node->add_info(category_id,raw_data);
#else
		node->add_info(category_id);
#endif
		current++;
		prgsIFC->report((double)current/total*100+0.5);
	}
	//out<<QDateTime::currentDateTime().time().toString()<<"\n";
	database_info.Stem_Trie->save(trie_path.toStdString().data());
	QFile file(trie_list_path.toStdString().data());
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream out(&file);   // we will serialize the data into the file
		out << cache_version();
		out << *(database_info.trie_nodes);
		file.close();
	}
	else
		error <<"Unexpected Error: Unable to write TRIE to file\n";
}

void buildTrie()
{
	//out<<QDateTime::currentDateTime().time().toString()<<"\n";
#ifndef TRIE_FROM_FILE
	buildfromfile();
#else
	QFile file(trie_list_path.toStdString().data());
	if (file.open(QIODevice::ReadOnly))
	{
		QDataStream in(&file);    // read the data serialized from the file
		QString version;
		in >> version;
		if (version==cache_version())
		{
			in >> *(database_info.trie_nodes);
			file.close();
		}
		else
		{
			file.close();
			buildfromfile();
			return;
		}
		QFile input(trie_path);
		if (input.open(QIODevice::ReadOnly))
		{
			delete database_info.Stem_Trie;
			input.close();
			database_info.Stem_Trie=new ATTrie(trie_path.toStdString().data());
			//out<<QDateTime::currentDateTime().time().toString()<<"\n";
		}
	}
	else
		buildfromfile();
#endif
}
#endif

void fillMap(item_types type,QHash<Map_key,Map_entry> * map)
{
	QSqlQuery query(db);
	QString table = interpret_type(type);
	QString stmt( "SELECT %1_id, category_id, raw_data, POS, description.name %2 FROM %1_category,description WHERE %1_category.description_id=description.id");
	stmt=stmt.arg(table).arg((type==STEM?", abstract_categories":""));
	assert (execute_query(stmt,query)) ;
	while (query.next())
	{
		long long item_id=query.value(0).toULongLong();
		long category_id =query.value(1).toULongLong();
		QString raw_data=query.value(2).toString();
		QString POS=query.value(3).toString();
		QString description=query.value(4).toString();
		bitset<max_sources> abstract_categories;
		if (type==STEM)
				abstract_categories=string_to_bitset(query.value(5));
		else
				abstract_categories.reset();
		Map_key key(item_id,category_id,raw_data);
		Map_entry entry(abstract_categories,description,POS);
		map->insertMulti(key,entry);
	}
}

database_info_block::database_info_block()
{
    Prefix_Tree=new tree();
    Suffix_Tree=new tree();
#ifdef USE_TRIE
	Stem_Trie= new ATTrie();
	trie_nodes=new QVector<StemNode>();
#endif
    rules_AA=new compatibility_rules(AA);
    rules_AB=new compatibility_rules(AB);
    rules_AC=new compatibility_rules(AC);
    rules_BC=new compatibility_rules(BC);
    rules_CC=new compatibility_rules(CC);

	map_prefix=new QHash<Map_key,Map_entry>;
	map_stem=new QHash<Map_key,Map_entry>;
	map_suffix=new QHash<Map_key,Map_entry>;
}
void database_info_block::fill(ATMProgressIFC *p)
{
    Prefix_Tree->build_affix_tree(PREFIX);
    Suffix_Tree->build_affix_tree(SUFFIX);
	prgsIFC=p;
#ifdef USE_TRIE
	buildTrie();
#endif
    rules_AA->fill();
    rules_AB->fill();
    rules_AC->fill();
    rules_BC->fill();
    rules_CC->fill();

	fillMap(PREFIX,map_prefix);
	fillMap(STEM,map_stem);
	fillMap(SUFFIX,map_suffix);
	filling=false;
}

database_info_block::~database_info_block()
{
    delete Prefix_Tree;
    delete Suffix_Tree;
#ifdef USE_TRIE
	delete Stem_Trie;
	delete trie_nodes;
#endif
    delete rules_AA;
    delete rules_AB;
    delete rules_AC;
    delete rules_BC;
    delete rules_CC;

	delete map_stem;
	delete map_prefix;
	delete map_suffix;
}

database_info_block database_info;