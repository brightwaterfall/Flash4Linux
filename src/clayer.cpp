/***************************************************************************
              clayer.cpp  -  description
                 -------------------
    begin                : Mon Jun 30 2003
    copyright            : (C) 2003 by ùzkan pakdil
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

#include "clayer.h"
#include "ccanvas.h"
#include "canvasItem.h"

CLayer::CLayer(QWidget *parent, const QString &name) : QObject (parent)
{
    if (!name.isEmpty())
        setObjectName(name);
    pCanvasItemList = new QPtrList < CCanvasItem >;
    pCanvasList = new QPtrList < CCanvas >;
}

CLayer::~CLayer ()
{}
