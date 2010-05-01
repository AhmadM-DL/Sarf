#ifndef STEMNODE_H
#define STEMNODE_H

#include <QString>
#include <QVector>
#include <QList>
#include "common_structures/common.h"

class StemNode {
public:
	typedef QVector<long> CategoryVector;

	QString key;
	unsigned long long stem_id;
//private:
	CategoryVector category_ids;
public:
	void add_info(long cat_id);
	bool exists(long cat_id);
	#ifdef REDUCE_THRU_DIACRITICS
	public:
		typedef QList<QString> RawDatasEntry;
		typedef QVector<RawDatasEntry> RawDatas;

		void add_info(long cat_id,QString raw_data);
		void add_info(long cat_id,RawDatasEntry raw_data_entry);
		bool exists(long cat_id, QString raw_data);
		bool get(long cat_id, RawDatasEntry &raw_datas_entry);
	//private:
		RawDatas raw_datas;
	#endif
};

QDataStream &operator>>(QDataStream &in, StemNode &node);
QDataStream &operator<<(QDataStream &out, const StemNode &node);

QDataStream &operator>>(QDataStream &in, StemNode* &node);
QDataStream &operator<<(QDataStream &out, const StemNode* &node);

class Search_StemNode
{
private:
	StemNode * node;
	int cat_index;
#ifdef REDUCE_THRU_DIACRITICS
	int rawdata_index;
#endif

public:
	Search_StemNode();
	Search_StemNode(StemNode *);
	void setNode(StemNode *);
	bool retrieve(long &category_id);
#ifdef REDUCE_THRU_DIACRITICS
	bool retrieve(minimal_item_info & info);
#endif
};

#endif // STEMNODE_H
