#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE_LEN 1024	// 一行の最大文字数
#define MAX_STR_LEN 69		// 氏名・住所の最大文字数
#define MAX_PROFILES 10000	// 蓄積させられる最大件数

struct date{
  int y;
  int m;
  int d;
};

struct profile{
  int id;
  char name[MAX_STR_LEN+1];
  struct date birth;
  char home[MAX_STR_LEN+1];
  char *comment;
};

struct profile profile_data_store[MAX_PROFILES];/*グローバル変数*/
int profile_data_nitems = 0;/*登録%C件数を数えるためのカウンタ*/


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
  return n;//変換した回数の値を返す
}

/*文字列操作関数','を'\0'へ*/
int split(char *str, char *ret[], char sep, int max)
{
<<<<<<< HEAD
	int n=0;

	ret[n]=str;
	n = n + 1;

	while(*str && n < max){
		if(*str == sep){
			cmd_read() */
int get_line(char *fp, char *line)
=======
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
int get_line(char *line)
>>>>>>> 37e8f80b84ffc2cf6db95b683fc7ae6fb0333154
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

   /* 学校名: name */
   strncpy(p->name, ptr[1], MAX_STR_LEN);
   p->name[MAX_STR_LEN] = '\0';

   /* 設立年月日: birthday */
   if (new_date(&p->birth, ptr[2]) == NULL)
     return NULL; /* format error */

   /* 所在地: home */
   strncpy(p->home, ptr[3], MAX_STR_LEN);//上に同じ．
   p->home[MAX_STR_LEN] = '\0';

   /*備考*/
   /*malloc：指定バイト分の，メモリ領域を確保
    strlen(ptr[4])+1分のメモリを確保して先頭アドレスを返す．
   返却された先頭アドレスを(char*)とみなしp->commentに代入*/
   p->comment = (char *)malloc(sizeof(char) * (strlen(ptr[4])+1));
   strcpy(p->comment, ptr[4]);

   return p;
 }


/*終了*/
void cmd_quit()
{
  exit(0);
}
/*登録件数*/
void cmd_check()
{
  printf("登録件数：%d件\n",profile_data_nitems);
}
/*先頭からn件表示 n = 0: 全件表示, n < 0 後ろから -n件表示*/
char *date_to_string(char buf[], struct date *date)
 {
   sprintf(buf, "%04d-%02d-%02d", date->y, date->m, date->d);
   return buf;
 }


void print_profile(int i,struct profile *p)/*n件表示*/
{
 char date[11];

   printf("(line%d)\n",i+1);
   printf("Id    : %d\n", p->id);
   printf("Name  : %s\n", p->name);
   printf("Birth : %s\n", date_to_string(date, &p->birth));
   printf("Addr  : %s\n", p->home);
   printf("Com.  : %s\n", p->comment);
 }


void cmd_print(int n)/*nで場合分け，ループ処理*/
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
      printf("登録件数を確認してください．\n\n");
      }
}

void cmd_help()
{
  printf("\n# HELP\n");
  printf("## Commands\n");
  printf("- Q: Quit\n");
  printf("- C: check\n");
  printf("- P n: Print n elements (n=0:all, n<0:from behind)\n");
  printf("- R file: Read from file\n");
  printf("- W file: Write as file\n");
  printf("- F word: Search by word, print like P\n");
  printf("- S n: Sort datas as the column of n\n");
  printf("- H: Show usage of commands\n");
}

/*コマンド文字によって，適切な関数に引数などを送るり，処理を行う．*/
void exec_command(char cmd, char *param)
{
  switch(cmd){
  case'Q':cmd_quit();  break;
  case'C':cmd_check(); break;
  case'P':cmd_print(atoi(param)); break;
  case'H':cmd_help();  break;

  default:printf("error\n");
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
  while (get_line(line)){

    parse_line(line);
    }
  return 0;
}
