/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   string.h - pomocná knihovna pro lexikální analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_STRING_H
#define IFJ_STRING_H

#define INIT_ALLOC_SIZE 10


typedef struct {
    char *value; // retezec
    int length; // delka retezce
    int lengthAllocated; // alokovana delka
} string;

int stringInit(string *);
int stringAddChar(string *, char);
int stringAddFirstChar(string *, char);
int stringClear(string *);

void stringToLowercase(string *);


int stringIsKeyWord(string *);
char stringGetLastChar(string *);

// FUNKCE PRO PRACI SE ZNAKY
void charUndo(char);
int charIsSpace(char);
int charIsTab(char);
int charIsDigit(char);
int charIsLetter(char);

#endif //IFJ_STRING_H