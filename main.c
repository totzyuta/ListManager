/* Ideas
 * - make a manual of commands
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 69 
#define MAX_PROFILES 10000

/* Global Variables */
int profile_data_ntimes = 0;	// Number of elements in profile_data_store 

struct profile profile_data_store[MAX_PROFILES];

struct date 
{
	int y, m, d;
};

struct profile 
{
	int id;
	char name[MAX_STR_LEN + 1];
	struct date birthday;
	char home[MAX_STR_LEN + 1];
	char *info;
};

/**
 * Create a new date into D from STR like "2004-05-02".
 * return: struct date *D itself
 * */
struct date *new_date(struct date *d, char *str)
{
	char *ptr[3];

	if (split(str, ptr, '-', 3) != )
		return NULL;

	d->y = atoi(ptr[0]);
	d->m = atoi(ptr[1]);
	d->d = atoi(ptr[2]);

	/* XXX: more code is to be here to see if y, m, and d are valid. */

	return d;
}

/**
 * Create a new profile into P from CSV string like
 * "0,Takahashi Kazuyuki,1977-04-27,Saitama,Fukuoka Softbank Hawks".
 * return: struct profile *P itself
 * */
struct profile *new_profile(struct profile *p, char *csv)
{
	char *ptr[5];

	if (split(csv, ptr, ',', 5) != 5)	// <-
		return NULL;	/* format error */

	/* ID: id */
	p->id = atoi(ptr[0]);	// <-

	/* School Name: name */
	strncpy(p->name, ptr[1], MAX_STR_LEN);	// <-
	p->name[MAX_STR_LEN] = '\0';

	/* Birthday: birthday */
	if (new_date(&p->birthday, ptr[2]) == NULL)	// <-
		return NULL;	/* format error */

	/* Address: home */
	strncpy(p->home, ptr[3], MAX_STR_LEN);	// <-
	p->home[MAX_STR_LEN] = '\0';

	/* Other Information: info */
	p->info = (char *)malloc(sizeof(char) * (strlen(ptr[4]) + 1));
	strcpy(p->info, ptr[4]);

	return p;
}

void cmd_quit()		// Exit Program 
{
	printf("Command: Quit\n");
	exit(0);
}

void cmd_check()	// Check registered contents
{
	printf("Command: Check\n");
}

void cmd_print(char *prm)	// Print "n" elements from head
{
	printf("Command: print\n");
	printf("Prm = %s\n", prm);
}

void cmd_read(char *prm)	// Read "file"
{
	printf("Command: read\n");
	printf("Prm = %s\n", prm);
}

void cmd_write(char *prm)	// Write to "file"
{
	printf("Command: write\n");
	printf("Prm = %s\n", prm);
}

void cmd_find(char *prm)	// Search and Print the result
{
	printf("Command: find\n");
	printf("Prm = %s\n", prm);
}

void cmd_sort(char *prm)	// CSVのn番目の項目で整列(表示はしない）
{
	printf("Command: sort\n");
	printf("Prm = %s\n", prm);
}

/* コマンド文字と引数で示された処理を実行 */
int exec_command(char cmd, char *prm)
{
	switch (cmd) {
		case 'Q': cmd_quit();		break;
		case 'C': cmd_check();		break;
		case 'P': cmd_print(prm);	break;
		case 'R': cmd_read(prm);	break;
		case 'W': cmd_write(prm);	break;
		case 'F': cmd_find(prm);	break;
		case 'S': cmd_sort(prm);	break;
		default:
			 // error processing 
		break;
	}
}

/* 行をコンマで５つの文字列に分割 */
int split(char *str, char *ret[], char sep, int max)
{
	int i = 0;
	while (*str) {
		if (str[i] != ',') {
    			ret[i] = str;
  		} else {
			ret[i] = '\0';
			i++;
		}
		str++;
	}
	return ret;
}

/* ５つの文字列を項目毎に構造体や配列に保存 */
int new_profile(int np, char *line)
{
	/* use split_test function for the test of parse_line()
	split();
	構造体profileの各メンバについて:
		1番目の文字列をid	に	数値	として代入;
		2番目の文字列をname	に	文字列	として代入;
		3番目の文字列をbirthdayに	日付	として代入;
		4番目の文字列をhome	に	文字列	として代入;
		5番目の文字列をcomment	に	任意長の文字列として代入;
	*/
	
	// split_test()
	int i, cols;
	char *ret[5];

	cols = split(line, ret, ',', 5);

	for (i=0; i<cols; i++) {
		printf("	RET[%d]='%s'\n", i, ret[i]);
	}
	printf("\n");
	return 1;
}

#define MAX_LINE_LEN 1024

int subst(char *str, char c1, char c2)
{
	int n = 0;

	while (*str) {
		if (*str == c1) {
			*str = c2;
			n++;
		}
		str++;
	}
	return n;
}


/**
 * Read at most the number of characters
 * specified by MAX_LINE_LEN and store them in the string LINE including '\n'.
 * LINE must have a room for MAX_LINE_LEN+1 chars (additional 1 is for \0).
 */

/* 標準入力から１行読み込む */
int get_line(char *line)
{
  if (fgets(line, MAX_LINE_LEN + 1, stdin) == NULL)
    return 0; /* failed or EOF */
  
  subst(line, '\n', '\0');
  
  return 1; /* succeeded */
}

/* 読み込んだ行を処理する */
int parse_line(char *line)
{
	if (line[0] == '%') {
		exec_command(line[1], &line[3]);
	} else {
		new_profile(&profile_data_store[profile_data_ntimes++], line);
	}
	return 1;	// succeeded 
}

/* CSVの行を処理する */
int parse_csv()
{

}

int main()
{
	/* test
	p.id = 94245566;
	strncpy(p.name, "Yuta Totsuka", 70);
	d.y = 2012;
	d.m = 5;
	d.d = 1;
	strncpy(p.place, "Kurashiki Okayama", 70);
	new_pr(&p, "Information");
	
	printf("Name: %s\n", (*profile_data_store).name);
	*/

	char line[MAX_LINE_LEN + 1];
	while (get_line(line))
	{
		parse_line(line);
	};
	return 0;
}
