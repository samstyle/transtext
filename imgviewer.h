#pragma once

#include <QAbstractListModel>
#include <QDialog>
#include <QUuid>

#include "ui_imgviewer.h"

enum {
	IMV_VIEW,
	IMV_SELECT
};

class imgViewModel : public QAbstractListModel {
	public:
		imgViewModel();
	private:
		int rowCount(const QModelIndex& = QModelIndex()) const;
		QVariant data(const QModelIndex&, int = Qt::DisplayRole) const;
};

class ImgViewer : public QDialog {
	Q_OBJECT
	public:
		ImgViewer(QWidget* = nullptr);
		void show(int = IMV_VIEW);
	signals:
		void picClicked(QUuid);
	private:
		int mode;
		imgViewModel* ivmod;
		Ui::ImgViewer ui;
	private slots:
		void selChanged(const QItemSelection&, const QItemSelection&);
		void picDoubleClicked(QModelIndex);
		void openImg();
		void delImg();
};
