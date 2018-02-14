#ifndef BASE_H
#define BASE_H

#include <QtCore>
#include <QTableWidget>

enum {
	CP_SJIS = 0,
	CP_UNICODE
};

enum {
	TL_SRC = 0,
	TL_TRN
};

enum {
	TL_TEXT = 1,
	TL_SELECT,
	TL_NOTE,
	TL_SEPARATOR,
	TL_LABEL,
	TL_COM
};

//#define	FL_HIDDEN	1
//#define FL_BOOKMARK	(1 << 1)

enum {
	LS_NONE = 0,
	LS_UNTRN,
	LS_TRN
};

struct TPhrase {
	QString name;
	QString text;
};

struct TLine {
	int type = TL_TEXT;
	int flag = 0;
	QUuid bmrkId;
	TPhrase src;
	TPhrase trn;
};

struct TIcon {
	QUuid id;
	QString name;
	QIcon icon;
};

struct TPage {
	QUuid id;
	int flag;
	int curRow;
	QString name;
	QUuid iconId;
	QList<TLine> text;
};

struct TBookmark {
	QUuid id;
	QString name;
	QString descr;
};

void prjInit();

TPage* createPage();
TPage* addPage(TPage);
TPage* putPage(TPage);
TPage* findPage(QUuid);
TPage loadPage(QString, int);
void removePage(QUuid);

extern QList<TPage> book;

QIcon findIcon(QUuid);
int addIcon(TIcon);
void rmIcon(QUuid);

extern QList<TIcon> icons;

TBookmark* findBookmark(QUuid);
QUuid addBookmark(TBookmark);
void rmBookmark(QUuid);

extern QList<TBookmark> bookmarks;

void getCounts(TPage*,int&,int&);
int getProgress(TPage*);
void normLine(TLine&);

void normLine(TLine&);
int getLineStatus(TLine);

#endif // BASE_H
