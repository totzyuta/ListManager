#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE_LEN 1024
#define MAX_STR_LEN 69
#define MAX_PROFILES 10000

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

struct profile profile_data_store[MAX_PROFILES];
int profile_data_nitems = 0;

int subst(char *str, char c1, char c2){
  int n=0;
  while(*str!='\0'){
    if(*str == c1){
      *str=c2;
      n++;
    }
      *str++;
  }
  return n;
}

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

int get_line(FILE *fp,char *line)
{
  if(fgets(line,1025,fp) == NULL)
    return 0;
  
  subst(line,'\n','\0');

  return 1;
}

 /**
  * Create a new date into D from STR like "2004-05-02".
  * return: struct date *D itself
  */
struct date *check_date(struct date *d)
{

  if((d->m)>12) return NULL;
  

  if(((d->y)%4) ==0){
     if(((d->y)%400) == 0 || ((d->y)%100) != 0){
       if((d->m) == 2 && (d->d)>29) return NULL;
     }else{
       if((d->m) == 2 && (d->d)>28) return NULL;
     }
   }else{
       if((d->m) == 2 && (d->d)>28) return NULL;
   }
 
   if((d->m)== 4 ||
      (d->m)== 6 ||
      (d->m)== 9 ||
      (d->m)== 11 ){
     if((d->d)>30) return NULL;
   }
   if((d->m)==1 ||
      (d->m)==3 ||
      (d->m)==5 ||
      (d->m)==7 ||
      (d->m)==8 ||
      (d->m)==10||
      (d->m)==12){
     if((d->d)>31) return NULL;
     }
}

struct date *new_date(struct date *d, char *str)
{
   char *ptr[3];
 
   if (split(str, ptr, '-', 3) != 3){
     return NULL;
   }

   d->y = atoi(ptr[0]);
   d->m = atoi(ptr[1]);
   d->d = atoi(ptr[2]);
 
   return d;
}
 

struct profile *check_id(struct profile *p1)
{
  int i;
  struct profile *p2;

  if(profile_data_nitems == 0){
    return p1;
  }

  for(i=0;i<profile_data_nitems;i++){
    p2=&profile_data_store[i];
  if(p1->id == p2->id){
    return NULL;
      }
  }


}

/**
 * Create a new profile into P from CSV string like
 *  "0,Takahashi Kazuyuki,1977-04-27,Saitama,Fukuoka Softbank Hawks".
 * return: struct profile *P itself
 */
struct profile *new_profile(struct profile *p, char *csv,int edt)
 {
   char *ptr[5];
 
   if (split(csv, ptr, ',', 5) != 5){
     fprintf(stderr,"error: invalid style of input or data\n\n");
     return NULL;
   }

   p->id = atoi(ptr[0]); 
   if(check_id(p) == NULL){
     if(edt!=1){
       fprintf(stderr,"error: ID already exists.\n To edit the data, use E command\n");
       return NULL;
     }
   }

   strncpy(p->name, ptr[1], MAX_STR_LEN);  
   p->name[MAX_STR_LEN] = '\0';

   if (new_date(&p->birth, ptr[2]) == NULL ){
     fprintf(stderr,"invalid style of input");
     return NULL; 
   }
   if(check_date(&p->birth) == NULL){
     fprintf(stderr,"error: %04d-%02d-%02d is invalid date\n\n",(p->birth).y, (p->birth).m, (p->birth).d);
     return NULL;
   }
  
   strncpy(p->home, ptr[3], MAX_STR_LEN); 
   p->home[MAX_STR_LEN] = '\0';
 
   p->comment = (char *)malloc(sizeof(char) * (strlen(ptr[4])+1));
   strcpy(p->comment, ptr[4]);
 
   profile_data_nitems++;
   
   return p;
}

void cmd_quit()
{
  printf("Program has ended\n");
  exit(0); 
}

void cmd_check()
{
  printf("Number of elements: %d\n\n",profile_data_nitems);
}

void print_profile(int i,struct profile *p)
{
  char date[11];
  printf("(line%d)\n",i+1);
  printf("id:%d\n",p->id);
  printf("name:%s\n",p->name);
  printf("Birth:%04d-%02d-%02d\n",(p->birth).y,(p->birth).m,(p->birth).d);
  printf("addr:%s\n",p->home);
  printf("comment:%s\n",p->comment);
}

// %P: print
int min(int a, int b)
{
  if(a < b) return a;
  return b;
}

int max(int a, int b)
{
  if(a > b) return a;
  return b;
}

void cmd_print(int nitems)
{
  int i, start = 0, end = profile_data_nitems;

  if(nitems>0){
    end = min(nitems,profile_data_nitems);
  }
  if(nitems<0){
    start = max(end - (-nitems),0);
  }

  for(i = start; i < end; i++){
    print_profile(i,&profile_data_store[i]);
    printf("\n\n");
  }
}

int cmd_read(char *filename)
{
  FILE *fp;
  char line[MAX_LINE_LEN+1];

  fp = fopen(filename,"r");

  if(fp == NULL){
    fprintf(stderr,"error: could not read the file\n\n");
    return -1;
  }

  while(get_line(fp,line)){
    parse_line(line);
  }
  fclose(fp);
  return 0;
}

// %W: write
void fprint_profile_csv(FILE *fp,struct profile *p)
{
  fprintf(fp,"%d,%s,%04d-%02d-%02d,%s,%s\n",p->id, p->name, (p->birth).y, p->birth.m, (p->birth).d, (p->home), p->comment);
}

int cmd_write(char *filename)
{
  FILE *fp;
  char line[MAX_LINE_LEN+1];
  int i;
  
  fp = fopen(filename,"w");
  
  if(fp == NULL){
    fprintf(stderr,"error: could not open the file\n\n");
    return -1;
  }

  for (i = 0; i < profile_data_nitems; i++){
    fprint_profile_csv(fp,&profile_data_store[i]);
  }
  fclose(fp);
  return 0;
}

// %F: find
char *date_to_string(char buf[],struct date *date)
{
  sprintf(buf,"%04d-%02d-%02d",date->y,date->m,date->d);
  return buf;
}

void cmd_find(char *word)
{
  int i;
  struct profile *p;
  char s[8];  
  char birthday_str[11];
  int n=0;
  
  for(i = 0; i < profile_data_nitems; i++){
    p = &profile_data_store[i];
    sprintf(s,"%d",p->id);
    date_to_string(birthday_str,&p->birth);
    if(strcmp(s,word) == 0 ||
       strcmp(p->name,word) == 0 ||
       strcmp(birthday_str,word) == 0 ||
       strcmp(p->home,word) == 0 ||
       strcmp(p->comment,word) == 0){
        print_profile(i,p);
        printf("\n");
    }else{
      n++;
    }
  }
  if(n = profile_data_nitems){
    fprintf(stderr,"error: could not find\n\n");
  }
}


// %S sort 
void swap_profile(struct profile *p1,struct profile *p2)
{  
  struct profile temp;

  temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

int compare_date(struct date *d1,struct date *d2)
{
  if(d1->y != d2->y) return d1->y - d2->y;
  if(d1->m != d2->m) return d1->m - d2->m;
  return d1->d - d2->d;
}

int profile_compare(struct profile *p1,struct profile *p2,int param)
{
   
  switch(param){
  case 1:
    printf("Sorted by id");
    return p1->id - p2->id;
  case 2:
    return strcmp(p1->name,p2->name);
  case 3:
    return compare_date(&p1->birth,&p2->birth);
  case 4:
    return strcmp(p1->home,p2->home);
  case 5:
    return strcmp(p1->comment,p2->comment);
  }
}

void cmd_sort(int param)
{
  int i,j,cmp;
  int left=0,right=profile_data_nitems - 1;

  if(param>5 || 0>=param) fprintf(stderr,"error: input a number of 1 to 5\n");

  for(i=left; i<=right; i++){
    for(j=left; j<=right-1; j++){
      cmp = profile_compare(&profile_data_store[j],&profile_data_store[j+1],param);
      if(cmp > 0){
        swap_profile(&profile_data_store[j],&profile_data_store[j+1]);
      }
    }
  }
}

//%E edit 
void cmd_edit()
{
  int i,id,edt=0;
  char m[MAX_LINE_LEN],after[MAX_LINE_LEN];
  char *ptr[5],*aft;
  struct profile *p;

  printf("What is ID of the data you want to edit?\n id:");
  fgets(m,MAX_LINE_LEN + 1,stdin);
  id = atoi(m);

  for(i = 0; i < profile_data_nitems; i++){
    p = &profile_data_store[i];
   
    if(id == p->id){
      printf("Before\n%d,%s,%04d-%02d-%02d,%s,%s\n",p->id, p->name, (p->birth).y, p->birth.m, (p->birth).d, (p->home), p->comment);
      printf("After\n");
      fgets(after,MAX_LINE_LEN+1,stdin);

      if(new_profile(&profile_data_store[i],after,edt)!=NULL){
	      profile_data_nitems--;
      }
    }
  }
}

// %H help
void cmd_help()
{
  printf("\n# HELP\n");
  printf("## Commands\n");
  printf("- Q: Quit\n");
  printf("- C: check the number of registered datas\n");
  printf("- P n: Print n elements (n=0:all, n<0:from behind)\n");
  printf("- R file: Read from file\n");
  printf("- W file: Write as file\n");
  printf("- F word: Search by word, print like P\n");
  printf("- S n: Sort datas as the column of n\n");
  printf("- H: Show usage of commands\n\n");
}

void exec_command(char cmd, char *param)
{
  switch(cmd){
  case'Q':cmd_quit();  break;
  case'C':cmd_check(); break;
  case'P':cmd_print(atoi(param)); break;
  case'R':cmd_read(param);  break;
  case'W':cmd_write(param); break;
  case'F':cmd_find(param);  break;
  case'S':cmd_sort(atoi(param));  break;
  case'E':cmd_edit(); break;
  case'H':cmd_help(); break;
  default:fprintf(stderr,"error\n\n");
  }
  printf(":");
}

int parse_line(char *line)
{
  int cmd,std=1;
  char *param;
  
  if(*line == '%'){
    cmd = line[1];
    param = &line[3];
    exec_command(cmd,param);
  }else if(*line == '\0'){
    fprintf(stderr,"error: no input\n\n:");
    return 0;
  }else if(profile_data_nitems == MAX_PROFILES){
    fprintf(stderr,"error: over the limit of datas\n\n:");
  }else {
    new_profile(&profile_data_store[profile_data_nitems],line,std);
  }
  return 0;
}


int main()
{
  printf("Program has sterted.Type a command after a symbol of parcent. To show help, type H command.\n:");
  char line[MAX_LINE_LEN+1];
  while(get_line(stdin,line)){
    parse_line(line);
  }
  return 0;
}

