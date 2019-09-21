//
//  libAddr.cpp
//  DecodeShapeLib
//
//  Created by Louis Gehrig on 5/11/19.
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

// Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <math.h>

// STL includes
#include <iterator>
#include <vector>

// Project includes
#include <libAddr.hpp>

namespace libAddr {

	// Compare conversion types
	int compareConversionType( const void *left, const void *right) {
		S_CONVERSION_TYPE *ctLeft = (S_CONVERSION_TYPE *) left;
		S_CONVERSION_TYPE *ctRight = (S_CONVERSION_TYPE *) right;
		return( strcmp( ctLeft->type, ctRight->type));
	}

	// Construct an address compression
	const char * addressCompression::KNOWN_DIRECTIONALS [] = { "E" , "N" , "S" , "W" , "NE" , "NW" , "SE" , "SW" , 0x0 };
	const char * addressCompression::KNOWN_PO_BOX_HEADERS [] = { "POBOX " , "PO BOX " , "PO " , 0x0 };
	const char * addressCompression::KNOWN_RURAL_ROUTE_HEADERS [] = { "RURAL ROUTE ", "RURAL RTE ", "RR ", 0x0 };
	S_CONVERSION_TYPE addressCompression::KNOWN_STREET_TYPES [] = {
		{ "ALLEE" , "ALY" },
		{ "ALLEY" , "ALY" },
		{ "ALLY" , "ALY" },
		{ "ALY" , "ALY" },
		{ "ANEX" , "ANX" },
		{ "ANNEX" , "ANX" },
		{ "ANNX" , "ANX" },
		{ "ANX" , "ANX" },
		{ "ARCADE" , "ARC" },
		{ "ARC" , "ARC" },
		{ "AVENUE" , "AVE" },
		{ "AVENU" , "AVE" },
		{ "AVEN" , "AVE" },
		{ "AVE" , "AVE" },
		{ "AVNUE" , "AVE" },
		{ "AVN" , "AVE" },
		{ "AV" , "AVE" },
		{ "BAYOO" , "BYU" },
		{ "BAYOU" , "BYU" },
		{ "BCH" , "BCH" },
		{ "BEACH" , "BCH" },
		{ "BEND" , "BND" },
		{ "BGS" , "BGS" },
		{ "BG" , "BG" },
		{ "BLFS" , "BLFS" },
		{ "BLF" , "BLF" },
		{ "BLUFFS" , "BLFS" },
		{ "BLUFF" , "BLF" },
		{ "BLUF" , "BLF" },
		{ "BLVD" , "BLVD" },
		{ "BND" , "BND" },
		{ "BOTTM" , "BTM" },
		{ "BOTTOM" , "BTM" },
		{ "BOT" , "BTM" },
		{ "BOULEVARD" , "BLVD" },
		{ "BOULV" , "BLVD" },
		{ "BOUL" , "BLVD" },
		{ "BRANCH" , "BR" },
		{ "BRDGE" , "BRG" },
		{ "BRG" , "BRG" },
		{ "BRIDGE" , "BRG" },
		{ "BRKS" , "BRKS" },
		{ "BRK" , "BRK" },
		{ "BRNCH" , "BR" },
		{ "BROOKS" , "BRKS" },
		{ "BROOK" , "BRK" },
		{ "BR" , "BR" },
		{ "BTM" , "BTM" },
		{ "BURGS" , "BGS" },
		{ "BURG" , "BG" },
		{ "BYPASS" , "BYP" },
		{ "BYPAS" , "BYP" },
		{ "BYPA" , "BYP" },
		{ "BYPS" , "BYP" },
		{ "BYP" , "BYP" },
		{ "BYU" , "BYU" },
		{ "CAMP" , "CP" },
		{ "CANYN" , "CYN" },
		{ "CANYON" , "CYN" },
		{ "CAPE" , "CPE" },
		{ "CAUSEWAY" , "CSWY" },
		{ "CAUSWA" , "CSWY" },
		{ "CENTERS" , "CTRS" },
		{ "CENTER" , "CTR" },
		{ "CENTRE" , "CTR" },
		{ "CENTR" , "CTR" },
		{ "CENT" , "CTR" },
		{ "CEN" , "CTR" },
		{ "CIRCLES" , "CIRS" },
		{ "CIRCLE" , "CIR" },
		{ "CIRCL" , "CIR" },
		{ "CIRC" , "CIR" },
		{ "CIRS" , "CIRS" },
		{ "CIR" , "CIR" },
		{ "CLB" , "CLB" },
		{ "CLFS" , "CLFS" },
		{ "CLF" , "CLF" },
		{ "CLIFFS" , "CLFS" },
		{ "CLIFF" , "CLF" },
		{ "CLUB" , "CLB" },
		{ "CMNS" , "CMNS" },
		{ "CMN" , "CMN" },
		{ "CMP" , "CP" },
		{ "CNTER" , "CTR" },
		{ "CNTR" , "CTR" },
		{ "CNYN" , "CYN" },
		{ "COMMONS" , "CMNS" },
		{ "COMMON" , "CMN" },
		{ "CORNERS" , "CORS" },
		{ "CORNER" , "COR" },
		{ "CORS" , "CORS" },
		{ "COR" , "COR" },
		{ "COURSE" , "CRSE" },
		{ "COURTS" , "CTS" },
		{ "COURT" , "CT" },
		{ "COVES" , "CVS" },
		{ "COVE" , "CV" },
		{ "CPE" , "CPE" },
		{ "CP" , "CP" },
		{ "CRCLE" , "CIR" },
		{ "CRCL" , "CIR" },
		{ "CREEK" , "CRK" },
		{ "CRESCENT" , "CRES" },
		{ "CREST" , "CRST" },
		{ "CRES" , "CRES" },
		{ "CRK" , "CRK" },
		{ "CROSSING" , "XING" },
		{ "CROSSROADS" , "XRDS" },
		{ "CROSSROAD" , "XRD" },
		{ "CRSENT" , "CRES" },
		{ "CRSE" , "CRSE" },
		{ "CRSNT" , "CRES" },
		{ "CRSSNG" , "XING" },
		{ "CRST" , "CRST" },
		{ "CSWY" , "CSWY" },
		{ "CTRS" , "CTRS" },
		{ "CTR" , "CTR" },
		{ "CTS" , "CTS" },
		{ "CT" , "CT" },
		{ "CURVE" , "CURV" },
		{ "CURV" , "CURV" },
		{ "CVS" , "CVS" },
		{ "CV" , "CV" },
		{ "CYN" , "CYN" },
		{ "DALE" , "DL" },
		{ "DAM" , "DM" },
		{ "DIVIDE" , "DV" },
		{ "DIV" , "DV" },
		{ "DL" , "DL" },
		{ "DM" , "DM" },
		{ "DRIVES" , "DRS" },
		{ "DRIVE" , "DR" },
		{ "DRIV" , "DR" },
		{ "DRS" , "DRS" },
		{ "DRV" , "DR" },
		{ "DR" , "DR" },
		{ "DVD" , "DV" },
		{ "DV" , "DV" },
		{ "ESTATES" , "ESTS" },
		{ "ESTATE" , "EST" },
		{ "ESTS" , "ESTS" },
		{ "EST" , "EST" },
		{ "EXPRESSWAY" , "EXPY" },
		{ "EXPRESS" , "EXPY" },
		{ "EXPR" , "EXPY" },
		{ "EXPW" , "EXPY" },
		{ "EXPY" , "EXPY" },
		{ "EXP" , "EXPY" },
		{ "EXTENSIONS" , "EXTS" },
		{ "EXTENSION" , "EXT" },
		{ "EXTNSN" , "EXT" },
		{ "EXTN" , "EXT" },
		{ "EXTS" , "EXTS" },
		{ "EXT" , "EXT" },
		{ "FALLS" , "FLS" },
		{ "FALL" , "FALL" },
		{ "FERRY" , "FRY" },
		{ "FIELDS" , "FLDS" },
		{ "FIELD" , "FLD" },
		{ "FLATS" , "FLTS" },
		{ "FLAT" , "FLT" },
		{ "FLDS" , "FLDS" },
		{ "FLD" , "FLD" },
		{ "FLS" , "FLS" },
		{ "FLTS" , "FLTS" },
		{ "FLT" , "FLT" },
		{ "FORDS" , "FRDS" },
		{ "FORD" , "FRD" },
		{ "FORESTS" , "FRST" },
		{ "FOREST" , "FRST" },
		{ "FORGES" , "FRGS" },
		{ "FORGE" , "FRG" },
		{ "FORG" , "FRG" },
		{ "FORKS" , "FRKS" },
		{ "FORK" , "FRK" },
		{ "FORT" , "FT" },
		{ "FRDS" , "FRDS" },
		{ "FRD" , "FRD" },
		{ "FREEWAY" , "FWY" },
		{ "FREEWY" , "FWY" },
		{ "FRGS" , "FRGS" },
		{ "FRG" , "FRG" },
		{ "FRKS" , "FRKS" },
		{ "FRK" , "FRK" },
		{ "FRRY" , "FRY" },
		{ "FRST" , "FRST" },
		{ "FRT" , "FT" },
		{ "FRWAY" , "FWY" },
		{ "FRWY" , "FWY" },
		{ "FRY" , "FRY" },
		{ "FT" , "FT" },
		{ "FWY" , "FWY" },
		{ "GARDENS" , "GDNS" },
		{ "GARDEN" , "GDN" },
		{ "GARDN" , "GDN" },
		{ "GATEWAY" , "GTWY" },
		{ "GATEWY" , "GTWY" },
		{ "GATWAY" , "GTWY" },
		{ "GDNS" , "GDNS" },
		{ "GDN" , "GDN" },
		{ "GLENS" , "GLNS" },
		{ "GLEN" , "GLN" },
		{ "GLNS" , "GLNS" },
		{ "GLN" , "GLN" },
		{ "GRDEN" , "GDN" },
		{ "GRDNS" , "GDNS" },
		{ "GRDN" , "GDN" },
		{ "GREENS" , "GRNS" },
		{ "GREEN" , "GRN" },
		{ "GRNS" , "GRNS" },
		{ "GRN" , "GRN" },
		{ "GROVES" , "GRVS" },
		{ "GROVE" , "GRV" },
		{ "GROV" , "GRV" },
		{ "GRVS" , "GRVS" },
		{ "GRV" , "GRV" },
		{ "GTWAY" , "GTWY" },
		{ "GTWY" , "GTWY" },
		{ "HARBORS" , "HBRS" },
		{ "HARBOR" , "HBR" },
		{ "HARBR" , "HBR" },
		{ "HARB" , "HBR" },
		{ "HAVEN" , "HVN" },
		{ "HBRS" , "HBRS" },
		{ "HBR" , "HBR" },
		{ "HEIGHTS" , "HTS" },
		{ "HIGHWAY" , "HWY" },
		{ "HIGHWY" , "HWY" },
		{ "HILLS" , "HLS" },
		{ "HILL" , "HL" },
		{ "HIWAY" , "HWY" },
		{ "HIWY" , "HWY" },
		{ "HLLW" , "HOLW" },
		{ "HLS" , "HLS" },
		{ "HL" , "HL" },
		{ "HOLLOWS" , "HOLW" },
		{ "HOLLOW" , "HOLW" },
		{ "HOLWS" , "HOLW" },
		{ "HOLW" , "HOLW" },
		{ "HRBOR" , "HBR" },
		{ "HTS" , "HTS" },
		{ "HT" , "HTS" },
		{ "HVN" , "HVN" },
		{ "HWAY" , "HWY" },
		{ "HWY" , "HWY" },
		{ "INLET" , "INLT" },
		{ "INLT" , "INLT" },
		{ "ISLANDS" , "ISS" },
		{ "ISLAND" , "IS" },
		{ "ISLES" , "ISLE" },
		{ "ISLE" , "ISLE" },
		{ "ISLNDS" , "ISS" },
		{ "ISLND" , "IS" },
		{ "ISS" , "ISS" },
		{ "IS" , "IS" },
		{ "JCTION" , "JCT" },
		{ "JCTNS" , "JCTS" },
		{ "JCTN" , "JCT" },
		{ "JCTS" , "JCTS" },
		{ "JCT" , "JCT" },
		{ "JUNCTIONS" , "JCTS" },
		{ "JUNCTION" , "JCT" },
		{ "JUNCTN" , "JCT" },
		{ "JUNCTON" , "JCT" },
		{ "KEYS" , "KYS" },
		{ "KEY" , "KY" },
		{ "KNLS" , "KNLS" },
		{ "KNL" , "KNL" },
		{ "KNOLLS" , "KNLS" },
		{ "KNOLL" , "KNL" },
		{ "KNOL" , "KNL" },
		{ "KYS" , "KYS" },
		{ "KY" , "KY" },
		{ "LAKES" , "LKS" },
		{ "LAKE" , "LK" },
		{ "LANDING" , "LNDG" },
		{ "LAND" , "LAND" },
		{ "LANE" , "LN" },
		{ "LCKS" , "LCKS" },
		{ "LCK" , "LCK" },
		{ "LDGE" , "LDG" },
		{ "LDG" , "LDG" },
		{ "LF" , "LF" },
		{ "LGTS" , "LGTS" },
		{ "LGT" , "LGT" },
		{ "LIGHTS" , "LGTS" },
		{ "LIGHT" , "LGT" },
		{ "LKS" , "LKS" },
		{ "LK" , "LK" },
		{ "LNDG" , "LNDG" },
		{ "LNDNG" , "LNDG" },
		{ "LN" , "LN" },
		{ "LOAF" , "LF" },
		{ "LOCKS" , "LCKS" },
		{ "LOCK" , "LCK" },
		{ "LODGE" , "LDG" },
		{ "LODG" , "LDG" },
		{ "LOOPS" , "LOOP" },
		{ "LOOP" , "LOOP" },
		{ "MALL" , "MALL" },
		{ "MANORS" , "MNRS" },
		{ "MANOR" , "MNR" },
		{ "MDWS" , "MDWS" },
		{ "MDW" , "MDWS" },
		{ "MDW" , "MDW" },
		{ "MEADOWS" , "MDWS" },
		{ "MEADOW" , "MDW" },
		{ "MEDOWS" , "MDWS" },
		{ "MEWS" , "MEWS" },
		{ "MILLS" , "MLS" },
		{ "MILL" , "ML" },
		{ "MISSION" , "MSN" },
		{ "MISSN" , "MSN" },
		{ "MLS" , "MLS" },
		{ "ML" , "ML" },
		{ "MNRS" , "MNRS" },
		{ "MNR" , "MNR" },
		{ "MNTAIN" , "MTN" },
		{ "MNTNS" , "MTNS" },
		{ "MNTN" , "MTN" },
		{ "MNT" , "MT" },
		{ "MOTORWAY" , "MTWY" },
		{ "MOUNTAINS" , "MTNS" },
		{ "MOUNTAIN" , "MTN" },
		{ "MOUNTIN" , "MTN" },
		{ "MOUNT" , "MT" },
		{ "MSN" , "MSN" },
		{ "MSSN" , "MSN" },
		{ "MTIN" , "MTN" },
		{ "MTNS" , "MTNS" },
		{ "MTN" , "MTN" },
		{ "MTWY" , "MTWY" },
		{ "MT" , "MT" },
		{ "NCK" , "NCK" },
		{ "NECK" , "NCK" },
		{ "OPAS" , "OPAS" },
		{ "ORCHARD" , "ORCH" },
		{ "ORCHRD" , "ORCH" },
		{ "ORCH" , "ORCH" },
		{ "OVAL" , "OVAL" },
		{ "OVERPASS" , "OPAS" },
		{ "OVL" , "OVAL" },
		{ "PARKS" , "PARK" },
		{ "PARKWAYS" , "PKWY" },
		{ "PARKWAY" , "PKWY" },
		{ "PARKWY" , "PKWY" },
		{ "PARK" , "PARK" },
		{ "PASSAGE" , "PSGE" },
		{ "PASS" , "PASS" },
		{ "PATHS" , "PATH" },
		{ "PATH" , "PATH" },
		{ "PIKES" , "PIKE" },
		{ "PIKE" , "PIKE" },
		{ "PINES" , "PNES" },
		{ "PINE" , "PNE" },
		{ "PKWAY" , "PKWY" },
		{ "PKWYS" , "PKWY" },
		{ "PKWY" , "PKWY" },
		{ "PKY" , "PKWY" },
		{ "PLACE" , "PL" },
		{ "PLAINS" , "PLNS" },
		{ "PLAIN" , "PLN" },
		{ "PLAZA" , "PLZ" },
		{ "PLNS" , "PLNS" },
		{ "PLN" , "PLN" },
		{ "PLZA" , "PLZ" },
		{ "PLZ" , "PLZ" },
		{ "PL" , "PL" },
		{ "PNES" , "PNES" },
		{ "PNE" , "PNE" },
		{ "POINTS" , "PTS" },
		{ "POINT" , "PT" },
		{ "PORTS" , "PRTS" },
		{ "PORT" , "PRT" },
		{ "PRAIRIE" , "PR" },
		{ "PRK" , "PARK" },
		{ "PRR" , "PR" },
		{ "PRTS" , "PRTS" },
		{ "PRT" , "PRT" },
		{ "PR" , "PR" },
		{ "PSGE" , "PSGE" },
		{ "PTS" , "PTS" },
		{ "PT" , "PT" },
		{ "RADIAL" , "RADL" },
		{ "RADIEL" , "RADL" },
		{ "RADL" , "RADL" },
		{ "RAD" , "RADL" },
		{ "RAMP" , "RAMP" },
		{ "RANCHES" , "RNCH" },
		{ "RANCH" , "RNCH" },
		{ "RAPIDS" , "RPDS" },
		{ "RAPID" , "RPD" },
		{ "RDGE" , "RDG" },
		{ "RDGS" , "RDGS" },
		{ "RDG" , "RDG" },
		{ "RDS" , "RDS" },
		{ "RD" , "RD" },
		{ "REST" , "RST" },
		{ "RIDGES" , "RDGS" },
		{ "RIDGE" , "RDG" },
		{ "RIVER" , "RIV" },
		{ "RIVR" , "RIV" },
		{ "RIV" , "RIV" },
		{ "RNCHS" , "RNCH" },
		{ "RNCH" , "RNCH" },
		{ "ROADS" , "RDS" },
		{ "ROAD" , "RD" },
		{ "ROUTE" , "RTE" },
		{ "ROW" , "ROW" },
		{ "RPDS" , "RPDS" },
		{ "RPD" , "RPD" },
		{ "RST" , "RST" },
		{ "RTE" , "RTE" },
		{ "RUE" , "RUE" },
		{ "RUN" , "RUN" },
		{ "RVR" , "RIV" },
		{ "SHLS" , "SHLS" },
		{ "SHL" , "SHL" },
		{ "SHOALS" , "SHLS" },
		{ "SHOAL" , "SHL" },
		{ "SHOARS" , "SHRS" },
		{ "SHOAR" , "SHR" },
		{ "SHORES" , "SHRS" },
		{ "SHORE" , "SHR" },
		{ "SHRS" , "SHRS" },
		{ "SHR" , "SHR" },
		{ "SKWY" , "SKWY" },
		{ "SKYWAY" , "SKWY" },
		{ "SMT" , "SMT" },
		{ "SPGS" , "SPGS" },
		{ "SPG" , "SPG" },
		{ "SPNGS" , "SPGS" },
		{ "SPNG" , "SPG" },
		{ "SPRINGS" , "SPGS" },
		{ "SPRING" , "SPG" },
		{ "SPRNGS" , "SPGS" },
		{ "SPRNG" , "SPG" },
		{ "SPURS" , "SPUR" },
		{ "SPUR" , "SPUR" },
		{ "SQRE" , "SQ" },
		{ "SQRS" , "SQS" },
		{ "SQR" , "SQ" },
		{ "SQS" , "SQS" },
		{ "SQUARES" , "SQS" },
		{ "SQUARE" , "SQ" },
		{ "SQU" , "SQ" },
		{ "SQ" , "SQ" },
		{ "STATION" , "STA" },
		{ "STATN" , "STA" },
		{ "STA" , "STA" },
		{ "STN" , "STA" },
		{ "STRAVENUE" , "STRA" },
		{ "STRAVEN" , "STRA" },
		{ "STRAVN" , "STRA" },
		{ "STRAV" , "STRA" },
		{ "STRA" , "STRA" },
		{ "STREAM" , "STRM" },
		{ "STREETS" , "STS" },
		{ "STREET" , "ST" },
		{ "STREME" , "STRM" },
		{ "STRM" , "STRM" },
		{ "STRT" , "ST" },
		{ "STRVNUE" , "STRA" },
		{ "STRVN" , "STRA" },
		{ "STR" , "ST" },
		{ "STS" , "STS" },
		{ "ST" , "ST" },
		{ "SUMITT" , "SMT" },
		{ "SUMIT" , "SMT" },
		{ "SUMMIT" , "SMT" },
		{ "TERRACE" , "TER" },
		{ "TERR" , "TER" },
		{ "TER" , "TER" },
		{ "THROUGHWAY" , "TRWY" },
		{ "TPKE" , "TPKE" },
		{ "TRACES" , "TRCE" },
		{ "TRACE" , "TRCE" },
		{ "TRACKS" , "TRAK" },
		{ "TRACK" , "TRAK" },
		{ "TRAFFICWAY" , "TRFY" },
		{ "TRAILER" , "TRLR" },
		{ "TRAILS" , "TRL" },
		{ "TRAIL" , "TRL" },
		{ "TRAK" , "TRAK" },
		{ "TRCE" , "TRCE" },
		{ "TRFY" , "TRFY" },
		{ "TRKS" , "TRAK" },
		{ "TRK" , "TRAK" },
		{ "TRLRS" , "TRLR" },
		{ "TRLR" , "TRLR" },
		{ "TRLS" , "TRL" },
		{ "TRL" , "TRL" },
		{ "TRNPK" , "TPKE" },
		{ "TRWY" , "TRWY" },
		{ "TUNEL" , "TUNL" },
		{ "TUNLS" , "TUNL" },
		{ "TUNL" , "TUNL" },
		{ "TUNNELS" , "TUNL" },
		{ "TUNNEL" , "TUNL" },
		{ "TUNNL" , "TUNL" },
		{ "TURNPIKE" , "TPKE" },
		{ "TURNPK" , "TPKE" },
		{ "UNDERPASS" , "UPAS" },
		{ "UNIONS" , "UNS" },
		{ "UNION" , "UN" },
		{ "UNS" , "UNS" },
		{ "UN" , "UN" },
		{ "UPAS" , "UPAS" },
		{ "VALLEYS" , "VLYS" },
		{ "VALLEY" , "VLY" },
		{ "VALLY" , "VLY" },
		{ "VDCT" , "VIA" },
		{ "VIADCT" , "VIA" },
		{ "VIADUCT" , "VIA" },
		{ "VIA" , "VIA" },
		{ "VIEWS" , "VWS" },
		{ "VIEW" , "VW" },
		{ "VILLAGES" , "VLGS" },
		{ "VILLAGE" , "VLG" },
		{ "VILLAG" , "VLG" },
		{ "VILLE" , "VL" },
		{ "VILLG" , "VLG" },
		{ "VILLIAGE" , "VLG" },
		{ "VILL" , "VLG" },
		{ "VISTA" , "VIS" },
		{ "VIST" , "VIS" },
		{ "VIS" , "VIS" },
		{ "VLGS" , "VLGS" },
		{ "VLG" , "VLG" },
		{ "VLLY" , "VLY" },
		{ "VLYS" , "VLYS" },
		{ "VLY" , "VLY" },
		{ "VL" , "VL" },
		{ "VSTA" , "VIS" },
		{ "VST" , "VIS" },
		{ "VWS" , "VWS" },
		{ "VW" , "VW" },
		{ "WALKS" , "WALK" },
		{ "WALK" , "WALK" },
		{ "WALL" , "WALL" },
		{ "WAYS" , "WAYS" },
		{ "WAY" , "WAY" },
		{ "WELLS" , "WLS" },
		{ "WELL" , "WL" },
		{ "WLS" , "WLS" },
		{ "WL" , "WL" },
		{ "WY" , "WAY" },
		{ "XING" , "XING" },
		{ "XRDS" , "XRDS" },
		{ "XRD" , "XRD" },
		{ 0x0 , 0x0 }
	};
	S_CONVERSION_TYPE addressCompression::KNOWN_UNIT_TYPES [] = {
		{ "APARTMENT" , "APT" },
		{ "APT" , "APT" },
		{ "BASEMENT" , "BSMT" },
		{ "BLDG" , "BLDG" },
		{ "BSMT" , "BSMT" },
		{ "BUILDING" , "BLDG" },
		{ "DEPARTMENT" , "DEPT" },
		{ "DEPT" , "DEPT" },
		{ "FL" , "FL" },
		{ "FLOOR" , "FL" },
		{ "FRNT" , "FRNT" },
		{ "FRONT" , "FRNT" },
		{ "HANGER" , "HNGR" },
		{ "HNGR" , "HNGR" },
		{ "KEY" , "KEY" },
		{ "LBBY" , "LBBY" },
		{ "LOBBY" , "LBBY" },
		{ "LOT" , "LOT" },
		{ "LOWER" , "LOWR" },
		{ "LOWR" , "LOWR" },
		{ "OFC" , "OFC" },
		{ "OFFICE" , "OFC" },
		{ "PENTHOUSE" , "PH" },
		{ "PH" , "PH" },
		{ "PIER" , "PIER" },
		{ "REAR" , "REAR" },
		{ "RM" , "RM" },
		{ "ROOM" , "RM" },
		{ "SIDE" , "SIDE" },
		{ "SLIP" , "SLIP" },
		{ "SPACE" , "SPC" },
		{ "SPC" , "SPC" },
		{ "STE" , "STE" },
		{ "STOP" , "STOP" },
		{ "SUITE" , "STE" },
		{ "TRAILER" , "TRLR" },
		{ "TRLR" , "TRLR" },
		{ "UNIT" , "UNIT" },
		{ "UPPER" , "UPPR" },
		{ "UPPR" , "UPPR" },
		{ 0x0 , 0x0 }
	};
	S_CONVERSION_TYPE addressCompression::OTHER_CONVERSION [] = {
		{ "1ST" , "FIRST" },
		{ "2ND" , "SECOND" },
		{ "3RD" , "THIRD" },
		{ "4TH" , "FOURTH" },
		{ "5TH" , "FIFTH" },
		{ "6TH" , "SIXTH" },
		{ "7TH" , "SEVENTH" },
		{ "8TH" , "EIGHT" },
		{ "9TH" , "NINTH" },
		{ 0x0 , 0x0 }
	};
	int addressCompression::nStreetTypes = 0x0;
	int addressCompression::nUnitTypes = 0x0;
	int addressCompression::nOtherConversion = 0x0;

	// Construct the address compression class
	addressCompression::addressCompression() {

		// Need to count and sort street types?
		if( 0x0 == nStreetTypes) {
			int nST = 0;
			while( (const char *) 0x0 != KNOWN_STREET_TYPES[nST ++].type);
			nStreetTypes = nST - 1;
			qsort( KNOWN_STREET_TYPES, nStreetTypes, sizeof( S_CONVERSION_TYPE), compareConversionType);
		}

		// Need to count unit types?
		if( 0x0 == nUnitTypes) {
			int nUT = 0;
			while( (const char *) 0x0 != KNOWN_UNIT_TYPES[nUT ++].type);
			nUnitTypes = nUT - 1;
			qsort( KNOWN_UNIT_TYPES, nUnitTypes, sizeof( S_CONVERSION_TYPE), compareConversionType);
		}

		// Need to count other conversions?
		if( 0x0 == nOtherConversion) {
			int nOT = 0;
			while( (const char *) 0x0 != OTHER_CONVERSION[nOT ++].type);
			nOtherConversion = nOT - 1;
			qsort( OTHER_CONVERSION, nOtherConversion, sizeof( S_CONVERSION_TYPE), compareConversionType);
		}

	}

	// Destructor
	addressCompression::~addressCompression() {

	}

	// Lookup street type
	S_CONVERSION_TYPE * addressCompression::lookupStreetType( const char *streetType) {
		S_CONVERSION_TYPE key = { streetType, (const char *) 0x0 };
		S_CONVERSION_TYPE *ctNode = (S_CONVERSION_TYPE *) bsearch( &key, KNOWN_STREET_TYPES, nStreetTypes, sizeof( S_CONVERSION_TYPE), compareConversionType);
		return( ctNode);
	}

	// Lookup unit type
	S_CONVERSION_TYPE * addressCompression::lookupUnitType( const char *unitType) {
		S_CONVERSION_TYPE key = { unitType, (const char *) 0x0 };
		S_CONVERSION_TYPE *ctNode = (S_CONVERSION_TYPE *) bsearch( &key, KNOWN_UNIT_TYPES, nUnitTypes, sizeof( S_CONVERSION_TYPE), compareConversionType);
		return( ctNode);
	}

	void addressCompression::normalizeDeliveryLine( char *addrLine, const size_t allocStringSize) {

		// Trivial?
		if( (char *) 0x0 == addrLine) return;
		if( 0x0 == allocStringSize) return;
		if( 0x0 == addrLine[0]) return;

		// Copy the input into a delivery line
		deliveryLine dl( addrLine);

		// Ignore if there is no street name
		if( 0x0 == dl.getStreetName()[0]) return;

		// Normalize the street name
		char origStreetName [MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];
		memset( origStreetName, 0x0, sizeof( origStreetName));
		char newStreetName [MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];
		char *lasts = (char *) 0x0;
		memset( newStreetName, 0x0, sizeof( newStreetName));
		strcpy( origStreetName, dl.getStreetName());
		char *snToken = strtok_r( origStreetName, " ", &lasts);
		while( (char *) 0x0 != snToken) {
			S_CONVERSION_TYPE key = { snToken , (const char *) 0x0 };
			S_CONVERSION_TYPE *ctNode = (S_CONVERSION_TYPE *) bsearch( &key, OTHER_CONVERSION, nOtherConversion, sizeof( S_CONVERSION_TYPE), compareConversionType);
			if( (S_CONVERSION_TYPE *) 0x0 == ctNode)
				strcat( newStreetName, snToken);
			else
				strcat( newStreetName, ctNode->preftype);
			strcat( newStreetName, " ");
			snToken = strtok_r( (char *) 0x0, " ", &lasts);
		}
		newStreetName[strlen( newStreetName) - 1] = 0x0;

		// Put it all back together
		char retLine[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];
		memset( retLine, 0x0, sizeof( retLine));
		if( 0x0 != dl.getStreetNumber()[0]) {
			strcat( retLine, dl.getStreetNumber());
			strcat( retLine, " ");
		}
		if( 0x0 != dl.getPreDirectional()[0]) {
			strcat( retLine, dl.getPreDirectional());
			strcat( retLine, " ");
		}
		strcat( retLine, newStreetName);
		strcat( retLine, " ");
		if( 0x0 != dl.getStreetType()[0]) {
			strcat( retLine, dl.getStreetType());
			strcat( retLine, " ");
		}
		if( 0x0 != dl.getPostDirectional()[0]) {
			strcat( retLine, dl.getPostDirectional());
			strcat( retLine, " ");
		}
		if( 0x0 != dl.getUnitType()[0]) {
			strcat( retLine, dl.getUnitType());
			strcat( retLine, " ");
		}
		if( 0x0 != dl.getUnitNumber()) {
			strcat( retLine, dl.getUnitNumber());
			strcat( retLine, " ");
		}
		size_t nSize = strlen( retLine) - 1;
		while( ' ' == retLine [nSize]) retLine [nSize --] = 0x0;

		// Copy the result back into the input
		strncpy( addrLine, retLine, allocStringSize);

	}

	// Construct a delivery line
	deliveryLine::deliveryLine( const char *inputLine) {

		// Clear values
		memset( acStreetNum, 0x0, sizeof( acStreetNum));
		memset( acPreDirectional, 0x0, sizeof( acPreDirectional));
		memset( acStreetName, 0x0, sizeof( acStreetName));
		memset( acStreetType, 0x0, sizeof( acStreetType));
		memset( acPostDirectional, 0x0, sizeof( acPostDirectional));
		memset( acUnitType, 0x0, sizeof( acUnitType));
		memset( acUnitNumber, 0x0, sizeof( acUnitNumber));
		memset( acPOBox, 0x0, sizeof( acPOBox));
		memset( acRuralRoute, 0x0, sizeof( acRuralRoute));
		memset( acRemainder, 0x0, sizeof( acRemainder));

		// Trivial?
		if( (const char *) 0x0 == inputLine) return;
		if( 0x0 == inputLine[0]) return;

		// Allocate space for the tokens
		addressCompression addrComp;
		typedef std::vector<char *> CNT_STRPTR;
		typedef CNT_STRPTR::iterator ITR_STRPTR;
		CNT_STRPTR allTokens;

		// Make a copy of the input, removing punctuation
		char copyValue [MAX_DELIVERY_LINE_ELEMENT_SIZE * 4 + 1];
		memset( copyValue, 0x0, sizeof( copyValue));
		int nPos, nCopyPos;
		nPos = nCopyPos = 0;
		for( ; ((MAX_DELIVERY_LINE_ELEMENT_SIZE * 4) > nPos) && (0x0 != inputLine[nPos]); ++ nPos) {
			if( ('#' == inputLine[nPos]) || (! ispunct( inputLine[nPos]))) copyValue[nCopyPos ++] = toupper(inputLine[nPos]);
		}

		// PO Box?
		bool isPOBox = false;
		for( int nPO = 0x0; addressCompression::KNOWN_PO_BOX_HEADERS [nPO] != (const char *) 0x0; ++ nPO) {
			if( 0x0 == strncmp( addressCompression::KNOWN_PO_BOX_HEADERS [nPO], copyValue, strlen( addressCompression::KNOWN_PO_BOX_HEADERS [nPO]))) {
				// Remove box prefix - clears for later tokenization
				memset( copyValue, ' ', strlen(addressCompression::KNOWN_PO_BOX_HEADERS [nPO]));
				isPOBox = true;
				break;
			}
		}

		// Rural route?
		bool isRuralRoute = false;
		for( int nRR = 0x0; addressCompression::KNOWN_RURAL_ROUTE_HEADERS [nRR] != (const char *) 0x0; ++ nRR) {
			if( 0x0 == strncmp( addressCompression::KNOWN_RURAL_ROUTE_HEADERS [nRR], copyValue, strlen( addressCompression::KNOWN_RURAL_ROUTE_HEADERS [nRR]))) {
				// Remove RR prefix - clears for later tokenization
				memset( copyValue, ' ', strlen(addressCompression::KNOWN_RURAL_ROUTE_HEADERS [nRR]));
				isRuralRoute = true;
				break;
			}
		}


		// Tokenize
		char *lasts = (char *) 0x0;
		char *token = strtok_r( copyValue, " \t", &lasts);
		while( (char *) 0x0 != token) {
			allTokens.push_back( token);
			token = strtok_r( (char *) 0x0, " \t", &lasts);
		}

		// PO Box?
		if( isPOBox) {
			if( allTokens.size() >= 2) {
				sprintf( acPOBox, "PO BOX %s", allTokens[1]);
			}
			for( int nToken = 2; allTokens.size() > nToken; ++ nToken) {
				strcat( acRemainder, allTokens[nToken]);
				strcat( acRemainder, " ");
			}
			return;
		}

		// Rural route?
		if( isRuralRoute) {

			int nextToken = 1;

			// Potential case of "Rural Route RR#BOX"
			if( allTokens.size() == 1) {
				allTokens.clear();
				lasts = (char *) 0x0;
				token = strtok_r( copyValue, " \t#", &lasts);
				while( (char *) 0x0 != token) {
					allTokens.push_back( token);
					token = strtok_r( (char *) 0x0, " \t", &lasts);
				}
			}

			// Enough for rural route & box
			if( allTokens.size() >= 2) {

				// Or rural route as least!
				sprintf( acRuralRoute, "RURAL ROUTE %s", allTokens[0]);

				// Jump the box header
				if( (0x0 == strcmp( allTokens[nextToken], "#")) ||
				   	(0x0 == strcmp( allTokens[nextToken], "BOX")) ||
				   	(0x0 == strcmp( allTokens[nextToken], "UNIT"))) {
					++ nextToken;
				}

				// Capture the box
				if( allTokens.size() > nextToken) {
					strcat( acRuralRoute, " BOX ");
					if( '#' != allTokens[nextToken][0])
						strcat( acRuralRoute, allTokens[nextToken]);
					else
						strcat( acRuralRoute, (allTokens[nextToken]) + 1);
					++ nextToken;
				}

				// And remainder
				for( ; allTokens.size() > nextToken; ++ nextToken) {
					strcat( acRemainder, allTokens[nextToken]);
					strcat( acRemainder, " ");
				}
			}

			return;

		} // endif rural route

		// Starting from the right look for a sreet tyoe
		unsigned long nStreetTypePos = -1;
		unsigned long nCurToken = allTokens.size() - 1;
		while( nCurToken > 1) {
			char *pToken = allTokens[nCurToken];
			S_CONVERSION_TYPE *ctNode = addrComp.lookupStreetType( pToken);
			if( (S_CONVERSION_TYPE *) 0x0 != ctNode) {
				nStreetTypePos = nCurToken;
				strncpy( acStreetType, ctNode->preftype, (sizeof( acStreetType) / sizeof( acStreetType[0])) - 1);
				break;
			}
			-- nCurToken;
		}

		// If the street type was found, look left for apartment or unit type
		unsigned long nUnitTypePos = -1;
		if( -1 != nStreetTypePos) {

			// Look for the unit type
			for( -- nCurToken; (0 <= nCurToken) && (allTokens.size() > nCurToken) ; -- nCurToken) {
				char *pToken = allTokens[nCurToken];
				S_CONVERSION_TYPE *ctNode = addrComp.lookupUnitType( pToken);
				if( 0x0 == strcmp( pToken, "#")) {
					nUnitTypePos = nCurToken;
					strncpy( acUnitType, "UNIT", (sizeof( acUnitType) / sizeof( acUnitType[0])) - 1);
					break;
				}
				else if( '#' == pToken[0]) {
					strncpy( acUnitType, "UNIT", (sizeof( acUnitType) / sizeof( acUnitType[0])) - 1);
					strncpy( acUnitNumber, pToken + 1, (sizeof( acUnitNumber) / sizeof( acUnitNumber[0])) - 1);
					ITR_STRPTR itErase = allTokens.begin();
					std::advance(itErase, nCurToken);
					allTokens.erase( itErase);
					nUnitTypePos = -1;
					-- nStreetTypePos;
					break;
				}
				else if( (S_CONVERSION_TYPE *) 0x0 != ctNode) {
					nUnitTypePos = nCurToken;
					strncpy( acUnitType, pToken, (sizeof( acUnitType) / sizeof( acUnitType[0])) - 1);
					break;
				}
			}

			// Extract the unit number information
			if( 0 == nUnitTypePos) {
				// Address starts with unit number
				// Assume unit number is only the second part
				// Then remove it from the tokens list becuase it will mess things up
				strncpy( acUnitNumber, allTokens[1], (sizeof( acUnitNumber) / sizeof( acUnitNumber[0])) - 1);
				allTokens.erase( allTokens.begin());
				allTokens.erase( allTokens.begin());
				nStreetTypePos -= 2;
				nUnitTypePos = -1;
			}
			else if( -1 != nUnitTypePos) {
				// Early find of the unit type and number
				// Save it, but then remove them for the list
				char *pToken = allTokens[nUnitTypePos + 1];
				strncpy( acUnitNumber, pToken, (sizeof( acStreetType) / sizeof( acStreetType[0])) - 1);
				ITR_STRPTR itErase = allTokens.begin();
				std::advance(itErase, nUnitTypePos);
				itErase = allTokens.erase( itErase);
				allTokens.erase( itErase);
				nStreetTypePos -= 2;
				nUnitTypePos = -1;
			}

		}

		// If a street type was found, then find other values
		unsigned long nRemainder = 0;
		if( -1 != nStreetTypePos) {

			nRemainder = nStreetTypePos + 1;

			// Extract street number
			long streetNumber = -1;
			if( 1 != sscanf( allTokens.front(), "%ld", &streetNumber)) streetNumber = -1;

			// Is there a pre-directional?
			unsigned long nStreetNameTo = nStreetTypePos - 1;
			char *pToken = allTokens[nStreetTypePos - 1];
			for( int nPos = 0; (const char *) 0x0 != addressCompression::KNOWN_DIRECTIONALS[nPos]; ++ nPos) {
				if( 0x0 == strcmp( pToken, addressCompression::KNOWN_DIRECTIONALS[nPos])) {
					strncpy( acPreDirectional, pToken, (sizeof(acPreDirectional) / sizeof( acPreDirectional[0])) - 1);
					-- nStreetNameTo;
					break;
				}
			}

			// Pull the street name
			unsigned long nStreetNameFrom = ((-1 == streetNumber) ? 0 : 1);
			if( -1 != nUnitTypePos) nStreetNameFrom = nUnitTypePos + 2;
			size_t nameLen = 0;
			for( unsigned long nPos = nStreetNameFrom; nStreetNameTo >= nPos; ++ nPos) {
				pToken = allTokens[nPos];
				strncat( acStreetName, pToken, (sizeof( acStreetName) / sizeof( acStreetName[0])) - nameLen - 2);
				strcat( acStreetName, " ");
				nameLen += strlen( pToken) + 1;
			}

			// A pre-directional with no street name means the pre-directional IS the street name
			if( (0x0 != acPreDirectional[0]) && (0x0 == acStreetName[0])) {
				strncpy( acStreetName, acPreDirectional, sizeof( acStreetName) - 1);
				memset( acPreDirectional, 0x0, sizeof( acPreDirectional));
			}

			// Have a street number?
			if( -1 != streetNumber) {
				strncpy( acStreetNum, allTokens[0], (sizeof( acStreetNum) / sizeof(acStreetNum[0])) - 1);
			}

			// Is there a post directional?
			if( allTokens.size() > (nStreetTypePos + 1)) {
				pToken = allTokens[nStreetTypePos + 1];
				for( int nPos = 0; (const char *) 0x0 != addressCompression::KNOWN_DIRECTIONALS[nPos]; ++ nPos) {
					if( 0x0 == strcmp( pToken, addressCompression::KNOWN_DIRECTIONALS[nPos])) {
						strncpy( acPostDirectional, pToken, (sizeof(acPostDirectional) / sizeof( acPostDirectional[0])) - 1);
						++ nRemainder;
						break;
					}
				}
			}

			// Need to look right for a unit number?
			if( 0x0 == acUnitType[0]) {

				for( unsigned long nPos = nStreetTypePos + 1; allTokens.size() > nPos; ++ nPos) {
					pToken = allTokens[nPos];
					S_CONVERSION_TYPE *ctNode = addrComp.lookupUnitType( pToken);
					if( 0x0 == strcmp( pToken, "#")) {
						nUnitTypePos = nPos;
						strncpy( acUnitType, "UNIT", (sizeof( acUnitType) / sizeof( acUnitType[0])) - 1);
						nRemainder = nPos + 1;
						break;
					}
					else if( '#' == pToken[0]) {
						strncpy( acUnitType, "UNIT", (sizeof( acUnitType) / sizeof( acUnitType[0])) - 1);
						strncpy( acUnitNumber, pToken + 1, (sizeof( acUnitNumber) / sizeof( acUnitNumber[0])) - 1);
						nRemainder = nPos + 1;
						break;
					}
					else if( (S_CONVERSION_TYPE *) 0x0 != ctNode) {
						nRemainder = nPos + 1;
						nUnitTypePos = nPos;
						strncpy( acUnitType, pToken, (sizeof( acUnitType) / sizeof( acUnitType[0])) - 1);
						break;
					}
				}
				if( (-1 != nUnitTypePos) && (allTokens.size() > (nUnitTypePos + 1))){
					char *pToken = allTokens[nUnitTypePos + 1];
					strncpy( acUnitNumber, pToken, (sizeof( acUnitNumber) / sizeof( acUnitNumber[0])) - 1);
					++ nRemainder;
				}

			} // endif look for unit type

		} // endif found street type

		// Capture the remainder
		size_t remainderLen = 0;
		for( unsigned long nPos = nRemainder; allTokens.size() > nPos; ++ nPos) {
			char *pToken = allTokens[nPos];
			strncat( acRemainder, pToken, (sizeof( acRemainder) / sizeof( acRemainder[0])) - remainderLen - allTokens.size());
			strcat( acRemainder, " ");
			remainderLen += strlen( pToken) + 1;
		}
		acRemainder[remainderLen - 1] = 0x0;

		// Trim the street name
		for( size_t nPos = strlen( acStreetName) - 1; (0 < nPos) && (' ' == acStreetName[nPos]); --nPos)
			acStreetName[nPos] = 0x0;

		// Fix for numbered streets (like state highways)
		if( (0x0 == acPostDirectional[0]) && (0x0 == acUnitType[0]) && (0x0 == acUnitNumber[0]) && (0x0 != acRemainder[0])) {

			bool allNumbers = true;
			for( size_t nPos = 0; 0x0 != acRemainder[nPos]; ++ nPos) {
				allNumbers &= (isdigit( acRemainder[nPos]) || isspace( acRemainder[nPos]));
			}
			if( allNumbers) {
				char newValue[4 * MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];
				sprintf( newValue, "%s %s %s", acStreetName, acStreetType, acRemainder);
				memset( acStreetName, 0x0, sizeof( acStreetName));
				memset( acStreetType, 0x0, sizeof( acStreetType));
				memset( acRemainder, 0x0, sizeof( acRemainder));
				strncpy( acStreetName, newValue, MAX_DELIVERY_LINE_ELEMENT_SIZE);
			}

		}

	}

	// Destruct a delivery line
	deliveryLine::~deliveryLine() {

	}

	// Debug dump
	void deliveryLine::debugDump( FILE *fOutput) {

		fprintf( fOutput, "Street number:    ~%s~\n", acStreetNum);
		fprintf( fOutput, "Pre directional:  ~%s~\n", acPreDirectional);
		fprintf( fOutput, "Street name:      ~%s~\n", acStreetName);
		fprintf( fOutput, "Street type:      ~%s~\n", acStreetType);
		fprintf( fOutput, "Post directional: ~%s~\n", acPostDirectional);
		fprintf( fOutput, "Unit type:        ~%s~\n", acUnitType);
		fprintf( fOutput, "Unit number:      ~%s~\n", acUnitNumber);
		fprintf( fOutput, "Remainder:        ~%s~\n", acRemainder);

	}

}

