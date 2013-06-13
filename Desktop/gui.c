#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void get_id()
{
system("git --git-dir ~/QCube/.git log|grep -w ^commit>~/Desktop/id.txt");
system("sed -i 's/commit //g' ~/Desktop/id.txt");
}
void get_author()
{
 system("git --git-dir ~/QCube/.git log|grep -w ^Author: >~/Desktop/author.txt");
system("sed -i 's/Author: //g' ~/Desktop/author.txt");
}
void get_date()
{
system("git --git-dir ~/QCube/.git log|grep -w ^Date:   >~/Desktop/date.txt");
system("sed -i 's/Date:   //g' ~/Desktop/date.txt");
}
void get_msg()
{
system("git --git-dir ~/QCube/.git log --pretty=format:-%s >~/Desktop/msg.txt");
}
void main()
{
get_id();
get_author();
get_date();
get_msg();
}



