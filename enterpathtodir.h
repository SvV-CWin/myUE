#ifndef _ENTERPATHTODIR_H
#define _ENTERPATHTODIR_H

#ifndef _WALKTREE_H
#include "walktree.h"
#endif

#ifndef _CONIO_RETRO_COLORS
#undef _CONIO_H
#define _CONIO_RETRO_COLORS
#include <conio.h>
#endif

#define eptd_START 0
#define eptd_NODIR 1
#define eptd_DIR 2
#define eptd_LISTdir 3
#define eptd_TAKEdir 4
#define eptd_CHECKdir 5

#if (defined UNICODE)||(defined _UNICODE)
#define EnterPathToDir wEnterPathToDir
#define eptd_found weptd_found
#else /*!UNICODE*/
#define EnterPathToDir _EnterPathToDir
#define eptd_found _eptd_found
#endif /*?UNICODE*/

void _EnterPathToDir(char sourcebegin[], int max);
void wEnterPathToDir(wchar_t sourcebegin[], int max);
char _eptd_found(struct _finddata64_t *object, const char folder[], int i, void *data);
char weptd_found(struct _wfinddata64_t *object, const wchar_t folder[], int i, void *data);
#include "CRTenterpathtodir.c"

#endif /*_ENTERPATHTODIR_H*/
