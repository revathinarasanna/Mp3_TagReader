#include<stdio.h>
#include<string.h>
#include "common.h"
#include "mp3_view.h"
Status read_and_validate_view_args(char *argv , Music *music)
{
    /*check extenstion is .mp3 or not */
    if(strcmp(strstr(argv,"."),".mp3") == 0)
    {
	/*true then store the file name into structure variable*/
	music -> mp3_fname = argv;
	return m_success;
    }
    else
    {
	printf("! Provided a wrong input file !!\nPlease provide a .mp3 file\nExample : sample.mp3\n");
	m_failure;
    }
}

Status Open_files_view(Music *music)
{
    /*opening the file in write and read mode*/
    music->fptr_fname = fopen(music-> mp3_fname,"r+");

    if(music->fptr_fname == NULL)
    {
	perror("fopen");
	fprintf(stderr, "\nERROR: Unable to open file %s\n", music->mp3_fname);
	return m_failure;
    }
    else
    {
	return m_success;
    }

}
Status check_header_and_version(FILE *fname)
{
    /*set the offset position to 0*/
    fseek(fname,0,SEEK_SET);
    char header[4];
    char ver[2];
    /*reading the ID3 tag which is 3 bytes*/
    fread(header,1,3,fname);
    header[4] = '\0';
    /*reading the version which is 2 bytes*/
    fread(ver,1,2,fname);
    /*comparing if it is ID3V2 or not 0X03 00 represents V2*/
    if(strcmp(header,"ID3") == 0  && ver[0] == 0X03 && ver[1] == 0X00) 
    {
	return m_success;
    }
    return m_failure;
}
Status readInfo(Music *music)
{
    /*set offset pos to 0 again*/
    rewind(music->fptr_fname);

    /*Skipping the header which is 10 bytes */
    fseek(music->fptr_fname,10,SEEK_CUR);

    /*to read the tags running loop for 6 times*/
    for(int i = 0 ; i < 6 ; i++)
    {
        TagReader t = tagreader(music->fptr_fname);
 	switch(t)
	{
             /*when tag is Title*/
	    case m_tit2 :
		/*storing the positon of tags*/	
		music -> pos[m_tit2] = ftell(music->fptr_fname);

		if( tagsizereader(music->fptr_fname,&music->title_size) != m_success)
                {
                       
                         printf("Error reading title size\n");
                         return m_failure;
                }
		fseek(music->fptr_fname,3,SEEK_CUR);
		readtaginfo(music->fptr_fname,&music->Title,music->title_size);
		break;
		/*when tag is Artist*/
	    case m_tpe1 :
		music -> pos[m_tpe1] = ftell(music->fptr_fname);
		if( tagsizereader(music->fptr_fname,&music->artist_size) != m_success)
                {
                          
                        printf("Error reading artist size\n");
                        return m_failure;
                }
		fseek(music->fptr_fname,3,SEEK_CUR);
		readtaginfo(music->fptr_fname,&music->Artist,music->artist_size);
		break;
		/*Album*/
	    case m_talb:
		music -> pos[m_talb] = ftell(music->fptr_fname);
		if( tagsizereader(music->fptr_fname,&music->album_size) != m_success)
                {
                          
                         printf("Error reading album size\n");
                         return m_failure;
                }
		fseek(music->fptr_fname,3,SEEK_CUR);
		readtaginfo(music->fptr_fname,&music->Album,music->album_size);
		break;
		/*for year*/
	    case m_tyer:
		music -> pos[m_tyer] = ftell(music->fptr_fname);
		tagsizereader(music->fptr_fname,&music->year_size);
		fseek(music->fptr_fname,3,SEEK_CUR);
		readtaginfo(music->fptr_fname,&music->Year,music->year_size);
		break;
		/*Genre*/
	    case m_tcon:
		music -> pos[m_tcon] = ftell(music->fptr_fname);
		tagsizereader(music->fptr_fname,&music->genre_size);
		fseek(music->fptr_fname,3,SEEK_CUR);
		readtaginfo(music->fptr_fname,&music->Genre,music->genre_size);
		break;
		/*comment*/
            case m_tcom:
		music -> pos[m_tcom] = ftell(music->fptr_fname);

		if( tagsizereader(music->fptr_fname,&music->comment_size) != m_success)
                {
                	printf("Error reading comment size\n");
                        return m_failure;
                }
		fseek(music->fptr_fname,3,SEEK_CUR);
		if( readtaginfo(music->fptr_fname,&music->Comment,music->comment_size) != m_success)
                { 
                        printf("Error reading comment data\n");
                         return m_failure;
                }
		break;    
    	    default:
		break;
	}
    }
    return m_success;
}


TagReader tagreader(FILE *fptr)
{
    char tag[5];
    /*reading the Tag*/
    fread(tag,1,4,fptr);
    tag[4] = '\0';
    /*comparing the tags and returning status*/
    if(!strcmp(tag,"TIT2"))
	return m_tit2;
    else if(!strcmp(tag,"TALB"))
	return m_talb;
    else if(!strcmp(tag,"TPE1"))
	return m_tpe1;
    else if(!strcmp(tag,"TYER"))
	return m_tyer;
    else if(!strcmp(tag,"TCON"))
	return m_tcon;
    else if(!strcmp(tag,"COMM"))
	return m_tcom;
}
Status tagsizereader(FILE * fptr,int *size)
{
    unsigned char ch;
    *size = 0;
    /*Size is in 4 characters means 4 bytes*/
    for(int i = 0 ; i < 4 ; i++)
    {
	/*reading one character from file*/
	fread(&ch,1,1,fptr);
 
	*size = *size <<  7 | ( ch & 0x7F ) ;
        printf("size-->%d\n",*size) ;
    }
    return m_success ;
}
Status printInfo(Music *music)
{

    printf("\n........................Selected View Details...................\n");
    printf("\n................................................................\n");
    printf("\n                MP3 TAG READER AND EDITOR FOR ID3v2          \n");
    printf("\n................................................................\n\n");
    printf("TITLE          : %s \n", music->Title);
    printf("ARTIST         : %s \n", music->Artist);
    printf("ALBUM          : %s \n", music->Album);
    printf("YEAR           : %s \n", music->Year);
    printf("MUSIC          : %s \n", music->Genre);
    printf("COMMENT        : %s \n", music->Comment);
    printf("\n................................................................\n\n");
    printf("\n........................Details Displayed Successfully................\n\n");
    return m_success;
}
Status readtaginfo(FILE *fptr, char **name, int size)
{
    *name = malloc(size + 1); // Allocate memory for the string
    if (*name == NULL) // Check if memory allocation succeeded
    {
        return -1; // Return error if memory allocation fails
    }

    for (int i = 0; i < size - 1; i++) 
    {
        int ch = fgetc(fptr); // Read one character at a time
        if (ch == EOF) 
        {
            (*name)[i] = '\0'; // Null-terminate if EOF is encountered
            return -1; // Return error status for EOF
        }
        (*name)[i] =( char) ch; // Store the character in the string
    }

    (*name)[size - 1] = '\0'; // Null-terminate the string after reading

    return 0; // Return success status
}

