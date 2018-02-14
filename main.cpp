#include <QDialog>

#include "base.h"
#include "mainwin.h"

MWindow* win;
QList<TPage> book;
QList <TIcon> icons;
QList <TBookmark> bookmarks;

QList<TPage> getBook() {
	return book;
}

void prjInit() {
	book.clear();
	icons.clear();
	bookmarks.clear();
}

TPage* putPage(TPage page) {
	book.append(page);
	return &book.last();
}

TPage* addPage(TPage page) {
	page.id = QUuid::createUuid();
	book.append(page);
	return &book.last();
}

TPage* createPage() {
	TPage page;
	page.curRow = -1;
	page.id = QUuid::createUuid();
	page.text.clear();
	book.append(page);
	return &book.last();
}

void removePage(QUuid id) {
	int i;
	for (i = 0; i < book.size(); i++) {
		if (book[i].id == id) {
			book.removeAt(i);
			break;
		}
	}
}

void putText(TLine& line,QString data,int type) {
	switch (type) {
		case TL_SRC:
			line.src.text = data;
			line.trn.text.clear();
			break;
		case TL_TRN:
			line.src.text.clear();
			line.trn.text = data;
			break;
	}
}

QStringList splitLine(QString text, QString div) {
	QStringList lst;
	lst << "" << "";
	int pos = text.indexOf(div);
	if (pos < 0) {
		lst.last() = text;
	} else {
		lst.first() = text.left(pos);
		lst.last() = text.mid(pos + 1);
	}
	return lst;
}

void normLine(TLine& line) {
	QStringList pair;
	line.src.name.remove(QObject::trUtf8("　"));
	line.src.text.remove(QObject::trUtf8("　"));
	pair.clear();
	pair.append(line.src.name);
	pair.prepend(line.src.text);
	if (line.src.name == "") {
		if (line.src.text.endsWith(QObject::trUtf8("」"))) {
			pair = splitLine(line.src.text,QObject::trUtf8("「"));
			line.src.name = pair.first();
			line.src.text = pair.last();
			line.src.text.remove(QObject::trUtf8("「"));
			line.src.text.remove(QObject::trUtf8("」"));
		}
		if (line.src.text.indexOf(QObject::trUtf8("\t")) != -1) {
			pair = splitLine(line.src.text,QObject::trUtf8("\t"));
			line.src.name = pair.first();
			line.src.text = pair.last();
		}
		if (line.src.text.startsWith(QObject::trUtf8("【"))) {
			int idx = line.src.text.indexOf(QObject::trUtf8("】"));
			if (idx != -1) {
				line.src.name = line.src.text.mid(1,idx-1);
				line.src.text = line.src.text.mid(idx+1);
			}
		}
		line.src.name.remove(QObject::trUtf8("【"));
		line.src.name.remove(QObject::trUtf8("】"));
	}

	if (line.trn.name == "") {
		pair = splitLine(line.trn.text,QObject::trUtf8("\t"));
		line.trn.name = pair.first();
		line.trn.text = pair.last();
	}
}

// icons

QIcon findIcon(QUuid id) {
	if (id.isNull()) return QIcon(":/folder.png");
	QIcon ico;
	for (int i = 0; i < icons.size(); i++) {
		if (icons.at(i).id == id) {
			ico = icons.at(i).icon;
		}
	}
	return ico;
}

int addIcon(TIcon ico) {
	int res = 1;
	if (ico.id.isNull()) {
		ico.id = QUuid::createUuid();
	}
	if (findIcon(ico.id).isNull()) {
		icons.append(ico);
	} else {
		res = 0;
	}
	return res;
}

void rmIcon(QUuid id) {
	for (int i = icons.size() - 1; i >= 0; i--) {
		if (icons.at(i).id == id)
			icons.removeAt(i);
	}
}

// bookmarks

TBookmark* findBookmark(QUuid id) {
	TBookmark* res = NULL;
	for (int i = 0; i < bookmarks.size(); i++) {
		if (bookmarks.at(i).id == id) {
			res = &bookmarks[i];
		}
	}
	return res;
}

QUuid addBookmark(TBookmark bm) {
	TBookmark* tb = NULL;
	if (bm.id.isNull()) {			// new bookmark
		bm.id = QUuid::createUuid();
		bookmarks.append(bm);
	} else {				// find if exists
		tb = findBookmark(bm.id);
		if (!tb) {
			bookmarks.append(bm);		// not found
		} else {
			tb->name = bm.name;		// found: update existing bookmark
			tb->descr = bm.descr;
		}
	}
	return bm.id;
}

void rmBookmark(QUuid id) {
	for (int i = bookmarks.size() - 1; i >= 0; i--) {
		if (bookmarks.at(i).id == id)
			bookmarks.removeAt(i);
	}
}

// abelsoft scripts

TPage loadAbelsoft(QString fnam, int cpage) {
	TPage page;
	QFile file(fnam);
	QString line;
	QString name;
	TLine tlin;
	tlin.type = TL_TEXT;
	int idx,i;
	QStringList taglist;
	QStringList arglist;
	QStringList argvals;
	QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
	if (file.open(QFile::ReadOnly)) {
		while (!file.atEnd()) {
			line = codec->toUnicode(file.readLine());
			line.remove("\r");
			line.remove("\n");
			idx = line.indexOf(";");
			if (idx > -1) line = line.left(idx);
			if (!line.isEmpty()) {
				if (line.startsWith("<")) {
					taglist = line.split(QRegExp("[<>]"),QString::SkipEmptyParts);
					for (idx = 0; idx < taglist.size(); idx++) {
						arglist = taglist.at(idx).split(" ",QString::SkipEmptyParts);
						if (arglist.first() == "WINDOW") {
							for (i = 1; i < arglist.size(); i++) {
								argvals = arglist.at(i).split("=",QString::SkipEmptyParts);
								if (argvals.first() == "NAME") name = argvals.last().remove("\"");
							}
						} else if (arglist.first() == "IMG") {
							for (i = 1; i < arglist.size(); i++) {
								argvals = arglist.at(i).split("=",QString::SkipEmptyParts);
								if (argvals.first() == "SRC") {
									tlin.src.name.clear();
									tlin.src.text.clear();
									page.text.append(tlin);
									tlin.src.text = QString("[img = ").append(argvals.last().remove("\"")).append("]");
									page.text.append(tlin);
								}
							}
						} else if (arglist.first() == "SOUND") {
							// do nothing
						} else {
							tlin.src.name.clear();
							tlin.src.text = QString("[%0]").arg(taglist.at(idx));
							page.text.append(tlin);
						}
					}
				} else {
					if (!name.isEmpty()) {
						if (line.startsWith(QObject::trUtf8("「"))) line.remove(0,1);
						if (line.endsWith(QObject::trUtf8("」"))) line.resize(line.length() - 1);
					}

					tlin.src.name = name;
					tlin.src.text = line;
					page.text.append(tlin);
					name.clear();
				}
			}
		}
	}
	return page;
}

TPage loadPage(QString fnam, int type) {
	TPage page;
	QFile file(fnam);
	QString line;
	QStringList com;
	QString cond = "";
	QStringList condStack;
	TLine tlin;

	QString bg = "";
	QString pl = "";
	QString pm = "";
	QString pr = "";
	page.id = 0;
	page.text.clear();
	condStack.clear();
	if (file.open(QFile::ReadOnly)) {
		while (!file.atEnd()) {

			tlin.type = 0;
			tlin.src.text.clear();
			tlin.src.name.clear();
			tlin.trn.text.clear();
			tlin.trn.name.clear();

			line = QString::fromUtf8(file.readLine()).remove("\r").remove("\n");

//			if (line != "") {
				if (line.startsWith("@")) {
					com = line.split(":",QString::KeepEmptyParts);
					while (com.size() < 5) com.append("");

					// @S:id:name		selection
					if (com[0] == "@S") {
						tlin.type = TL_SELECT;
						putText(tlin,com[2],type);
						page.text.append(tlin);
					}

					// @B:id ... @E		block
					if (com[0] == "@B") {
						if (cond != "") condStack.append(cond);
						cond = com[1];
					}
					if (com[0] == "@E") {
						if (condStack.size() > 0) {
							cond = condStack.last();
							condStack.removeLast();
						} else {
							cond.clear();
						}
					}

					// @L:id:text		condition line
					if (com[0] == "@L") {
						tlin.type = TL_TEXT;
						putText(tlin,com[2],type);
						page.text.append(tlin);
					}

					// @BG:name
					if (com[0] == "@BG") {
						bg = com[1];
					}

					// @PL[PM,PR]:name
					if (com[0] == "@PL") {
						pl = com[1];
					}
					if (com[0] == "@PM") {
						pm = com[1];
					}
					if (com[0] == "@PR") {
						pr = com[1];
					}


				} else {
					tlin.type = TL_TEXT;
					tlin.flag = 0;
					putText(tlin,line,type);
					normLine(tlin);
					page.text.append(tlin);
				}
//			}
		}
	}
	return page;
}

int getLineStatus(TLine line) {
	int res = LS_NONE;
	if (line.type == TL_TEXT) {
		if (!(line.src.text.startsWith("[") ||
			(line.src.text.startsWith("==")) ||
			(line.src.text.isEmpty() && line.src.name.isEmpty()))) {
				res = LS_UNTRN;
				if (line.src.text.isEmpty() || !line.trn.text.isEmpty())
					res = LS_TRN;
		}
	}
	return res;
}

void getCounts(TPage* page,int& trans, int& total) {
	trans = 0;
	total = 0;
	foreach(TLine line, page->text) {
		switch(getLineStatus(line)) {
			case LS_TRN:
				trans++;
			case LS_UNTRN:
				total++;
				break;
		}
	}
}

int getProgress(TPage* page) {
	int total;
	int trans;
	getCounts(page,trans,total);
	if (total == 0) return 0;
	return trans * 100.0 / (double)total;
}

TPage* findPage(QUuid id) {
	int i;
	TPage* page = NULL;
	for (i=0; i<book.size(); i++) {
		if (book[i].id == id) {
			page = &book[i];
			break;
		}
	}
	return page;
}

int main(int ac,char** av) {
	QApplication app(ac,av);

	win = new MWindow;
	win->show();

	if (ac > 1) win->openPrj(QString(av[1]));

	return app.exec();
}
