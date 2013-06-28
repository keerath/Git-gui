#include<stdio.h>
#include<stdlib.h>
#include<jansson.h>
#include "httpd.h"
#include<string.h>
#include<time.h>
httpVar *var1,*var2;
FILE *idf, *datef,*msgf,*authorf;
char *str;
int nlines()
{int c;
int n=0;
while((c=fgetc(idf))!=EOF)
{
if(c=='\n')
n++;
}
fclose(idf);
idf=fopen("/home/keerat/Desktop/id.txt","r");
return n;
}
void get()
{
char clone[200]="git clone ";
char fid[200]="git --git-dir ~/Desktop/";
char fauth[200]="git --git-dir ~/Desktop/";
char fdate[200]="git --git-dir ~/Desktop/";
char fmsg[200]="git --git-dir ~/Desktop/";
char lid[100]="/.git log --pretty=format:'%h' >~/Desktop/id.txt";
char lauth[100]="/.git log|grep -w ^Author: >~/Desktop/author.txt";
char ldate[100]="/.git log|grep -w ^Date:   >~/Desktop/date.txt";
char lmsg[100]="/.git log --pretty=format:%s >~/Desktop/msg.txt";
	strcat(clone,var2->value);
	system(clone);
	strcat(fid,var1->value);
	strcat(fid,lid);
	printf("%s",fid);
	system(fid);
	strcat(fauth,var1->value);
	strcat(fauth,lauth);
	system(fauth);
	system("sed -i 's/Author: //g' ~/Desktop/author.txt");
	system("sed -i 's/</(/g' ~/Desktop/author.txt");
	system("sed -i 's/>/)/g' ~/Desktop/author.txt");
	strcat(fdate,var1->value);
	strcat(fdate,ldate);
	system(fdate);
	system("sed -i 's/Date:   //g' ~/Desktop/date.txt");
	strcat(fmsg,var1->value);
	strcat(fmsg,lmsg);
	system(fmsg);

}

struct node
{
	char id[30];
	char author[100];
	char msg[250];
	char date[50];
	struct node *next;
}*start=NULL;

void create()
{	
	int n=1;
	while(n<=(nlines()))
	{	
		struct node *new_node,*current;
		new_node=(struct node *)malloc(sizeof(struct node));
		fgets(new_node->id,30,idf);
		fgets(new_node->author,100,authorf);
		fgets(new_node->msg,250,msgf);
		fgets(new_node->date,50,datef);
		new_node->next=NULL;
		if(start==NULL)
		{
			start=new_node;
			current=new_node;
		}
		else
		{
			current->next=new_node;
			current=new_node;
		}
		n++;
	}
}
void clear()
{
start=NULL;
}
void convert_json()
{	
	str=NULL;
	json_t *array = json_array();
	struct node *new_node;
	new_node=start;

	while(new_node!=NULL)
	{ 
		json_t *obj=json_object();
		
		char id[10]="id";
		char author[10]="author";
		char msg[10]="msg";
		char date[10]="date";

		json_object_set(obj,id,json_string(new_node->id));
		json_object_set(obj,author,json_string(new_node->author));
		json_object_set(obj,date,json_string(new_node->date));
		json_object_set(obj,msg,json_string(new_node->msg));
		json_array_append(array,obj);



		new_node=new_node->next;
	}
	json_dump_file(array,"/home/keerat/Desktop/output",JSON_INDENT(4));
	str=json_dumps(array,JSON_INDENT(4));
	printf("%s",str);
}
void get_data(server)
	httpd *server;
{
	if(strcmp(httpdRequestMethodName(server),"GET")==0)

		httpdPrintf(server,"%s",str);
}

int main()
{				
	httpd *server;
	server=httpdCreate(NULL,9080);
	if(server==NULL)
		perror("Cant create");
	httpdSetAccessLog(server,stdout);
	httpdSetErrorLog(server,stderr);
	httpdSetFileBase(server,"/home/keerat/Desktop");
	httpdAddWildcardContent(server,"/css",NULL,"css");
	httpdAddWildcardContent(server,"/js",NULL,"js");
	httpdAddWildcardContent(server,"/pic",NULL,".");
	httpdAddFileContent(server,"/","index.html",HTTP_TRUE,NULL,"index.html");
	httpdAddFileContent(server,"/","data.html",HTTP_FALSE,NULL,"data.html");	
	while(1==1)
	{
		if(httpdGetConnection(server,NULL)<0)
			continue;
		if(httpdReadRequest(server)<0)
		{
			httpdEndRequest(server);
			continue;
		}
		httpdProcessRequest(server);
		var1=httpdGetVariableByName(server,"name");
		var2=httpdGetVariableByName(server,"url");
		
		if(var1!=NULL&&var2!=NULL)
		{		
			printf("%s%s",var1->value,var2->value);
			get();
			sleep(7);
			idf=fopen("/home/keerat/Desktop/id.txt","r");
			datef=fopen("/home/keerat/Desktop/date.txt","r");
			msgf=fopen("/home/keerat/Desktop/msg.txt","r");
			authorf=fopen("/home/keerat/Desktop/author.txt","r");
			clear();
			create();
			convert_json();
			httpdAddCContent(server,"/","get_data",HTTP_FALSE,NULL,get_data);
		}
		
		httpdEndRequest(server);
	}	
}
