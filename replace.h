#ifndef REPLACE_H
#define REPLACE_H

#include <QDialog>
#include "ui_replacedialog.h"

class Replacer : public QDialog {
	Q_OBJECT
	public:
		Replacer(QWidget* = NULL);
	signals:
		void confirm(QString, QString);
	private:
		Ui::Replacer ui;
	private slots:
		void onOk();
};

#endif // REPLACE_H
