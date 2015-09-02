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

#ifndef DensManager_H
#define DensManager_H

#include <iostream>
#include <cmath>
#include "MyTypes.h"

class DensManager {
    signed long halfSide;
    unsigned long crownStep;
    MyTypes::idVsCountAndNorm crCountVsNorm;
  public:
    DensManager(const signed long &, const unsigned long &);
    unsigned long crRecog(const MyTypes::coords &);
    unsigned long circleSites(const unsigned long &);
    void counterInit();
    void counterTimeScale(const unsigned long &);
    void counterUpdate(const MyTypes::coordsList &);
    void counterReset();
    friend std::ostream& operator<< (std::ostream &, 
                                     const DensManager &);

};

#endif
