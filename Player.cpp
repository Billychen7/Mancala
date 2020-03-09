//
//  Player.cpp
//  Project 3
//
//  Created by Billy Chen on 5/18/19.
//  Copyright © 2019 William Chen. All rights reserved.
//

#include "Player.h"

Player::Player(string name){
    m_name = name;
}
// Create a Player with the indicated name.
string Player::name() const{
    return m_name;
}
//Return the name of the player.
bool Player::isInteractive() const{
    return false;
}
// Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
Player::~Player(){
}

HumanPlayer::HumanPlayer(string name):Player(name){
    
}
bool HumanPlayer::isInteractive() const{
    return true;
}
int HumanPlayer::chooseMove(const Board& b, Side s) const{
    cout << "Choose a hole, " << name() << ": ";
    int move = 0;
    bool isValid = false;
    //check if the move is valid
    while (!isValid)
    {
        cin >> move;
        
        if (move <= 0 || move > b.holes() || b.beans(s, move) == 0)
        {
            cout << "Hole is not valid";
        }
        else
            isValid = true;
    }
    return move;
}
HumanPlayer::~HumanPlayer(){
}

BadPlayer::BadPlayer(string name)
:Player(name)
{}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    int max = b.beans(s,1), largest = 1;

    //Find the hole with the most amount of beans
    for (int x = 1; x < b.holes() + 1; x++)
    {
        if (b.beans(s, x) > max)
        {
            max = b.beans(s,x);
            largest = x;
        }
    }
    
    //If the max is zero, then there is an error
    if (max == 0)
    {
        return -1;
    }
    
    return largest;
}

BadPlayer::~BadPlayer()
{}

SmartPlayer::SmartPlayer(string name)
:Player(name)
{}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    int max = b.beans(s,1), largest = 1;
    
    //Find the hole with the most amount of beans
    for (int x = 1; x < b.holes() + 1; x++)
    {
        if (b.beans(s, x) > max)
        {
            max = b.beans(s,x);
            largest = x;
        }
    }
    
    //If the max is zero, then there is an error
    if (max == 0)
    {
        return -1;
    }

    return largest;
}

SmartPlayer::~SmartPlayer()
{}
