#include<string.h>
#include<stdio.h>
#include "mp3_view.h"
#include "edit.h"
int main(int argc, char *argv[])
{
	/*Declaring the structure variable in main function*/
	Music music;

	/*checking the arguments atleast 3 should be there to run */
	if(argc < 2 )
	{
		/*function to print the Error*/
		invalid_arguments();
	}
	else
	{
		/*To print the Help informantion*/
		if(check_operation_type(argv) == m_help)
		{
			print_help();
		}
		/*check if option is -v or - e*/
		else if(check_operation_type(argv) == m_view)
		{
			/*if -v validating and opening the file */
			if(read_and_validate_view_args(argv[2],&music) == m_success)
			{
				if(Open_files_view(&music) == m_success)
				{
					/*checking ID3v2 or not */
					if(check_header_and_version(music.fptr_fname)  == m_success)
					{
						/*Reading the tags and content*/
						if(readInfo(&music)==m_success)
						{
							/*printing the info about mp3*/
							printInfo(&music);
						}
					}
				}

			}
		}
                /*if option is -e then*/
		else if(check_operation_type(argv) == m_edit)
		{
			if(argc > 4 && argc < 6)
			{
				if(read_and_validate_view_args(argv[4],&music)==m_success)
				{
					/*editing the content w r to user*/
					editinfo(&music,argv[2],argv[3]);
				}
			}
			else
			{
				invalid_arguments();
			}
		}
                else
		{
                	/*function to print the Error*/
			invalid_arguments();
               }
	}
}
/*Function to print help*/
void print_help()
{
	printf("\n-----------------------------HELP MENU------------------------\n");
	printf("\n1. -v -> to view mp3 file contents\n2. -e -> to edit mp3 file contents\n\t 2.1. -t -> to edit song title\n\t 2.2 -a -> to edit artist name\n\t 2.3. -A -> to edit album name\n\t 2.4. -y -> to edit year\n\t 2.5. -m -> to edit content\n\t 2.1. -c -> to edit comment\n");

	printf("\n--------------------------------------------------------------\n");
}
/*function to error*/
void invalid_arguments()
{
	printf("-----------------------------------------------------\n");
	printf("\n");
	printf("ERROR : ./a.out : INVALID ARGUMENTS\n");
	printf("USAGE :\n");
	printf("To view please pass like : ./a.out -v mp3filename\n");
	printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
	printf("TO get help pass like : ./a.out --help\n");
	printf("-----------------------------------------------------\n");

}

/*function to check the option*/
Operationtype check_operation_type(char *argv[])
{
	if(strcmp(argv[1],"--help") == 0)
        {
		return m_help;
        }
	else if(strcmp(argv[1],"-v") == 0)
        {
               return m_view;
        }
	else if(strcmp(argv[1],"-e") == 0)
        {
		return m_edit;
        }
	else
		return m_unsupported;
}



















