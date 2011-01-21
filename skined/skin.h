/*************************************************************
* KwikStrip - An icon-based application launcher
* Copyright (C) 2006 BioHazard
*
* This program is free software; you can redistribute and/or
* modify it under the terms of the GNU General Public License
* v2 or later as published by the Free Software Foundation.
* This program is distributed in the hope that it will be
* useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE. See the GNU General Public License for details.
**************************************************************
* kwik.h : Prototypes and defines for stuff
*************************************************************/
#include <windows.h>
#include <stdio.h>
#include <io.h>

#define STR_TITLE "KwikStrip SkinEd 2.0 BETA 3"
#define STR_HELPURL "http://biohazard.drdteam.org/help/kwikstrip.htm"
#define KS_FORMAT 0x03
#define SndMsg(itm,msg,wp,lp) SendMessage(GetDlgItem(dlg,itm),msg,(WPARAM)wp,(LPARAM)lp)

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
}METRICS;
#define KS_TXA_BOLD 0x01
#define KS_TXA_ITALIC 0x02

 ///////////////////////
// Resource stuff
#define IDC_STATIC -1
#define ICO_ICON 100
#define BMP_ABOUT 101
#define DLG_ABOUT 500
// Config dialog
#define DLG_SKIN 501
	#define EDT_BMP 1000
	#define BTN_BMP 1001
	#define EDT_TITLE 1002
	#define EDT_FONT 1003
	#define EDT_MX 1004
	#define DRW_PREV 1005
// Menu
#define MNU_MENU 200
	// File
	#define MNU_NEW 2000
	#define MNU_LOAD 2001
	#define MNU_SAVE 2002
	#define MNU_EXIT 2003
	// Metrics
	#define MNU_16PX 2004
	#define MNU_32PX 2005
	#define MNU_48PX 2006
	// Help
	#define MNU_ABOUT 2007
	#define MNU_HELP 2008
