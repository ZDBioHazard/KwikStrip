/**********************************************************************
* KwikStrip - An icon-based application launcher
* Copyright (C) 2006 BioHazard / Vectec Software
***********************************************************************
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 or any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
***********************************************************************
* kwik.h : Prototypes and defines for stuff
**********************************************************************/
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

 ///////////////////////
// Important defines
#define SndMsg(itm,msg,wp,lp) SendMessage(GetDlgItem(dlg,itm),msg,(WPARAM)wp,(LPARAM)lp)
#define STR_TITLE "KwikStrip 2.5 BETA 6"
#define STR_HELPURL "http://biohazard.drdteam.org/help/kwikstrip.htm"
#define KS_EXIT_RESTART 1
#define MAX_BTNS 0xFF

 ///////////////////////
// Vars struct
typedef struct{
	unsigned char btns,rows,open; // # of buttons, rows, open state and hover state
	RECT rct; // Window dimensions
	HWND wnd,arw,hvr,dwn; // Strip, Arrow, Hover position and pressed button handles
	HDC gfx; // Program graphics
	char *ini; // INI file path (ABSOLUTELY MUST BE LAST)
}VARS;extern VARS vr;

 ///////////////////////
// Config struct
typedef struct{
	unsigned char cols,infw,anim,clse,disp; // Config options
	unsigned short yoff,hkey; // Hotkey code and Y-offset
	char *skin,*fnt; // Strip skin and font name
}CONFIG;extern CONFIG cf,dcf;
#define KS_DSP_DEF 0
#define KS_DSP_TOP 1
#define KS_DSP_KEY 2

 ///////////////////////
// Metrics struct
typedef struct{
	COLORREF txc; // Text color and transparent color
	unsigned char
		txt,txa, // Text attributes
		arw,btn,ilf,ict,irt, // X
		up,hv,dn,inf, // Y
		arx,ary,bnx,bny,inx,iny,icx,icy, // Sizes
		bpx,bpy,bdx,bdy,ipx,ipy; // Button icon spacing
}METRICS;extern METRICS mx,dmx;
#define KS_TXA_BOLD 0x01
#define KS_TXA_ITALIC 0x02

 ///////////////////////
// ITEM struct
typedef struct{
	char *nam,*exe,*dir,*arg,*ico; // Item info
	unsigned char idx; // Icon index
	HICON icon; // Cached icon graphic
	HWND wnd; // Handle
}ITEM;extern ITEM *itm;

 ///////////////////////
// cfg.c
#define KS_INI_NONE -1
#define KS_INI_NOBT 0
void Cfg_DeleteItem(unsigned char idx);
signed short Config_Load();
LRESULT CALLBACK ConfigProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp);

 ///////////////////////
// strip.c
void Strip_Setup();
void Strip_Draw();
void Strip_OpenClose();
unsigned char Strip_BtnFrmWnd(HWND wnd);
void Strip_DrawBtn(HWND wnd,unsigned char st,unsigned char inf);
void Strip_Position(HWND dlg);

 ///////////////////////
// crash.c
LRESULT CALLBACK CrashProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp);

 ///////////////////////
// biolib.c
unsigned char Bio_IsFile(char *path);
unsigned char Bio_ReadINI(char *section,char *key,char **out,char *ini);

 ///////////////////////
// Resource stuff
#define IDC_STATIC -1
#define ICO_ICON 100
#define BMP_ABOUT 101
#define BMP_GFX 102
// Common buttons
#define BTN_OK 1000
#define BTN_CANCEL 1001
// Config dialog
#define DLG_CONFIG 500
	// Items
	#define LST_ITEMS 1002
	#define BTN_ADD 1003
	#define BTN_DELETE 1004
	#define BTN_UP 1005
	#define BTN_DOWN 1006
// Strip options
	#define CHK_CLOSE 1007
	#define CHK_ANIM 1008
	#define SLD_ANIM 1009
	#define EDT_COLS 1010
	#define SPN_COLS 1011
	#define EDT_INFSZ 1012
	#define SPN_INFSZ 1013
	#define EDT_YOFF 1014
	#define SPN_YOFF 1015
	#define EDT_SKIN 1016
	#define BTN_SKIN 1017
	#define RAD_DEFZ 1018
	#define RAD_ONTOP 1019
	#define RAD_HOTKEY 1020
	#define HTK_HOTKEY 1021
// Properties dialog
#define DLG_PROP 501
	#define EDT_NAME 1022
	#define EDT_PATH 1023
	#define BTN_PATH 1024
	#define EDT_DIR 1025
	#define EDT_ARGS 1026
	#define EDT_ICON 1027
	#define EDT_INDEX 1028
	#define SPN_INDEX 1029
	#define BTN_ICON 1030
	#define ICO_ICOPRV 1031
// Crash dialog
#define DLG_CRASH 502
	#define EDT_DUMP 1032
	#define BTN_SELECT 1033
	#define BTN_FORUM 1034
#define DLG_ARGS 503
#define DLG_ABOUT 504
// Menu
#define MNU_MENU 200
	#define MNU_OPNCLS 2000
	#define MNU_LAUNCH 2001
	#define MNU_ARGS 2002
	#define MNU_DIR 2003
	#define MNU_FOLDER 2004
	#define MNU_CONFIG 2005
	#define MNU_ABOUT 2006
	#define MNU_HELP 2007
	#define MNU_EXIT 2008
