//!!voor myUE: deze is zonder Windows.h

/*
*	Een equivalent voor "dir *term* /s": een map en onderliggende mappen wordt doorzocht, op zoek naar een zoekterm (map of bestand)
*	De functie _ftw() vraagt veel minder code, maar is veel trager
*
*	int walktree(const TCHAR bron[], const unsigned short int max_array_bron, const TCHAR zoek[], char functie(LPWIN32_FIND_DATA object, const TCHAR map[], int teller, void *data), const char TODO, int i, void *data)
*	int walktree(const _tchar bron[], const unsigned short int max_array_bron, const _tchar zoek[], char functie(struct _tfinddata64_t *object, const _tchar map[], int teller, void *data), const char TODO, int i, void *data)
*	char w_R1(LPWIN32_FIND_DATA object, const TCHAR map[], int teller, void *data)
*	char w_R1(struct _finddata64_t *object, const _tchar folder[], int i, void *data)
*	| argumenten:	[array] map waarbinnen gezocht moet worden (mag '*' of '?' bevatten);
*					[int] maximumlengte van de array die de(ze) bronmap bevat (inclusief '\0') (onbelangrijk bij de vlag w_CHECK en w_DIR);
*					[array] zoekterm (mag geen '*' of '?' bevatten) (onbelangrijk bij de vlag w_CHECK; bij de vlag w_DIR en w_COMPLETE is "" aangeraden);
*					[functie] functienaam van de op te roepen functie bij elk gevonden object;
*					[char] vlag (zie uitleg verder);
*					[int] [MOET 0 zijn, behalve bij de vlag w_CHECK] teller voor het aantal gevonden objecten
*					[] pointer naar data die kan doorgegeven worden naar de functie die wordt opgeroepen
*	| over de functie die wordt opgeroepen: de functie wordt een pointer naar een _finddata64_t/WIN32_FIND_DATA struct doorgegeven met info over het gevonden object,
*		een array met het pad naar de map waarin het gevonden object zich bevindt, een int die de telling meegeeft (zie returnwaarde) en een pointer naar het void-object
*		dat meegegeven is als parameter; als deze functie 0 returned, wordt er gestopt met zoeken
*	| over w_R1: deze dummy functie kan je als argumentfunctie gebruiken bij het oproepen van walktree. Deze functie returned altijd 1.
*	| over de vlag:	w_WALK: de map en alle onderliggende mappen worden doorzocht naar de zoekterm; gevonden objecten worden doorgegeven naar de functie;
*						er wordt rekening gehouden met de maximumlengte
*					w_DIR: zelfde als w_WALK, onderliggende mappen worden niet doorzocht (er wordt geen rekening gehouden met de maximumlengte)
*					w_COMPLETE: alle mogelijke aanvullingen voor de map of het bestand in de array, rekeninghoudend met de zoekterm, worden doorgegeven naar de functie;
*						er wordt niet in onderliggende mappen gezocht; er wordt rekening gehouden met de maximumlengte
*					w_CHECK: er wordt alleen gecontroleerd of het bestand of de map met de naam in de array bestaat (returnwaarde 1: object bestaat,
*						returnwaarde 0: object bestaat niet); bij een bestaand object wordt de _finddata64_t struct doorgegeven naar de functie
*	| returnwaarde: tweemaal het aantal gevonden objecten; er wordt een eenheid bijgeteld als paden werden gevonden die langer zijn dan dat de array toelaat (alleen bij de vlag
*		w_WALK en w_COMPLETE); het aantal wordt tegengesteld gemaakt als de zoekopdracht is afgebroken geweest; -1 bij een error (de array kan niets meer bijhouden)
*
*	MAKER: Sven Verlinden
*/

#ifndef _WALKTREE_H
#define _WALKTREE_H

#ifndef _STDIO_H
#include <stdio.h>
#endif
#ifndef _IO_H
#include <io.h>
#endif
#ifndef _STRING_H
#include <string.h>
#endif
#ifndef _WCHAR_H
#include <wchar.h>
#endif
#ifndef _ERRNO_H
#include <errno.h>
#endif

#define w_WALK 1
#define w_DIR 0
#define w_COMPLETE 3
#define w_CHECK 2

#if (defined UNICODE)||(defined _UNICODE)
#define walktree wwalktree
#define w_R1 ww_R1
#else /*!UNICODE*/
#define walktree _walktree
#define w_R1 _w_R1
#endif /*?UNICODE*/

int _walktree(const char bron[], const unsigned short int max_array_bron, const char zoek[], char functie(struct _finddata64_t *object, const char map[], int teller, void *data), const char TODO, int i, void *data);
char _w_R1(struct _finddata64_t *object, const char folder[], int i, void *data);
int wwalktree(const wchar_t bron[], const unsigned short int max_array_bron, const wchar_t zoek[], char functie(struct _wfinddata64_t *object, const wchar_t map[], int teller, void *data), const char TODO, int i, void *data);
char ww_R1(struct _wfinddata64_t *object, const wchar_t folder[], int i, void *data);
#include "CRTwalktree.c"

#endif /*_WALKTREE_H*/
