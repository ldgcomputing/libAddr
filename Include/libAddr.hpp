//
//  libAddr.hpp
//  DecodeShapeLib
//
//  Created by Louis Gehrig on 5/11/19.
//  Copyright © 2019 Louis Gehrig. All rights reserved.
//

#ifndef libAddr_hpp
#define libAddr_hpp

// Project defines
#define	MAX_DELIVERY_LINE_ELEMENT_SIZE		(64)

namespace libAddr {

	// Street type structure
	struct s_conversion_types {
		const char *type;			// What might be expected
		const char *preftype;		// The value preferred by the USPS
	};
	typedef struct s_conversion_types S_CONVERSION_TYPE;
	int compareConversionType( const void *left, const void *right);

	//
	// A class to hold address compression data and utils
	//

	class addressCompression {

	public:

		// Directionals
		static const char * KNOWN_DIRECTIONALS [];

		// PO box headers
		static const char * KNOWN_PO_BOX_HEADERS [];

		// Street types
		static S_CONVERSION_TYPE KNOWN_STREET_TYPES [];

		// Secondary unit designations
		static S_CONVERSION_TYPE KNOWN_UNIT_TYPES [];

		// Other conversion values
		static S_CONVERSION_TYPE OTHER_CONVERSION [];

		// Construction
		// WARNING - construction is not thread safe!v
		addressCompression();

		// Destruction
		virtual ~addressCompression();

		// Lookup street type - input must be capitalized
		S_CONVERSION_TYPE * lookupStreetType( const char *streetType);

		// Lookup unit type - input must be capitalized
		S_CONVERSION_TYPE * lookupUnitType( const char *unitType);

		// Normalize a line - input will be adjusted
		void normalizeDeliveryLine( char *addrLine, const size_t allocStringSize);

	protected:

		// The number of street types
		static int nStreetTypes;

		// The number of unit types
		static int nUnitTypes;

		// The number of other conversion
		static int nOtherConversion;

	};

	//
	// A class for parsing and normalizing street addresses
	//
	// The address elements will be blank if they do not apply.
	// For example, a PO Box will have no street number, name,
	// directional, etc.
	//
	// Anything not understood will be dropped into the "Remainder"
	// output, and this may exceed the MAX_DELIVERY_LINE_ELEMENT_SIZE
	// size limits.
	//

	class deliveryLine {

	public:

		// Construction - from raw input street line
		// Input larger than 4 * MAX_DELIVERY_LINE_ELEMENT_SIZE may be trimmed
		deliveryLine( const char *inputLine);

		// Destruction
		virtual ~deliveryLine();

		// Debug output dump
		void debugDump( FILE *fOutput);

		// Return the full, clean line
		const char *getFullLine() const;

		// Return the full, clean line without the street number
		// This will also return blank if the address is a PO box, rural route, etc.
		const char *getFullLineNoNumber() const ;

		// Return the street number
		const char *getStreetNumber() const { return( acStreetNum); }

		// Return the pre-diretional
		const char *getPreDirectional() const { return( acPreDirectional); }

		// Return the street name
		const char *getStreetName() const { return( acStreetName); }

		// Return the street type
		const char *getStreetType() const { return( acStreetType); }

		// Return the post-directional
		const char *getPostDirectional() const { return( acPostDirectional); }

		// Return the unit type
		const char *getUnitType() const { return( acUnitType); }

		// Return the unit number
		const char *getUnitNumber() const { return( acUnitNumber); }

		// Return the PO box
		const char *getPOBox() const { return( acPOBox); }

		// Return the rural route
		const char *getRuralRoute() const { return( acRuralRoute); }

		// Return the remainder
		const char *getRemainder() const { return( acRemainder); }

	protected:

		// The street number
		char acStreetNum[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// The pre-directional
		char acPreDirectional[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// The street name
		char acStreetName[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// The street type
		char acStreetType[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// The street post-directional
		char acPostDirectional[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// The unit type
		char acUnitType[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// The unit number
		char acUnitNumber[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// PO Box information
		char acPOBox[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// Rural route information
		char acRuralRoute[MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

		// Remainder
		char acRemainder[4 * MAX_DELIVERY_LINE_ELEMENT_SIZE + 1];

	};

};

#endif /* libAddr_hpp */
