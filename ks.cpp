#include <QDialog>
#include <exception>

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
	pos = line.indexOf(" ");
	if (pos < 0) {			// no params
		wrk = false;
		pos = line.size();
	}
	res.com = line.left(pos);
	line.remove(0,pos+1);
	while ((line.size() > 0) && wrk) {
		pos = line.indexOf("=");
		if (pos > 0) {
			par.name = line.left(pos).trimmed();
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
	return res;
}

QString getAttribute(ParLine par, QString name) {
	QString res;
	for (int i = 0; i < par.pars.size(); i++) {
		if (par.pars[i].name == name) res = par.pars[i].value;
	}
	return res;
}

TPage loadKS(QString fnam, int cpage) {
	TPage page;
	QFile file(fnam);
	if (!file.open(QFile::ReadOnly)) return page;
	page.id = QUuid::createUuid();
	TLine elin;
	elin.type = TL_TEXT;
	elin.flag = 0;
	TLine nlin = elin;
	TLine tlin = elin;
	int pos;
	QString line;
	QString comline;
	QString str;
	ParLine param;
	QTextCodec* codec;
	int name = 0;
	switch (cpage) {
		case CP_UCS2:
			file.read(2);		// skip BOM (FF FE)
			codec = QTextCodec::codecForName("UCS-2");
			break;
		case CP_UTF8:
			file.read(3);		// skip BOM
			codec = QTextCodec::codecForName("UTF8");
			break;
		default:
			codec = QTextCodec::codecForName("Shift-JIS");
			break;
	}
	while (!file.atEnd()) {
		line = codec->toUnicode(file.readLine());
		line.remove("\r");
		line.remove("\n");
		line.remove("\t");
		line.remove(QDialog::trUtf8("　"));
		tlin.src.text.clear();
		while(line.size() > 0) {
			if (line.startsWith(";")) {
				line.clear();
			} else if (line.startsWith("[")) {
				pos = line.indexOf("]");
				if (pos < 0) {
					line.clear();
				} else {
					comline = line.mid(1, pos-1);
					line = line.mid(pos+1);
					param = parseKS(comline);
					if (param.com == "name") {
						tlin.src.name = getAttribute(param,"text");
					} else if (param.com == QDialog::trUtf8("名前")) {
						tlin.src.name = getAttribute(param,"id");
					} else if ((param.com == "cg_i") || (param.com == "cg_a")) {
						page.text.append(elin);
						nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"storage"));
						page.text.append(nlin);
					} else if (param.com == "cg_x") {
						page.text.append(elin);
						nlin.src.text = QString("[BGX:%0]").arg(getAttribute(param,"storage"));
						page.text.append(nlin);
					} else if (param.com == QDialog::trUtf8("イベント")) {
						nlin.src.text = QString("[BGX:%0]").arg(getAttribute(param,"file"));
						page.text.append(elin);
						page.text.append(nlin);
					} else if (param.com == "cg_a_cyara_lr_on") {
						page.text.append(elin);
						nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"haikei"));
						page.text.append(nlin);
					} else if ((param.com == "haikei") || (param.com == "bg") || (param.com == "ev") || (param.com == "evcg")) {
						str =getAttribute(param, "file");
						if (str.isEmpty())
							str = getAttribute(param, "storage");
						if (!str.isEmpty()) {
							nlin.src.text = QString("[BG:%0]").arg(str);
							page.text.append(elin);
							page.text.append(nlin);
						}
					} else if (param.com == QDialog::trUtf8("背景")) {
						nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"file"));
						page.text.append(elin);
						page.text.append(nlin);
					} else if (param.com == "jump") {
						nlin.src.text = QString("[jump %0:%1]").arg(getAttribute(param,"storage")).arg(getAttribute(param,"target"));
						page.text.append(nlin);
					} else if (param.com == "link") {
						nlin.src.name = getAttribute(param,"target");
					} else if (param.com.startsWith("CH_NAME") || (param.com == "cn")) {
						tlin.src.name = getAttribute(param,"name");
						if (tlin.src.name == QObject::trUtf8("ト書き")) tlin.src.name.clear();
					} else if (param.com == "NAME_M") {
						tlin.src.name = getAttribute(param,"n");
					} else if ((param.com == "FAID_IN") || (param.com == "TR")) {
						page.text.append(elin);
						nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"bmp"));
						page.text.append(nlin);
					} else if ((param.com == "FAID_IN_CG") || (param.com == "ALL_OFF")) {
						page.text.append(elin);
						nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"back_cg"));
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
					} else if (param.com == "select") {
						if (param.pars.isEmpty()) {
							nlin.src.text = "[SELECT]";
							page.text.append(elin);
							page.text.append(nlin);
						} else {
							nlin.src.name = getAttribute(param, "target");
							nlin.src.text = getAttribute(param, "text");
							page.text.append(nlin);
							nlin.src.name.clear();
						}
					} else if (param.com == "msg") {
						nlin.src.name = getAttribute(param,"name");
					} else if (param.com == "lcg") {
						// page.text.append(elin);
						nlin.src.name.clear();
						nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"storage"));
						page.text.append(nlin);
					} else if (param.com == QDialog::trUtf8("「")) {
						line.prepend(QDialog::trUtf8("「"));
					} else if (param.com == QDialog::trUtf8("（")) {
						line.prepend(QDialog::trUtf8("（"));
					} else if (param.com.startsWith("m") && !param.com.startsWith("mw") && (param.pars.size() == 0)) {
						tlin.src.name = param.com.mid(1);
					} else if (param.com == "ns") {
						name = 1;
					} else if ((param.com == "r") || (param.com == "nse")) {
						if (param.com == "nse") name = 0;
						line.append(codec->toUnicode(file.readLine()));
						line.remove("\r");
						line.remove("\n");
						line.remove("\t");
						line.remove(QDialog::trUtf8("　"));
					//} else if (param.pars.isEmpty() && !line.isEmpty()) {
					//	if (param.com.contains(QDialog::trUtf8("/"))) {
					//		param.com = param.com.split(QDialog::trUtf8("/")).first();
					//	}
					//	tlin.src.name = param.com;
					} else if (param.com == "src_end") {
						page.text.append(elin);
						nlin.src.text = QString("[JUMP %0]").arg(getAttribute(param,"src"));
						page.text.append(nlin);
					} else if (param.com == "mruby") {
						tlin.src.text.append(getAttribute(param, "text"));
					}
				}
			} else if (line.startsWith("@")) {
				comline = line.mid(1);
				param = parseKS(comline);
				if ((param.com == "bg_FI") || (param.com == "E_FI")) {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if (param.com.startsWith("BG_")) {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if ((param.com == "bg_") || (param.com == "bg")) {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"f"));
					page.text.append(nlin);
				} else if  (param.com == "bgf") {
					page.text.append(elin);
					nlin.src.text = QString("[BG:%0]").arg(getAttribute(param,"bg"));
					page.text.append(nlin);
				} else if (param.com == "FLASH") {
					page.text.append(elin);
					nlin.src.text = QString("[FLASH BG:%0]").arg(getAttribute(param,"storage"));
					page.text.append(nlin);
				} else if (param.com == "name") {
					tlin.src.name = getAttribute(param,"chara");
					if (tlin.src.name == QDialog::trUtf8("地")) tlin.src.name.clear();
				} else if (param.com == "jump") {
					nlin.src.text = QString("[jump %0:%1]").arg(getAttribute(param,"storage")).arg(getAttribute(param,"target"));
					page.text.append(nlin);
				} else if (param.com == "Msg") {
					tlin.src.name = getAttribute(param, "name");
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
				nlin.src.text.clear();
				line.clear();
			} else if (line.startsWith("*")) {
				if (!line.endsWith("|")) {
					nlin.src.text = QString("== %0").arg(line.mid(1));
					page.text.append(elin);
					page.text.append(nlin);
					page.text.append(elin);
				}
				nlin.src.text.clear();
				line.clear();
			} else {
				pos = line.indexOf("[");
				if (pos < 0) {
					comline = line;
					line.clear();
				} else {
					comline = line.left(pos);
					line = line.mid(pos);
				}
				if (name) {
					tlin.src.name.append(comline);
				} else {
					tlin.src.text.append(comline);
				}
			}
		}
		if (!tlin.src.text.isEmpty()) {
			if (tlin.src.text.startsWith(QDialog::trUtf8("「")) && tlin.src.text.endsWith(QDialog::trUtf8("」"))) {
				tlin.src.text = tlin.src.text.mid(1, tlin.src.text.size() - 2);
			}
			tlin.src.name.remove("【");
			tlin.src.name.remove("】");
			page.text.append(tlin);
			tlin.src.name.clear();
		}
	}
	return page;
}
