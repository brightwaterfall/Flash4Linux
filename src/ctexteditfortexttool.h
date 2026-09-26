#ifndef CTEXTEDITFORTEXTTOOL_H
#define CTEXTEDITFORTEXTTOOL_H

#include <qtextedit.h>

/// when we press inside canview there is a textbox
/// for entering words. it comes from this class.
class CTextEditForTextTool:public QTextEdit
{
Q_OBJECT public:
    CTextEditForTextTool (QWidget * parent = 0, const char *name = 0);
    void setColor (const QColor & color);
    void setSelection (int paraFrom, int indexFrom, int paraTo, int indexTo);
    void keyPressEvent (QKeyEvent * e);
    void paintEvent (QPaintEvent * event);
    void contextMenuEvent (QContextMenuEvent * event);

protected:

public slots:		// Public slots
    /** This function is used for right click menu's last element. Select Font ->slot. */
    void setUserFont ();
};

#endif
