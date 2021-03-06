/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   scanner.h - lexikální analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */


#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "string.h"

// datkovy typ enum popisujici stavy automatu
typedef enum {
    sStart,                     //      POCATECNI STAV AUTOMATU

    /* identifikator / klicove slovo */
    sIdentificator,             //      KONCOVY STAV
    sIdentificatorOrKeyWord,    //      mezistav
    sKeyWord,                   //      KONCOVY STAV
    //sResKeyWord,                //      KONCOVY STAV

    /* datove type */
    sInteger,                   //      KONCOVY STAV

    sDouble,                    //      KONCOVY STAV
    sDoublePoint,               //      mezistav
    sDoublePointNumber,         //      mezistav
    sDoubleExponent,            //      mezistav
    sDoubleExponentOperator,    //      mezistav

    sString,                    //      KONCOVY STAV
    sStringStart,               //      mezistav
    sStringEscape,              //      mezistav
    sStringEscapeNumber,        //      mezistav

    /* operatory */
    sPlus,                      //      KONCOVY STAV    +
    sMinus,                     //      KONCOVY STAV    -
    sDivideD,                   //      KONCOVY STAV    /
    sDivideDOrBlockComment,     //      mezistav
    sMultiply,                  //      KONCOVY STAV    *
    sDivideI,                   //      KONCOVY STAV    obracene lomitko
    sLess,                      //      KONCOVY STAV    <
    sNotEqual,                  //      KONCOVY STAV    <>
    sLessEqual,                 //      KONCOVY STAV    <=
    sMore,                      //      KONCOVY STAV    >
    sMoreEqual,                 //      KONCOVY STAV    >=
    sAssignment,                //      KONCOVY STAV    =

    sLeftPar,                   //      KONCOVY STAV    (
    sRightPar,                  //      KONCOVY STAV    )
    sSemicolon,                 //      KONCOVY STAV    ;
    sComma,                     //      KONCOVY STAV    ,

    /* komentare */
    sLineComment,               //      mezistav        '
    sBlockComment,              //      mezistav /' ( ceka na ukonceni komentare: '/ )

    sEndOfLine,                 //      NOVY RADEK
    sEndOfFile,                 //      KONEC VSTUPNIHO SOUBORU
    //sEnd
    /* chybovy stav */
    sLexError = -1,             //      LEXIKALNI CHYBA



} tState;

// struktura popisujici token
typedef struct {
    string atr;
    int type;
} tToken;

// funkce vracejici dalsi token
tToken getNextToken();

#endif //IFJ_SCANNER_H
