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

#include "Grid.h"

using namespace std;
using namespace MyTypes;

Grid::Grid(const signed long &bound,
           const coords &trapPos,
           const bool &initiallyTrapped,
           const double &glueStrength,
           const partsCounter &initiallyTrappedPart,
           const partsCounter &initiallyGridPart,
           const unsigned &GenSeed):
           halfSide(bound),
           gridTrap(new Trap(trapPos, initiallyTrapped, 
                             glueStrength, initiallyTrappedPart)),
           gridPart(initiallyGridPart),
           seed(GenSeed),
           RandGen(seed)
           { particlesDistribute(); }

void Grid::particlesDistribute() {
  coords partPos;
  partsCounter partIndex;
  for(partIndex=1; partIndex<=gridPart;){
    partPos.first = RandGen.IRandomX(0, 2*halfSide) - halfSide;
    partPos.second = RandGen.IRandomX(0, 2*halfSide) - halfSide;
    if(gridTrap->initiallyPopulated) {
      gridWalkers.push_back(partPos);
      ++partIndex;
    } else {
      if(partPos.first || partPos.second) {
        gridWalkers.push_back(partPos);
        ++partIndex;
      }
      
    }
  }
}

struct Grid::isAt {
	public:
		isAt(coords trap = coords (0,0)): trapPos(trap) {}
		bool operator() (const coords &pos) {
			return pos == trapPos;
		}
	private:
		coords trapPos;
};

void Grid::trappedRemoval() {
  partsCounter initial = gridWalkers.size();
  gridWalkers.remove_if(isAt(gridTrap->position));
  gridTrap->totalTrapped += (initial - gridWalkers.size());
}

void Grid::trapEmission() {
	if(RandGen.Random()>gridTrap->stickiness) {
		--gridTrap->totalTrapped;
		++gridTrap->emissionCounter;
		int mossa = RandGen.IRandomX(0, 3);
		coords expelled(gridTrap->position);
		if(mossa == 0) {
			expelled.first += 1;
		} else if(mossa == 1) {
			expelled.second += 1;
		} else if(mossa == 2) {
			expelled.first -= 1;
		} else {
			expelled.second -= 1;
		}
		gridWalkers.push_back(expelled);
	}
}

void Grid::oneStepMove() {
  partsCounter previouslyTrapped = gridTrap->totalTrapped;
	coordsList::iterator it1;
	int mossa;
	for(it1=gridWalkers.begin();it1!=gridWalkers.end();/*inside*/) {
		mossa = RandGen.IRandom(0, 3);
		if(mossa == 0) {
			if((*it1).first == halfSide) {
				(*it1).first = -halfSide;
			} else ++((*it1).first);
		} else if(mossa == 1) {
			if((*it1).second == halfSide) {
				(*it1).second = -halfSide;
			} else ++((*it1).second);
		} else if(mossa == 2) {
			if((*it1).first == -halfSide) {
				(*it1).first = halfSide;
			} else --((*it1).first);
		} else {
			if((*it1).second == -halfSide) {
				(*it1).second = halfSide;
			} else --((*it1).second);

		}
		if((*it1) == gridTrap->position) {
			it1 = gridWalkers.erase(it1);
			++gridTrap->totalTrapped;
			++gridTrap->trappingCounter;
		} else {
			it1++;
		}
	}
  if(previouslyTrapped) {
    trapEmission();
  }
}

coordsList Grid::getWalkers() {
  return gridWalkers;
}
partsCounter Grid::howManyWalkers() {
  return gridWalkers.size();
}
partsCounter Grid::howManyTrapped() {
  return gridTrap->totalTrapped;
}
unsigned long Grid::howManyTrappings() {
  return gridTrap->trappingCounter;
}
unsigned long Grid::howManyEmissions() {
  return gridTrap->emissionCounter;
}


Grid::~Grid(){
  cout << "###Simulation completed###" << endl
       << "#Free particles: " << howManyWalkers() << endl
       << "#Trapped particles: " << howManyTrapped() << endl
       << "#Entrapment events: " << howManyTrappings() << endl
       << "#Emissions from trap: " << howManyEmissions() << endl
       << "#Mersenne Twister seed: " << seed << endl;
}

ostream &operator<<( ostream &output, const Grid &myGrid) {
	coordsList::const_iterator it1;
	for(it1=myGrid.gridWalkers.begin();
		it1!=myGrid.gridWalkers.end();
		++it1) {
		cout << "(" << (*it1).first << ","
			 << (*it1).second << ") ";
	} cout << endl;
	return output;
}
