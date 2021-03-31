#include "models.h"
#include "base.h"

BMLModel::BMLModel(QObject* p):QAbstractTableModel(p) {}

int BMLModel::columnCount(const QModelIndex&) const {
	return 3;
}

int BMLModel::rowCount(const QModelIndex&) const {
	return bookmarks.size();
}

QVariant BMLModel::data(const QModelIndex& idx, int role) const {
	QVariant res;
	if (!idx.isValid()) return res;
	int row = idx.row();
	int col = idx.column();
	if ((row < 0) || (col < 0)) return res;
	if (row >= rowCount()) return res;
	if (col >= columnCount()) return res;
	TBookmark bm = bookmarks.at(row);
	TPage* pg = findPage(bm.pgid);
	switch (role) {
		case Qt::DisplayRole:
			switch (col) {
				case 0:
					res = bm.name;
					break;
				case 1:
					if (pg) res = pg->name;
					break;
				case 2:
					res = bm.row;
					break;
			}
			break;
	}
	return res;
}
