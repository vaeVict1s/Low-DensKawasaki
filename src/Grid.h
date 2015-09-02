/*
   Copyright (C) 2015 Leonardo Pozzati

   Author: Leonardo Pozzati
   e-mail: leon.pozzati@gmail.com
       
   Special thanks to
   - Vincenzo Lo Cicero < e-mail: vincenzo.locicero@libero.it > for myoptParser
     ( files: myoptParser.h myoptLexer.h myoptLexer.c myoptSymtab.h myoptSymtab.c myoptParser.c )
   - Agner Fog < agner@agner.org > for Mersenne Twister 
     ( files: randomc.h mersenne.cpp )

   This file is part of Low-DensKawasaki.

   Low-DensKawasaki is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   myoptParserLib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Low-DensKawasaki.
   If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Grid_H
#define Grid_H

#include <iostream>
#include "MyTypes.h"
#include "randomc.h"

class Grid {
  signed long halfSide;
  MyTypes::Trap *gridTrap;
  MyTypes::partsCounter gridPart;
  unsigned seed;
  CRandomMersenne RandGen;
  MyTypes::coordsList gridWalkers;
  struct isAt;

  public:
    Grid(const signed long &bound,
         const MyTypes::coords &trapPos,
         const bool &initiallyTrapped,
         const double &glueStrength,
         const MyTypes::partsCounter &initiallyTrappedPart,
         const MyTypes::partsCounter &initiallyGridPart,
         const unsigned &GenSeed);
    void particlesDistribute();
    void trappedRemoval();
    void trapEmission();
    void oneStepMove();
    MyTypes::coordsList getWalkers();
    MyTypes::partsCounter howManyWalkers();
    MyTypes::partsCounter howManyTrapped();
    unsigned long howManyTrappings();
    unsigned long howManyEmissions();
    ~Grid();
    friend std::ostream& operator<< (std::ostream &, 
                                     const Grid &); 
};

#endif
