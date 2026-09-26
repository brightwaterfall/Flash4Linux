/***************************************************************************
              clistboxitem.h  -  description
                 -------------------
    begin                : Mon Jun 9 2003
    copyright            : (C) 2003 by özkan pakdil
    email                : ozkanpakdil@users.sourceforge.net
 ***************************************************************************/


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CLISTVIEWITEM_H
#define CLISTVIEWITEM_H

#include <qlistview.h>

/** used for showing pixmaps in items inside timeLine's listview.
  *@author özkan pakdil
  */
class CListViewItem:public /*QObject, */ QListViewItem
{
        //Q_OBJECT
public:
    CListViewItem (QListView * parent = 0, QString label1 = QString(),
               QString label2 = QString(), QString label3 =
                   QString(), QString label4 =
                   QString(), QString label5 =
                   QString(), QString label6 =
                   QString(), QString label7 =
                   QString(), QString label8 = QString());

        // CListViewItem( QListView * parent = 0,QString label1=NULL ):
        // QListViewItem(parent,label1){setHeight(15);}
        //CListBoxItem();
    ~CListViewItem ();

    void setup (){
        setExpandable (TRUE);
        setHeight (20);
                //qDebug("%d",height());
                //QListViewItem::setup();
    }
	
	int m_Row;

        //    int width( const QListBox* ) ;
        //    int height( const QListBox* ) ;
        //    void paint( QPainter * );

};
#endif	/*

*/
