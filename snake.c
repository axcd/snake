#include <math.h>
#include<time.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

typedef struct _Node
{
	int x , y ;
	struct _Node* pre;	
	struct _Node* next; 
} Node;

SDL_Surface *message = NULL;
SDL_Surface *message1 = NULL;
SDL_Surface *message2 = NULL;
SDL_Surface *square = NULL;
SDL_Surface *screen = NULL;
SDL_Event event;

TTF_Font *font = NULL;
TTF_Font *font2 = NULL;
SDL_Color textColor = { 0, 0, 255 };
SDL_Color textColor2 = { 50, 100, 200 };

Node box, *node, *nodes;

int xVel, yVel, fps;
int quit = 0;
int flag = 0;
int score = 0;
char str[5]={'0'};

char* itoa(int integer)
{
	int x, n=0, i=0;
	
	x = integer;
	while (x>0){
		x=x/10;
		n++;
	}
	
	x = integer;
	while (i<n){
		str[i] = 48+x/( int )pow( 10, n-i-1);
		x=x%( int )pow( 10, n-i-1);
		i++;
	}
	
   str[++i]='\0';
   
   return str;
}

void getList()
{
	nodes -> pre = nodes ;
	nodes -> next = nodes ;
	nodes -> x = 0 ;
	nodes -> y = 0 ;
	return;
}

void getBox()
{
	
	node = nodes;      
    while (1)
    {
    	if ( box.x == node->x && box.y == node->y )
    	{
    		srand(time(NULL)); 
    		box.x=rand()%32*20;
    		box.y=rand()%24*20;
    	}
    	if (node == nodes->pre)break;
    	node = node->next;   
    }
    return;
}

SDL_Surface *load_image( const char* filename )
{
    SDL_Surface* loadedImage = NULL;

    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, NULL, destination, &offset );
    return;
}

int init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 0;
    }

    screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );

    if( screen == NULL )
    {
        return 0;
    }  
    
    if( TTF_Init() == -1 )
    {
        return 0;
    }
    
   nodes = (Node *)malloc(sizeof(Node));
   getList();
   getBox();
   
    SDL_WM_SetCaption( "Snake", NULL );

    return 1;
}

int load_files()
{
    square = load_image( "resource/square.bmp" );
    
    if( square == NULL )
    {
        return 0;
    }
    
    font = TTF_OpenFont( "resource/lazy.ttf", 28 );
    font2 = TTF_OpenFont( "resource/lazy.ttf", 100 );

    if( font == NULL )
    {
        return 0;
    }
    
    if( font2 == NULL )
    {
        return 0;
    }
    
    return 1;
}

void clean_up()
{
    SDL_FreeSurface( square );
    SDL_FreeSurface( message );
    SDL_FreeSurface( message1 );
    SDL_FreeSurface( message2 );
    
    TTF_CloseFont( font );
    TTF_CloseFont( font2 );
    TTF_Quit();

    SDL_Quit();
    return;
}

int isHit()
{
	node = nodes->next;       
    while (1)
    {
    	if (node == nodes->pre) break;
    	node = node->next;   
    	if ( node->x == nodes->x && node->y == nodes->y ) return 1;
    }
    return 0;
}

void updateHead()
{
	nodes = nodes->pre;
	
	nodes->x = nodes->next->x + xVel;
	nodes->y = nodes->next->y + yVel;
		
	if (nodes->x<0)
	{
		nodes->x = nodes->x+640;
	}
	
	if (nodes->x>=640)
	{
		nodes->x = nodes->x-640;
	}
	
	if (nodes->y<0)
	{
		nodes->y = nodes->y+480;
	}
	
	if (nodes->y>=480)
	{
		nodes->y = nodes->y-480;
	}
	return ;
}

void insertTail()
{
	node = (Node *)malloc(sizeof(Node));
	node->pre = nodes->pre;
	node->next = nodes;
	nodes->pre = node;
	node->pre->next = node;
	node->x = box.x;
	node->y = box.y;
	return ;
}

void handle_input()
{
    if( event.type == SDL_KEYUP )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: 
                if (yVel==0){
                    xVel = 0 ;
                    yVel = -20 ; 
                }
                break;
            case SDLK_DOWN:        
                if (yVel==0){
                    xVel = 0 ;
                    yVel = +20 ; 
                }
                break;
            case SDLK_LEFT: 
                if (xVel==0){
                    yVel = 0 ;
                    xVel = -20 ; 
                }
                break;
            case SDLK_RIGHT: 
                if (xVel==0){
                    yVel = 0 ;
                    xVel = +20 ; 
                }
                break;
        }
    }
    return;
}

void move()
{    
	if ( isHit() == 1 ) 
	{
		flag = flag>0? 0:1 ;
		return;
	}
    updateHead();

    if ( box.x == nodes->pre->x && box.y == nodes->pre->y )
    {
    	insertTail();
    	score++;
    	getBox();
    }
    return;
}

void show( int x, int y )
{
    apply_surface( x, y, square, screen );
    return;
}

void showNodes()
{
	node = nodes;      
    while (1)
    {
    	show(node->x,node->y);
    	if (node == nodes->pre)break;
    	node = node->next;   
    }
    return;
}

int main( int argc, char* args[] )
{
    if( init() == 0 )
    {
        return 1;
    }

    if( load_files() == 0 )
    {
        return 1;
    }       
    
    message1 = TTF_RenderText_Solid( font, "score:", textColor );
    message2 = TTF_RenderText_Solid( font2, "GAME OVER", textColor2 );
    
    while( quit == 0 )
    {
    	
        fps = SDL_GetTicks();

        while ( SDL_PollEvent( &event ) )
        {            
            if( event.type == SDL_QUIT )
            {
                quit = 1;
            }
        }

        handle_input();
        move();

        message = TTF_RenderText_Solid( font, itoa(score), textColor );

        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) ); 
        apply_surface( 260, 40, message1, screen );
        apply_surface( 350, 40, message, screen );
        
        show(box.x,box.y); 
        if (flag==0)
        {
            showNodes();
        }else{
        	apply_surface( 100, 240, message2, screen );
        }
        
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        fps = SDL_GetTicks()-fps;
        if( fps < 300 )
        {
            SDL_Delay( 300 - fps );
        }
    }
    
    clean_up();

    return 0;
}