/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   scanner.c - lexikální analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "error_code.h"
#include "scanner.h"

tToken token; // token
tState state;

tToken getNextToken(){
    if (token.atr.value == NULL)
        stringInit(&token.atr); // inicializace retezce tokenu
    else
        stringClear(&token.atr); // smazani obsahu retezce tokenu
    token.type = sStart; // inicializace typu tokenu

    state = sStart; // inicializace automatu na pocatecni stav
    char c; // aktualne cteny znak ze vstupniho souboru

    while (1) {
        c = getchar(); // nacteni dalsiho znaku ze vstupu

        switch (state) {
            /* ----------------------------------------START POCATECNI STAV AUTOMATU----------------------------------*/
            case sStart:
                if ( charIsSpace(c) || charIsTab(c) )
                    state = sStart;
                else if ( c == EOF ) { // konec souboru
                    stringAddChar(&token.atr, 'E');
                    stringAddChar(&token.atr, 'O');
                    stringAddChar(&token.atr, 'F');
                    token.type = sEndOfFile;
                    return token;
                }
                else if ( c == '\n' ) { // novy radek
                    stringAddChar(&token.atr, 'E');
                    stringAddChar(&token.atr, 'O');
                    stringAddChar(&token.atr, 'L');
                    token.type = sEndOfLine;
                    return token;
                }
                else if (c == '-' ) {
                    stringAddChar(&token.atr, c); // zapsani znaku do tokenu
                    state = sMinus;
                }
                else if (c == '+' ) {
                    stringAddChar(&token.atr, c);
                    state = sPlus;
                }
                else if (c == '*' ) {
                    stringAddChar(&token.atr, c);
                    state = sMultiply;
                }
                else if (c == '/' ) {
                    stringAddChar(&token.atr, c);
                    state = sDivideDOrBlockComment;
                }
                else if (c == '\\' ) {
                    stringAddChar(&token.atr, c);
                    state = sDivideI;
                }
                else if (c == '(' ) {
                    stringAddChar(&token.atr, c);
                    state = sLeftPar;
                }
                else if (c == ')' ) {
                    stringAddChar(&token.atr, c);
                    state = sRightPar;
                }
                else if (c == ';' ) {
                    stringAddChar(&token.atr, c);
                    state = sSemicolon;
                }
                else if (c == '=' ) {
                    stringAddChar(&token.atr, c);
                    state = sAssignment;
                }
                else if (c == '<' ) {
                    stringAddChar(&token.atr, c);
                    state = sLess;
                }
                else if (c =='>' ) {
                    stringAddChar(&token.atr, c);
                    state = sMore;
                }
                else if (c == '_' ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else if( charIsLetter(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificatorOrKeyWord;
                }
                else if (c == '\'' ) {
                    state = sLineComment;
                }
                else if ( charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sInteger;
                }
                else if ( c == '!' ) { // pocatek retezce
                    //stringAddChar(&token.atr, c);
                    state = sStringStart;
                }
                else {// nacteni nepovoleneho znaku: lex error
                    stringAddChar(&token.atr, c);
                    token.type = sLexError;
                    return token;
                }
                break;
            /* ----------------------------------------END POCATECNI STAV AUTOMATU------------------------------------*/


            /* ----------------------------------------START LESS / MORE----------------------------------------------*/
            case sLess: // <
                if (c == '=') { // vrat token <=
                    stringAddChar(&token.atr, c);
                    state = sLessEqual;
                }
                else if (c == '>') { // vrat token <>
                    stringAddChar(&token.atr, c);
                    state = sNotEqual;
                }
                else { // vrat token <
                    charUndo(c);
                    token.type = sLess;
                    return token;
                }
                break;

            case sMore: // >
                if (c == '=') { // vrat token >=
                    stringAddChar(&token.atr, c);
                    state = sMoreEqueal;
                }
                else { // vrat token >
                    charUndo(c);
                    token.type = sMore;
                    return token;
                }
                break;
            /* ----------------------------------------END LESS / MORE------------------------------------------------*/


            /* ----------------------------------------START IDENTIFIKATOR / KLICOVE SLOVO----------------------------*/
            case sIdentificatorOrKeyWord:
                if (c == '_' || charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else if ( charIsLetter(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificatorOrKeyWord;
                }
                else { // vrat token identifikator / klicove slovo
                    charUndo(c);
                    // zjisteni, jeslti je token keyword nebo identifikator
                    stringToLowercase(&token.atr); // prevod atributu tokenu na lowercase
                    if ( stringIsKeyWord(&token.atr) ) {
                        token.type = sKeyWord;
                    }
                    else {
                        token.type = sIdentificator;
                    }
                    return token;
                }
                break;

            case sIdentificator:
                if ( c == '_' || charIsLetter(c) || charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else { // vrat token identifikator
                    charUndo(c);
                    token.type = sIdentificator;
                    return token;
                }
                break;

            case sKeyWord:
                break;
            /* ----------------------------------------END IDENTIFIKATOR / KLICOVE SLOVO------------------------------*/


            /* ----------------------------------------START KOMENTARE----------------------------------------------- */
            case sLineComment: // '
                if (c == '\n' || c == EOF) {
                    state = sStart;
                }
                // pokud precte jakykoliv jiny znak nez '\n' nebo EOF nic se nestane
                break;

            case sBlockComment: // /'
                if ( ((stringGetLastChar(&token.atr) == '\'') && (c == '/')) || (c == EOF) ) { // ukonceni blokoveho komentare
                    stringClear(&token.atr);
                    state = sStart;
                    break;
                }
                stringAddChar(&token.atr, c);
                break;
            /* ----------------------------------------END KOMENTARE------------------------------------------------- */


            /* ----------------------------------------START INTEGER------------------------------------------------- */
            case sInteger:
                if ( charIsDigit(c) ) { // cteni dalsich cislic
                    stringAddChar(&token.atr, c);
                    state = sInteger;
                }
                else if (c == '.') {
                    stringAddChar(&token.atr, c);
                    state = sDoublePoint;
                }
                else if (c == 'e' || c == 'E') {
                    stringAddChar(&token.atr, c);
                    state = sDoubleExponent;
                }
                else { // vrat token integer
                    charUndo(c);
                    token.type = sInteger;
                    return token;
                }
                break;
            /* ----------------------------------------END INTEGER--------------------------------------------------- */


            /* ----------------------------------------START DOUBLE-------------------------------------------------- */
            case sDoublePoint:
                if ( charIsDigit(c) ) { // cteni dalsich cislic
                    stringAddChar(&token.atr, c);
                    state = sDoublePointNumber;
                }
                else { // nacteni nepovoleneho znaku: lex error
                    charUndo(c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sDoublePointNumber:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla
                    stringAddChar(&token.atr, c);
                    state = sDoublePointNumber;
                }
                else if ( c == 'e' || c == 'E' ) {
                    stringAddChar(&token.atr, c);
                    state = sDoubleExponent;
                }
                else { // nepovoleny znak: vrat token double
                    charUndo(c);
                    token.type = sDouble;
                    return token;
                }
                break;

            case sDoubleExponent:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla: presun do stavu sDouble
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else if ( c == '+' || c == '-') {
                    stringAddChar(&token.atr, c);
                    state = sDoubleExponentOperator;
                }
                else { // nepovoleny znak: lex error
                    charUndo(c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sDoubleExponentOperator:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else { // nepovoleny znak: lex error
                    charUndo(c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sDouble:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else { // pri nacteni nepovoleneho znaku: vrat token double
                    charUndo(c);
                    token.type = sDouble;
                    return token;
                }
                break;
            /* ----------------------------------------END DOUBLE---------------------------------------------------- */


            /* ----------------------------------------START STRING-------------------------------------------------- */
            case sStringStart: // !
                if ( c == '"' ) { // znak "
                    //stringAddChar(&token.atr, c);
                    state = sString;
                }
                else { // nepovoleneny znak: lex error
                    charUndo(c);
                    stringAddChar(&token.atr, '!'); // pro lepsi porozumeni chybe: prida do tokenu znak !
                    stringAddChar(&token.atr, c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sString:
                if ( c == '"' ) { // znak " ukonceni stringu: vrat token string
                    //stringAddChar(&token.atr, c);
                    token.type = sString;
                    return token;
                }
                else if ( c > 31 ) {
                    stringAddChar(&token.atr, c);
                    state = sString;
                }
                else { // nepovoleny znak: lex error
                    charUndo(c);
                    stringAddFirstChar(&token.atr, '"'); // zapis !" do tokenu, aby bylo jasne, ze k chybe doslo ve stringu
                    stringAddFirstChar(&token.atr, '!'); // zapis !" do tokenu, aby bylo jasne, ze k chybe doslo ve stringu
                    token.type = sLexError;
                    return token;
                }
                break;
            /* ----------------------------------------END STRING---------------------------------------------------- */


            /* ----------------------------------------START OSTATNI KONCOVE STAVY------------------------------------*/
            case sMinus:
            case sPlus:
            case sMultiply:
            case sDivideD:
            case sDivideI:
            case sLeftPar:
            case sRightPar:
            case sSemicolon:
            case sAssignment:
            case sLessEqual:
            case sMoreEqueal:
            case sNotEqual:
                charUndo(c); // vrati zpet aktualne cteny znak
                token.type = state; // naplni token typem nalezeneho lexemu
                return token;
                break;
            /* ----------------------------------------END OSTATNI KONCOVE STAVY--------------------------------------*/


            /* ----------------------------------------START OSTATNI POMOCNE STAVY------------------------------------*/
            case sDivideDOrBlockComment: // deleno / nebo blokovy komentar /'
                if (c == '\'') { // start blokoveho komentare
                    stringClear(&token.atr); // smaze znak / z tokenu
                    state = sBlockComment;
                }
                else {
                    charUndo(c);
                    state = sDivideD;
                }
                break;
            /* ----------------------------------------END OSTATNI POMOCNE STAVY--------------------------------------*/


            case sEndOfLine: // pouze pro preklad
            case sEndOfFile:
            case sLexError:
                break;
        }
    }
}