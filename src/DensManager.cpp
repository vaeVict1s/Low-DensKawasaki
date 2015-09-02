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

#include "DensManager.h"

using namespace std;
using namespace MyTypes;

DensManager::DensManager(const signed long &bound,
                         const unsigned long &step):
                         halfSide(bound), crownStep(step) 
                         { counterInit(); }

unsigned long DensManager::crRecog(const coords &position) {
  double a = sqrt( (position.first)*(position.first) + 
                   (position.second)*(position.second) );
  if(a == 0) {
    return 1;
  } else if(a > halfSide) {
    return halfSide / crownStep + 1;
  } else if(fmod(a, crownStep) != 0) {
    return int(a / crownStep) + 1;
  } else {
    return int(a / crownStep);
  }
}

unsigned long DensManager::circleSites(const unsigned long &r){
  unsigned long diagonal = (int) (r/sqrt(2));
  unsigned long internal = 0;
  unsigned long counter = 0;
  for (counter=1;counter<=diagonal;++counter) {
    internal += sqrt(r*r - counter*counter) - counter;
  }
  return 8*internal + 4*(diagonal + r) + 1;
}

void DensManager::counterInit() {
  unsigned long out = 0, in = 0, counter = 0;
  for(; counter<(halfSide/crownStep);++counter){
    out = circleSites((counter+1)*crownStep);
    crCountVsNorm.insert(make_pair(counter+1,
                         make_pair(0, out - in)));
    in = out;
  }
  crCountVsNorm.insert(make_pair(counter+1,
                       make_pair(0,
                       (2*halfSide+1)*(2*halfSide+1)
                       - out))
                      );
  (crCountVsNorm[1]).second -= 1;
}

void DensManager::counterTimeScale(const unsigned long &timeScale){
	idVsCountAndNorm::iterator it1;
	for(it1=crCountVsNorm.begin();it1!=crCountVsNorm.end();++it1) {
		((*it1).second).second *= timeScale;
	}
}

void DensManager::counterUpdate(const coordsList &walkersCoords){
	coordsList::const_iterator it1;
	unsigned long crown;
	for(it1=walkersCoords.begin(); it1!=walkersCoords.end(); ++it1){
		crown = crRecog(*it1);
		crCountVsNorm[crown].first += 1;
	}
}

void DensManager::counterReset(){
	idVsCountAndNorm::iterator it1;
	for(it1=crCountVsNorm.begin();it1!=crCountVsNorm.end();++it1){
		((*it1).second).first = 0;
	}
}

ostream &operator<<( ostream &output, const DensManager &toPrint){
	idVsCountAndNorm::const_iterator it1;
	for(it1=toPrint.crCountVsNorm.begin();
		it1!=--toPrint.crCountVsNorm.end();++it1){
		cout << (float)((*it1).second.first) / 
				(float)((*it1).second.second) << ",";	
	} cout << (float)((*it1).second.first) /
			  (float)((*it1).second.second);
	return output;
}
