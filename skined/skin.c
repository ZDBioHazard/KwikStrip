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
* skin.c : Skin editor
*************************************************************/
#include "skin.h"
METRICS mx={0x00DDDDDD,12,1,0,18,0,18,36,0,36,72,108,18,36,36,36,18,18,32,32,2,2,3,3,2,2}; // 32px

// Default metrics for the reset functions
#define STR_DEF16 "#DDDDDD,8,0,0,10,0,10,20,0,20,40,60,10,20,20,20,10,12,16,16,2,2,3,3,2,1"
#define STR_DEF32 "#DDDDDD,12,1,0,18,0,18,36,0,36,72,108,18,36,36,36,18,18,32,32,2,2,3,3,2,2"
#define STR_DEF48 "#DDDDDD,22,1,0,27,0,27,54,0,54,108,162,27,54,54,54,27,27,48,48,3,3,4,4,3,3"

// Global variables
WNDPROC OldPrevProc;
char *file=0;
char *font="Tahoma";
HFONT fnt=0;
HDC gfx=0;

COLORREF swaprb(COLORREF i){
	unsigned long o=i;
	char *po=(char*)&o,*pi=(char*)&i;
//
	po[2]=pi[0];
	po[0]=pi[2];
	return o;
}

unsigned char IsKSSkin(FILE *fptr){
	unsigned short len=0;
	char tmp[11]={0};
//
	fseek(fptr,0-sizeof(short),SEEK_END);
	fread(&len,sizeof(short),1,fptr);
	fseek(fptr,0-len,SEEK_END);
	fgets(tmp,10,fptr);
	if(strcmp(tmp,"KWIKSTRIP")){return 0;}
	return fgetc(fptr);
}

LRESULT CALLBACK AboutProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){if(msg==WM_CLOSE){EndDialog(dlg,0);}return 0;}

LRESULT CALLBACK PrevProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	unsigned short i,x,stx,sty,dst;
	PAINTSTRUCT ps;
	RECT rct;
//
	case WM_PAINT: // Draw the preview
		BeginPaint(dlg,&ps);
			GetWindowRect(dlg,&rct);
			// Since the results of GetWindowRect() are flawed for this
			// application, we need to fiddle with the values a bit
			rct.bottom-=rct.top;
			rct.right-=rct.left;
			rct.top=rct.left=0;
			// Erase the background and draw our preview
			FillRect(ps.hdc,&rct,WHITE_BRUSH);
			if(gfx&&file){
				stx=mx.arx+mx.bnx+mx.inx;
				sty=(mx.bny*2)+1;
				dst=(mx.arx+(2*mx.bnx))-mx.inx;
				x=((unsigned short)(rct.right-rct.left))-(mx.arx+(mx.bnx*3))-2;
			// Strip
				// Arrows
				BitBlt(ps.hdc,x,1,mx.arx,mx.ary,gfx,mx.arw,mx.up,SRCCOPY);
				BitBlt(ps.hdc,(x-mx.arx)-(mx.arx/4),1,mx.arx,mx.ary,gfx,mx.arw,mx.dn,SRCCOPY);
				BitBlt(ps.hdc,(x-(mx.arx*2))-((mx.arx/4)*2),1,mx.arx,mx.ary,gfx,mx.arw,mx.hv,SRCCOPY);
				// Buttons
				for(i=0;i<2;i++){
					BitBlt(ps.hdc,x+mx.arx,(i*mx.bny)+1,mx.bnx,mx.bny,gfx,mx.btn,mx.up,SRCCOPY);
					BitBlt(ps.hdc,x+mx.arx+mx.bnx,(i*mx.bny)+1,mx.bnx,mx.bny,gfx,mx.btn,mx.hv,SRCCOPY);
					BitBlt(ps.hdc,x+mx.arx+(mx.bnx*2),(i*mx.bny)+1,mx.bnx,mx.bny,gfx,mx.btn,mx.dn,SRCCOPY);
				}
				DrawIconEx(ps.hdc,x+mx.arx+mx.bpx,mx.bpy+1,LoadIcon(0,IDI_APPLICATION),mx.icx,mx.icy,0,0,DI_NORMAL);
				DrawIconEx(ps.hdc,x+mx.arx+mx.bnx+mx.bpx,mx.bpy+1,LoadIcon(0,IDI_APPLICATION),mx.icx,mx.icy,0,0,DI_NORMAL);
				DrawIconEx(ps.hdc,x+mx.arx+(mx.bnx*2)+mx.bdx,mx.bdy+1,LoadIcon(0,IDI_APPLICATION),mx.icx,mx.icy,0,0,DI_NORMAL);
			// InfoBox
				rct.left=x+mx.arx+mx.bnx+(mx.ipx*2);
				rct.top=sty+mx.ipy;
				rct.right=x+(mx.arx+(3*mx.bnx))-(mx.ipy*2);
				rct.bottom=(sty+mx.iny)-mx.ipy;
				// Prepare text
				SelectObject(ps.hdc,fnt);
				SetTextColor(ps.hdc,mx.txc);
				SetBkMode(ps.hdc,TRANSPARENT);
				SetStretchBltMode(ps.hdc,HALFTONE);
				// Draw the infobox with text
				BitBlt(ps.hdc,x+mx.arx+mx.bnx,sty,mx.inx,mx.iny,gfx,mx.ilf,mx.inf,SRCCOPY);
				for(i=0;i<(dst/mx.inx)-1;i++){
					BitBlt(ps.hdc,x+(stx+(i*mx.inx)),sty,mx.inx,mx.iny,gfx,mx.ict,mx.inf,SRCCOPY);
				}
				BitBlt(ps.hdc,x+mx.bnx+dst,sty,mx.inx,mx.iny,gfx,mx.irt,mx.inf,SRCCOPY);
				DrawText(ps.hdc,"Info Text",9,&rct,DT_SINGLELINE|DT_NOPREFIX|DT_END_ELLIPSIS);
			}else{ // if(gfx&&file)
				DrawText(ps.hdc,"No bitmap loaded",16,&rct,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			}
		EndPaint(dlg,&ps);
	break;
}return CallWindowProc(OldPrevProc,dlg,msg,wp,lp);}

OPENFILENAME ofn={sizeof(OPENFILENAME),0,0,"Bitmap files (*.bmp,*.rle)\0*.bmp;*.rle\0",0,0,0,0,MAX_PATH,0,0,0,0,OFN_FILEMUSTEXIST|OFN_EXPLORER,0,0,"bmp",0,0,0};
LRESULT CALLBACK MainProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	unsigned short len=0;
	HBITMAP bmp;
	FILE *fptr;
	char *tmp;
	HDC hdc;
//
	case WM_INITDIALOG: // General startup
		SndMsg(EDT_TITLE,EM_LIMITTEXT,0xFE,0);
		SndMsg(EDT_FONT,EM_LIMITTEXT,0xFE,0);
		SndMsg(EDT_MX,EM_LIMITTEXT,127,0);
		// Set up preview, we need to override it's window procedure
		OldPrevProc=(WNDPROC)SetWindowLong(GetDlgItem(dlg,DRW_PREV),GWL_WNDPROC,(long)&PrevProc);
		// Prep the bitmap DC
		hdc=GetDC(GetDlgItem(dlg,DRW_PREV));
			gfx=CreateCompatibleDC(hdc);
		ReleaseDC(dlg,hdc);
		// Set up feilds
		SendMessage(dlg,WM_COMMAND,MAKELONG(EDT_MX,EN_KILLFOCUS),0);
		SndMsg(EDT_BMP,WM_SETTEXT,0,"Load a bitmap by clicking \"browse\" over there ->");
		SetFocus(GetDlgItem(dlg,EDT_TITLE));
	break;
	case WM_COMMAND:switch(HIWORD(wp)){
		case EN_KILLFOCUS:if(LOWORD(wp)==EDT_MX){
			tmp=calloc(128,sizeof(char));
				SndMsg(EDT_MX,WM_GETTEXT,0xFF,tmp);
				_snprintf(tmp,128,"#%.6X,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
							swaprb(mx.txc),mx.txt,mx.txa,mx.arw,mx.btn,mx.ilf,mx.ict,mx.irt,mx.up,mx.hv,mx.dn,mx.inf,mx.arx,mx.ary,mx.bnx,mx.bny,mx.inx,mx.iny,mx.icx,mx.icy,mx.bpx,mx.bpy,mx.bdx,mx.bdy,mx.ipx,mx.ipy);
				SndMsg(EDT_MX,WM_SETTEXT,0,tmp);
				SndMsg(EDT_FONT,WM_SETTEXT,0,font);
			free(tmp);
		}break;
		case EN_UPDATE:
			switch(LOWORD(wp)){
				case EDT_MX:
					tmp=calloc(SndMsg(EDT_MX,WM_GETTEXTLENGTH,0,0)+1,sizeof(char));
						SndMsg(EDT_MX,WM_GETTEXT,0xFF,tmp);
						sscanf(tmp,"#%X,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
								&mx.txc,&mx.txt,&mx.txa,&mx.arw,&mx.btn,&mx.ilf,&mx.ict,&mx.irt,&mx.up,&mx.hv,&mx.dn,&mx.inf,&mx.arx,&mx.ary,&mx.bnx,&mx.bny,&mx.inx,&mx.iny,&mx.icx,&mx.icy,&mx.bpx,&mx.bpy,&mx.bdx,&mx.bdy,&mx.ipx,&mx.ipy);
						mx.txc=swaprb(mx.txc);
						if(!mx.inx){mx.inx=1;}
					free(tmp);
				break;
				case EDT_FONT:
					if(font){free(font);}
					font=calloc(SndMsg(EDT_FONT,WM_GETTEXTLENGTH,0,0)+1,sizeof(char));
					SndMsg(EDT_FONT,WM_GETTEXT,0xFF,font);
					if(fnt){DeleteObject(fnt);}
					fnt=CreateFont(mx.txt,0,0,0,(mx.txa&KS_TXA_BOLD)?FW_BOLD:0,(mx.txa&KS_TXA_ITALIC)?1:0,0,0,0,0,0,0,0,font);
					// free(font); // This is done at the end of the program
				break;
			}
			InvalidateRect(GetDlgItem(dlg,DRW_PREV),0,0);
		break;
		case BN_CLICKED:switch(LOWORD(wp)){
			case MNU_LOAD: // Load a bitmap
			case BTN_BMP:
				ofn.hInstance=GetModuleHandle(0);
				ofn.hwndOwner=dlg;
				ofn.lpstrTitle=LOWORD(wp)==MNU_LOAD?"Load a KwikStrip skin file":"Load a bitmap file";
				ofn.lpstrFile=calloc(MAX_PATH,sizeof(char));
				if(GetOpenFileName(&ofn)){
					free(file);
					file=ofn.lpstrFile;
					SndMsg(EDT_BMP,WM_SETTEXT,0,ofn.lpstrFile);
					// Load the bitmap
					bmp=LoadImage(GetModuleHandle(0),ofn.lpstrFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
					DeleteObject(SelectObject(gfx,bmp));
					if(LOWORD(wp)==MNU_LOAD){
						if((fptr=fopen(file,"rb"))){
							if(IsKSSkin(fptr)==KS_FORMAT){
								// Get title
								len=fgetc(fptr);
								tmp=calloc(len+2,sizeof(char));
									fgets(tmp,len+1,fptr);
									SndMsg(EDT_TITLE,WM_SETTEXT,0,tmp);
								free(tmp);
								// Get font
								len=fgetc(fptr);
								tmp=calloc(len+2,sizeof(char));
									fgets(tmp,len+1,fptr);
									SndMsg(EDT_FONT,WM_SETTEXT,0,tmp);
								free(tmp);
								// Get font
								len=fgetc(fptr);
								fread(&mx,sizeof(METRICS)<len?sizeof(METRICS):len,1,fptr);
								SendMessage(dlg,WM_COMMAND,MAKELONG(EDT_MX,EN_KILLFOCUS),0);
							}else{MessageBox(dlg,"This isn't a KwikStrip skin.\nOnly the bitmap data has been loaded.","SkinEd: Notice",MB_OK|MB_ICONINFORMATION);}
							fclose(fptr);
						}else{MessageBox(dlg,"Couldn't open the file to read it's information!","SkinEd: Error!",MB_OK|MB_ICONEXCLAMATION);}
					}
				}else{free(ofn.lpstrFile);}
				// Update preview
				InvalidateRect(GetDlgItem(dlg,DRW_PREV),0,0);
			break;
			case MNU_SAVE: // Save the file
				if(!SndMsg(EDT_BMP,WM_GETTEXTLENGTH,0,0)){
					MessageBox(dlg,"You must load a bitmap first!","SkinEd: Can't save!",MB_OK|MB_ICONEXCLAMATION);
				}else if(!SndMsg(EDT_TITLE,WM_GETTEXTLENGTH,0,0)){
					MessageBox(dlg,"You have to give this skin a name!","SkinEd: Can't save!",MB_OK|MB_ICONEXCLAMATION);
				}else if(!SndMsg(EDT_FONT,WM_GETTEXTLENGTH,0,0)){
					MessageBox(dlg,"You need to specify a font for the info box!","SkinEd: Can't save!",MB_OK|MB_ICONEXCLAMATION);
				}else if(!SndMsg(EDT_MX,WM_GETTEXTLENGTH,0,0)){
					MessageBox(dlg,"There absolutely must be a metrics formula.","SkinEd: Can't save!",MB_OK|MB_ICONEXCLAMATION);
				}else{
					if((fptr=fopen(file,"r+b"))){
					// See if it's already KS-ified and truncate it if it is
						if(IsKSSkin(fptr)){
							fseek(fptr,0-sizeof(short),SEEK_END);
							fread(&len,sizeof(short),1,fptr);
							_chsize(_fileno(fptr),ftell(fptr)-len);
						}
					// Get title
						tmp=calloc(SndMsg(EDT_TITLE,WM_GETTEXTLENGTH,0,0)+1,sizeof(char));
							SndMsg(EDT_TITLE,WM_GETTEXT,0xFE,tmp);
						// Write the file
							fseek(fptr,0,SEEK_END);
							fprintf(fptr,"KWIKSTRIP%c%c%s%c%s%c",KS_FORMAT,strlen(tmp),tmp,strlen(font),font,sizeof(METRICS));
							fwrite(&mx,sizeof(METRICS),1,fptr);
							len=11+strlen(tmp)+1+strlen(font)+1+sizeof(METRICS)+sizeof(short);
							fwrite(&len,sizeof(short),1,fptr);
						// Free stuff and exit
						free(tmp);
						fclose(fptr);
					}else{MessageBox(dlg,"Could not open the file for writing!","SkinEd: File Error!",MB_OK|MB_ICONEXCLAMATION);}
				}
			break;
		// Handle default metrics sets
			case MNU_16PX:
				SndMsg(EDT_MX,WM_SETTEXT,0,STR_DEF16);
				SndMsg(EDT_FONT,WM_SETTEXT,0,"Small Fonts");
			break;
			case MNU_32PX:
				SndMsg(EDT_MX,WM_SETTEXT,0,STR_DEF32);
				SndMsg(EDT_FONT,WM_SETTEXT,0,"Tahoma");
			break;
			case MNU_48PX:
				SndMsg(EDT_MX,WM_SETTEXT,0,STR_DEF48);
				SndMsg(EDT_FONT,WM_SETTEXT,0,"Tahoma");
			break;
		// Help stuff
			case MNU_ABOUT:DialogBox(GetModuleHandle(0),MAKEINTRESOURCE(DLG_ABOUT),dlg,AboutProc);break;
			case MNU_HELP:ShellExecute(HWND_DESKTOP,"open",STR_HELPURL,"","",SW_NORMAL);break;
		// Ways to exit
			case MNU_EXIT:
			case WM_DESTROY:
				if(file){free(file);}
				DeleteDC(gfx);
				EndDialog(dlg,0);
			break;
		}break;
	}break;
}return 0;}

int WINAPI WinMain(HINSTANCE inst,HINSTANCE prev,LPSTR cline,int cshow){
	return DialogBox(GetModuleHandle(0),MAKEINTRESOURCE(DLG_SKIN),HWND_DESKTOP,MainProc);
}
