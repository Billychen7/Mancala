//
//  Board.cpp
//  Project 3
//
//  Created by Billy Chen on 5/16/19.
//  Copyright © 2019 William Chen. All rights reserved.
//

#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole){
    numholes = nHoles;
    if(nHoles < 0)
        numholes = 1;
    if(nInitialBeansPerHole < 0)
        nInitialBeansPerHole = 0;
    m_northBeans.push_back(0);
    m_southBeans.push_back(0);
    for (int i = 0; i < numholes; i++)
    {
        m_northBeans.push_back(nInitialBeansPerHole);
        m_southBeans.push_back(nInitialBeansPerHole);
    }
}
//Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
int Board::holes() const{
    return numholes;
}
//Return the number of holes on a side (not counting the pot).
int Board::beans(Side s, int hole) const{
    if (hole < 0 || hole > numholes)
        return -1;
    if(s == NORTH){
        return m_northBeans[hole];
    }
    else{
        return m_southBeans[hole];
    }
}
//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
int Board::beansInPlay(Side s) const{
    int count = 0;
    if(s == NORTH){
        for (int i = 1; i < holes() + 1; i++){
            count += m_northBeans[i];
        }
    }
    else{
        for (int i = 1; i < holes() + 1; i++){
            count += m_southBeans[i];
        }
    }
        return count;

}
//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
int Board::totalBeans() const{
    return beansInPlay(SOUTH) + beansInPlay(NORTH) + beans(SOUTH, 0) + beans(NORTH,0);
}
//Return the total number of beans in the game, including any in the pots.
bool Board::sow(Side s, int hole, Side& endSide, int& endHole){
    if (hole <= 0 || hole > numholes || beans(s, hole) == 0)
        return false;
    int numbeans = 0;
    int lasthole = -1;
    bool first = true;
    if(s == SOUTH){
        numbeans = beans(SOUTH, hole);
        m_southBeans[hole] = 0;
        while(numbeans > 0){
            for(int i = hole+1; i < numholes + 1; i++){
                m_southBeans[i]++;
                numbeans--;
                if (numbeans == 0)//there are no more beans from inital hole
                {
                    //If we are at the South pot, break out of this loop
                    if (i == numholes + 1)
                        break;
                    endHole = i;
                    endSide = SOUTH;
                    lasthole = i;
                    break;
                }
            }
            if (endSide == SOUTH && endHole == lasthole)
                break; //leave while loop, done sowing
            if (first)
            {
                m_southBeans[0]++;
                numbeans--;
                first = false;
                
                if (numbeans == 0)//there are no more beans from inital hole
                {
                    lasthole = 0;
                    endSide = SOUTH;
                    endHole = lasthole;
                    break;
                }
            }
            lasthole = 1;
            if (numbeans == 0)//there are no more beans from inital hole
            {
                endSide = SOUTH;
                endHole = lasthole;
                break;
            }
            int lastholeN = -1;
            for (int j = numholes; j >= 1; j--)
            {
                m_northBeans[j]++;
                numbeans--;
                
                if (numbeans == 0)//there are no more beans from inital hole
                {
                    lastholeN = j;
                    endSide = NORTH;
                    endHole = lastholeN;
                    break;
                }
            }
            if (endSide == NORTH && endHole == lastholeN)
                break;
        }
    }
    else if(s == NORTH){
        numbeans = beans(NORTH, hole);
        m_northBeans[hole] = 0;
        while(numbeans > 0){
            for(int i = hole-1; i >=0; i--){
                m_northBeans[i]++;
                numbeans--;
                if (numbeans == 0)//there are no more beans from inital hole
                {
                    endHole = i;
                    endSide = NORTH;
                    lasthole = i;
                    break;
                }
            }
            if (endSide == NORTH && endHole == lasthole)
                break; //leave while loop, done sowing
            lasthole = numholes;
            int lastholeS = -1;
            for (int x = 1; x < numholes+ 1;x++)
            {
                m_southBeans[x]++;
                numbeans--;
                if (numbeans == 0)//there are no more beans from inital hole
                {
                    lastholeS = x;
                    endSide = SOUTH;
                    endHole = lastholeS;
                    break;
                }
            }
            if (endSide == SOUTH && endHole == lastholeS)
                break;
        }
    }
    return true;
}
//If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
bool Board::moveToPot(Side s, int hole, Side potOwner){
    if (hole <= 0 || hole > numholes)
        return false;
    if(s == NORTH && potOwner == NORTH){
        m_northBeans[0] += m_northBeans[hole];
        m_northBeans[hole] = 0;
    }
    else if(s == SOUTH && potOwner == NORTH){
        m_northBeans[0] += m_southBeans[hole];
        m_southBeans[hole] = 0;
    }
    else if(s == NORTH && potOwner == SOUTH){
        m_southBeans[0] += m_northBeans[hole];
        m_northBeans[hole] = 0;
    }
    else if(s == SOUTH && potOwner == SOUTH){
        m_southBeans[0] += m_southBeans[hole];
        m_southBeans[hole] = 0;
    }
    return true;
}
//If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
bool Board::setBeans(Side s, int hole, int beans){
    if (hole < 0 || hole > numholes || beans < 0)
        return false;
    
    if (s == NORTH)
    {
        m_northBeans[hole] = beans;
        return true;
    }
    else{
    m_southBeans[hole] = beans;
    return true;
    }
}
//If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This may change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
