#include "mainwin.h"
#include "filetypes.h"

#include <string.h>

QColor blkcol;
TPage* curPage = NULL;

MWindow::MWindow() {
	curPage = NULL;
	curItem = NULL;
	prjPath.clear();
	clip = QApplication::clipboard();

	ui.setupUi(this);
	model = new TBModel;
	ui.table->setModel(model);

	QTableView* tab = ui.table;
	tab->setColumnWidth(0,20);
	tab->setColumnWidth(1,50);
	tab->setColumnWidth(2,200);
	tab->setColumnWidth(3,50);
	tab->setColumnWidth(4,200);
	tab->horizontalHeader()->setStretchLastSection(true);
	tab->setSelectionBehavior(QTableWidget::SelectRows);
	tab->verticalHeader()->setDefaultSectionSize(17);
	tab->verticalHeader()->setVisible(false);

	ui.widFind->hide();

//	repwin = new QDialog(this);
//	rui.setupUi(repwin);
//	connect(rui.butok,SIGNAL(released()),this,SLOT(replace()));
//	connect(rui.butcan,SIGNAL(released()),repwin,SLOT(hide()));

	connect(ui.actNewProj,SIGNAL(triggered()),this,SLOT(newPrj()));
	connect(ui.actOpen,SIGNAL(triggered()),this,SLOT(openPrj()));
	connect(ui.actSave,SIGNAL(triggered()),this,SLOT(saveIt()));
	connect(ui.actSaveAs,SIGNAL(triggered()),this,SLOT(savePrj()));

	connect(ui.actNewPage,SIGNAL(triggered()),this,SLOT(newPage()));
	connect(ui.actOpenSrc,SIGNAL(triggered()),this,SLOT(openSrc()));
	connect(ui.actInsertSrc,SIGNAL(triggered()),this,SLOT(insertSrc()));
	connect(ui.actSaveSrc,SIGNAL(triggered()),this,SLOT(saveSrc()));

	connect(ui.srcline,SIGNAL(textChanged(QString)),this,SLOT(changeSrc(QString)));
	connect(ui.trnline,SIGNAL(textChanged(QString)),this,SLOT(changeTrn(QString)));
	connect(ui.srcname,SIGNAL(textChanged(QString)),this,SLOT(changeSNm(QString)));
	connect(ui.trnname,SIGNAL(textChanged(QString)),this,SLOT(changeTNm(QString)));

	connect(ui.tree->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changePage()));
	connect(ui.table->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(changeRow(QItemSelection)));

	connect(clip,SIGNAL(dataChanged()),this,SLOT(appendCbrd()));

	ui.tree->addAction(ui.actNewDir);
	ui.tree->addAction(ui.actNewPage);
	ui.tree->addAction(ui.actSort);
	ui.tree->addAction(ui.actMerge);
	// ui.tree->addAction(ui.actChangeIcon);
	ui.tree->addAction(ui.actDelPage);
	connect(ui.actNewDir,SIGNAL(triggered()),this,SLOT(newDir()));
	connect(ui.actDelPage,SIGNAL(triggered()),this,SLOT(delPage()));
	connect(ui.actSort,SIGNAL(triggered()),this,SLOT(sortTree()));
	// connect(ui.actChangeIcon,SIGNAL(triggered()),this,SLOT(changeIcon()));
	connect(ui.actMerge, SIGNAL(triggered()), this, SLOT(mergePages()));

	tbMenu = new QMenu();
	sjMenu = tbMenu->addMenu("Choises");
	tbMenu->addAction(ui.actFindUntrn);
	tbMenu->addAction(ui.actClearTrn);
	tbMenu->addAction(ui.actSplit);
	tbMenu->addAction(ui.actDelRows);
	connect(ui.table,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(tbContextMenu()));
	connect(sjMenu,SIGNAL(triggered(QAction*)),this,SLOT(jumpLine(QAction*)));
	connect(ui.actClearTrn,SIGNAL(triggered()),this,SLOT(clearTrn()));

	connect(ui.actSplit,SIGNAL(triggered()),this,SLOT(pageSplit()));
	connect(ui.actDelRows,SIGNAL(triggered()),this,SLOT(rowDelete()));
	connect(ui.actFindUntrn,SIGNAL(triggered()),this,SLOT(findUntrn()));

	connect(ui.leFind, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));

}

bool askSure(QString text) {
	QMessageBox box(QMessageBox::Question,"Question",text,QMessageBox::Yes | QMessageBox::No);
	// int res = box.exec();
	return (box.exec() == QMessageBox::Yes);
}

void MWindow::clearTrn() {
	if (!curPage) return;
	if (!askSure("Clear translation?")) return;
	for (int i = 0; i < curPage->text.size(); i++) {
		curPage->text[i].trn.name.clear();
		curPage->text[i].trn.text.clear();
	}
	setProgress();
	model->update();
}

void MWindow::findUntrn() {
	if (!curPage) return;
	for (int i = 0; i < curPage->text.size(); i++) {
		if (getLineStatus(curPage->text.at(i)) == LS_UNTRN) {
			ui.table->selectRow(i);
			break;
		}
	}
}

void MWindow::tbContextMenu() {
	tbMenu->move(QCursor::pos());
	tbMenu->show();
}

void MWindow::jumpLine(QAction *act) {
	int newRow = act->data().toInt();
	ui.table->selectRow(newRow);
}

void MWindow::fillSJMenu() {
	sjMenu->clear();
	if (!curPage) return;
	int i;
	for (i = 0; i < curPage->text.size(); i++) {
		if (curPage->text[i].src.text.contains("[select]")) {
			sjMenu->addAction(QString::number(i))->setData(i);
		}
	}
}

/*
void MWindow::replace() {
	repwin->hide();
	if (curPage == NULL) return;
	QString fstr = rui.sfind->text();
	QString rstr = rui.sreplace->text();
	if ((fstr == "") || (rstr == "")) return;
	for (int i = 0; i < curPage->text.size(); i++) {
		if (curPage->text[i].src.name.contains(fstr)) curPage->text[i].src.name.replace(fstr,rstr);
		if (curPage->text[i].src.text.contains(fstr)) curPage->text[i].src.text.replace(fstr,rstr);
		if (curPage->text[i].trn.name.contains(fstr)) curPage->text[i].trn.name.replace(fstr,rstr);
		if (curPage->text[i].trn.text.contains(fstr)) curPage->text[i].trn.text.replace(fstr,rstr);
	}
	model->update();
}
*/

void MWindow::appendCbrd() {
	if (!ui.actGrabCbrd->isChecked()) return;
	if (curPage == NULL) return;
	QString txt = clip->text();
	TLine tlin;
	tlin.type = TL_TEXT;
	tlin.flag = 0;
	tlin.src.text = txt;
	normLine(tlin);
	curPage->text.append(tlin);
	model->update();
}

void MWindow::sortTree() {
	ui.tree->sortItems(0,Qt::AscendingOrder);
}

void MWindow::setPage(int id) {
	curPage = findPage(id);
	model->update();
}

QIcon getIcon(TPage* page) {
	QPixmap pix(32,32);
	QPainter pnt;
	int prc = getProgress(page);
	pnt.begin(&pix);
	int high = 0.32 * prc;
	pnt.fillRect(0,0,32,32,Qt::lightGray);
	pnt.fillRect(0,0,high,8,Qt::green);
	pnt.fillRect(high,0,32-high,8,Qt::red);
	pnt.setFont(QFont("FreeSans",15,QFont::Bold));
	pnt.setPen(Qt::black);
	pnt.drawText(QRect(0,8,32,24),Qt::AlignCenter,QString::number(prc));
	pnt.end();
	return QIcon(pix);
}

void MWindow::setProgress() {
	if (!curItem || !curPage) {
		ui.progress->setMaximum(1);
		ui.progress->setValue(0);
	} else {
		int tot,trn;
		if (curItem->data(0,Qt::UserRole) == 0) {
			trn = 0;
			tot = 1;
		} else {
			getCounts(curPage,trn,tot);
			curItem->setIcon(0,getIcon(curPage));
		}
		ui.progress->setMaximum(tot);
		ui.progress->setValue(trn);
	}
}

QString getString(TLine line, int wut) {
	QString res;
	switch (wut) {
		case TL_SRC:
			res = line.src.text;
			if (line.src.name.isEmpty()) break;
			res.prepend("\t").prepend(line.src.name);
			break;
		case TL_TRN:
			res = line.trn.text;
			if (line.trn.name.isEmpty()) break;
			res.prepend("\t").prepend(line.trn.name);
			break;
	}
	return res;
}

int MWindow::getCurrentRow() {
	int res = -1;
	QModelIndexList idxs = ui.table->selectionModel()->selectedRows();
	if (idxs.size() == 1) res = idxs.first().row();
	return res;
}

void MWindow::filter(QString str) {
	int i;
	bool match;
	if (ui.widFind->isHidden() || str.isEmpty()) {
		for (i = 0; i < ui.table->model()->rowCount(); i++) {
			ui.table->setRowHidden(i, false);
		}
	} else {
		for (i = 0; i < ui.table->model()->rowCount(); i++) {
			match = curPage->text[i].trn.text.contains(str, Qt::CaseInsensitive) || \
				curPage->text[i].trn.name.contains(str, Qt::CaseInsensitive) || \
				curPage->text[i].src.text.contains(str, Qt::CaseInsensitive) || \
				curPage->text[i].src.name.contains(str, Qt::CaseInsensitive);
			ui.table->setRowHidden(i, !match);
		}
	}
}

// protected

void MWindow::keyPressEvent(QKeyEvent* ev) {
	// int row = getCurrentRow();
	int pos;
	QString tx;
	if (ev->modifiers() & Qt::ControlModifier) {
		switch (ev->key()) {
			case Qt::Key_Insert:
				rowInsert(true);
				break;
			case Qt::Key_S:
				savePrj(prjPath);
				break;
			case Qt::Key_O:
				openPrj();
				break;
			case Qt::Key_F:
				if (ui.widFind->isHidden()) {
					ui.leFind->clear();
					ui.widFind->show();
				} else {
					ui.widFind->hide();
					filter("");
				}
				break;
			case Qt::Key_D:
				if (ui.srcname->isVisible()) break;
				tx = ui.srcline->text();
				pos = tx.indexOf(trUtf8("（"));
				if (pos > 0) {
					ui.srcname->setText(tx.left(pos));
					ui.srcline->setText(tx.mid(pos));
				}
				ui.srcname->setVisible(true);
				ui.trnname->setVisible(true);
				break;
		}
	} else {
		switch (ev->key()) {
			case Qt::Key_Delete:
				rowDelete();
				break;
			case Qt::Key_Insert:
				rowInsert(false);
				break;
			case Qt::Key_Up:
				lineUp();
				break;
			case Qt::Key_Down:
				lineDown();
				break;
			case Qt::Key_Escape:
				if (ui.widFind->isVisible()) {
					ui.widFind->hide();
					filter("");
				}
				break;
		}
	}
}

void MWindow::lineUp() {
	do {
		curRow--;
	} while ((curRow > 0) && ui.table->isRowHidden(curRow));
	if (curRow < 0) return;
	ui.table->selectionModel()->setCurrentIndex(model->index(curRow,0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void MWindow::lineDown() {
	if (curRow < 0) return;
	do {
		curRow++;
	} while ((curRow < model->rowCount()) && ui.table->isRowHidden(curRow));
	if (curRow >= model->rowCount()) {
		curRow = model->rowCount() - 1;
	}
	if (curRow < 0) return;
	ui.table->selectionModel()->setCurrentIndex(model->index(curRow,0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void MWindow::closeEvent(QCloseEvent* ev) {
	if (book.size() != 0) {
		QMessageBox msg(QMessageBox::Question,"Question","Save @ exit?",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,this);
		switch (msg.exec()) {
			case QMessageBox::Yes:
				if (savePrj(prjPath)) {
					ev->accept();
				} else {
					ev->ignore();
				}
				break;
			case QMessageBox::No:
				ev->accept();
				break;
			case QMessageBox::Cancel:
				ev->ignore();
				break;
		}
	} else {
		ev->accept();
	}
}

// page

void MWindow::changeIcon() {
	if (curItem == NULL) return;
	if (curItem->data(0,Qt::UserRole).toInt() != 0) return;
	QString path = QFileDialog::getOpenFileName(this,"Select icon");
	if (path == "") return;
	QPixmap pxm(path);
	QIcon icon(pxm.scaled(16,16));
	curItem->setIcon(0,icon);
}

void MWindow::delPage() {
	QList<QTreeWidgetItem*> items = ui.tree->selectedItems();
	if (items.isEmpty()) return;
	if (!askSure("Delete selected element(s)?")) return;
	ui.tree->selectionModel()->clear();
	foreach(QTreeWidgetItem* itm, items) {
		delItem(itm);
	}
}

void MWindow::mergePages() {
	QList<QTreeWidgetItem*> items = ui.tree->selectedItems();
	if (items.size() < 2) return;
	int pid = items.first()->data(0, Qt::UserRole).toInt();
	if (pid == 0) return;
	TPage* par = findPage(pid);
	TPage* pg;
	items.removeFirst();
	QTreeWidgetItem* itm;
	int id;
	foreach(itm, items) {
		id = itm->data(0, Qt::UserRole).toInt();
		if (id != 0) {
			pg = findPage(id);
			par->text.append(pg->text);
			delItem(itm);
		}
	}
	model->update();
	setProgress();
}

void MWindow::delItem(QTreeWidgetItem* item) {
	int id = item->data(0,Qt::UserRole).toInt();
	if (id == 0) {
		for (id = 0; id < item->childCount(); id++) {
			delItem(item->child(id));
		}
	} else {
		removePage(id);
	}
	QTreeWidgetItem* par = item->parent();
	if (par == NULL) par = ui.tree->invisibleRootItem();
	if (par->indexOfChild(item) >= 0)
		par->removeChild(item);
}

// pix

void MWindow::pageSplit() {
	if (curItem == NULL) return;
	int idx = getCurrentRow();
	if (idx < 0) return;
	if (idx >= ui.table->model()->rowCount()) return;
	QList<TLine> tx1 = curPage->text.mid(0,idx);
	QList<TLine> tx2 = curPage->text.mid(idx);
	TPage* newp = newPage();
	newp->text = tx2;
	curPage->text = tx1;
	model->update();
	setProgress();
}

// rows actions

void MWindow::rowDelete() {
	if (!ui.table->isEnabled()) return;
	QModelIndexList list = ui.table->selectionModel()->selectedRows();
	if (list.size() == 0) return;
	QList<int> rowList;
	rowList.append(list.first().row());
	list.removeFirst();
	int i,row;
	while (list.size() > 0) {
		row = list.first().row();
		if (row > rowList.first()) {
			rowList.prepend(row);
		} else if (row < rowList.last()) {
			rowList.append(row);
		} else {
			for (i = 0; i < rowList.size() - 1; i++) {
				if (row < rowList.at(i)) {
					rowList.insert(i, row);
					break;
				}
			}
		}
		list.removeFirst();

	}
	row = ui.table->currentIndex().row();
	foreach(i, rowList) {
		curPage->text.removeAt(i);
		model->removeRow(i);
	}
	if (row >= model->rowCount())
		row--;
	ui.table->selectRow(row);
	setProgress();
}

void MWindow::rowInsert(bool before) {
	if (!ui.table->isEnabled()) return;
	TLine line;
	line.type = TL_TEXT;
	line.flag = 0;
	int row = (curRow < 0) ? 0 : (before ? curRow : curRow + 1);
	curPage->text.insert(row,line);
	model->insertRow(row);
}

// edit block

void MWindow::setEdit(bool on) {
	ui.editGrid->setEnabled(on);
	if (!on) {
		ui.srcname->clear();
		ui.srcline->clear();
		ui.trnname->clear();
		ui.trnline->clear();
	}
}

void MWindow::disableTab() {
	curPage = NULL;
	curRow = -1;
	ui.tabs->setEnabled(false);
	setEdit(false);
	ui.table->update();
}

void MWindow::changePage() {
	if (curPage) {
		curPage->curRow = ui.table->currentIndex().row();
		curPage = NULL;
	}
	if (ui.tree->selectedItems().size() != 1) {
		disableTab();
	} else {
		QTreeWidgetItem* itm = ui.tree->currentItem();
		if (!itm) {
			disableTab();
		} else {
			curItem = itm;
			int id = itm->data(0,Qt::UserRole).toInt();
			if (id != 0) {
				if (!curPage || (curPage->id != id)) {
					setPage(id);
					ui.tabs->setEnabled(true);
					setEdit(true);
					ui.table->scrollToTop();
					ui.table->selectionModel()->clear();
					setEdit(false);
				}
			} else {
				disableTab();
			}
		}
	}
	model->update();
	fillSJMenu();
	setProgress();
	if (curPage) {
		curRow = curPage->curRow;
		if (curRow > -1) {
			ui.table->selectRow(curRow);
		}
	}
	ui.widFind->hide();
	filter("");
}

void MWindow::changeRow(QItemSelection) {
//	QModelIndexList idx = sel.indexes();
	int row = getCurrentRow();
	curRow = row;
	QString text;
	if (row < 0) {
		setEdit(false);
	} else {
		if ((curPage->text[row].type == TL_TEXT) || (curPage->text[row].type == TL_SELECT)) {
			ui.srcname->setText(curPage->text[row].src.name);
			ui.srcline->setText(curPage->text[row].src.text);
			ui.trnname->setText(curPage->text[row].trn.name);
			ui.trnline->setText(curPage->text[row].trn.text);
			ui.srcline->setCursorPosition(0);
			ui.trnline->setCursorPosition(0);
			text = ui.srcline->text();
			setEdit(true);
			if (curPage->text[row].src.name.isEmpty() && curPage->text[row].trn.name.isEmpty()) {
				ui.trnname->setVisible(false);
				ui.srcname->setVisible(false);
				if (ui.trnname->hasFocus()) ui.trnline->setFocus();
			} else {
				ui.trnname->setVisible(true);
				ui.srcname->setVisible(true);
				if (ui.trnline->hasFocus()) ui.trnname->setFocus();
			}
			if (!ui.srcname->text().isEmpty()) {
				text.prepend(QDialog::trUtf8("「")).prepend(ui.srcname->text()).append(QDialog::trUtf8("」"));
			}
			if (!ui.actGrabCbrd->isChecked()) clip->setText(text);
//			ui.transline->setText(text);
		} else {
			setEdit(false);
		}
	}
}

void MWindow::changeSrc(QString text) {
	if (!ui.editGrid->isEnabled()) return;
	if (!curPage || (curRow < 0)) return;
	curPage->text[curRow].src.text = text;
	model->updateCell(curRow,2);
	setProgress();
}

void MWindow::changeTrn(QString text) {
	if (!ui.editGrid->isEnabled()) return;
	if (!curPage || (curRow < 0)) return;
	curPage->text[curRow].trn.text = text;
	model->updateCell(curRow,4);
	setProgress();
	if (text.contains("[select]"))
		fillSJMenu();
}

void MWindow::changeSNm(QString text) {
	if (!ui.editGrid->isEnabled()) return;
	if (!curPage || (curRow < 0)) return;
	curPage->text[curRow].src.name = text;
	model->updateCell(curRow,1);
	setProgress();
}

void MWindow::changeTNm(QString text) {
	if (!ui.editGrid->isEnabled()) return;
	if (!curPage || (curRow < 0)) return;
	curPage->text[curRow].trn.name = text;
	model->updateCell(curRow,3);
	setProgress();
}

// menu

void MWindow::newPrj() {
	prjInit();
	disableTab();
	ui.tree->clear();
	prjPath = "";
}

#define TPART	1
#define TPAGE	2
#define TNOTE	3
#define TPIXX	4
#define TEND	-1

#define	TBOOK	10
#define TTREE	11

struct TRBItem {
	size_t pos;
	size_t size;
};

struct TRBHead {
	union {
	TRBItem all[256];
	struct {
		TRBItem pages;
		TRBItem tree;
		TRBItem bg;
	};
	};
};

void MWindow::loadVer4(QByteArray& data, QTreeWidgetItem* par) {
	int type;
	int temp;
	int count;
	int i;
	TPage page;
	TPage* pageptr;
	TLine line;
	QString text;
	QString name;
	QBuffer buf;
	QByteArray bytes;
	QDataStream stream;
	data = qUncompress(data.mid(4));
	buf.setBuffer(&data);
	buf.open(QIODevice::ReadOnly);
	stream.setDevice(&buf);
	while (!stream.atEnd()) {
		stream >> type;
		switch(type) {
			case TNOTE:
				stream >> name;
				break;
			case TPART:
				stream >> name;
				stream >> temp;		// id
				stream >> count;	// txt size (0)
				for (i = 0; i < count; i++) {
					stream >> text;	// src
					stream >> text;	// trn
					stream >> temp;	// flag
					stream >> temp;	// id
					stream >> temp;	// imgid
				}
				stream >> count;		// pix count
				for (i = 0; i < count; i++) {
					stream >> temp;		// id
					stream >> text;		// name
					stream >> bytes;	// img
				}
				stream >> text;		// note
				par = addItem(par,name,0);
				break;
			case TEND:
				par = par->parent();
				if (!par) par = ui.tree->invisibleRootItem();
				break;
			case TPAGE:
				page.text.clear();
				stream >> name;
				stream >> temp;			// id
				stream >> count;		// size
				for (i = 0; i < count; i++) {
					line.src.name.clear();
					stream >> line.src.text;
					line.trn.name.clear();
					stream >> line.trn.text;
					normLine(line);
					stream >> temp;		// flag
					stream >> temp;		// id
					stream >> temp;		// imgid
					line.type = TL_TEXT;
					line.flag = 0;
					page.text.append(line);
				}
				stream >> count;		// pix count
				for (i = 0; i < count; i++) {
					stream >> temp;		// id
					stream >> name;		// name
					stream >> bytes;	// img
				}
				stream >> name;			// notes
				pageptr = addPage(page);
				addItem(par,name,pageptr->id);
				break;
			case TPIXX:
				stream >> count;
				for (i = 0; i < count; i++) {
					stream >> temp;
					stream >> text;
					stream >> bytes;
				}
				break;
		}
	}
	buf.close();
}

void MWindow::loadVer5(QByteArray& data, QTreeWidgetItem* par) {
	int type;
	int temp;
	int count;
	int i;
	TPage page;
	TLine line;
	QString name;
	QBuffer buf;
	QBuffer puf;
	QByteArray bytes;
	QDataStream stream;
	QDataStream strm;
	prjInit();
	data.remove(0,4);
	buf.setBuffer(&data);
	buf.open(QIODevice::ReadOnly);
	stream.setDevice(&buf);
	while (!stream.atEnd()) {
		stream >> type;		// block type
		switch (type) {
			case TBOOK:
				stream >> temp;		// pages
				while (temp > 0) {
					stream >> bytes;	// compress page text
					bytes = qUncompress(bytes);
					puf.setBuffer(&bytes);
					puf.open(QIODevice::ReadOnly);
					strm.setDevice(&puf);
					strm >> page.id;
					strm >> name;
					page.text.clear();
					strm >> count;		// text size
					for (i = 0; i < count; i++) {
						strm >> line.type;
						line.flag = 0;
						strm >> name;		// cond
						strm >> name;		// selid
						strm >> name;		// note
						strm >> line.src.name;
						strm >> line.src.text;
						strm >> line.trn.name;
						strm >> line.trn.text;
						strm >> name;		// bground;
						strm >> name;		// prsLeft;
						strm >> name;		// prsMiddle;
						strm >> name;		// prsRight;
						normLine(line);
						page.text.append(line);
					}
					puf.close();

					stream >> count;		// images in page
					for (i = 0; i < count; i++) {	// uncompress image data
						stream >> name;		// name
						stream >> bytes;	// data
					}

					stream >> count;	// images in page
					for (i = 0; i < count; i++) {	// uncompress image data
						stream >> name;		// name;
						stream >> bytes;	// data;
					}

					putPage(page);
					temp--;
				}
				break;
			case TTREE:
				stream >> bytes;
				bytes = qUncompress(bytes);
				puf.setBuffer(&bytes);
				puf.open(QIODevice::ReadOnly);
				strm.setDevice(&puf);
				while(!strm.atEnd()) {
					strm >> temp;
					switch (temp) {
						case TPAGE:
							strm >> name;
							strm >> count;
							addItem(par,name,count);
							break;
						case TPART:
							strm >> name;
							par = addItem(par,name,0);
							break;
						case TEND:
							par = par->parent();
							if (!par) par = ui.tree->invisibleRootItem();
							break;
					}
				}
				puf.close();
				break;
		}
	}
	buf.close();
}

void MWindow::loadVer6(QByteArray& data, QTreeWidgetItem* par) {
	int temp;
	int count;
	TLine line;
	TPage page;
	QBuffer buf;
	QString name;
	QByteArray bytes;
	QDataStream strm;
	prjInit();
	data.remove(0,5);
	TRBHead* hd = (TRBHead*)data.data();			// header

	qDebug() << hd->pages.pos << hd->pages.size;

	bytes = qUncompress(data.mid(hd->pages.pos,hd->pages.size));	// pages database
	buf.setBuffer(&bytes);
	buf.open(QIODevice::ReadOnly);
	strm.setDevice(&buf);
	strm >> temp;
	while (temp > 0) {
		strm >> page.id;
		strm >> page.flag;
		strm >> count;
		page.text.clear();
		while (count > 0) {
			strm >> line.type;
			strm >> line.flag;
			strm >> line.src.name;
			strm >> line.src.text;
			strm >> line.trn.name;
			strm >> line.trn.text;
			page.text.append(line);
			count--;
		}
		putPage(page);
		temp--;
	}
	buf.close();

	qDebug() << hd->tree.pos << hd->tree.size;

	bytes = qUncompress(data.mid(hd->tree.pos,hd->tree.size));	// tree struct
	buf.setBuffer(&bytes);
	buf.open(QIODevice::ReadOnly);
	strm.setDevice(&buf);
	while(!strm.atEnd()) {
		strm >> temp;
		switch (temp) {
			case TPAGE:
				strm >> name;
				strm >> count;
				addItem(par,name,count);
				break;
			case TPART:
				strm >> name;
				par = addItem(par,name,0);
				break;
			case TEND:
				par = par->parent();
				if (!par) par = ui.tree->invisibleRootItem();
				break;
		}
	}
	buf.close();
}

#define T7_PAGE	0x3F
#define	T7_TREE	0x3E
#define T7_END	0x00

#define TP_ID	0x41		// page id
#define	TL_TYPE	TP_ID
#define	TP_FLAG	0x42		// page flag
#define	TL_FLAG	TP_FLAG
#define	TP_LINE	0x03
#define	TL_SN	0x84		// src name
#define	TL_ST	0x85		// src text
#define	TL_TN	0x86		// trn name
#define	TL_TT	0x87		// trn text

#define	TT_DIR	0x08
#define	TT_PAGE	0x09
#define	TT_NAME	0x8A		// name of (dir/page)
#define	TT_PID	0x4B		// assotiated page id
#define	TT_ICON	0xCC		// icon
#define	TT_END	0x0D		// end of current dir list

void skipUnknown(QDataStream& strm, int type) {
	int tint;
	QString tstr;
	QByteArray tarr;
	switch(type & 0xc0) {
		case 0x40: strm >> tint; break;
		case 0x80: strm >> tstr; break;
		case 0xc0: strm >> tarr; break;
	}
}

void MWindow::loadVer7(QByteArray& data, QTreeWidgetItem* par) {
	int id;
	int type;
	QIcon icon;
	int haveicon;
	TPage page;
	page.curRow = -1;
	TLine lin;
	QString name;
	QBuffer buf;
	QDataStream strm;
	prjInit();
	data.remove(0,4);		// header (TRB7)
	data = qUncompress(data);	// unpack data
	buf.setBuffer(&data);
	buf.open(QIODevice::ReadOnly);
	strm.setDevice(&buf);
	strm >> type;
	while (!strm.atEnd() && (type != T7_END)) {
		switch(type) {
			case T7_PAGE:
				page.text.clear();
				strm >> type;
				while (type != T7_END) {
					switch(type) {
						case TP_ID: strm >> page.id; break;
						case TP_FLAG: strm >> page.flag; break;
						case TP_LINE:
							lin.type = TL_TEXT;
							lin.flag = 0;
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
									default: skipUnknown(strm, type); break;
								}
								strm >> type;
							}
							page.text.append(lin);
							break;
						default: skipUnknown(strm,type); break;
					}
					strm >> type;
				}
				putPage(page);
				break;
			case T7_TREE:
				strm >> type;
				while (type != T7_END) {
					switch (type) {
						case TT_DIR:
							haveicon = 0;
							strm >> type;
							while (type != TT_END) {
								switch(type) {
									case TT_NAME: strm >> name; break;
									case TT_ICON: strm >> icon; haveicon = 1; break;
									default: skipUnknown(strm,type); break;
								}
								strm >> type;
							}
							if (haveicon)
								par = addItem(par,name,0,icon);
							else
								par = addItem(par,name,0);
							break;
						case TT_PAGE:
							strm >> type;
							while (type != TT_END) {
								switch(type) {
									case TT_NAME: strm >> name; break;
									case TT_PID: strm >> id; break;
									default: skipUnknown(strm,type); break;
								}
								strm >> type;
							}
							addItem(par,name,id);
							break;
						case TT_END:
							par = par->parent();
							if (par == NULL) par = ui.tree->invisibleRootItem();
							break;
						default:
							skipUnknown(strm,type);
							break;
					}
					strm >> type;
				}
				break;
			default:
				skipUnknown(strm,type);
				break;
		}
		strm >> type;
	}
}

void MWindow::openPrj(QString path) {
	if (path == "") path = QFileDialog::getOpenFileName(this,"Open book","","Book files (*.trb)");
	if (path == "") return;
	QFile file(path);
	if (file.open(QFile::ReadOnly)) {
		QByteArray data = file.readAll();
		file.close();
		if (QDialog::trUtf8(data.left(3)) == "TRB") {
			prjPath = path;
			int ver = QDialog::trUtf8(data.mid(3,1)).toInt();
			qDebug()<<"ver "<<ver;
			ui.tree->clear();
			book.clear();
			QTreeWidgetItem* par = ui.tree->invisibleRootItem();
			switch (ver) {
				case 4: loadVer4(data,par); break;
				case 5: loadVer5(data,par); break;
				case 6: loadVer6(data,par); break;
				case 7: loadVer7(data,par); break;
			}
		}
	}
}

void MWindow::saveLeaf(QTreeWidgetItem* par, QBuffer* buf) {
	int i;
	int id;
	QTreeWidgetItem* itm;
	QDataStream stream(buf);
	for (i = 0; i < par->childCount(); i++) {
		itm = par->child(i);
		id = itm->data(0,Qt::UserRole).toInt();
		if (id == 0) {
			stream << (int)TPART;
			stream << itm->text(0);
			saveLeaf(itm,buf);
		} else {
			stream << (int)TPAGE;
			stream << itm->text(0);
			stream << id;
		}
	}
	stream << (int)TEND;
}

void MWindow::saveIt() {
	savePrj(prjPath);
}

void saveLeaf7(QDataStream& strm, QTreeWidgetItem* par) {
	int i;
	int id;
	QTreeWidgetItem* itm;
	for (i = 0; i < par->childCount(); i++) {
		itm = par->child(i);
		id = itm->data(0,Qt::UserRole).toInt();
		if (id == 0) {
			strm << (int)TT_DIR;
			strm << (int)TT_NAME; strm << itm->text(0);
			if (!itm->icon(0).name().contains("folder.png")) {
				strm << (int)TT_ICON; strm << itm->icon(0);
			}
			strm << (int)TT_END;
			saveLeaf7(strm, itm);
		} else {
			strm << (int)TT_PAGE;
			strm << (int)TT_NAME; strm << itm->text(0);
			strm << (int)TT_PID; strm << (int)id;
			strm << (int)TT_END;
		}
	}
	strm << (int)TT_END;
}

bool MWindow::savePrj(QString path) {
	QByteArray data;	// all data
	QBuffer buf;

	QDataStream strm;

	if (path == "") path = QFileDialog::getSaveFileName(this,"Save book",prjPath,"Book files (*.trb)");
	if (path == "") return false;

	QFile file(path);
	if (!file.open(QFile::WriteOnly)) return false;
	prjPath = path;

	buf.setBuffer(&data);
	buf.open(QIODevice::WriteOnly);
#if 1
//	int pc,lc;
	TPage page;
	TLine line;
	strm.setDevice(&buf);
	foreach(page, book) {
//	for(pc = 0; pc < book.size(); pc++) {
//		page = book.at(pc);
		strm << (int)T7_PAGE;
		strm << (int)TP_ID << (int)page.id;
		strm << (int)TP_FLAG << (int)page.flag;
		foreach(line, page.text) {
//		for (lc = 0; lc < page.text.size(); lc++) {
//			line = page.text.at(lc);
			if (~line.flag & FL_HIDDEN) {
				strm << (int)TP_LINE;
				strm << (int)TL_TYPE; strm << line.type;
				strm << (int)TL_FLAG; strm << line.flag;
				strm << (int)TL_SN; strm << line.src.name;
				strm << (int)TL_ST; strm << line.src.text;
				strm << (int)TL_TN; strm << line.trn.name;
				strm << (int)TL_TT; strm << line.trn.text;
				strm << (int)T7_END;
			}
		}
		strm << (int)T7_END;
	}
	strm << (int)T7_TREE;
	saveLeaf7(strm,ui.tree->invisibleRootItem());
	strm << (int)T7_END;
	buf.close();
	data = qCompress(data);

	file.write(QString("TRB7").toUtf8());	// signature
	file.write(data);
	file.close();
#else
	QByteArray cdata;	// compressed block
	QBuffer cbuf;

	TRBHead hd;
	memset(&hd,0x00,sizeof(TRBHead));

	cdata.clear();
	cbuf.setBuffer(&cdata);
	cbuf.open(QIODevice::WriteOnly);
	strm.setDevice(&cbuf);
	strm << book.size();
	foreach (TPage page, book) {
		strm << page.id;
		strm << page.flag;
		strm << page.text.size();
		foreach (TLine line, page.text) {
			strm << line.type;
			strm << line.flag;
			strm << line.src.name;
			strm << line.src.text;
			strm << line.trn.name;
			strm << line.trn.text;
		}
	}
	cbuf.close();
	cdata = qCompress(cdata);
	hd.pages.pos = buf.pos() + sizeof(TRBHead);
	hd.pages.size = cdata.size();
	buf.write(cdata);

	cdata.clear();					// tree structure
	cbuf.setBuffer(&cdata);
	cbuf.open(QIODevice::WriteOnly);
	saveLeaf(ui.tree->invisibleRootItem(),&cbuf);
	cbuf.close();
	cdata = qCompress(cdata);
	hd.tree.pos = buf.pos() + sizeof(TRBHead);
	hd.tree.size = cdata.size();
	buf.write(cdata);

	cdata.clear();					// backgrounds
	cbuf.setBuffer(&cdata);
	strm.setDevice(&cbuf);
	strm << bg.size();
	foreach(TImage img, bg) {
		strm << img.id;
		strm << img.name;
		strm << img.data;
	}
	cbuf.close();
	hd.bg.pos = buf.pos() + sizeof(TRBHead);
	hd.bg.size = cdata.size();
	buf.write(cdata);

	buf.close();

	file.write(QString("TRB6").toUtf8());	// signature
	file.putChar(0x01);			// version
	file.write((const char*)&hd,sizeof(TRBHead));
	file.write(data);
	file.close();
#endif
	return true;
}

void MWindow::saveSrc() {
	if (curPage == NULL) return;
	QString path = QFileDialog::getSaveFileName(this,"Save src text","","All files (*)");
	if (path == "") return;
	QFile file(path);
	if (file.open(QFile::WriteOnly)) {
		for (int i = 0; i < curPage->text.size(); i++) {
			if (curPage->text.at(i).src.name.isEmpty()) {
				file.write(curPage->text.at(i).src.text.toUtf8());
			} else {
				file.write(curPage->text.at(i).src.name.toUtf8());
				file.write("「");
				file.write(curPage->text.at(i).src.text.toUtf8());
				file.write("」");
			}
			file.write("\r\n");
		}
		file.close();
	}
}

QList<TPage> openFiles(QFileDialog::FileMode mode) {
	QList<TPage> res;
	QFileDialog qfd;
	qfd.setNameFilters(QStringList() << "Text files (*)"
			   << "EAGLS script(*)"
			   << "KS files (*.ks)"
			   << "Abelsoft script ADV (*.adv)"
			   << "Enmon script ENM (*.enm)"
			   << "SNX engine (*.snx)"
			   );
	qfd.setWindowTitle("Open sources");
	qfd.setFileMode(mode);
	if (!qfd.exec())
		return res;
	QStringList paths = qfd.selectedFiles();
	QString path = qfd.selectedNameFilter();
	TPage page;
	TPage(*callback)(QString) = NULL;
	if (path.contains("Abelsoft")) callback = &loadAbelsoft;
	if (path.contains("Enmon")) callback = &loadEnmon;
	if (path.contains("KS files")) callback = &loadKS;
	if (path.contains("EAGLS")) callback = &loadEAGLS;
	if (path.contains("SNX")) callback = &loadSNX;
	foreach(path, paths) {
		if (callback) {
			page = callback(path);
		} else {
			page = loadPage(path, TL_SRC);
		}
		if (page.id != 0) {
			page.name = QFileInfo(path).baseName();
			res.append(page);
		}
	}
	return res;
}

void MWindow::insertSrc() {
	if (!ui.editGrid->isEnabled()) return;
	if (!curPage || (curRow < 0)) return;
	QList<TPage> pages = openFiles(QFileDialog::ExistingFile);
	if (pages.size() != 1) return;
	TPage page = pages.first();
	int row = curRow;
	TLine lin;
	foreach(lin, page.text) {
		curPage->text.insert(row, lin);
		model->insertRow(row);
		row++;
	}
	setProgress();
}

void MWindow::openSrc() {
	QList<TPage> pages = openFiles(QFileDialog::ExistingFiles);
	if (pages.size() == 0) return;
	TPage page;
	TPage* pg;
	QTreeWidgetItem* par = getCurrentParent();
	foreach(page, pages) {
		pg = addPage(page);
		addItem(par, page.name, pg->id);
	}
}

QTreeWidgetItem* MWindow::getCurrentParent() {
	QModelIndexList rws = ui.tree->selectionModel()->selectedRows(0);
	QTreeWidgetItem* par = (rws.size() == 1) ? ui.tree->currentItem() : NULL;
	if (par) {
		if (par->data(0,Qt::UserRole).toInt() != 0)
			par = par->parent();
	}
	return par;
}

void MWindow::newDir() {
	QString name = QTime::currentTime().toString("hhmmss");
	QTreeWidgetItem* par = getCurrentParent();
	addItem(par,name,0);
}

// pages

TPage* MWindow::newPage() {
	TPage* pg = createPage();
	QTreeWidgetItem* par = getCurrentParent();
	addItem(par,"",pg->id);
	return pg;
}

QTreeWidgetItem* MWindow::addItem(QTreeWidgetItem* par, QString nam, int id, QIcon icon) {
	TPage* page;
	QTreeWidgetItem* itm = new QTreeWidgetItem();
	itm->setData(0,Qt::UserRole,id);
	if (nam == "") {
		nam = (id == 0) ? "New dir" : QString("Page ").append(QString::number(id + 1000).right(3));
	}
	itm->setText(0,nam);
	itm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable); // | Qt::ItemIsUserCheckable);
	if (id == 0) {
		itm->setFlags(itm->flags() | Qt::ItemIsDropEnabled);
		if (icon.isNull()) {
			itm->setIcon(0,QIcon(":/folder.png"));
		} else {
			itm->setIcon(0,icon);
		}
	} else {
		page = findPage(id);
		if (page) itm->setIcon(0,getIcon(page));
	}

	if (par == NULL) par = ui.tree->invisibleRootItem();
	par->addChild(itm);
	return itm;
}
