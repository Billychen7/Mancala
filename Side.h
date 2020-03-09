//
//  Side.h
//  Project 3
//
//  Created by Billy Chen on 5/18/19.
//  Copyright © 2019 William Chen. All rights reserved.
//

#ifndef Side_h
#define Side_h

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif /* Side_h */
