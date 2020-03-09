//
//  Main.cpp
//  Project 3
//
//  Created by Billy Chen on 5/16/19.
//  Copyright © 2019 William Chen. All rights reserved.
//
#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    HumanPlayer hp("Marge");
    BadPlayer bp("Homer");
    Board b(3, 2);
    Game g(b, &hp, &bp);
    g.play();
}
