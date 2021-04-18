#pragma once

#include <QAbstractListModel>
#include <QDialog>

#include "ui_imgviewer.h"

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
	private:
		imgViewModel* ivmod;
		Ui::ImgViewer ui;
	private slots:
		void selChanged(const QItemSelection&, const QItemSelection&);
		void openImg();
		void delImg();
};
