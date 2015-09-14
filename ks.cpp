#include <QDialog>

#include "base.h"

struct Param {
	QString name;
	QString value;
};

struct ParLine {
	QString com;
	QList<Param> pars;
};

ParLine parseKS(QString& line) {
	bool wrk = true;
	bool subw = true;
	bool quote = false;
	int pos;
	ParLine res;
	Param par;
	line.remove(0,1);	// [ @
	pos = line.indexOf(" ");
	if (pos < 0) {			// no params
		pos = line.indexOf("]");
		wrk = false;
		if (pos < 0) {
			pos = line.size();
		}
	}
	res.com = line.left(pos);
	line.remove(0,pos+1);
	while ((line.size() > 0) && wrk) {
		if (line.startsWith("]")) {
			line.remove(0,1);
			wrk = false;
		} else {
			pos = line.indexOf("=");
			if (pos > 0) {
				par.name = line.left(pos);
				line.remove(0,pos+1);
				while (line.startsWith(" ")) line.remove(0,1);
				par.value.clear();
				subw = true;
				while (subw && (line.size() > 0)) {
					if (line.startsWith("\"")) {
						quote = !quote;
					} else {
						if (quote) {
							par.value.append(line.at(0));
						} else if (line.startsWith(" ")) {
							subw = false;
						} else if (line.startsWith("]")) {
							subw = false;
							wrk = false;
						} else {
							par.value.append(line.at(0));
						}
					}
					line.remove(0,1);
				}
				res.pars.append(par);
			} else {
				wrk = false;
			}
		}
	}
	return res;
}

QString getAttribute(ParLine par, QString name) {
	QString res;
	for (int i = 0; i < par.pars.size(); i++) {
		if (par.pars[i].name == name) res = par.pars[i].value;
	}
	return res;
}

TPage loadKS(QString fnam) {
	TPage page;
	page.id = 0;
	QFile file(fnam);
	if (!file.open(QFile::ReadOnly)) return page;
	page.id = getid();
	TLine nlin;
	TLine elin;
	nlin.flag = 0;
	nlin.type = TL_TEXT;
	elin.type = TL_TEXT;
	elin.flag = 0;
	int pos;
	QString line;
	QString val;
	ParLine param;
	QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
	bool nxtline = false;
	while (!file.atEnd()) {
		line.clear();
		do {
			line.append(codec->toUnicode(file.readLine()));
			line.remove("\r");
			line.remove("\n");
			line.remove("\t");
			line.remove(QDialog::trUtf8("　"));
			if (line.endsWith("\\")) line = line.left(line.size()-1);
			nxtline = line.endsWith("[r]");
			line.remove("[r]");
			line.remove("[en]");
		} while (nxtline && !file.atEnd());
		while (line.size() > 0) {
			if (line.startsWith(";")) {	// ; comment
				line.clear();
			} else if (line.startsWith("*")) {	// *label except of *(something)|
				val = line.mid(1);
				if (!val.contains("|") && (val.size() > 1)) {
					nlin.src.text = QString("== %0").arg(val);
					page.text.append(elin);
					page.text.append(nlin);
					page.text.append(elin);
				}
				line.clear();
			} else if (line.startsWith("[")) {	//  && !line.endsWith(QDialog::trUtf8("」")) && !line.endsWith(QDialog::trUtf8("）"))
				param = parseKS(line);
				if (param.com == "name") {
					nlin.src.name = getAttribute(param,"text");
				} else if ((param.com == "cg_i") || (param.com == "cg_a")) {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if (param.com == "cg_x") {
					page.text.append(elin);
					nlin.src.text = QString("[BGX:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if (param.com == "cg_a_cyara_lr_on") {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"haikei"));
					page.text.append(nlin);
				} else if ((param.com == "haikei") || (param.com == "bg")) {
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"file"));
					page.text.append(elin);
					page.text.append(nlin);
				} else if (param.com == "jump") {
					nlin.src.text = QString("[jump %0:%1]").arg(getAttribute(param,"storage")).arg(getAttribute(param,"target"));
					page.text.append(nlin);
				} else if (param.com == "link") {
					nlin.src.name = getAttribute(param,"target");
				} else if (param.com == "NAME_M") {
					nlin.src.name = getAttribute(param,"n");
				} else if ((param.com == "FAID_IN") || (param.com == "TR")) {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"bmp"));
					page.text.append(nlin);
				} else if (param.com == "FLASH") {
					page.text.append(elin);
					nlin.src.text = QString("[FLASH:%0]").arg(getAttribute(param,"in_bmp"));
					page.text.append(nlin);
				} else if (param.com == "SELECT") {
					page.text.append(elin);
					nlin.src.text = QString("[select]");
					page.text.append(nlin);
					nlin.src.name = QString("%0 : %1").arg(getAttribute(param,"file1")).arg(getAttribute(param,"tag1"));
					nlin.src.text = getAttribute(param,"sel1");
					if (!nlin.src.name.isEmpty()) page.text.append(nlin);
					nlin.src.name = QString("%0 : %1").arg(getAttribute(param,"file2")).arg(getAttribute(param,"tag2"));
					nlin.src.text = getAttribute(param,"sel2");
					if (!nlin.src.name.isEmpty()) page.text.append(nlin);
					nlin.src.name = QString("%0 : %1").arg(getAttribute(param,"file3")).arg(getAttribute(param,"tag3"));
					nlin.src.text = getAttribute(param,"sel3");
					if (!nlin.src.name.isEmpty()) page.text.append(nlin);
					nlin.src.name.clear();
				} else if (param.com == "msg") {
					nlin.src.name = getAttribute(param,"name");
				} else if (param.com == "lcg") {
					// page.text.append(elin);
					nlin.src.name.clear();
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if (param.com.startsWith("m") && !param.com.startsWith("mw") && (param.pars.size() == 0)) {
					nlin.src.name = param.com.mid(1);
				}
			} else if (line.startsWith("@")) {
				param = parseKS(line);
				if ((param.com == "bg_FI") || (param.com == "E_FI")) {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if (param.com == "bg_") {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"f"));
					page.text.append(nlin);
				} else if (param.com == "FLASH") {
					page.text.append(elin);
					nlin.src.text = QString("[FLASH BG:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if (param.com == "name") {
					nlin.src.name = getAttribute(param,"chara");
					if (nlin.src.name == QDialog::trUtf8("地")) nlin.src.name.clear();
				} else if (param.com == "jump") {
					nlin.src.text = QString("[jump %0:%1]").arg(getAttribute(param,"storage")).arg(getAttribute(param,"target"));
					page.text.append(nlin);
				} else if (param.com == "select") {
					page.text.append(elin);
					nlin.src.text = QString("[select]");
					page.text.append(nlin);
					nlin.src.name = getAttribute(param,"target1");
					nlin.src.text = getAttribute(param,"text1");
					if (!nlin.src.name.isEmpty()) page.text.append(nlin);
					nlin.src.name = getAttribute(param,"target2");
					nlin.src.text = getAttribute(param,"text2");
					if (!nlin.src.name.isEmpty()) page.text.append(nlin);
					nlin.src.name = getAttribute(param,"target3");
					nlin.src.text = getAttribute(param,"text3");
					if (!nlin.src.name.isEmpty()) page.text.append(nlin);
					nlin.src.name = getAttribute(param,"target4");
					nlin.src.text = getAttribute(param,"text4");
					if (!nlin.src.name.isEmpty()) page.text.append(nlin);
					nlin.src.name.clear();
				}
			} else {
				if (line.startsWith("[")) {
					pos = line.indexOf("]");
					if (pos > 0) {
						nlin.src.name = line.mid(1,pos-1);
						line.remove(0,pos+1);
						pos = nlin.src.name.indexOf("/");
						if (pos > 0) nlin.src.name = nlin.src.name.left(pos);
					}
				}
				pos = line.indexOf("[");
				if (pos > 0) {
					nlin.src.text = line.left(pos);
					line.remove(0,pos);
				} else {
					nlin.src.text = line;
					line.clear();
				}
				nlin.src.name.remove(QDialog::trUtf8("　"));
				nlin.src.text.remove(QDialog::trUtf8("　"));
				if (nlin.src.text.startsWith(QDialog::trUtf8("「")) && nlin.src.text.endsWith(QDialog::trUtf8("」"))) {
					nlin.src.text.remove(0,1);
					nlin.src.text.remove(nlin.src.text.size() - 1, 1);
				}

				page.text.append(nlin);
				nlin.src.name.clear();
			}
		}
	}
	return page;
}
