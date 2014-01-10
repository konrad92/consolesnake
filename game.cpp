#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include "libconsole/console.h"
#include "game.h"

using namespace std;
using namespace con;

const char *logoData[] = {
    " ____              _        ",
    "/ ___| _ __   __ _| | _____ ",
    "\\___ \\| '_ \\ / _` | |/ / _ \\",
    " ___) | | | | (_| |   <  __/",
    "|____/|_| |_|\\__,_|_|\\_\\___|",
};

///************************************************************
///
/// Utilities
///
///************************************************************

/// Print out the data
void pout( color col, cursor cur, const char **data, int rows )
{
    cout<< col;

    for( int n = 0; n < rows; n++ )
    {
        cout<< cur<< data[n];
        cur.move( 0, 1 );
    }
}

/// Test the collision with snake
bool insnake( vector<cursor> snake, cursor obstacle )
{
    for( int n = 0; n < snake.size(); n++ )
    {
        if( snake[n] == obstacle )
            return true;
    }

    return false;
}

///************************************************************
///
/// Game application class
///
///************************************************************

/// The default constructor
CGame::CGame( int width, int height )
    : m_width( width ), m_height( height ), m_gamestate( GAMESTATE_MENU )
{
    // initialize console dimmension
    cout<< dimm( m_width, m_height, true );
}

/// Run the game!
void CGame::run()
{
    // game mainloop
    while( m_gamestate != GAMESTATE_QUIT )
    {
        cout<< cls();

        // switch between states
        switch( m_gamestate )
        {
        case GAMESTATE_MENU:
            this->menu();
            break;
        case GAMESTATE_GAMEPLAY:
            this->gameplay();
            break;
        }
    }
}

/// Menu state
void CGame::menu()
{
    bool redraw = true,
         keydwn = false;
    char choice = 0;

    pout( color(color::green|color::intensity), cursor(11, 2), logoData, 5 );

    cout<< color(color::intensity)
        << cursor(50 - strlen(AUTHOR), 8)<< "by "AUTHOR;

    cout<< color(color::red|color::intensity)
        << cursor(59 - strlen(VERSION), 28)<< VERSION;

    // menu loop
    while( true )
    {
        if( kb::keyDown( kb::vk_escape ) )
        {
            SETSTATE( GAMESTATE_QUIT );
        }

        // user choice
        if( kb::keyDown( kb::vk_down ) || kb::keyDown( kb::vk_up ) )
        {
            if( !keydwn && kb::keyDown( kb::vk_down ) )
            {
                choice = MIN( 2, choice + 1 );
                redraw = true;
            }
            else
            if( !keydwn && kb::keyDown( kb::vk_up ) )
            {
                choice = MAX( 0, choice - 1 );
                redraw = true;
            }

            keydwn = true;
        }
        else
            keydwn = false;

        // handle the choice
        if( kb::keyDown( kb::vk_return ) )
        {
            switch( choice )
            {
            case 0: SETSTATE( GAMESTATE_GAMEPLAY );
            //case 1: SETSTATE( GAMESTATE_SCOREBOARD );
            case 2: SETSTATE( GAMESTATE_QUIT );
            }
        }

        // redraw the navigation
        if( redraw )
        {
            cout<< color( (choice==0)? color::white: color::grey )
                << cursor( 19, 19 )<< "> Start the game!";

            cout<< color( (choice==1)? color::white: color::grey )
                << cursor( 19, 20 )<< "> Look at scoreboard (WIP)";

            cout<< color( (choice==2)? color::white: color::grey )
                << cursor( 19, 21 )<< "> Quit to Windows";

            redraw = false;
        }
    }
}

/// Gameplay state
void CGame::gameplay()
{
    vector<cursor> snake;

    cursor apple( m_width/2, m_height/2 - 8 );

    unsigned int timer = ticks() + SNAKE_SPEED;
    char dir = SNAKEDIR_UP;
    bool redraw = true,
         died = false;

    // initialize snake
    snake.push_back( cursor(m_width/2, m_height/2 - 1) );
    snake.push_back( cursor(m_width/2, m_height/2) );
    snake.push_back( cursor(m_width/2, m_height/2) );

    // gameplay loop
    while( !died )
    {
        // snake control
        if( kb::keyDown( kb::vk_right ) && dir != SNAKEDIR_LEFT )
        {
            dir = SNAKEDIR_RIGHT;
        }

        if( kb::keyDown( kb::vk_down ) && dir != SNAKEDIR_UP )
        {
            dir = SNAKEDIR_DOWN;
        }

        if( kb::keyDown( kb::vk_left ) && dir != SNAKEDIR_RIGHT )
        {
            dir = SNAKEDIR_LEFT;
        }

        if( kb::keyDown( kb::vk_up ) && dir != SNAKEDIR_DOWN )
        {
            dir = SNAKEDIR_UP;
        }

        // snake move
        if( ticks() > timer )
        {
            cursor old = snake[0], b_old(0, 0);

            for( int n = 1; n < snake.size(); n++ )
            {
                b_old = snake[n];

                cout<< snake[n]<< ' ';
                snake[n] = old;
                old = b_old;
            }

            switch( dir )
            {
            case SNAKEDIR_RIGHT:
                if( snake[0].getx() == m_width - 1 )
                    died = true;
                else
                    snake[0].move( 1, 0 );
                break;

            case SNAKEDIR_DOWN:
                if( snake[0].gety() == m_height - 1 )
                    died = true;
                else
                    snake[0].move( 0, 1 );
                break;

            case SNAKEDIR_LEFT:
                if( snake[0].getx() == 0 )
                    died = true;
                else
                    snake[0].move( -1, 0 );
                break;

            case SNAKEDIR_UP:
                if( snake[0].gety() == 0 )
                    died = true;
                else
                    snake[0].move( 0, -1 );
                break;
            }

            // snake ate apple
            if( snake[0] == apple )
            {
                while( insnake( snake, apple ) )
                    apple.set( rand()%m_width, rand()%m_height );

                snake.push_back( snake.back() );
            }

            // snake ate himself
            for( int n = 1; n < snake.size(); n++ )
            {
                if( snake[0] == snake[n] )
                    died = true;
            }

            timer = ticks() + SNAKE_SPEED;
            redraw = true;
        }

        // redraw the snake
        if( redraw )
        {
            // paint the snake
            cout<< color(color::cyan|color::intensity);

            for( int n = 0; n < snake.size(); n++ )
            {
                cout<< snake[n]<< (n==0? SNAKE_HEAD: SNAKE_TAIL);
            }

            // paint the apple
            cout<< color(color::red|color::intensity)<< apple<< "o";

            redraw = false;
        }
    }

    // player died
    stringstream scoreinf;
    scoreinf<< "You died! tail length: "<< snake.size();

    cout<< color( color::red|color::intensity )
        << cursor(m_width/2-scoreinf.str().size()/2, m_height/2)
        << scoreinf.str();

    cout<< cursor(m_width/2-10, m_height/2+1)
        << "Restart the game? Y/N";

    // died loop
    while( died )
    {
        if( kb::keyDown( 'Y' ) )
        {
            RESETSTATE();
        }
        else
        if( kb::keyDown( 'N' ) )
        {
            SETSTATE( GAMESTATE_MENU );
        }
    }
}


