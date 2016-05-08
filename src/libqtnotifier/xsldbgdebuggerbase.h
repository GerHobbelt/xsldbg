
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keithisdale@gmail.com>
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


#ifndef XSLDBGDEBUGGERBASE_H
#define XSLDBGDEBUGGERBASE_H

#if defined WIN32
#  include <libxsldbg/xsldbgwin32config.h>
#else
#  ifndef  XSLDBG_SO_API
#     define XSLDBG_SO_API
#  endif
#endif

#include <QObject>
#include <QStringList>
#include <libxml/tree.h>

class XsldbgEvent;
class QApplication;
class QStringList;

/** 
    We delegate the task of emitting our signals to XsldbgEvent , so 
    Xsldbg must become our friend
*/
class XsldbgDebuggerBase : public QObject
{
    Q_OBJECT

    friend class XsldbgEvent;

 public:
  XsldbgDebuggerBase(void);
  ~XsldbgDebuggerBase(void);

  void setInitialized(bool state) {initialized = state; };
  bool getInitialized(void) {return initialized ;};
void setUpdateTimerID(int ID) { updateTimerID = ID;}; int getUpdateTimerID(void) {return updateTimerID ;};


  /** Convert from libxslt UTF8 to a QString */	
  static QString fromUTF8(const char *text);
  static QString fromUTF8FileName(const char *text);
	
  /** Convert from libxslt UTF8 to a QString */	
  static QString fromUTF8(const xmlChar *text);
  static QString fromUTF8FileName(const xmlChar *text);

  /* list of command yet to be processed */
  QStringList commandQueue(void) {return _commandQueue ;};
  
  void queueMessage(const QString &text);

protected:
    QString updateText;

 private:
  bool initialized;
  int updateTimerID;
  QStringList _commandQueue;	

  signals: // Signals
   /** line number and/or file name changed */
   void lineNoChanged(QString /* fileName */, int /* lineNumber */, bool /* breakpoint */);

   /** Show a message in debugger window */
   void showMessage(QString /* msg*/);

   /** Add breakpoint to view, First parameter is QString()
       to indicate start of breakpoint list notfication */
   void breakpointItem(QString /* fileName*/, int /* lineNumber */, 
		       QString /*templateName*/, QString /* modeName */,
 		      bool /* enabled */, int /* id */);

  /** Add global variable to view, First parameter is QString()
      to indicate start of global variable list notfication */
  void globalVariableItem(QString /* name */, QString /* fileName */, int /* lineNumber */);

  /** Add local variable to view, First parameter is QString()
      to indicate start of local variable list notfication */
  void localVariableItem(QString /*name */, QString /* templateContext*/,
			 QString /* fileName */, int /*lineNumber */);	
  
    /** Add a variable to view, First parameter is QString()
      to indicate start of local variable list notfication */
  void variableItem(QString /*name */, QString /* templateContext*/,
			 QString /* fileName */, int /*lineNumber */, 
			 QString /* select XPath */, bool /* is it a local variable */);	

  /** Add template to view, First parameter is QString()
      to indicate start of template list notfication */
  void templateItem(QString /* name*/, QString /*mode*/, QString /* fileName */, int /* lineNumber */);

  /** Add source to view, First parameter is QString()
      to indicate start of source list notfication */
  void sourceItem(QString /* fileName */, QString /* parentFileName */, int /*lineNumber */);

  /** Add parameter to view, First parameter is QString()
      to indicate start of parameter list notfication */
  void parameterItem(QString /* name*/, QString /* value */);

  /** Add callStack to view, First parameter is QString()
      to indicate start of callstack list notfication */
  void callStackItem(QString /* tempalteName*/, QString /* fileName */, int /* lineNumber */);
	
  /** Add entity to view, First parameter is QString()
      to indicate start of entity list notfication */
  void entityItem(QString /*SystemID*/,  QString /*PublicID*/);	
	
  /* Show the URI for SystemID or PublicID requested */
  void resolveItem(QString /*URI*/);
  
  /* Display a integer option value First parameter is QString()
     to indicate start of option list notification */
  void intOptionItem(QString /* name*/, int /* value */);

  /* Display a string option value. First parameter is QString()
     to indicate start of option list notification */
  void stringOptionItem(QString /* name*/, QString /* value */);

  /* Cause the names for source, data and output files to be reload from xsldbg */
  void fileDetailsChanged();

  void debuggerReady();
  void debuggerStopped();

};

#endif
