#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include "regex_word.h"

#define MAX_STRING_LEN        (50)
#define REGEX_FLAGS           (REG_EXTENDED | REG_ICASE | REG_NEWLINE)

//list struct
typedef struct word_list_s
{
	char                 str[MAX_STRING_LEN];
	struct word_list_s*  next_p;
} word_list_s;

//show menu and get choice
int GetChoice(char* chr)
{
	int ch;
	char* option_list = "SFQ";
	
	//show menu
	puts("Please select from the following options:");
	puts("S:store a new word");
	puts("F:find a word");
	puts("Q:quit");
	
	//get input
	while((ch = getchar()) != EOF)
	{
		//jump all the other letters
		while(getchar() != '\n')
		{
			continue;
		}
		
		//turn to cap
		ch = toupper(ch);
		
		//check
		if(strchr(option_list, ch) == NULL)
		{
			puts("Unexpected input found, please input again.");
		}
		else
		{
			break;
		}
	}
	
	if(ch == EOF)
	{
		ch = 'Q';
	}
	
	*chr = ch;
	
	return 0;
}

//get a word from input and check
int GetWord(char* wd)
{
	char temp_str[MAX_STRING_LEN];
	int word_len, idx;
	bool jump_out;
	
	memset(temp_str, 0, MAX_STRING_LEN);
	
	jump_out = false;
	
	//show indication
	puts("Please input a new word.");
	
	while(1)
	{
		//get input string
		if(gets(temp_str) == NULL)
		{
			//wrong happens
			puts("Fail to read word input.");
			fprintf(stderr, "Fail to read word input.\n");
			exit(1);
		}
		
		//check word format
		word_len = strlen(temp_str);
		
		if(word_len == 0)
		{
			puts("You did not input a word, please input again.");
			
			continue;
		}
		
		jump_out = true;
		
		for(idx = 0; idx < word_len; idx++)
		{
			//is a letter
			if(isalpha(temp_str[idx]))
			{
				if(idx > 0)
				{
					temp_str[idx] = tolower(temp_str[idx]);
				}
			}
			//is a '''
			else if((idx > 0) && (temp_str[idx] == '\''))
			{
				//do nothing
			}
			//need to input again
			else
			{
				puts("Not a word, please input again.");
				jump_out = false;
				
				break;
			}
		}
		
		//jump out
		if(jump_out == true)
		{
			break;
		}
	}
	
	//copy str
	//printf("Get word: %s", temp_str);
	//remeber to set the tail to 0 
	strncpy(wd, temp_str, word_len);	
	wd[word_len] = '\0';
	
	return 0;
}

int GetRegex(char* str)
{
	char temp_str[MAX_STRING_LEN];
	int word_len, idx;
	bool jump_out;
	char* regex_head = "^\\w*'*(";
	char* regex_tail = ")+\\w*'*$";
	
	memset(temp_str, 0, MAX_STRING_LEN);
	
	jump_out = false;
	
	//show indication
	puts("Please input several letters.");
	
	while(1)
	{
		//get input string
		if(gets(temp_str) == NULL)
		{
			//wrong happens
			puts("Fail to read letters input.");
			fprintf(stderr, "Fail to read letters input.\n");
			exit(1);
		}
		
		//check word format
		word_len = strlen(temp_str);
		
		if(word_len == 0)
		{
			puts("You did not input any letter, please input again.");
			
			continue;
		}
		
		jump_out = true;
		
		for(idx = 0; idx < word_len; idx++)
		{
			//is a letter
			if(isalpha(temp_str[idx]))
			{
				temp_str[idx] = tolower(temp_str[idx]);
			}
			//is a '''
			else if(temp_str[idx] == '\'')
			{
				//do nothing
			}
			//need to input again
			else
			{
				puts("Not a letter, please input again.");
				jump_out = false;
				
				break;
			}
		}
		
		//jump out
		if(jump_out == true)
		{
			break;
		}
	}
	
	printf("GetRegex 0.\n");
	printf("word len: %d", word_len);
	
	word_len = 0;
	//copy regex head
	strncpy(str, regex_head, strlen(regex_head));
	word_len += strlen(regex_head);
	str[word_len] = '\0';
	
	//copy str
	strncat(str, temp_str, strlen(temp_str));
	word_len += strlen(temp_str);
	
	//regex tail
	strncat(str, regex_tail, strlen(regex_tail));
	word_len += strlen(regex_tail);
	str[word_len] = '\0';
	
	printf("Get regex: %s", str);
	printf("\n");
	
	return 0;
}

int StoreWord(char* wd, char* fil)
{
	FILE* fp = NULL;
	char  tmp_str[MAX_STRING_LEN], last_str[MAX_STRING_LEN];
	word_list_s *next_list_p, *curr_list_p, *temp_list_p, *list_head;
	int temp;
	
	memset(tmp_str, 0, MAX_STRING_LEN);
	memset(last_str, 0, MAX_STRING_LEN);
	next_list_p = NULL;
	curr_list_p = NULL; 
	temp_list_p = NULL; 
	list_head = NULL;
	
	//check word len
	if(wd == NULL)
	{
		puts("Bad word address.");
		exit(1);
	}
	
	if(strlen(wd) == 0)
	{
		puts("Empty word.");
		exit(1);
	}
	
	if(fil == NULL)
	{
		puts("Bad file address.");
	}
	
	//open word file
	if((fp = fopen(fil, "r")) == NULL)
	{
		puts("Fail to open to read word file.");
		exit(1);
	}
	
	//printf("StoreWord - 0\n");
	
	list_head = NULL;
	
	//read out each string and compare
	while((fgets(tmp_str, MAX_STRING_LEN - 1, fp)) != NULL)
	{
		next_list_p = (word_list_s*)malloc(sizeof(word_list_s));
		memset(next_list_p, 0, sizeof(word_list_s));
		strncpy(next_list_p->str, tmp_str, strlen(tmp_str));
		next_list_p->str[strlen(tmp_str)] = '\0';
		next_list_p->next_p = NULL;
		////printf("next str: %s", next_list_p->str);
		
		//set "\n" in tmp_str to '\0'
		temp = strlen(tmp_str);
		temp--;
		tmp_str[temp] = '\0';
		
		//compare word with this string and the last_str
		if((strcmp(wd, last_str) > 0) && (strcmp(wd, tmp_str) < 0))
		{
			//insert in between
			temp_list_p = (word_list_s*)malloc(sizeof(word_list_s));
			memset(next_list_p, 0, sizeof(word_list_s));
			strncpy(temp_list_p->str, wd, strlen(wd));
			temp_list_p->str[strlen(wd)] = '\0';
			//add a '\n'
			strcat(temp_list_p->str, "\n");
			temp_list_p->next_p = next_list_p;
			//printf("temp str: %s", temp_list_p->str);
			
			if(list_head == NULL)
			{
				//printf("StoreWord - 0a\n");
				list_head = temp_list_p;
			}
			
			if(curr_list_p != NULL)
			{
				//printf("StoreWord - 0b\n");
				curr_list_p->next_p = temp_list_p;
			}
		}
		else
		{
			if(list_head == NULL)
			{
				//printf("StoreWord - 0c\n");
				list_head = next_list_p;
			}
			
			if(curr_list_p != NULL)
			{
				////printf("StoreWord - 0d\n");
				curr_list_p->next_p = next_list_p;
			}
		}
		
		curr_list_p = next_list_p;
		
		strcpy(last_str, tmp_str);
	}
	
	//printf("StoreWord - 1\n");
	fclose(fp);
	//printf("StoreWord - 1a\n");
	
	//write out back
	if((fp = fopen(fil, "w")) == NULL)
	{
		puts("Fail to open to write word file.");
		exit(1);
	}
	
	//printf("StoreWord - 1b\n");
	curr_list_p = list_head;
	while(curr_list_p != NULL)
	{
		//puts(curr_list_p->str);
		fputs(curr_list_p->str, fp);
		temp_list_p = curr_list_p;
		curr_list_p = curr_list_p->next_p;
		free(temp_list_p);
	}
	//printf("StoreWord - 2\n");
	fclose(fp);
	//printf("StoreWord - 3\n");
}

int SearchForWord(char* input, char* fil)
{
	FILE* fp;
	char temp_str[MAX_STRING_LEN];
	int status;
	regmatch_t pmatch[1];
	regex_t reg;

	//check word len
	if(input == NULL)
	{
		puts("Bad regex address.");
		exit(1);
	}
	
	if(strlen(input) == 0)
	{
		puts("Empty regex.");
		exit(1);
	}
	
	if(fil == NULL)
	{
		puts("Bad file address.");
	}
		
	//open file
	if((fp = fopen(fil, "r")) == NULL)
	{
		puts("Fail to open to read word file.");
		exit(1);
	}
	
	regcomp(&reg, input, REGEX_FLAGS);
	
	while(fgets(temp_str, MAX_STRING_LEN, fp) != NULL)
	{
		status = regexec(&reg, temp_str, 1, pmatch, 0);
		
		if(status == REG_NOMATCH)
		{
			//puts("No match.");
		}
		else if (0 == status)
		{
			puts(temp_str);
			//printf("  start: %d end: %d", pmatch[0].rm_so, pmatch[0].rm_eo);
			//printf("\n");
		}
	}
	
	regfree(&reg);
	fclose(fp);
	
	return 0;
}

int main(void)
{
	char choice, temp_str[MAX_STRING_LEN];
	char* word_file = "/home/danielduan/words";
	
	while(1)
	{
		GetChoice(&choice);
	
		switch(choice)
		{
			case 'S':
				//store a word
				GetWord(temp_str);
				//printf("Get Word: %s", temp_str);
				StoreWord(temp_str, word_file);
				//StoreWord("MMMMM", word_file);
			break;
			
			case 'F':
				//find a word by regex
				GetRegex(temp_str);
				SearchForWord(temp_str, word_file);
			break;
			
			case 'Q':
				//quit
				return 0;
			break;
			
			default:
			break;
		}
	}
	
	return 0;
}