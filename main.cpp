#include <QtGui>
#include "base.h"
#include "mainwin.h"

MWindow* win;
QList<TPage> book;
QList<TImage> bg;
int maxid = 0;

int getid() {
	maxid++;
	return maxid;
}

QList<TPage> getBook() {
	return book;
}

void prjInit() {
	book.clear();
	maxid = 0;
}

TPage* putPage(TPage page) {
	book.append(page);
	if (page.id >= maxid) maxid = page.id + 1;
	return &book.last();
}

TPage* addPage(TPage page) {
	page.id = getid();
	book.append(page);
	return &book.last();
}

TPage* createPage() {
	TPage page;
	page.curRow = -1;
	page.id = getid();
	page.text.clear();
	book.append(page);
	return &book.last();
}

void removePage(int id) {
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
	pair.clear();
	pair.append(line.src.name);
	pair.prepend(line.src.text);
	if (line.src.name == "") {
		if (line.src.text.endsWith(QDialog::trUtf8("」"))) {
			pair = splitLine(line.src.text,QDialog::trUtf8("「"));
			line.src.name = pair.first();
			line.src.text = pair.last();
			line.src.text.remove(QDialog::trUtf8("「"));
			line.src.text.remove(QDialog::trUtf8("」"));
		}
		if (line.src.text.indexOf(QDialog::trUtf8("\t")) != -1) {
			pair = splitLine(line.src.text,QDialog::trUtf8("\t"));
			line.src.name = pair.first();
			line.src.text = pair.last();
		}
		if (line.src.text.startsWith(QDialog::trUtf8("【"))) {
			int idx = line.src.text.indexOf(QDialog::trUtf8("】"));
			if (idx != -1) {
				line.src.name = line.src.text.mid(1,idx-1);
				line.src.text = line.src.text.mid(idx+1);
			}
		}
		line.src.name.remove(QDialog::trUtf8("【"));
		line.src.name.remove(QDialog::trUtf8("】"));
	}

	if (line.trn.name == "") {
		pair = splitLine(line.trn.text,QDialog::trUtf8("\t"));
		line.trn.name = pair.first();
		line.trn.text = pair.last();
	}
}

// eagls script

// QMap<QString,QString> pars;

QStringList parseEAGLCom(QString& line) {
	QStringList res;
	int pos1 = line.indexOf("(");
	int pos2 = line.indexOf(")");
	if ((pos1 < 0) || (pos2 < pos1)) {
		res << line;
		line.clear();
	} else {
		QString pars = line.mid(pos1 + 1, pos2 - pos1 - 1);
		res = pars.split(",",QString::SkipEmptyParts);
		for (int i = 0; i < res.size(); i++) {
			res[i] = res[i].remove("\"");
		}
		res.prepend(line.left(pos1));
		line = line.mid(pos2 + 1);
	}
	return res;
}

QMap<QString,QString> eamap;

QString eagIfPars(QStringList prs) {
	QString ifpar;
	if (prs.size() > 3) {
		ifpar = QString("(%1 %3 %2)").arg(prs[1]).arg(prs[2]).arg(prs[3]);
		prs.removeFirst();
		prs.removeFirst();
		prs.removeFirst();
		while (prs.size() > 3) {
			ifpar.append(QString(" %4 (%1 %3 %2)").arg(prs[1]).arg(prs[2]).arg(prs[3]).arg(prs.last()));
			prs.removeFirst();
			prs.removeFirst();
			prs.removeFirst();
		}
	} else {
		ifpar = prs.join(",");
	}
	return ifpar;
}

void parseEAGLine(QString line, TPage* page, QString* name) {
	if (line.isEmpty()) return;
	QStringList prs;
	QStringList ddz;
	QString num;
	TLine tlin;
	tlin.flag = 0;
	int pos;
	if (line.startsWith("$")) {			// $label
		tlin.src.text = QString("== ").append(line);
		tlin.type = TL_LABEL;
		page->text.append(tlin);
	} else if (line.startsWith("#")) {
		*name = line.split(QRegExp("[#=]"),QString::SkipEmptyParts).first();
	} else if (QString("123456789").contains(line.at(0))) {
		tlin.type = TL_COM;
		num = line;
		prs = parseEAGLCom(line);
		if (prs[0] == "52") {
			//tlin.src.text = QString("[%1 = %2]").arg(prs[1]).arg(prs[2]);
			//page->text.append(tlin);
			eamap[prs[1]] = prs[2];
		} else if (prs[0] == "41") {
			tlin.type = TL_TEXT;
			tlin.src.text = QString("[ jp %1 # %2]").arg(prs[2]).arg(prs[1]);
			page->text.append(tlin);
		} else if (prs[0] == "42") {
			if (prs[2] == "00Haikei.dat") {
				if (prs[1] == "Black") num = "black"; else num = eamap["_Haikei"];
				tlin.type = TL_TEXT;
				tlin.src.text = QString("[BG : %1]").arg(num);
				page->text.append(tlin);
			} else if (((prs[1] == "DrawBG") || (prs[1] == "DrawCG")) && (prs[2] == "00Draw.dat")) {
				tlin.type = TL_TEXT;
				tlin.src.text = QString("[BG : %1]").arg(eamap["_GRPName"]);
				page->text.append(tlin);
			} else if (((prs[1] == "Cg") || (prs[1] == "Bg")) && (prs[2] == "00Draw.dat")) {
				tlin.type = TL_TEXT;
				tlin.src.text = QString("[BG : %1]").arg(eamap["_DrawName"]);
				page->text.append(tlin);
/*			} else if ((prs[1] == "DrawChar") && (prs[2] == "00DrawChar.dat")) {
				tlin.type = TL_TEXT;
				if (eamap.count("_CharName") > 0) {
					tlin.src.text = QString("[CH : %1]").arg(eamap["_CharName"]);
				} else {
					tlin.src.text = QString("[CH : %1]").arg(eamap["_GRPName"]);
				}
				page->text.append(tlin);
*/
			} else if ((prs[2] == "00CGIcon.dat") && (prs[1] == "CGIcon")) {
				tlin.type = TL_TEXT;
				tlin.src.text = QString("[BG : %1]").arg(eamap["_Haikei"]);
				page->text.append(tlin);
			} else if ((prs[2] == "Choices.dat") && (prs[1].startsWith("Choices"))) {
				tlin.type = TL_TEXT;
				tlin.src.text.clear();
				page->text.append(tlin);
				tlin.src.text = "[select]";
				page->text.append(tlin);
				ddz.clear();
				ddz << "_Character" << "_Character2" << "_Character3" << "_Character4" << "_Character5" << "_Character6";
				foreach (num, ddz) {
					if (eamap.count(num) > 0) {
						if (!eamap[num].isEmpty()) {
							tlin.src.text = QString(eamap[num]);
							page->text.append(tlin);
						}
					}
				}
				tlin.src.text.clear();
				page->text.append(tlin);
				eamap.clear();
			} else if (prs[2].endsWith("Select.dat") || (prs[2] == "95SelectH.dat")) {
				tlin.type = TL_TEXT;
				tlin.src.text.clear();
				page->text.append(tlin);
				tlin.src.text = "[select]";
				page->text.append(tlin);
				ddz.clear();
				ddz << "_SelStr0" << "_SelStr1" << "_SelStr2" << "_SelStr3" << "_SelStr4" << "_SelStr5";
				ddz << "_SelChar0" << "_SelChar1" << "_SelChar2" << "_SelChar3" << "_SelChar4" << "_SelChar5";
				foreach (num, ddz) {
					if (eamap.count(num) > 0) {
						if (!eamap[num].isEmpty()) {
							tlin.src.text = QString(eamap[num]);
							page->text.append(tlin);
						}
					}
				}
				tlin.src.text.clear();
				page->text.append(tlin);
				eamap.clear();
			} else if ((prs[2] == "00MovPlay.dat") || (prs[2] == "00HMovPlay.dat")) {
				tlin.type = TL_TEXT;
				if (eamap.count("_MOVName")) tlin.src.text = QString("[MOV : %1]").arg(eamap["_MOVName"]);
				else if (eamap.count("_MovieFileName")) tlin.src.text = QString("[MOV : %1]").arg(eamap["_MovieFileName"]);
				else if (eamap.count("_GRPName")) tlin.src.text = QString("[BG : %1]").arg(eamap["_GRPName"]);
				page->text.append(tlin);
			} else if (prs[2] == "01draw.dat") {
				if ((prs[1] == "DrawCGEX") && (eamap.count("_GRPName") > 0)) {
					tlin.src.text = QString("[BigBG : %1]").arg(eamap["_GRPName"]);
					page->text.append(tlin);
				}
			} else if ((prs[2] == "90examine.dat") && (prs[1] == "examine")) {
				tlin.type = TL_TEXT;
				tlin.src.text.clear();
				page->text.append(tlin);
				tlin.src.text = "[select]";
				page->text.append(tlin);
				ddz.clear();
				ddz << "_SQ_Sel01Text" << "_SQ_Sel02Text" << "_SQ_Sel03Text" << "_SQ_Sel04Text" << "_SQ_Sel05Text";
				foreach(num,ddz) {
					if (eamap.count(num) > 0) {
						if (!eamap[num].isEmpty()) {
							tlin.src.text = QString(eamap[num]);
							page->text.append(tlin);
							eamap[num].clear();
						}
					}
				}
				eamap.clear();
			}
		} else if (prs[0].startsWith("120,")) {
			pos = num.indexOf("(");
			num = num.mid(4,pos-4);
			tlin.src.text = QString("[ if.%1 %2 ]").arg(num).arg(eagIfPars(prs));
			tlin.type = TL_TEXT;
			page->text.append(tlin);
			pos = line.indexOf("{");
			line = line.mid(pos + 1);
		} else if (prs[0].startsWith("122,")) {
			pos = num.indexOf("(");
			num = num.mid(4,pos-4);
			tlin.type = TL_TEXT;
			tlin.src.text = QString("[ elseif.%1 %2 ]").arg(num).arg(eagIfPars(prs));
			page->text.append(tlin);
			pos = line.indexOf("{");
			line = line.mid(pos + 1);
		} else if (prs[0].startsWith("121,")) {
			pos = line.indexOf("{");
			num = num.mid(4,pos-4);
			tlin.type = TL_TEXT;
			tlin.src.text = QString("[ else.%1 ]").arg(num);
			page->text.append(tlin);
			line = line.mid(pos + 1);
		}
		parseEAGLine(line,page,name);
	} else if (line.startsWith("}")) {
		pos = line.indexOf(";");
		num = line.mid(1,pos-1);
		tlin.src.text = QString("[ endif.%1 ]").arg(num);
		tlin.type = TL_TEXT;
		page->text.append(tlin);
		line = line.mid(pos+1);
		parseEAGLine(line,page,name);
	} else if (line.startsWith("_")) {

	} else if (line.startsWith("&")) {
		int pos = line.indexOf("\"");
		if (pos > 0) line = line.mid(pos + 1);
		tlin.src.name = *name;
		tlin.src.name.remove(QDialog::trUtf8("　"));
		tlin.src.name.replace(":NameSuffix",QDialog::trUtf8("俺"));
		name->clear();
		pos = line.indexOf("\"");
		if (pos > 0) {
			tlin.src.text = line.left(pos);
			if (tlin.src.text.startsWith(QDialog::trUtf8("「")) && tlin.src.text.endsWith(QDialog::trUtf8("」"))) {
				tlin.src.text.remove(0,1);
				tlin.src.text.remove(tlin.src.text.size()-1,1);
			}
			tlin.src.text.remove("(e)");
			tlin.type = TL_TEXT;
			page->text.append(tlin);
			line = line.mid(pos + 1);
		}
		parseEAGLine(line,page,name);
	}
}

TPage loadEAGLS(QString fnam) {
	TPage page;
	QFile file(fnam);
	if (!file.open(QFile::ReadOnly)) return page;
	QString line;
	QString name;
	QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
	name.clear();
	while (!file.atEnd()) {
		line = codec->toUnicode(file.readLine());
		line.remove("\r");
		line.remove("\n");
		line.remove("\t");
		line.remove("(E)");
		line.remove("(e)");
		parseEAGLine(line,&page,&name);
	}
	page.id = 1;
	return page;
}

// abelsoft scripts

TPage loadAbelsoft(QString fnam) {
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
	if (file.open(QFile::ReadOnly)) {
		page.id = 1;
		while (!file.atEnd()) {
			line = QString::fromUtf8(file.readLine());
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
							tlin.src.text = QString("[").append(taglist.at(idx)).append("]");
							page.text.append(tlin);
						}
					}
//					throw(0);
				} else {
					if (!name.isEmpty()) {
						if (line.startsWith(QDialog::trUtf8("「"))) line.remove(0,1);
						if (line.endsWith(QDialog::trUtf8("」"))) line.resize(line.length() - 1);
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

// enmon engine script

TPage loadEnmon(QString fnam) {
	TPage page;
	QFile file(fnam);
	QString line;
	QString arg1,name;
	TLine tlin,elin;
	tlin.type = TL_TEXT;
	if (file.open(QFile::ReadOnly)) {
		page.id = 1;
		while (!file.atEnd()) {
			line = QString::fromUtf8(file.readLine()).remove("\n").remove("\r");
			if (line.startsWith(QDialog::trUtf8("#"))) {
				if (line.startsWith(QDialog::trUtf8("#文章表示("))) {
					line.remove(QDialog::trUtf8("#文章表示("));
					arg1 = line.split(QDialog::trUtf8(",")).first();
					name.clear();
					if (!arg1.isEmpty() && (arg1 != "0")) name = arg1;
				} else if (line.startsWith(QDialog::trUtf8("#SceneName("))) {
					line.remove(QDialog::trUtf8("#SceneName("));
					line.remove(QDialog::trUtf8(")"));
					tlin.src.name.clear();
					tlin.src.text = "== ";
					tlin.src.text.append(line);
					page.text.append(elin);
					page.text.append(tlin);
					page.text.append(elin);
				} else if (line.startsWith(QDialog::trUtf8("#Jump("))) {
					line.remove(QDialog::trUtf8("#Jump("));
					line.remove(QDialog::trUtf8(")"));
					tlin.src.name.clear();
					tlin.src.text = QString("[JP ").append(line).append(" ]");
					page.text.append(elin);
					page.text.append(tlin);
				} else if (line.startsWith(QDialog::trUtf8("#ＣＧ表示("))) {
					line.remove(QDialog::trUtf8("#ＣＧ表示("));
					line.remove(QDialog::trUtf8(")"));
					tlin.src.name.clear();
					tlin.src.text = QString("[:bg:").append(line).append("]");
					page.text.append(elin);
					page.text.append(tlin);
				}
			} else if (line.size() > 0) {
				tlin.src.name.clear();
				if (line.startsWith(QDialog::trUtf8("「"))) tlin.src.name = name;
				tlin.src.text = line;
				page.text.append(tlin);
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
		page.id = 1;
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
			(line.flag & FL_HIDDEN) ||
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

TPage* findPage(int id) {
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
