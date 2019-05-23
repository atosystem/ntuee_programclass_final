#include<time.h>
#include<SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include<iostream>
#include<stdio.h>
#include<thread>
#include<mutex>
#include<cmath>
#include"texture.h"
#include"steingate2.h"
#include"characterinf.h"
#include"picture.h"
using namespace std;
extern main1texture* picture;
extern SDL_Renderer* gRenderer;
struct longbuttonline
{
    int beginbutnum;
    int endbutnum;
    bool press;
    int line;
    bool hurt;
    longbuttonline(int bn,int c1)
    {
        beginbutnum=bn;
        line=c1;
        press=false;
        hurt=false;
    }
    ~longbuttonline()
    {
        beginbutnum=0;
        endbutnum=0;
        press=false;
        hurt=false;
    }
};
struct button
{
    int line;
    int buttype;
    int longbutnum;
    clock_t gotime;
    bool press;
    SDL_Rect rect;
    bool animate;
    float positionrecord;
    button(int blive,int c1,clock_t t)
    {
        line=c1;
        buttype=blive;
        gotime=t;
        press=false;
        positionrecord=0.0;
        animate=false;
    }
    ~button()
    {
        line=0;
        buttype=0;
        gotime=0;
        press=false;
        positionrecord=0.0;
        animate=false;
    }

};
static void butanimation1(int type);
static void butanimation2(int type);
static void butanimation3(int type);
static int totalbutton1;//
static int totallongbutton;
static main1texture tbgmusic1;
static main1texture tbgmusic2;
static main1texture** cbut;
static main1texture** clongbutline;
static button** sbut;
static longbuttonline** slongbutline;
static main1texture* scoretexture=NULL;
static texttexture* combotext=NULL;
static Mix_Music *gamemusic1 = NULL;
static Mix_Chunk *ching = NULL;
static const clock_t movetime=1500;
static TTF_Font *gFont = NULL;
static TTF_Font *gFontcombo = NULL;
bool loadsoundmusic(std::string path)
{
    Mix_FreeMusic( gamemusic1 );
	gamemusic1 = Mix_LoadMUS(path.c_str());
	if( gamemusic1 == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	}
	else
        return true;
}
void closegamemusic1()
{
	//Free loaded images
	tbgmusic1.free();
	Mix_FreeMusic( gamemusic1 );
	Mix_FreeChunk( ching);
	gamemusic1 = NULL;
}

void butini(std::string tempo)
{
    cout<<"butini";
    FILE* music1=fopen(tempo.c_str(),"r");
    fscanf(music1,"%d%d",&totalbutton1,&totallongbutton);
    cbut=new main1texture* [totalbutton1];
    clongbutline=new main1texture* [totallongbutton];
    sbut=new button* [totalbutton1];
    slongbutline=new longbuttonline* [totallongbutton];
    int bn=0;
    int lnbn=0;
    int blive=0;
    main1texture load;
    if(!load.loadfromfile("picture/green.png")) cout<<"green fail";
    SDL_Rect loadrect;
    clock_t t;
    if(music1==NULL) {std::cout<<"can't open music1 tempo"; return;}
    while(fscanf(music1,"%d",&t)!=EOF)
    {
        cout<<"into";
        for(int c1=1;c1<=7;c1++)
        {
            fscanf(music1,"%d",&blive);
            if((blive==1)||(blive==2)||(blive==4))
            {
                sbut[bn]=new button(blive,c1,t);
                cbut[bn]=new main1texture;
                cbut[bn]->setalpha(255);

                if(blive==1)
                {
                    sbut[bn]->longbutnum=-1;
                    if(!cbut[bn]->loadfromfile( "picture/button.png",255,255,255 )) {printf( "Failed to load enter1 media!\n" );return;}
                }
                else if(blive==2)
                {
                    sbut[bn]->longbutnum=lnbn;
                    slongbutline[lnbn]=new longbuttonline(bn,c1);
                    clongbutline[lnbn]=new main1texture;
                    switch(c1)
                    {
                        case 1: if(!clongbutline[lnbn]->loadfromfile( "picture/r1.png" ,255,255,255)) {printf( "Failed to load r1 media!\n" );return;} break;
                        case 2: if(!clongbutline[lnbn]->loadfromfile( "picture/r2.png" )) {printf( "Failed to load r2 media!\n" );return;} break;
                        case 3: if(!clongbutline[lnbn]->loadfromfile( "picture/r3.png" )) {printf( "Failed to load r3 media!\n" );return;} break;
                        case 4: if(!clongbutline[lnbn]->loadfromfile( "picture/r4.png" )) {printf( "Failed to load r4 media!\n" );return;} break;
                        case 5: if(!clongbutline[lnbn]->loadfromfile( "picture/r5.png" )) {printf( "Failed to load r5 media!\n" );return;} break;
                        case 6: if(!clongbutline[lnbn]->loadfromfile( "picture/r6.png" )) {printf( "Failed to load r6 media!\n" );return;} break;
                        case 7: if(!clongbutline[lnbn]->loadfromfile( "picture/r7.png" )) {printf( "Failed to load r7 media!\n" );return;} break;
                    }
                    if(!cbut[bn]->loadfromfile( "picture/buttonlong.png",255,255,255 )) {printf( "Failed to load enter1 media!\n" );}
                    lnbn++;
                }
                else if(blive==4)
                {
                    for(int c2=1;c2<=7;c2++)///////lnbn has already ++
                    {
                        if(slongbutline[lnbn-c2]->line==c1)
                        {
                            sbut[bn]->longbutnum=lnbn-c2;
                            slongbutline[lnbn-c2]->endbutnum=bn;
                            break;
                        }
                    }
                    if(!cbut[bn]->loadfromfile( "picture/buttonlong.png",255,255,255 )) {printf( "Failed to load enter1 media!\n" );}
                }
                loadrect={0,0,screen_wide*bn/totalbutton1,screen_height};
                load.render(NULL,&loadrect);
                SDL_RenderPresent(gRenderer);
                cout<<(float)bn/totalbutton1<<endl;
                bn++;
            }
        }
    }
}
static const int hedge=100;
static const clock_t laterange=400;
static const clock_t experttime=50;
static const clock_t greattime=100;
static const clock_t goodtime=200;
static int nowscanbut=-1;
static main1texture* cba1=NULL;
static main1texture* cba2=NULL;
static main1texture* cba3=NULL;

enum scoretype{miss=-2,bad,good,great,perfect};
scoretype examscore(clock_t examt)
{
    if(examt<0) examt=-examt;
    if(examt<=experttime) return perfect;
    else if(examt<=greattime) return great;
    else if(examt<=goodtime) return good;
}
float atkrate(scoretype score,player* nowplayer)
{
    if(score==perfect) return 1.0*(float)(1.0+nowplayer->totlamagatk/1000);
    else if(score==great) return 0.9*(float)(1.0+nowplayer->totlamagatk/1000);
    else if(score==good) return 0.7*(float)(1.0+nowplayer->totlamagatk/1000);
}
mutex tmutext0;
void scoredisplay(scoretype score,int combo)
{
    tmutext0.lock();
    SDL_Color textColor;
    if(combo>300) textColor = { 253,249,104 };
    else if(combo>200) textColor = { 250,106,125 };
    else if(combo>100) textColor = { 117,250,125 };
    else if(combo>50) textColor = { 251,196,38 };
    else textColor = { 25,193,255 };
    scoretexture=new main1texture;
    combotext=new texttexture;
    switch(score)
    {
        case good:
            if(!scoretexture->loadfromfile( "picture/good.png",255,255,255 ))printf( "Failed to load score\n" );
            break;
        case great:
            if(!scoretexture->loadfromfile( "picture/great.png",255,255,255 ))printf( "Failed to load score\n" );
            break;
        case perfect:
            if(!scoretexture->loadfromfile( "picture/perfect.png",255,255,255 ))printf( "Failed to load score\n" );
            break;
    }
    char* c=new char[20];
    int n=sprintf(c,"%d",combo);
    combotext->loadFromRenderedText(c,textColor,gFontcombo);
    SDL_Delay(100);

    scoretexture->free();
    scoretexture=NULL;
    combotext->free();
    combotext=NULL;
    tmutext0.unlock();
}
int* gobutton(player* nowplayer,bool mv, bool skillanimation,int goal)
{
    extern void gopicture1(std::string bg,std::string cha);
    Mix_PlayMusic(gamemusic1,0);
    //------------display variable-------//
    clock_t startt=clock();
    clock_t tempt;////
    clock_t skillendtime;
    scoretype score;
    SDL_Event e;
    float position;
    float positionedit;
    float playwide;
    float playheight;
    bool skilling=false;
    bool press[7]={false,false,false,false,false,false,false};
    int energylevel=3;
    int eneenergylevel=3;
    int speeddimension=2;
    int changecolor=0;
    int combo=0;
    int combomax=0;
    int* skill=new int[5];
    for(int c1=0;c1<5;c1++) skill[c1]=0;
    //-------------player parameter------------//
    int blood=nowplayer->totalblood;
    int phyatk=nowplayer->totalphyatk;
    int magatk=nowplayer->totlamagatk;
    int eblood=nowplayer->totaleneblood;
    int ephyatk=nowplayer->totalenephyatk;
    int emagatk=nowplayer->totalenemagatk;
    int enekill=0;
    int goalkill=goal;
    //----------return value------------------//
    static int* r=new int[2];
    //------------displaying initialization-----------//
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    main1texture* energybarout=new main1texture;
    main1texture* energybarin=new main1texture;
    main1texture* eneenergybarout=new main1texture;
    main1texture* eneenergybarin=new main1texture;
    if(!energybarout->loadfromfile( "picture/energybar1.png",255,255,255 ))printf( "Failed to load energybar1\n" );
    if(!energybarin->loadfromfile( "picture/energybar2.png",255,255,255 ))printf( "Failed to load energybar2\n" );
    if(!eneenergybarout->loadfromfile( "picture/energybar1.png",255,255,255 ))printf( "Failed to load energybar1\n" );
    if(!eneenergybarin->loadfromfile( "picture/energybar2.png",255,255,255 ))printf( "Failed to load energybar2\n" );
    SDL_Rect eborect={screen_wide-energybarout->getWidth()/3-10,10,energybarout->getWidth()/3,energybarout->getHeight()/4};
    SDL_Rect ebirect={screen_wide-energybarin->getWidth()/3-10,10,energybarin->getWidth()/3,energybarout->getHeight()/4};
    SDL_Rect eneeborect={10,10,eneenergybarout->getWidth()/3,eneenergybarout->getHeight()/4};
    SDL_Rect eneebirect={10,10,eneenergybarin->getWidth()/3,eneenergybarin->getHeight()/4};
    SDL_Rect scorerect={screen_wide/2-screen_wide/16,400,screen_wide/8,screen_height/8};
    SDL_Rect comborect;
    SDL_Color textColor = { 0,0,0 };
    texttexture *chabloodtext=new texttexture;
    texttexture *enebloodtext=new texttexture;
    texttexture *killnum=new texttexture;
    char *chabloodchar=new char[20];
    char *enebloodchar=new char[20];
    char *killnumchar=new char[20];
    int gar1=sprintf(chabloodchar,"%d/%d",blood,nowplayer->totalblood);
    int gar2=sprintf(enebloodchar,"%d/%d",eblood,nowplayer->totaleneblood);
    int gar3=sprintf(killnumchar,"%d/%d",enekill,goalkill);
    chabloodtext->loadFromRenderedText(chabloodchar,textColor,gFont);
    enebloodtext->loadFromRenderedText(enebloodchar,textColor,gFont);
    killnum->loadFromRenderedText(killnumchar,textColor,gFont);
    SDL_Rect chabloodrect={screen_wide-chabloodtext->getWidth()-5,40,chabloodtext->getWidth(),chabloodtext->getHeight()};
    SDL_Rect enebloodrect={5,40,enebloodtext->getWidth(),enebloodtext->getHeight()};
    SDL_Rect killnumrect={5,70,killnum->getWidth(),killnum->getHeight()};

    //----------game start-------------------//
    while(Mix_PlayingMusic()==1)
    {
        SDL_RenderClear( gRenderer );//----------------------->render clear
        if(mv==true) SDL_RenderCopy( gRenderer, sdlTexture,  NULL, NULL);//----------------------->render
        else tbgmusic2.render();//----------------------->render
        tbgmusic1.render();//----------------------->render
        //--------------skill display----------//
        if(skillanimation==true)
        {
           if(picture!=NULL)
            {
                picture->render(NULL,&rectdes1);//----------------------->render
                if(characterpicture!=NULL);
                {
                    characterpicture->render(NULL,&rectdes2);//----------------------->render
                }
            }
        }
        //-------------------------------------------//
        for(nowscanbut=0;nowscanbut<totalbutton1;nowscanbut++)
        {
            if(sbut[nowscanbut]==NULL) continue;
            tempt=clock()-startt;
            ///////////////////////////////
            //   exam for long button    //
            //                           //
            ///////////////////////////////
            if(sbut[nowscanbut]->buttype==2)
            {
                    if(tempt>=(sbut[nowscanbut]->gotime-movetime))
                    {
                        int tlnbn=sbut[nowscanbut]->longbutnum;
                        int ebnum=slongbutline[tlnbn]->endbutnum;
                        if(sbut[nowscanbut]->press==true)
                        {
                            positionedit=sbut[nowscanbut]->positionrecord;
                            if(positionedit>0) positionedit=1.0;
                        }
                        else
                        {
                            position=1.0-(float)(sbut[nowscanbut]->gotime-tempt)/movetime;
                            if(position>1.0) position=1.0;//little can't fit
                            positionedit=pow(position,speeddimension);
                        }
                        float positionend=1.0-(float)(sbut[ebnum]->gotime-tempt)/movetime;
                        if(positionend<0.0) positionend=0.0;
                        float positionendedit=pow(positionend,speeddimension);
                        float linewide=(float)clongbutline[tlnbn]->getWidth()/clongbutline[tlnbn]->getHeight()*(screen_height-hedge);
                        SDL_Rect trectso={0
                                        ,clongbutline[tlnbn]->getHeight()*positionendedit
                                        ,clongbutline[tlnbn]->getWidth()
                                        ,clongbutline[tlnbn]->getHeight()*(positionedit-positionendedit)};
                        SDL_Rect trectdes={(sbut[nowscanbut]->line==1)?screen_wide*(sbut[nowscanbut]->line-1)/7+10: (sbut[nowscanbut]->line<=4)? screen_wide*(sbut[nowscanbut]->line-1)/7:screen_wide-(7-sbut[nowscanbut]->line)*screen_wide/7-linewide*0.97
                                        ,(screen_height-hedge)*positionendedit
                                        ,(sbut[nowscanbut]->line<=4)?linewide*0.95:linewide*0.94
                                        ,(screen_height-hedge)*(positionedit-positionendedit)};
                        clongbutline[tlnbn]->render(&trectso,&trectdes);//----------------------->render
                        if(tempt<(sbut[ebnum]->gotime-goodtime))
                        {
                            while(SDL_PollEvent(&e));
                            {
                                currentKeyStates=SDL_GetKeyboardState( NULL );
                                switch(sbut[nowscanbut]->line)
                                {
                                    case 1:  if(currentKeyStates[SDL_SCANCODE_1]==0) slongbutline[tlnbn]->press=false; break;
                                    case 2:  if(currentKeyStates[SDL_SCANCODE_2]==0) slongbutline[tlnbn]->press=false; break;
                                    case 3:  if(currentKeyStates[SDL_SCANCODE_3]==0) slongbutline[tlnbn]->press=false; break;
                                    case 4:  if(currentKeyStates[SDL_SCANCODE_4]==0) slongbutline[tlnbn]->press=false; break;
                                    case 5:  if(currentKeyStates[SDL_SCANCODE_5]==0) slongbutline[tlnbn]->press=false; break;
                                    case 6:  if(currentKeyStates[SDL_SCANCODE_6]==0) slongbutline[tlnbn]->press=false; break;
                                    case 7:  if(currentKeyStates[SDL_SCANCODE_7]==0) slongbutline[tlnbn]->press=false; break;

                                }
                            }
                        }
                    }

            }
            ///////////////////////////////
            //   when button need to go  //
            //    into screen            //
            ///////////////////////////////
            if((tempt>=(sbut[nowscanbut]->gotime-movetime))&&(tempt<=(sbut[nowscanbut]->gotime+laterange)))
            {
                if(sbut[nowscanbut]->press==true)   positionedit=sbut[nowscanbut]->positionrecord;
                else
                {
                    position=1.0-(float)(sbut[nowscanbut]->gotime-tempt)/movetime;
                    positionedit=pow(position,speeddimension);
                }
                playwide=(float)screen_wide*((1.0/7-1.0/49)*positionedit+1.0/49);
                playheight=(float)playwide*screen_height/screen_wide;
                sbut[nowscanbut]->rect={
                screen_wide/2-playwide/2+(sbut[nowscanbut]->line-4)*playwide,
                (screen_height-hedge)*positionedit,
                playwide,
                playheight};
                cbut[nowscanbut]->render(NULL,&sbut[nowscanbut]->rect);//----------------------->render

                if(sbut[nowscanbut]->press==true) continue;

                ///////////////////////////////
                //   when button need to be  //
                //    press                  //
                ///////////////////////////////
                if((tempt<=(sbut[nowscanbut]->gotime+goodtime))&&(tempt>=(sbut[nowscanbut]->gotime-goodtime)))
                {
                        while(SDL_PollEvent(&e));
                        {
                            currentKeyStates=SDL_GetKeyboardState( NULL );
                            switch(sbut[nowscanbut]->line)
                            {
                                case 1:
                                    if((currentKeyStates[SDL_SCANCODE_1]==1)&&(press[0]==false))  {press[0]=true; changecolor=1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    if((currentKeyStates[SDL_SCANCODE_1]==0)&&(press[0]==true)) {press[0]=false; changecolor=-1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    break;
                                case 2:
                                    if((currentKeyStates[SDL_SCANCODE_2]==1)&&(press[1]==false))  {press[1]=true; changecolor=1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    if((currentKeyStates[SDL_SCANCODE_2]==0)&&(press[1]==true)) {press[1]=false; changecolor=-1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    break;
                                case 3:
                                    if((currentKeyStates[SDL_SCANCODE_3]==1)&&(press[2]==false))  {press[2]=true; changecolor=1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    if((currentKeyStates[SDL_SCANCODE_3]==0)&&(press[2]==true)) {press[2]=false; changecolor=-1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    break;
                                case 4:
                                    if((currentKeyStates[SDL_SCANCODE_4]==1)&&(press[3]==false))  {press[3]=true; changecolor=1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    if((currentKeyStates[SDL_SCANCODE_4]==0)&&(press[3]==true)) {press[3]=false; changecolor=-1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    break;
                                case 5:
                                    if((currentKeyStates[SDL_SCANCODE_5]==1)&&(press[4]==false))  {press[4]=true; changecolor=1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    if((currentKeyStates[SDL_SCANCODE_5]==0)&&(press[4]==true)) {press[4]=false; changecolor=-1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    break;
                                case 6:
                                    if((currentKeyStates[SDL_SCANCODE_6]==1)&&(press[5]==false))  {press[5]=true; changecolor=1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    if((currentKeyStates[SDL_SCANCODE_6]==0)&&(press[5]==true)) {press[5]=false; changecolor=-1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    break;
                                case 7:
                                    if((currentKeyStates[SDL_SCANCODE_7]==1)&&(press[6]==false))  {press[6]=true; changecolor=1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    if((currentKeyStates[SDL_SCANCODE_7]==0)&&(press[6]==true)) {press[6]=false; changecolor=-1;score=examscore(tempt-sbut[nowscanbut]->gotime);}
                                    break;
                            }
                            if(skilling==true)
                            {
                                if((skill[2]==1)&&(score<great)) score=great;
                                if((skill[2]==2)&&(score<perfect)) score=perfect;
                            }
                            if(((changecolor==-1)&&(sbut[nowscanbut]->buttype==4)&&(slongbutline[sbut[nowscanbut]->longbutnum]->press==true))||((changecolor==1)&&(sbut[nowscanbut]->buttype==2))||((changecolor==1)&&(sbut[nowscanbut]->buttype==1)))
                            {
                                //--------------parameter adjust---------//
                                combo++;
                                eblood-=(int)phyatk*atkrate(score,nowplayer);
                                //cout<<eblood;
                                if(skilling==true){ eblood-=skill[1]; }//cout<<"powerup "<<skill[1]<<endl;}
                                if(eblood<=0)
                                {
                                    eblood=nowplayer->totaleneblood;
                                    enekill++;
                                    if(!eneenergybarin->loadfromfile( "picture/energybar2.png",255,255,255 ))printf( "Failed to load energybar1\n" );
                                    eneenergylevel=3;
                                }
                                //----------------whether long button being press----------//
                                if(sbut[nowscanbut]->buttype==2)
                                {
                                    slongbutline[sbut[nowscanbut]->longbutnum]->press=true;
                                }
                                //----------------button being press animation--------------//
                                if(cba1==NULL)
                                {
                                    sbut[nowscanbut]->press=true;
                                    sbut[nowscanbut]->positionrecord=positionedit;
                                    cba1=cbut[nowscanbut];
                                    thread m1thread(butanimation1,sbut[nowscanbut]->buttype);
                                    m1thread.detach();
                                }
                                else if(cba2==NULL)
                                {
                                    sbut[nowscanbut]->press=true;
                                    sbut[nowscanbut]->positionrecord=positionedit;
                                    cba2=cbut[nowscanbut];
                                    thread m2thread(butanimation2,sbut[nowscanbut]->buttype);
                                    m2thread.detach();
                                }
                                else if(cba3==NULL)
                                {
                                    sbut[nowscanbut]->press=true;
                                    sbut[nowscanbut]->positionrecord=positionedit;
                                    cba3=cbut[nowscanbut];
                                    thread m3thread(butanimation3,sbut[nowscanbut]->buttype);
                                    m3thread.detach();
                                }
                                //----------skill--------------//
                                if((skillanimation==true)&&(skilling==false)&&(specialskill(nowplayer,combo,blood,enekill,skill)==true))
                                {
                                    skilling=true;
                                    //thread skillthread(gopicture1,"picture/fire.png","picture/chapicture/Riven.png");
                                    char ct[30];
                                    strcpy(ct,"picture/chapicture/");
                                    strcat(ct,nowplayer->gamecha[skill[4]]->name);
                                    strcat(ct,".png");
                                    thread skillthread(gopicture1,"picture/fire.png",ct);
                                    skillthread.detach();
                                    skillendtime=clock()+clock_t(skill[0]*1000);
                                    blood+=skill[3];
                                    if((energylevel==1)&&(blood>=nowplayer->totalblood/4))
                                    {
                                        if(!energybarin->loadfromfile( "picture/energybar3.png",255,255,255 ))printf( "Failed to load energybar2\n" );
                                        energylevel++;
                                    }
                                    else if((energylevel==2)&&(blood>=nowplayer->totalblood*2/3))
                                    {
                                        if(!energybarin->loadfromfile( "picture/energybar2.png",255,255,255 ))printf( "Failed to load energybar2\n" );
                                        energylevel++;
                                    }
                                    //cout<<"recover "<<skill[3]<<endl;
                                    //cout<<"skilltime start"<<endl;
                                }
                                //----------------score combo animation-------//
                                thread sthread(scoredisplay,score,combo);
                                sthread.detach();
                                //--------------------------------------------//
                            }
                            changecolor=0;
                        }

                }
            }
            ///////////////////////////////
            //    if not press           //
            //                           //
            ///////////////////////////////
            else if(tempt>sbut[nowscanbut]->gotime+laterange)
            {
                //--------------parameter adjust---------//
                if(sbut[nowscanbut]->press==false)
                {
                    if(combo>combomax) combomax=combo;
                    combo=0;
                    if(sbut[nowscanbut]->buttype==2)
                    {
                        if(slongbutline[sbut[nowscanbut]->longbutnum]->hurt==false)
                        {
                            blood-=ephyatk;
                            slongbutline[sbut[nowscanbut]->longbutnum]->hurt=true;
                        }
                    }
                    else
                    {
                        blood-=ephyatk;
                    }
                }
                //-----------delete button--------------//
                if(sbut[nowscanbut]->buttype==1)
                {
                    if(sbut[nowscanbut]!=NULL)
                    {
                        delete sbut[nowscanbut];
                        sbut[nowscanbut]=NULL;
                        cbut[nowscanbut]->free();
                    }
                }
                //-----------delete long button---------//
                else if(sbut[nowscanbut]->buttype==4)
                {
                    int tlnbn=sbut[nowscanbut]->longbutnum;
                    int bbnum=slongbutline[tlnbn]->beginbutnum;
                    if(sbut[bbnum]!=NULL)
                    {
                        delete sbut[bbnum];
                        sbut[bbnum]=NULL;
                        cbut[bbnum]->free();
                    }
                    if(sbut[nowscanbut]!=NULL)
                    {
                        delete sbut[nowscanbut];
                        sbut[nowscanbut]=NULL;
                        cbut[nowscanbut]->free();
                    }
                }
            }
            //--------exam skill end----------//
            if((skilling==true)&&(clock()>skillendtime))
            {
                skilling=false;
                //cout<<"skilltime end"<<endl;
            }
        }
        //------outside one time button exam----------//
        //-----------display combo and score----------//
        if(combotext!=NULL)
        {
            scoretexture->render(NULL,&scorerect);//----------------------->render
            comborect={screen_wide/2-combotext->getWidth()/2,450,combotext->getWidth(),combotext->getHeight()};
            combotext->render(NULL,&comborect);//----------------------->render
        }
        //----------display bloodbar-------------//
        if((energylevel==2)&&(blood<nowplayer->totalblood/4))
        {
            if(!energybarin->loadfromfile( "picture/energybar4.png",255,255,255 ))printf( "Failed to load energybar2\n" );
            energylevel--;
        }
        else if((energylevel==3)&&(blood<nowplayer->totalblood*2/3))
        {
            if(!energybarin->loadfromfile( "picture/energybar3.png",255,255,255 ))printf( "Failed to load energybar2\n" );
            energylevel--;
        }

        if((eneenergylevel==2)&&(eblood<nowplayer->totaleneblood/4))
        {
            if(!eneenergybarin->loadfromfile( "picture/energybar4.png",255,255,255 ))printf( "Failed to load energybar2\n" );
            eneenergylevel--;
        }
        else if((eneenergylevel==3)&&(eblood<nowplayer->totaleneblood*2/3))
        {
            if(!eneenergybarin->loadfromfile( "picture/energybar3.png",255,255,255 ))printf( "Failed to load energybar2\n" );
            eneenergylevel--;
        }
        eneebirect={10,10,eneenergybarout->getWidth()/3*eblood/nowplayer->totaleneblood,eneenergybarout->getHeight()/4};
        ebirect=
        {screen_wide-energybarout->getWidth()/3-10
        ,10
        ,(blood<=nowplayer->totalblood)?energybarout->getWidth()/3*blood/nowplayer->totalblood:energybarout->getWidth()/3
        ,energybarout->getHeight()/4};
        energybarin->render(NULL,&ebirect);
        energybarout->render(NULL,&eborect);
        eneenergybarin->render(NULL,&eneebirect);
        eneenergybarout->render(NULL,&eneeborect);
        //---------display blood number-----------//
        int gar1=sprintf(chabloodchar,"%d/%d",blood,nowplayer->totalblood);
        int gar2=sprintf(enebloodchar,"%d/%d",eblood,nowplayer->totaleneblood);
        int gar3=sprintf(killnumchar,"%d/%d",enekill,goalkill);
        chabloodtext->loadFromRenderedText(chabloodchar,textColor,gFont);
        enebloodtext->loadFromRenderedText(enebloodchar,textColor,gFont);
        killnum->loadFromRenderedText(killnumchar,textColor,gFont);
        chabloodtext->render(NULL,&chabloodrect);
        enebloodtext->render(NULL,&enebloodrect);
        killnum->render(NULL,&killnumrect);
        //-----------exam blood---------//
        if(blood<=0) break;
        SDL_RenderPresent( gRenderer );//----------------------->render present
    }
    if((blood<0)||(enekill<goalkill))
    {
        r[0]=-1;
        r[1]=-1;
        //cout<<r[0]<<" "<<r[1]<<endl;
        return r;
    }
    else
    {
        r[0]=combomax;
        r[1]=enekill;
        //cout<<r[0]<<" "<<r[1]<<endl;
        return r;
    }
}
static mutex tmutext1;
void butanimation1(int type)
{
            //cout<<1;
            tmutext1.lock();
            if(type==1)
            {
              if(!(cba1->loadfromfile( "picture/pressbutton.png",255,255,255 ))) printf( "Failed to load pressbutton media!\n" );
            }
            else
            {
              if(!(cba1->loadfromfile( "picture/longpressbutton.png",255,255,255 ))) printf( "Failed to load pressbutton media!\n" );
            }
            for(int c1=0;c1<10;c1++)
            {
              SDL_Delay(20);
              cba1->setalpha(255-25.5*c1);
            }
            cba1=NULL;
            tmutext1.unlock();
}
static mutex tmutext2;
void butanimation2(int type)
{
            //cout<<2;
            tmutext2.lock();
            if(type==1)
            {
              if(!(cba2->loadfromfile( "picture/pressbutton.png",255,255,255 ))) printf( "Failed to load pressbutton media!\n" );
            }
            else
            {
              if(!(cba2->loadfromfile( "picture/longpressbutton.png",255,255,255 ))) printf( "Failed to load pressbutton media!\n" );
            }
            for(int c1=0;c1<10;c1++)
            {
              SDL_Delay(20);
              cba2->setalpha(255-25.5*c1);
            }
            cba2=NULL;
            tmutext2.unlock();
}
static mutex tmutext3;
void butanimation3(int type)
{
            //cout<<3;
            tmutext3.lock();
            if(type==1)
            {
              if(!(cba3->loadfromfile( "picture/pressbutton.png",255,255,255 ))) printf( "Failed to load pressbutton media!\n" );
            }
            else
            {
              if(!(cba3->loadfromfile( "picture/longpressbutton.png",255,255,255 ))) printf( "Failed to load pressbutton media!\n" );
            }
            for(int c1=0;c1<10;c1++)
            {
              SDL_Delay(20);
              cba3->setalpha(255-25.5*c1);
            }
            cba3=NULL;
            tmutext3.unlock();
}



int* music(std::string music,std::string tempo,std::string musicbackground,player* nowplayer,bool mv,bool skillanimation,int goal)
{


    cout<<1;
    static int* r=new int[2];
    if(!tbgmusic1.loadfromfile( "picture/musictable3.png",255,255,255 )){printf( "Failed to load music1table media!\n" );}
    if(!tbgmusic2.loadfromfile( musicbackground.c_str())){printf( "Failed to load backgroundmedia!\n" );}
    if(!loadsoundmusic(music.c_str())){printf( "Failed to load gamemusic1\n" );}
    ching = Mix_LoadWAV( "music/effect1.wav" );
	if( ching == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
    gFont = TTF_OpenFont( "new.ttc", 18 );
    if( gFont == NULL ) printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    gFontcombo= TTF_OpenFont( "new.ttc", 60 );
    if( gFontcombo == NULL ) printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    SDL_RenderClear( gRenderer );//----------------------->render clear
    butini(tempo.c_str());
    for(int c1=0;c1<=10;c1++)
    {
        SDL_RenderClear( gRenderer );//----------------------->render clear
        if(mv==false)
        {
            tbgmusic2.setalpha(c1*20);
            tbgmusic2.render();//----------------------->render
        }
        tbgmusic1.setalpha(c1*20);
        tbgmusic1.render();//----------------------->render
        SDL_RenderPresent( gRenderer );//----------------------->render present
        SDL_Delay(20);
    }
    if(mv==true)
    {
        thread sthread(movie);
        sthread.detach();
    }
    r=gobutton(nowplayer,mv,skillanimation,goal);
    closegamemusic1();
    return r;
}

