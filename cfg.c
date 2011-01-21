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
* cfg.c : Configuration and INI stuff.
**********************************************************************/
#include "kwik.h"

 ////////////////////////////////////////////////
// Cfg_ParseSkin : Parses the skinfile in cf.skin
/// RETURN //////////////////////////////////////
// 0 : Success!
// 1 : File not found
// 2 : No KWIKSTRIP header or bad version
unsigned char Cfg_ParseSkin(){
	signed short i=0;
	char tmp[11]={0};
	FILE *fptr=0;
//
	if((fptr=fopen(cf.skin,"rb"))){
		fseek(fptr,0-sizeof(short),SEEK_END);
		fread(&i,sizeof(short),1,fptr);
		fseek(fptr,0-i,SEEK_END);
		fgets(tmp,10,fptr);
	// Check format
		if(strcmp(tmp,"KWIKSTRIP")||fgetc(fptr)!=0x03){
			fclose(fptr);
			// Big long error message
			MessageBox(0,"The skin you have selected does not appear to be a KwikStrip skin.\nIt probably just needs to be updated to the new version.\n\nKwikStrip will revert to the default skin, but you should go into\nthe config dialog and erase the skin path from the config so this\nmessage doesn't bother you every time you start up.","KwikStrip: Skin Error!",MB_OK|MB_ICONEXCLAMATION);
			free(cf.skin);cf.skin=0; // Free the skin since we aren't using it.
			return 2;
		}
	// We don't need the title for anything here so we skip it
		fseek(fptr,fgetc(fptr),SEEK_CUR);
	// We do need the font, so we read it out
		i=fgetc(fptr);
		cf.fnt=calloc(i+1,sizeof(char)); // This gets freed at the end of the program
		fgets(cf.fnt,i+1,fptr);
	// Read the metrics
		// We need to make a check for the lengths here so we don't get a buffer overflow.
		i=fgetc(fptr);
		fread(&mx,sizeof(METRICS)<i?sizeof(METRICS):i,1,fptr);
 		if(!mx.inx){mx.inx=1;} // Make sure inx is at least 1 so we don't get div-by-0s
	// Aaaaaand we're done!
		fclose(fptr);
	}else{return 1;}
	return 0;
}

 /////////////////////////////////////////////////////
// Cfg_DeleteItem : Deletes an item from the item list
/// ARGS /////////////////////////////////////////////
// idx : Item to delete
void Cfg_DeleteItem(unsigned char idx){
	if(itm[idx].nam){free(itm[idx].nam);itm[idx].nam=0;}
	if(itm[idx].exe){free(itm[idx].exe);itm[idx].exe=0;}
	if(itm[idx].ico){free(itm[idx].ico);itm[idx].ico=0;}
	if(itm[idx].dir){free(itm[idx].dir);itm[idx].dir=0;}
	if(itm[idx].arg){free(itm[idx].arg);itm[idx].arg=0;}
	DestroyIcon(itm[idx].icon);
	DestroyWindow(itm[idx].wnd);
	memcpy(&itm[idx],&itm[idx+1],(vr.btns-idx)*sizeof(ITEM));
	vr.btns--;
	itm=realloc(itm,vr.btns*sizeof(ITEM));
}

 /////////////////////////////////////////////////////////
// Config_Load : Saves the cf and itm vars to the INI file
/// RETURN ///////////////////////////////////////////////
// -1 : No INI file found
//  0 : No items
// >0 : Number of buttons added
signed short Config_Load(){
	unsigned char i=0;
	char *tmp=0,*cfghd="KwikStrip.Config",*itmhd="KwikStrip.Items";
//
// Reset the system vars
	memset(&vr,0,sizeof(VARS)-sizeof(char*)); // Remember not to erase vr.ini
	memcpy(&cf,&dcf,sizeof(CONFIG));
	memcpy(&mx,&dmx,sizeof(METRICS));
// Set up config file path
	if(!Bio_IsFile(vr.ini)){return -1;} // Make sure there is an INI file
// Read config
	// Vars
	if(!Bio_ReadINI(cfghd,"clse",&tmp,vr.ini)){cf.clse=(unsigned char )atol(tmp);}
	if(!Bio_ReadINI(cfghd,"anim",&tmp,vr.ini)){cf.anim=(unsigned char )atol(tmp);}
	if(!Bio_ReadINI(cfghd,"cols",&tmp,vr.ini)){cf.cols=(unsigned char )atol(tmp);}
	if(!Bio_ReadINI(cfghd,"disp",&tmp,vr.ini)){cf.disp=(unsigned char )atol(tmp);}
	if(!Bio_ReadINI(cfghd,"infw",&tmp,vr.ini)){cf.infw=(unsigned char )atol(tmp);}
	if(!Bio_ReadINI(cfghd,"hkey",&tmp,vr.ini)){cf.hkey=(unsigned short)atol(tmp);}
	if(!Bio_ReadINI(cfghd,"yoff",&tmp,vr.ini)){cf.yoff=(unsigned short)atol(tmp);}
	free(tmp); // Each Bio_ReadINI frees it's own tmp except the lest one
	// Skin
	if(!Bio_ReadINI(cfghd,"skin",&cf.skin,vr.ini)){
		Cfg_ParseSkin();
	}
// Read items
	tmp=calloc(9,sizeof(char));
		for(vr.btns=0;vr.btns<MAX_BTNS;vr.btns++){
		// Allocate and initialize a new item
			itm=realloc(itm,(vr.btns+1)*sizeof(ITEM));
			memset(&itm[vr.btns],0,sizeof(ITEM));
		// Read the item details
			_snprintf(tmp,8,"%d",vr.btns);
			Bio_ReadINI(itmhd,tmp,&itm[vr.btns].nam,vr.ini);
 			_snprintf(tmp,8,"%dexe",vr.btns);
			Bio_ReadINI(itmhd,tmp,&itm[vr.btns].exe,vr.ini);
		// If name or exe is missing, free this item and stop.
			if(!itm[vr.btns].nam||!itm[vr.btns].exe){break;}
		// Read the rest of the item data
			_snprintf(tmp,8,"%ddir",vr.btns);
			Bio_ReadINI(itmhd,tmp,&itm[vr.btns].dir,vr.ini);
			_snprintf(tmp,8,"%darg",vr.btns);
			Bio_ReadINI(itmhd,tmp,&itm[vr.btns].arg,vr.ini);
			_snprintf(tmp,8,"%dico",vr.btns);
			Bio_ReadINI(itmhd,tmp,&itm[vr.btns].ico,vr.ini);
			_snprintf(tmp,8,"%didx",vr.btns);
			if(!Bio_ReadINI(itmhd,tmp,&tmp,vr.ini)){
				itm[vr.btns].idx=(unsigned char)atol(tmp);
			}
		// Load the icon
			itm[vr.btns].icon=ExtractIcon(GetModuleHandle(0),itm[vr.btns].ico?itm[vr.btns].ico:itm[vr.btns].exe,itm[vr.btns].idx);
		}
	free(tmp);
	return vr.btns;
}

 /////////////////////////////////////////////////////////
// Config_Save : Saves the cf and itm vars to the INI file
/// RETURN ///////////////////////////////////////////////
// 1 on sucsess, else 0.
unsigned char Config_Save(HWND dlg){
	unsigned char i=0;
	FILE *fptr;
//
	// If the file can't be opened, there's a problem.
	if(!(fptr=fopen(vr.ini,"w"))){
		if(MessageBox(dlg,"Could not open the config file for writing,\nperhaps it is set to read-only?\n\nDiscard changes?","KwikStrip: Error saving config!",MB_YESNO|MB_DEFBUTTON2|MB_ICONEXCLAMATION)==IDYES){
			return 1; // Don't exit the config dialog yet!
		}else{
			return 0;
		}
	}
	// Save settings
		fprintf(fptr,"[KwikStrip.Config]\nclse=%d\nanim=%d\ncols=%d\ndisp=%d\ninfw=%d\nyoff=%d\nhkey=%d\n",cf.clse,cf.anim,cf.cols,cf.disp,cf.infw,cf.yoff,cf.hkey);
		if(strlen(cf.skin)){fprintf(fptr,"skin=%s\n",cf.skin);}
	// Save buttons
		if(vr.btns){
			fprintf(fptr,"\n[KwikStrip.Items]\n");
			for(i=0;i<vr.btns;i++){
				fprintf(fptr,"%d=%s\n%dexe=%s\n",i,itm[i].nam,i,itm[i].exe);
				if(itm[i].ico){fprintf(fptr,"%dico=%s\n",i,itm[i].ico);}
				if(itm[i].dir){fprintf(fptr,"%ddir=%s\n",i,itm[i].dir);}
				if(itm[i].arg){fprintf(fptr,"%darg=%s\n",i,itm[i].arg);}
				if(itm[i].idx){fprintf(fptr,"%didx=%d\n",i,itm[i].idx);}
			}
		}
	fclose(fptr);
	return 1;
}

unsigned short field[]={EDT_NAME,EDT_PATH,EDT_DIR,EDT_ARGS,EDT_ICON}; // Commonly used
OPENFILENAME ofn={sizeof(OPENFILENAME),0,0,0,0,0,0,0,MAX_PATH,0,0,0,0,OFN_FILEMUSTEXIST|OFN_EXPLORER,0,0,0,0,0,0};
LRESULT CALLBACK PropProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	static unsigned char sel;
	static HICON icn; // Preview icon
	char *tmpp,**dst[5];
	unsigned char i;
//
	case WM_INITDIALOG:
		sel=(unsigned char)lp; // Set the currently selected item
	// Set pointers
		dst[0]=&itm[sel].nam;
		dst[1]=&itm[sel].exe;
		dst[2]=&itm[sel].dir;
		dst[3]=&itm[sel].arg;
		dst[4]=&itm[sel].ico;
	// Set editboxes
		for(i=0;i<5;i++){
			SndMsg(field[i],WM_SETTEXT,0,*dst[i]);
			SndMsg(field[i],EM_LIMITTEXT,0,MAX_PATH);
		}SndMsg(SPN_INDEX,UDM_SETPOS,0,itm[sel].idx);
		SndMsg(SPN_INDEX,UDM_SETRANGE,0,255);
	break;
	case WM_COMMAND:switch(HIWORD(wp)){
		case EN_CHANGE:if(LOWORD(wp)==EDT_INDEX||LOWORD(wp)==EDT_ICON||LOWORD(wp)==EDT_PATH){
		// Use exe if there is no specific icon
			if(SndMsg(EDT_ICON,WM_GETTEXTLENGTH,0,0)){
				tmpp=calloc(SndMsg(EDT_ICON,WM_GETTEXTLENGTH,0,0)+1,sizeof(char));
				SndMsg(EDT_ICON,WM_GETTEXT,MAX_PATH,tmpp);
			}else{
				tmpp=calloc(SndMsg(EDT_PATH,WM_GETTEXTLENGTH,0,0)+1,sizeof(char));
				SndMsg(EDT_PATH,WM_GETTEXT,MAX_PATH,tmpp);
			}
		// Update preview
			DestroyIcon(icn); // Delete the old preview icon
			icn=ExtractIcon(GetModuleHandle(0),tmpp,SndMsg(SPN_INDEX,UDM_GETPOS,0,0));
			SndMsg(ICO_ICOPRV,STM_SETICON,icn,0);
			free(tmpp); // Free icon path
		}break;
		case BN_CLICKED:switch(LOWORD(wp)){
		// Browse dialogs
			case BTN_PATH:case BTN_ICON:
			// Set regular stuff
				ofn.hInstance=GetModuleHandle(0);
				ofn.hwndOwner=dlg;
				ofn.lpstrFile=calloc(MAX_PATH,sizeof(char));
				// Set the filters
					if(LOWORD(wp)==BTN_ICON){
						ofn.lpstrFilter="Possible icon files (*.ico,*.exe,*.dll)\0*.ico;*.exe;*.dll\0";
					}else{ofn.lpstrFilter="Executable files (*.exe)\0*.exe\0All files (*.*)\0*.*\0";}
				// Start the dialog and update the editbox if neccisary
					if(GetOpenFileName(&ofn)){
						SndMsg(LOWORD(wp)==BTN_PATH?EDT_PATH:EDT_ICON,WM_SETTEXT,0,ofn.lpstrFile);
					}
				free(ofn.lpstrFile);
			break;
			case BTN_OK:
			// Check for required fields
				if(!SndMsg(EDT_NAME,WM_GETTEXTLENGTH,0,0)){MessageBox(dlg,"You must specify a name!","KwikStrip: Error!",MB_OK|MB_ICONEXCLAMATION);break;}
				if(!SndMsg(EDT_PATH,WM_GETTEXTLENGTH,0,0)){MessageBox(dlg,"You must specify a file!","KwikStrip: Error!",MB_OK|MB_ICONEXCLAMATION);break;}
			// Set the pointers
				dst[0]=&itm[sel].nam;
				dst[1]=&itm[sel].exe;
				dst[2]=&itm[sel].dir;
				dst[3]=&itm[sel].arg;
				dst[4]=&itm[sel].ico;
			// Update fields
				for(i=0;i<5;i++){
				// Allocate space and retrieve string
					if(*dst[i]){free(*dst[i]);*dst[i]=0;}
					*dst[i]=calloc(SndMsg(field[i],WM_GETTEXTLENGTH,0,0)+1,sizeof(char));
					SndMsg(field[i],WM_GETTEXT,MAX_PATH,*dst[i]);
				// Resize string
					if(strlen(*dst[i])){
						*dst[i]=realloc(*dst[i],strlen(*dst[i])+1);
					}else{free(*dst[i]);*dst[i]=0;} // Free the space if there is no string
				}itm[sel].idx=(unsigned char)SndMsg(SPN_INDEX,UDM_GETPOS,0,0);
			// Update icon
				DestroyIcon(itm[sel].icon);itm[sel].icon=0;
				itm[sel].icon=ExtractIcon(GetModuleHandle(0),itm[sel].ico?itm[sel].ico:itm[sel].exe,itm[sel].idx);
			case BTN_CANCEL:case WM_DESTROY:
				DestroyIcon(icn); // Free preview icon
				EndDialog(dlg,LOWORD(wp));
			break;
		}break;
	}break;
}return 0;}

LRESULT CALLBACK ConfigProc(HWND dlg,UINT msg,WPARAM wp,LPARAM lp){switch(msg){
	DRAWITEMSTRUCT *ds;
	signed long i;
	ITEM titm;
//
	case WM_INITDIALOG:
		SndMsg(LST_ITEMS,LB_SETITEMHEIGHT,0,17);
	// Checkboxes
		SndMsg(CHK_CLOSE,BM_SETCHECK,cf.clse,0);
	// Animation check/slider
		SndMsg(CHK_ANIM,BM_SETCHECK,cf.anim,0);
		SndMsg(SLD_ANIM,TBM_SETRANGE,0,MAKELONG(4,12));
		if(cf.anim){
			SndMsg(SLD_ANIM,TBM_SETPOS,1,cf.anim);
		}else{EnableWindow(GetDlgItem(dlg,SLD_ANIM),0);}
	// Spinners
		SndMsg(SPN_COLS,UDM_SETRANGE32,1,0xFF);
		SndMsg(SPN_COLS,UDM_SETPOS,0,cf.cols);
		SndMsg(SPN_INFSZ,UDM_SETRANGE32,0,0xFF);
		SndMsg(SPN_INFSZ,UDM_SETPOS,0,cf.infw);
		SndMsg(SPN_YOFF,UDM_SETRANGE32,0,0xFFFF);
		SndMsg(SPN_YOFF,UDM_SETPOS,0,cf.yoff);
	// Set up the strip config area
		SndMsg(RAD_DEFZ+cf.disp,BM_SETCHECK,1,0);
		SndMsg(HTK_HOTKEY,HKM_SETHOTKEY,cf.hkey,0);
		if(cf.disp!=KS_DSP_KEY){EnableWindow(GetDlgItem(dlg,HTK_HOTKEY),0);}
	// Theme options
		if(cf.skin){SndMsg(EDT_SKIN,WM_SETTEXT,0,cf.skin);}
	// Item list
		for(i=0;i<vr.btns;i++){SndMsg(LST_ITEMS,LB_ADDSTRING,0,0);}
	break;
// Draw listbox items
	case WM_DRAWITEM:
		ds=(DRAWITEMSTRUCT*)lp;
		if(ds->itemAction==ODA_SELECT||ds->itemAction==ODA_DRAWENTIRE){
			if(ds->itemState&ODS_SELECTED){ // Not sure why it doesn't pass ODS_SELECTED...
				FillRect(ds->hDC,&ds->rcItem,(HBRUSH)(COLOR_HIGHLIGHT+1));
				SetTextColor(ds->hDC,GetSysColor(COLOR_HIGHLIGHTTEXT));
			}else{
				FillRect(ds->hDC,&ds->rcItem,(HBRUSH)(COLOR_WINDOW+1));
				SetTextColor(ds->hDC,GetSysColor(COLOR_WINDOWTEXT));
			}SetBkMode(ds->hDC,TRANSPARENT);
			DrawIconEx(ds->hDC,ds->rcItem.left+1,ds->rcItem.top,itm[ds->itemID].icon,16,16,0,0,DI_NORMAL);
			ds->rcItem.left+=18;
			DrawText(ds->hDC,itm[ds->itemID].nam,strlen(itm[ds->itemID].nam),&ds->rcItem,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_NOPREFIX);
		}
	break;
	case WM_COMMAND:switch(HIWORD(wp)){
		// Real-time update the y-offset so you can position the strip easier
		case EN_UPDATE:if(LOWORD(wp)==EDT_YOFF){
			SetWindowPos(vr.wnd,0,vr.rct.left+(vr.rct.right-mx.arx),vr.rct.top+SndMsg(SPN_YOFF,UDM_GETPOS,0,0),0,0,SWP_NOSIZE|SWP_NOZORDER);
		}break;
		case LBN_DBLCLK:
			i=SndMsg(LST_ITEMS,LB_GETCURSEL,0,0);
			if(DialogBoxParam(GetModuleHandle(0),MAKEINTRESOURCE(DLG_PROP),dlg,PropProc,i)==BTN_OK){
			// Update item name
				SndMsg(LST_ITEMS,LB_DELETESTRING,i,0);
				SndMsg(LST_ITEMS,LB_INSERTSTRING,i,0);
			}
		break;
		// Buttons/Checkboxes/Radio buttons
		case BN_CLICKED:switch(LOWORD(wp)){
		// Add item
			case BTN_ADD:if(vr.btns<MAX_BTNS){
				itm=realloc(itm,(vr.btns+1)*sizeof(ITEM));
				memset(&itm[vr.btns],0,sizeof(ITEM));
				if(DialogBoxParam(GetModuleHandle(0),MAKEINTRESOURCE(DLG_PROP),dlg,PropProc,vr.btns)==BTN_OK){
					SndMsg(LST_ITEMS,LB_ADDSTRING,0,0);
					vr.btns++;
				}else{itm=realloc(itm,(vr.btns)*sizeof(ITEM));}
			}else{MessageBox(dlg,"You already have the maximum allowed amount of buttons","KwikStrip: Error!",MB_OK|MB_ICONEXCLAMATION);}
			break;
		// Delete item
			case BTN_DELETE:if((i=SndMsg(LST_ITEMS,LB_GETCURSEL,0,0))!=LB_ERR){
				Cfg_DeleteItem((unsigned char)i);
				SndMsg(LST_ITEMS,LB_DELETESTRING,i,0);
				SndMsg(LST_ITEMS,LB_SETCURSEL,i,0);
			}break;
		// Move up
			case BTN_UP:if((i=SndMsg(LST_ITEMS,LB_GETCURSEL,0,0))!=LB_ERR&&i>0){
				memcpy(&titm,&itm[i-1],sizeof(ITEM));
				memcpy(&itm[i-1],&itm[i],sizeof(ITEM));
				memcpy(&itm[i],&titm,sizeof(ITEM));
				SndMsg(LST_ITEMS,LB_SETCURSEL,i-1,0);
			}break;
		// Move down
			case BTN_DOWN:if((i=SndMsg(LST_ITEMS,LB_GETCURSEL,0,0))!=LB_ERR&&i<SndMsg(LST_ITEMS,LB_GETCOUNT,0,0)-1){
				memcpy(&titm,&itm[i+1],sizeof(ITEM));
				memcpy(&itm[i+1],&itm[i],sizeof(ITEM));
				memcpy(&itm[i],&titm,sizeof(ITEM));
				SndMsg(LST_ITEMS,LB_SETCURSEL,i+1,0);
			}break;
		// Animation control
			case CHK_ANIM:EnableWindow(GetDlgItem(dlg,SLD_ANIM),SndMsg(CHK_ANIM,BM_GETCHECK,0,0));break;
		// Display option
			case RAD_DEFZ:case RAD_ONTOP:case RAD_HOTKEY:EnableWindow(GetDlgItem(dlg,HTK_HOTKEY),SndMsg(RAD_HOTKEY,BM_GETCHECK,0,0));break;
		// Theme buttons
			case BTN_SKIN:
			// Set regular stuff
				ofn.hwndOwner=dlg;
				ofn.hInstance=GetModuleHandle(0);
				ofn.lpstrFilter="KwikStrip tagged bitmap files (*.bmp)\0*.bmp\0";
				ofn.lpstrFile=calloc(MAX_PATH,sizeof(char));
				// Start the dialog and update the editbox if neccisary
					if(GetOpenFileName(&ofn)){
						SndMsg(EDT_SKIN,WM_SETTEXT,0,ofn.lpstrFile);
					}
				free(ofn.lpstrFile);
			break;
		// Apply settings
			case BTN_OK:
			// Checkboxes
				cf.clse=(unsigned char)SndMsg(CHK_CLOSE,BM_GETCHECK,0,0);
				if(SndMsg(CHK_ANIM,BM_GETCHECK,0,0)){
					cf.anim=(unsigned char)SndMsg(SLD_ANIM,TBM_GETPOS,0,0);
				}else{cf.anim=0;}
			// Spinners
				cf.cols=(unsigned char)SndMsg(SPN_COLS,UDM_GETPOS,0,0);
				cf.infw=(unsigned char)SndMsg(SPN_INFSZ,UDM_GETPOS,0,0);
				cf.yoff=(unsigned short)SndMsg(SPN_YOFF,UDM_GETPOS,0,0);
			// Display
				if(SndMsg(RAD_ONTOP,BM_GETCHECK,0,0)){cf.disp=1;}
				else if(SndMsg(RAD_HOTKEY,BM_GETCHECK,0,0)){cf.disp=2;}
				else{cf.disp=0;}
				cf.hkey=(unsigned short)SndMsg(HTK_HOTKEY,HKM_GETHOTKEY,0,0);
			// Skin
				if(cf.skin){free(cf.skin);cf.skin=0;}
				if((i=SndMsg(EDT_SKIN,WM_GETTEXTLENGTH,0,0)+1)){
					cf.skin=calloc(i,sizeof(char));
 					SndMsg(EDT_SKIN,WM_GETTEXT,i,cf.skin);
				}
			// Save settings
				if(!Config_Save(dlg)){break;}
			// break; // Continue the exiting below
			case BTN_CANCEL:case WM_DESTROY:EndDialog(dlg,LOWORD(wp));break;
		}break;
	}break;
}return 0;}
