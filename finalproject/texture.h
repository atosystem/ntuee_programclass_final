#ifndef TEXTURE_H
#define TEXTURE_H
#include<SDL.h>
#include <SDL_image.h>
#include<string>
#include <SDL_ttf.h>
extern const int screen_wide;
extern const int screen_height;
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
class main1texture
{
	public:
		main1texture()
        {
            mtexture = NULL;
            mwidth = 0;
            mheight = 0;
        }
		~main1texture()
		{
		    free();
		}
        bool loadfromfile( std::string path ,int r=0,int g=0,int b=0)
        {
            free();
            SDL_Texture* newtexture = NULL;
            SDL_Surface* loadedsurface = IMG_Load( path.c_str() );
            if( loadedsurface == NULL )
                printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
            else
            {
                SDL_SetColorKey( loadedsurface, SDL_TRUE, SDL_MapRGB( loadedsurface->format, r, g, b ) );
                newtexture = SDL_CreateTextureFromSurface( gRenderer, loadedsurface );

                if( newtexture == NULL )
                    printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
                else
                {
                    mwidth = loadedsurface->w;
                    mheight = loadedsurface->h;
                }
                SDL_FreeSurface( loadedsurface );
            }

            mtexture = newtexture;
            return mtexture !=NULL;
        }
		void free()
		{
		    if( mtexture != NULL )
            {
                SDL_DestroyTexture( mtexture );
                mtexture = NULL;
                mwidth = 0;
                mheight = 0;
            }
		}
		void setColor( Uint8 red, Uint8 green, Uint8 blue )
		{
		    SDL_SetTextureColorMod( mtexture, red, green, blue );
		}
		void setblendmode( SDL_BlendMode blending )
		{
		    SDL_SetTextureBlendMode( mtexture, blending );
		}
		void setalpha( Uint8 alpha )
		{
		    SDL_SetTextureAlphaMod( mtexture, alpha );
		}
		void render(SDL_Rect* source = NULL,SDL_Rect* destination = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE )
		{
            SDL_RenderCopy( gRenderer, mtexture, source, destination );
		}
		int getWidth()
		{
		    return mwidth;
		}
		int getHeight()
		{
		    return mheight;
		}
		SDL_Texture* gettextture()
		{
		    return mtexture;
		}
	protected:
		SDL_Texture* mtexture;
		int mwidth;
		int mheight;
};
class texttexture:public main1texture
{
public:
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor,TTF_Font *tFont )
    {
        //Get rid of preexisting texture
        free();

        //Render text surface
        SDL_Surface* textSurface = TTF_RenderText_Solid( tFont, textureText.c_str(), textColor );
        if( textSurface == NULL )
        {
            printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        }
        else
        {
            //Create texture from surface pixels
            mtexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
            if( mtexture == NULL )
            {
                printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Get image dimensions
                mwidth = textSurface->w;
                mheight = textSurface->h;
            }

            //Get rid of old surface
            SDL_FreeSurface( textSurface );
        }

        //Return success
        return mtexture != NULL;
    }
};
#endif // TEXTURE_H
