#ifndef _MWINDOW_H
#define _MWINDOW_H

#include <QtGui>
#include "base.h"
#include "ui_mainwin.h"

extern TPage* curPage;

class TBModel : public QAbstractTableModel {
	public:
		TBModel(QObject* p = NULL);
		int rowCount(const QModelIndex& idx = QModelIndex()) const;
		int columnCount(const QModelIndex& idx = QModelIndex()) const;
		QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const;
		QVariant headerData(int,Qt::Orientation, int role = Qt::DisplayRole) const;
		void update();
		void updateCell(int,int);
		void insertRow(int, const QModelIndex& idx = QModelIndex());
		void removeRow(int, const QModelIndex& idx = QModelIndex());
};

class MWindow : public QMainWindow {
	Q_OBJECT
	public:
		MWindow();
	public slots:
		void openPrj(QString path = "");
	private:
		Ui::MainWin ui;
//		Ui::Replacer rui;
		int curRow;
		QTreeWidgetItem* curItem;
		QString prjPath;
		QClipboard* clip;
		TBModel* model;

		QMenu* tbMenu;
		QMenu* sjMenu;
		QMenu* bmMenu;
		void fillSJMenu();

		void fillBlock(const QList<TLine>*);
		void disableTab();
		QTreeWidgetItem* addItem(QTreeWidgetItem*,QString,int,QIcon = QIcon());

		void setPage(int);
		void setProgress();
		void setEdit(bool);

		void lineUp();
		void lineDown();

		void saveLeaf(QTreeWidgetItem*,QBuffer*);

		int getCurrentRow();
		QTreeWidgetItem* getCurrentParent();

		void loadVer4(QByteArray&, QTreeWidgetItem*);
		void loadVer5(QByteArray&, QTreeWidgetItem*);
		void loadVer6(QByteArray&, QTreeWidgetItem*);
		void loadVer7(QByteArray&, QTreeWidgetItem*);
	private slots:
		void tbContextMenu();
		void jumpLine(QAction*);
		void findUntrn();
		void filter(QString);

		void appendCbrd();
		void changePage();
		void changeRow(QItemSelection);
		void changeSrc(QString);
		void changeTrn(QString);
		void changeSNm(QString);
		void changeTNm(QString);

		void newDir();
		void delPage();
		void delItem(QTreeWidgetItem*);
		void sortTree();
		void mergePages();
		void clearTrn();

//		void replace();

		void rowDelete();
		void rowInsert(bool);
		void pageSplit();
		void changeIcon();

		void newPrj();
		bool savePrj(QString path = "");
		void saveIt();

		TPage* newPage();
		void openSrc();
		void insertSrc();
		void saveSrc();
	protected:
		void keyPressEvent(QKeyEvent*);
		void closeEvent(QCloseEvent*);
};

#endif // WINDOW_H
