#include <QDialog>

#include "base.h"

// enmon engine script

TPage loadEnmon(QString fnam) {
	TPage page;
	QFile file(fnam);
	QString line;
	QString arg1,name;
	TLine tlin,elin;
	tlin.flag = 0;
	elin.flag = 0;
	tlin.type = TL_TEXT;
	QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
	if (file.open(QFile::ReadOnly)) {
		page.id = 1;
		while (!file.atEnd()) {
			line = codec->toUnicode(file.readLine());
			line.remove("\n");
			line.remove("\r");
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
				} else if (line.startsWith(QDialog::trUtf8("#背景表示"))) {
					line.remove(QDialog::trUtf8("#背景表示"));
					line.remove(QDialog::trUtf8(")"));
					tlin.src.name.clear();
					tlin.src.text = QString("[BG: ").append(line).append("]");
					page.text.append(elin);
					page.text.append(tlin);
				} else if (line.startsWith(QDialog::trUtf8("#ＣＧ表示("))) {
					line.remove(QDialog::trUtf8("#ＣＧ表示("));
					line.remove(QDialog::trUtf8(")"));
					tlin.src.name.clear();
					tlin.src.text = QString("[BG: ").append(line).append("]");
					page.text.append(elin);
					page.text.append(tlin);
				}
			} else if (line.size() > 0) {
				tlin.src.name.clear();
				if (line.startsWith(QDialog::trUtf8("「"))) {
					tlin.src.name = name;
					line = line.mid(1, line.size() - 2);
				}
				tlin.src.text = line;
				page.text.append(tlin);
			}
		}
	}
	return page;
}
