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
	{ "" , "" , "" , "" , "" , "" , "" , "" , "" , "" }

};

//////////
// MAIN //
//////////

int main( int argc, char **argv) {

	// Constants
	const char * FAILURE_OUTPUT_FORMAT = "%-64s  %-64s\n";

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
			printf( FAILURE_OUTPUT_FORMAT, "Expected", "Obtained");
			printf( FAILURE_OUTPUT_FORMAT, "--------", "--------");
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pStreetNumber,		dl.getStreetNumber());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pPreDirectional,		dl.getPreDirectional());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pStreetName,			dl.getStreetName());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pStreetType,			dl.getStreetType());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pPostDirectional,		dl.getPostDirectional());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pUnitType,			dl.getUnitType());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pUnitNumber,			dl.getUnitNumber());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pPOBox,				dl.getPOBox());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pRuralRoute,			dl.getRuralRoute());
			printf( FAILURE_OUTPUT_FORMAT,testOutput->pRemainder,			dl.getRemainder());
			printf( FAILURE_OUTPUT_FORMAT, "--------", "--------");
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

