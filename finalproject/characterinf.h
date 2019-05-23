#ifndef CHARACTERINF_H
#define CHARACTERINF_H
#include<string.h>
#include<iostream>
using namespace std;
class name
{
public:
    int playing_cha;
    int playing_enemy;
    int name_max=20;
    char** chaname;
    char** enename;
    name(int num1,int num2)
        :playing_cha(num1),playing_enemy(num2)
    {
        chaname=new char*[playing_cha];
        for(int c1=0;c1<playing_cha;c1++)
        {
            chaname[c1]=new char[name_max];
        }
        enename=new char*[playing_enemy];
        for(int c1=0;c1<playing_cha;c1++)
        {
            enename[c1]=new char[name_max];
        }
    }
    ~name()
    {
        for(int c1=0;c1<playing_cha;c1++)
        {
            delete[] chaname[c1];
        }
        delete[] chaname;
        for(int c1=0;c1<playing_enemy;c1++)
        {
            delete[] enename[c1];
        }
        delete[] enename;
    }
    setvalue(bool who,char** c)
    {
        if(who==true)//cha
        {
            for(int c1=0;c1<playing_cha;c1++)
            {
                chaname[c1]=c[c1];
                //strcpy(chaname[c1],c[c1]);
            }
        }
        else//ene
        {
            for(int c1=0;c1<playing_enemy;c1++)
            {
                enename[c1]=c[c1];
                //strcpy(enename[c1],c[c1]);
            }
        }
    }
};

class character
{
public:
    int bloodmax;
    int phyatk;
    int magatk;
    int blood;
    int special[4]; //技能時間/ 攻擊力加成(int) /判定加成(scoretype)/ 回血(int)/
    int specialneed[3];// combo門檻 血量太低 展殺對手
    char name[20];
    character(std::string charactertype)
    {
        readinf(charactertype.c_str());
    }
    ~character()
    {
        /*bloodmax=0;
        blood=0;
        phyatk=0;
        magatk=0;*/
    }
    void readinf(std::string charactertype)
    {
        FILE* information=fopen("character/characterinf.txt","r");
        if(information==NULL) {cout<<"can't open characterinf"; return;}
        const char* goal=charactertype.c_str();
        char ts[20];
        int td;
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,goal)!=0);
        strcpy(name,ts);
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"blood_max")!=0);
        fscanf(information,"%d",&bloodmax);
        blood=bloodmax;
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"physic_attack")!=0);
        fscanf(information,"%d",&phyatk);
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"magic_attack")!=0);
        fscanf(information,"%d",&magatk);
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"specialskill")!=0);
        fscanf(information,"%d%d%d%d",&special[0],&special[1],&special[2],&special[3]);
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"need")!=0);
        fscanf(information,"%d%d%d",&specialneed[0],&specialneed[1],&specialneed[2]);
    }

};
class enemy
{
public:
    int enemybloodmax;
    int enemyblood;
    int enemyphyatk;
    int enemymagatk;
    enemy(std::string enemytype)
    {
        readinf(enemytype.c_str());
    }
    ~enemy()
    {
        enemybloodmax=0;
        enemyblood=0;
        enemyphyatk=0;
        enemymagatk=0;
    }
    void readinf(std::string enemytype)
    {
        FILE* information=fopen("character/enemyinf.txt","r");
        if(information==NULL) {cout<<"can't open enemyinf"; return;}
        const char* goal=enemytype.c_str();
        char ts[20];
        int td;
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,goal)!=0);
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"enemy_blood_max")!=0);
        fscanf(information,"%d",&enemybloodmax);
        enemyblood=enemybloodmax;
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"enemy_physic_attack")!=0);
        fscanf(information,"%d",&enemyphyatk);
        do
        {
            fscanf(information,"%s",ts);
        }while(strcmp(ts,"enemy_magic_attack")!=0);
        fscanf(information,"%d",&enemymagatk);
    }
};
class player
{
private:
    int gamechanum;
    int gameenenum;
public:
    int totalblood=0;
    int totalphyatk=0;
    int totlamagatk=0;
    int totaleneblood=0;
    int totalenephyatk=0;
    int totalenemagatk=0;
    character** gamecha;
    enemy** gameene;
    player(int i)
    {
        ;
    }
    player(const name& namet)
        :gamechanum(namet.playing_cha),gameenenum(namet.playing_enemy)
    {
        cout<<"b1";
        gamecha=new character* [gamechanum];
        cout<<"b2";
        for(int c1=0;c1<gamechanum;c1++)
        {
            gamecha[c1]=new character(namet.chaname[c1]);
            totalblood+=gamecha[c1]->bloodmax;
            totalphyatk+=gamecha[c1]->phyatk;
            totlamagatk+=gamecha[c1]->magatk;
        }
        cout<<"b3";
        gameene=new enemy* [gameenenum];
        cout<<"b4";
        for(int c1=0;c1<gameenenum;c1++)
        {
            gameene[c1]=new enemy(namet.enename[c1]);
            totaleneblood+=gameene[c1]->enemybloodmax;
            totalenephyatk+=gameene[c1]->enemyphyatk;
            totalenemagatk+=gameene[c1]->enemymagatk;
        }
        cout<<"b5";
    }
    /*player(char **c)
    {
        for(int c1=0;c1<gamechanum;c1++)
            gamecha[c1]=new character(c[c1]);
    }*/
    ~player()
    {
        for(int c1=0;c1<gamechanum;c1++)
            delete gamecha[c1];
        delete[] gamecha;
        for(int c1=0;c1<gameenenum;c1++)
            delete gameene[c1];
        delete[] gameene;
    }
    void setplayerinf(const name &namet)
    {
        //same as player(name &namet)
        gamechanum=namet.playing_cha;
        gameenenum=namet.playing_enemy;
        gamecha=new character* [gamechanum];
        for(int c1=0;c1<gamechanum;c1++)
        {
            gamecha[c1]=new character(namet.chaname[c1]);
            totalblood+=gamecha[c1]->bloodmax;
            totalphyatk+=gamecha[c1]->phyatk;
            totlamagatk+=gamecha[c1]->magatk;
        }
        gameene=new enemy* [gameenenum];
        for(int c1=0;c1<gameenenum;c1++)
        {
            gameene[c1]=new enemy(namet.enename[c1]);
            totaleneblood+=gameene[c1]->enemybloodmax;
            totalenephyatk+=gameene[c1]->enemyphyatk;
            totalenemagatk+=gameene[c1]->enemymagatk;
        }
    }

};
#endif
