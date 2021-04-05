#pragma once

#include <QtGui>
#include <QFileDialog>

#include "ui_mainwin.h"
#include "ui_iconwindow.h"
#include "ui_bookmark.h"
#include "ui_bookmarks.h"

#include "models.h"
#include "base.h"

extern TPage* curPage;

enum {
	roleId = Qt::UserRole,
	roleIcon,
};

class TRBLoader {
	public:
		TRBLoader() {}
		int load(QString, QTreeWidgetItem*);
		int save(QString, QTreeWidgetItem*);
	private:
		QBuffer buf;
		QDataStream strm;

		QList<int> getlist();
		void putlist(QList<int>&);
		QTreeWidgetItem* add_item(QTreeWidgetItem*, QString, QUuid, QUuid=0);

		void v7_load(QTreeWidgetItem*);
		int v7_load_icons();
		int v7_load_bookmarks();
		int v7_load_page();
		int v7_load_tree(QTreeWidgetItem*);

		int v7_save(QTreeWidgetItem*);
		void v7_save_tree(QTreeWidgetItem*);
		void v7_save_leaf(QTreeWidgetItem*);

		void v8_load(QTreeWidgetItem*);
		void v8_load_icons();
		void v8_load_bookmarks();
		void v8_load_pages();
		void v8_load_tree(QTreeWidgetItem*);

		int v8_save(QTreeWidgetItem*);
		QByteArray v8_save_pages(QTreeWidgetItem*);
		QByteArray v8_save_tree(QTreeWidgetItem*);
		QByteArray v8_save_icons(QTreeWidgetItem*);
		QByteArray v8_save_bookmarks(QTreeWidgetItem*);
};

class MWindow : public QMainWindow {
	Q_OBJECT
	public:
		MWindow();
	public slots:
		void openPrj(QString path = "");
		void replace(QString, QString);
	signals:
		void rqReplace();
	private:
		Ui::MainWin ui;
		Ui::IconWin icoui;
		Ui::AddBookmark bmui;
		Ui::Bookmarks blui;

		QDialog* icowin;
		QDialog* bmwin;
		QFileDialog fdial;

		QDialog* blwin;
		BMLModel* blmod;

		int curRow;
		QTreeWidgetItem* curItem;
		QString prjPath;
		QClipboard* clip;
		TBModel* model;

		QMenu* tbMenu;
		QMenu* sjMenu;
		QMenu* bmMenu;
		QMenu* treeMenu;
		void fillSJMenu();

		void fillBlock(const QList<TLine>*);
		void disableTab();
		QTreeWidgetItem* addItem(QTreeWidgetItem*,QString,QUuid,QUuid = QUuid(0));

		void setPage(QUuid);
		void setProgress();
		void setEdit(bool);

		void lineUp();
		void lineDown();

		void saveLeaf(QTreeWidgetItem*,QBuffer*);

		int getCurrentRow();
		QTreeWidgetItem* getCurrentParent();
		int selectItemByPageID(QUuid);

		TRBLoader trb;

		void loadVer78(QByteArray&, QTreeWidgetItem*);
//		void loadVer8(QByteArray&, QTreeWidgetItem*);
	private slots:
		void treeContextMenu();
		void tbContextMenu();
		void jumpLine(QAction*);
		void findUntrn();

		void findStr(QString);
		void findNext();
		void findPrev();

		void appendCbrd();
		void changePage();
		void changeRow(QItemSelection);
		void changeSrc(QString);
		void changeTrn(QString);
		void changeSNm(QString);
		void changeTNm(QString);

		void pageInfo();
		void bmList();
		void goToBookmark(const QModelIndex&);
		void treeItemChanged(QTreeWidgetItem*);

		void saveBranch();

		void newDir();
		void delPage();
		void delItem(QTreeWidgetItem*);
		void sortTree();
		void mergePages();
		void clearTrn();

		void joinLine();
		void splitLine();
		void splitName();

		void rowDelete();
		void rowInsert(bool);
		void pageSplit();

		void changeIcon();
		void fillIconList();
		void setIcon(QListWidgetItem*);
		void loadIcon();
		void delIcon();

		void askBookmark();
		void askRmBookmark();
		void newBookmark();

		void newPrj();
		void mergePrj(QString path = "");
		bool savePrj(QString path = "", QTreeWidgetItem* = NULL);
		void saveIt();
		int saveChanged();

		TPage* newPage();
		void openSrc();
		void insertSrc();
		void saveSrc();
	protected:
		void keyPressEvent(QKeyEvent*);
		void closeEvent(QCloseEvent*);
};
