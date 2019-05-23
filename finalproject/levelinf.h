#ifndef LEVEL_H
#define LEVEL_H
#include<string.h>
#include<iostream>
using namespace std;
class levelinf
{
public:
    char* music;
    char* tempo;
    char* background;
    char* enemy;
    int goalkill;
    int bestkill;
    int bestcombo;
    int levelnum;
    levelinf(int i)
    :levelnum(i)
    {
        int nt;
        char *ts=new char[20];
        music=new char[20];;
        tempo=new char[20];;
        background=new char[20];;
        enemy=new char[20];;
        FILE* levelinfo=fopen("levelinf/level.txt","r");
        if(levelinfo==NULL) {cout<<"can't open levelinf";}
        do
        {
            do
            {
                fscanf(levelinfo,"%s",ts);
            }while(strcmp(ts,"level")!=0);
            fscanf(levelinfo,"%d",&nt);
        }while(nt!=levelnum);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,"music")!=0);
        fscanf(levelinfo,"%s",music);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,"tempo")!=0);
        fscanf(levelinfo,"%s",tempo);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,"background")!=0);
        fscanf(levelinfo,"%s",background);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,"enemy")!=0);
        fscanf(levelinfo,"%s",enemy);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,"goalkill")!=0);
        fscanf(levelinfo,"%d",&goalkill);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,"bestkill")!=0);
        fscanf(levelinfo,"%d",&bestkill);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,"bestcombo")!=0);
        fscanf(levelinfo,"%d",&bestcombo);
        //cout<<music<<"\n"<<tempo<<"\n"<<background<<"\n"<<enemy<<"\n"<<goalkill<<"\n"<<bestkill<<"\n"<<bestcombo;
        fclose(levelinfo);
    }
    ~levelinf()
    {
        /*delete music;
        delete tempo;
        delete background;
        delete enemy;*/
    }
    void write(std::string where,int number)
    {
        char *ts=new char[20];
        int nt,mouse;
        FILE* levelinfo=fopen("levelinf/level.txt","r+");
        if(levelinfo==NULL) {cout<<"can't write levelinf";}
        do
        {
            do
            {
                fscanf(levelinfo,"%s",ts);
            }while(strcmp(ts,"level")!=0);
            fscanf(levelinfo,"%d",&nt);
        }while(nt!=levelnum);
        do
        {
            fscanf(levelinfo,"%s",ts);
        }while(strcmp(ts,where.c_str())!=0);
        mouse=ftell(levelinfo);
        rewind(levelinfo);
        fseek(levelinfo,mouse+2,SEEK_SET);
        fprintf(levelinfo,"%04d",number);
        fclose(levelinfo);
    }
    void writeall(int nowkill, int nowcombo)
    {
        write("bestkill",nowkill);
        write("bestcombo",nowcombo);
    }
};
#endif
