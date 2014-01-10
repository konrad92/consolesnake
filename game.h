#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include "libconsole/console.h"

#define AUTHOR "Konrad Nowakowski"
#define VERSION "1.0a"

#define SNAKE_SPEED 110
#define SNAKE_HEAD ((char)0x1)
#define SNAKE_TAIL 'x'

#define MIN(x, y) (((x) < (y))? (x): (y))
#define MAX(x, y) (((x) > (y))? (x): (y))

#define SETSTATE(news) { m_gamestate = (news); return; }
#define RESETSTATE() { return; }

/** \brief Game state constants
 */
enum
{
    GAMESTATE_QUIT = 0,
    GAMESTATE_MENU,
    GAMESTATE_GAMEPLAY,
    GAMESTATE_SCOREBOARD,
};

/** \brief Snake movement direction constant.
 */
enum
{
    SNAKEDIR_RIGHT = 0,
    SNAKEDIR_DOWN,
    SNAKEDIR_LEFT,
    SNAKEDIR_UP,
};

/** \brief Utility function to print out the data at given position and colour.
 *
 * \param col The colour.
 * \param cur The cursor.
 * \param data Data to print out.
 * \param rows Rows of the data.
 *
 */
void pout( con::color col, con::cursor cur, const char **data, int rows );

/** \brief Game application class.
 */
class CGame
{
public:
    /** \brief The default constructor.
     *
     * \param width,height The size of the game console.
     *
     */
    CGame( int width, int height );

    /** \brief Run the game!
     *
     */
    void run();

    /** \brief Menu gamestate.
     *
     */
    void menu();

    /** \brief Gameplay gamestate.
     *
     */
    void gameplay();

private:
    int m_width, m_height;

    int m_gamestate;
};


#endif // _GAME_H_
