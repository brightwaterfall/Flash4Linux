/***************************************************************************
              colorpopup.cpp  -  description
                 -------------------
    begin                :  2003
    copyright            : (C) 2003 by ùzkan pakdil
    email                : ozkanpakdil@users.sourceforge.net
 ***************************************************************************/

#include "colorpopup.h"
#include "tools.h"
#include "ccolorswatches.h"
#include "f4lm.h"
#include <qcolordialog.h>
#include <qimage.h>
#include "cursor/eye_dropper_tool.xpm"

CColorPopup::CColorPopup (QWidget * parent, const char *name, WFlags f)
: QWidget(parent)
{
    if (name)
        setObjectName (name);
    setWindowFlags (f);
    colorShower = q3Label(this, "color shower");
    colorShower->setFrameStyle (QFrame::WinPanel | QFrame::Sunken);
    colorShower->setLineWidth (2);
    colorShower->setMidLineWidth (1);
    colorShower->resize (37, 20);
    q3SetPaletteBackground (colorShower, QColor (0, 0, 0));
    colorShower->move (5, 2);
    colorName = new QTextEdit (this);
    colorName->resize (55, 20);
    colorName->move (50, 2);
    colorName->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    colorName->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    
	QFont f1 ("Times", 8, QFont::Light);
    colorName->setFont (f1);
    colorName->clearFocus ();
    s = new CColorSwatches (this, "", ((CTools *) parent)->dad);
    s->resize (210, 125);
    s->move (2, 29);
    
	QToolButton * colordialog =new CToolButton (this, tr ("Open color dialog"));
    colordialog->move (180, 3);
    colordialog->resize (20, 20);
    colordialog->setIcon (QIconSet (QPixmap ("colordialogBut.png")));
    colordialog->setToolTip (tr ("Open Color Dialog"));
    connect (colordialog, SIGNAL (clicked ()), this,SLOT (slotColordialog ()));

        //setFocusPolicy (Qt::StrongFocus);
    setMouseTracking (true);
    mcursor = QPixmap ((const char **) eye_dropper_tool_xpm);
    setCursor (QCursor (mcursor, 1, 15));
}

CColorPopup::~CColorPopup ()
{
}

void CColorPopup::slotColordialog ()
{
    QColor c = QColorDialog::getColor ();
    ((CTools *) parent ())->dad->setDefObjCOLOR (c);
}

void CColorPopup::mouseMoveEvent (QMouseEvent * e)
{
    QPoint p = e->globalPos ();
    QPixmap pm = q3GrabWindow (0, p.x (), p.y (), 1, 1);
    QImage i = pm.toImage ();
    QRgb px = i.pixel (0, 0);
    QColor color (qRed (px), qGreen (px), qBlue (px));
    q3SetPaletteBackground (colorShower, color);
    QString tmp;
    tmp = QString::asprintf ("#%02x%02x%02x", qRed (px), qGreen (px), qBlue (px));
    colorName->setText (tmp.toUpper ());
}

void CColorPopup::mousePressEvent (QMouseEvent * e)
{
    ((CTools *) parent ())->dad->setDefObjCOLOR (q3BackgroundColor (colorShower));
    hide ();
}

void CColorPopup::showEvent (QShowEvent *)
{
    grabMouse (QCursor (mcursor, 1, 15));
}

void CColorPopup::hideEvent (QHideEvent *)
{
    releaseMouse ();
}
