; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "toolbar.h"
LastPage=0

ClassCount=5
Class1=CToolbarApp
Class2=CToolbarDoc
Class3=CToolbarView
Class4=CMainFrame

ResourceCount=4
Resource1=IDR_TBCOLOR
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Resource3=IDR_CONTEXTMENU
Resource4=IDD_ABOUTBOX

[CLS:CToolbarApp]
Type=0
HeaderFile=toolbar.h
ImplementationFile=toolbar.cpp
Filter=N
LastObject=CToolbarApp

[CLS:CToolbarDoc]
Type=0
HeaderFile=toolbarDoc.h
ImplementationFile=toolbarDoc.cpp
Filter=N
LastObject=ID_WIDTH_VTHIN
BaseClass=CDocument
VirtualFilter=DC

[CLS:CToolbarView]
Type=0
HeaderFile=toolbarView.h
ImplementationFile=toolbarView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CToolbarView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=toolbar.cpp
ImplementationFile=toolbar.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_VIEW_COLORBAR
Command17=ID_COLOR_BLACK
Command18=ID_COLOR_BLUE
Command19=ID_COLOR_GREEN
Command20=ID_COLOR_CYAN
Command21=ID_COLOR_RED
Command22=ID_COLOR_MAGENTA
Command23=ID_COLOR_YELLOW
Command24=ID_COLOR_WHITE
Command25=ID_WIDTH_VTHIN
Command26=ID_WIDTH_THIN
Command27=ID_WIDTH_MEDIUM
Command28=ID_WIDTH_THICK
Command29=ID_WIDTH_VTHICK
Command30=ID_APP_ABOUT
CommandCount=30

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_VIEW_COLORBAR
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_CONTEXTMENU]
Type=1
Class=?
Command1=ID_WIDTH_VTHIN
Command2=ID_WIDTH_THIN
Command3=ID_WIDTH_MEDIUM
Command4=ID_WIDTH_THICK
Command5=ID_WIDTH_VTHICK
Command6=ID_COLOR_BLACK
Command7=ID_COLOR_BLUE
Command8=ID_COLOR_GREEN
Command9=ID_COLOR_CYAN
Command10=ID_COLOR_RED
Command11=ID_COLOR_MAGENTA
Command12=ID_COLOR_YELLOW
Command13=ID_COLOR_WHITE
CommandCount=13

[TB:IDR_TBCOLOR]
Type=1
Class=?
Command1=ID_COLOR_BLACK
Command2=ID_COLOR_BLUE
Command3=ID_COLOR_GREEN
Command4=ID_COLOR_CYAN
Command5=ID_COLOR_RED
Command6=ID_COLOR_MAGENTA
Command7=ID_COLOR_YELLOW
Command8=ID_COLOR_WHITE
CommandCount=8

