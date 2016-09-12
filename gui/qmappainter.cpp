#include "qmappainter.h"
#include <qevent.h>

using namespace GMapping;

QMapPainter::QMapPainter(QWidget * parent, const char * name, Qt::WindowFlags f) :
    QWidget(parent, f)
{
  m_pixmap = new QPixmap(size());
  m_pixmap->fill(Qt::white);
}

void QMapPainter::resizeEvent(QResizeEvent * sizeev)
{
  m_pixmap->scaled(sizeev->size());
}

QMapPainter::~QMapPainter()
{
  delete m_pixmap;
}

void QMapPainter::timerEvent(QTimerEvent * te)
{
  if (te->timerId() == timer)
    update();
}

void QMapPainter::start(int period)
{
  timer = startTimer(period);
}

void QMapPainter::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, *m_pixmap, 0, 0, m_pixmap->width(), m_pixmap->height());
}
