
/***************************************************************************
                          xsldbgevent.h  -  event to notify app of 
                                                   data from xsldbg
                             -------------------
    begin                : Fri Feb 1 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef XSLDBGEVENT_H
#define XSLDBGEVENT_H

#include <qevent.h>
#include <libxsldbg/xsldbgmsg.h>
#include <qlist.h>

class XsldbgDebuggerBase;

/* how many column do we have */
#define XSLDBGEVENT_COLUMNS 3
class XsldbgEventData {

 public:
  XsldbgEventData();
  ~XsldbgEventData();

  void setText(int column, QString text);
  QString getText(int column);

  void setInt(int column, int value);
  int  getInt(int column);

 private:
  /** Below are the messages that this class will suport 
      Values are mapped left to right ie the first QString value mapps
      to textValues[0], the second mapps to textValues[1] 
      the third mapps s to textValues[2] etc.. */
  QString textValues[XSLDBGEVENT_COLUMNS];

  /**
     Both int and bool values are mapped to intValues in the same manner as
     stated above */
  int intValues[XSLDBGEVENT_COLUMNS];

  // /** line number and/or file name changed */
  //  void lineNoChanged(QString /* fileName */, int /* lineNumber */, bool /* breakpoint */);
  // These data items are mapped to attributes of this class with the same name


  //	/** Show a message in debugger window */
  //	void showMessage(QString /* msg*/);
  //   These data item is mapped to the text attribute


  //	/** Add breakpoint to view, First parameter is QString::null
  //			to indicate start of breakpoint list notfication */
  //	void breakpointItem(QString /* fileName*/, int /* lineNumber */, QString /*templateName*/,
  //                                        bool /* enabled */, int /* id */);
  // These data items are mapped to attributes of this class with the same name
												

  //	/** Add global variable to view, First parameter is QString::null
  //        to indicate start of global variable list notfication */
  //	void globalVariableItem(QString /* name */, QString /* fileName */, int /* lineNumber */);
  // These data items are mapped to attributes of this class with the same name


  //	/** Add local variable to view, First parameter is QString::null
  //			to indicate start of local variable list notfication */
  //	void localVariableItem(QString /*name */, QString /* templateContext*/,
  //								QString /* fileName */, int /*lineNumber */);	
  // These data items are mapped to attributes of this class with the same name


  //	/** Add template to view, First parameter is QString::null
  //			to indicate start of template list notfication */
  //	void templateItem(QString /* name*/, QString /*mode*/, QString /* fileName */, int /* lineNumber */);

  //	/** Add source to view, First parameter is QString::null
  //			to indicate start of source list notfication */
  //	void sourceItem(QString /* fileName */, QString /* parentFileName */, int /*lineNumber */);

  //	/** Add parameter to view, First parameter is QString::null
  //			to indicate start of parameter list notfication */
  //	void parameterItem(QString /* name*/, QString /* value */);

  //	/** Add callStack to view, First parameter is QString::null
  //			to indicate start of callstack list notfication */
  //	void callStackItem(QString /* tempalteName*/, QString /* fileName */, int /* lineNumber */);
	
  //	/** Add entity to view, First parameter is QString::null
  //			to indicate start of entity list notfication */
  //	void entityItem(QString /*SystemID*/,  QString /*PublicID*/);	
	
  //	/* Show the URI for SystemID or PublicID requested */
  //	void resolveItem(QString /*URI*/);
  
  //    /* Display a integer option value First parameter is QString::null
  //			to indicate start of option list notification */
  //   void intOptionItem(QString /* name*/, int /* value */);

  //    /* Display a string option value. First parameter is QString::null
  //			to indicate start of option list notification */
  //   void stringOptionItem(QString /* name*/, QString /* value */);

};


class XsldbgEvent : public QEvent {

 public:  
  XsldbgEvent(XsldbgMessageEnum type, void *data);
  ~XsldbgEvent();

  /** main control for emitting messages */
  void emitMessage(XsldbgDebuggerBase *debugger);

  /** emit a single message */
  void emitMessage(XsldbgEventData *eventData);

 private:
  XsldbgEventData * createEventData(XsldbgMessageEnum type, void *msgData);
  void handleLineNoChanged(XsldbgEventData *eventData, void *msgData);
  void handleShowMessage(XsldbgEventData *eventData, void *msgData);
  void handleBreakpointItem(XsldbgEventData *eventData, void *msgData);
  void handleGlobalVariableItem(XsldbgEventData *eventData, void *msgData);
  void handleLocalVariableItem(XsldbgEventData *eventData, void *msgData);
  void handleTemplateItem(XsldbgEventData *eventData, void *msgData);
  void handleSourceItem(XsldbgEventData *eventData, void *msgData);
  void handleIncludedSourceItem(XsldbgEventData *eventData, void *msgData);
  void handleParameterItem(XsldbgEventData *eventData, void *msgData);
  void handleCallStackItem(XsldbgEventData *eventData, void *msgData);
  void handleEntityItem(XsldbgEventData *eventData, void *msgData);
  void handleResolveItem(XsldbgEventData *eventData, void *msgData);
  void handleIntOptionItem(XsldbgEventData *eventData, void *msgData);
  void handleStringOptionItem(XsldbgEventData *eventData, void *msgData);


 private:

  XsldbgMessageEnum type;

  /* this is a volitile value that is only valid for the duration 
    of the constuctor. It will be set to 0L imedediately after*/
  void *data; 

  /* This is a volitile value only valid for duration of emitMessage 
     frunction. It will be set to 0L imedediately after */
  XsldbgDebuggerBase *debugger;

  /** this is the data associated with this event 
      each data item in the list will be of the type required
      by the "type" this event
   */
  QList<XsldbgEventData> list;
    };

#endif
