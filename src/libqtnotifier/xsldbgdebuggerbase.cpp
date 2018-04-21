
/***************************************************************************
    begin                : Sat Dec 22 2001
    copyright            : (C) 2001-2018 by Keith Isdale
    email                : keithisdale@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


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
