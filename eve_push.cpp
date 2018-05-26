#include "balls.h"
#include <iostream>
#include "quad_tree.h"

int main()
{
    QuadTree::Test();
    Balls balls("EveLigh engine", 800, 600);
    balls.Start();

    return 0;
}
