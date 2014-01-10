#include <iostream>
#include "libconsole/console.h"
#include "game.h"

using namespace std;
using namespace con;

/// Program entry point
int main()
{
    CGame game( 60, 30 );
    game.run();

    return 0;
}
