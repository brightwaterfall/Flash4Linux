/***************************************************************************
              f4lm.cpp  -  description
                 -------------------
    begin                : Sat Jun  7 02:29:46 EEST 2003
    copyright            : (C) 2003 by �kan pakdil
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

// Qt includes
#include <qvbox.h>
#include <qaccel.h>
#include <qdockwindow.h>
#include <qcolor.h>
#include <qimage.h>
#include <qdockarea.h>
#include <qmap.h>
#include <qtextstream.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qfocusdata.h>
//#include <iostream>

// application specific includes
#include "f4lmview.h"
#include "f4lmdoc.h"
#include "f4lm.h"
#include "timeline.h"
#include "tools.h"
#include "ccolordialog.h"

#include "cursor/filenew.xpm"
#include "cursor/fileopen.xpm"
#include "cursor/filesave.xpm"

F4lmApp::F4lmApp ()
{
  setObjectName ("F4lmApp");
  q3SetCaption(this, tr ("F4lm "));
  printer = new QPrinter;
  untitledCount = 0;
  pDocList = new QPtrList < F4lmDoc > ();
  pDocList->setAutoDelete (true);

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initView ();
  initActions ();
  initToolBar ();
  initStatusBar ();
  initDockWindows ();
  initMenuBar ();
  //resize( 450, 400 );

  //viewToolBar->setOn(true);
  //  viewStatusBar->setOn(true);
  fileToolbar->hide ();

}

F4lmApp::~F4lmApp ()
{
  delete printer;
}

void F4lmApp::initActions ()
{
  // fillTheStringTable();
  QPixmap openIcon, saveIcon, newIcon;
  newIcon = QPixmap (filenew);
  openIcon = QPixmap (fileopen);
  saveIcon = QPixmap (filesave);

  fileNew = q3NewAction (tr ("New File"), newIcon, tr ("&New"), QKeySequence (tr ("Ctrl+N")), this);
  fileNew->setStatusTip (tr ("Creates a new document"));
  fileNew->setWhatsThis (tr ("New File\n\nCreates a new document"));
  connect (fileNew, SIGNAL (triggered ()), this, SLOT (slotFileNew ()));

  fileNewFromTemplate = q3NewAction (tr ("New From Template"), tr ("N&ew From Template"), 0, this);
  fileNewFromTemplate->setStatusTip (tr ("Create a new document from a template"));
  fileNewFromTemplate->setWhatsThis (tr ("New From Template\n\nCreate a new document from a template"));
  connect (fileNewFromTemplate, SIGNAL (triggered ()), this, SLOT (slotfileNewFromTemplate ()));

  fileOpen = q3NewAction (tr ("Open File"), openIcon, tr ("&Open..."), 0, this);
  fileOpen->setStatusTip (tr ("Opens an existing document"));
  fileOpen->setWhatsThis (tr ("Open File\n\nOpens an existing document"));
  connect (fileOpen, SIGNAL (triggered ()), this, SLOT (slotFileOpen ()));

  fileOpenAsLibrary = q3NewAction (tr ("Open as Library..."), tr ("Open as &Library..."), 0, this);
  fileOpenAsLibrary->setStatusTip (tr ("Open a document as a floating Library window"));
  fileOpenAsLibrary->setWhatsThis (tr ("Open as Library...\n\n Open a document as a floating Library window "));
  connect (fileOpenAsLibrary, SIGNAL (triggered ()), this, SLOT (slotfileOpenAsLibrary ()));

  fileSaveAsTemplate = q3NewAction (tr ("Save As Template..."), tr ("Save As &Template..."), 0, this);
  fileSaveAsTemplate->setStatusTip (tr ("Save the active document as a template"));
  fileSaveAsTemplate->setWhatsThis (tr ("Save As Template...\n\n Save the active document as a template "));
  connect (fileSaveAsTemplate, SIGNAL (triggered ()), this, SLOT (slotfileSaveAsTemplate ()));

  fileRevert = q3NewAction (tr ("Revert"), tr ("Rever&t"), 0, this);
  fileRevert->setStatusTip (tr ("Revert to the last saved version of the active document"));
  fileRevert->setWhatsThis (tr ("Revert\n\n Revert to the last saved version of the active document "));
  connect (fileRevert, SIGNAL (triggered ()), this, SLOT (slotfileRevert ()));

  fileImport = q3NewAction (tr ("Import..."), tr ("&Import..."), 0, this);
  fileImport->setStatusTip (tr ("Import files created in another application"));
  fileImport->setWhatsThis (tr ("Import...\n\n Import files created in another application "));
  connect (fileImport, SIGNAL (triggered ()), this, SLOT (slotfileImport ()));

  fileImportToLibrary = q3NewAction (tr ("Import to Library..."), tr ("&Import to Library..."), QKeySequence (tr ("Ctrl+Alt+R")), this);
  fileImportToLibrary->setStatusTip (tr ("Import files created in another application to the document library"));
  fileImportToLibrary->setWhatsThis (tr ("Import to Library...\n\n Import files created in another application to the document library "));
  connect (fileImportToLibrary, SIGNAL (triggered ()), this, SLOT (slotfileImportToLibrary ()));

  fileExportMovie = q3NewAction (tr ("Export Movie..."), tr ("Export &Movie..."), QKeySequence (tr ("Ctrl+Alt+Shift+S")), this);
  fileExportMovie->setStatusTip (tr ("Save the document as a movie for use in another application"));
  fileExportMovie->setWhatsThis (tr ("Export Movie...\n\n Save the document as a movie for use in another application "));
  connect (fileExportMovie, SIGNAL (triggered ()), this, SLOT (slotfileExportMovie ()));

  fileExportImage = q3NewAction (tr ("Export Image..."), tr ("&Export Image..."), 0, this);
  fileExportImage->setStatusTip (tr ("Save a drawing in a format for use in another application"));
  fileExportImage->setWhatsThis (tr ("Export Image...\n\n Save a drawing in a format for use in another application "));
  connect (fileExportImage, SIGNAL (triggered ()), this, SLOT (slotfileExportImage ()));

  filePublishSetting = q3NewAction (tr ("Publish Settings..."), tr ("Publish Settin&gs..."), QKeySequence (tr ("Ctrl+Shift+F12")), this);
  filePublishSetting->setStatusTip (tr ("Modify publish settings\nPublish Settings"));
  filePublishSetting->setWhatsThis (tr ("Publish Settings...\n\n Modify publish settings\nPublish Settings "));
  connect (filePublishSetting, SIGNAL (triggered ()), this, SLOT (slotfilePublishSetting ()));

  filePublishPreview_Default = q3NewAction (tr ("Default"), tr ("&Default"), QKeySequence (tr ("Ctrl+F12")), this);
  filePublishPreview_Default->setStatusTip (tr ("Publish and preview the document with the default format"));
  filePublishPreview_Default->setWhatsThis (tr ("Default\n\n Publish and preview the document with the default format "));
  connect (filePublishPreview_Default, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_Default ()));

  filePublishPreview_Flash = q3NewAction (tr ("Flash"), tr ("&Flash"), 0, this);
  filePublishPreview_Flash->setStatusTip (tr ("Publish and preview the document in the Flash (swf) format"));
  filePublishPreview_Flash->setWhatsThis (tr ("Flash\n\nPublish and preview the document in the Flash (swf) format "));
  connect (filePublishPreview_Flash, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_Flash ()));

  filePublishPreview_Html = q3NewAction (tr ("HTML"), tr ("&HTML"), 0, this);
  filePublishPreview_Html->setStatusTip (tr ("Publish and preview the document in HTML format"));
  filePublishPreview_Html->setWhatsThis (tr ("HTML\n\n Publish and preview the document in HTML format "));
  connect (filePublishPreview_Html, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_Html ()));

  filePublishPreview_GIF = q3NewAction (tr ("GIF"), tr ("&GIF"), 0, this);
  filePublishPreview_GIF->setStatusTip (tr ("Publish and preview the document as a GIF image"));
  filePublishPreview_GIF->setWhatsThis (tr ("GIF \n\n Publish and preview the document as a GIF image "));
  connect (filePublishPreview_GIF, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_GIF ()));

  filePublishPreview_JPEG = q3NewAction (tr ("JPEG"), tr ("&JPEG"), 0, this);
  filePublishPreview_JPEG->setStatusTip (tr ("Publish and preview the document as a JPEG image"));
  filePublishPreview_JPEG->setWhatsThis (tr ("JPEG\n\n Publish and preview the document as a JPEG image "));
  connect (filePublishPreview_JPEG, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_JPEG ()));

  filePublishPreview_PNG = q3NewAction (tr ("PNG"), tr ("&PNG"), 0, this);
  filePublishPreview_PNG->setStatusTip (tr ("Publish and preview the document as a PNG image"));
  filePublishPreview_PNG->setWhatsThis (tr ("PNG\n\n Publish and preview the document as a PNG image "));
  connect (filePublishPreview_PNG, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_PNG ()));

  filePublishPreview_Projector = q3NewAction (tr ("Projector"), tr ("P&rojector"), 0, this);
  filePublishPreview_Projector->setStatusTip (tr ("Publish and preview the document as a standalone projector"));
  filePublishPreview_Projector->setWhatsThis (tr ("Projector\n\n Publish and preview the document as a standalone projector "));
  connect (filePublishPreview_Projector, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_Projector ()));

  filePublishPreview_Quicktime = q3NewAction (tr ("QuickTime"), tr ("&QuickTime"), 0, this);
  filePublishPreview_Quicktime->setStatusTip (tr ("Publish and preview the document as a QuickTime movie"));
  filePublishPreview_Quicktime->setWhatsThis (tr ("QuickTime\n\n Publish and preview the document as a QuickTime movie "));
  connect (filePublishPreview_Quicktime, SIGNAL (triggered ()), this, SLOT (slotfilePublishPreview_Quicktime ()));

  filePublish = q3NewAction (tr ("Publish"), tr ("Pu&blish"), QKeySequence (tr ("Shift+F12")), this);
  filePublish->setStatusTip (tr ("Publish document in selected formats\nPublish"));
  filePublish->setWhatsThis (tr ("Publish\n\n Publish document in selected formats\nPublish "));
  connect (filePublish, SIGNAL (triggered ()), this, SLOT (slotfilePublish ()));

  filePageSetup = q3NewAction (tr ("Page Setup..."), tr ("Page Set&up..."), 0, this);
  filePageSetup->setStatusTip (tr ("Change the paper type and print margins"));
  filePageSetup->setWhatsThis (tr ("Page Setup...\n\n Change the paper type and print margins "));
  connect (filePageSetup, SIGNAL (triggered ()), this, SLOT (slotfilePageSetup ()));

  filePrintPreview = q3NewAction (tr ("Print Preview"), tr ("Print Pre&view"), 0, this);
  filePrintPreview->setStatusTip (tr ("Show pages as they will appear on the printer\nPrint Preview"));
  filePrintPreview->setWhatsThis (tr ("Print Preview\n\n Show pages as they will appear on the printer\nPrint Preview "));
  connect (filePrintPreview, SIGNAL (triggered ()), this, SLOT (slotfilePrintPreview ()));


  fileSend = q3NewAction (tr ("Send..."), tr ("Sen&d..."), 0, this);
  fileSend->setStatusTip (tr ("Send a mail message with an attached document"));
  fileSend->setWhatsThis (tr("Send...\n\n Send a mail message with an attached document "));
  connect (fileSend, SIGNAL (triggered ()), this, SLOT (slotfileSend ()));

  fileRecentFile = q3NewAction (tr ("Recent File"), tr ("R&ecent File"), 0, this);
  fileRecentFile->setStatusTip (tr ("Open this document"));
  fileRecentFile->setWhatsThis (tr ("Recent File\n\n Open the recen file "));
  connect (fileRecentFile, SIGNAL (triggered ()), this, SLOT (slotfileRecentFile ()));

  fileSave = q3NewAction (tr ("Save File"), saveIcon, tr ("&Save"), QKeySequence (tr ("Ctrl+S")), this);
  fileSave->setStatusTip (tr ("Saves the actual document"));
  fileSave->setWhatsThis (tr ("Save File.\n\nSaves the actual document"));
  connect (fileSave, SIGNAL (triggered ()), this, SLOT (slotFileSave ()));

  fileSaveAs = q3NewAction (tr ("Save File As"), tr ("Save &as..."), 0, this);
  fileSaveAs->setStatusTip (tr ("Saves the actual document under a new filename"));
  fileSaveAs->setWhatsThis (tr ("Save As\n\nSaves the actual document under a new filename"));
  connect (fileSaveAs, SIGNAL (triggered ()), this, SLOT (slotFileSave ()));

  fileClose = q3NewAction (tr ("Close File"), tr ("&Close"), QKeySequence (tr ("Ctrl+W")), this);
  fileClose->setStatusTip (tr ("Closes the actual document"));
  fileClose->setWhatsThis (tr ("Close File\n\nCloses the actual document"));
  connect (fileClose, SIGNAL (triggered ()), this, SLOT (slotFileClose ()));

  filePrint = q3NewAction (tr ("Print File"), tr ("&Print"), QKeySequence (tr ("Ctrl+P")), this);
  filePrint->setStatusTip (tr ("Prints out the actual document"));
  filePrint->setWhatsThis (tr ("Print File\n\nPrints out the actual document"));
  connect (filePrint, SIGNAL (triggered ()), this, SLOT (slotFilePrint ()));

  fileQuit = q3NewAction (tr ("Exit"), tr ("E&xit"), QKeySequence (tr ("Ctrl+Q")), this);
  fileQuit->setStatusTip (tr ("Quits the application"));
  fileQuit->setWhatsThis (tr ("Exit\n\nQuits the application"));
  connect (fileQuit, SIGNAL (triggered ()), this, SLOT (slotFileQuit ()));

  /////////////////////////////////////////////////////////////////////////////////
  editCut = q3NewAction (tr ("Cut"), tr ("Cu&t"), QKeySequence (tr ("Ctrl+X")), this);
  editCut->setStatusTip (tr ("Cuts the selected section and puts it to the clipboard"));
  editCut->setWhatsThis (tr ("Cut\n\nCuts the selected section and puts it to the clipboard"));
  connect (editCut, SIGNAL (triggered ()), this, SLOT (slotEditCut ()));

  editCopy =q3NewAction (tr ("Copy"), tr ("&Copy"), QKeySequence (tr ("Ctrl+C")), this);
  editCopy->setStatusTip (tr ("Copies the selected section to the clipboard"));
  editCopy->setWhatsThis (tr ("Copy\n\nCopies the selected section to the clipboard"));
  connect (editCopy, SIGNAL (triggered ()), this, SLOT (slotEditCopy ()));

  editUndo = q3NewAction (tr ("Undo"), tr ("&Undo"), QKeySequence (tr ("Ctrl+Z")), this);
  editUndo->setStatusTip (tr ("Reverts the last editing action"));
  editUndo->setWhatsThis (tr ("Undo\n\nReverts the last editing action"));
  connect (editUndo, SIGNAL (triggered ()), this, SLOT (slotEditUndo ()));

  editPaste = q3NewAction (tr ("Paste"), tr ("&Paste"),QKeySequence (tr ("Ctrl+V")), this);
  editPaste->setStatusTip (tr ("Pastes the clipboard contents to actual position"));
  editPaste-> setWhatsThis (tr("Paste\n\nPastes the clipboard contents to actual position"));
  connect (editPaste, SIGNAL (triggered ()), this, SLOT (slotEditPaste ()));


  editRedo = q3NewAction (tr ("Redo"), tr ("&Redo"),QKeySequence (tr ("Ctrl+Y")), this);
  editRedo->setStatusTip (tr ("Redo the previously undone action\nRedo"));
  editRedo->setWhatsThis (tr ("Redo\n\nRedo the previously undone action\nRedo"));
  connect (editRedo, SIGNAL (triggered ()), this, SLOT (sloteditRedo ()));

  editPasteinPlace =q3NewAction (tr ("Paste in Place"), tr ("Paste i&n Place"),QKeySequence (tr ("Ctrl+Shift+V")), this);
  editPasteinPlace->setStatusTip (tr("Insert the Clipboard contents without centering in the window"));
  editPasteinPlace->setWhatsThis (tr("Paste in Place\n\nInsert the Clipboard contents without centering in the window"));
  connect (editPasteinPlace, SIGNAL (triggered ()), this,SLOT (sloteditPasteinPlace ()));


  editPasteSpecial =q3NewAction (tr ("Paste Special..."), tr ("Paste &Special..."), 0, this);
  editPasteSpecial->setStatusTip (tr("Insert the Clipboard contents with options"));
  editPasteSpecial->setWhatsThis (tr("Paste Special...\n\nInsert the Clipboard contents with options"));
  connect (editPasteSpecial, SIGNAL (triggered ()), this, SLOT (sloteditPasteSpecial ()));

  editClear = q3NewAction (tr ("Clear"), tr ("Cle&ar"),  QKeySequence (Qt::Key_Backspace), this);
  editClear->setStatusTip (tr ("Delete the selection"));
  editClear->setWhatsThis (tr ("Clear\n\nDelete the selection"));
  connect (editClear, SIGNAL (triggered ()), this, SLOT (sloteditClear ()));


  editDuplicate = q3NewAction (tr ("Duplicate"), tr ("&Duplicate"), QKeySequence (Qt::CTRL + Qt::Key_D), this);
  editDuplicate->setStatusTip (tr ("Duplicate the selection"));
  editDuplicate->setWhatsThis (tr ("Duplicate\n\nDuplicate the selection"));
  connect (editDuplicate, SIGNAL (triggered ()), this,SLOT (sloteditDuplicate ()));

  editSelectAll = q3NewAction (tr ("Select All"), tr ("Select A&ll"), QKeySequence (Qt::CTRL + Qt::Key_A), this);
  editSelectAll->setStatusTip (tr ("Select the entire drawing"));
  editSelectAll->setWhatsThis (tr ("Select All\n\nSelect the entire drawing"));
  connect (editSelectAll, SIGNAL (triggered ()), this, SLOT (sloteditSelectAll ()));

  editDeselectAll =q3NewAction (tr ("Deselect All"), tr ("D&eselect All"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_A), this);
  editDeselectAll->setStatusTip (tr ("Deselect all selected elements in the drawing"));
  editDeselectAll->setWhatsThis (tr("Deselect All\n\nDeselect all selected elements in the drawing"));
  connect (editDeselectAll, SIGNAL (triggered ()), this,SLOT (sloteditDeselectAll ()));

  editCutFrames =q3NewAction (tr ("Cut Frames"), tr ("Cut Fra&mes"), 0, this);
  editCutFrames->setStatusTip (tr ("Cut the selected frames to the frame clipboard"));
  editCutFrames->setWhatsThis (tr("Cut Frames\n\nCut the selected frames to the frame clipboard"));
  connect (editCutFrames, SIGNAL (triggered ()), this,SLOT (sloteditCutFrames ()));

  editCopyFrames =q3NewAction (tr ("Copy Frames"), tr ("C&opy Frames"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_C), this);
  editCopyFrames->setStatusTip (tr ("Copy the selected frames to the frame clipboard"));
  editCopyFrames->setWhatsThis (tr("Copy Frames\n\nCopy the selected frames to the frame clipboard"));
  connect (editCopyFrames, SIGNAL (triggered ()), this,SLOT (sloteditCopyFrames ()));

  editPasteFrames =q3NewAction (tr ("Paste Frames"), tr ("Paste &Frames"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_V), this);
  editPasteFrames->setStatusTip (tr ("Paste frames from the frame clipboard"));
  editPasteFrames->setWhatsThis (tr("Paste Frames\n\nPaste frames from the frame clipboard"));
  connect (editPasteFrames, SIGNAL (triggered ()), this, SLOT (sloteditPasteFrames ()));

  editClearFrames =q3NewAction (tr ("Clear Frames"), tr ("Clear F&rames"),QKeySequence (Qt::ALT + Qt::Key_Delete), this);
  editClearFrames->setStatusTip (tr ("Delete the selected frames"));
  editClearFrames->setWhatsThis (tr ("Clear Frames\n\nDelete the selected frames"));
  connect (editClearFrames, SIGNAL (triggered ()), this,SLOT (sloteditClearFrames ()));

  editSelectAllFrames = q3NewAction (tr ("Select All Frames"), tr ("Select All Fram&es"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_A), this);
  editSelectAllFrames->setStatusTip (tr("Select all the frames in the current scene or symbol"));
  editSelectAllFrames->setWhatsThis (tr("Select All Frames\n\nSelect all the frames in the current scene or symbol"));
  connect (editSelectAllFrames, SIGNAL (triggered ()), this, SLOT (sloteditSelectAllFrames ()));

  editEditSymbols = q3NewAction (tr ("Edit Symbols"), tr ("Edit S&ymbols"),QKeySequence (Qt::CTRL + Qt::Key_E), this);
  editEditSymbols->setStatusTip (tr ("Toggle between editing the symbol or movie scenes"));
  editEditSymbols->setWhatsThis (tr("Edit Symbols\n\nToggle between editing the symbol or movie scenes"));
  connect (editEditSymbols, SIGNAL (triggered ()), this,SLOT (sloteditEditSymbols ()));

  editEditSelected =q3NewAction (tr ("Edit Selected"), tr ("Ed&it Selected"), 0, this);
  editEditSelected->setStatusTip (tr ("Edit the contents of the selected object"));
  editEditSelected->setWhatsThis (tr("Edit Selected\n\nEdit the contents of the selected object"));
  connect (editEditSelected, SIGNAL (triggered ()), this, SLOT (sloteditEditSelected ()));

  editEditInPlace =q3NewAction (tr ("Edit in Place"), tr ("Edit in Place"), 0, this);
  editEditInPlace->setStatusTip (tr ("Edit the symbol in place\nEdit"));
  editEditInPlace->setWhatsThis (tr ("Edit in Place\n\nEdit the symbol in place\nEdit"));
  connect (editEditInPlace, SIGNAL (triggered ()), this,SLOT (sloteditEditInPlace ()));

  editEditAll = q3NewAction (tr ("Edit All"), tr ("Edit &All"), 0, this);
  editEditAll->setStatusTip (tr ("Return to editing the entire drawing"));
  editEditAll->setWhatsThis (tr ("Edit All\n\nReturn to editing the entire drawing"));
  connect (editEditAll, SIGNAL (triggered ()), this,SLOT (sloteditEditAll ()));


  editPrefences = q3NewAction (tr ("Preferences..."), tr ("Pre&ferences..."), 0, this);
  editPrefences->setStatusTip (tr ("Change various default settings"));
  editPrefences->setWhatsThis (tr ("Preferences...\n\nChange various default settings"));
  connect (editPrefences, SIGNAL (triggered ()), this, SLOT (sloteditPrefences ()));

  editKeyboardShourtCuts =q3NewAction (tr ("Keyboard Shortcuts..."), tr ("&Keyboard Shortcuts..."), 0, this);
  editKeyboardShourtCuts->setStatusTip (tr ("Customize keyboard shortcuts for menus and tools"));
  editKeyboardShourtCuts->setWhatsThis (tr("Keyboard Shortcuts...\n\nCustomize keyboard shortcuts for menus and tools"));
  connect (editKeyboardShourtCuts, SIGNAL (triggered ()), this,SLOT (sloteditKeyboardShourtCuts ()));

  editFontMapping =q3NewAction (tr ("Font Mapping..."), tr ("Font Mappin&g..."), 0, this);
  editFontMapping->setStatusTip (tr ("Customize font mappings for text with missing font"));
  editFontMapping->setWhatsThis (tr("Font Mapping...\n\nCustomize font mappings for text with missing font"));
  connect (editFontMapping, SIGNAL (triggered ()), this, SLOT (sloteditFontMapping ()));

  viewToolBar =  q3NewAction (tr ("Toolbar"), tr ("Tool&bar"), 0, this, 0, true);
  viewToolBar->setStatusTip (tr ("Enables/disables the toolbar"));
  viewToolBar->setWhatsThis (tr ("Toolbar\n\nEnables/disables the toolbar"));
  connect (viewToolBar, SIGNAL (toggled (bool)), this, SLOT (slotViewToolBar (bool)));

  viewStatusBar =q3NewAction (tr ("Statusbar"), tr ("&Statusbar"), 0, this, 0, true);
  viewStatusBar->setStatusTip (tr ("Enables/disables the statusbar"));
  viewStatusBar->setWhatsThis (tr ("Statusbar\n\nEnables/disables the statusbar"));
  connect (viewStatusBar, SIGNAL (toggled (bool)), this,SLOT (slotViewStatusBar (bool)));

  viewFirst = q3NewAction (tr ("First"), tr ("&First"), QKeySequence (Qt::Key_Home),this);
  viewFirst->setStatusTip (tr ("Go to the first scene of the movie"));
  viewFirst->setWhatsThis (tr ("First\n\nGo to the first scene of the movie"));
  connect (viewFirst, SIGNAL (triggered ()), this, SLOT (slotviewFirst ()));

  viewPrevious = q3NewAction (tr ("Previous"), tr ("Previous"),QKeySequence (Qt::Key_PageUp), this);
  viewPrevious->setStatusTip (tr ("Go to the previous scene of the movie"));
  viewPrevious->setWhatsThis (tr ("Previous\n\nGo to the previous scene of the movie"));
  connect (viewPrevious, SIGNAL (triggered ()), this,SLOT (slotviewPrevious ()));

  viewNext = q3NewAction (tr ("Next"), tr ("&Next"), QKeySequence (Qt::Key_PageDown),this);
  viewNext->setStatusTip (tr ("Go to the next scene of the movie"));
  viewNext->setWhatsThis (tr ("Next\n\nGo to the next scene of the movie"));
  connect (viewNext, SIGNAL (triggered ()), this, SLOT (slotviewNext ()));

  viewLast =q3NewAction (tr ("Last"), tr ("&Last"), QKeySequence (Qt::Key_End), this);
  viewLast->setStatusTip (tr ("Go to the last scene of the movie"));
  viewLast->setWhatsThis (tr ("Last\n\nGo to the last scene of the movie"));
  connect (viewLast, SIGNAL (triggered ()), this, SLOT (slotviewLast ()));

  viewScenes = q3NewAction (tr ("Scenes"), tr ("&Scenes"), 0, this);
  viewScenes->setStatusTip (tr ("Go to a scene of the movie"));
  viewScenes->setWhatsThis (tr ("Scenes\n\nGo to a scene of the movie"));
  connect (viewScenes, SIGNAL (triggered ()), this,SLOT (slotviewScenes ()));


  viewZoomIn =q3NewAction (tr ("Zoom In"), tr ("Zoom &In"),QKeySequence (Qt::CTRL + Qt::Key_Plus), this);
  viewZoomIn->setStatusTip (tr ("Show a smaller area of the drawing with more detail"));
  viewZoomIn->setWhatsThis (tr("Zoom In\n\nShow a smaller area of the drawing with more detail"));
  connect (viewZoomIn, SIGNAL (triggered ()), this,SLOT (slotviewZoomIn ()));

  viewZoomOut =q3NewAction (tr ("Zoom Out"), tr ("Zoom &Out"),QKeySequence (Qt::CTRL + Qt::Key_Minus), this);
  viewZoomOut->setStatusTip (tr ("Show a larger area of the drawing with less detail"));
  viewZoomOut->setWhatsThis (tr("Zoom Out\n\nShow a larger area of the drawing with less detail"));
  connect (viewZoomOut, SIGNAL (triggered ()), this,SLOT (slotviewZoomOut ()));

  viewMagnification25 = q3NewAction (tr ("25%"), tr ("&25%"), 0, this);
  viewMagnification25->setStatusTip (tr ("Zoom to 25%"));
  viewMagnification25->setWhatsThis (tr ("25%\n\nZoom to 25%"));
  connect (viewMagnification25, SIGNAL (triggered ()), this,SLOT (slotviewMagnification25 ()));

  viewMagnification50 = q3NewAction (tr ("50%"), tr ("&50%"), 0, this);
  viewMagnification50->setStatusTip (tr ("Zoom to 50%"));
  viewMagnification50->setWhatsThis (tr ("50%\n\nZoom to 50%"));
  connect (viewMagnification50, SIGNAL (triggered ()), this,SLOT (slotviewMagnification50 ()));

  viewMagnification100 =q3NewAction (tr ("100%"), tr ("&100%"),QKeySequence (Qt::CTRL + Qt::Key_1), this);
  viewMagnification100->setStatusTip (tr ("Zoom to 100%"));
  viewMagnification100->setWhatsThis (tr ("100%\n\nZoom to 100%"));
  connect (viewMagnification100, SIGNAL (triggered ()), this,SLOT (slotviewMagnification100 ()));

  viewMagnification200 = q3NewAction (tr ("200%"), tr ("&200%"), 0, this);
  viewMagnification200->setStatusTip (tr ("Zoom to 200%"));
  viewMagnification200->setWhatsThis (tr ("200%\n\nZoom to 200%"));
  connect (viewMagnification200, SIGNAL (triggered ()), this,SLOT (slotviewMagnification200 ()));

  viewMagnification400 = q3NewAction (tr ("400%"), tr ("&400%"), 0, this);
  viewMagnification400->setStatusTip (tr ("Zoom to 400%"));
  viewMagnification400->setWhatsThis (tr ("400%\n\nZoom to 400%"));
  connect (viewMagnification400, SIGNAL (triggered ()), this,SLOT (slotviewMagnification400 ()));

  viewMagnification800 = q3NewAction (tr ("800%"), tr ("&800%"), 0, this);
  viewMagnification800->setStatusTip (tr ("Zoom to 800%"));
  viewMagnification800->setWhatsThis (tr ("800%\n\nZoom to 800%"));
  connect (viewMagnification800, SIGNAL (triggered ()), this,SLOT (slotviewMagnification800 ()));


  viewShowFrame =q3NewAction (tr ("Show Frame"), tr ("Show &Frame"),QKeySequence (Qt::CTRL + Qt::Key_2), this);
  viewShowFrame->setStatusTip (tr ("Show the entire frame in the window"));
  viewShowFrame->setWhatsThis (tr ("Show Frame\n\nShow the entire frame in the window"));
  connect (viewShowFrame, SIGNAL (triggered ()), this,SLOT (slotviewShowFrame ()));

  viewShowAll =q3NewAction (tr ("Show All"), tr ("Show &All"),QKeySequence (Qt::CTRL + Qt::Key_3), this);
  viewShowAll->setStatusTip (tr("Show the entire contents of the drawing in the window"));
  viewShowAll->setWhatsThis (tr("Show All\n\nShow the entire contents of the drawing in the window"));
  connect (viewShowAll, SIGNAL (triggered ()), this,SLOT (slotviewShowAll ()));

  viewOutlines =q3NewAction (tr ("Outlines"), tr ("O&utlines"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_O),this, "Outlines", true);
  viewOutlines->setStatusTip (tr ("Show only the outines of the movie"));
  viewOutlines->setWhatsThis (tr ("Outlines\n\nShow only the outines of the movie"));
  connect (viewOutlines, SIGNAL (triggered ()), this,SLOT (slotviewOutlines ()));

  viewFast =q3NewAction (tr ("Fast"), tr ("Fa&st"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_F),this, "Fast", true);
  viewFast->setStatusTip (tr ("Show the fast view of the movie"));
  viewFast->setWhatsThis (tr ("Fast\n\nShow the fast view of the movie"));
  connect (viewFast, SIGNAL (triggered ()), this, SLOT (slotviewFast ()));

  viewAntialias =q3NewAction (tr ("Antialias"), tr ("A&ntialias"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_A),this, "Antialias", true);
  viewAntialias->setStatusTip (tr ("Show the edges antialiased in the movie"));
  viewAntialias->setWhatsThis (tr("Antialias\n\nShow the edges antialiased in the movie"));
  connect (viewAntialias, SIGNAL (triggered ()), this,SLOT (slotviewAntialias ()));

  viewAntialiasText =q3NewAction (tr ("Antialias Text"), tr ("Antialias &Text"), QKeySequence (Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_T),this, "Antialias_text", true);
  viewAntialiasText->setStatusTip (tr ("Show the edges and text antialiased in the movie"));
  viewAntialiasText->setWhatsThis (tr("Antialias Text\n\nShow the edges and text antialiased in the movie"));
  connect (viewAntialiasText, SIGNAL (triggered ()), this,SLOT (slotviewAntialiasText ()));

  viewAction = new QActionGroup (this);
  viewAction->setExclusive (true);
  viewAction->addAction (viewOutlines);
  viewAction->addAction (viewFast);
  viewAction->addAction (viewAntialias);
  viewAction->addAction (viewAntialiasText);

  viewTimeline =q3NewAction (tr ("Timeline"), tr ("Time&line"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_T), this,"viewTimeline", true);
  viewTimeline->setStatusTip (tr("Show or hide the animation timeline and layers controls"));
  viewTimeline->setWhatsThis (tr("Timeline\n\nShow or hide the animation timeline and layers controls"));
  connect (viewTimeline, SIGNAL (triggered ()), this,SLOT (slotviewTimeline ()));

  viewWorkArea =q3NewAction (tr ("Work Area"), tr ("&Work Area"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_W), this,"viewWorkArea", true);
  viewWorkArea->setStatusTip (tr ("Show or hide the work area that surrounds the document frame"));
  viewWorkArea->setWhatsThis (tr("Work Area\n\nShow or hide the work area that surrounds the document frame"));
  connect (viewWorkArea, SIGNAL (triggered ()), this,SLOT (slotviewWorkArea ()));

  viewRulers =q3NewAction (tr ("Rulers"), tr ("&Rulers"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::ALT + Qt::Key_R), this, "viewRulers", true);
  viewRulers->setStatusTip (tr ("Show or hide the rulers"));
  viewRulers->setWhatsThis (tr ("Rulers\n\nShow or hide the rulers"));
  connect (viewRulers, SIGNAL (triggered ()), this,SLOT (slotviewRulers ()));

  viewShowGrid =q3NewAction (tr ("Show Grid"), tr ("Show Gri&d"),QKeySequence (Qt::CTRL + Qt::Key_Apostrophe), this,"viewShowGrid", true);
  viewShowGrid->setStatusTip (tr ("Show or hide the drawing grid"));
  viewShowGrid->setWhatsThis (tr ("Show Grid\n\nShow or hide the drawing grid"));
  connect (viewShowGrid, SIGNAL (triggered ()), this,SLOT (slotviewShowGrid ()));

  viewSnaptoGrid =q3NewAction (tr ("Snap to Grid"), tr ("Sna&p to Grid"), QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_Apostrophe),this, "viewShowGrid", true);
  viewSnaptoGrid->setStatusTip (tr ("Show or hide the drawing grid"));
  viewSnaptoGrid->setWhatsThis (tr ("Snap to Grid\n\nShow or hide the drawing grid"));
  connect (viewSnaptoGrid, SIGNAL (triggered ()), this,SLOT (slotviewSnaptoGrid ()));

  viewEditGrid =q3NewAction (tr ("Edit Grid..."), tr ("&Edit Grid..."),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_G), this);
  viewEditGrid->setStatusTip (tr ("Open the grid properties dialog"));
  viewEditGrid->setWhatsThis (tr ("Edit Grid...\n\nOpen the grid properties dialog"));
  connect (viewEditGrid, SIGNAL (triggered ()), this,SLOT (slotviewEditGrid ()));

  viewShowGuides =q3NewAction (tr ("Show Guides"), tr ("Show G&uides"),QKeySequence (Qt::CTRL + Qt::Key_Semicolon), this);
  viewShowGuides->setStatusTip (tr ("Show or hide the guides"));
  viewShowGuides->setWhatsThis (tr ("Show Guides\n\nShow or hide the guides"));
  connect (viewShowGuides, SIGNAL (triggered ()), this, SLOT (slotviewShowGuides ()));

  viewLockGuides =q3NewAction (tr ("Lock Guides"), tr ("Loc&k Guides"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_Semicolon), this);
  viewLockGuides->setStatusTip (tr ("Lock or unlock the guides"));
  viewLockGuides->setWhatsThis (tr ("Lock Guides\n\nLock or unlock the guides"));
  connect (viewLockGuides, SIGNAL (triggered ()), this,SLOT (slotviewLockGuides ()));

  viewSnaptoGuides =q3NewAction (tr ("Snap to Guides"), tr ("Snap to Gu&ides"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_Semicolon),this);
  viewSnaptoGuides->setStatusTip (tr ("Turn Snap to Guides on or off"));
  viewSnaptoGuides->setWhatsThis (tr ("Snap to Guides\n\nTurn Snap to Guides on or off"));
  connect (viewSnaptoGuides, SIGNAL (triggered ()), this,SLOT (slotviewSnaptoGuides ()));

  viewEditGuides =q3NewAction (tr ("Edit Guides..."), tr ("Edit Guides..."),QKeySequence (Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_G),this);
  viewEditGuides->setStatusTip (tr ("Turn Snap to Guides on or off"));
  viewEditGuides->setWhatsThis (tr ("Edit Guides...\n\nTurn Snap to Guides on or off"));
  connect (viewEditGuides, SIGNAL (triggered ()), this,SLOT (slotviewEditGuides ()));

  viewSnaptoPixels =q3NewAction (tr ("Snap to Pixels"), tr ("Snap to Pixels"), 0, this);
  viewSnaptoPixels->setStatusTip (tr ("Turn Snap to Pixels on or off"));
  viewSnaptoPixels->setWhatsThis (tr ("Snap to Pixels\n\nTurn Snap to Pixels on or off"));
  connect (viewSnaptoPixels, SIGNAL (triggered ()), this,SLOT (slotviewSnaptoPixels ()));

  viewSnaptoObjects =q3NewAction (tr ("Snap to Objects"), tr ("Snap to O&bjects"), 0, this);
  viewSnaptoObjects->setStatusTip (tr("Turn snap to objects and automatic connection of lines on or off\nSnap to Objects"));
  viewSnaptoObjects->setWhatsThis (tr("Snap to Objects\n\nTurn snap to objects and automatic connection of lines on or off\nSnap to Objects"));
  connect (viewSnaptoObjects, SIGNAL (triggered ()), this, SLOT (slotviewSnaptoObjects ()));

  viewShowShapeHints =q3NewAction (tr ("Show Shape Hints"), tr ("Show Sh&ape Hints"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_H), this);
  viewShowShapeHints->setStatusTip (tr ("Show or hide the tweening shape hints"));
  viewShowShapeHints->setWhatsThis (tr("Show Shape Hints\n\nShow or hide the tweening shape hints"));
  connect (viewShowShapeHints, SIGNAL (triggered ()), this,SLOT (slotviewShowShapeHints ()));

  viewHideEdges =q3NewAction (tr ("Hide Edges"), tr ("&Hide Edges"),QKeySequence (Qt::CTRL + Qt::Key_H), this);
  viewHideEdges->setStatusTip (tr("When turned on  suppresses hilighting of selected items"));
  viewHideEdges->setWhatsThis (tr("Hide Edges\n\nWhen turned on  suppresses hilighting of selected items"));
  connect (viewHideEdges, SIGNAL (triggered ()), this,SLOT (slotviewHideEdges ()));

  viewHidePanels =q3NewAction (tr ("Hide Panels"), tr ("Hide &Panels"), 0, this);
  viewHidePanels->setStatusTip (tr ("Hide or show all panels and tools"));
  viewHidePanels->setWhatsThis (tr ("Hide Panels\n\nHide or show all panels and tools"));
  connect (viewHidePanels, SIGNAL (triggered ()), this,SLOT (slotviewHidePanels ()));

  insertConverttoSymbol =q3NewAction (tr ("Convert to Symbol..."), tr ("&Convert to Symbol..."),QKeySequence (Qt::Key_F8), this);
  insertConverttoSymbol->setStatusTip (tr("Create a new symbol from the selection that can be used in multiple places"));
  insertConverttoSymbol->setWhatsThis (tr("Convert to Symbol...\n\nCreate a new symbol from the selection that can be used in multiple places"));
  connect (insertConverttoSymbol, SIGNAL (triggered ()), this,SLOT (slotinsertConverttoSymbol ()));

  insertNewSymbol =q3NewAction (tr ("New Symbol..."), tr ("&New Symbol..."),QKeySequence (Qt::CTRL + Qt::Key_F8), this);
  insertNewSymbol->setStatusTip (tr("Create a new symbol that can be used in multiple places"));
  insertNewSymbol->setWhatsThis (tr("New Symbol...\n\nCreate a new symbol that can be used in multiple places"));
  connect (insertNewSymbol, SIGNAL (triggered ()), this,SLOT (slotinsertNewSymbol ()));

  insertLayer = q3NewAction (tr ("Layer"), tr ("&Layer"), 0, this);
  insertLayer->setStatusTip (tr ("Insert a new layer in the timeline"));
  insertLayer->setWhatsThis (tr ("Layer\n\nInsert a new layer in the timeline"));
  connect (insertLayer, SIGNAL (triggered ()), this,SLOT (slotinsertLayer ()));

  insertLayerFolder =q3NewAction (tr ("Layer Folder"), tr ("Layer F&older"), 0, this);
  insertLayerFolder->setStatusTip (tr ("Insert a new layer folder in the timeline"));
  insertLayerFolder->setWhatsThis (tr("Layer Folder\n\nInsert a new layer folder in the timeline"));
  connect (insertLayerFolder,SIGNAL (triggered ()),this,SLOT(slotinsertLayerFolder ()));
  insertMotionGuide =q3NewAction (tr ("Motion Guide"), tr ("&Motion Guide"), 0, this);
  insertMotionGuide->setStatusTip (tr ("Add a motion guide for the current layer"));
  insertMotionGuide->setWhatsThis (tr("Motion Guide\n\nAdd a motion guide for the current layer"));
  connect (insertMotionGuide, SIGNAL (triggered ()), this,SLOT (slotinsertMotionGuide ()));

  insertFrame =q3NewAction (tr ("Frame"), tr ("&Frame"), QKeySequence (Qt::Key_F5),this);
  insertFrame->setStatusTip (tr ("Insert frames in the selected layers"));
  insertFrame->setWhatsThis (tr ("Frame\n\nInsert frames in the selected layers"));
  connect (insertFrame, SIGNAL (triggered ()), this,SLOT (slotinsertFrame ()));

  insertRemoveFrames =q3NewAction (tr ("Remove Frames"), tr ("R&emove Frames"),QKeySequence (Qt::SHIFT + Qt::Key_F5), this);
  insertRemoveFrames->setStatusTip (tr ("Delete the selected frames"));
  insertRemoveFrames->setWhatsThis (tr ("Remove Frames\n\nDelete the selected frames"));
  connect (insertRemoveFrames, SIGNAL (triggered ()), this,SLOT (slotinsertRemoveFrames ()));

  insertKeyframe =q3NewAction (tr ("Keyframe"), tr ("&Keyframe"), QKeySequence (Qt::Key_F6),this);
  insertKeyframe->setStatusTip (tr("Make keyframes with the same contents as the selected frames"));
  insertKeyframe->setWhatsThis (tr("Keyframe\n\nMake keyframes with the same contents as the selected frames"));
  connect (insertKeyframe, SIGNAL (triggered ()), this,SLOT (slotinsertKeyframe ()));

  insertBlankKeyframe =q3NewAction (tr ("Blank Keyframe"), tr ("&Blank Keyframe"),QKeySequence (Qt::Key_F7), this);
  insertBlankKeyframe->setStatusTip (tr ("Create blank keyframes at the selected frames"));
  insertBlankKeyframe->setWhatsThis (tr("Blank Keyframe\n\nCreate blank keyframes at the selected frames"));
  connect (insertBlankKeyframe, SIGNAL (triggered ()), this,SLOT (slotinsertBlankKeyframe ()));

  insertClearKeyframe =q3NewAction (tr ("Clear Keyframe"), tr ("Cle&ar Keyframe"),QKeySequence (Qt::SHIFT + Qt::Key_F6), this);
  insertClearKeyframe->setStatusTip (tr ("Delete the selected keyframes"));
  insertClearKeyframe->setWhatsThis (tr ("Clear Keyframe\n\nDelete the selected keyframes"));
  connect (insertClearKeyframe, SIGNAL (triggered ()), this,SLOT (slotinsertClearKeyframe ()));

  insertCreateMotionTween =q3NewAction (tr ("Create Motion Tween"), tr ("Create Motion &Tween"), 0,this);
  insertCreateMotionTween->setStatusTip (tr ("Set the selected frames to motion tweening"));
  insertCreateMotionTween->setWhatsThis (tr("Create Motion Tween\n\nSet the selected frames to motion tweening"));
  connect (insertCreateMotionTween, SIGNAL (triggered ()), this,SLOT (slotinsertCreateMotionTween ()));

  insertScene = q3NewAction (tr ("Scene"), tr ("&Scene"), 0, this);
  insertScene->setStatusTip (tr ("Add a new scene after the current scene"));
  insertScene->setWhatsThis (tr ("Scene\n\nAdd a new scene after the current scene"));
  connect (insertScene, SIGNAL (triggered ()), this,SLOT (slotinsertScene ()));

  insertRemoveScene =q3NewAction (tr ("Remove Scene"), tr ("&Remove Scene"), 0, this);
  insertRemoveScene->setStatusTip (tr ("Delete the current scene"));
  insertRemoveScene->setWhatsThis (tr ("Remove Scene\n\nDelete the current scene"));
  connect (insertRemoveScene, SIGNAL (triggered ()), this,SLOT (slotinsertRemoveScene ()));

  modifyLayer = q3NewAction (tr ("&Layer..."), tr ("&Layer..."), 0, this);
  modifyLayer->setStatusTip (tr ("Edit the properties of the current layer"));
  modifyLayer->setWhatsThis (tr("&Layer...\n\nEdit the properties of the current layer"));
  connect (modifyLayer, SIGNAL (triggered ()), this,SLOT (slotmodifyLayer ()));

  modifyScene = q3NewAction (tr ("&Scene..."), tr ("&Scene..."), 0, this);
  modifyScene->setStatusTip (tr("Show or change a list of the scenes in the current movie"));
  modifyScene->setWhatsThis (tr("&Scene...\n\nShow or change a list of the scenes in the current movie"));
  connect (modifyScene, SIGNAL (triggered ()), this,SLOT (slotmodifyScene ()));

  modifyDocument =q3NewAction (tr ("&Document..."), tr ("&Document..."),QKeySequence (Qt::CTRL + Qt::Key_M), this);
  modifyDocument->setStatusTip (tr("Change the size and other attributes of the document"));
  modifyDocument->setWhatsThis (tr("&Document...\n\nChange the size and other attributes of the document"));
  connect (modifyDocument, SIGNAL (triggered ()), this,SLOT (slotmodifyDocument ()));

  modifySmooth = q3NewAction (tr ("&Smooth"), tr ("&Smooth"), 0, this);
  modifySmooth->setStatusTip (tr ("Smooth the selected lines\nSmooth"));
  modifySmooth->setWhatsThis (tr ("&Smooth\n\nSmooth the selected lines\nSmooth"));
  connect (modifySmooth, SIGNAL (triggered ()), this,SLOT (slotmodifySmooth ()));

  modifyStraighten =q3NewAction (tr ("S&traighten"), tr ("S&traighten"), 0, this);
  modifyStraighten->setStatusTip (tr ("Straighten the selected lines\nStraighten"));
  modifyStraighten->setWhatsThis (tr("S&traighten\n\nStraighten the selected lines\nStraighten"));
  connect (modifyStraighten, SIGNAL (triggered ()), this,SLOT (slotmodifyStraighten ()));

  modifyOptimize = q3NewAction (tr ("&Optimize..."), tr ("&Optimize..."),QKeySequence (Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_C), this);
  modifyOptimize->setStatusTip (tr("Smooth the selected curves so that they use less resources"));
  modifyOptimize-> setWhatsThis (tr("&Optimize...\n\nSmooth the selected curves so that they use less resources"));
  connect (modifyOptimize, SIGNAL (triggered ()), this,SLOT (slotmodifyOptimize ()));

  modifyConvertLinestoFills =q3NewAction (tr ("&Convert Lines to Fills"), tr ("&Convert Lines to Fills"), 0, this);
  modifyConvertLinestoFills->setStatusTip (tr ("Convert lines to filled areas"));
  modifyConvertLinestoFills->setWhatsThis (tr("&Convert Lines to Fills\n\nConvert lines to filled areas"));
  connect (modifyConvertLinestoFills, SIGNAL (triggered ()), this,SLOT (slotmodifyConvertLinestoFills ()));

  modifyExpandFill =q3NewAction (tr ("&Expand Fill..."), tr ("&Expand Fill..."), 0, this);
  modifyExpandFill->setStatusTip (tr ("Expand or inset the edges of the selected shapes"));
  modifyExpandFill->setWhatsThis (tr("&Expand Fill...\n\nExpand or inset the edges of the selected shapes"));
  connect (modifyExpandFill, SIGNAL (triggered ()), this,SLOT (slotmodifyExpandFill ()));

  modifySoftenFillEdges =q3NewAction (tr ("So&ften Fill Edges..."), tr ("So&ften Fill Edges..."), 0, this);
  modifySoftenFillEdges->setStatusTip (tr("Blend the edges of the selected shapes to transparent"));
  modifySoftenFillEdges->setWhatsThis (tr("So&ften Fill Edges...\n\nBlend the edges of the selected shapes to transparent"));
  connect (modifySoftenFillEdges, SIGNAL (triggered ()), this,SLOT (slotmodifySoftenFillEdges ()));

  modifyAddShapeHint =q3NewAction (tr ("&Add Shape Hint"), tr ("&Add Shape Hint"), QKeySequence (Qt::CTRL + Qt::Key_H), this);
  modifyAddShapeHint->setStatusTip (tr ("Add a tweening shape hint"));
  modifyAddShapeHint->setWhatsThis (tr ("&Add Shape Hint\n\nAdd a tweening shape hint"));
  connect (modifyAddShapeHint, SIGNAL (triggered ()), this, SLOT (slotmodifyAddShapeHint ()));

  modifyRemoveAllHints =q3NewAction (tr ("Re&move All Hints"), tr ("Re&move All Hints"), 0, this);
  modifyRemoveAllHints->setStatusTip (tr ("Remove all the tweening shape hints"));
  modifyRemoveAllHints->setWhatsThis (tr("Re&move All Hints\n\nRemove all the tweening shape hints"));
  connect (modifyRemoveAllHints, SIGNAL (triggered ()), this,SLOT (slotmodifyRemoveAllHints ()));

  modifySwapSymbol = q3NewAction (tr ("Swap Symbol..."), tr ("Swap Symbol..."), 0, this);
  modifySwapSymbol->setStatusTip (tr ("Replaces an instance with another symbol"));
  modifySwapSymbol->setWhatsThis (tr("Swap Symbol...\n\nReplaces an instance with another symbol"));
  connect (modifySwapSymbol, SIGNAL (triggered ()), this,SLOT (slotmodifySwapSymbol ()));

  modifyDuplicateSymbol =q3NewAction (tr ("Duplicate Symbol..."), tr ("Duplicate Symbol..."), 0, this);
  modifyDuplicateSymbol->setStatusTip (tr ("Replaces an instance with a copy of the selected symbol"));
  modifyDuplicateSymbol->setWhatsThis (tr("Duplicate Symbol...\n\nReplaces an instance with a copy of the selected symbol"));
  connect (modifyDuplicateSymbol, SIGNAL (triggered ()), this,SLOT (slotmodifyDuplicateSymbol ()));

  modifySwapBitmap =q3NewAction (tr ("Swap Bitmap..."), tr ("Swap Bitmap..."), 0, this);
  modifySwapBitmap->setStatusTip (tr ("Replaces an instance with another bitmap symbol"));
  modifySwapBitmap->setWhatsThis (tr("Swap Bitmap...\n\nReplaces an instance with another bitmap symbol"));
  connect (modifySwapBitmap, SIGNAL (triggered ()), this,SLOT (slotmodifySwapBitmap ()));

  modifyTraceBitmap = q3NewAction (tr ("Trace &Bitmap..."), tr ("Trace &Bitmap..."), 0, this);
  modifyTraceBitmap->setStatusTip (tr ("Convert a bitmap object to curves"));
  modifyTraceBitmap->setWhatsThis (tr("Trace &Bitmap...\n\nConvert a bitmap object to curves"));
  connect (modifyTraceBitmap, SIGNAL (triggered ()), this,SLOT (slotmodifyTraceBitmap ()));

  modifyFreeTransform =q3NewAction (tr ("&Free Transform"), tr ("&Free Transform"), 0, this);
  modifyFreeTransform->setStatusTip (tr("Show handles to rotate  slant  or skew the selection\nFree Transform"));
  modifyFreeTransform->setWhatsThis (tr("&Free Transform\n\nShow handles to rotate  slant  or skew the selection\nFree Transform"));
  connect (modifyFreeTransform, SIGNAL (triggered ()), this,SLOT (slotmodifyFreeTransform ()));

  modifyDistort = q3NewAction (tr ("&Distort"), tr ("&Distort"), 0, this);
  modifyDistort->setStatusTip (tr ("Show handles to distort the selection\nDistort"));
  modifyDistort->setWhatsThis (tr("&Distort\n\nShow handles to distort the selection\nDistort"));
  connect (modifyDistort, SIGNAL (triggered ()), this,SLOT (slotmodifyDistort ()));

  modifyEnvelope =q3NewAction (tr ("&Envelope"), tr ("&Envelope"), 0, this);
  modifyEnvelope->setStatusTip (tr ("Show handles to envelope the selection\nEnvelope"));
  modifyEnvelope->setWhatsThis (tr("&Envelope\n\nShow handles to envelope the selection\nEnvelope"));
  connect (modifyEnvelope, SIGNAL (triggered ()), this, SLOT (slotmodifyEnvelope ()));

  modifyScale = q3NewAction (tr ("&Scale"), tr ("&Scale"), 0, this);
  modifyScale->setStatusTip (tr("Show handles to enlarge or shrink the selection\nScale"));
  modifyScale->setWhatsThis (tr("&Scale\n\nShow handles to enlarge or shrink the selection\nScale"));
  connect (modifyScale, SIGNAL (triggered ()), this,SLOT (slotmodifyScale ()));

  modifyRotateandSkew =q3NewAction (tr ("&Rotate and Skew"), tr ("&Rotate and Skew"), 0, this);
  modifyRotateandSkew->setStatusTip (tr ("Show handles to rotate or slant the selection\nRotate and Skew"));
  modifyRotateandSkew->setWhatsThis (tr ("&Rotate and Skew\n\nShow handles to rotate or slant the selection\nRotate and Skew"));
  connect (modifyRotateandSkew, SIGNAL (triggered ()), this,SLOT (slotmodifyRotateandSkew ()));

  modifyScaleandRotate =q3NewAction (tr ("S&cale and Rotate..."), tr ("S&cale and Rotate..."), QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_S), this);
  modifyScaleandRotate->setStatusTip (tr("Scale and/or rotate the selection using numeric values"));
  modifyScaleandRotate->setWhatsThis (tr("S&cale and Rotate...\n\nScale and/or rotate the selection using numeric values"));
  connect (modifyScaleandRotate, SIGNAL (triggered ()), this,SLOT (slotmodifyScaleandRotate ()));

  modifyRotate90CW =q3NewAction (tr ("Rotate 9&0 CW"), tr ("Rotate 9&0 CW"), 0, this);
  modifyRotate90CW->setStatusTip (tr ("Rotate the selection 90 degrees to the right"));
  modifyRotate90CW->setWhatsThis (tr("Rotate 9&0 CW\n\nRotate the selection 90 degrees to the right"));
  connect (modifyRotate90CW, SIGNAL (triggered ()), this,SLOT (slotmodifyRotate90CW ()));

  modifyRotate90CCW =q3NewAction (tr ("Rotate &90 CCW"), tr ("Rotate &90 CCW"), 0, this);
  modifyRotate90CCW->setStatusTip (tr ("Rotate the selection 90 degrees to the left"));
  modifyRotate90CCW->setWhatsThis (tr("Rotate &90 CCW\n\nRotate the selection 90 degrees to the left"));
  connect (modifyRotate90CCW, SIGNAL (triggered ()), this,SLOT (slotmodifyRotate90CCW ()));

  modifyFlipVertical =q3NewAction (tr ("Flip &Vertical"), tr ("Flip &Vertical"), 0, this);
  modifyFlipVertical->setStatusTip (tr ("Flip the selection so it appears upside-down"));
  modifyFlipVertical->setWhatsThis (tr("Flip &Vertical\n\nFlip the selection so it appears upside-down"));
  connect (modifyFlipVertical, SIGNAL (triggered ()), this,SLOT (slotmodifyFlipVertical ()));

  modifyFlipHorizontal =q3NewAction (tr ("Flip &Horizontal"), tr ("Flip &Horizontal"), 0, this);
  modifyFlipHorizontal->setStatusTip (tr("Flip the selection so that the left and right sides are reversed"));
  modifyFlipHorizontal->setWhatsThis (tr("Flip &Horizontal\n\nFlip the selection so that the left and right sides are reversed"));
  connect (modifyFlipHorizontal, SIGNAL (triggered ()), this,SLOT (slotmodifyFlipHorizontal ()));


  modifyRemoveTransform =q3NewAction (tr ("Remove Trans&form"), tr ("Remove Trans&form"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_Z), this);
  modifyRemoveTransform->setStatusTip (tr("Remove any rotation or scaling from the selected objects"));
  modifyRemoveTransform->setWhatsThis (tr("Remove Trans&form\n\nRemove any rotation or scaling from the selected objects"));
  connect (modifyRemoveTransform, SIGNAL (triggered ()), this,SLOT (slotmodifyRemoveTransform ()));

  modifyBringtoFront =q3NewAction (tr ("Bring to &Front"), tr ("Bring to &Front"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_Up), this);
  modifyBringtoFront->setStatusTip (tr("Move the selected objects in to the front of their layer"));
  modifyBringtoFront->setWhatsThis (tr("Bring to &Front\n\nMove the selected objects in to the front of their layer"));
  connect (modifyBringtoFront, SIGNAL (triggered ()), this,SLOT (slotmodifyBringtoFront ()));

  modifyBringForward =q3NewAction (tr ("B&ring Forward"), tr ("B&ring Forward"), 0, this);
  modifyBringForward->setStatusTip (tr("Move the selected objects ahead of any overlaping objects"));
  modifyBringForward->setWhatsThis (tr("B&ring Forward\n\nMove the selected objects ahead of any overlaping objects"));
  connect (modifyBringForward, SIGNAL (triggered ()), this,SLOT (slotmodifyBringForward ()));

  modifySendBackward =q3NewAction (tr ("S&end Backward"), tr ("S&end Backward"), 0, this);
  modifySendBackward->setStatusTip (tr("Move the selected objects behind any overlaping objects"));
  modifySendBackward->setWhatsThis (tr("S&end Backward\n\nMove the selected objects behind any overlaping objects"));
  connect (modifySendBackward, SIGNAL (triggered ()), this,SLOT (slotmodifySendBackward ()));


  modifySendtoBack =q3NewAction (tr ("Send to &Back"), tr ("Send to &Back"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_Down), this);
  modifySendtoBack->setStatusTip (tr("Move the selected objects to the back of their layer"));
  modifySendtoBack->setWhatsThis (tr ("Send to &Back\n\nMove the selected objects to the back of their layer"));
  connect (modifySendtoBack, SIGNAL (triggered ()), this,SLOT (slotmodifySendtoBack ()));

  modifyLock =q3NewAction (tr ("&Lock"), tr ("&Lock"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_L), this);
  modifyLock->setStatusTip (tr("Lock the selected objects so that they cannot be accidentally modified"));
  modifyLock->setWhatsThis (tr("&Lock\n\nLock the selected objects so that they cannot be accidentally modified"));
  connect (modifyLock, SIGNAL (triggered ()), this,SLOT (slotmodifyLock ()));

  modifyUnlockAll =q3NewAction (tr ("&Unlock All"), tr ("&Unlock All"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::SHIFT + Qt::Key_L),this);
  modifyUnlockAll->setStatusTip (tr ("Unlock all of the locked objects in the drawing"));
  modifyUnlockAll->setWhatsThis (tr("&Unlock All\n\nUnlock all of the locked objects in the drawing"));
  connect (modifyUnlockAll, SIGNAL (triggered ()), this,SLOT (slotmodifyUnlockAll ()));

  modifyReverse = q3NewAction (tr ("&Reverse"), tr ("&Reverse"), 0, this);
  modifyReverse->setStatusTip (tr("Reverse selected frames so the animation plays backwards"));
  modifyReverse->setWhatsThis (tr("&Reverse\n\nReverse selected frames so the animation plays backwards"));
  connect (modifyReverse, SIGNAL (triggered ()), this,SLOT (slotmodifyReverse ()));

  modifySynchronizeSymbols =q3NewAction (tr ("&Synchronize Symbols"), tr ("&Synchronize Symbols"), 0,this);
  modifySynchronizeSymbols->setStatusTip (tr("Adjust the symbol first frames to loop continuously across keyframes"));
  modifySynchronizeSymbols->setWhatsThis (tr("&Synchronize Symbols\n\nAdjust the symbol first frames to loop continuously across keyframes"));
  connect (modifySynchronizeSymbols, SIGNAL (triggered ()), this,SLOT (slotmodifySynchronizeSymbols ()));

  modifyConverttoKeyframes =q3NewAction (tr ("Convert to &Keyframes"), tr ("Convert to &Keyframes"),0, this);
  modifyConverttoKeyframes->setStatusTip (tr ("Convert the selected frames to keyframes"));
  modifyConverttoKeyframes->setWhatsThis (tr("Convert to &Keyframes\n\nConvert the selected frames to keyframes"));
  connect (modifyConverttoKeyframes, SIGNAL (triggered ()), this,SLOT (slotmodifyConverttoKeyframes ()));

  modifyConverttoBlankKeyframes =q3NewAction (tr ("Convert to &Blank Keyframes"),tr ("Convert to &Blank Keyframes"), 0, this);
  modifyConverttoBlankKeyframes->setStatusTip (tr ("Convert selected frames to blank keyframes"));
  modifyConverttoBlankKeyframes->setWhatsThis (tr("Convert to &Blank Keyframes\n\nConvert selected frames to blank keyframes"));
  connect (modifyConverttoBlankKeyframes, SIGNAL (triggered ()), this,SLOT (slotmodifyConverttoBlankKeyframes ()));

  modifyGroup =q3NewAction (tr ("&Group"), tr ("&Group"), QKeySequence (Qt::CTRL + Qt::Key_G), this);
  modifyGroup->setStatusTip (tr ("Create a new group object"));
  modifyGroup->setWhatsThis (tr ("&Group\n\nCreate a new group object"));
  connect (modifyGroup, SIGNAL (triggered ()), this,SLOT (slotmodifyGroup ()));

  modifyUngroup =q3NewAction (tr ("&Ungroup"), tr ("&Ungroup"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_G), this);
  modifyUngroup->setStatusTip (tr ("Ungroup the selected group objects"));
  modifyUngroup->setWhatsThis (tr ("&Ungroup\n\nUngroup the selected group objects"));
  connect (modifyUngroup, SIGNAL (triggered ()), this,SLOT (slotmodifyUngroup ()));

  modifyBreakApart =q3NewAction (tr ("Brea&k Apart"), tr ("Brea&k Apart"),QKeySequence (Qt::CTRL + Qt::Key_B), this);
  modifyBreakApart->setStatusTip(tr("Break apart the selected objects into their component pieces"));
  modifyBreakApart->setWhatsThis(tr("Brea&k Apart\n\nBreak apart the selected objects into their component pieces"));
  connect (modifyBreakApart, SIGNAL (triggered ()), this,SLOT (slotmodifyBreakApart ()));

  modifyDistributetoLayers =q3NewAction (tr ("Distribute to Layers"), tr ("Distribute to Layers"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_D), this);
  modifyDistributetoLayers->setStatusTip (tr ("Move the selected objects onto their own layers"));
  modifyDistributetoLayers->setWhatsThis (tr("Distribute to Layers\n\nMove the selected objects onto their own layers"));
  connect (modifyDistributetoLayers, SIGNAL (triggered ()), this,SLOT (slotmodifyDistributetoLayers ()));

  textFontFace =q3NewAction (tr ("Font Face"), tr ("Font Face"), 0, this);
  textFontFace->setStatusTip (tr ("Select the Font to be used"));
  textFontFace->setWhatsThis (tr ("Font Face\n\nSelect the Font to be used"));
  connect (textFontFace, SIGNAL (triggered ()), this,SLOT (slottextFontFace ()));

  text8 = q3NewAction (tr ("&8"), tr ("&8"), 0, this);
  text8->setStatusTip (tr ("Changes text to 8 point font size"));
  text8->setWhatsThis (tr ("&8\n\nChanges text to 8 point font size"));
  connect (text8, SIGNAL (triggered ()), this, SLOT (slottext8 ()));

  text9 = q3NewAction (tr ("&9"), tr ("&9"), 0, this);
  text9->setStatusTip (tr ("Changes text to 9 point font size"));
  text9->setWhatsThis (tr ("&9\n\nChanges text to 9 point font size"));
  connect (text9, SIGNAL (triggered ()), this, SLOT (slottext9 ()));

  text10 = q3NewAction (tr ("1&0"), tr ("1&0"), 0, this);
  text10->setStatusTip (tr ("Changes text to 10 point font size"));
  text10->setWhatsThis (tr ("1&0\n\nChanges text to 10 point font size"));
  connect (text10, SIGNAL (triggered ()), this, SLOT (slottext10 ()));

  text11 = q3NewAction (tr ("1&1"), tr ("1&1"), 0, this);
  text11->setStatusTip (tr ("Changes text to 11 point font size"));
  text11->setWhatsThis (tr ("1&1\n\nChanges text to 11 point font size"));
  connect (text11, SIGNAL (triggered ()), this, SLOT (slottext11 ()));

  text12 = q3NewAction (tr ("1&2"), tr ("1&2"), 0, this);
  text12->setStatusTip (tr ("Changes text to 12 point font size"));
  text12->setWhatsThis (tr ("1&2\n\nChanges text to 12 point font size"));
  connect (text12, SIGNAL (triggered ()), this, SLOT (slottext12 ()));

  text14 = q3NewAction (tr ("1&4"), tr ("1&4"), 0, this);
  text14->setStatusTip (tr ("Changes text to 14 point font size"));
  text14->setWhatsThis (tr ("1&4\n\nChanges text to 14 point font size"));
  connect (text14, SIGNAL (triggered ()), this, SLOT (slottext14 ()));

  text18 = q3NewAction (tr ("1&8"), tr ("1&8"), 0, this);
  text18->setStatusTip (tr ("Changes text to 18 point font size"));
  text18->setWhatsThis (tr ("1&8\n\nChanges text to 18 point font size"));
  connect (text18, SIGNAL (triggered ()), this, SLOT (slottext18 ()));

  text24 = q3NewAction (tr ("2&4"), tr ("2&4"), 0, this);
  text24->setStatusTip (tr ("Changes text to 24 point font size"));
  text24->setWhatsThis (tr ("2&4\n\nChanges text to 24 point font size"));
  connect (text24, SIGNAL (triggered ()), this, SLOT (slottext24 ()));

  text36 = q3NewAction (tr ("&36"), tr ("&36"), 0, this);
  text36->setStatusTip (tr ("Changes text to 36 point font size"));
  text36->setWhatsThis (tr ("&36\n\nChanges text to 36 point font size"));
  connect (text36, SIGNAL (triggered ()), this, SLOT (slottext36 ()));

  text48 = q3NewAction (tr ("&48"), tr ("&48"), 0, this);
  text48->setStatusTip (tr ("Changes text to 48 point font size"));
  text48->setWhatsThis (tr ("&48\n\nChanges text to 48 point font size"));
  connect (text48, SIGNAL (triggered ()), this, SLOT (slottext48 ()));

  text72 = q3NewAction (tr ("&72"), tr ("&72"), 0, this);
  text72->setStatusTip (tr ("Changes text to 72 point font size"));
  text72->setWhatsThis (tr ("&72\n\nChanges text to 72 point font size"));
  connect (text72, SIGNAL (triggered ()), this, SLOT (slottext72 ()));

  text96 = q3NewAction (tr ("&96"), tr ("&96"), 0, this);
  text96->setStatusTip (tr ("Changes text to 96 point font size"));
  text96->setWhatsThis (tr ("&96\n\nChanges text to 96 point font size"));
  connect (text96, SIGNAL (triggered ()), this, SLOT (slottext96 ()));

  text120 = q3NewAction (tr ("&120"), tr ("&120"), 0, this);
  text120->setStatusTip (tr ("Changes text to 120 point font size"));
  text120->setWhatsThis (tr ("&120\n\nChanges text to 120 point font size"));
  connect (text120, SIGNAL (triggered ()), this, SLOT (slottext120 ()));

  textPlain =q3NewAction (tr ("&Plain"), tr ("&Plain"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_P), this);
  textPlain->setStatusTip (tr ("Make text plain"));
  textPlain->setWhatsThis (tr ("&Plain\n\nMake text plain"));
  connect (textPlain, SIGNAL (triggered ()), this, SLOT (slottextPlain ()));

  textBold =q3NewAction (tr ("&Bold"), tr ("&Bold"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_B), this);
  textBold->setStatusTip (tr ("Make text bold"));
  textBold->setWhatsThis (tr ("&Bold\n\nMake text bold"));
  connect (textBold, SIGNAL (triggered ()), this, SLOT (slottextBold ()));

  textItalic = q3NewAction (tr ("&Italic"), tr ("&Italic"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_I), this);
  textItalic->setStatusTip (tr ("Make text italic"));
  textItalic->setWhatsThis (tr ("&Italic\n\nMake text italic"));
  connect (textItalic, SIGNAL (triggered ()), this,SLOT (slottextItalic ()));

  textSubscript =q3NewAction (tr ("&Subscript"), tr ("&Subscript"), 0, this);
  textSubscript->setStatusTip (tr ("Make text subscript"));
  textSubscript->setWhatsThis (tr ("&Subscript\n\nMake text subscript"));
  connect (textSubscript, SIGNAL (triggered ()), this,SLOT (slottextSubscript ()));

  textSuperscript =q3NewAction (tr ("S&uperscript"), tr ("S&uperscript"), 0, this);
  textSuperscript->setStatusTip (tr ("Make text superscript"));
  textSuperscript->setWhatsThis (tr ("S&uperscript\n\nMake text superscript"));
  connect (textSuperscript, SIGNAL (triggered ()), this,SLOT (slottextSuperscript ()));

  textAlignLeft =q3NewAction (tr ("Align &Left"), tr ("Align &Left"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_L), this);
  textAlignLeft->setStatusTip (tr ("Make text flush left"));
  textAlignLeft->setWhatsThis (tr ("Align &Left\n\nMake text flush left"));
  connect (textAlignLeft, SIGNAL (triggered ()), this,SLOT (slottextAlignLeft ()));

  textAlignCenter =q3NewAction (tr ("Align &Center"), tr ("Align &Center"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_C), this);
  textAlignCenter->setStatusTip (tr ("Make text aligned center"));
  textAlignCenter->setWhatsThis (tr ("Align &Center\n\nMake text aligned center"));
  connect (textAlignCenter, SIGNAL (triggered ()), this,SLOT (slottextAlignCenter ()));

  textAlignRight = q3NewAction (tr ("Align &Right"), tr ("Align &Right"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_R), this);
  textAlignRight->setStatusTip (tr ("Make text flush right"));
  textAlignRight->setWhatsThis (tr ("Align &Right\n\nMake text flush right"));
  connect (textAlignRight, SIGNAL (triggered ()), this,SLOT (slottextAlignRight ()));

  textJustify = q3NewAction (tr ("&Justify"), tr ("&Justify"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_J), this);
  textJustify->setStatusTip (tr ("Make text align flush with left and right edges"));
  textJustify->setWhatsThis (tr("&Justify\n\nMake text align flush with left and right edges"));
  connect (textJustify, SIGNAL (triggered ()), this,SLOT (slottextJustify ()));

  textIncrease =q3NewAction (tr ("&Increase"), tr ("&Increase"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_Right), this);
  textIncrease->setStatusTip (tr ("Increase the space between letters"));
  textIncrease->setWhatsThis (tr ("&Increase\n\nIncrease the space between letters"));
  connect (textIncrease, SIGNAL (triggered ()), this,SLOT (slottextIncrease ()));

  textDecrease =q3NewAction (tr ("&Decrease"), tr ("&Decrease"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_Left), this);
  textDecrease->setStatusTip (tr ("Reduce the space between letters"));
  textDecrease->setWhatsThis (tr ("&Decrease\n\nReduce the space between letters"));
  connect (textDecrease, SIGNAL (triggered ()), this,SLOT (slottextDecrease ()));

  textReset =q3NewAction (tr ("&Reset"), tr ("&Reset"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_Up), this);
  textReset->setStatusTip (tr ("Reset the space between letters to the default"));
  textReset->setWhatsThis (tr("&Reset\n\nReset the space between letters to the default"));
  connect (textReset, SIGNAL (triggered ()), this, SLOT (slottextReset ()));

  textScrollable =q3NewAction (tr ("Scrollable"), tr ("Scrollable"), 0, this);
  textScrollable->setStatusTip (tr ("Toggle Text Field Scrollable Mode"));
  textScrollable->setWhatsThis (tr ("Scrollable\n\nToggle Text Field Scrollable Mode"));
  connect (textScrollable, SIGNAL (triggered ()), this,SLOT (slottextScrollable ()));

  controlPlay =q3NewAction (tr ("&Play"), tr ("&Play"), QKeySequence (Qt::Key_Return),this);
  controlPlay->setStatusTip (tr ("Start playing (or stop) the animation\nPlay"));
  controlPlay->setWhatsThis (tr("&Play\n\nStart playing (or stop) the animation\nPlay"));
  connect (controlPlay, SIGNAL (triggered ()), this,SLOT (slotcontrolPlay ()));

  controlRewind =q3NewAction (tr ("&Rewind"), tr ("&Rewind"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_R), this);
  controlRewind->setStatusTip (tr ("Rewind to beginning\nRewind"));
  controlRewind->setWhatsThis (tr ("&Rewind\n\nRewind to beginning\nRewind"));
  connect (controlRewind, SIGNAL (triggered ()), this,SLOT (slotcontrolRewind ()));

  controlGoToEnd =q3NewAction (tr ("&Go To End"), tr ("&Go To End"), 0, this);
  controlGoToEnd->setStatusTip (tr ("Fast forward to end\nGo To End"));
  controlGoToEnd->setWhatsThis (tr ("&Go To End\n\nFast forward to end\nGo To End"));
  connect (controlGoToEnd, SIGNAL (triggered ()), this,SLOT (slotcontrolGoToEnd ()));

  controlStepForward =q3NewAction (tr ("Step &Forward"), tr ("Step &Forward"),QKeySequence ("Ctrl+>"), this);
  controlStepForward->setStatusTip (tr("Step forward one frame\nStep Forward [>]"));
  controlStepForward->setWhatsThis (tr("Step &Forward\n\nStep forward one frame\nStep Forward [>]"));
  connect (controlStepForward, SIGNAL (triggered ()), this, SLOT (slotcontrolStepForward ()));

  controlStepBackward =q3NewAction (tr ("Step &Backward"), tr ("Step &Backward"),QKeySequence ("Ctrl+<"), this);
  controlStepBackward->setStatusTip (tr ("Step back one frame\nStep Back [<]"));
  controlStepBackward->setWhatsThis (tr("Step &Backward\n\nStep back one frame\nStep Back [<]"));
  connect (controlStepBackward, SIGNAL (triggered ()), this,SLOT (slotcontrolStepBackward ()));

  controlTestMovie =q3NewAction (tr ("Test &Movie"), tr ("Test &Movie"),QKeySequence (Qt::CTRL + Qt::Key_Return), this);
  controlTestMovie->setStatusTip (tr ("Run the movie in test mode"));
  controlTestMovie->setWhatsThis (tr ("Test &Movie\n\nRun the movie in test mode"));
  connect (controlTestMovie, SIGNAL (triggered ()), this,SLOT (slotcontrolTestMovie ()));

  controlDebugMovie =q3NewAction (tr ("&Debug Movie"), tr ("&Debug Movie"),QKeySequence (Qt::CTRL + Qt::SHIFT + Qt::Key_Return), this);
  controlDebugMovie->setStatusTip (tr ("Run the movie in test mode and open the debugger"));
  controlDebugMovie->setWhatsThis (tr("&Debug Movie\n\nRun the movie in test mode and open the debugger"));
  connect (controlDebugMovie, SIGNAL (triggered ()), this,SLOT (slotcontrolDebugMovie ()));

  controlTestScene =q3NewAction (tr ("Test &Scene"), tr ("Test &Scene"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_Return), this);
  controlTestScene->setStatusTip (tr ("Run the current scene in test mode"));
  controlTestScene->setWhatsThis (tr ("Test &Scene\n\nRun the current scene in test mode"));
  connect (controlTestScene, SIGNAL (triggered ()), this,SLOT (slotcontrolTestScene ()));

  controlLoopPlayback =q3NewAction (tr ("&Loop Playback"), tr ("&Loop Playback"), 0, this);
  controlLoopPlayback->setStatusTip (tr ("Play the animation in a continuous loop"));
  controlLoopPlayback->setWhatsThis (tr("&Loop Playback\n\nPlay the animation in a continuous loop"));
  connect (controlLoopPlayback, SIGNAL (triggered ()), this,SLOT (slotcontrolLoopPlayback ()));

  controlPlayAllScenes =q3NewAction (tr ("Play &All Scenes"), tr ("Play &All Scenes"), 0, this);
  controlPlayAllScenes->setStatusTip (tr ("Play all movie scenes as a continous animation"));
  controlPlayAllScenes->setWhatsThis (tr("Play &All Scenes\n\nPlay all movie scenes as a continous animation"));
  connect (controlPlayAllScenes, SIGNAL (triggered ()), this,SLOT (slotcontrolPlayAllScenes ()));

  controlEnableSimpleFrameActions =q3NewAction (tr ("Enable Simple Frame Act&ions"),tr ("Enable Simple Frame Act&ions"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_A), this);
  controlEnableSimpleFrameActions->setStatusTip (tr("Perform some frame actions when playing the animation"));
  controlEnableSimpleFrameActions->setWhatsThis (tr("Enable Simple Frame Act&ions\n\nPerform some frame actions when playing the animation"));
  connect (controlEnableSimpleFrameActions, SIGNAL (triggered ()), this,SLOT (slotcontrolEnableSimpleFrameActions ()));

  controlEnableSimpleButtons =q3NewAction (tr ("Enable Simple Bu&ttons"), tr ("Enable Simple Bu&ttons"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_B), this);
  controlEnableSimpleButtons->setStatusTip (tr ("Test the action of buttons in the movie"));
  controlEnableSimpleButtons->setWhatsThis (tr("Enable Simple Bu&ttons\n\nTest the action of buttons in the movie"));
  connect (controlEnableSimpleButtons, SIGNAL (triggered ()), this,SLOT (slotcontrolEnableSimpleButtons ()));

  controlMuteSounds =q3NewAction (tr ("Mute Sou&nds"), tr ("Mute Sou&nds"),QKeySequence (Qt::CTRL + Qt::ALT + Qt::Key_M), this);
  controlMuteSounds->setStatusTip (tr ("Mute the playing of sounds with the animation\n"));
  controlMuteSounds->setWhatsThis (tr("Mute Sou&nds\n\nMute the playing of sounds with the animation\n"));
  connect (controlMuteSounds, SIGNAL (triggered ()), this,SLOT (slotcontrolMuteSounds ()));

  controlEnableLivePreview =q3NewAction (tr ("Enable Live Preview"), tr ("Enable Live Preview"), 0,this);
  controlEnableLivePreview->setStatusTip (tr ("Enable Live Preview of Components"));
  controlEnableLivePreview->setWhatsThis (tr("Enable Live Preview\n\nEnable Live Preview of Components"));
  connect (controlEnableLivePreview, SIGNAL (triggered ()), this, SLOT (slotcontrolEnableLivePreview ()));

  /*windowMain = new QAction(tr( "&Main"),tr( "&Main"),0,this);
     windowMain->setStatusTip(tr("Show or hide the main toolbar"));
     windowMain->setWhatsThis(tr("&Main\n\nShow or hide the main toolbar"));
     connect(windowMain,SIGNAL(activated()), this, SLOT(slotwindowMain()));

     windowStatus = new QAction(tr( "&Status"),tr( "&Status"),0,this);
     windowStatus->setStatusTip(tr("Show or hide the status bar"));
     windowStatus->setWhatsThis(tr("&Status\n\nShow or hide the status bar"));
     connect(windowStatus,SIGNAL(activated()), this, SLOT(slotwindowStatus())); */

  windowController =q3NewAction (tr ("C&ontroller"), tr ("C&ontroller"), 0, this);
  windowController->setStatusTip (tr ("Show or hide the movie playback controller"));
  windowController->setWhatsThis (tr("C&ontroller\n\nShow or hide the movie playback controller"));
  connect (windowController, SIGNAL (triggered ()), this,SLOT (slotwindowController ()));

  windowAnswers = q3NewAction (tr ("Answers"), tr ("Answers"), 0, this);
  windowAnswers->setStatusTip (tr("Get answers to technical questions directly from Macromedia on-line"));
  windowAnswers->setWhatsThis (tr("Answers\n\nGet answers to technical questions directly from Macromedia on-line"));
  connect (windowAnswers, SIGNAL (triggered ()), this,SLOT (slotwindowAnswers ()));

  windowAlign =q3NewAction (tr ("Ali&gn"), tr ("Ali&gn"),QKeySequence (Qt::CTRL + Qt::Key_K), this);
  windowAlign->setStatusTip (tr ("Align selected objects to each other or the stage"));
  windowAlign->setWhatsThis (tr("Ali&gn\n\nAlign selected objects to each other or the stage"));
  connect (windowAlign, SIGNAL (triggered ()), this,SLOT (slotwindowAlign ()));

  windowColorMixer =q3NewAction (tr ("Color Mi&xer"), tr ("Color Mi&xer"), QKeySequence (Qt::SHIFT + Qt::Key_F2), this);
  windowColorMixer->setStatusTip (tr("Change the current color using RGB  HSB or HEX values"));
  windowColorMixer->setWhatsThis (tr("Color Mi&xer\n\nChange the current color using RGB  HSB or HEX values"));
  connect (windowColorMixer, SIGNAL (triggered ()), this,SLOT (slotwindowColorMixer ()));

  windowColorSwatches =q3NewAction (tr ("Color S&watches"), tr ("Color S&watches"),QKeySequence (Qt::SHIFT + Qt::Key_F3), this);
  windowColorSwatches->setStatusTip (tr ("Select colors from swatches and manage swatches"));
  windowColorSwatches->setWhatsThis (tr("Color S&watches\n\nSelect colors from swatches and manage swatches"));
  connect (windowColorSwatches, SIGNAL (triggered ()), this,SLOT (slotwindowColorSwatches ()));

  windowInfo =q3NewAction (tr ("&Info"), tr ("&Info"),QKeySequence (Qt::CTRL + Qt::Key_I), this);
  windowInfo->setStatusTip (tr("Show or change the properties and position of the selected object"));
  windowInfo->setWhatsThis (tr("&Info\n\nShow or change the properties and position of the selected object"));
  connect (windowInfo, SIGNAL (triggered ()), this,SLOT (slotwindowInfo ()));

  windowScene = q3NewAction (tr ("Sc&ene"), tr ("Sc&ene"),QKeySequence (Qt::CTRL + Qt::Key_U), this);
  windowScene->setStatusTip (tr("Show or change a list of the scenes in the current movie"));
  windowScene->setWhatsThis (tr("Sc&ene\n\nShow or change a list of the scenes in the current movie"));
  connect (windowScene, SIGNAL (triggered ()), this,SLOT (slotwindowScene ()));

  windowTransform =q3NewAction (tr ("&Transform"), tr ("&Transform"),QKeySequence (Qt::CTRL + Qt::Key_T), this);
  windowTransform->setStatusTip (tr("Scale and/or rotate the selection using numeric values"));
  windowTransform->setWhatsThis (tr("&Transform\n\nScale and/or rotate the selection using numeric values"));
  connect (windowTransform, SIGNAL (triggered ()), this,SLOT (slotwindowTransform ()));

  windowActions =q3NewAction (tr ("&Actions"), tr ("&Actions"), QKeySequence (Qt::Key_F2), this);
  windowActions->setStatusTip (tr ("Show or hide the Actions panel"));
  windowActions->setWhatsThis (tr ("&Actions\n\nShow or hide the Actions panel"));
  connect (windowActions, SIGNAL (triggered ()), this,SLOT (slotwindowActions ()));

  windowDebugger = q3NewAction (tr ("&Debugger"), tr ("&Debugger"),QKeySequence (Qt::SHIFT + Qt::Key_F4), this);
  windowDebugger->setStatusTip (tr ("Show or hide the Debugger"));
  windowDebugger->setWhatsThis (tr ("&Debugger\n\nShow or hide the Debugger"));
  connect (windowDebugger, SIGNAL (triggered ()), this,SLOT (slotwindowDebugger ()));

  windowMovieExplorer =q3NewAction (tr ("&Movie Explorer"), tr ("&Movie Explorer"),QKeySequence (Qt::Key_F4), this);
  windowMovieExplorer->setStatusTip (tr ("Show or hide the Movie Explorer"));
  windowMovieExplorer->setWhatsThis (tr ("&Movie Explorer\n\nShow or hide the Movie Explorer"));
  connect (windowMovieExplorer, SIGNAL (triggered ()), this,SLOT (slotwindowMovieExplorer ()));

  windowReference =q3NewAction (tr ("Re&ference"), tr ("Re&ference"),QKeySequence (Qt::SHIFT + Qt::Key_F1), this);
  windowReference->setStatusTip (tr ("Show or hide the Reference panel"));
  windowReference->setWhatsThis (tr ("Re&ference\n\nShow or hide the Reference panel"));
  connect (windowReference, SIGNAL (triggered ()), this,SLOT (slotwindowReference ()));

  windowOutput =q3NewAction (tr ("O&utput"), tr ("O&utput"),QKeySequence (Qt::SHIFT + Qt::Key_F9), this);
  windowOutput->setStatusTip (tr ("Display output information when testing a movie"));
  windowOutput->setWhatsThis (tr("O&utput\n\nDisplay output information when testing a movie"));
  connect (windowOutput, SIGNAL (triggered ()), this,SLOT (slotwindowOutput ()));

  windowAccessibility =q3NewAction (tr ("Acce&ssibility"), tr ("Acce&ssibility"),QKeySequence (Qt::Key_F9), this);
  windowAccessibility->setStatusTip (tr("Apply accessibility information to selected objects on the stage"));
  windowAccessibility->setWhatsThis (tr("Acce&ssibility\n\nApply accessibility information to selected objects on the stage"));
  connect (windowAccessibility, SIGNAL (triggered ()), this,SLOT (slotwindowAccessibility ()));

  windowComponents =q3NewAction (tr ("&Components"), tr ("&Components"),QKeySequence (Qt::Key_F11), this);
  windowComponents->setStatusTip (tr ("Macromedia Component Widgets"));
  windowComponents->setWhatsThis (tr ("&Components\n\nMacromedia Component Widgets"));
  connect (windowComponents, SIGNAL (triggered ()), this,SLOT (slotwindowComponents ()));

  windowComponentParameters =q3NewAction (tr ("Component Paramete&rs"), tr ("Component Paramete&rs"),QKeySequence (Qt::SHIFT + Qt::Key_F11), this);
  windowComponentParameters->setStatusTip (tr("Show or change the parameters associated with the selected movie clip"));
  windowComponentParameters->setWhatsThis (tr("Component Paramete&rs\n\nShow or change the parameters associated with the selected movie clip"));
  connect (windowComponentParameters, SIGNAL (triggered ()), this,SLOT (slotwindowComponentParameters ()));

  windowLibrary =q3NewAction (tr ("&Library"), tr ("&Library"),QKeySequence (Qt::CTRL + Qt::Key_L), this);
  windowLibrary->setStatusTip (tr ("Show the floating library window for this document"));
  windowLibrary->setWhatsThis (tr("&Library\n\nShow the floating library window for this document"));
  connect (windowLibrary, SIGNAL (triggered ()), this,SLOT (slotwindowLibrary ()));

  windowlibraries = q3NewAction (tr ("libraries"), tr ("libraries"), 0, this);
  windowlibraries->setStatusTip (tr ("Open a library window"));
  windowlibraries->setWhatsThis (tr ("libraries\n\nOpen a library window"));
  connect (windowlibraries, SIGNAL (triggered ()), this,SLOT (slotwindowlibraries ()));

  windowPanelSets = q3NewAction (tr ("Panel Sets"), tr ("Panel Sets"), 0, this);
  windowPanelSets->setStatusTip (tr ("Open panel set"));
  windowPanelSets->setWhatsThis (tr ("Panel Sets\n\nOpen panel set"));
  connect (windowPanelSets, SIGNAL (triggered ()), this,SLOT (slotwindowPanelSets ()));

  windowSavePanelLayout =q3NewAction (tr ("Save Panel Layout..."), tr ("Save Panel Layout..."), 0,this);
  windowSavePanelLayout->setStatusTip (tr ("Saves the current panel layout settings"));
  windowSavePanelLayout->setWhatsThis (tr("Save Panel Layout...\n\nSaves the current panel layout settings"));
  connect (windowSavePanelLayout, SIGNAL (triggered ()), this,SLOT (slotwindowSavePanelLayout ()));

  windowCloseAllPanels =q3NewAction (tr ("Close All Panels"), tr ("Close All Panels"), 0, this);
  windowCloseAllPanels->setStatusTip (tr ("Closes all panels"));
  windowCloseAllPanels->setWhatsThis (tr ("Close All Panels\n\nCloses all panels"));
  connect (windowCloseAllPanels, SIGNAL (triggered ()), this,SLOT (slotwindowCloseAllPanels ()));

  windowNewWindow =q3NewAction (tr ("New Window"), tr ("&New Window"), 0, this);
  windowNewWindow->setStatusTip (tr ("Opens a new view for the current document"));
  windowNewWindow->setWhatsThis (tr("New Window\n\nOpens a new view for the current document"));
  connect (windowNewWindow, SIGNAL (triggered ()), this, SLOT (slotWindowNewWindow ()));

  windowCascade = q3NewAction (tr ("Cascade"), tr ("&Cascade"), 0, this);
  windowCascade->setStatusTip (tr ("Cascades all windows"));
  windowCascade->setWhatsThis (tr ("Cascade\n\nCascades all windows"));
  connect (windowCascade, SIGNAL (triggered ()), pWorkspace, SLOT (cascadeSubWindows ()));

  windowTile = q3NewAction (tr ("Tile"), tr ("&Tile"), 0, this);
  windowTile->setStatusTip (tr ("Tiles all windows"));
  windowTile->setWhatsThis (tr ("Tile\n\nTiles all windows"));
  connect (windowTile, SIGNAL (triggered ()), pWorkspace, SLOT (tileSubWindows ()));

  windowProperties =q3NewAction (tr ("Properties"), tr ("&Properties"),QKeySequence (tr ("Ctrl+F3")), this, 0, true);
  windowProperties->setStatusTip (tr ("Show or hide the Property Inspector"));
  windowProperties->setWhatsThis (tr ("Properties\n\nShow or hide the Property Inspector"));
  windowProperties->setChecked (true);
  connect (windowProperties, SIGNAL (triggered ()), this,SLOT (slotWindowProperities ()));

  windowTimeLine =q3NewAction (tr ("Time Line"), tr ("Time Line"),QKeySequence (tr ("Ctrl+Alt+T")), this, 0, true);
  windowTimeLine->setStatusTip (tr("Show or hide the animation timeline and layers controls"));
  windowTimeLine->setWhatsThis (tr("Time Line\n\nShow or hide the animation timeline and layers controls"));
  windowTimeLine->setChecked (true);
  connect (windowTimeLine, SIGNAL (triggered ()), this,SLOT (slotWindowTimeLine ()));

  windowTools = q3NewAction (tr ("Tools"), tr ("Tools"), 0, this, 0, true);
  windowTools->setStatusTip (tr ("Show or hide the drawing toolbar"));
  windowTools->setWhatsThis (tr ("Tools\n\nShow or hide the drawing toolbar"));
  windowTools->setChecked (true);
  connect (windowTools, SIGNAL (triggered ()), this,SLOT (slotWindowTools ()));

  windowAction = new QActionGroup (this);
  windowAction->setExclusive (false);
  windowAction->addAction (windowNewWindow);
  windowAction->addAction (windowCascade);
  windowAction->addAction (windowTile);
  helpUsingF4l =q3NewAction (tr ("Using &F4L"), tr ("Using &F4L"), QKeySequence (Qt::Key_F1), this);
  helpUsingF4l->setStatusTip (tr ("Display the help contents"));
  helpUsingF4l->setWhatsThis (tr ("Using &F4L\n\nDisplay the help contents"));
  connect (helpUsingF4l, SIGNAL (triggered ()), this,SLOT (slothelpUsingF4l ()));

  helpActionScriptDictionary =q3NewAction (tr ("ActionScript &Dictionary"),tr ("ActionScript &Dictionary"), 0, this);
  helpActionScriptDictionary->setStatusTip (tr ("Display the ActionScript Dictionary"));
  helpActionScriptDictionary->setWhatsThis (tr("ActionScript &Dictionary\n\nDisplay the ActionScript Dictionary"));
  connect (helpActionScriptDictionary, SIGNAL (triggered ()), this,SLOT (slothelpActionScriptDictionary ()));

  helpF4lExchange =q3NewAction (tr ("F4L Exchange"), tr ("F4L Exchange"), 0, this);
  helpF4lExchange->setStatusTip (tr ("Go to the F4L Exhange web site."));
  helpF4lExchange->setWhatsThis (tr ("F4L Exchange\n\nGo to the F4L Exhange web site."));
  connect (helpF4lExchange, SIGNAL (triggered ()), this,SLOT (slothelpF4lExchange ()));

  helpManageExtensions =q3NewAction (tr ("Manage Extensions..."), tr ("Manage Extensions..."), 0,this);
  helpManageExtensions->setStatusTip (tr ("Manage Extensions"));
  helpManageExtensions->setWhatsThis (tr ("Manage Extensions...\n\nManage Extensions"));
  connect (helpManageExtensions, SIGNAL (triggered ()), this,SLOT (slothelpManageExtensions ()));

  helpSamples = q3NewAction (tr ("&Samples"), tr ("&Samples"), 0, this);
  helpSamples->setStatusTip (tr (""));
  helpSamples->setWhatsThis (tr ("&Samples\n\n"));
  connect (helpSamples, SIGNAL (triggered ()), this,SLOT (slothelpSamples ()));
  helpF4lSupportCenter = q3NewAction (tr ("F4L Support &Center"), tr ("F4L Support &Center"), 0, this);
  helpF4lSupportCenter->setStatusTip (tr ("Open your web browser to the F4L Developers Center"));
  helpF4lSupportCenter->setWhatsThis (tr("F4L Support &Center\n\nOpen your web browser to the F4L Developers Center"));
  connect (helpF4lSupportCenter, SIGNAL (triggered ()), this,SLOT (slothelpF4lSupportCenter ()));

  helpRegisterF4l = q3NewAction (tr ("&Register F4L"), tr ("&Register F4L"), 0, this);
  helpRegisterF4l->setStatusTip (tr ("Register your copy of F4L using your web browser"));
  helpRegisterF4l->setWhatsThis (tr("&Register F4L\n\nRegister your copy of F4L using your web browser"));
  connect (helpRegisterF4l, SIGNAL (triggered ()), this,SLOT (slothelpRegisterF4l ()));

  helpAboutApp = q3NewAction (tr ("About"), tr ("&About..."), 0, this);
  helpAboutApp->setStatusTip (tr ("About the application"));
  helpAboutApp->setWhatsThis (tr ("About\n\nAbout the application"));
  connect (helpAboutApp, SIGNAL (triggered ()), this,SLOT (slotHelpAbout ()));
}

void F4lmApp::initMenuBar ()
{

  ///////////////////////////////////////////////////////////////////
  // MENUBAR
  ///////////////////////////////////////////////////////////////////
  // menuBar entry pFileMenu
  pFileMenu = new QPopupMenu ();
  q3AddTo(fileNew, pFileMenu);
  q3AddTo(fileNewFromTemplate, pFileMenu);
  q3AddTo(fileOpen, pFileMenu);
  q3AddTo(fileOpenAsLibrary, pFileMenu);
  q3AddTo(fileClose, pFileMenu);
  pFileMenu->insertSeparator ();
  q3AddTo(fileSave, pFileMenu);
  q3AddTo(fileSaveAs, pFileMenu);
  q3AddTo(fileSaveAsTemplate, pFileMenu);
  q3AddTo(fileRevert, pFileMenu);
  pFileMenu->insertSeparator ();
  q3AddTo(fileImport, pFileMenu);
  q3AddTo(fileImportToLibrary, pFileMenu);
  q3AddTo(fileExportMovie, pFileMenu);
  q3AddTo(fileExportImage, pFileMenu);
  pFileMenu->insertSeparator ();
  QPopupMenu * pFileMenuPublishPreview = new QPopupMenu ();
  q3AddTo(filePublishPreview_Default, pFileMenuPublishPreview);
  q3AddTo(filePublishPreview_Flash, pFileMenuPublishPreview);
  q3AddTo(filePublishPreview_GIF, pFileMenuPublishPreview);
  q3AddTo(filePublishPreview_Html, pFileMenuPublishPreview);
  q3AddTo(filePublishPreview_JPEG, pFileMenuPublishPreview);
  q3AddTo(filePublishPreview_PNG, pFileMenuPublishPreview);
  q3AddTo(filePublishPreview_Projector, pFileMenuPublishPreview);
  q3AddTo(filePublishPreview_Quicktime, pFileMenuPublishPreview);
  pFileMenu->insertItem (tr ("Publish Preview"), pFileMenuPublishPreview);
  q3AddTo(filePublish, pFileMenu);
  pFileMenu->insertSeparator ();
  q3AddTo(filePageSetup, pFileMenu);
  q3AddTo(filePrintPreview, pFileMenu);
  q3AddTo(filePrint, pFileMenu);
  pFileMenu->insertSeparator ();
  q3AddTo(fileSend, pFileMenu);
  pFileMenu->insertSeparator ();
  pFileMenu->insertItem (tr ("burda recent olacak ama nasl?"));
  pFileMenu->insertSeparator ();
  q3AddTo(fileQuit, pFileMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry editMenu
  pEditMenu = new QPopupMenu ();
  q3AddTo(editUndo, pEditMenu);
  q3AddTo(editRedo, pEditMenu);
  pEditMenu->insertSeparator ();
  q3AddTo(editCut, pEditMenu);
  q3AddTo(editCopy, pEditMenu);
  q3AddTo(editPaste, pEditMenu);
  q3AddTo(editPasteinPlace, pEditMenu);
  q3AddTo(editPasteSpecial, pEditMenu);
  q3AddTo(editClear, pEditMenu);
  pEditMenu->insertSeparator ();
  q3AddTo(editDuplicate, pEditMenu);
  q3AddTo(editSelectAll, pEditMenu);
  q3AddTo(editDeselectAll, pEditMenu);
  pEditMenu->insertSeparator ();
  q3AddTo(editCutFrames, pEditMenu);
  q3AddTo(editCopyFrames, pEditMenu);
  q3AddTo(editPasteFrames, pEditMenu);
  q3AddTo(editClearFrames, pEditMenu);
  q3AddTo(editSelectAllFrames, pEditMenu);
  pEditMenu->insertSeparator ();
  q3AddTo(editEditSymbols, pEditMenu);
  q3AddTo(editEditSelected, pEditMenu);
  q3AddTo(editEditInPlace, pEditMenu);
  q3AddTo(editEditAll, pEditMenu);
  pEditMenu->insertSeparator ();
  q3AddTo(editPrefences, pEditMenu);
  q3AddTo(editKeyboardShourtCuts, pEditMenu);
  q3AddTo(editFontMapping, pEditMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry viewMenu
  pViewMenu = new QPopupMenu ();
  //pViewMenu->setCheckable(true);
  /*viewToolBar->addTo(pViewMenu);
     viewStatusBar->addTo(pViewMenu); */
  QPopupMenu * pViewMenuGOTO = new QPopupMenu ();
  q3AddTo(viewFirst, pViewMenuGOTO);
  q3AddTo(viewPrevious, pViewMenuGOTO);
  q3AddTo(viewNext, pViewMenuGOTO);
  q3AddTo(viewLast, pViewMenuGOTO);
  pViewMenuGOTO->insertSeparator ();
  q3AddTo(viewScenes, pViewMenuGOTO);
  pViewMenu->insertItem (tr ("&Go To"), pViewMenuGOTO);
  pViewMenu->insertSeparator ();
  q3AddTo(viewZoomIn, pViewMenu);
  q3AddTo(viewZoomOut, pViewMenu);
  QPopupMenu * pViewMenuMagnification = new QPopupMenu ();
  q3AddTo(viewMagnification25, pViewMenuMagnification);
  q3AddTo(viewMagnification50, pViewMenuMagnification);
  q3AddTo(viewMagnification100, pViewMenuMagnification);
  q3AddTo(viewMagnification200, pViewMenuMagnification);
  q3AddTo(viewMagnification400, pViewMenuMagnification);
  q3AddTo(viewMagnification800, pViewMenuMagnification);
  pViewMenuMagnification->insertSeparator ();
  q3AddTo(viewShowFrame, pViewMenuMagnification);
  q3AddTo(viewShowAll, pViewMenuMagnification);
  pViewMenu->insertItem (tr ("&Magnification"), pViewMenuMagnification);
  pViewMenu->insertSeparator ();
  q3AddTo(viewAction, pViewMenu);
  pViewMenu->insertSeparator ();
  q3AddTo(viewTimeline, pViewMenu);
  q3AddTo(viewWorkArea, pViewMenu);
  pViewMenu->insertSeparator ();
  q3AddTo(viewRulers, pViewMenu);
  QPopupMenu * pViewMenuGrid = new QPopupMenu ();
  q3AddTo(viewShowGrid, pViewMenuGrid);
  q3AddTo(viewSnaptoGrid, pViewMenuGrid);
  q3AddTo(viewEditGrid, pViewMenuGrid);
  pViewMenu->insertItem (tr ("Gri&d"), pViewMenuGrid);
  QPopupMenu * pViewMenuGuides = new QPopupMenu ();
  q3AddTo(viewShowGuides, pViewMenuGuides);
  q3AddTo(viewLockGuides, pViewMenuGuides);
  q3AddTo(viewSnaptoGuides, pViewMenuGuides);
  q3AddTo(viewEditGuides, pViewMenuGuides);
  pViewMenu->insertSeparator ();
  q3AddTo(viewSnaptoPixels, pViewMenu);
  q3AddTo(viewSnaptoObjects, pViewMenu);
  pViewMenu->insertSeparator ();
  q3AddTo(viewShowShapeHints, pViewMenu);
  pViewMenu->insertSeparator ();
  q3AddTo(viewHideEdges, pViewMenu);
  q3AddTo(viewHidePanels, pViewMenu);

  ///////////////////////////////////////////////////////////////////
  // EDIT YOUR APPLICATION SPECIFIC MENUENTRIES HERE
  pInsertMenu = new QPopupMenu ();
  q3AddTo(insertConverttoSymbol, pInsertMenu);
  q3AddTo(insertNewSymbol, pInsertMenu);
  pInsertMenu->insertSeparator ();
  q3AddTo(insertLayer, pInsertMenu);
  q3AddTo(insertLayerFolder, pInsertMenu);
  q3AddTo(insertMotionGuide, pInsertMenu);
  pInsertMenu->insertSeparator ();
  q3AddTo(insertFrame, pInsertMenu);
  q3AddTo(insertRemoveFrames, pInsertMenu);
  pInsertMenu->insertSeparator ();
  q3AddTo(insertKeyframe, pInsertMenu);
  q3AddTo(insertBlankKeyframe, pInsertMenu);
  q3AddTo(insertClearKeyframe, pInsertMenu);
  pInsertMenu->insertSeparator ();
  q3AddTo(insertCreateMotionTween, pInsertMenu);
  pInsertMenu->insertSeparator ();
  q3AddTo(insertScene, pInsertMenu);
  q3AddTo(insertRemoveScene, pInsertMenu);

  pModifyMenu = new QPopupMenu ();
  q3AddTo(modifyLayer, pModifyMenu);
  q3AddTo(modifyScene, pModifyMenu);
  q3AddTo(modifyDocument, pModifyMenu);
  pModifyMenu->insertSeparator ();
  q3AddTo(modifySmooth, pModifyMenu);
  q3AddTo(modifyStraighten, pModifyMenu);
  q3AddTo(modifyOptimize, pModifyMenu);

  QPopupMenu * pModifyMenuShape = new QPopupMenu ();
  q3AddTo(modifyConvertLinestoFills, pModifyMenuShape);
  q3AddTo(modifyExpandFill, pModifyMenuShape);
  q3AddTo(modifySoftenFillEdges, pModifyMenuShape);
  pModifyMenuShape->insertSeparator ();
  q3AddTo(modifyAddShapeHint, pModifyMenuShape);
  q3AddTo(modifyRemoveAllHints, pModifyMenuShape);
  pModifyMenu->insertItem (tr ("Sha&pe"), pModifyMenuShape);
  pModifyMenu->insertSeparator ();
  q3AddTo(modifySwapSymbol, pModifyMenu);
  q3AddTo(modifyDuplicateSymbol, pModifyMenu);
  pModifyMenu->insertSeparator ();
  q3AddTo(modifySwapBitmap, pModifyMenu);
  q3AddTo(modifyTraceBitmap, pModifyMenu);
  pModifyMenu->insertSeparator ();

  QPopupMenu * pModifyMenuTransform = new QPopupMenu ();
  q3AddTo(modifyFreeTransform, pModifyMenuTransform);
  q3AddTo(modifyDistort, pModifyMenuTransform);
  q3AddTo(modifyEnvelope, pModifyMenuTransform);
  q3AddTo(modifyScale, pModifyMenuTransform);
  q3AddTo(modifyRotateandSkew, pModifyMenuTransform);
  q3AddTo(modifyScaleandRotate, pModifyMenuTransform);
  pModifyMenuTransform->insertSeparator ();
  q3AddTo(modifyRotate90CW, pModifyMenuTransform);
  q3AddTo(modifyRotate90CCW, pModifyMenuTransform);
  pModifyMenuTransform->insertSeparator ();
  q3AddTo(modifyFlipVertical, pModifyMenuTransform);
  q3AddTo(modifyFlipHorizontal, pModifyMenuTransform);
  pModifyMenuTransform->insertSeparator ();
  q3AddTo(modifyRemoveTransform, pModifyMenuTransform);
  pModifyMenu->insertItem (tr ("&Transform"), pModifyMenuTransform);

  QPopupMenu * pModifyMenuArrange = new QPopupMenu ();
  q3AddTo(modifyBringtoFront, pModifyMenuArrange);
  q3AddTo(modifyBringForward, pModifyMenuArrange);
  q3AddTo(modifySendBackward, pModifyMenuArrange);
  q3AddTo(modifySendtoBack, pModifyMenuArrange);
  pModifyMenuArrange->insertSeparator ();
  q3AddTo(modifyLock, pModifyMenuArrange);
  q3AddTo(modifyUnlockAll, pModifyMenuArrange);
  pModifyMenuArrange->insertSeparator ();
  pModifyMenu->insertItem (tr ("&Arrange"), pModifyMenuArrange);
  pModifyMenu->insertSeparator ();

  QPopupMenu * pModifyMenuFrames = new QPopupMenu ();
  q3AddTo(modifyReverse, pModifyMenuFrames);
  q3AddTo(modifySynchronizeSymbols, pModifyMenuFrames);
  q3AddTo(modifyConverttoKeyframes, pModifyMenuFrames);
  q3AddTo(modifyConverttoBlankKeyframes, pModifyMenuFrames);
  pModifyMenu->insertItem (tr ("Fram&es"), pModifyMenuFrames);
  pModifyMenu->insertSeparator ();
  q3AddTo(modifyGroup, pModifyMenu);
  q3AddTo(modifyUngroup, pModifyMenu);
  pModifyMenu->insertSeparator ();
  q3AddTo(modifyBreakApart, pModifyMenu);
  q3AddTo(modifyDistributetoLayers, pModifyMenu);
  pTextMenu = new QPopupMenu ();
  QPopupMenu * pTextMenuFont = new QPopupMenu ();
  //textFontFace->addTo(pTextMenuFont);
  QStringList familyNames;
  QFontDatabase fdb;
  QStringList scriptNames;
  QValueList < int >scriptScripts;
  QStringList scriptSamples;
  QString family;
  QString script;
  QString style;
  QString size;
  familyNames = fdb.families ();
  QStringList newList;
  QString s;
  QStringList::Iterator it = familyNames.begin ();
  int idx = 0;
  for (; it != familyNames.end (); it++)
  {
    s = *it;
#if 0
    if (fdb.isSmoothlyScalable (*it))
      newList.append (s + "(TT)");
    else if (fdb.isBitmapScalable (*it))
      newList.append (s + "(BT)");
    else
#endif	/*
      */
      pTextMenuFont->insertItem ((s), properties->fontProperties,
                                 SLOT (slotFontChanged (int)), 0, idx++);
    newList.append (s);
  }
  //family=pTextMenuFont->currentText();

  pTextMenu->insertItem (tr ("&Font"), pTextMenuFont);
  QPopupMenu * pTextMenuSize = new QPopupMenu ();
  q3AddTo(text8, pTextMenuSize);
  q3AddTo(text9, pTextMenuSize);
  q3AddTo(text10, pTextMenuSize);
  q3AddTo(text11, pTextMenuSize);
  q3AddTo(text12, pTextMenuSize);
  q3AddTo(text14, pTextMenuSize);
  q3AddTo(text18, pTextMenuSize);
  q3AddTo(text24, pTextMenuSize);
  q3AddTo(text36, pTextMenuSize);
  q3AddTo(text48, pTextMenuSize);
  q3AddTo(text72, pTextMenuSize);
  q3AddTo(text96, pTextMenuSize);
  q3AddTo(text120, pTextMenuSize);

  pTextMenu->insertItem (tr ("&Size"), pTextMenuSize);

  QPopupMenu * pTextMenuStyle = new QPopupMenu ();
  q3AddTo(textPlain, pTextMenuStyle);

  pTextMenuStyle->insertSeparator ();
  q3AddTo(textBold, pTextMenuStyle);
  q3AddTo(textItalic, pTextMenuStyle);
  pTextMenuStyle->insertSeparator ();

  q3AddTo(textSubscript, pTextMenuStyle);
  q3AddTo(textSuperscript, pTextMenuStyle);
  pTextMenu->insertItem (tr ("St&yle"), pTextMenuStyle);
  QPopupMenu * pTextMenuAlign = new QPopupMenu ();

  q3AddTo(textAlignLeft, pTextMenuAlign);
  q3AddTo(textAlignCenter, pTextMenuAlign);
  q3AddTo(textAlignRight, pTextMenuAlign);
  q3AddTo(textJustify, pTextMenuAlign);

  pTextMenu->insertItem (tr ("&Align"), pTextMenuAlign);

  QPopupMenu * pTextMenuTracking = new QPopupMenu ();
  q3AddTo(textIncrease, pTextMenuTracking);
  q3AddTo(textDecrease, pTextMenuTracking);
  pTextMenuTracking->insertSeparator ();
  q3AddTo(textReset, pTextMenuTracking);
  pTextMenu->insertItem (tr ("&Tracking"), pTextMenuTracking);
  pTextMenu->insertSeparator ();
  q3AddTo(textScrollable, pTextMenu);

  pControlMenu = new QPopupMenu ();
  q3AddTo(controlPlay, pControlMenu);
  q3AddTo(controlRewind, pControlMenu);
  q3AddTo(controlGoToEnd, pControlMenu);
  pControlMenu->insertSeparator ();
  q3AddTo(controlStepForward, pControlMenu);
  q3AddTo(controlStepBackward, pControlMenu);
  pControlMenu->insertSeparator ();
  q3AddTo(controlTestMovie, pControlMenu);
  q3AddTo(controlDebugMovie, pControlMenu);
  q3AddTo(controlTestScene, pControlMenu);
  pControlMenu->insertSeparator ();
  q3AddTo(controlLoopPlayback, pControlMenu);
  q3AddTo(controlPlayAllScenes, pControlMenu);
  pControlMenu->insertSeparator ();
  q3AddTo(controlEnableSimpleFrameActions, pControlMenu);
  q3AddTo(controlEnableSimpleButtons, pControlMenu);
  q3AddTo(controlMuteSounds, pControlMenu);
  q3AddTo(controlEnableLivePreview, pControlMenu);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry windowMenu
  pWindowMenu = new QPopupMenu (this);

  //pWindowMenu->setCheckable(true);


  connect (pWindowMenu, SIGNAL (aboutToShow ()), this,
           SLOT (windowMenuAboutToShow ()));


  ///////////////////////////////////////////////////////////////////
  // menuBar entry helpMenu
  pHelpMenu = new QPopupMenu ();

  q3AddTo(helpUsingF4l, pHelpMenu);

  q3AddTo(helpActionScriptDictionary, pHelpMenu);

  pHelpMenu->insertSeparator ();

  q3AddTo(helpF4lExchange, pHelpMenu);
  q3AddTo(helpManageExtensions, pHelpMenu);
  q3AddTo(helpSamples, pHelpMenu);

  pHelpMenu->insertSeparator ();
  q3AddTo(helpF4lSupportCenter, pHelpMenu);
  q3AddTo(helpRegisterF4l, pHelpMenu);
  pHelpMenu->insertSeparator ();
  q3AddTo(helpAboutApp, pHelpMenu);
  pHelpMenu->insertSeparator ();
  pHelpMenu->insertItem (tr ("What's &This"), this, SLOT (whatsThis ()),
                         QKeySequence (Qt::SHIFT + Qt::Key_F1));

  pFileMenu->setTitle (tr ("&File"));
  pEditMenu->setTitle (tr ("&Edit"));
  pViewMenu->setTitle (tr ("&View"));
  pInsertMenu->setTitle (tr ("&Insert"));
  pModifyMenu->setTitle (tr ("&Modify"));
  pTextMenu->setTitle (tr ("&Text"));
  pControlMenu->setTitle (tr ("&Control"));
  pWindowMenu->setTitle (tr ("&Window"));
  pHelpMenu->setTitle (tr ("&Help"));
  menuBar()->addMenu (pFileMenu);
  menuBar()->addMenu (pEditMenu);
  menuBar()->addMenu (pViewMenu);
  menuBar()->addMenu (pInsertMenu);
  menuBar()->addMenu (pModifyMenu);
  menuBar()->addMenu (pTextMenu);
  menuBar()->addMenu (pControlMenu);
  menuBar()->addMenu (pWindowMenu);
  menuBar()->addMenu (pHelpMenu);
}

void F4lmApp::initToolBar ()
{
  ///////////////////////////////////////////////////////////////////
  // TOOLBAR
  fileToolbar = new QToolBar (tr ("file operations"), this);
  q3AddTo(fileNew, fileToolbar);
  q3AddTo(fileOpen, fileToolbar);
  q3AddTo(fileSave, fileToolbar);
  fileToolbar->addSeparator ();
  fileToolbar->addAction (QWhatsThis::createAction (this));
}

void F4lmApp::initStatusBar ()
{
  ///////////////////////////////////////////////////////////////////
  //STATUSBAR
  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::initView ()
{
  ////////////////////////////////////////////////////////////////////
  // set the main widget here
  QVBox * view_back = new QVBox (this);
  view_back->setFrameStyle (QFrame::StyledPanel | QFrame::Sunken);
  pWorkspace = new QWorkspace (view_back);
  setCentralWidget (view_back);
}

void F4lmApp::createClient (F4lmDoc * doc)
{
  F4lmView * ex = slotCurrentView ();
  F4lmView * w = new F4lmView (doc, pWorkspace, 0, WDestructiveClose);
  QMdiSubWindow * sub = pWorkspace->addWindow (w);
  sub->setAttribute (Qt::WA_DeleteOnClose);
  sub->installEventFilter (this);
  w->installEventFilter (this);
  if (ex)
  {
    w->defObjID = ex->defObjID;
    w->defObjCOLOR = ex->defObjCOLOR;
  }
  doc->addView (w);
  if (pWorkspace->windowList ().count () <= 1)
    sub->showMaximized ();
  else
    w->show ();
}



void F4lmApp::openDocumentFile (const QString &file)
{
  statusBar ()->showMessage (tr ("Opening file..."));

  F4lmDoc * doc;
  // check, if document already open. If yes, set the focus to the first view
  for (doc = pDocList->first (); doc != nullptr; doc = pDocList->next ())
  {
    if (doc->pathName () == file)
    {
      F4lmView * view = doc->firstView ();
      view->setFocus ();
      return;
    }
  }

  doc = new F4lmDoc ();
  pDocList->append (doc);
  doc->newDocument ();

  // Creates an untitled window if file is empty
  if (file.isEmpty ())
  {
    untitledCount += 1;
    QString fileName = QString (tr ("Untitled%1")).arg (untitledCount);
    doc->setPathName (fileName);
    doc->setTitle (fileName);
  }
  // Open the file
  else
  {
    if (!doc->openDocument (file))
    {
      QMessageBox::critical (this, tr ("Error !"),
                             tr ("Could not open document !"));
      delete doc;
      return;
    }
  }
  // create the window
  createClient (doc);

  statusBar ()->showMessage (tr ("Ready."));
}

bool F4lmApp::queryExit ()
{
  QMessageBox::StandardButton exit =
    QMessageBox::information (this, tr ("Quit..."),
                              tr ("Do your really want to quit?"),
                              QMessageBox::Ok | QMessageBox::Cancel);

  if (exit == QMessageBox::Ok)
  {}
  else
  {}

  return (exit == QMessageBox::Ok);
}

bool F4lmApp::eventFilter (QObject * object, QEvent * event)
{
  if ((event->type () == QEvent::Close) && (object != this))
  {
    QCloseEvent * e = (QCloseEvent *) event;
    F4lmView * pView = qobject_cast<F4lmView *> (object);
    if (!pView)
    {
      QMdiSubWindow * sub = qobject_cast<QMdiSubWindow *> (object);
      if (sub)
        pView = qobject_cast<F4lmView *> (sub->widget ());
    }
    if (!pView)
      return QWidget::eventFilter (object, event);

    F4lmDoc * pDoc = pView->getDocument ();
    if (pDoc->canCloseFrame (pView))
    {
      pDoc->removeView (pView);
      if (!pDoc->firstView ())
        pDocList->remove
        (pDoc);
      e->accept ();
    }
    else
      e->ignore ();
  }

  return QWidget::eventFilter (object, event);	// standard event processing
}



/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


void F4lmApp::slotFileNew ()
{
  statusBar ()->showMessage (tr ("Creating new file..."));
  openDocumentFile ();
  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotfileNewFromTemplate (void) {}

void F4lmApp::slotFileOpen ()
{
  statusBar ()->showMessage (tr ("Opening file..."));

  QString fileName = QFileDialog::getOpenFileName (this);
  if (!fileName.isEmpty ())
  {
    openDocumentFile (fileName);
  }

  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotFileSave ()
{
  statusBar ()->showMessage (tr ("Saving file..."));

  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
  {
    F4lmDoc * doc = m->getDocument ();
    if (doc->title ().contains (tr ("Untitled")))
      slotFileSaveAs ();
    else
      if (!doc->saveDocument (doc->pathName ()))
        QMessageBox::critical (this, tr ("I/O Error !"),
                               tr ("Could not save the current document !"));
  }

  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotFileSaveAs ()
{
  statusBar ()->showMessage (tr ("Saving file under new filename..."));

  QString fn = QFileDialog::getSaveFileName (this);
  if (!fn.isEmpty ())
  {
    F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
    if (m)
    {
      F4lmDoc * doc = m->getDocument ();
      if (!doc->saveDocument (fn))
      {
        QMessageBox::critical (this, tr ("I/O Error !"),
                               tr
                               ("Could not save the current document !"));
        return;
      }
      doc->changedViewList ();
    }
  }

  statusBar ()->showMessage (tr ("Ready."));
}


void F4lmApp::slotFileClose ()
{
  statusBar ()->showMessage (tr ("Closing file..."));

  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
  {
    F4lmDoc * doc = m->getDocument ();
    doc->closeDocument ();
  }

  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotFilePrint ()
{
  statusBar ()->showMessage (tr ("Printing..."));

  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    m->print (printer);

  statusBar ()->showMessage (tr ("Ready."));
}


void F4lmApp::slotFileQuit ()
{

  statusBar ()->showMessage (tr ("Exiting application..."));

  ///////////////////////////////////////////////////////////////////
  // exits the Application
  //  if(doc->isModified())
  //  {
  //    if(queryExit())
  //    {
  //      qApp->quit();
  //    }
  //    else
  //    {
  //
  //    };
  //  }
  //  else
  //  {
  qApp->quit ();

  //  };

  statusBar ()->showMessage (tr ("Ready."));

}

void F4lmApp::slotEditUndo ()
{
  statusBar ()->showMessage (tr ("Reverting last action..."));

  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    //   m->undo();
    statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotEditCut ()
{
  statusBar ()->showMessage (tr ("Cutting selection..."));

  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    //  m->cut();
    statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotEditCopy ()
{
  statusBar ()->showMessage (tr ("Copying selection to clipboard..."));

  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    //  m->copy();
    statusBar ()->showMessage (tr ("Ready."));
}


void F4lmApp::slotEditPaste ()
{
  statusBar ()->showMessage (tr ("Inserting clipboard contents..."));

  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    //   m->paste();
    statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotViewToolBar (bool toggle)
{
  statusBar ()->showMessage (tr ("Toggle toolbar..."));
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off
  if (toggle == false)
  {
    fileToolbar->hide ();
  }
  else
  {
    fileToolbar->show ();
  };

  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotViewStatusBar (bool toggle)
{
  statusBar ()->showMessage (tr ("Toggle statusbar..."));
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off
  if (toggle == false)
  {
    statusBar ()->hide ();
  }
  else
  {
    statusBar ()->show ();
  }

  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotWindowNewWindow ()
{
  statusBar ()->showMessage (tr ("Opening new document view..."));
  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
  {
    F4lmDoc * doc = m->getDocument ();
    createClient (doc);
  }
  statusBar ()->showMessage (tr ("Ready."));
}

void F4lmApp::slotHelpAbout ()
{
  QMessageBox::about (this, tr ("About..."),
                      tr ("F4lm\nVersion BETA"
                          "\n(c) 2005 by �kan Pakdil"));
}

void F4lmApp::slotStatusHelpMsg (const QString & text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  statusBar ()->showMessage (text, 2000);
}

void F4lmApp::windowMenuAboutToShow ()
{
  pWindowMenu->clear ();
  q3AddTo(windowNewWindow, pWindowMenu);
  pWindowMenu->insertSeparator ();

  QPopupMenu * pWindowMenuToolbars = new QPopupMenu ();
  q3AddTo(viewToolBar, pWindowMenuToolbars);
  q3AddTo(viewStatusBar, pWindowMenuToolbars);
  q3AddTo(windowController, pWindowMenuToolbars);
  pWindowMenu->insertItem ("T&oolbars", pWindowMenuToolbars);
  q3AddTo(windowTools, pWindowMenu);
  q3AddTo(windowTimeLine, pWindowMenu);
  q3AddTo(windowProperties, pWindowMenu);
  q3AddTo(windowAnswers, pWindowMenu);
  pWindowMenu->insertSeparator ();
  q3AddTo(windowAlign, pWindowMenu);
  q3AddTo(windowColorMixer, pWindowMenu);
  q3AddTo(windowColorSwatches, pWindowMenu);
  q3AddTo(windowInfo, pWindowMenu);
  q3AddTo(windowScene, pWindowMenu);
  q3AddTo(windowTransform, pWindowMenu);
  pWindowMenu->insertSeparator ();
  q3AddTo(windowActions, pWindowMenu);
  q3AddTo(windowDebugger, pWindowMenu);
  q3AddTo(windowMovieExplorer, pWindowMenu);
  q3AddTo(windowReference, pWindowMenu);
  q3AddTo(windowOutput, pWindowMenu);
  q3AddTo(windowAccessibility, pWindowMenu);
  q3AddTo(windowComponents, pWindowMenu);
  q3AddTo(windowComponentParameters, pWindowMenu);
  q3AddTo(windowLibrary, pWindowMenu);

  QPopupMenu * pWindowMenuLibraries = new QPopupMenu ();
  q3AddTo(windowlibraries, pWindowMenuLibraries);
  pWindowMenu->insertItem ("Common Li&braries", pWindowMenuLibraries);
  pWindowMenu->insertSeparator ();

  QPopupMenu * pWindowMenuPanelSets = new QPopupMenu ();
  q3AddTo(windowPanelSets, pWindowMenuPanelSets);
  pWindowMenu->insertItem ("Panel Sets", pWindowMenuPanelSets);
  q3AddTo(windowSavePanelLayout, pWindowMenu);
  q3AddTo(windowCloseAllPanels, pWindowMenu);
  pWindowMenu->insertSeparator ();
  q3AddTo(windowCascade, pWindowMenu);
  q3AddTo(windowTile, pWindowMenu);
  if (pWorkspace->windowList ().isEmpty ())
  {
    windowAction->setEnabled (false);
  }
  else
  {
    windowAction->setEnabled (true);
  }
  pWindowMenu->insertSeparator ();

  QPopupMenu * pWindowMenuWindowList = new QPopupMenu ();
  QWidgetList windows = pWorkspace->windowList ();
  for (int i = 0; i < int (windows.count ()); ++i)
  {
    int id = pWindowMenuWindowList->insertItem (QString ("&%1 ").arg (i + 1) +
             windows.at (i)->windowTitle (), this,
             SLOT (windowMenuActivated (int)));
    pWindowMenuWindowList->setItemParameter (id, i);
    pWindowMenuWindowList->setItemChecked (id,
                                           pWorkspace->activeWindow () ==
                                           windows.at (i));

  }
  pWindowMenu->insertItem ("Window list", pWindowMenuWindowList);
}

void F4lmApp::windowMenuActivated (int id)
{
  QWidget * w = pWorkspace->windowList ().at (id);
  if (w)
    w->setFocus ();
}



/** This function makes that toolbar around scene */
void F4lmApp::initDockWindows ()
{
  //setRightJustification(false);
  timeLineDockableWindow = new QDockWindow (QDockWindow::InDock, this);
  timeLineDockableWindow->setResizeEnabled (true);
  timeLineDockableWindow->setHorizontalStretchable (true);
  timeLineDockableWindow->setVerticalStretchable (true);
  q3SetCaption(timeLineDockableWindow, "Time Line");

  tl = new CTimeLine (timeLineDockableWindow, "Time Line");
  //tl->removeEventFilter(timeLineDockableWindow);
  tl->resize (width (), 150);
  tl->setMinimumHeight (150);
  //tl->setMaximumHeight(150);
  //tl->setMaximumWidth(500);
  //tl->setMinimumWidth(100);
  /*dtl->setMinimumHeight(200);
     dtl->setMaximumHeight(200);
     dtl->setMaximumWidth(10);
     dtl->setMinimumWidth(10); */
  //dtl->setFixedExtentHeight(150);
  timeLineDockableWindow->setMinimumHeight (150);
  timeLineDockableWindow->resize (width (), 150);
  timeLineDockableWindow->setWidget (tl);

  addDockWidget (Qt::TopDockWidgetArea, timeLineDockableWindow);

  QDockArea * areaFotTimeLine = timeLineDockableWindow->area ();

  areaFotTimeLine->moveDockWindow (timeLineDockableWindow, QPoint (0, 0),
                                   QRect (0, 0, width (), 150), false);

  //areaFotTimeLine->setMaximumWidth (width()-200);
  toolsDockableWindow = new QDockWindow (QDockWindow::InDock, this);
  toolsDockableWindow->setResizeEnabled (true);
  toolsDockableWindow->setHorizontalStretchable (true);
  toolsDockableWindow->setVerticalStretchable (true);
  q3SetCaption(toolsDockableWindow, "Tools");

  tools = new CTools (toolsDockableWindow, "Tools", 0, this);
  /*tools->setMinimumHeight(300);
     tools->setMaximumHeight(300);
     tools->setMaximumWidth(60);
     tools->setMinimumWidth(60);
     dtools->setMinimumHeight(300);
     dtools->setMaximumHeight(300);
     dtools->setMaximumWidth(60);
     dtools->setMinimumWidth(60); */
  toolsDockableWindow->setWidget (tools);

  addDockWidget (Qt::LeftDockWidgetArea, toolsDockableWindow);

  colorSwatchesDockableWindow = new QDockWindow (QDockWindow::InDock, this);
  //colorSwatche->setResizeEnabled(true);
  q3SetCaption(colorSwatchesDockableWindow, "Color Swatches");
  pColorSwatches =
    new CColorSwatches (colorSwatchesDockableWindow, "color_swatches", this);

  pColorSwatches->setMinimumHeight (125);	//130
  pColorSwatches->setMaximumHeight (125);

  pColorSwatches->setMaximumWidth (210);	//210
  pColorSwatches->setMinimumWidth (210);

  colorSwatchesDockableWindow->setWidget (pColorSwatches);

  addDockWidget (Qt::RightDockWidgetArea, colorSwatchesDockableWindow);

  colorDialogDockableWindow = new QDockWindow (QDockWindow::InDock, this);
  colorDialogDockableWindow->setResizeEnabled (true);
  q3SetCaption(colorDialogDockableWindow, "Color Mixer");

  CColorDialog * colordia = new CColorDialog (colorDialogDockableWindow, "Color Mixer", this);

  /*colordia->setMinimumHeight(300);
     colordia->setMaximumHeight(300);
     colordia->setMaximumWidth(60);
     colordia->setMinimumWidth(60);
     color->setMinimumHeight(300);
     color->setMaximumHeight(300);
     color->setMaximumWidth(60);
     color->setMinimumWidth(60); */
  colorDialogDockableWindow->setWidget (colordia);


  addDockWidget (Qt::RightDockWidgetArea, colorDialogDockableWindow);
  splitDockWidget (colorSwatchesDockableWindow, colorDialogDockableWindow, Qt::Vertical);
  propertiesDockableWindow = new QDockWindow (QDockWindow::InDock, this);
  propertiesDockableWindow->setResizeEnabled (true);
  q3SetCaption(propertiesDockableWindow, "Properties");

  properties = new CProperties (propertiesDockableWindow, "Properties");
  properties->setMinimumHeight (80);
  /* //prop->setMinimumHeight(300);
     properties->setMaximumHeight(300);
     properties->setMaximumWidth(60);
     properties->setMinimumWidth(60);
     //prop->setMaximumWidth(400);
     properties->setMinimumHeight(100); */
  //prop->setMinimumHeight(100);
  //properties->resize(80,100);
  propertiesDockableWindow->setWidget (properties);

  addDockWidget (Qt::BottomDockWidgetArea, propertiesDockableWindow);
}

/** sets the ID of default object for drawing. */
void F4lmApp::setDefObjID (int ID)
{
  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    m->defObjID = ID;
  //qDebug(QString::number(m->defObjID));
}


void F4lmApp::setDefObjCOLOR (QColor color)
{
  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    m->defObjCOLOR = color;

  if (tools->StrokeColor->isChecked ())
  {
    tools->strokpix.fill (color);
    tools->StrokeColor->setIcon (QIconSet (tools->strokpix));
  }

  if (tools->FillColor->isChecked ())
  {
    //tools->FillColor->setOn(false);
    tools->fillpix.fill (color);
    tools->FillColor->setIcon (QIconSet (tools->fillpix));
  }
}

F4lmView * F4lmApp::slotCurrentView ()
{
  F4lmView * m = (F4lmView *) pWorkspace->activeWindow ();
  if (m)
    return m;
  return 0;
}

void F4lmApp::slotWindowProperities ()
{
  if (propertiesDockableWindow->isVisible ())
    propertiesDockableWindow->hide ();
  else
    propertiesDockableWindow->show ();
}

void F4lmApp::slotWindowTools ()
{
  if (toolsDockableWindow->isVisible ())
    toolsDockableWindow->hide ();
  else
    toolsDockableWindow->show ();
}

void F4lmApp::slotWindowTimeLine ()
{
  if (timeLineDockableWindow->isVisible ())
    timeLineDockableWindow->hide ();
  else
    timeLineDockableWindow->show ();
}

void F4lmApp::fillTheStringTable ()
{
  //stringTable=new QMap<int,QString>;
  //stringTable[int]=QString

  QFile f ("string_table.txt");
  if (!f.open (QIODevice::ReadOnly))
    return;
  QTextStream t (&f);

  int i;
  QString s, temp;
  while (!t.atEnd ())
  {
    //t>>i>>s;
    s = t.readLine ();
    temp = s.section ('\t', 0, 0);
    i = temp.toInt ();
    s = s.section ('\t', 1, 1);
    s = s.remove (0, 1);
    s = s.remove (s.length () - 1, 1);
    //stringTable->insert(i, s);
    stringTable[i] = s;
    //      qDebug(QString::number(i)+" "+stringTable[i]);
    //cout<<i<<" "<<stringTable[i]<<endl;
  }
}

void F4lmApp::slotviewFirst () {}
void F4lmApp::slotviewPrevious () {}
void F4lmApp::slotviewNext () {}
void F4lmApp::slotviewLast () {}
void F4lmApp::slotviewScenes () {}
void F4lmApp::slotviewZoomIn () {}
void F4lmApp::slotviewZoomOut () {}
void F4lmApp::slotviewMagnification25 () {}
void F4lmApp::slotviewMagnification50 () {}
void F4lmApp::slotviewMagnification100 () {}
void F4lmApp::slotviewMagnification200 () {}
void F4lmApp::slotviewMagnification400 () {}
void F4lmApp::slotviewMagnification800 () {}
void F4lmApp::slotviewShowFrame () {}
void F4lmApp::slotviewShowAll () {}
void F4lmApp::slotviewOutlines () {}
void F4lmApp::slotviewFast () {}
void F4lmApp::slotviewAntialias () {}
void F4lmApp::slotviewAntialiasText () {}
void F4lmApp::slotviewTimeline () {}
void F4lmApp::slotviewWorkArea () {}
void F4lmApp::slotviewRulers () {}
void F4lmApp::slotviewShowGrid () {}
void F4lmApp::slotviewSnaptoGrid () {}
void F4lmApp::slotviewEditGrid () {}
void F4lmApp::slotviewShowGuides () {}
void F4lmApp::slotviewLockGuides () {}
void F4lmApp::slotviewSnaptoGuides () {}
void F4lmApp::slotviewEditGuides () {}
void F4lmApp::slotviewSnaptoPixels () {}
void F4lmApp::slotviewSnaptoObjects () {}
void F4lmApp::slotviewShowShapeHints () {}
void F4lmApp::slotviewHideEdges () {}
void F4lmApp::slotviewHidePanels () {}
void F4lmApp::slotviewToolBar () {}
void F4lmApp::slotviewStatusBar () {}
void F4lmApp::slotinsertConverttoSymbol () {}
void F4lmApp::slotinsertNewSymbol () {}
void F4lmApp::slotinsertLayer () {}
void F4lmApp::slotinsertLayerFolder () {}
void F4lmApp::slotinsertMotionGuide () {}
void F4lmApp::slotinsertFrame ()
{
  tl->slotInsertFrame (tl->timeLineTable->currentRow (),
                       tl->timeLineTable->currentColumn ());
}

void F4lmApp::slotinsertRemoveFrames () {}
void F4lmApp::slotinsertKeyframe () {}
void F4lmApp::slotinsertBlankKeyframe () {}
void F4lmApp::slotinsertClearKeyframe () {}
void F4lmApp::slotinsertCreateMotionTween ()
{
  ///TODO:set frame as motionTween true.
}
void F4lmApp::slotinsertScene () {}
void F4lmApp::slotinsertRemoveScene () {}
void F4lmApp::slotmodifyLayer () {}
void F4lmApp::slotmodifyScene () {}
void F4lmApp::slotmodifyDocument () {}
void F4lmApp::slotmodifySmooth () {}
void F4lmApp::slotmodifyStraighten () {}
void F4lmApp::slotmodifyOptimize () {}
void F4lmApp::slotmodifyConvertLinestoFills () {}
void F4lmApp::slotmodifyExpandFill () {}
void F4lmApp::slotmodifySoftenFillEdges () {}
void F4lmApp::slotmodifyAddShapeHint () {}
void F4lmApp::slotmodifyRemoveAllHints () {}
void F4lmApp::slotmodifySwapSymbol () {}
void F4lmApp::slotmodifyDuplicateSymbol () {}
void F4lmApp::slotmodifySwapBitmap () {}
void F4lmApp::slotmodifyTraceBitmap () {}
void F4lmApp::slotmodifyFreeTransform () {}
void F4lmApp::slotmodifyDistort () {}
void F4lmApp::slotmodifyEnvelope () {}
void F4lmApp::slotmodifyScale () {}
void F4lmApp::slotmodifyRotateandSkew () {}
void F4lmApp::slotmodifyScaleandRotate () {}
void F4lmApp::slotmodifyRotate90CW () {}
void F4lmApp::slotmodifyRotate90CCW () {}
void F4lmApp::slotmodifyFlipVertical () {}
void F4lmApp::slotmodifyFlipHorizontal () {}
void F4lmApp::slotmodifyRemoveTransform () {}
void F4lmApp::slotmodifyBringtoFront () {}
void F4lmApp::slotmodifyBringForward () {}
void F4lmApp::slotmodifySendBackward () {}
void F4lmApp::slotmodifySendtoBack () {}
void F4lmApp::slotmodifyLock () {}
void F4lmApp::slotmodifyUnlockAll () {}
void F4lmApp::slotmodifyReverse () {}
void F4lmApp::slotmodifySynchronizeSymbols () {}
void F4lmApp::slotmodifyConverttoKeyframes () {}
void F4lmApp::slotmodifyConverttoBlankKeyframes () {}
void F4lmApp::slotmodifyGroup () {}
void F4lmApp::slotmodifyUngroup () {}
void F4lmApp::slotmodifyBreakApart () {}
void F4lmApp::slotmodifyDistributetoLayers () {}
void F4lmApp::slottextFontFace () {}
void F4lmApp::slottext8 ()
{
  properties->fontProperties->size = "8";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext9 ()
{
  properties->fontProperties->size = "9";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext10 ()
{
  properties->fontProperties->size = "10";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext11 ()
{
  properties->fontProperties->size = "11";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext12 ()
{
  properties->fontProperties->size = "12";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext14 ()
{
  properties->fontProperties->size = "14";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext18 ()
{
  properties->fontProperties->size = "18";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext24 ()
{
  properties->fontProperties->size = "24";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext36 ()
{
  properties->fontProperties->size = "36";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext48 ()
{
  properties->fontProperties->size = "48";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext72 ()
{
  properties->fontProperties->size = "72";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext96 ()
{
  properties->fontProperties->size = "96";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottext120 ()
{
  properties->fontProperties->size = "120";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextPlain ()
{
  properties->fontProperties->style = "Normal";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextBold ()
{
  properties->fontProperties->style = "Bold";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextItalic ()
{
  properties->fontProperties->style = "Italic";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextSubscript ()
{
  properties->fontProperties->style = "Underline";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextSuperscript ()
{
  properties->fontProperties->style = "Strikeout";
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextAlignLeft () {}
void F4lmApp::slottextAlignCenter () {}
void F4lmApp::slottextAlignRight () {}
void F4lmApp::slottextJustify () {}
void F4lmApp::slottextIncrease ()
{
  int i = properties->fontProperties->size.toInt () + 1;
  QString s = QString::number (i);

  properties->fontProperties->size = s;
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextDecrease ()
{
  int i = properties->fontProperties->size.toInt () - 1;
  QString s = QString::number (i);

  properties->fontProperties->size = s;
  properties->fontProperties->updateCanvasFont ();
}

void F4lmApp::slottextReset () {}
void F4lmApp::slottextScrollable () {}

///////control menu
void F4lmApp::slotcontrolPlay ()
{
  for(int i=0;i<tl->layerMaxColNum;i++)
  {
  }
}
void F4lmApp::slotcontrolRewind () {}
void F4lmApp::slotcontrolGoToEnd () {}
void F4lmApp::slotcontrolStepForward () {}
void F4lmApp::slotcontrolStepBackward () {}
void F4lmApp::slotcontrolTestMovie () {}
void F4lmApp::slotcontrolDebugMovie () {}
void F4lmApp::slotcontrolTestScene () {}
void F4lmApp::slotcontrolLoopPlayback () {}
void F4lmApp::slotcontrolPlayAllScenes () {}
void F4lmApp::slotcontrolEnableSimpleFrameActions () {}
void F4lmApp::slotcontrolEnableSimpleButtons () {}
void F4lmApp::slotcontrolMuteSounds () {}
void F4lmApp::slotcontrolEnableLivePreview () {}

//window menu

void F4lmApp::slotwindowTile () {}
void F4lmApp::slotwindowCascade () {}
void F4lmApp::slotwindowMain () {}
void F4lmApp::slotwindowStatus () {}
void F4lmApp::slotwindowController () {}
void F4lmApp::slotwindowAnswers () {}
void F4lmApp::slotwindowAlign () {}
void F4lmApp::slotwindowColorMixer () {}
void F4lmApp::slotwindowColorSwatches () {}
void F4lmApp::slotwindowInfo () {}
void F4lmApp::slotwindowScene () {}
void F4lmApp::slotwindowTransform () {}
void F4lmApp::slotwindowActions () {}
void F4lmApp::slotwindowDebugger () {}
void F4lmApp::slotwindowMovieExplorer () {}
void F4lmApp::slotwindowReference () {}
void F4lmApp::slotwindowOutput () {}
void F4lmApp::slotwindowAccessibility () {}
void F4lmApp::slotwindowComponents () {}
void F4lmApp::slotwindowComponentParameters () {}
void F4lmApp::slotwindowLibrary () {}
void F4lmApp::slotwindowlibraries () {}
void F4lmApp::slotwindowPanelSets () {}
void F4lmApp::slotwindowSavePanelLayout () {}
void F4lmApp::slotwindowCloseAllPanels () {}

//////////help menu
void F4lmApp::slothelpUsingF4l () {}
void F4lmApp::slothelpActionScriptDictionary () {}
void F4lmApp::slothelpF4lExchange () {}
void F4lmApp::slothelpManageExtensions () {}
void F4lmApp::slothelpSamples () {}
void F4lmApp::slothelpF4lSupportCenter () {}

void F4lmApp::slothelpRegisterF4l ()
{
  QMessageBox::about (this, tr ("Regsiter..."),
                      tr ("I will happy if u send me that u compiled f4l and tried it out."
                          "\nozkanpakdil@users.sourceforge.net"));
}

void F4lmApp::slotfileOpenAsLibrary () {}
void F4lmApp::slotfileSaveAsTemplate () {}
void F4lmApp::slotfileRevert () {}
void F4lmApp::slotfileImport ()
{
  QFileDialog fd (this);
  fd.setWindowTitle (tr ("Import"));
  fd.setFileMode (QFileDialog::ExistingFile);
  fd.setOption (QFileDialog::DontUseNativeDialog, true);

  CFilePreview * preview = new CFilePreview (&fd);
  preview->setMinimumSize (160, 120);
  if (QGridLayout * layout = qobject_cast < QGridLayout * >(fd.layout ()))
    layout->addWidget (preview, 1, layout->columnCount (),
                       layout->rowCount () - 1, 1);

  connect (&fd, SIGNAL (currentChanged (const QString &)),
           preview, SLOT (previewPath (const QString &)));

  if (fd.exec () != QDialog::Accepted)
    return;

  QString fileName1 = fd.selectedFiles ().value (0);
  if (fileName1.isEmpty () || !slotCurrentView ())
    return;

  QCanvasSprite * s1 =
    new QCanvasSprite (new QCanvasPixmapArray (fileName1),
                       slotCurrentView ()->canvasViewer->canvas ());

  s1->moveBy (slotCurrentView ()->canvasViewer->canvas ()->width () / 2,
              slotCurrentView ()->canvasViewer->canvas ()->height () / 2);
  s1->show ();
}

void F4lmApp::slotfileImportToLibrary () {}
void F4lmApp::slotfileExportMovie ()
{
  /// @todo selected should be exported not first change line below.
  pDocList->first()->slotfileExportMovie();
}
void F4lmApp::slotfileExportImage () {}
void F4lmApp::slotfilePublishSetting () {}
void F4lmApp::slotfilePublishPreview_Default () {}
void F4lmApp::slotfilePublishPreview_Flash () {}
void F4lmApp::slotfilePublishPreview_Html () {}
void F4lmApp::slotfilePublishPreview_GIF () {}
void F4lmApp::slotfilePublishPreview_JPEG () {}
void F4lmApp::slotfilePublishPreview_PNG () {}
void F4lmApp::slotfilePublishPreview_Projector () {}
void F4lmApp::slotfilePublishPreview_Quicktime () {}
void F4lmApp::slotfilePublish () {}
void F4lmApp::slotfilePageSetup () {}
void F4lmApp::slotfilePrintPreview () {}
void F4lmApp::slotfileSend () {}
void F4lmApp::slotfileRecentFile () {}
void F4lmApp::sloteditRedo () {}
void F4lmApp::sloteditPasteinPlace () {}
void F4lmApp::sloteditPasteSpecial () {}
void F4lmApp::sloteditClear () {}
void F4lmApp::sloteditDuplicate () {}
void F4lmApp::sloteditSelectAll () {}
void F4lmApp::sloteditDeselectAll () {}
void F4lmApp::sloteditCutFrames () {}
void F4lmApp::sloteditCopyFrames () {}
void F4lmApp::sloteditPasteFrames () {}
void F4lmApp::sloteditClearFrames () {}
void F4lmApp::sloteditSelectAllFrames () {}
void F4lmApp::sloteditEditSymbols () {}
void F4lmApp::sloteditEditSelected () {}
void F4lmApp::sloteditEditInPlace () {}
void F4lmApp::sloteditEditAll () {}
void F4lmApp::sloteditPrefences () {}
void F4lmApp::sloteditKeyboardShourtCuts () {}
void F4lmApp::sloteditFontMapping () {}

/*void F4lmApp::mouseMoveEvent(QMouseEvent *e){
	QWidget::mouseMoveEvent(e);
 
	int x=mapToGlobal(e->pos()).x()
		,y=mapToGlobal(e->pos()).y();
    QPixmap p = QPixmap::grabWindow( QApplication::desktop()->winId(),  x, y, 2, 2 );
    QImage image = p.convertToImage();
 
 
	grabMouse();
	QPoint p=e->globalPos();
     QWidget *desktop = QApplication::desktop();
     QPixmap pm = QPixmap::grabWindow( desktop->winId(), p.x(), p.y(), 1, 1);
     QImage i = pm.convertToImage();
      i.pixel(0,0);
	QRgb px = i.pixel(0,0);
	qDebug(" %3d,%3d,%3d  #%02x%02x%02x",
		qRed(px), qGreen(px), qBlue(px),
		qRed(px), qGreen(px), qBlue(px));
 
}
 
void F4lmApp::mousePressEvent(QMouseEvent* e){
	//releaseMouse();
	//grabMouse();
}*/

void CFilePreview::previewUrl (const QUrl & u)
{
  previewPath (u.toLocalFile ());
}

void CFilePreview::previewPath (const QString & path)
{
  QPixmap pix (path);
  if (pix.isNull ())
    setText ("This is not a image");
  else
  {
    pix = pix.scaled (width (), height (), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap (pix);
  }
}
