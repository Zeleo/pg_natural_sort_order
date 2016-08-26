#include "postgres.h"
#include "fmgr.h"
#include <ctype.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

// Constants
const int NUMERIC_NORMALIZATION_DEFAULT_SIZE = 75;
const int NUMERIC_NORMALIZATION_MAX_SIZE = 150;


// Signatures
Datum natural_sort_order( PG_FUNCTION_ARGS );
int normalizeNumeric(char *to, char *from, int toStart, int fromLength, int normalizationSize);


/**
 * Given an original string this method will normalize all numerics held within
 * that string (if any) to allow for Natural Sort Ordering.
 *
 *  https://en.wikipedia.org/wiki/Natural_sort_order
 *
 *  Algorithm:
 *  For each numeric normalize it and construct a new string with the normalized
 *  numeric in place of the original.
 *  
 *  Normalization in this instance is that each numeric contain the same number of
 *  places: 75 by default. Each numeric less than 75 will be prepended with zeros. 
 *
 *  This algorithm can also be thought of as a mapping in which a numeric is
 *  mapped to another numeric that always contains 75 places.
 *
 *  Any numeric greater than 75 results in a bad order and a warning is emitted. 
 *  Nothing can be done beyond increasing the normalization places. Highly unlikely.
 * 
 * 
 */

PG_FUNCTION_INFO_V1( natural_sort_order );
Datum
natural_sort_order( PG_FUNCTION_ARGS ) {
    const int OUTPUT_BUFFER_LENGTH = 10000;
    const int NUMERIC_BUFFER_LENGTH = 200; // more than enough
    
    // variable declarations
    text *original;
    int originallen;
    char *originalData;

    int32 numericSize;
    
    char numericBuffer[NUMERIC_BUFFER_LENGTH];
    text *outputBuffer;
   
    int k = 0, i = 0, j = 0;
    
    
    // Get arguments. If we declare our function as STRICT, then
    // this check is superfluous.
    if( PG_ARGISNULL(0) ) {
        PG_RETURN_NULL();
    }

    // Get the original string and its length
    original = PG_GETARG_TEXT_P(0);
    originallen = VARSIZE(original) - VARHDRSZ;

    numericSize = PG_GETARG_INT32(1);
    numericSize = (numericSize > 0 && numericSize <= NUMERIC_NORMALIZATION_MAX_SIZE) ? numericSize : NUMERIC_NORMALIZATION_DEFAULT_SIZE;
    
    // construct an output buffer of max OUTPUT_BUFFER_LENGTH. Add 10 for safety. :)
    outputBuffer = (text *)palloc(OUTPUT_BUFFER_LENGTH + 10);
    SET_VARSIZE(outputBuffer, OUTPUT_BUFFER_LENGTH + VARHDRSZ);

    // Loop through the data character per character.
    // If you find a digit keep grabbing the digits until you have the entire numeric.
    // Once you hit a non-digit character and there is a numeric, normailze that and add
    // the non digit.
    // If you find a non-digit and there is no numeric simply add it and continue to the
    // next character.
    originalData = VARDATA(original);
    for(i = 0; i < originallen && j < OUTPUT_BUFFER_LENGTH; i++) { 
        if(isdigit(originalData[i]) && k < numericSize) {
            numericBuffer[k++] = originalData[i];
        } else if(k>0) {
            j = normalizeNumeric(VARDATA(outputBuffer), numericBuffer, j, k, numericSize);
            k = 0;
            VARDATA(outputBuffer)[j++] = originalData[i];
        } else {
            VARDATA(outputBuffer)[j++] = originalData[i];
        }
    }

    // Edge case in which there is a number at the end of the original string.
    if(k>0) {
        j = normalizeNumeric(VARDATA(outputBuffer), numericBuffer, j, k, numericSize);
    }

    // The size of the string we generated. Don't forget the variable header size.
    SET_VARSIZE(outputBuffer, j + VARHDRSZ);

    // Return to the caller.
    PG_RETURN_TEXT_P( outputBuffer );
}


/*
  Will copy data from -> to for fromLength at toStart. 
  The normalizationSize determines the leading zeros to insert.
 */
int normalizeNumeric(char *to, char *from, int toStart, int fromLength, int normalizationSize) {
    int zeros = normalizationSize - fromLength; // number of leading zeros required.
    int i;
    for(i = 0; i < zeros; to[toStart++] = '0', i++); // pad with leading zeros
    for(i = 0; i < fromLength; to[toStart++] = from[i++]); // copy the numeric.

    return toStart; // where we left off
}
