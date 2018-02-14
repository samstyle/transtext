#ifndef _MWINDOW_H
#define _MWINDOW_H

#include <QtGui>
#include <QFileDialog>
#include "base.h"
#include "ui_mainwin.h"
#include "ui_iconwindow.h"
#include "ui_bookmark.h"

extern TPage* curPage;

enum {
	roleId = Qt::UserRole,
	roleIcon
};

class TBModel : public QAbstractTableModel {
	public:
		TBModel(QObject* p = NULL);
		int rowCount(const QModelIndex& idx = QModelIndex()) const;
		int columnCount(const QModelIndex& idx = QModelIndex()) const;
		QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const;
		QVariant headerData(int,Qt::Orientation, int role = Qt::DisplayRole) const;
		void update();
		void updateLine(int);
		void updateCell(int,int);
		void insertRow(int, const QModelIndex& idx = QModelIndex());
		void removeRow(int, const QModelIndex& idx = QModelIndex());
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
};

class MWindow : public QMainWindow {
	Q_OBJECT
	public:
		MWindow();
	public slots:
		void openPrj(QString path = "");
	private:
		Ui::MainWin ui;
		Ui::IconWin icoui;
		Ui::AddBookmark bmui;

		QDialog* icowin;
		QDialog* bmwin;
		QFileDialog fdial;

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

		void loadVer78(QByteArray&, QTreeWidgetItem*);
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
		bool savePrj(QString path = "");
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

#endif // WINDOW_H
