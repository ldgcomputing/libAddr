//
//  UnitTests.cpp
//  libAddr
//
//  This program simply runs a series of unit tests against
//  various inputs and ensures the proper results are
//  obtained.
//
//  Created by Louis Gehrig on 9/15/19.
//  Copyright © 2019 Louis Gehrig. All rights reserved.
//

/***

	MIT License

	Copyright (c) 2019 Louis Gehrig

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

***/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>

// Project includes
#include <libAddr.hpp>

// The structure of the known results
struct s_known_output {

	const char *pStreetNumber;
	const char *pPreDirectional;
	const char *pStreetName;
	const char *pStreetType;
	const char *pPostDirectional;
	const char *pUnitType;
	const char *pUnitNumber;
	const char *pPOBox;
	const char *pRuralRoute;
	const char *pRemainder;

};
typedef struct s_known_output S_KNOWN_OUTPUT;

// Known inputs
const char * TEST_ADDR [] = {
	"5397 Apt 16-18 Cedar Lake Road",
	"5397 # 16-18 Cedar Lake Road",
	"5397 #16-18 Cedar Lake Road",
	"13298 Citrus Grove Blvd",
	"5600 Broken Sound Blvd NW",
	"5397 Cedar Lake Road Apt 1618",
	"5397 Cedar Lake Road # 1618",
	"5397 Cedar Lake Road #1618",
	"5397 Cedar Lake Road #",
	"5397 Cedar Lake Road Apt",
	"123 E Rd",
	"123 STATE HWY 715",
	"Rural Route 2 Box 123",
	"Rural Rte 5 # 332",
	"Rural Rte 5 #332",
	"Rural Rte 5#332",
	0x0
};

// Known outputs
const S_KNOWN_OUTPUT TEST_OUTPUTS [] = {

	{ "5397", "", "CEDAR LAKE", "RD", "", "APT", "1618", "", "", "" },
	{ "5397", "", "CEDAR LAKE", "RD", "", "UNIT", "1618", "", "", "" },
	{ "5397", "", "CEDAR LAKE", "RD", "", "UNIT", "1618", "", "", "" },
	{ "13298", "", "CITRUS GROVE", "BLVD", "", "", "", "", "", "" },
	{ "5600", "", "BROKEN SOUND", "BLVD", "NW", "", "", "", "", "" },
	{ "5397", "", "CEDAR LAKE", "RD", "", "APT", "1618", "", "", "" },
	{ "5397", "", "CEDAR LAKE", "RD", "", "UNIT", "1618", "", "", "" },
	{ "5397", "", "CEDAR LAKE", "RD", "", "UNIT", "1618", "", "", "" },
	{ "5397", "", "CEDAR LAKE", "RD", "", "UNIT", "", "", "", "" },
	{ "5397", "", "CEDAR LAKE", "RD", "", "APT", "", "", "", "" },
	{ "123", "", "E", "RD", "", "", "", "", "", "" },
	{ "123", "", "STATE HWY 715", "", "", "", "", "", "", "" },
	{ "" , "" , "" , "" , "" , "" , "" , "" , "RURAL ROUTE 2 BOX 123" , "" },
	{ "" , "" , "" , "" , "" , "" , "" , "" , "RURAL ROUTE 5 BOX 332" , "" },
	{ "" , "" , "" , "" , "" , "" , "" , "" , "RURAL ROUTE 5 BOX 332" , "" },
	{ "" , "" , "" , "" , "" , "" , "" , "" , "RURAL ROUTE 5 BOX 332" , "" },
	{ "" , "" , "" , "" , "" , "" , "" , "" , "" , "" }

};

//////////
// MAIN //
//////////

int main( int argc, char **argv) {

	// Constants
	const char * FAILURE_OUTPUT_FORMAT = "%-15s %-64s  %-64s\n";

	// Variables
	bool bAllPassed = true;
	int nFailed = 0;
	int nPassed = 0;
	int nPos = 0;

	// Loop over all of the input until the empty string is returned
	for( ; (const char *) 0x0 != TEST_ADDR [nPos] ; ++ nPos) {

		// Get values
		const char *testInput = TEST_ADDR [nPos];
		const S_KNOWN_OUTPUT *testOutput = TEST_OUTPUTS + nPos;

		// Execute
		libAddr::deliveryLine dl( testInput);

		// Validate
		bool bThisPassed = true;
		bThisPassed &= (0x0 == strcmp( testOutput->pStreetNumber,		dl.getStreetNumber()));
		bThisPassed &= (0x0 == strcmp( testOutput->pPreDirectional,		dl.getPreDirectional()));
		bThisPassed &= (0x0 == strcmp( testOutput->pStreetName,			dl.getStreetName()));
		bThisPassed &= (0x0 == strcmp( testOutput->pStreetType,			dl.getStreetType()));
		bThisPassed &= (0x0 == strcmp( testOutput->pPostDirectional,	dl.getPostDirectional()));
		bThisPassed &= (0x0 == strcmp( testOutput->pUnitType,			dl.getUnitType()));
		bThisPassed &= (0x0 == strcmp( testOutput->pUnitNumber,			dl.getUnitNumber()));
		bThisPassed &= (0x0 == strcmp( testOutput->pPOBox,				dl.getPOBox()));
		bThisPassed &= (0x0 == strcmp( testOutput->pRuralRoute,			dl.getRuralRoute()));
		bThisPassed &= (0x0 == strcmp( testOutput->pRemainder,			dl.getRemainder()));
		bAllPassed &= bThisPassed;
		if(bThisPassed) {
			++ nPassed;
		}
		else {
			++ nFailed;
			printf( "FAILURE for input ===== %s =====\n", testInput);
			printf( FAILURE_OUTPUT_FORMAT, "Field", "Expected", "Obtained");
			printf( FAILURE_OUTPUT_FORMAT, "-----", "--------", "--------");
			printf( FAILURE_OUTPUT_FORMAT, "Street Number",	testOutput->pStreetNumber,		dl.getStreetNumber());
			printf( FAILURE_OUTPUT_FORMAT, "Pre Dir",		testOutput->pPreDirectional,		dl.getPreDirectional());
			printf( FAILURE_OUTPUT_FORMAT, "Street Name",	testOutput->pStreetName,			dl.getStreetName());
			printf( FAILURE_OUTPUT_FORMAT, "Street Type",	testOutput->pStreetType,			dl.getStreetType());
			printf( FAILURE_OUTPUT_FORMAT, "Post Dir",		testOutput->pPostDirectional,		dl.getPostDirectional());
			printf( FAILURE_OUTPUT_FORMAT, "Unit Type",		testOutput->pUnitType,			dl.getUnitType());
			printf( FAILURE_OUTPUT_FORMAT, "Unit Num",		testOutput->pUnitNumber,			dl.getUnitNumber());
			printf( FAILURE_OUTPUT_FORMAT, "PO Bpx",		testOutput->pPOBox,				dl.getPOBox());
			printf( FAILURE_OUTPUT_FORMAT, "Rural Route",	testOutput->pRuralRoute,			dl.getRuralRoute());
			printf( FAILURE_OUTPUT_FORMAT, "Remainder",		testOutput->pRemainder,			dl.getRemainder());
			printf( FAILURE_OUTPUT_FORMAT, "-----", "--------", "--------");
		}

	}

	// If everything passed ...
	if( bAllPassed)
		printf( "All %d unit tests passed\n", nPassed);
	else
		printf( "Failure count: %d -- Pass count: %d\n", nFailed, nPassed);

	// And return the result
	return( bAllPassed ? EXIT_SUCCESS : EXIT_FAILURE);

}

