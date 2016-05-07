#ifndef BASE_H
#define BASE_H

#include <QtCore>
#include <QTableWidget>

#define	TL_SRC	0
#define	TL_TRN	1

#define	TL_TEXT		1
#define	TL_SELECT	2
#define	TL_NOTE		4
#define TL_SEPARATOR	5
#define	TL_LABEL	6
#define	TL_COM		7

#define	FL_HIDDEN	1
#define FL_BOOKMARK	(1 << 1)

#define LS_NONE		0
#define	LS_UNTRN	1
#define	LS_TRN		2

struct TPhrase {
	QString name;
	QString text;
};

struct TLine {
	int type;
	int flag;
	TPhrase src;
	TPhrase trn;
};

struct TPage {
	QUuid id;
	int flag;
	int curRow;
	QString name;
	QList<TLine> text;
};

void prjInit();

TPage* createPage();
TPage* addPage(TPage);
TPage* putPage(TPage);
TPage* findPage(QUuid);
TPage loadPage(QString, int);
void removePage(QUuid);

extern QList<TPage> book;

void getCounts(TPage*,int&,int&);
int getProgress(TPage*);
void normLine(TLine&);

TPage loadEnmon(QString);
TPage loadAbelsoft(QString);
TPage loadEAGLS(QString);
TPage loadSRP(QString);

void normLine(TLine&);
int getLineStatus(TLine);

#endif // BASE_H
