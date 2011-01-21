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
* crash.c : Handles exceptions with pretty dialogs
**********************************************************************/
#include "kwik.h"
#define BUF_LEN 512

LRESULT CALLBACK CrashProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	EXCEPTION_RECORD *exc;
	unsigned char i;
	char *tmp;
//
	case WM_INITDIALOG:
			SendMessage(dlg,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(GetModuleHandle(0),MAKEINTRESOURCE(ICO_ICON)));
			SndMsg(EDT_DUMP,WM_SETFONT,CreateFont(14,0,0,0,0,0,0,0,0,0,0,0,0,"Courier New"),0);
			SndMsg(EDT_DUMP,EM_LIMITTEXT,0,0xFFFFFFFF);
			exc=((EXCEPTION_POINTERS*)lp)->ExceptionRecord;
		// Read log contents
		__try{
			tmp=calloc(BUF_LEN,sizeof(char));
			// Display the exception code and address
				_snprintf(tmp,BUF_LEN,"%s crash report\r\nException 0x%.8X at address 0x%.8X\r\n\r\n--==Var dump==--",
							STR_TITLE,exc->ExceptionCode,exc->ExceptionAddress);
				SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
				// vr
				_snprintf(tmp,BUF_LEN,"\r\nvr (Internal Variables)\r\n btns=%.3u rows=%.3u open=%.3u\r\n rct={%d,%d,%d,%d}\r\n wnd->0x%.8X  arw->0x%.8X  hvr->0x%.8X\r\n dwn->0x%.8X  gfx->0x%.8X\r\n ini->0x%.8X=\"%s\"",
							vr.btns,vr.rows,vr.open,vr.rct.left,vr.rct.top,vr.rct.right,vr.rct.bottom,vr.wnd,vr.arw,vr.hvr,vr.dwn,vr.gfx,vr.ini,vr.ini);
				SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
				// cf
				_snprintf(tmp,BUF_LEN,"\r\ncf (User Configuration)\r\n cols=%.3u  infw=%.3u  anim=%.3u  yoff=%.5u\r\n clse=%.3u  disp=%.3u  hkey=0x%.4X\r\n skin->0x%.8X=\"%s\"\r\n fnt->0x%.8X=\"%s\"",
							cf.cols,cf.infw,cf.anim,cf.yoff,cf.clse,cf.disp,cf.hkey,cf.skin,cf.skin,cf.fnt,cf.fnt);
				SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
				// mx
				_snprintf(tmp,BUF_LEN,"\r\nmx (Skin Metrics)\r\n arw=%.3u  btn=%.3u  ilf=%.3u  ict=%.3u  irt=%.3u   up=%.3u\r\n  hv=%.3u   dn=%.3u  inf=%.3u  arx=%.3u  ary=%.3u  bnx=%.3u\r\n bny=%.3u  inx=%.3u  iny=%.3u  icx=%.3u  icy=%.3u  txt=%.3u\r\n bpx=%.3u  bpy=%.3u  bdx=%.3u  bdy=%.3u  ipx=%.3u  ipy=%.3u\r\n txa=%.3u  txc=%.6X#",
							mx.arw,mx.bnx,mx.ilf,mx.ict,mx.irt,mx.up,mx.hv,mx.dn,mx.inf,mx.arx,mx.ary,mx.bnx,mx.bny,mx.inx,mx.iny,mx.icx,mx.icy,mx.txt,mx.bpx,mx.bpy,mx.bdx,mx.bdy,mx.ipx,mx.ipy,mx.txa,mx.txc);
				SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
				// itm
				_snprintf(tmp,BUF_LEN,"\r\nitm (Item Matrix)\r\n itm->0x%.8X\r\n Attempting to dump %u items...",itm,vr.btns);
				SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
				for(i=0;i<vr.btns;i++){
					_snprintf(tmp,BUF_LEN,"\r\n [%.3u]->0x%.8X\r\n   nam->0x%.8X=\"%s\"\r\n   exe->0x%.8X=\"%s\"",
								i,&itm[i],itm[i].nam,itm[i].nam,itm[i].exe,itm[i].exe);
					SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
					if(itm[i].dir){
						_snprintf(tmp,BUF_LEN,"\r\n   dir->0x%.8X=\"%s\"",itm[i].dir,itm[i].dir);
						SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
					}
					if(itm[i].arg){
						_snprintf(tmp,BUF_LEN,"\r\n   arg->0x%.8X=\"%s\"",itm[i].arg,itm[i].arg);
						SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
					}
					if(itm[i].ico){
						_snprintf(tmp,BUF_LEN,"\r\n   ico->0x%.8X=\"%s\"",itm[i].ico,itm[i].ico);
						SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
					}
					_snprintf(tmp,BUF_LEN,"\r\n   idx=%.3u  icon->0x%.8X  wnd->0x%.8X",
								itm[i].idx,itm[i].icon,itm[i].wnd);
					SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
				}
				_snprintf(tmp,BUF_LEN,"\r\n%u items dumped...\r\n--==Crash dump complete!==--",i);
				SndMsg(EDT_DUMP,EM_REPLACESEL,0,tmp);
			free(tmp);
		}__except(EXCEPTION_EXECUTE_HANDLER){
			SndMsg(EDT_DUMP,EM_REPLACESEL,0,"\r\n\r\nThe debug dump function broke.\r\nIsin't it great when the function to help find errors breaks? ;)");
		}
		SetFocus(GetDlgItem(dlg,BTN_SELECT)); // So the text gets de-selected
	break;
	case WM_COMMAND:switch(HIWORD(wp)){
		case BN_CLICKED:switch(LOWORD(wp)){
			case BTN_SELECT:
				SetFocus(GetDlgItem(dlg,EDT_DUMP));
				SndMsg(EDT_DUMP,EM_SETSEL,0,-1);
			break;
			case BTN_FORUM:ShellExecute(dlg,"open","http://forum.drdteam.org/","","",SW_NORMAL);break;
			case WM_DESTROY:case BTN_OK:EndDialog(dlg,0);break;
		}break;
	}break;
}return 0;}
