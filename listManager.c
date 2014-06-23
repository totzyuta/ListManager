#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE_LEN 1024	// Maximum Length of a Line
#define MAX_STR_LEN 69		// Maximum Length of Name and Address
#define MAX_PROFILES 10000	// Maximum number of elements

struct date{
	int y, m, d;
};

struct profile{
	int id;
	char name[MAX_STR_LEN+1];
	struct date birth;
	char home[MAX_STR_LEN+1];
	char *comment;
};

struct profile profile_data_store[MAX_PROFILES];    // global variable 
int profile_data_nitems = 0;    // number of elements (for %C)

/* Function Prototype */
// int get_line(char, char);
// int parse_line(char);

int subst(char *str, char c1, char c2)
{
	int n=0;
	while(*str!='\0'){
		if(*str == c1){
			*str=c2;
			n++;
		}
		*str++;
	}
	return n;	// return the times to change
}

/*文字列操作関数','を'\0'へ*/
int split(char *str, char *ret[], char sep, int max)
{
	int n=0;

	ret[n]=str;
	n = n + 1;

	while(*str && n < max){
		if(*str == sep){
			*str = '\0'; 
			ret[n] = str + 1;
		n++;
		}
		str++;
	}
	return n;
}

/*一行を読み込みsubstへ送る*/
/* Extended for cmd_read() */
int get_line(char *fp, char *line)
{
	if(fgets(line,1025,stdin) == NULL){
		return 0;
	}
	subst(line,'\n','\0');
	return 1;
}

 /**
  * Create a new date into D from STR like "2004-05-02".
  * return: struct date *D itself
  */
/*Birthの分割，数値の代入*/
struct date *new_date(struct date *d, char *str)
{
	char *ptr[3];

	if (split(str, ptr, '-', 3) != 3)
		return NULL;

	d->y = atoi(ptr[0]);
	d->m = atoi(ptr[1]);
	d->d = atoi(ptr[2]);

	return d;
}

/**
* Create a new profile into P from CSV string like
*  "0,Takahashi Kazuyuki,1977-04-27,Saitama,Fukuoka Softbank Hawks".
* return: struct profile *P itself
*/
/*CSV形式の一文を分割，文字列，値の代入・格納*/
struct profile *new_profile(struct profile *p, char *csv)
{
	char *ptr[5];

	if (split(csv, ptr, ',', 5) != 5)
		return NULL; 

	/* ID: id */
	p->id = atoi(ptr[0]); 

	/* Name of School: name */
	strncpy(p->name, ptr[1], MAX_STR_LEN);  
	p->name[MAX_STR_LEN] = '\0';

	/* Date of foundation: birthday */
	if (new_date(&p->birth, ptr[2]) == NULL)
		return NULL; /* format error */

	/* Address: home */
	strncpy(p->home, ptr[3], MAX_STR_LEN);//上に同じ．  
	p->home[MAX_STR_LEN] = '\0';

	/* Other Information */
	/*malloc：指定バイト分の，メモリ領域を確保
	strlen(ptr[4])+1分のメモリを確保して先頭アドレスを返す．
	返却された先頭アドレスを(char*)とみなしp->commentに代入*/
	p->comment = (char *)malloc(sizeof(char) * (strlen(ptr[4])+1));	
	strcpy(p->comment, ptr[4]);

	return p;
 }


/* Exit This Program */
void cmd_quit()
{
	exit(0); 
}

/* Print the number of elements */
void cmd_check()
{
	printf("登録件数：%d件\n",profile_data_nitems);
}

/* Print n elements from ahead (n = 0: all elements, n<0: from behind) */
char *date_to_string(char buf[], struct date *date)
{
	sprintf(buf, "%04d-%02d-%02d", date->y, date->m, date->d);
	return buf;
}

void print_profile(int i,struct profile *p) // print n elements
{
	char date[11];

	printf("(line%d)\n",i+1);
	printf("Id    : %d\n", p->id);
	printf("Name  : %s\n", p->name);
	printf("Birth : %s\n", date_to_string(date, &p->birth));
	printf("Addr  : %s\n", p->home);
	printf("Com.  : %s\n", p->comment);
}

void cmd_print(int n)   // judge n and switch by n/loop processes
{
	int i; 
	if(n>0 && n<=profile_data_nitems){
		for(i=0;i<n;i++){
			print_profile(i,&profile_data_store[i]);
			printf("\n");
		}
	}else if(n==0){
		for(i=0;i<profile_data_nitems;i++){
			print_profile(i,&profile_data_store[i]);
			printf("\n");
		}
	}else if(n<0 && (-n)<=profile_data_nitems){
		for(i=(profile_data_nitems+n);i<profile_data_nitems;i++){
			print_profile(i,&profile_data_store[i]);
			printf("\n");
		}
	}else{
		printf("Error: comfirm the number of elements\n\n");
	}
}

int cmd_read(char *filename)
{
	char line[MAX_LINE_LEN + 1];
	
	FILE *fp;
	
	fp = fopen(filename, "w");
	
	if (fp == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		return -1;
	}
	fprintf(fp, "Hello World!\n");

	/*
	while (get_line(fp, line)) {
		parse_line(line);
	}
	*/

	fclose(fp);
	return 0;
}

/* print_profileの代わりにCSV形式で出力するfunction */
int fprint_profile_csv(struct profile *p)
{
	char date[11];
	// change to print by type of CSV
	/*printf("(line%d)\n",i+1);
	printf("Id    : %d\n", p->id);
	printf("Name  : %s\n", p->name);
	printf("Birth : %s\n", date_to_string(date, &p->birth));
	printf("Addr  : %s\n", p->home);
	printf("Com.  : %s\n", p->comment);
	*/
}

int cmd_write(char *filename)
{
	FILE *fp;

	fp = fopen(filename, "w");

	if (fp == NULL) {
		fprintf(stderr, "Could not open file: %s\n", filename);
		return -1;
	}

	for (int i=0; i<profile_data_nitems; i++) {
		fprint_profile_csv(&profile_data_store[i]);
	}

	fclose(fp);
	return 0;
}

/**
 * Command %F word
 */
void cmd_find()
{
  int i;
  
  /* change int data to cahr data
  char *word = "3";
  int id = 3;
  char s[4];
  sprintf(s, "%d", id);   // save as char array
  if (strcmp(s, word) == 0) {
  
  }
  */
  
  int w = atoi(word);

  for (i=0; i<profile_data_nitems; i++) {
    p = &profile_data_store[i];

    if (strcmp(p->name, word) == 0 ||
        strcmp(p->home, word) == 0 ||
        strcmp(p->comment, word) == 0 ||
        id == w) {
      print_profile(p);
      printf("\n");
    }
  }
}

void cmd_sort()
{
  void swap(int *a, int *b)
  {
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
  }

  /**
   * stupid sort for testing.
   */
  void b_sort(int p[], int left, int right)
  {
    int i, j;
    
    for (i = left; i <= right; i++) {
      for (j = left; j <= right - 1; j++) {
        if (p[j] > p[j+1])
          swap(&p[j], &p[j+1]);
      }
    }
  }
}

/* judge inputed command and carry on commands/pass parameter */
void exec_command(char cmd, char *param)
{
	switch(cmd){
		case'Q':cmd_quit();  break;
		case'C':cmd_check(); break;
		case'P':cmd_print(atoi(param)); break;	// atoi: string to integer
		case'R':cmd_read(param); break;
		case'W':cmd_write(param); break;
		case'F':cmd_find(param); break;
		case'S':cmd_sort(atoi(param)); break;
		default:printf("error: input correct command\n");
	}
}

/*一行を読み込み，コマンド，CSV形式の解析．それぞれの値をexec_command,new_profileに送る．*/
int parse_line(char *line)
{
	int cmd;
	char *param;
	if(*line == '%'){
		cmd = line[1];
		param = &line[3];
		exec_command(cmd,param);
	}else if(profile_data_nitems == MAX_PROFILES){
		printf("登録限度を越えています．\n");
	}else if(line[0] == '\0'){
		return 0;
		//printf("muri");
	} else {
		new_profile(&profile_data_store[profile_data_nitems++],line);
/*profile_data_store[profile_data_nitems]の先頭アドレスを送り，nitemsを1増やす．*/
	}
}

/*一行を読み込み，perse_lineへ送る．*/
int main()
{
	int n=0;
	char line[MAX_LINE_LEN+1];
	while (get_line(stdin, line)){
		parse_line(line);
	}
	return 0;
}
