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

bool xPlayer::playLine(TPage* pg, int ln) { // TLine _l) {
	int res = false;
	// lin = _l;
	lin = pg->text.at(ln);
	QPainter pnt;
	// text overlay
	QString txt;
	int flag = Qt::TextWordWrap;
	if (lin.trn.text.isEmpty()) {
		txt = lin.src.text;
		flag |= Qt::TextWrapAnywhere;
	} else {
		txt = lin.trn.text;
	}
	ovr = QPixmap(size());
	ovr.fill(Qt::transparent);
	pnt.begin(&ovr);
	pnt.setFont(fnt);
	pnt.setPen(Qt::white);
	if (lin.flag & TF_SELECT) {
		int _ln = ln;
		QStringList variants;
		TLine slin;
		do {
			_ln++;
			slin = pg->text.at(_ln);
			if (slin.flag & TF_SELITEM) {
				if (slin.trn.text.isEmpty()) {
					variants.append(slin.src.text);
				} else {
					variants.append(slin.trn.text);
				}
			}
		} while (slin.flag & TF_SELITEM);
		int cnt = variants.size();
		int ys = (height() >> 1) - (cnt * 50);
		int xs = width() >> 2;
		int w = width() >> 1;
		int h = 45;
		while (cnt > 0) {
			pnt.fillRect(xs, ys, w, h, QColor(0, 0, 0, 200));
			pnt.drawText(xs+2, ys+2, w-4, h-4, Qt::AlignHCenter | Qt::AlignVCenter, variants.takeFirst());
			ys += (h + 5);
			cnt--;
		}
	} else {
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
	}
	pnt.end();

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

	frameChanged();
	return res;
}

void xPlayer::frameChanged() {
	QPainter pnt;
	// frame
	QPixmap pxm = mov->currentPixmap();
	if (pxm.isNull() || curimgpath.isEmpty()) {
		pxm = QPixmap(size());
		pxm.fill(Qt::black);
	} else {
		pxm = pxm.scaled(1280,720,Qt::KeepAspectRatio);
	}
	setFixedSize(pxm.size());
	// draw overlay
	pnt.begin(&pxm);
	pnt.drawPixmap(0,0,ovr);
	pnt.end();
	setPixmap(pxm);
}

void xPlayer::mouseReleaseEvent(QMouseEvent *ev) {
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
