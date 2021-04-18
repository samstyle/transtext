#include "imgviewer.h"
#include "base.h"

#include <QFileDialog>
#include <QMessageBox>

extern TPage* curPage;
extern int changed;

imgViewModel::imgViewModel():QAbstractListModel() {

}

int imgViewModel::rowCount(const QModelIndex&) const {
	int res = 0;
	if (curPage != nullptr) {
		res = curPage->imgs.size();
	}
	return res;
}

QVariant imgViewModel::data(const QModelIndex& idx, int role) const {
	QVariant res;
	int row = idx.row();
	TImage img;
	if (curPage == nullptr) return res;
	if ((row < 0) || (row >= rowCount())) return res;
	img = curPage->imgs.at(row);
	switch (role) {
		case Qt::DisplayRole:
			res = img.name;
			break;
		case Qt::DecorationRole:
			res = img.img.scaled(64, 64, Qt::KeepAspectRatio);
			break;
	}
	return res;
}

// window

ImgViewer::ImgViewer(QWidget* p):QDialog(p) {
	ui.setupUi(this);
	ivmod = new imgViewModel;
	ui.table->setModel(ivmod);

	connect(ui.tbOpen,SIGNAL(released()),this,SLOT(openImg()));
	connect(ui.tbDelete,SIGNAL(released()),this,SLOT(delImg()));
	connect(ui.table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),this,SLOT(selChanged(const QItemSelection&, const QItemSelection&)));
}

void ImgViewer::openImg() {
	QString path = QFileDialog::getOpenFileName(this, "Open image", "", "Images (*.png *.jpg *.bmp)", nullptr, QFileDialog::DontUseNativeDialog);
	if (path.isEmpty()) return;
	TImage img;
	if (img.img.load(path)) {
		if ((img.img.width() > 256) || (img.img.height() > 256))
			img.img = img.img.scaled(256,256,Qt::KeepAspectRatio);
		img.name = QFileInfo(path).fileName();
		curPage->imgs.append(img);
		ui.table->reset();
		changed = 1;
	}
}

void ImgViewer::selChanged(const QItemSelection&, const QItemSelection&) {
	QModelIndexList idxlist = ui.table->selectionModel()->selectedIndexes();
	QPixmap pix;
	if ((idxlist.size() == 0) || !curPage) {
		pix = QPixmap(256, 256);
		pix.fill(Qt::transparent);
	} else {
		int row = idxlist.first().row();
		TImage img = curPage->imgs.at(row);
		pix = QPixmap::fromImage(img.img).scaled(256, 256, Qt::KeepAspectRatio);
	}
	ui.preview->setPixmap(pix);
}

void ImgViewer::delImg() {
	QModelIndexList idxlist = ui.table->selectionModel()->selectedRows();
	if (idxlist.isEmpty()) return;
	QModelIndex idx = idxlist.first();
	int row = idx.row();
	int res = QMessageBox::question(this, "Question", "Delete this image?");
	if (res == QMessageBox::Yes) {
		curPage->imgs.removeAt(row);
		ui.table->reset();
		selChanged(QItemSelection(), QItemSelection());
		changed = 1;
	}
}
