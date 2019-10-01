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

#include "MyTypes.h"
#include "Grid.h"
#include "DensManager.h"
#include "randomc.h"
#include "myoptParser.h"

using namespace std;
using namespace MyTypes;

int main(int argc, char **argv) {
  signed long bound;
  unsigned long step;
  partsCounter gridPart, trappedPart;
  coords trapPos(0, 0);
  double glue;
  bool initiallyTrapped = false;
  unsigned seed;
  unsigned long duration, printStep;

  myopt_Parser_t parser;
  char strDescr[MAX_LEN_STR + 1];
  int32_t idGroup;
  int32_t idOption;
  bool bParse;

  parser = myopt_InitParser();
  if ( !parser )
    return -1;

  idGroup = myopt_AddGroup(parser, "Gruppo 0", false, false);
  strcpy(strDescr, "Print this help message and exit.");
  myopt_AddOption(parser, idGroup, 'h', "help", strDescr,
	          false, MOB_OVERRIDE, 0, 0, "");   

  strcpy(strDescr, "Mersenne Twister initialization.");
  myopt_AddOption(parser, idGroup, 's', "seed", strDescr,
	          false, MOB_OVERRIDE, 1, 1, "int");

  strcpy(strDescr, "Particles distributed on the grid at initialization.");   
  myopt_AddOption(parser, idGroup, 'd', "distribParticles", strDescr,
	          true, MOB_OVERRIDE, 1, 1, "int");

  strcpy(strDescr, "Particles trapped at initialization.");   
  myopt_AddOption(parser, idGroup, 't', "trappedParticles", strDescr,
	          false, MOB_OVERRIDE, 1, 1, "int");

  strcpy(strDescr, "Half side of the grid.");   
  myopt_AddOption(parser, idGroup, 'H', "halfSide", strDescr,
	          true, MOB_OVERRIDE, 1, 1, "int");
   
  strcpy(strDescr, "Difference between outer and inner radius of annuli which cover the grid.");   
  myopt_AddOption(parser, idGroup, 'a', "annulusStep", strDescr,
	          true, MOB_OVERRIDE, 1, 1, "int");

  strcpy(strDescr, "Total iterations of the simulation.");   
  myopt_AddOption(parser, idGroup, 'D', "duration", strDescr,
	          true, MOB_OVERRIDE, 1, 1, "int");

  strcpy(strDescr, "Iterations between two consecutive measurements.");   
  myopt_AddOption(parser, idGroup, 'p', "printStep", strDescr,
	          true, MOB_OVERRIDE, 1, 1, "int");

  strcpy(strDescr, "Stickiness of the trap.");   
  myopt_AddOption(parser, idGroup, 'g', "glue", strDescr,
	          true, MOB_OVERRIDE, 1, 1, "float");

  strcpy(strDescr, "Flag to consider the trap for particles distribution at initialization.");   
  myopt_AddOption(parser, idGroup, 'i', "initTrap", strDescr,
	          false, MOB_OVERRIDE, 0, 0, "");

  myopt_SetPositionalArgsParams(parser, "POS_ARGS", 0, 0, "");

  bParse = myopt_ParseArray(parser, argc, argv);

  if(parser->countInternalErrors > 0 ) {
    cout << "Internal errors:" << endl
         << parser->strInternalErrors;
    return -1;
  }      

  idOption = myopt_LookupLong(parser, "help");
  if (idOption >= 0 &&
      parser->arrayOptArgs[idOption].countOccurrences > 0 ) {
    cout << parser->strUsage;
    myopt_FreeParser(parser);
    return 0;
  }   

  if ( !bParse ) {
    cout << parser->strUsage
         << parser->strErrors;
    myopt_FreeParser(parser);
    return -1;
  }

  seed = (unsigned) time(NULL);
  idOption = myopt_LookupLong(parser, "seed");
  if (idOption >= 0 && 
      parser->arrayOptArgs[idOption].countOccurrences > 0 )
    if (parser->arrayOptArgs[idOption].countArgs > 0 ) {
      seed = (unsigned) parser->arrayOptArgs[idOption].arrayArgs[0].intValue;
    }

  idOption = myopt_LookupLong(parser, "distribParticles");	
  gridPart = (partsCounter)parser->arrayOptArgs[idOption].arrayArgs[0].intValue;

  idOption = myopt_LookupLong(parser, "halfSide");	
  bound = (signed long)parser->arrayOptArgs[idOption].arrayArgs[0].intValue;

  idOption = myopt_LookupLong(parser, "annulusStep");	
  step = (unsigned long)parser->arrayOptArgs[idOption].arrayArgs[0].intValue;

  idOption = myopt_LookupLong(parser, "duration");	
  duration = (unsigned long)parser->arrayOptArgs[idOption].arrayArgs[0].intValue;

  idOption = myopt_LookupLong(parser, "printStep");	
  printStep = (unsigned long)parser->arrayOptArgs[idOption].arrayArgs[0].intValue;

  idOption = myopt_LookupLong(parser, "glue");	
  glue = (double)parser->arrayOptArgs[idOption].arrayArgs[0].floatValue;

  idOption = myopt_LookupLong(parser, "initTrap");
  if (idOption >= 0 && 
      parser->arrayOptArgs[idOption].countOccurrences > 0)
    initiallyTrapped = true;

  idOption = myopt_LookupLong(parser, "trappedParticles");
  if (idOption >= 0 && 
      parser->arrayOptArgs[idOption].countOccurrences > 0)
    trappedPart = (partsCounter)parser->arrayOptArgs[idOption].arrayArgs[0].intValue;


  Grid MyGrid(bound, trapPos, initiallyTrapped, glue,
              trappedPart, gridPart, seed);
  DensManager DensHandle(bound, step);
  if(initiallyTrapped) {
    MyGrid.trappedRemoval();
  }
  DensHandle.counterUpdate(MyGrid.getWalkers());
  cout << DensHandle << "," 
       << MyGrid.howManyTrapped() << ",0" << endl;
  DensHandle.counterReset();
  DensHandle.counterTimeScale(printStep);
  unsigned long time, cicle;
  for(cicle=1;cicle<=duration/printStep;cicle++){
    time = (cicle - 1) * printStep + 1;
    for(; time<=cicle * printStep; time++) {
      MyGrid.oneStepMove();
      DensHandle.counterUpdate(MyGrid.getWalkers());
    }
    cout << DensHandle << ","
         << MyGrid.howManyTrapped() << ","
         << time - 1 << endl;
    DensHandle.counterReset();
  }
  return 0;
}
