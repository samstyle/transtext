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

bool xPlayer::playLine(TLine lin) {
	int res = false;
	QPixmap pxm;
	QString txt;
	QPainter pnt;
	int flag = 0;
	int w,h;
	if (lin.trn.text.isEmpty()) {
		txt = lin.src.text;
		flag = Qt::TextWordWrap | Qt::TextWrapAnywhere;
	} else {
		txt = lin.trn.text;
		flag = Qt::TextWordWrap;
	}
/*
	if (lin.type == TL_SELECT) {
		res = true;
		pxm = pixmap(Qt::ReturnByValue);
		flag = 0;
		w = width();
		h = height();
		int x = w * 3 / 10;
		int y = h / 10 + cnt * 40;
		int ws = w * 4 / 10;
		int hs = 38;
		pnt.begin(&pxm);
		pnt.setFont(fnt);
		pnt.setPen(Qt::white);
		QRect rct(x, y, ws, hs);
		pnt.fillRect(rct, QColor(0,0,0,200));
		pnt.drawText(x+3, y+3, ws-6, hs-6, 0, txt);
		pnt.end();
		cnt++;
	} else {
*/
//		cnt = 0;
		if (curimgpath != lin.imgpath) {
			curimgpath = lin.imgpath;
			if (!curimgpath.isEmpty() && QFile::exists(curimgpath)) {
				mov->stop();
				mov->setFileName(curimgpath);
				mov->start();
				pxm = mov->currentPixmap(); //.scaled(1280, 720, Qt::KeepAspectRatio);
			} else {
				pxm = QPixmap(size());
				pxm.fill(Qt::black);
			}
			w = pxm.width();
			h = pxm.height();
			setFixedSize(w, h);
			// mov->setScaledSize(size());
		} else {
			w = width();
			h = height();
			if (mov->state() != QMovie::Running)
				mov->start();
		}
		qDebug() << curimgpath;
		ovr = QPixmap(w, h);
		ovr.fill(Qt::transparent);
		pnt.begin(&ovr);
		pnt.setFont(fnt);
		pnt.setPen(Qt::white);
		QRect rct(0, h-200, w, 200);
		QLinearGradient grd(rct.topLeft(),rct.bottomLeft());
		grd.setColorAt(0, QColor(0,0,0,200));
		grd.setColorAt(1, QColor(0,0,0,120));
		pnt.fillRect(rct, grd);
		pnt.drawText(10,h-190, w-20, 180, flag, txt);
		if (!lin.src.name.isEmpty()) {
			pnt.fillRect(0,h-240,300,38,QBrush(QColor(1,1,1,200)));
			if (lin.trn.name.isEmpty()) {
				pnt.drawText(10,h-238,290,34,0,lin.src.name);
			} else {
				pnt.drawText(10,h-238,290,34,0,lin.trn.name);
			}
		}
		pnt.end();
		frameChanged();
//	}
	return res;
}

void xPlayer::frameChanged() {
	QPixmap pxm = mov->currentPixmap();
	if (pxm.isNull() || curimgpath.isEmpty()) {
		pxm = QPixmap(size());
		pxm.fill(Qt::black);
	}
	QPainter pnt;
	pnt.begin(&pxm);
	pnt.drawPixmap(0,0,ovr);
	pnt.end();
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
