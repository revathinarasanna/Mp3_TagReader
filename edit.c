#include "edit.h"

Status editinfo(Music *music , char *ch , char *name)
{
	if(Open_files_view(music) == m_success)
	{
		FILE *fptr_dest = fopen("sample.mp3","wb+");

		if(fptr_dest == NULL)
		{
			perror("fopen");
			fprintf(stderr, "ERROR: Unable to open file %s\n", "sample.mp3");
			return m_failure;

		}

		if(check_header_and_version(music->fptr_fname) == m_success)
		{
			if(readInfo(music) == m_success)
			{
				renametag(music,ch,name,fptr_dest);
      				copytoriginal(music->fptr_fname,fptr_dest);
                                // Print the selected edit details
                                printf("\n\n-----------------------------------selected edit details----------------------------------------\n\n");
                		printf("-------------------------------------Selected edit option------------\n\n");

                		// Display appropriate message based on the edit option
                		if (strcmp(ch, "-t") == 0)
                		{
                    			printf("------------------------------Change the title-------\n\n");
                    			printf("Title: %s\n\n", name);
                    			printf("----------------------------Title changed successfully----------------------\n\n");
                		}
                		else if (strcmp(ch, "-a") == 0)
                		{
                    			printf("--------------------------------Change the album-------\n\n");
                    			printf("Album: %s\n\n", name);
                    			printf("---------------------------Album changed successfully--------------------\n\n");
                		}
                                else if (strcmp(ch, "-A") == 0)
                                {
                    			printf("--------------------------------Change the artist-------\n\n");
                    			printf("Artist: %s\n\n", name);
                    			printf("-------------Artist changed successfully--------------------------------\n\n");
                		}
                		else if (strcmp(ch, "-y") == 0)
                		{
                    			printf("--------------------------------Change the year-----------\n\n");
                    			printf("Year: %s\n\n", name);
                    			printf("-------------Year changed successfully----------------------\n\n");
                		}
                		else if (strcmp(ch, "-m") == 0)
                		{
                    			printf("------------------------------Change the genre-------\n\n");
                    			printf("Genre: %s\n\n", name);
                    			printf("-------------Genre changed successfully\n\n");
                		}
                     		else if (strcmp(ch, "-c") == 0)
                		{
                   			 printf("----------------------------Change the comment-------\n\n");
                    			 printf("Comment: %s\n\n", name);
                    			 printf("-------------Comment changed successfully---------------\n\n");
                		}
                		else
                		{
                    			printf("Unknown edit option!\n");
                		}
            		}
               }
    }
    return m_success;
}

Status renametag(Music *music,char *ch , char *name , FILE *fptr_dest)
{
 //   rewind(music->fptr_fname);
	if(copyheader(music->fptr_fname,fptr_dest) == m_success)
	{
		switch(checkedit(ch))
		{
			case t:
				printf("Entered to edit title t\n");
				copyContent(music->fptr_fname, music->pos[m_tit2], fptr_dest);
				copynewcon(music->fptr_fname, name, fptr_dest);
				copyremaining(music->fptr_fname, music->title_size, fptr_dest);
				break;
			case a:
				copyContent(music->fptr_fname, music->pos[m_talb], fptr_dest);
				copynewcon(music->fptr_fname, name, fptr_dest);
				copyremaining(music->fptr_fname, music->album_size, fptr_dest);
				break;
			case A:
				copyContent(music->fptr_fname, music->pos[m_tpe1], fptr_dest);
				copynewcon(music->fptr_fname, name, fptr_dest);
				copyremaining(music->fptr_fname, music->artist_size, fptr_dest);
				break;
			case y:
				copyContent(music->fptr_fname, music->pos[m_tyer], fptr_dest);
				copynewcon(music->fptr_fname, name, fptr_dest);
				copyremaining(music->fptr_fname, music->year_size, fptr_dest);
				break;
			case m:
				copyContent(music->fptr_fname, music->pos[m_tcon], fptr_dest);
				copynewcon(music->fptr_fname, name, fptr_dest);
				copyremaining(music->fptr_fname, music->genre_size, fptr_dest);
				break;
			case c:
				copyContent(music->fptr_fname, music->pos[m_tcom], fptr_dest);
				copynewcon(music->fptr_fname, name, fptr_dest);
				copyremaining(music->fptr_fname, music->comment_size, fptr_dest);
				break;
			default:
				break;

		}
	}
	return m_success;
}

Status copyContent(FILE *src, int pos , FILE *dest)
{
	char ch;
	for(int i = 10 ; i < pos ; i++)
	{
		fread(&ch,1,1,src);
		fwrite(&ch,1,1,dest);
	}
}

Status copynewcon(FILE *src, char *name, FILE *dest)
{
	int len = strlen(name) + 1;
	char bytes[4];
	for(int i = 0 ; i < 4 ; i++)
	{
		bytes[3 - i] = ((len >> 8 * i) & 0XFF);
	}
	fwrite(bytes,1,4,dest);
	fseek(src,4,SEEK_CUR);
	char buffer[3];
	fread(buffer,1,3,src);
	fwrite(buffer,1,3,dest);
	fwrite(name,len - 1,1,dest);
	return m_success;
}

Status copyremaining(FILE *src , int size , FILE *dest)
{
	fseek(src,size-1,SEEK_CUR);
	char ch;
	while(fread(&ch,1,1,src))
	{
		fwrite(&ch,1,1,dest);
	}
	return m_success;
}

Status copytoriginal(FILE *src,FILE *dest)
{

	rewind(src);
	rewind(dest);
	char ch;
	while(fread(&ch,1,1,dest) > 0)
	{
		fwrite(&ch,1,1,src);
	}
	fclose(dest);
	remove("sample.mp3");
	return m_success;
}

Status copyheader(FILE *fptr_src,FILE *fptr_dest)
{
	rewind(fptr_src);
	
	char buffer[10];
	
	fread(buffer,1,10,fptr_src);

	
	fwrite(buffer,1,10,fptr_dest);
	
	return m_success;
}

edittags checkedit(char *ch)
{
	if(!strcmp(ch,"-t"))
		return t;
	if(!strcmp(ch,"-a"))
		return a;
	if(!strcmp(ch,"-A"))
		return A;
	if(!strcmp(ch,"-y"))
		return y;
	if(!strcmp(ch,"-m"))
		return m;
	if(!strcmp(ch,"-c"))
		return c;
}

