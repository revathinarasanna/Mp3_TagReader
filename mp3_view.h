#ifndef VIEW_H
#define VIEW_H

#include<stdio.h>
#include "common.h"
#include <stdlib.h>
#include <string.h>

#define pos_size 6
typedef struct MusicInfo
{
	//Input File details
	char *mp3_fname;
	FILE *fptr_fname;

	//Variable to read Tag details
	char *Title;
	int title_size;

	char *Artist;
	int artist_size;

	char *Album;
	int album_size;

	char *Year;
	int year_size;

	char* Genre;
	int genre_size;
       
        char *Comment ;
        int comment_size ;

	int pos[pos_size];

}Music;

void print_help();

void invalid_arguments();

Status read_and_validate_view_args(char *argv,Music *music);

Status Open_files_view(Music *music);

Status check_header_and_version(FILE *fname);

Status readInfo(Music *music);

TagReader tagreader(FILE *fptr);

Status tagsizereader(FILE *fptr,int *size);

Status readtaginfo(FILE *fptr, char **name,int size);

Status printInfo(Music *music);

#endif

