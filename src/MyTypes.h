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

#ifndef MyTypes_H
#define MyTypes_H

#include <utility>
#include <list>
#include <map>

namespace MyTypes{

  // types used both in Grid and DensManager classes
  typedef std::pair<signed long, signed long> coords;
  typedef std::list<coords> coordsList;
  typedef std::list<coords>::size_type partsCounter;

  // struct for the trap used in the Grid class
  struct Trap {
    coords position;
    bool initiallyPopulated;
    double stickiness;
    partsCounter totalTrapped;
    unsigned long trappingCounter, emissionCounter;
        
    Trap(coords trapPos, bool partAtInit, 
         double glue, partsCounter trapped): 
         position(trapPos), initiallyPopulated(partAtInit), 
         stickiness(glue), totalTrapped(trapped) {}
  };

  // types used in the DensManager class only
  typedef std::pair<unsigned long, unsigned long> countAndNorm;
  typedef std::map<unsigned long, countAndNorm> idVsCountAndNorm;
}

#endif
