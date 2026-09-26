#include "ctexteditfortexttool.h"
#include "canview.h"
#include "ccanvastext.h"

#include <qpopupmenu.h>
#include <qfontdialog.h>
#include <qevent.h>

CTextEditForTextTool::CTextEditForTextTool (QWidget * parent,
        const char *name):
        QTextEdit (parent)
{
    if (name)
        setObjectName (name);
    setFrameStyle (QFrame::Box | QFrame::Raised);
    setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setLineWrapMode (QTextEdit::NoWrap);
    resize (20, 30);
}

void CTextEditForTextTool::setColor (const QColor & color)
{
    setTextColor (color);
}

void CTextEditForTextTool::setSelection (int, int indexFrom, int, int indexTo)
{
    QTextCursor cursor = textCursor ();
    cursor.setPosition (indexFrom);
    cursor.setPosition (indexTo, QTextCursor::KeepAnchor);
    setTextCursor (cursor);
}

void CTextEditForTextTool::keyPressEvent (QKeyEvent * e)
{
    QTextEdit::keyPressEvent (e);
    QSizeF docSize = document ()->size ();
    if (docSize.height () > viewport ()->height ()
            || docSize.width () > viewport ()->width ())
        resize (docSize.width () + 10, docSize.height () + 10);
    if (e->key () == Qt::Key_Escape) {
        ((canview *) parent ())->text->setFont (currentFont ());
        ((canview *) parent ())->text->setText (toPlainText ());
        ((canview *) parent ())->text->show ();
        ((canview *) parent ())->setFocus ();
        close ();
    }
}

void CTextEditForTextTool::paintEvent (QPaintEvent * event)
{
    QTextEdit::paintEvent (event);
}

void CTextEditForTextTool::contextMenuEvent (QContextMenuEvent * event)
{
    QMenu * textRightClickMenu = createStandardContextMenu ();
    textRightClickMenu->addSeparator ();
    textRightClickMenu->addAction ("Select font", this, SLOT (setUserFont ()));
    textRightClickMenu->exec (event->globalPos ());
    delete textRightClickMenu;
}


/** No descriptions */
void CTextEditForTextTool::setUserFont ()
{
        //  if(selectedText()!=""){
    bool ok;
    QFont f = QFontDialog::getFont (&ok);
    setCurrentFont (f);
        //  }
}
