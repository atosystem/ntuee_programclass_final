#include<SDL_mixer.h>
#include<SDL.h>
#include<stdio.h>
#include<SDL_image.h>
#include"texture.h"
static Mix_Music *entermusic = NULL;
static Mix_Music *admusic1 = NULL;
static Mix_Music *admusic2 = NULL;
static Mix_Chunk *effect1 = NULL;
static main1texture gprompttexture;
static main1texture glogotexture;

/*bool loadmedia()
{


	bool success = true;

	Mix_FreeMusic( entermusic );
	entermusic = Mix_LoadMUS( "music/bangdreamop.wav" );
	if( entermusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

    Mix_FreeMusic( admusic1 );
	admusic1 = Mix_LoadMUS( "music/bushimo.wav" );
	if( admusic1 == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	Mix_FreeMusic( admusic2 );
	admusic2 = Mix_LoadMUS( "music/craftegg.wav" );
	if( admusic1 == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	return success;
}*/

void ad1()
{

    Mix_AllocateChannels(2);
    Mix_PlayMusic( admusic1, 0 );
    if(!gprompttexture.loadfromfile( "picture/bushimo.png" )){printf( "Failed to load enter1 media!\n" );return;}
    gprompttexture.setblendmode(SDL_BLENDMODE_BLEND);
    for(int c1=0;c1<=100;c1++)
    {
        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255);
        SDL_RenderClear( gRenderer );
        gprompttexture.setalpha(2.55*c1);
        gprompttexture.render( 0, 0 );
        SDL_RenderPresent( gRenderer );
        SDL_Delay(20);
    }

}

void ad2()
{

    Mix_AllocateChannels(2);
    Mix_PlayMusic( admusic2, 0 );
    if(!gprompttexture.loadfromfile( "picture/craftegg2.png" )){printf( "Failed to load enter1 media!\n" );return;}
    gprompttexture.setblendmode(SDL_BLENDMODE_BLEND);
    for(int c1=0;c1<=100;c1++)
    {
        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255);
        SDL_RenderClear( gRenderer );
        gprompttexture.setalpha(2.55*c1);
        gprompttexture.render( 0, 0 );
        SDL_RenderPresent( gRenderer );
        SDL_Delay(20);
    }

}

void goenter()
{
    ///////fade in bangdream wallpaper//////////
    if(!(gprompttexture.loadfromfile( "picture/778315.png" ))){printf( "Failed to load enter1 media!\n" );return;} //mtexture=bangdream wallpaper
    Mix_PlayMusic( entermusic, -1 );
    gprompttexture.setblendmode(SDL_BLENDMODE_BLEND);
    for(int c1=0;c1<=25;c1++)
    {
        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255);
        SDL_RenderClear( gRenderer );

        gprompttexture.setalpha(10.2*c1);
        gprompttexture.render();
        SDL_RenderPresent( gRenderer );
        SDL_Delay(20);
    }
    ///////fade in logo//////////
    if(!glogotexture.loadfromfile( "picture/logo.png" )){printf( "Failed to load enter1 media!\n" );return;} //reinitiate mtexture and grender//mtexture=bangdream logo

    glogotexture.setblendmode(SDL_BLENDMODE_BLEND);
    for(int c1=0;c1<=25;c1++)
    {
        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);
        gprompttexture.render();

        glogotexture.setalpha(10.2*c1);
        SDL_Rect tempdesrect={screen_wide/2-glogotexture.getWidth()/2,screen_height-glogotexture.getHeight()-c1,glogotexture.getWidth()*0.8,glogotexture.getHeight()*0.8};
        glogotexture.render(NULL,&tempdesrect);
        SDL_RenderPresent( gRenderer );
        SDL_Delay(20);
    }

    //////exam button////////
    bool enter = false;
    SDL_Event e;
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    while( !enter )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_KEYDOWN )
            {
                currentKeyStates = SDL_GetKeyboardState( NULL );
                if( currentKeyStates[ SDL_SCANCODE_SPACE ] ) enter=true;
            }
        }
    }
    Mix_HaltMusic();
}

/*bool init()
{
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_wide, screen_height, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )////////////////////
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
    SDL_RenderPresent(gRenderer);
	return success;
}

*/
//void closestart()
/*{
	gprompttexture.free();

	Mix_FreeChunk( effect1 );
	effect1 = NULL;

	Mix_FreeMusic( entermusic );
	entermusic = NULL;

	//Destroy window
	/*SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();*/
//}

//void  gamestart()
/*{
	//if(!init()) {printf( "Failed to initialize!\n" );}/////
	//if(!loadmedia()) {printf( "Failed to load media\n" );return;}
	////////
    //ad1();
    //SDL_Delay(200);
    //ad2();
    //SDL_Delay(200);
    //goenter();
    //closestart();
}*/
