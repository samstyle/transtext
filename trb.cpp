#include <stdlib.h>
#include <stdio.h>

#include <QList>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "mainwin.h"

// ver 7

void idError(int type, int subtype) {
	QMessageBox msg(QMessageBox::Critical, QString("Error"), QString(), QMessageBox::Ok);
	switch(type) {
		case T7_ICON:
			msg.setText(QString("Unknown ICON id %0").arg(subtype));
			break;
		case T7_PAGE:
			msg.setText(QString("Unknown PAGE id %0").arg(subtype));
			break;
		case T7_TREE:
			msg.setText(QString("Unknown TREE id %0").arg(subtype));
			break;
		case TP_LINE:
			msg.setText(QString("Unknown LINE id %0").arg(subtype));
			break;
		case TT_DIR:
			msg.setText(QString("Unknown TREE.DIR id %0").arg(subtype));
			break;
		case TT_PAGE:
			msg.setText(QString("Unknown TREE.PAGE id %0").arg(subtype));
			break;
		default:
			msg.setText(QString("Unknown GLOBAL id %0:%1").arg(type).arg(subtype));
			break;
	}
	msg.exec();
}

QTreeWidgetItem* TRBLoader::add_item(QTreeWidgetItem* par, QString name, QUuid id, QUuid iconid) {
	TPage* page;
	QString tip;
	QIcon ico;
	QTreeWidgetItem* itm = new QTreeWidgetItem();
	itm->setData(0,Qt::UserRole,id.toByteArray());
	if (name == "") {
		name = id.toString();
	}
	itm->setText(0, name);
	itm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable); // | Qt::ItemIsUserCheckable);
	if (id.isNull()) {						// directory
		itm->setFlags(itm->flags() | Qt::ItemIsDropEnabled);
		ico = getIcon(iconid);
		itm->setIcon(0, ico.isNull() ? QIcon(":/folder.png") : ico);
		itm->setData(0, roleIcon, iconid.toByteArray());
	} else {							// page
		page = findPage(id);
		if (page) {
			tip = QString("id : %0").arg(page->id.toString());
			itm->setToolTip(0,tip);
			itm->setIcon(0, getPageIcon(page));
		}
	}
	par->addChild(itm);
	return itm;
}

int TRBLoader::v7_load_icons() {
	TIcon ico;
	int type;
	int err = 0;
	strm >> type;
	while ((type != T7_END) && !err) {
		switch (type) {
			case TI_ID: strm >> ico.id; break;
			case TI_ICO: strm >> ico.icon; break;
			case TI_NAME: strm >> ico.name; break;
			case TI_END: addIcon(ico); break;
			default:
				idError(T7_ICON, type);
				err = 1;
				break;
		}
		if (!err) strm >> type;
	}
	return err;
}

int TRBLoader::v7_load_bookmarks() {
	int type;
	int err = 0;
	TBookmark bm;
	strm >> type;
	while (type != T7_END) {
		switch(type) {
			case TB_ID: strm >> bm.id; break;
			case TB_NAME: strm >> bm.name; break;
			case TB_DSC: strm >> bm.descr; break;
			case TB_END: addBookmark(bm); break;
			default:
				idError(T7_BMRK, type);
				err = 1;
				break;
		}
		if (err) break;
		strm >> type;
	}
	return err;
}

int TRBLoader::v7_load_page() {
	int type;
	int err = 0;
	TPage page;
	page.curRow = -1;
	TLine lin;
	TBookmark bm;
	strm >> type;
	while (type != T7_END) {
		switch(type) {
			case TP_UUID:
				strm >> page.id;
				break;
			case TP_FLAG:
				strm >> page.flag;
				break;
			case TP_LINE:
				lin.type = TL_TEXT;
				lin.flag = 0;
				lin.bmrkId = QUuid();
				lin.src.name.clear();
				lin.src.text.clear();
				lin.trn.name.clear();
				lin.trn.text.clear();
				strm >> type;
				while (type != T7_END) {
					switch(type) {
						case TL_SN: strm >> lin.src.name; break;
						case TL_ST: strm >> lin.src.text; break;
						case TL_TN: strm >> lin.trn.name; break;
						case TL_TT: strm >> lin.trn.text; break;
						case TL_TYPE: strm >> lin.type; break;
						case TL_FLAG: strm >> lin.flag; break;
						case TL_BMID: strm >> lin.bmrkId;
							break;
						default:
							idError(TP_LINE, type);
							err = 1;
							break;
					}
					if (err) break;
					strm >> type;
				}
				if (!err) {
					if (!lin.bmrkId.isNull()) {
						bm.id = lin.bmrkId;
						bm.pgid = page.id;
						bm.row = page.text.size();
						addBookmark(bm);
					}
					lin.src.text.remove(QObject::trUtf8("　"));
					normLine(lin);
					page.text.append(lin);
				}
				break;
			default:
				idError(T7_PAGE, type);
				err = 1;
				break;
		}
		if (!err) strm >> type;
	}
	if (!err) putPage(page);
	return err;
}

int TRBLoader:: v7_load_tree(QTreeWidgetItem* par) {

}

void TRBLoader::v7_load(QTreeWidgetItem* root) {
	QTreeWidgetItem* par = root;
	QUuid id;
	QUuid iconid;
	QIcon icon;
	int type;
	QString name;
	strm >> type;
	int err = 0;
	while (!strm.atEnd() && (type != T7_END)) {
		switch(type) {
			case T7_ICON:
				v7_load_icons();
				break;
			case T7_BMRK:
				err = v7_load_bookmarks();
				break;
			case T7_PAGE:
				err = v7_load_page();
				break;
			case T7_TREE:
				strm >> type;
				while (type != T7_END) {
					switch (type) {
						case TT_DIR:
							strm >> type;
							while (type != TT_END) {
								switch(type) {
									case TT_NAME:
										strm >> name;
										break;
									case TT_ICON:
										strm >> icon;
										break;
									case TT_ICONID:
										strm >> iconid;
										break;
									default:
										idError(TT_DIR, type);
										err = 1;
										break;
								}
								if (err) break;
								strm >> type;
							}
							if (err) break;
							par = add_item(par,name,0,iconid);
							iconid = 0;
							break;
						case TT_PAGE:
							strm >> type;
							while (type != TT_END) {
								switch(type) {
									case TT_NAME:
										strm >> name;
										break;
									case TT_UUID:
										strm >> id;
										break;
									case TT_ICON:
										strm >> icon;
										break;
									default:
										idError(TT_PAGE, type);
										err = 1;
										break;
								}
								if (err) break;
								strm >> type;
							}
							if (err) break;
							add_item(par, name, id);
							break;
						case TT_END:
							par = par->parent();
							if (par == nullptr)
								par = root;
							break;
						default:
							idError(T7_TREE, type);
							err = 1;
							break;
					}
					if (err) break;
					strm >> type;
				}
				break;
			default:
				idError(type, 0);
				err = 1;
				break;
		}
		if (err) break;
		strm >> type;
	}
	if (err) {
		prjInit();
	}
}

// ver 8

QList<int> TRBLoader::getlist() {
	int cnt;
	int off;
	strm >> cnt;
	QList<int> lst;
	while (cnt > 0) {
		strm >> off;
		lst.append(off);
	}
	return lst;
}

void TRBLoader::v8_load_pages() {

}

void TRBLoader::v8_load_tree(QTreeWidgetItem* root) {

}

void TRBLoader::v8_load_icons() {

}

void TRBLoader::v8_load_bookmarks() {

}

void TRBLoader::v8_load(QTreeWidgetItem* root) {
	book.clear();
	icons.clear();
	bookmarks.clear();
	QList<int> moff = getlist();
	if (moff.size() > 1) {
		buf.seek(moff.at(0));
		v8_load_pages();
		buf.seek(moff.at(1));
		v8_load_tree(root);
	}
	if (moff.size() > 2) {
		buf.seek(moff.at(2));
		v8_load_icons();
	}
	if (moff.size() > 3) {
		buf.seek(moff.at(3));
		v8_load_bookmarks();
	}
}

// load file

int TRBLoader::load(QString path, QTreeWidgetItem* root) {
	QFile file(path);
	QByteArray sig;
	QByteArray data;
	int res = 1;
	if (file.open(QFile::ReadOnly)) {
		sig = file.read(4);
		data = file.readAll();
		data = qUncompress(data);
		buf.setBuffer(&data);
		buf.open(QBuffer::ReadOnly);
		strm.setVersion(QDataStream::Qt_5_0);
		strm.setDevice(&buf);
		if (sig == "TRB7") {
			v7_load(root);
		} else if (sig == "TRB8") {
			v8_load(root);
		} else {
			QMessageBox::critical(nullptr, "Error", "File signature or version mismatch");
			res = 0;
		}
		buf.close();
		strm.resetStatus();
		strm.setDevice(nullptr);
	} else {
		QMessageBox::critical(nullptr, "Error", "Can't open file");
		res = 0;
	}
	return res;
}

// v7 save

void TRBLoader::v7_save_leaf(QTreeWidgetItem* par) {
	int i;
	QUuid id;
	QTreeWidgetItem* itm;
	for (i = 0; i < par->childCount(); i++) {
		itm = par->child(i);
		id = QUuid(itm->data(0, roleId).toByteArray());
		if (id.isNull()) {
			strm << TT_DIR;
			strm << TT_NAME << itm->text(0);
			strm << TT_ICONID << QUuid(itm->data(0,roleIcon).toByteArray());
			strm << TT_END;
			v7_save_leaf(itm);
		} else {
			strm << TT_PAGE;
			strm << TT_NAME << itm->text(0);
			strm << TT_UUID << id;
			strm << TT_END;
		}
	}
	strm << TT_END;
}

void TRBLoader::v7_save_tree(QTreeWidgetItem* root) {
	strm << T7_TREE;
// TODO: option to save root element (for save-branch)
#if 0
	if (saveroot) {
		strm << TT_DIR;
		strm << TT_NAME << root->text(0);
		strm << TT_ICONID << QUuid(root->data(0,roleIcon).toByteArray());
		strm << TT_END;
	}
#endif
	v7_save_leaf(root);
	strm << T7_END;
}

QList<QUuid> getTreeIds(QTreeWidgetItem*);
QList<QUuid> getTreeIcons(QTreeWidgetItem*);
QList<QUuid> getTreeBMrk(QTreeWidgetItem*);

int TRBLoader::v7_save(QTreeWidgetItem* par) {
//	qDebug() << "v7_save";
	int res = 1;
	QList<QUuid> idlist;
	QUuid id;
	TIcon* ico;
	TPage* pg;
	TLine line;
	TBookmark* bm;
// pages
//	qDebug() << "begin (pages): " << buf.pos();
	idlist = getTreeIds(par);
	foreach(id, idlist) {
		pg = findPage(id);
		if (pg != NULL) {
			strm << T7_PAGE;
			strm << TP_UUID << pg->id;
			strm << TP_FLAG << pg->flag;
			foreach(line, pg->text) {
				strm << TP_LINE;
				strm << TL_TYPE << line.type;
				strm << TL_FLAG << line.flag;
				strm << TL_BMID << line.bmrkId;
				strm << TL_SN << line.src.name;
				strm << TL_ST << line.src.text;
				strm << TL_TN << line.trn.name;
				strm << TL_TT << line.trn.text;
				strm << T7_END;
			}
			strm << T7_END;
		}
	}
// icons
//	qDebug() << "treeicons: " << buf.pos();
	strm << T7_ICON;
	idlist = getTreeIcons(par);
	foreach(id, idlist) {
		ico = findIcon(id);
		if (ico) {
			strm << TI_ID << ico->id;
			strm << TI_NAME << ico->name;
			strm << TI_ICO << ico->icon;
			strm << TI_END;
		}
	}
	strm << T7_END;
// bookmarks
//	qDebug() << "bookmarks: " << buf.pos();
	strm << T7_BMRK;
	idlist = getTreeBMrk(par);
	foreach(id, idlist) {
		bm = findBookmark(id);
		if (bm) {
			strm << TB_ID << bm->id;
			strm << TB_NAME << bm->name;
			strm << TB_DSC << bm->descr;
			strm << TB_END;
		}
	}
	strm << T7_END;
// tree
//	qDebug() << "tree: " << buf.pos();
	v7_save_tree(par);
//	qDebug() << "end: " << buf.pos();
	return res;
}

// v8

void TRBLoader::putlist(QList<int>& lst) {
	strm << lst.size();
	for (int i = 0; i < lst.size(); i++) {
		strm << lst.at(i);
	}
}

void ba_puti(QByteArray& arr, int val) {
	arr.append(val & 0xff);
	arr.append((val >> 8) & 0xff);
	arr.append((val >> 16) & 0xff);
	arr.append((val >> 24) & 0xff);
}

QByteArray lst_to_ba(QList<int> lst) {
	QByteArray res;
	int off;
	ba_puti(res, lst.size());
	while (lst.size() > 0) {
		off = lst.takeFirst();
		ba_puti(res, off);
	}
	return res;
}

// page
// 0	id (QUuid)
// 1	flag (int)
// 2	text data
QByteArray v8_save_page(TPage* pg) {
	QByteArray res;
	QList<int> lst;

	return res;
}

QByteArray TRBLoader::v8_save_pages(QTreeWidgetItem* root) {
	QByteArray res;
	QByteArray blk;
	QList<int> lst;
	QList<QUuid> ids = getTreeIds(root);
	TPage* pg;
	foreach (QUuid id, ids) {
		pg = findPage(id);
		if (pg != NULL) {
			blk = v8_save_page(pg);
			lst.append(res.size());
			res.append(blk);
		}
	}
	blk = lst_to_ba(lst);
	res.prepend(blk);
	return res;
}

QByteArray TRBLoader::v8_save_tree(QTreeWidgetItem* root) {

}

QByteArray TRBLoader::v8_save_icons(QTreeWidgetItem* root) {

}

QByteArray TRBLoader::v8_save_bookmarks(QTreeWidgetItem* root) {

}

int TRBLoader::v8_save(QTreeWidgetItem* root) {
	int res = 1;
	QList<int> lst;
	QByteArray arr;
	QByteArray blk;
	// pages
	blk = v8_save_pages(root);
	lst.append(arr.size());
	arr.append(blk);
	// tree
	blk = v8_save_tree(root);
	lst.append(arr.size());
	arr.append(blk);
	// icons
	blk = v8_save_icons(root);
	lst.append(arr.size());
	arr.append(blk);
	// bookmarks
	blk = v8_save_bookmarks(root);
	lst.append(arr.size());
	arr.append(blk);
	// concat
	blk = lst_to_ba(lst);
	strm << blk;
	strm << arr;
	return res;
}

int TRBLoader::save(QString path, QTreeWidgetItem* root) {
	int res = 1;
	QFile file(path);
	QByteArray data;
	strm.setVersion(QDataStream::Qt_5_0);
	buf.setBuffer(&data);
	buf.open(QBuffer::WriteOnly);
	strm.setDevice(&buf);

	v7_save(root);

	buf.close();
	strm.resetStatus();
	strm.setDevice(nullptr);
	data = qCompress(data);
	if (file.open(QFile::WriteOnly)) {
		file.write("TRB7");
		file.write(data);
		file.close();
	} else {
		res = 0;
	}
	return res;
}
