
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

/* how many columns do we have */
#define XSLDBGEVENT_COLUMNS 3

/**
 * This class is used to convert a message from xsldbg into a simple data type
 *
 * @short convertor of xsldbg message to a data class
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
class XsldbgEventData {

 public:
  XsldbgEventData();
  ~XsldbgEventData();


  /**
   * Set the text for the column specified
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS 
   * @param text The text value to store in column indicated
   */
  void setText(int column, QString text);


  /**
   * Get the text from the column specified
   * 
   * @returns QString::null if invalid column number
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS   
   *  
   */
  QString getText(int column);


  /**
   * Set the integer value for the column specified
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS 
   * @param value The value to store in column indicated
   */
  void setInt(int column, int value);


  /**
   * Get the integer value from the column specified
   *
   * @returns -1 if invalid column number
   *
   * @param column 0 =< @p column < XSLDBGEVENT_COLUMNS 
   *
   */
  int  getInt(int column);

 private:
  /** Below are the messages that this class will support 
      Values are mapped left to right ie the first QString value maps
      to textValues[0], the second mapps to textValues[1] 
      the third maps to textValues[2] etc.. */
  QString textValues[XSLDBGEVENT_COLUMNS];

  /**
     Both int and bool values are mapped to intValues in the same manner as
     stated above */
  int intValues[XSLDBGEVENT_COLUMNS];

  /** - - - - - - The message/signal types supported   - - - - - - */
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


/**
 * This class is posted to the applications event queue. When the application
 *  has time to process the event this class then aids in emitting 
 *   the relavant signals for the event.    
 *
 * @short Emit signals to QT application via debugger base class
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
class XsldbgEvent : public QEvent {

 public:  
  XsldbgEvent(XsldbgMessageEnum type, const void *data);
  ~XsldbgEvent();

  /** Main control for emitting messages, use this from the application
      inside its event processing function */
  void emitMessage(XsldbgDebuggerBase *debugger);

  /** Emit a single message. It uses handleXXX to do the actual emitting
     of signal from debugger */
  void emitMessage(XsldbgEventData *eventData);

 private:
  /** Create the XsldbgEventData for this message. Is used by our constructor  
     it uses handleXXX function to fill in the approriate values in
     the XsldbgEventData provided */
  XsldbgEventData * createEventData(XsldbgMessageEnum type, const  void *msgData);

  /** The following functions are directly related to the eventual signals that
      will be emitted ie the signal 
             lineNoChanged(QString, int bool)  
	is mapped to 
	      handleLineNoChanged(XsldbgEventData *, void *)
    */
  void handleLineNoChanged(XsldbgEventData *eventData, const  void *msgData);
  void handleShowMessage(XsldbgEventData *eventData, const  void *msgData);
  void handleBreakpointItem(XsldbgEventData *eventData, const  void *msgData);
  void handleGlobalVariableItem(XsldbgEventData *eventData, const  void *msgData);
  void handleLocalVariableItem(XsldbgEventData *eventData, const  void *msgData);
  void handleTemplateItem(XsldbgEventData *eventData, const  void *msgData);
  void handleSourceItem(XsldbgEventData *eventData, const  void *msgData);
  void handleIncludedSourceItem(XsldbgEventData *eventData, const  void *msgData);
  void handleParameterItem(XsldbgEventData *eventData, const  void *msgData);
  void handleCallStackItem(XsldbgEventData *eventData, const  void *msgData);
  void handleEntityItem(XsldbgEventData *eventData, const  void *msgData);
  void handleResolveItem(XsldbgEventData *eventData, const  void *msgData);
  void handleIntOptionItem(XsldbgEventData *eventData, const  void *msgData);
  void handleStringOptionItem(XsldbgEventData *eventData, const  void *msgData);


 private:

  /** What type is the items in list */
  XsldbgMessageEnum itemType;

  /** A flag that gets set once the list has been filled with approriate
    XsldbgEventData */
  bool beenCreated;

  /** This is a volitile value that is only valid for the duration 
    of the constuctor. It will be set to 0L immediately after */
  const void *data; 

  /** This is a volitile value only valid for duration of emitMessage 
     function. It will be set to 0L imedediately after */
  XsldbgDebuggerBase *debugger;

  /** This is the data associated with this event 
      each data item in the list will be of the type required
      by the "type" this event
   */
  QList<XsldbgEventData> list;
    };

#endif
