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
* kwik.c : Window procedures and global vars
**********************************************************************/
#include "kwik.h"
// System vars
VARS vr={0};CONFIG cf={0};METRICS mx={0};ITEM *itm=0;
CONFIG dcf={8,6,9,1,0,0,MAKEWORD('K',HOTKEYF_CONTROL|HOTKEYF_ALT),0,"Tahoma"};
METRICS dmx={0x00DDDDDD,12,1,0,18,0,18,36,0,36,72,108,18,36,36,36,18,18,32,32,2,2,3,3,2,2}; // 32px
static char *arg=0; // Used in the "custom args" dialog

LRESULT CALLBACK AboutProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){if(msg==WM_CLOSE){EndDialog(dlg,0);}return 0;}

LRESULT CALLBACK ArgProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	case WM_INITDIALOG:
		SndMsg(EDT_ARGS,WM_SETTEXT,0,itm[lp].arg);
		SetFocus(GetDlgItem(dlg,EDT_ARGS));
	break;
	case WM_COMMAND:switch(HIWORD(wp)){
		case BN_CLICKED:switch(LOWORD(wp)){
			case BTN_OK:
				arg=calloc(SndMsg(EDT_ARGS,WM_GETTEXTLENGTH,0,0),sizeof(char));
				SndMsg(EDT_ARGS,WM_GETTEXT,0xFFFFFFFF,arg);
				// free(arg); // Freed at the end of BtnProc:WM_LBUTTONUP
			case BTN_CANCEL:EndDialog(dlg,LOWORD(wp));break;
		}break;
	}break;
}return 0;}

TRACKMOUSEEVENT mevnt={sizeof(TRACKMOUSEEVENT),TME_LEAVE,0,HOVER_DEFAULT}; // Put this out here so we can use it.
LRESULT CALLBACK BtnProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	unsigned char i;
	char *tmp;
	POINT pt;
//
	case WM_PAINT:Strip_DrawBtn(dlg,mx.up,0);break;
// Hovering
	case WM_MOUSEMOVE:if(vr.hvr!=dlg){vr.hvr=dlg;
		Strip_DrawBtn(dlg,mx.hv,1);
		mevnt.hwndTrack=dlg;_TrackMouseEvent(&mevnt);
	}break;
	case WM_MOUSELEAVE:Strip_DrawBtn(dlg,mx.up,0);vr.hvr=0;break;
// Pressing
	case WM_LBUTTONDOWN:vr.dwn=dlg;
		Strip_DrawBtn(dlg,mx.dn,0);
	break;
// Releasing
	case WM_LBUTTONUP:if(vr.dwn==dlg){vr.dwn=0;
		if(dlg!=vr.arw){
			arg=0;
			Strip_DrawBtn(dlg,mx.up,0);
			i=Strip_BtnFrmWnd(dlg);
			if(LOWORD(wp)&MK_SHIFT){
			// Use custom args
				if(DialogBoxParam(GetModuleHandle(0),MAKEINTRESOURCE(DLG_ARGS),vr.wnd,ArgProc,i)==BTN_CANCEL){break;}
			}else{     // Don't let the args string be allocated
				arg=0; // or all manner of bad-stuff can happen.
			}
		// Choose the directory to launch out of
			if(LOWORD(wp)&MK_CONTROL){
				tmp=calloc(2,sizeof(char));
				strcpy(tmp,".");
			}else if(!itm[i].dir&&strrchr(itm[i].exe,'\\')){ // This might be a URL, which doesn't have a '\\'
				tmp=calloc(strlen(itm[i].exe)+1,sizeof(char));
				strcpy(tmp,itm[i].exe);
				strrchr(tmp,'\\')[1]='\0';
			}else{tmp=0;}
		// GOoOOoooOOOOOooOOOoooOO!
			ShellExecute(0,"open",itm[i].exe,arg?arg:itm[i].arg,tmp?tmp:itm[i].dir,SW_NORMAL);
			if(tmp){free(tmp);tmp=0;}
			if(arg){free(arg);arg=0;}
			if(cf.clse){Strip_OpenClose();}
		}else{Strip_OpenClose();}
	}break;
// Item menu
	case WM_RBUTTONUP:
		GetCursorPos(&pt);
		TrackPopupMenu(GetSubMenu(LoadMenu(GetModuleHandle(0),MAKEINTRESOURCE(MNU_MENU)),dlg!=vr.arw?1:(LOWORD(wp)&MK_SHIFT?2:0)),TPM_RIGHTALIGN,pt.x,pt.y,0,dlg,0);
	break;
	case WM_COMMAND:switch(HIWORD(wp)){
		case BN_CLICKED:switch(LOWORD(wp)){
			case MNU_ARGS:
			case MNU_DIR:
			case MNU_LAUNCH:
				SendMessage(dlg,WM_LBUTTONDOWN,0,0);
				Sleep(150);
				SendMessage(dlg,WM_LBUTTONUP,(LOWORD(wp)==MNU_DIR)?MK_CONTROL:(LOWORD(wp)==MNU_ARGS)?MK_SHIFT:(LOWORD(wp)==MNU_FOLDER)?MK_ALT:0,0);
			break;
			case MNU_FOLDER:
		// Open containing directory
				i=Strip_BtnFrmWnd(dlg);
				if(strrchr(itm[i].exe,'\\')){
					tmp=calloc(strlen(itm[i].exe)+1,sizeof(char));
						strcpy(tmp,itm[i].exe);
						*strrchr(tmp,'\\')='\0';
						ShellExecute(0,"open",tmp,0,0,SW_NORMAL);
						if(cf.clse){Strip_OpenClose();}
					free(tmp);
				}else{MessageBox(vr.wnd,"This item has no containing folder","KwikStrip: Error!",MB_OK|MB_ICONEXCLAMATION);}
			break;
			default:SendMessage(vr.wnd,msg,wp,lp);break; // Fall back to the default menu options
		}break;
	}break;
}return DefWindowProc(dlg,msg,wp,lp);}

LRESULT CALLBACK MainProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	POINT pt;
//
// Window creation
	case WM_CREATE:Strip_Setup(dlg);break;
// Ways to activate the strip
	case WM_HOTKEY:SetForegroundWindow(dlg);break;
// Displaying the menu
	case WM_RBUTTONUP:
		GetCursorPos(&pt);
		TrackPopupMenu(GetSubMenu(LoadMenu(GetModuleHandle(0),MAKEINTRESOURCE(MNU_MENU)),0),TPM_RIGHTALIGN,pt.x,pt.y,0,dlg,0);
	break;
	case WM_COMMAND:switch(LOWORD(wp)){
		case MNU_OPNCLS:Strip_OpenClose();break;
		case MNU_CONFIG:
			if(vr.open){Strip_OpenClose();} // Avoid a bug with drawing deleted items
			DialogBox(GetModuleHandle(0),MAKEINTRESOURCE(DLG_CONFIG),dlg,ConfigProc);
			PostQuitMessage(KS_EXIT_RESTART);
		break;
		case MNU_HELP:ShellExecute(HWND_DESKTOP,"open",STR_HELPURL,"","",SW_NORMAL);break;
		case MNU_ABOUT:DialogBox(GetModuleHandle(0),MAKEINTRESOURCE(DLG_ABOUT),dlg,AboutProc);break;
		case MNU_EXIT:PostQuitMessage(0);break;
	}break;
// Open key shortcut
	case WM_KEYDOWN:if(wp==VK_SPACE&&vr.dwn!=vr.arw){
		Strip_DrawBtn(vr.arw,mx.dn,0);
		vr.dwn=vr.arw;
	}break;
	case WM_KEYUP:if(wp==VK_SPACE){vr.dwn=0;
		Strip_DrawBtn(vr.arw,mx.up,0);
		Strip_OpenClose();
	}break;
// Hovering
	case WM_MOUSEMOVE:if(vr.hvr!=dlg){vr.hvr=dlg;
		Strip_Draw();
	}break;
// Fixing press detection bugs
	case WM_LBUTTONDOWN:vr.dwn=dlg;break;
	case WM_LBUTTONUP:vr.dwn=0;break;
// System messages
	case WM_DISPLAYCHANGE:Strip_Position(dlg);break;
	case WM_PAINT:Strip_Draw();break;
	case WM_CLOSE:
		if(MessageBox(dlg,"Are you sure you want to exit KwikStrip?","KwikStrip: Exit?",MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION)==IDYES){
			PostQuitMessage(0);
		}else{return 0;} // Don't let DefWindowProc() get it
	break;
}return DefWindowProc(dlg,msg,wp,lp);}

int WINAPI WinMain(HINSTANCE inst,HINSTANCE prev,LPSTR cline,int cshow){
	WNDCLASS kscls={CS_OWNDC,MainProc,0,0,inst,LoadIcon(inst,MAKEINTRESOURCE(ICO_ICON)),LoadCursor(0,IDC_ARROW),0,0,"kscls"};
	WNDCLASS bncls={CS_OWNDC,BtnProc,0,0,inst,0,LoadCursor(0,IDC_ARROW),0,0,"bncls"};
	unsigned long i=0;
	MSG msg;
//
	InitCommonControls();
	__try{ // Start exceptionable code
	// Register classes
		RegisterClass(&kscls);
		RegisterClass(&bncls);
	// Generate INI path
		vr.ini=calloc(MAX_PATH,sizeof(char));
		GetModuleFileName(GetModuleHandle(0),vr.ini,MAX_PATH);
		*(strrchr(vr.ini,'\\')+sizeof(char))='\0';
		strncat(vr.ini,"KwikStrip-",MAX_PATH-strlen(vr.ini));
		i=MAX_PATH-strlen(vr.ini);
		GetUserName(&vr.ini[strlen(vr.ini)],&i);
		strncat(vr.ini,".ini",MAX_PATH-strlen(vr.ini));
		vr.ini=realloc(vr.ini,strlen(vr.ini)+1);
  	// Create window
		do{switch(Config_Load()){ // Main program loop
			case KS_INI_NONE:
				if(MessageBox(HWND_DESKTOP,"This appears to be the first time you have run KwikStrip.\nYou need to set it up.\n\nOpen the config dialog now?","KwikStrip: Welcome!",MB_YESNO|MB_ICONINFORMATION)==IDYES){
					DialogBox(GetModuleHandle(0),MAKEINTRESOURCE(DLG_CONFIG),HWND_DESKTOP,ConfigProc);
					msg.wParam=KS_EXIT_RESTART;
					continue; // Try to load the config again
				}else{msg.wParam=0;}
			break;
			case KS_INI_NOBT:
				if(MessageBox(HWND_DESKTOP,"It appears that you have no buttons defined.\nYou need to add at least one for KwikStrip to operate properly.\n\nOpen the config dialog?","KwikStrip: No button definitions!",MB_YESNO|MB_ICONEXCLAMATION|MB_ICONQUESTION)==IDYES){
					DialogBox(GetModuleHandle(0),MAKEINTRESOURCE(DLG_CONFIG),HWND_DESKTOP,ConfigProc);
					msg.wParam=KS_EXIT_RESTART;
					continue; // Try to load the config again
				}else{msg.wParam=0;}
			break;
			default: // Start normally
				vr.wnd=CreateWindowEx(WS_EX_TOOLWINDOW|(cf.disp==KS_DSP_TOP?WS_EX_TOPMOST:0),"kscls",STR_TITLE,WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN,0,0,0,0,HWND_DESKTOP,0,inst,0);
				while(GetMessage(&msg,0,0,0)){TranslateMessage(&msg);DispatchMessage(&msg);}
			// Delete stuff
				DestroyWindow(vr.wnd);
				DeleteDC(vr.gfx);
				do{Cfg_DeleteItem(0);}while(vr.btns);
				DeleteObject(SelectObject(vr.gfx,0));
				free(itm);
				free(cf.fnt);
			break;
		}}while(msg.wParam==KS_EXIT_RESTART);
		free(vr.ini);
		return msg.wParam;
	// Crash handler
	}__except(DialogBoxParam(GetModuleHandle(0),MAKEINTRESOURCE(DLG_CRASH),vr.wnd,CrashProc,(LPARAM)GetExceptionInformation())?EXCEPTION_EXECUTE_HANDLER:EXCEPTION_EXECUTE_HANDLER){
		return 1;
	}
}
