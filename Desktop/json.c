#include<stdio.h>
#include<stdlib.h>
#include<jansson.h>
#include "httpd.h"
#include<string.h>

FILE *idf, *datef,*msgf,*authorf;
json_t *array;
char *str;

void get()
{
system("git --git-dir ~/QCube/.git log --pretty=format:'%h' >~/Desktop/id.txt");
system("git --git-dir ~/QCube/.git log|grep -w ^Author: >~/Desktop/author.txt");
system("sed -i 's/Author: //g' ~/Desktop/author.txt");
system("sed -i 's/</(/g' ~/Desktop/author.txt");
system("sed -i 's/>/)/g' ~/Desktop/author.txt");
system("git --git-dir ~/QCube/.git log|grep -w ^Date:   >~/Desktop/date.txt");
system("sed -i 's/Date:   //g' ~/Desktop/date.txt");
system("git --git-dir ~/QCube/.git log --pretty=format:%s >~/Desktop/msg.txt");
system("grep -w ^:: msg.txt>msg2.txt");
}
struct node
{
	char id[30];
	char author[100];
	char msg[250];
	char date[50];
	struct node *next;
}*start=NULL;

/*void get_msg(char *t,int c,FILE *temp)
{
fgets(t,c,temp);
while((strstr(t,"---")!=NULL))
{
fgets(t,c,temp */

void create()
{int n=1;
	while(n<=20)
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
	void display()
	{
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

		printf("NULL");
		str=json_dumps(array,JSON_INDENT(4));
		//json_dump_file(array,"/home/keerat/Desktop/jsn.jason",JSON_INDENT(4));
		//printf("%s",str);
		
	}
	/*void main()
	{
		idf=fopen("/home/keerat/Desktop/id.txt","r");
		datef=fopen("/home/keerat/Desktop/date.txt","r");
		msgf=fopen("/home/keerat/Desktop/msg.txt","r");
		authorf=fopen("/home/keerat/Desktop/author.txt","r");
		create();
		display();
	}*/
void get_data(server)
httpd *server;
{
if(strcmp(httpdRequestMethodName(server),"GET")==0)

httpdPrintf(server,"%s",str);
}

int main()
{
		
		get();
		idf=fopen("/home/keerat/Desktop/id.txt","r");
		datef=fopen("/home/keerat/Desktop/date.txt","r");
		msgf=fopen("/home/keerat/Desktop/msg.txt","r");
		authorf=fopen("/home/keerat/Desktop/author.txt","r");
		create();
		display();
	//printf("%s",(httpdUrlEncode(str)));
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
httpdAddCContent(server,"/","get_data",HTTP_FALSE,NULL,get_data);

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
httpdEndRequest(server);
}
}




