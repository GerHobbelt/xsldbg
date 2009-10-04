
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU General Public License as 
 *  published by the Free Software Foundation; either version 2 of 
 *  the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/


#include "xsldbgdebuggerbase.h"

#include <QApplication>
#include <QStringList>
#include <QUrl>

XsldbgDebuggerBase::XsldbgDebuggerBase()
  : QObject(0L)
{
  initialized = false;
  updateTimerID = -1;
}


XsldbgDebuggerBase::~XsldbgDebuggerBase()
{
  /* empty*/
}



QString XsldbgDebuggerBase::fromUTF8(const char *text)
{
  QString result;
  if (text != 0L)
    result = (const char*)text;
  return result;
}


QString XsldbgDebuggerBase::fromUTF8(const xmlChar *text)
{
  QString result;
  if (text != 0L)
    result = QString::fromUtf8((const char*)text);
  return result;
}


QString XsldbgDebuggerBase::fromUTF8FileName(const char *text)
{
  QString result;
  if (text != 0L){
    QUrl url(((const char*)text));
    result = url.toString();
  }
  return result;
}


QString XsldbgDebuggerBase::fromUTF8FileName(const xmlChar *text)
{
  QString result;
  if (text != 0L){
    QUrl url(QString::fromUtf8((const char*)text));
    result = url.toString();
  }
  return result;
}


void XsldbgDebuggerBase::queueMessage(const QString &text)
{
    updateText += text;
}
