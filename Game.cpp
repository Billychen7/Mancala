//
//  Game.cpp
//  Project 3
//
//  Created by Billy Chen on 5/19/19.
//  Copyright © 2019 William Chen. All rights reserved.
//

#include "Game.h"

Game::Game(const Board& b, Player* south, Player* north):board(b){
    nPlayer = north;
    sPlayer = south;
    side = SOUTH;
}
//Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
void Game::display() const{
    int nameSpacing = board.holes()/2+1;
    bool nPotDoubleDigit = false;
    int holeDoubleDigit = 0;
    if (board.beans(NORTH, 0) >= 10)
        nPotDoubleDigit = true;
    //input names
    for (int i = 0; i < nameSpacing; i++)
        cout << "  ";
    if(nPotDoubleDigit)
        cout << "  ";
    cout << nPlayer->name() << endl;
    //input North line of holes
    cout << " ";
    if(nPotDoubleDigit)
        cout << "  ";
    for (int i = 1; i < board.holes() + 1; i++)
    {
        cout << "  " << board.beans(NORTH, i);
    }
    cout << endl;
    //input the pot values
    cout << " " << board.beans(NORTH, 0);
    for (int i = 0; i < board.holes(); i++)
    {
        cout << "   ";
    }
    for (int i = 1; i < board.holes() + 1; i++)
    {
        if (board.beans(SOUTH, i) >= 10)
            holeDoubleDigit++;
    }
    for (int i = 0; i < holeDoubleDigit; i++)
    {
        cout << "  ";
    }
    
    cout << board.beans(SOUTH, 0);
    cout << endl;
    //input the south pot values
    cout << " ";
    if(nPotDoubleDigit)
        cout << "  ";
    for (int i = 1; i < board.holes() + 1; i++)
    {
        cout << "  " << board.beans(SOUTH, i);
    }
    cout << endl;
    //input name
    for (int i = 0; i < nameSpacing; i++)
        cout << "  ";
    if(nPotDoubleDigit)
        cout << "  ";
    cout << sPlayer->name() << endl;
    cout << "===========================" << endl;
}
void Game::status(bool& over, bool& hasWinner, Side& winner) const{
    //If game is over
    if (board.beansInPlay(NORTH) == 0 || board.beansInPlay(SOUTH) == 0)
    {
        //its a tie
        if (board.beans(NORTH, 0) == board.beans(SOUTH, 0))
        {
            hasWinner = false;
        }
        //south won
        else if (board.beans(SOUTH, 0) > board.beans(NORTH, 0))
        {
            hasWinner = true;
            winner = SOUTH;
        }
        //north won
        else if(board.beans(NORTH, 0) > board.beans(SOUTH, 0))
        {
            hasWinner = true;
            winner = NORTH;
        }
    over = true;
    }
    else
    over = false;
}
bool Game::move(){
    Side endSide;
    //create two pointers every move, one for the current playing hand the other for the opponent
    Player* ptr1;
    Player* ptr2;
    int move;
    int end;
    //used for status
    bool over;
    bool hasWinner;
    Side winner;
    //setting player pointers
    if (side == SOUTH)
    {
        ptr1 = sPlayer;
        ptr2 = nPlayer;
    }
    else
    {
        ptr1 = nPlayer;
        ptr2 = sPlayer;
    }
    move = ptr1 -> chooseMove(board, side);
    board.sow(side, move, endSide, end);
    status(over, hasWinner, winner);
    //check if someone won
    if(over)
    {
        display();
        
        for (int i = 0; i < board.holes() + 1; i++)
            board.moveToPot(opponent(side), i , opponent(side));
        return false;
    }
    //free turn from ending in your own pot
    while (endSide == side && end == 0)
    {
        display();
        cout << ptr1 -> name() << " gets a free turn." << endl;
        move = ptr1 -> chooseMove(board, side);
        board.sow(side, move, endSide, end);
        status(over, hasWinner, winner);
        if(over)
        {
            display();
            
            for (int i = 0; i < board.holes() + 1; i++)
                board.moveToPot(opponent(side), i , opponent(side));
            return false;
        }
    }
    //capture
    if (endSide == side && end != 0)
    {
        if (board.beans(side, end) - 1 == 0 && board.beans(opponent(side), end) != 0)
        {
            board.moveToPot(side, end, side);
            board.setBeans(side, end, 0);
            board.moveToPot(opponent(side), end, side);
            board.setBeans(opponent(side), end, 0);
        }
    }
    status(over, hasWinner, winner);
    //check if its over
    if(over)
    {
        display();
        
        for (int i = 0; i < board.holes() + 1; i++)
            board.moveToPot(opponent(side), i , opponent(side));
        return false;
    }
    side = opponent(side);
    return true;
}
void Game::play(){
    display();
    while (move())
    {
        //if there are two computers, use enter to continue to next mobes
        if(!nPlayer->isInteractive() && !sPlayer->isInteractive()){
            cout << "Press Enter" << endl;
            cin.ignore(100, '\n');
            display();
        }
        else
            display();
    }
        //final display
    display();
    bool over;
    bool won;
    Side winner;
    status(over, won, winner);
    if(over == true){
        if(won == true){
        //there is a winner
            if (winner == NORTH)
                cout << "The winner is " << nPlayer -> name() << endl;
            else if (winner == SOUTH)
                cout << "The winner is " << sPlayer -> name() << endl;
        }
        else
        cout << "It's a tie." << endl;
    }
}
int Game::beans(Side s, int hole) const{
    if (hole < 0 || hole > board.holes())
        return -1;
    return board.beans(s, hole);
}
