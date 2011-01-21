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
* strip.c : Strip initialization and drawing functions
**********************************************************************/
#include "kwik.h"

 ////////////////////////////////////////////////////////////
// Strip_Setup : Sizes and masks the strip and loads the skin
/// ARGS ////////////////////////////////////////////////////
// dlg : Handle of the window to apply all this to
void Strip_Setup(HWND dlg){
	unsigned char i=0,q=0,r=0;
	unsigned int hky=0;
	HBITMAP bmp;
	HRGN rgn;
//
// Get the number of rows
	if(vr.btns%cf.cols){
		vr.rows=(vr.btns/cf.cols)+1;
	}else{vr.rows=vr.btns/cf.cols;}
// Size the window
	if(vr.btns>cf.cols){ // Calculate row count
		vr.rct.right=mx.arx+(cf.cols*mx.bnx);
		vr.rct.bottom=mx.iny+(mx.bny*vr.rows);
	}else{ // 1 row
		vr.rct.right=mx.arx+(vr.btns*mx.bnx);
		vr.rct.bottom=mx.bny+mx.iny;
	}
// Mask out the un-needed part
	rgn=CreateRectRgn(0,0,vr.rct.right,vr.rct.bottom);
	CombineRgn(rgn,rgn,CreateRectRgn(0,mx.ary,mx.arx,vr.rct.bottom),RGN_DIFF);
	if(vr.btns>cf.infw&&cf.cols>cf.infw){ // Only mask out the info section if there is space to hide
		CombineRgn(rgn,rgn,CreateRectRgn(0,vr.rct.bottom-mx.iny,vr.rct.right-(mx.bnx*cf.infw),vr.rct.bottom),RGN_DIFF);
	}
	SetWindowRgn(dlg,rgn,1);
// Move to the right side
	Strip_Position(dlg);
// Load the theme stuff
	// Skin
	vr.gfx=CreateCompatibleDC(GetDC(dlg));
/*
 *  The code below creates a memory leak
 * \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/
 */
	if(Bio_IsFile(cf.skin)){
		bmp=LoadImage(GetModuleHandle(0),cf.skin,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	}else{bmp=LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(BMP_GFX));}
	DeleteObject(SelectObject(vr.gfx,bmp));
	DeleteObject(bmp);
	// Font
	DeleteObject(SelectObject(GetDC(dlg),CreateFont(mx.txt,0,0,0,(mx.txa&KS_TXA_BOLD)?FW_BOLD:0,(mx.txa&KS_TXA_ITALIC)?1:0,0,0,0,0,0,0,0,cf.fnt)));
/*
 * /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\
 *  The code above creates a memory leak
 */
	SetTextColor(GetDC(dlg),mx.txc);
	SetBkMode(GetDC(dlg),TRANSPARENT);
	SetStretchBltMode(GetDC(dlg),HALFTONE);
// Create buttons
	vr.arw=CreateWindow("bncls","Arrow",WS_VISIBLE|WS_CHILD,0,0,mx.arx,mx.ary,dlg,0,GetModuleHandle(0),0);
	for(i=0;i<vr.btns;i++){
		itm[i].wnd=CreateWindow("bncls","Button",WS_VISIBLE|WS_CHILD,mx.arx+(mx.bnx*q),mx.bny*r,mx.bnx,mx.bny,dlg,0,GetModuleHandle(0),0);
		q++;if(q>=cf.cols){r++;q=0;}
	}
// Set hotkey
	if(cf.disp==KS_DSP_KEY){
		// Translate the modifier key types
		if(HIBYTE(cf.hkey)&HOTKEYF_ALT){hky|=MOD_ALT;}
		if(HIBYTE(cf.hkey)&HOTKEYF_SHIFT){hky|=MOD_SHIFT;}
		if(HIBYTE(cf.hkey)&HOTKEYF_CONTROL){hky|=MOD_CONTROL;}
		RegisterHotKey(dlg,1,hky,LOBYTE(cf.hkey));
	}
}

 ////////////////////////////////////////////////////////
// Strip_InfoBox : Draws the infobox element of the strip
/// ARGS ////////////////////////////////////////////////
// text : Text to print in the window
/// REMARKS /////////////////////////////////////////////
// If [text] is 0 or blank, no string will be drawn
void Strip_InfoBox(char *text){
	unsigned short i,stx,sty,dst; // Infobox start
	RECT rct;
//
	if(!cf.infw){return;} // If there is no infobox, just exit
// Set variables
	// Start locations
	sty=mx.bny*vr.rows;
	if(vr.btns>=cf.infw&&cf.cols>=cf.infw){
		stx=vr.rct.right-(mx.bnx*cf.infw)+mx.inx;
		rct.left=stx+(mx.ipx*2);
	}else{stx=mx.arx+mx.inx;}
	dst=(vr.rct.right-mx.inx)/mx.inx;
	// Text boundary
	rct.left=(stx-mx.inx)+(mx.ipx*2);
	rct.top=sty+mx.ipy;
	rct.right=vr.rct.right-(mx.ipx*2);
	rct.bottom=vr.rct.bottom-mx.ipy;
// Draw the box
	BitBlt(GetDC(vr.wnd),stx-mx.inx,sty,mx.inx,mx.iny,vr.gfx,mx.ilf,mx.inf,SRCCOPY);
	for(i=0;i<dst;i++){BitBlt(GetDC(vr.wnd),stx+(i*mx.inx),sty,mx.inx,mx.iny,vr.gfx,mx.ict,mx.inf,SRCCOPY);}
	BitBlt(GetDC(vr.wnd),vr.rct.right-mx.inx,sty,mx.inx,mx.iny,vr.gfx,mx.irt,mx.inf,SRCCOPY);
// Draw the text (if any)
	if(text&&strlen(text)){DrawText(GetDC(vr.wnd),text,strlen(text),&rct,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_NOPREFIX);}
}

 /////////////////////////////////////////
// Strip_Draw : Draws the strip background
/// REMARKS //////////////////////////////
// This will erase the infobox
void Strip_Draw(){
	unsigned short stx,sty;
	unsigned char i,num;
//
// Draw the blanks (if needed)
	if(vr.btns>cf.cols&&(num=(cf.cols*vr.rows)%vr.btns)){
		stx=vr.rct.right-(num*mx.bnx);
		sty=(vr.rows-1)*mx.bny;
		for(i=0;i<num;i++){BitBlt(GetDC(vr.wnd),stx+(i*mx.bnx),sty,mx.bnx,mx.bny,vr.gfx,mx.btn,mx.up,SRCCOPY);}
	}Strip_InfoBox(STR_TITLE); // Draw infobox
}

 //////////////////////////////////////////////////////////
// Strip_BtnFrmWnd : Gets an itm index from a window handle
/// ARGS //////////////////////////////////////////////////
// dlg : Window to get the associated icon for
/// RETURN ////////////////////////////////////////////////
// itm array index for that button's information
unsigned char Strip_BtnFrmWnd(HWND wnd){
	unsigned char i=0;
//
	while(itm[i].wnd!=wnd&&i<MAX_BTNS){i++;}
	return i;
}

 ///////////////////////////////////////////////////////
// Strip_DrawBtn : Draws a button in the specified state
/// ARGS ///////////////////////////////////////////////
// wnd : Handle to the window to draw to
// st  : State to draw (from mx->)
// inf : Wether to draw the infobox
void Strip_DrawBtn(HWND wnd,unsigned char st,unsigned char inf){
	unsigned char i;
//
	if(wnd!=vr.arw){
		BitBlt(GetDC(wnd),0,0,mx.bnx,mx.bny,vr.gfx,mx.btn,st,SRCCOPY);
		i=Strip_BtnFrmWnd(wnd);
		DrawIconEx(GetDC(wnd),(st==mx.dn?mx.bdx:mx.bpx),(st==mx.dn?mx.bdy:mx.bpy),itm[i].icon,mx.icx,mx.icy,0,0,DI_NORMAL);
		if(inf){Strip_InfoBox(itm[i].nam);}
	}else{
		BitBlt(GetDC(wnd),0,0,mx.arx,mx.ary,vr.gfx,mx.arw,st,SRCCOPY);
		if(inf){Strip_InfoBox("Close Strip");}
	}
}

 ////////////////////////////////////////////////////
// Strip_OpenClose : Opens and closes the strip (duh)
void Strip_OpenClose(){
	signed long dest=vr.open?vr.rct.left+(vr.rct.right-mx.arx):vr.rct.left;
	signed short i=vr.open?vr.rct.left:vr.rct.left+(vr.rct.right-mx.arx);
//
	if(cf.anim){ // only animate if animation is turned on
		for(i;vr.open?i<=dest:i>=dest;vr.open?i+=cf.anim:(i-=cf.anim)){
			SetWindowPos(vr.wnd,0,i,vr.rct.top+cf.yoff,0,0,SWP_NOZORDER|SWP_NOSIZE);
			UpdateWindow(vr.wnd);
			Sleep(1);
		}
	}SetWindowPos(vr.wnd,0,dest,vr.rct.top+cf.yoff,0,0,SWP_NOZORDER|SWP_NOSIZE);
	vr.open=vr.open?0:1;
}

 ///////////////////////////////////////////////////////////
// Strip_Position : Puts the strip in the upper-right corner
/// ARGS ///////////////////////////////////////////////////
// dlg : Window to position
void Strip_Position(HWND dlg){
	RECT rct;
//
	SystemParametersInfo(SPI_GETWORKAREA,0,&rct,0);
	vr.rct.left=rct.right-vr.rct.right;
	vr.rct.top=(short)rct.top;
// Move the window into position
	SetWindowPos(dlg,0,vr.rct.left+(vr.rct.right-mx.arx),vr.rct.top+cf.yoff,vr.rct.right,vr.rct.bottom,SWP_NOZORDER);
// The strip should be closed now
	vr.open=0;
}
