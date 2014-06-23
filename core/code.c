#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COMMENT_NUM 64
#define MAX_COMMENT_TAG_LEN 8
#define COMMENT_TAG      ","
#define COMMENT_DELIMTOR "|"

struct code_comment_t {
	char start[MAX_COMMENT_TAG_LEN];
	char rear[MAX_COMMENT_TAG_LEN]; //if rear is empty, then this comment is line comments.
};


static char *strstrip(char *str)
{
	int len   = 0;
	char *tmp = NULL;

	while(*str != 0 && isspace(*str))str++;
	if(*str == 0) {
		return str;
	}

	len = strlen(str);
	tmp = str + len - 1;
	while(isspace(*tmp))tmp--;
	
	tmp++;
	*tmp = 0;
	return str;
}

static char *blank_string_pair(char* str, char str_char)
{
	char *tmpl = NULL;
	char *tmpr = NULL;
	while(1) {
		if((tmpl = strchr(str, str_char)) == NULL) {
			return str;
		}

		if((tmpr = strchr(tmpl+1, str_char)) == NULL) {
			return str;
		}
		
		memset(tmpl+1, ' ', tmpr - tmpl);
		str = tmpr + 1;
	}
	
	return NULL;
}

static int parse_comment_pair(const char *comm, struct code_comment_t *lpcom)
{
	int len = 0;
	int index = 1, max = 1, num = 0;
	char buff[256] = {0};
	char *argv[MAX_COMMENT_NUM] = {NULL};


	strncpy(buff, comm, sizeof(buff));
	
	argv[0] = strtok(buff, COMMENT_DELIMTOR);
	while((argv[max] = strtok(NULL, COMMENT_DELIMTOR)) != NULL && max < MAX_COMMENT_NUM)max++;

	for(index = 0; index < max; index++) {
		//argv[index] = strstrip(argv[index]);
		argv[index] = strtok(argv[index], COMMENT_TAG);
		argv[index] = strstrip(argv[index]);

		if(*(argv[index]) == 0) {
			continue;
		}

		strncpy(lpcom->start, argv[index], sizeof(lpcom->start));

		if((argv[index] = strtok(NULL, COMMENT_TAG)) != NULL) {
			argv[index] = strstrip(argv[index]);
			strncpy(lpcom->rear, argv[index], sizeof(lpcom->rear));
		} else {
		       memset(lpcom->rear, 0, sizeof(lpcom->rear));
		}	       

		lpcom++;
		num++;
	}

	return num;
}

int calc_code_lines(const char *filename, const char *lpcomment,  
			int *total, int *code, int *comment, int *blank)
{
	FILE *fp = fopen(filename, "r");
	char line[4096] = {0};
	char *str = NULL;
	char *tmp = NULL;
	struct code_comment_t comm[MAX_COMMENT_NUM] = {0};
	struct code_comment_t *pcur = NULL;
	int comm_num = 0, index = 0;

	if((comm_num = parse_comment_pair(lpcomment, comm)) <= 0) {
		fclose(fp);
		return -1;
	}

	*total = *code = *comment = *blank = 0;

	while(fgets(line, sizeof(line), fp) != NULL) {
		blank_string_pair(line, '\"');
		blank_string_pair(line, '\'');
		(*total) ++;
		str = line;

		if(pcur != NULL) {
			(*comment)++;
			if((str=strstr(str, pcur->rear)) == NULL) {
				continue;
			}

			str += strlen(pcur->rear);
			pcur = NULL;
			str = strstrip(str);
		
			if(*str == 0) { 
				continue;
			}
		}

		str = strstrip(str);
		if(*str == 0) {
			(*blank)++;
			continue;
		}

		for(index = 0; index < comm_num; index ++) {
			str = strstrip(str);
			if(*str == 0) {
				break;
			}

			if((tmp=strstr(str, comm[index].start)) == NULL) {
				continue;
			}

			if(tmp != str) {
				(*code)++;
			}

			(*comment)++;
			if(comm[index].rear[0] == 0) {
				break;
			}

			if((tmp=strstr(tmp+strlen(comm[index].start), comm[index].rear)) != NULL) {
				str = tmp+strlen(comm[index].rear);
				index = 0;
				continue;
			}

			pcur = comm + index;
			break;
		}

		if(index < comm_num) {
			continue;
		}

		(*code)++;
	}

	fclose(fp);
	return  0;
}

int main(int argc, char *argv[])
{
	int total, code, cmt, blank;
	calc_code_lines(argv[1], "/*-*/,//", &total, &code, &cmt, &blank);
	printf("file:%s, total:%d, code:%d, comment:%d, blank:%d\n", argv[1], total, code, cmt, blank);
	return 0;
}
