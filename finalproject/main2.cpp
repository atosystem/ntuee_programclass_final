#include <stdio.h>
#include <string>
#include<iostream>
#include<SDL.h>
#include <SDL_image.h>
#include"texture.h"
#include"characterinf.h"
#include"levelinf.h"
using namespace std;
const int screen_wide = 1080;
const int screen_height = 720;
//SDL_Window* gWindow = NULL;
//SDL_Renderer* gRenderer = NULL;
//--------------------------------------//
const int player_max_num=10;
player *playerset[player_max_num];
//--------------------------------------//
void startgame(int playermode,int levelnum,char *cha[5],bool mv,bool skillanimation)
{
    ///////
    cout<<"startgame"<<endl;
    for(int c1=0;c1<player_max_num;c1++)
    {
        playerset[c1]=new player(0);
    }//////
    cout<<"player"<<endl;
    int *exam=new int[2];///combo,kill
    cout<<"a1"<<endl;
    levelinf* nowlevel=new levelinf(levelnum);
    cout<<"a2"<<endl;
    char** ene=new char*[0];
    cout<<"a3"<<endl;
    ene[0]=nowlevel->enemy;
    cout<<"a4"<<endl;
    name* nametempt=new name(5,1);
    cout<<"a5"<<endl;
    nametempt->setvalue(true,cha);
    cout<<"a6"<<endl;
    nametempt->setvalue(false,ene);
    cout<<"a7"<<endl;
    playerset[playermode]->setplayerinf(*nametempt);
    //for(int c1=0;c1<5;c1++) cout<<playerset[playermode]->gamecha[c1]->name<<endl;
    cout<<"extern"<<endl;
    extern int* music(std::string music,std::string tempo,std::string musicbackground,player* nowplayer,bool mv,bool skillanimation,int goal);
    exam=music(nowlevel->music,nowlevel->tempo,nowlevel->background,playerset[playermode],mv,skillanimation,nowlevel->goalkill);
    if(exam[0]==-1)//fail
    {
        cout<<"you die";
    }
    else
    {
        if(exam[0]>nowlevel->bestcombo)
        {
            nowlevel->bestcombo=exam[0];
            nowlevel->write("bestcombo",exam[0]);
        }
        if(exam[1]>nowlevel->bestkill)
        {
            nowlevel->bestcombo=exam[1];
            nowlevel->write("bestkill",exam[1]);
        }
    }
    return;
}
/*int main( int argc, char* args[] )
{
    extern void gamestart();
    gamestart();
    //--------------------------------------//
    for(int c1=0;c1<player_max_num;c1++)
    {
        playerset[c1]=new player(0);
    }
    //--------------------------------------//
    char** c=new char* [5];
    c[0]="Riven";
    c[1]="Clausius";
    c[2]="Ellie";
    c[3]="Lime";
    c[4]="Muse";
    //-------------------------------------//
    startgame(0,2,c,false,true);
    return 0;
}*/


