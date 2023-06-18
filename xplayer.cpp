#include "mainwin.h"

xPlayer::xPlayer(QWidget* p):QLabel(p) {
	setWindowModality(Qt::ApplicationModal);
	fnt.setPixelSize(32);
	fnt.setFamily("Buxton Sketch");
	setFixedSize(1280,720);
	cnt = 0;
	mov = new QMovie;

	connect(mov, SIGNAL(frameChanged(int)),this,SLOT(frameChanged()));
}

void xPlayer::reset() {
	curimgpath.clear();
}

bool xPlayer::playLine(TLine _l) {
	int res = false;
	lin = _l;
	if (curimgpath != lin.imgpath) {
		curimgpath = lin.imgpath;
		if (!curimgpath.isEmpty() && QFile::exists(curimgpath)) {
			mov->stop();
			mov->setFileName(curimgpath);
			mov->start();
		} else {
			reset();
		}
	} else if (mov->state() != QMovie::Running) {
		mov->start();
	}
	qDebug() << curimgpath;
	frameChanged();
	return res;
}

void xPlayer::frameChanged() {
	QPainter pnt;
	// frame
	QPixmap pxm = mov->currentPixmap();
	if (pxm.isNull() || curimgpath.isEmpty()) {
		pxm = QPixmap(1280, 720);
		pxm.fill(Qt::black);
	} else {
		pxm = pxm.scaled(1280,720,Qt::KeepAspectRatio);
	}
	setFixedSize(pxm.size());
	// text overlay
	QString txt;
	int flag = Qt::TextWordWrap;
	if (lin.trn.text.isEmpty()) {
		txt = lin.src.text;
		flag |= Qt::TextWrapAnywhere;
	} else {
		txt = lin.trn.text;
	}
	pnt.begin(&pxm);
	pnt.setFont(fnt);
	pnt.setPen(Qt::white);
	QRect rct(0, height()-200, width(), 200);
	QLinearGradient grd(rct.topLeft(),rct.bottomLeft());
	grd.setColorAt(0, QColor(0,0,0,200));
	grd.setColorAt(1, QColor(0,0,0,120));
	pnt.fillRect(rct, grd);
	pnt.drawText(10,height()-190, width()-20, 180, flag, txt);
	if (!lin.src.name.isEmpty()) {
		pnt.fillRect(0,height()-240,300,38,QBrush(QColor(1,1,1,200)));
		if (lin.trn.name.isEmpty()) {
			pnt.drawText(10,height()-238,290,34,0,lin.src.name);
		} else {
			pnt.drawText(10,height()-238,290,34,0,lin.trn.name);
		}
	}
	pnt.end();
	// draw result
	setPixmap(pxm);
}

void xPlayer::mousePressEvent(QMouseEvent *ev) {
	if (ev->button() == Qt::LeftButton) {
		emit clicked();
	}
}

void xPlayer::wheelEvent(QWheelEvent* ev) {
	if (ev->angleDelta().y() < 0) {
		emit clicked();
	} else if (ev->angleDelta().y() > 0) {
		emit clicked_r();
	}
}

void xPlayer::keyPressEvent(QKeyEvent *ev) {
	switch(ev->key()) {
		case Qt::Key_Return: emit clicked(); break;
		case Qt::Key_Escape: mov->stop(); close(); break;
	}
}
