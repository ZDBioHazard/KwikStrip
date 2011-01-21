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
* biolib.c : Just a copy of Bio_ReadINI() stuff from BioLib
**********************************************************************/
#include "kwik.h"

  /////////////////////////////////////////////
 // IsFile - Checks is a file exists
//  By BioHazard, March 1st 2006 -- Version 1
//// USAGE ////////////////////////////////////
// path : Path to the file to be checked
//// RETURN ///////////////////////////////////
// 0 : File does not exist or cannot be opened
// 1 : File exists and can be opened
unsigned char Bio_IsFile(char *path){
	FILE *fptr=fopen(path,"rb");
//
	if(fptr){fclose(fptr);return 1;}
	return 0;
}

  /////////////////////////////////////////////////////////
 // LineLen - Returns the length of the next line of a file
//  By BioHazard, March 24th 2006 -- Version 2
/// ARGUMENTS /////////////////////////////////////////////
// file : Pointer to the file to get the char count from
/// RETURN ////////////////////////////////////////////////
// Number of characters on the next line of [flie]
/// REMARKS ///////////////////////////////////////////////
// ! Any lines over 2147483647 chars will be mis-reported
signed long Bio_LineLen(FILE *file){
	signed long i=0;
//
	do{i++;}while(fgetc(file)!='\n'&&!feof(file));
	fseek(file,-i-(feof(file)?-1:1),SEEK_CUR);
	return i;
}

  //////////////////////////////////////////////////////
 // ReadLine - Reads the next line of a file to a string
//  By BioHazard, May 23rd 2006 -- Version 3
/// ARGUMENTS //////////////////////////////////////////
// file : Pointer to the file to get the line from
// buf  : Pointer to a char* for the string to go into
/// RETURN /////////////////////////////////////////////
// Number of characters copied to [buf]
/// REMARKS ////////////////////////////////////////////
// ! Any lines over 2147483647 chars will be mis-read
// * Don't malloc() the output string, this function
//    will free() it an realloc() it automagically.
signed long Bio_ReadLine(FILE *file,char **buf){
	signed long i=Bio_LineLen(file);
//
	if(*buf){free(*buf);}
	if(!(*buf=calloc(i+1,sizeof(char)))){return 0;}
	fgets(*buf,i,file);
	fgetc(file);
	return i;
}

  /////////////////////////////////////////////////
 // SkipChar - Skips certain chars in a file
//  By BioHazard, May 31st 2006 -- Version 2
/// ARGUMENTS /////////////////////////////////////
// file : File who's chars are to be skipped
// mask : Specific chars to skip
/// REMARKS ///////////////////////////////////////
// * If [mask] is 0, " \t\n\r" (whitespace) is used
void Bio_SkipChar(FILE *file,char *mask){
	while(strchr(mask?mask:" \t\n\r",fgetc(file))&&!feof(file));
	if(!feof(file)){fseek(file,-1,SEEK_CUR);}
}

  //////////////////////////////////////////////////////
 // ReadINI - Reads an INI value to a buffer from a file
//  Written by BioHazard on July 9th 2006 -- Version 7
//// USAGE /////////////////////////////////////////////
// section : The INI section to start reading at
// entry   : The entry to read to [out]
// buf     : Adress of a char* for the string to go into
// ini     : Path to the file to be opened
//// RETURN ////////////////////////////////////////////
// 0 : No problems
// 1 : [ini] could not be opened for reading
// 2 : Temp strings failed to allocate
// 3 : [section] could not be found
// 4 : [entry] could not be found under [section]
// 5 : Couldn't find key/value seperator ('=')
// 6 : [out] failed to allocate
/// REMARKS ////////////////////////////////////////////
// ! Any lines over 2147483647 chars will be mis-read
// * Don't malloc() the output string, this function
//    will free() it an malloc() it automagically.
unsigned char Bio_ReadINI(char *section,char *key,char **out,char *ini){
	FILE *fptr=0;
	char *tmp=0,*cmp=0;
//
// Open the file and set up the strings
	if(!(fptr=fopen(ini,"r"))){return 1;} // Die if the file doesn't open
	if(!(tmp=calloc((strlen(section)+2>strlen(key)?strlen(section)+2:strlen(key))+2,sizeof(char)))){
		fclose(fptr);
		return 2;
	}
	if(!(cmp=calloc(strlen(section)+3,sizeof(char)))){
		free(tmp);
		fclose(fptr);
		return 2;
	}
// Get to the section
	cmp[0]='[';strcat(cmp,section);strcat(cmp,"]"); // Put the section in []'s
	while(strcmp(tmp,cmp)){
		Bio_SkipChar(fptr,0);
	// Read and compare
		memset(tmp,0,(strlen(cmp)+1)*sizeof(char));
		fgets(tmp,strlen(cmp)+1,fptr);
		if(!strrchr(tmp,'\n')){ // Skip the rest of the line
			while(fgetc(fptr)!='\n'&&!feof(fptr));
		}
	// Check if [section] can't be found
		if(feof(fptr)){
			fclose(fptr);
			free(tmp);
			free(cmp);
			return 3;
		}
	}
	free(cmp);
// Get to the key
	while(!feof(fptr)){
		Bio_SkipChar(fptr,0);
		memset(tmp,0,(strlen(key)+1)*sizeof(char));
		fgets(tmp,strlen(key)+1,fptr);
	// Check for new section
		if(tmp[0]=='['||feof(fptr)){
			fclose(fptr);
			free(tmp);
			return 4;
		}
	// Read the value
		if(!strcmp(tmp,key)){
			free(tmp); // We're done with tmp
		// Skip the = and the space around it
			Bio_SkipChar(fptr," \t");
		// Check for expected '='
			if(fgetc(fptr)!='='){
				fclose(fptr);
				return 5;
			}
			Bio_SkipChar(fptr," \t");
		// Yay! Read the result!
			if(!Bio_ReadLine(fptr,out)){
				fclose(fptr);
				return 7;
			}
			break; // Done!
		}
	// Skip the rest of the line
		if(!strrchr(tmp,'\n')){
			while(fgetc(fptr)!='\n'&&!feof(fptr));
		}
	}
// Finish up
 	fclose(fptr);
	return 0;
}
