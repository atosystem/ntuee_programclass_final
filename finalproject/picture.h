#include<time.h>
#include<SDL.h>
#include<iostream>
#include<stdio.h>
#include<thread>
#include<mutex>
using namespace std;
main1texture* picture=NULL;
main1texture* characterpicture=NULL;
SDL_Rect rectdes1;
SDL_Rect rectdes2;
static mutex tmutextpicture;
void gopicture1(std::string bg,std::string cha)
{
    tmutextpicture.lock();
    picture=new main1texture;
    characterpicture=new main1texture;
    int x=screen_wide;
    int nb1=10;
    int nb2=10;
    int step1=7;
    int step2=80;
    int step3=7;
    float n1=0.5;
    float n2=0.25;
    float n3=-0.5;
    int height=screen_height*0.6;
    picture->loadfromfile(bg.c_str());//"picture/fire.png"
    picture->setalpha(200);

    for(int c1=1;c1<=nb1;c1++)
    {
        rectdes1={
        screen_wide-screen_wide/nb1*c1,
        screen_height/2-5,
        screen_wide/nb1*c1,
        10//height/n*c1
        };
        SDL_Delay(20);
    }
    for(int c1=1;c1<=nb2;c1++)
    {
        rectdes1={
        0,
        screen_height/2-5-(height-10)/2/nb2*c1,
        screen_wide,
        10+(height-10)/nb2*c1
        };
        SDL_Delay(20);
    }
    if(characterpicture->loadfromfile(cha.c_str())<0) cout<<"fail";//"picture/act.png"
    characterpicture->setalpha(250);
    while(x>int(float(screen_wide)*n1))
    {
        //SDL_RenderClear(gRenderer);
        x-=screen_wide/2/step1;
        rectdes2={
        x,
        screen_height/2-(height/2),
        characterpicture->getWidth()*height/characterpicture->getHeight(),
        height
        };
        SDL_Delay(20);
    }
    while(x>int(float(screen_wide)*n2))
    {
        //SDL_RenderClear(gRenderer);
        x-=screen_wide/2/step2;
        rectdes2={
        x,
        screen_height/2-(height/2),
        characterpicture->getWidth()*height/characterpicture->getHeight(),
        height
        };
        SDL_Delay(20);
    }
    while(x>int(float(screen_wide)*n3))
    {
        //SDL_RenderClear(gRenderer);
        x-=screen_wide/2/step3;
        rectdes2={
        x,
        screen_height/2-(height/2),
        characterpicture->getWidth()*height/characterpicture->getHeight(),
        height
        };
        SDL_Delay(20);
    }
    //SDL_Delay(3000);
    delete picture;
    picture=NULL;
    delete characterpicture;
    characterpicture=NULL;
    tmutextpicture.unlock();
}
//skill[5]= 技能時間/ 攻擊力加成(int) /判定加成(scoretype)/ 回血(int)/c1(mean who )(gamecha[c1])
// combo門檻 血量太低 展殺對手
bool specialskill(player* nowplayer,int combo,int blood,int enekill,int* skill)
 {
     static bool doskill[5]={false,false,false,false,false};
     static int tcombo=0;//堆疊combo
     if(combo<tcombo) tcombo=combo;
     for(int c1=0;c1<5;c1++)
     {
         if(doskill[c1]==false)
         {
            if(((combo-tcombo)>=nowplayer->gamecha[c1]->specialneed[0])
               &&(blood<=nowplayer->gamecha[c1]->specialneed[1])
               &&(enekill>=nowplayer->gamecha[c1]->specialneed[2]))
            {
                tcombo=combo;
                doskill[c1]=true;
                for(int c2=0;c2<4;c2++)
                {
                    skill[c2]=nowplayer->gamecha[c1]->special[c2];
                }
                skill[4]=c1;
                //cout<<"true"<<endl;

                return true;
            }
         }
     }
     // cout<<"false"<<endl;
     return false;
 }
