//icon 1: http://www.pd4pic.com/flat-info-theme-action-extract-icon-extraction.html
//icon 2: http://www.flaticon.com/free-icon/eject-button_68686#term=extract&page=1&position=25, zet: "Icoon gemaakt door FadyUCF van www.flaticon.com" in de applicatie

#define VERSION L"v3.4"

#define UNICODE
#define _UNICODE

#include <process.h>	//for launching the backbone programs		//++use _P_DETACH?
#include <direct.h>		//for changing current directory and perform checks by removing directories
#include <locale.h>		//for correct (console) character handling
#include <string.h>		//for string manipulations
#define _CONIO_RETRO_COLORS
#include <conio.h>		//for _getch
#include <io.h>			//for manipulating stdout
#include <fcntl.h>		//macros for io.h
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>		//for _fullpath, system (for use with isxunpack)
#include <errno.h>		//for function errorhandling

#include <windows.h>	//for setconsoleicon
#pragma comment(lib,"user32.lib")	//for LoadIcon
#undef ERROR
#undef FAILED
BOOL WINAPI SetConsoleIcon(HICON hIcon);
#include "resources.h"	//for icon resource

#define BLACKBCKGR
#define ENGLISH
#include "enterpathtodir.h"

#define PATH 61
#define MAX_TEMP 2*256+20

#define SUCCESS 0
#define ERROR -1
#define FAILED 12345

#define IZARC 1
#define SZ 2
#define EXE 3
#define MSI 4
#define LIT 5
#define DBX 6
#define MHT 7
//#define SIT 8	//see ext function
#define HLP 9
#define CHM 10
#define KGB 11
#define LZO 12
#define NBH 13
//#define PEA 14 //see ext function
#define SZslave 15
#define UHA 16
//#define LZX 17
#define CAB 18
#define FB2 19
#define GAUP 20
#define GIF 21
#define EPUB 22
#define PDF 23
#define UPX 24
#define ASPACK1 25
#define ASPACK2 26
#define PECOMPACT 27
#define INNO 28
#define INSTALLSHIELD 29
#define SETUPFACTORY 30
#define ISHIELDCAB 31
#define INSTALLEXPLORER 32
#define LIB 33
#define DLL 34
#define SWF 35
#define DOC 36
#define XLS 37
#define PPT 38
#define VIDEO 112


char install(const wchar_t thisprogname[]);
int UE(const char todo, const wchar_t source[]);
wchar_t * extractto(const wchar_t source[]);
char ext(const wchar_t input[]);
wchar_t *TrIDanalysis(const wchar_t source[]);
char Qdir(struct _wfinddata64_t *object, const wchar_t folder[], int i, void *data);

char D = 0, isfile;
unsigned short int maxlen, tlen;	//wcslen(thispath) + 1 + 5 + 24
wchar_t thispath[PATH], dest[MAX_TEMP], temp[MAX_TEMP], *prog, *arg0;
intptr_t Pid, Pret;

BOOL WINAPI SetConsoleIcon(HICON hIcon)
{
	typedef BOOL (WINAPI *PSetConsoleIcon)(HICON);
	static PSetConsoleIcon pSetConsoleIcon = NULL;
	if(pSetConsoleIcon == NULL)
		pSetConsoleIcon = (PSetConsoleIcon)GetProcAddress(GetModuleHandle(L"kernel32"), "SetConsoleIcon");
	if(pSetConsoleIcon == NULL)
		return FALSE;
	return pSetConsoleIcon(hIcon);
}

int wmain(int argc, wchar_t *argv[])
{
	SetConsoleIcon(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icoon)));
	setlocale(LC_ALL, "");
	if((argc == 3)&&(!wcscmp(argv[2], L"debug")))
	{
		D = 1;
		argc = 2;
	}
	/*determine directory of this executable; thispath = current directory; argv[0] = executable name*/
	if(D)
	{
		_textcolor(DARKGRAY);
		printf("argv[0] = |%ls|\nargv[1] = |%ls|\n", argv[0], argv[1]);
	}
	if(wcschr(argv[0], L'\\') == NULL)
	{
		if(_wgetcwd(thispath, PATH) == NULL)
		{
			_textcolor(LIGHTRED);
			printf("The path to this executable is too long (max %d characters)!\nPress any key to exit. Move the executable and try again.\n%s\n", PATH-1, strerror(errno));
			D = _getch();
			if((D == 0)||(D == 224))
				_getch();
			_textcolor(WHITE);
			return -1;
		}
	}
	else
	{
		*wcsrchr(argv[0], L'\\') = L'\0';
		if(_wfullpath(thispath, argv[0], PATH) == NULL)
		{
			_textcolor(LIGHTRED);
			printf("The path to this executable is too long (max %d characters)!\nPress any key to exit. Move the executable and try again.\n", PATH-1);
			D = _getch();
			if((D == 0)||(D == 224))
				_getch();
			_textcolor(WHITE);
			return -1;
		}
		wmemmove(argv[0], wcsrchr(argv[0], L'\0')+1, wcslen(wcsrchr(argv[0], L'\0')+1)+1);
	}
	_textcolor(LIGHTGREEN);
	printf("My Universal Extractor (%ls)     by Sven Verlinden\n", VERSION);
	puts("All credits go to the team working on IZArc (http://www.izarc.org)\nand those created the other backbone programs (see the doc folder).\n");
	if(argc == 1)
		return install(argv[0]);
	if(!wwalktree(argv[1], 0, L"", Qdir, w_CHECK, 0, NULL))
	{
		_textcolor(LIGHTRED);
		printf("\"%ls\" doesn't seem to exist.\n", argv[1]);
	}
	maxlen = wcslen(thispath) + 1 + 5 + 24;
	wchar_t _prog[maxlen];			//thispath to backbone program
	wchar_t _arg0[maxlen+2];		//thispath to backbone program, with '"'
	prog = _prog;
	arg0 = _arg0;
	if(D == -1)						//walktree determined that argv[1] is a directory
	{
		swprintf(prog, maxlen, L"%ls\\bin\\izarc2go.exe", thispath);
		swprintf(arg0, maxlen+2, L"\"%ls\\bin\\izarc2go.exe\"", thispath);
		if(_wspawnl(_P_NOWAIT, prog, arg0, NULL) == -1)
		{
			_textcolor(LIGHTRED);
			perror("\nAn error occured when launching IZArc2Go");
			_textcolor(WHITE);
		}
		return 0;
	}
	if(argc != 2)
		return -1;
	if(D)
	{
		_textcolor(DARKGRAY);
		printf("(current directory is considered to be |%ls|)\n", thispath);
	}
	_wfullpath(temp, argv[1], MAX_TEMP);
	tlen = wcslen(temp)+3;
	wchar_t source[tlen];			//file, with '"'
	swprintf(source, tlen, L"\"%ls\"", temp);
	_textcolor(WHITE);
	printf("Processing %ls by its extension...\n", source);
	argc = ext(source);
	errno = 0;
	int further = 1;
	dest[0] = L'\0';
	switch(argc)
	{
		case EXE:
			TrIDanalysis(source);
			if(_wcsistr(temp, L" UPX ") != NULL)
			{
				switch(UE(UPX, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("Ultimate Packer for eXecutables cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			}
			if(_wcsistr(temp, L"ASPack") != NULL)
			{
				switch(UE(ASPACK1, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("AspackDie cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				if(!further)
					break;
				switch(UE(ASPACK2, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("CASPR cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			}
			if(_wcsistr(temp, L"PECompact") != NULL)
			{
				switch(UE(PECOMPACT, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("Unpacker PECompact cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			}
			switch(UE(SZ, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				case FAILED:
					break;
				default:
					_textcolor(RED);
					puts("7-zip cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			if(!further)
				break;
			if(extractto(source) == NULL)
				break;
			errno = 0;
			if(_wcsistr(temp, L"Inno Setup") != NULL)
			{
				switch(UE(INNO, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("Inno Setup Unpacker cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				if(!further)
					break;
				switch(UE(INSTALLEXPLORER, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("InstallExplorer plugin cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				if(!further)
					break;
				switch(UE(SZslave, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("7-zip cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			}
			if(_wcsistr(temp, L"Installshield") != NULL)
			{
				switch(UE(INSTALLSHIELD, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("InstallShield (By One Exe-file) Unpacker cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				if(!further)
					break;
				switch(UE(INSTALLEXPLORER, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("InstallExplorer plugin cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				if(!further)
					break;
				switch(UE(SZslave, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("7-zip cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			}
			if(_wcsistr(temp, L"Microsoft Windows Installer") == NULL)	//if it is a msi packed in exe, goto case MSI, else the exe is unidentified
			{
				switch(UE(SETUPFACTORY, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("Setup Factory Unpacker cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				if(!further)
					break;
				switch(UE(INSTALLEXPLORER, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("InstallExplorer plugin cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				if(!further)
					break;
				switch(UE(SZslave, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("7-zip cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			}
		case LIB:
			if(argc == LIB)
			{
				TrIDanalysis(source);
				if(_wcsistr(temp, L"Microsoft Windows Installer") == NULL)	//if it is a msi packed in lib, goto case MSI, else the lib is unidentified
				{
					switch(UE(IZARC, source))
					{
						case SUCCESS:
							further = 0;
							break;
						case ERROR:
							_textcolor(LIGHTRED);
							perror("\nAn error occured during processing");
							break;
						case FAILED:
							break;
						default:
							_textcolor(RED);
							puts("IZarc cannot extract the file.");
							if(D)
								printf("(process returned %d)\n", Pret);
							break;
					}
					if(!further)
						break;
					switch((dest[0] == L'\0')?UE(SZ, source):UE(SZslave, source))
					{
						case SUCCESS:
							further = 0;
							break;
						case ERROR:
							_textcolor(LIGHTRED);
							perror("\nAn error occured during processing");
							break;
						case FAILED:
							break;
						default:
							_textcolor(RED);
							puts("7-zip cannot extract the file.");
							if(D)
								printf("(process returned %d)\n", Pret);
							break;
					}
					break;
				}
			}
		case MSI:
			if((dest[0] == L'\0')&&(extractto(source) == NULL))
				break;
			switch(UE(MSI, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				default:
					_textcolor(RED);
					puts("MSI Unpacker from JSWare cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			if(!further)
				break;
			switch(UE(INSTALLEXPLORER, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				default:
					_textcolor(RED);
					puts("InstallExplorer plugin cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			if(!further)
				break;
			switch(UE(SZslave, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				default:
					_textcolor(RED);
					puts("7-zip cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			break;
		case CAB:
			switch(UE(IZARC, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				case FAILED:
					break;
				default:
					_textcolor(RED);
					puts("IZarc cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			if(!further)
				break;
			switch((dest[0] == L'\0')?UE(SZ, source):UE(SZslave, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				case FAILED:
					break;
				default:
					_textcolor(RED);
					puts("7-zip cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			if(!further)
				break;
			if((dest[0] == L'\0')&&(extractto(source) == NULL))
				break;
			switch(UE(ISHIELDCAB, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				default:
					_textcolor(RED);
					puts("IShield plugin cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			break;
		case XLS:
		case DOC:
		case PPT:
			switch(UE(argc, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				case FAILED:
					break;
				default:
					_textcolor(RED);
					puts("Binary(doc,xls,ppt) to OpenXMLTranslator cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			if(!further)
				break;
			wcscpy(source, dest);
			if(extractto(source) == NULL)
				break;
			switch(UE(IZARC, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				default:
					_textcolor(RED);
					puts("IZarc cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			break;
		case 0:
			_textcolor(RED);
			puts("The file could not be identified by its extension.");
			switch(UE(IZARC, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				case FAILED:
					break;
				default:
					_textcolor(RED);
					puts("IZarc cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			if(!further)
				break;
			switch((dest[0] == L'\0')?UE(SZ, source):UE(SZslave, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				case FAILED:
					break;
				default:
					_textcolor(RED);
					puts("7-zip cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			break;
		default:
			if((argc != LZO)&&(extractto(source) == NULL))						//lzop also lists the contents
				break;
			switch(UE(argc, source))
			{
				case SUCCESS:
					further = 0;
					break;
				case ERROR:
					_textcolor(LIGHTRED);
					perror("\nAn error occured during processing");
					break;
				case FAILED:
					break;
				default:
					_textcolor(RED);
					puts("The program or tool cannot extract the file.");
					if(D)
						printf("(process returned %d)\n", Pret);
					break;
			}
			break;
	}

	while(further)
	{
		_textcolor(LIGHTGRAY);
		puts("\nProcess the file again using TrID analysis (default, any other key);\n\
Analyze the file with Exeinfo PE by A.S.L. (a);\n\
Analyze the file deeper with TrID (b);\n\
Try using IZArc for listing and extracting (c);\n\
Try using 7-zip for listing and extracting (d);\n\
Try using Game Archive UnPacker PRO (e);\n\
Try using Inno Setup Unpacker (f);\n\
Try using InstallExplorer plugin (g);\n\
Exit myUE (ESC/-)");
		Pret = _getch();
		putchar('\n');
		switch(Pret)
		{
			case 'A':
			case 'a':
				wcscpy(wcsrchr(prog, L'\\')+1, L"exeinfope.exe");
				wcscpy(wcsrchr(arg0, L'\\')+1, L"exeinfope.exe\"");
				if(D)
				{
					_textcolor(DARKGRAY);
					printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
				}
				_wspawnl(_P_NOWAIT, prog, arg0, source, NULL);
				break;
			case 'B':
			case 'b':
				wcscpy(wcsrchr(prog, L'\\')+1, L"trid.exe");
				wcscpy(wcsrchr(arg0, L'\\')+1, L"trid.exe\"");
				if(D)
				{
					_textcolor(DARKGRAY);
					printf("::spawning |%ls| |%ls| -v |%ls|\n", prog, arg0, source);
				}
				_textcolor(BROWN);
				_wspawnl(_P_WAIT, prog, arg0, L"-v", source, NULL);
				break;
			case 'C':
			case 'c':
				switch(UE(IZARC, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					case FAILED:
						break;
					default:
						_textcolor(RED);
						puts("IZarc cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			case 'D':
			case 'd':
				switch(UE(SZ, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					case FAILED:
						break;
					default:
						_textcolor(RED);
						puts("7-zip cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			case 'E':
			case 'e':
				if((dest[0] == L'\0')&&(extractto(source) == NULL))
					break;
				switch(UE(GAUP, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("Game Archive Unpacker PRO plugin cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			case 'F':
			case 'f':
				if((dest[0] == L'\0')&&(extractto(source) == NULL))
					break;
				switch(UE(INNO, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("Inno Setup Unpacker cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
			case 'G':
			case 'g':
				if((dest[0] == L'\0')&&(extractto(source) == NULL))
					break;
				switch(UE(INSTALLEXPLORER, source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("InstallExplorer plugin cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
			break;
			case '-':
			case 27:
				_textcolor(WHITE);
				return 1;
				break;
			case 0:
			case 224:
				_getch();
			default:
				if((dest[0] == L'\0')&&(extractto(source) == NULL))
					break;
				switch(UE(ext(TrIDanalysis(source)), source))
				{
					case SUCCESS:
						further = 0;
						break;
					case ERROR:
						_textcolor(LIGHTRED);
						perror("\nAn error occured during processing");
						break;
					default:
						_textcolor(RED);
						puts("The program or tool cannot extract the file.");
						if(D)
							printf("(process returned %d)\n", Pret);
						break;
				}
				break;
		}
	}
	_textcolor(LIGHTGREEN);
	puts("\nExtraction process completed successfully.\nPress any key to exit...");
	Pret = _getch();
	if((Pret == 0)||(Pret == 224))
		_getch();
	_textcolor(WHITE);
	return 0;
}

char ext(const wchar_t input[])
{
	if(_wcsistr(input, L".LIT") != NULL)
		return LIT;
	if(_wcsistr(input, L".DBX") != NULL)
		return DBX;
	if((_wcsistr(input, L".MHT") != NULL)||(_wcsistr(input, L".MHTML") != NULL))
		return MHT;
/*	if(_wcsistr(input, L".SIT") != NULL)
		return SIT;*///stopped support, the format is outdated and used allmost entirely on Mac
	if((_wcsistr(input, L".HLP") != NULL)||(_wcsistr(input, L".MVB") != NULL)||(_wcsistr(input, L".ANN") != NULL)||(_wcsistr(input, L".CAC") != NULL)||(_wcsistr(input, L".AUX") != NULL))
		return HLP;
	if(_wcsistr(input, L".CHM") != NULL)
		return CHM;
	if((_wcsistr(input, L".KGB") != NULL)||(_wcsistr(input, L".KGE") != NULL))
		return KGB;
	if((_wcsistr(input, L".LZO") != NULL)||(_wcsistr(input, L".TZO") != NULL))
		return LZO;
	if(_wcsistr(input, L".NBH") != NULL)
		return NBH;
/*	if(_wcsistr(input, L".PEA") != NULL)
		return PEA;*/// problem with pea -> need gui for this
	if(_wcsistr(input, L".UHA") != NULL)
		return UHA;
/*	if(_wcsistr(input, L".LZX") != NULL)
		return LZX;*///stopped support, the format is outdated
	if(_wcsistr(input, L".EXE") != NULL)
		return EXE;
	if(_wcsistr(input, L".DLL") != NULL)
		return DLL;
	if(_wcsistr(input, L".LIB") != NULL)
		return LIB;
	if((_wcsistr(input, L".MSI") != NULL)||(_wcsistr(input, L".MSM") != NULL)||(_wcsistr(input, L".MSP") != NULL))
		return MSI;
	if(_wcsistr(input, L".7Z") != NULL)
		return SZ;
	if(_wcsistr(input, L".CAB") != NULL)
		return CAB;
	if(_wcsistr(input, L".FB2") != NULL)
		return FB2;
	if(_wcsistr(input, L".GIF") != NULL)
		return GIF;
	if(_wcsistr(input, L".EPUB") != NULL)
		return EPUB;
	if(_wcsistr(input, L".PDF") != NULL)
		return PDF;
	if(_wcsistr(input, L".SWF") != NULL)
		return SWF;
	if((_wcsistr(input, L".DOC") != NULL)&&(_wcsistr(input, L".DOCX") == NULL))
		return DOC;
	if((_wcsistr(input, L".XLS") != NULL)&&(_wcsistr(input, L".XLSX") == NULL))
		return XLS;
	if((_wcsistr(input, L".PPT") != NULL)&&(_wcsistr(input, L".PPTX") == NULL))
		return PPT;
	if((_wcsistr(input, L".AVI") != NULL)||(_wcsistr(input, L".WMV") != NULL)||(_wcsistr(input, L".MP4") != NULL)||(_wcsistr(input, L".MKV") != NULL)||(_wcsistr(input, L".MPG") != NULL)||(_wcsistr(input, L".MPEG") != NULL)||(_wcsistr(input, L".XVID") != NULL))
		return VIDEO;
	return 0;
}

wchar_t *TrIDanalysis(const wchar_t source[])
{
	int saved_stdout = _dup(_fileno(stdout));
	int handles[2];
	_pipe(handles, (74-1)*sizeof(char), _O_TEXT | _O_NOINHERIT);
	_dup2(handles[1], _fileno(stdout));
	_close(handles[1]);
	_textcolor(WHITE);
	swprintf(prog, maxlen, L"%ls\\bin\\trid.exe", thispath);
	swprintf(arg0, maxlen+2, L"\"%ls\\bin\\trid.exe\"", thispath);
	if(D)
	{
		_textcolor(DARKGRAY);
		printf("::spawning |%ls| |%ls| -r:01 |%ls|\n", prog, arg0, source);
	}
	Pid = _wspawnl(_P_NOWAIT, prog, arg0, L"-r:01", source, NULL);
	if(Pid == -1)
	{
		handles[1] = errno;
		_dup2(saved_stdout, _fileno(stdout));
		_close(saved_stdout);
		_close(handles[0]);
		errno = handles[1];
		return NULL;
	}
	_dup2(saved_stdout, _fileno(stdout));
	_close(saved_stdout);
	int read;
	char buffer[75];
	do
	{
		read = _read(handles[0], buffer, (74-1)*sizeof(char));
		buffer[read] = '\0';
	} while((strchr(buffer, '%') == NULL)&&(read != -1)&&(read != 0));
	_close(handles[0]);
	if(read == 0)
	{
		_textcolor(RED);
		puts("TrID detection failed!\nThis might not be a binary file.\n");
		if(D)
			puts("(last read from TrID output returned 0 (bytes))");
		return NULL;
	}
	if(read == -1)
	{
		_textcolor(LIGHTRED);
		puts("TrID detection failed!");
		if(D)
			perror("(error while reading TrID output)");
		return NULL;
	}
	*strrchr(buffer, ' ') = '\0';
	_textcolor(BROWN);
	printf("TrID detected %s\n", buffer+(buffer[0] == ' '));
	mbstowcs(temp, buffer, 75);
	return temp;
}

int UE(const char todo, const wchar_t source[])
{
	errno = 0;
	swprintf(prog, maxlen, L"%ls\\bin\\", thispath);
	swprintf(arg0, maxlen+2, L"\"%ls\\bin\\", thispath);
	isfile = 0;
	switch(todo)
	{
		case VIDEO:
			_textcolor(GREEN);
			puts("TIP: To manipulate videos, use a tool like Avidemux");
			return SUCCESS;
			break;
		case SZslave:
			wcscat(prog, L"7z.exe");
			wcscat(arg0, L"7z.exe\"");
			wmemmove(dest+2, dest, wcslen(dest)+1);
			dest[0] = L'-';	dest[1] = L'o';
			_textcolor(GREEN);
			puts("Trying to extract with 7-zip...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| x |%ls| |%ls|\n", prog, arg0, dest, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"x", dest, source, NULL);
			if(errno)
				return ERROR;
			wmemmove(dest, dest+2, wcslen(dest+2)+1);
			if((Pret == 0)||(Pret == 255))
				return SUCCESS;
			else
				return FAILED*(FAILED+Pret);
			break;
		case SZ:
			wcscat(prog, L"7z.exe");
			wcscat(arg0, L"7z.exe\"");
			_textcolor(YELLOW);
			puts("Trying to list archive contents with 7-zip...\nPRESS ANY KEY TO ABORT LISTING");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| l |%ls|\n", prog, arg0, source);
			}
			{
			int saved_stdout = _dup(_fileno(stdout));
			int handles[2];
			_pipe(handles, 512*sizeof(char), _O_TEXT | _O_NOINHERIT);
			_dup2(handles[1], _fileno(stdout));
			_close(handles[1]);
			_textcolor(BROWN);
			Pid = _wspawnl(_P_NOWAIT, prog, arg0, L"l", source, NULL);
			if(Pid == -1)
			{
				handles[1] = errno;
				_dup2(saved_stdout, _fileno(stdout));
				_close(saved_stdout);
				_close(handles[0]);
				errno = handles[1];
				return ERROR;
			}
			_dup2(saved_stdout, _fileno(stdout));
			_close(saved_stdout);
			int read;
			char buffer[512];
			do
			{
				read = _read(handles[0], buffer, 512*sizeof(char));
				if(read != 512)
					buffer[read] = '\0';
				printf("%s", buffer);
			} while((!_kbhit())&&(read != -1)&&(read != 0));
			_close(handles[0]);
			}
			Pret = 0;
			if(!_kbhit())
				_cwait(&Pret, Pid, 0);
			if((Pret != 0)&&(Pret != 255))
				return FAILED*(FAILED+Pret);
			if(extractto(source) == NULL)
				return FAILED;
			return UE(SZslave, source);
			break;
		case IZARC:
			wcscat(prog, L"izarce.exe");
			wcscat(arg0, L"izarce.exe\"");
			_textcolor(YELLOW);
			puts("Trying to list archive contents with IZArc Command Line Add-on...\nPRESS ANY KEY TO ABORT LISTING");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -v |%ls|\n", prog, arg0, source);
			}
			{
			int saved_stdout = _dup(_fileno(stdout));
			int handles[2];
			_pipe(handles, 512*sizeof(char), _O_TEXT | _O_NOINHERIT);
			_dup2(handles[1], _fileno(stdout));
			_close(handles[1]);
			_textcolor(BROWN);
			Pid = _wspawnl(_P_NOWAIT, prog, arg0, L"-v", source, NULL);
			if(Pid == -1)
			{
				handles[1] = errno;
				_dup2(saved_stdout, _fileno(stdout));
				_close(saved_stdout);
				_close(handles[0]);
				errno = handles[1];
				return ERROR;
			}
			_dup2(saved_stdout, _fileno(stdout));
			_close(saved_stdout);
			int read;
			char buffer[512];
			do
			{
				read = _read(handles[0], buffer, 512*sizeof(char));
				if(read != 512)
					buffer[read] = '\0';
				printf("%s", buffer);
			} while((!_kbhit())&&(read != -1)&&(read != 0));
			_close(handles[0]);
			}
			if(!_kbhit())
				_cwait(&Pret, Pid, 0);
			if((_kbhit()||(Pret == 0))&&(extractto(source) != NULL))
			{
				wmemmove(dest+2, dest, wcslen(dest)+1);
				dest[0] = L'-';	dest[1] = L'p';
				if(D)
				{
					_textcolor(DARKGRAY);
					printf("::spawning |%ls| |%ls| -e -d |%ls| |%ls|\n", prog, arg0, dest, source);
				}
				_textcolor(WHITE);
				Pret = _wspawnl(_P_WAIT, prog, arg0, L"-e", L"-d", dest, source, NULL);
				if(Pret == 0)
					return SUCCESS;
				_textcolor(RED);
				puts("IZarc Command Line Add-on cannot extract the file correctly.\n");
			}
			_textcolor(DARKGRAY);
			puts("Open the Izarc2Go dialog for extraction? ['Y'/other]");
			{
			int choice = _getch();
			if((choice == 0)||(choice == 224))
				_getch();
			if((choice != 'Y')&&(choice != 'y'))
				return FAILED;
			}
			wcscpy(wcsrchr(prog, L'.')-1, L"2go.exe");
			wcscpy(wcsrchr(arg0, L'.')-1, L"2go.exe\"");
			_textcolor(GREEN);
			puts("Opening the IZarc2Go dialog for extraction.\nUse the dialog to try to extract the file.");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			errno = 0;
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			if(errno)
				return ERROR;
			_textcolor(DARKGRAY);
			puts("Was the extraction succesfull? ['Y'/other]");
			{
			int choice = _getch();
			if((choice == 0)||(choice == 224))
				_getch();
			if((choice == 'Y')||(choice == 'y'))
				return SUCCESS;
			}
			return FAILED*(FAILED+Pret);
			break;
		case LIT:
			wcscpy(wcsrchr(dest, L'\"'), L"\\\"");
			wcscat(prog, L"clit.exe");
			wcscat(arg0, L"clit.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with ConvertLIT...\nTIP: clit.exe can also convert a LIT");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| |%ls|\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, dest, NULL);
			*wcsrchr(dest, L'\\') = L'\0';
			break;
		case GIF:
			wcscat(prog, L"gif2frames.exe");
			wcscat(arg0, L"gif2frames.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with gif2frames...");
			*wcsrchr(dest, L'\"') = L'\0';
			_wmkdir(dest+1);
			_wchdir(dest+1);
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| -png -unique\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, L"-png", L"-unique", NULL);
			_wchdir(L"..");
			break;
		case EPUB:
			wcscat(prog, L"izarce.exe");
			wcscat(arg0, L"izarce.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with IZarc Command Line Add-on...");
			wmemmove(dest+2, dest, wcslen(dest)+1);
			dest[0] = L'-';	dest[1] = L'p';
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -e -d |%ls| |%ls|\n", prog, arg0, dest, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-e", L"-d", dest, source, NULL);
			if(errno)
				return ERROR;
			if(Pret != 0)
			{
				_textcolor(RED);
				puts("IZarc Command Line Add-on cannot extract the file correctly.\n");
				wmemmove(dest, dest+2, wcslen(dest+2)+1);
				if(UE(SZslave, source) != SUCCESS)
					return FAILED*(FAILED+Pret);
			}
			wcscpy(wcsrchr(prog, L'\\')+1, L"balabolka_text.exe");
			wcscpy(wcsrchr(arg0, L'\\')+1, L"balabolka_text.exe\"");
			wcscat(wcsrchr(dest, L'\"'), L"\\all_text.txt\"");
			_textcolor(GREEN);
			puts("Trying to extract the text with Balabolka (Text Extract Utility)...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -f |%ls| -p |%ls| -u -e utf8\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-f", source, L"-p", dest, L"-u", L"-e", L"utf8", NULL);
			return SUCCESS;
			break;
		case PDF:
			wcscat(prog, L"pdfdetach.exe");
			wcscat(arg0, L"pdfdetach.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract embedded files with (PDF) document embedded file extractor...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -saveall -o |%ls| |%ls|\n", prog, arg0, dest, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-saveall", L"-o", dest, source, NULL);
			wcscpy(wcsrchr(prog, L'\\')+1, L"balabolka_text.exe");
			wcscpy(wcsrchr(arg0, L'\\')+1, L"balabolka_text.exe\"");
			wcscat(wcsrchr(dest, L'\"'), L"\\all_text.txt\"");
			_textcolor(GREEN);
			puts("Trying to extract the text with Balabolka (Text Extract Utility)...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -f |%ls| -p |%ls| -u -e utf8\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-f", source, L"-p", dest, L"-u", L"-e", L"utf8", NULL);
			wcscpy(wcsrchr(prog, L'\\')+1, L"pdfimages.exe");
			wcscpy(wcsrchr(arg0, L'\\')+1, L"pdfimages.exe\"");
			*wcsrchr(dest, L'\\') = L'\0';
			_wmkdir(dest+1);
			_wchdir(dest+1);
			_textcolor(GREEN);
			puts("Trying to extract images with (PDF) image extractor...\nTIP: For more operations on pdf's, check out\nhttp://www.pdftk.com or http://www.online2pdf.com");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -j |%ls| img\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-j", source, L"img", NULL);
			_wchdir(L"..");
			break;
		case DBX:
			swprintf(temp, MAX_TEMP, L"\"%ls\\bin\\dbxplug.wcx\"", thispath);
			wcscat(prog, L"cmdtotal.exe");
			wcscat(arg0, L"cmdtotal.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with DBX plugin...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| x |%ls| |%ls|\n", prog, arg0, temp, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, temp, L"x", source, dest, NULL);
			*wcsrchr(dest, L'\"') = L'\0';
			break;
		case FB2:
			wcscpy(wcsrchr(dest, L'\"'), L"\\\"");
			swprintf(temp, MAX_TEMP, L"\"%ls\\bin\\wcx_fb2.wcx\"", thispath);
			wcscat(prog, L"cmdtotal.exe");
			wcscat(arg0, L"cmdtotal.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with fb2 plugin...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| x |%ls| |%ls|\n", prog, arg0, temp, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, temp, L"x", source, dest, NULL);
			wcscpy(wcsrchr(prog, L'\\')+1, L"balabolka_text.exe");
			wcscpy(wcsrchr(arg0, L'\\')+1, L"balabolka_text.exe\"");
			wcscat(wcsrchr(dest, L'\"'), L"\\all_text.txt\"");
			_textcolor(GREEN);
			puts("Trying to extract the text with Balabolka (Text Extract Utility)...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -f |%ls| -p |%ls| -u -e utf8\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-f", source, L"-p", dest, L"-u", L"-e", L"utf8", NULL);
			*wcsrchr(dest, L'\\') = L'\0';
			break;
		case MHT:
			wcscat(prog, L"extractmht.exe");
			wcscat(arg0, L"extractmht.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with ExtractMHT...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| |%ls|\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, dest, NULL);
			wcscpy(wcsrchr(prog, L'\\')+1, L"balabolka_text.exe");
			wcscpy(wcsrchr(arg0, L'\\')+1, L"balabolka_text.exe\"");
			wcscat(wcsrchr(dest, L'\"'), L"\\all_text.txt\"");
			_textcolor(GREEN);
			puts("Trying to extract the text with Balabolka (Text Extract Utility)...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -f |%ls| -p |%ls| -u -e utf8\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-f", source, L"-p", dest, L"-u", L"-e", L"utf8", NULL);
			*wcsrchr(dest, L'\\') = L'\0';
			break;
		case HLP:
			wcscat(prog, L"helpdeco.exe");
			wcscat(arg0, L"helpdeco.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with HelpDecompiler...");
			*wcsrchr(dest, L'\"') = L'\0';
			_wmkdir(dest+1);
			_wchdir(dest+1);
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			_wchdir(L"..");
			break;
		case CHM:
			wcscat(prog, L"chmdecoder.exe");
			wcscat(arg0, L"chmdecoder.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract witch CHM Decoder...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| |%ls|\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, dest, NULL);
			wcscpy(wcsrchr(prog, L'\\')+1, L"balabolka_text.exe");
			wcscpy(wcsrchr(arg0, L'\\')+1, L"balabolka_text.exe\"");
			wcscat(wcsrchr(dest, L'\"'), L"\\all_text.txt\"");
			_textcolor(GREEN);
			puts("Trying to extract the text with Balabolka (Text Extract Utility)...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -f |%ls| -p |%ls| -u -e utf8\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-f", source, L"-p", dest, L"-u", L"-e", L"utf8", NULL);
			*wcsrchr(dest, L'\\') = L'\0';
			break;
		case KGB:
			wcscat(prog, L"kgb2_console.exe");
			wcscat(arg0, L"kgb2_console.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with KGB Archiver 2 beta 1...");
			*wcsrchr(dest, L'\"') = L'\0';
			_wmkdir(dest+1);
			_wchdir(dest+1);
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			_wchdir(L"..");
			break;
		case NBH:
			wcscat(prog, L"nbhextract.exe");
			wcscat(arg0, L"nbhextract.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with NBHextract...");
			*wcsrchr(dest, L'\"') = L'\0';
			_wmkdir(dest+1);
			_wchdir(dest+1);
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			_wchdir(L"..");
			break;
		case UHA:
			wcscat(prog, L"unuharc06.exe");
			wcscat(arg0, L"unuharc06.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with UnUHARC...");
			wmemmove(dest+2, dest, wcslen(dest)+1);
			dest[0] = L'-';	dest[1] = L't';
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| x |%ls| |%ls|\n", prog, arg0, dest, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"x", dest, source, NULL);
			*wcsrchr(dest, L'\"') = L'\0';
			wmemmove(dest, dest+2, wcslen(dest+2)+1);
			break;
		case LZO:
			wcscat(prog, L"lzop.exe");
			wcscat(arg0, L"lzop.exe\"");
			_textcolor(YELLOW);
			puts("Trying to list archive contents with lzop...\nPRESS ANY KEY TO ABORT LISTING");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -l |%ls|\n", prog, arg0, source);
			}
			{
			int saved_stdout = _dup(_fileno(stdout));
			int handles[2];
			_pipe(handles, 512*sizeof(char), _O_TEXT | _O_NOINHERIT);
			_dup2(handles[1], _fileno(stdout));
			_close(handles[1]);
			_textcolor(BROWN);
			Pid = _wspawnl(_P_NOWAIT, prog, arg0, L"-l", source, NULL);
			if(Pid == -1)
			{
				handles[1] = errno;
				_dup2(saved_stdout, _fileno(stdout));
				_close(saved_stdout);
				_close(handles[0]);
				errno = handles[1];
				return ERROR;
			}
			_dup2(saved_stdout, _fileno(stdout));
			_close(saved_stdout);
			int read;
			char buffer[512];
			do
			{
				read = _read(handles[0], buffer, 512*sizeof(char));
				if(read != 512)
					buffer[read] = '\0';
				printf("%s", buffer);
			} while((!_kbhit())&&(read != -1)&&(read != 0));
			_close(handles[0]);
			}
			Pret = 0;
			if(!_kbhit())
				_cwait(&Pret, Pid, 0);
			if(Pret)
				return FAILED*(FAILED+Pret);
			if(extractto(source) == NULL)
				return FAILED;
			_textcolor(GREEN);
			puts("Trying to extract with lzop...");
			wmemmove(dest+2, dest, wcslen(dest)+1);
			dest[0] = L'-';	dest[1] = L'p';
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -d -P |%ls| |%ls|\n", prog, arg0, dest, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-d", L"-P", dest, source, NULL);
			if(Pret)
				return FAILED*(FAILED+Pret);
			return SUCCESS;
			break;
		case SWF:
			wcscat(prog, L"officewiz.exe");
			wcscat(arg0, L"officewiz.exe\"");
			_textcolor(GREEN);
			puts("Opening the Office Image Extraction Wizard dialog for extraction.\nUse the dialog to try to extract the file.");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			errno = 0;
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			if(errno)
				return ERROR;
			_textcolor(DARKGRAY);
			puts("Was the extraction succesfull? ['Y'/other]");
			{
			int choice = _getch();
			if((choice == 0)||(choice == 224))
				_getch();
			if((choice == 'Y')||(choice == 'y'))
				return SUCCESS;
			}
			return FAILED*(FAILED+Pret);
			break;
		case XLS:
			wcscat(prog, L"xls2x.exe");
			wcscat(arg0, L"xls2x.exe\"");
			_textcolor(GREEN);
			puts("Trying to convert to xlsx with xls2x...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			wcscpy(dest, source);
			wcscpy(wcsrchr(dest, L'\"'), L"x\"");
			isfile = 1;
			break;
		case DOC:
			wcscat(prog, L"doc2x.exe");
			wcscat(arg0, L"doc2x.exe\"");
			_textcolor(GREEN);
			puts("Trying to convert to docx with doc2x...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			wcscpy(dest, source);
			wcscpy(wcsrchr(dest, L'\"'), L"x\"");
			isfile = 1;
			break;
		case PPT:
			wcscat(prog, L"ppt2x.exe");
			wcscat(arg0, L"ppt2x.exe\"");
			_textcolor(GREEN);
			puts("Trying to convert to pptx with ppt2x...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			wcscpy(dest, source);
			wcscpy(wcsrchr(dest, L'\"'), L"x\"");
			isfile = 1;
			break;
/*		case LZX:
			wcscat(prog, L"unlzx.exe");
			wcscat(arg0, L"unlzx.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with UnLZX...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -x |%ls| -o |%ls|\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pid = _wspawnl(_P_NOWAIT, prog, arg0, L"-x", source, L"-o", dest, NULL);
			break;*/	//dropped support, because of old age of Amiga LZX compression
		case UPX:
			wcscat(prog, L"upx.exe");
			wcscat(arg0, L"upx.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with Ultimate Packer for eXecutables...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -d |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-d", source, NULL);
			if(errno)
				return ERROR;
			if(!Pret)
				return SUCCESS;
			return FAILED*(FAILED+Pret);
			break;
		case ASPACK1:
			wcscat(prog, L"aspackdie.exe");
			wcscat(arg0, L"aspackdie.exe\"");
			wcscpy(dest, source);
			wcscpy(wcschr(dest, L'.'), L"_.exe");
			_textcolor(GREEN);
			puts("Trying to extract with AspackDie...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| |%ls|\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, dest, NULL);
			isfile = 1;
			break;
		case ASPACK2:
			wcscat(prog, L"caspr.exe");
			wcscat(arg0, L"caspr.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with CASPR...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| -P |%ls|\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, L"-P", dest, NULL);
			isfile = 1;
			break;
		case PECOMPACT:
			wcscat(prog, L"unpacker_pecompact.exe");
			wcscat(arg0, L"unpacker_pecompact.exe\"");
			wcscpy(dest, source);
			wcscpy(wcschr(dest, L'.'), L"_.exe");
			_textcolor(GREEN);
			puts("Trying to extract with Unpacker PECompact...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -c |%ls| |%ls|\n", prog, arg0, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-c", source, dest, NULL);
			isfile = 1;
			break;
		case INNO:
			wcscat(prog, L"innounp.exe");
			wcscat(arg0, L"innounp.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with Inno Setup Unpacker...");
			wmemmove(dest+2, dest, wcslen(dest)+1);
			dest[0] = L'-';	dest[1] = L'd';
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| -x -m -a -q -y |%ls| |%ls|\n", prog, arg0, dest, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, L"-x", L"-m", L"-a", L"-q", L"-y", dest, source, NULL);
			wmemmove(dest, dest+2, wcslen(dest+2)+1);
			*wcsrchr(dest, L'\"') = L'\0';
			break;
		case INSTALLSHIELD:
			wcscat(prog, L"isxunpack.exe");
			wcscat(arg0, L"isxunpack.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with InstallShield (By One Exe-file) Unpacker...");
			*wcsrchr(dest, L'\"') = L'\0';
			_wmkdir(dest+1);
			_wchdir(dest+1);
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			{
			int saved_stdout = _dup(_fileno(stdout));
			int handles[2];
			_pipe(handles, (74-1)*sizeof(char), _O_TEXT | _O_NOINHERIT);
			_dup2(handles[1], _fileno(stdout));
			_close(handles[1]);
			Pid = _wspawnl(_P_NOWAIT, prog, arg0, source, NULL);
			_dup2(saved_stdout, _fileno(stdout));
			_close(saved_stdout);
			if(Pid == -1)
			{
				_close(handles[0]);
				wcscat(dest, L"\"");
				return ERROR;
			}
			else
			{
				int read;
				char buffer[75];
				int i = 0;
				do
				{
					read = _read(handles[0], buffer+i, (75-1)*sizeof(char)-i*sizeof(char));
					i += read;
					buffer[i] = '\0';
					if(i >= (75-2))
						i = 0;
					if(strchr(buffer, '\n') != NULL)
					{
						memmove(buffer, strrchr(buffer, '\n')+1, strlen(strrchr(buffer, '\n')+1)+1);
						i = strlen(buffer);
					}
					printf("%s", buffer);
				} while((strstr(buffer, "Press Enter") == NULL)&&(read != -1)&&(read != 0));
				_close(handles[0]);
				system("taskkill /f /im isxunpack.exe");
				errno = 0;
			}
			}
			_wchdir(L"..");
			break;
		case INSTALLEXPLORER:
			wcscat(prog, L"cmdtotal.exe");
			wcscat(arg0, L"cmdtotal.exe\"");
			swprintf(temp, MAX_TEMP, L"\"%ls\\bin\\instexpl.wcx\"", thispath);
			_textcolor(GREEN);
			puts("Trying to extract with InstallExplorer plugin...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| x |%ls| |%ls|\n", prog, arg0, temp, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, temp, L"x", source, dest, NULL);
			*wcsrchr(dest, L'\"') = L'\0';
			break;
		case SETUPFACTORY:
			wcscat(prog, L"suf.exe");
			wcscat(arg0, L"suf.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with Setup Factory unpacker to the subfolder 'unpacked' of the entered folder...");
			*wcsrchr(dest, L'\"') = L'\0';
			_wmkdir(dest+1);
			_wchdir(dest+1);
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, source);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, source, NULL);
			putchar('\n');
			_wchdir(L"..");
			break;
		case MSI:
			wcscat(prog, L"jsmsix.exe");
			wcscat(arg0, L"jsmsix.exe.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with MSI Unpacker from JSWare...");
			swprintf(temp, MAX_TEMP, L"%ls|%ls", source, dest);
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls|\n", prog, arg0, temp);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, temp, NULL);
			*wcsrchr(dest, L'\"') = L'\0';
			break;
		case ISHIELDCAB:
			swprintf(temp, MAX_TEMP, L"\"%ls\\bin\\ishield.wcx\"", thispath);
			wcscat(prog, L"cmdtotal.exe");
			wcscat(arg0, L"cmdtotal.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with IShield plugin...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| x |%ls| |%ls|\n", prog, arg0, temp, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, temp, L"x", source, dest, NULL);
			*wcsrchr(dest, L'\"') = L'\0';
			break;
/*		case PEA:
			read = -1;
			i = ERROR/FAILED-FAILED;
			break;*/
		case GAUP:
			swprintf(temp, MAX_TEMP, L"\"%ls\\bin\\gaup_pro.wcx\"", thispath);
			wcscat(prog, L"cmdtotal.exe");
			wcscat(arg0, L"cmdtotal.exe\"");
			_textcolor(GREEN);
			puts("Trying to extract with Game Archive Unpacker PRO plugin...");
			if(D)
			{
				_textcolor(DARKGRAY);
				printf("::spawning |%ls| |%ls| |%ls| x |%ls| |%ls|\n", prog, arg0, temp, source, dest);
			}
			_textcolor(WHITE);
			Pret = _wspawnl(_P_WAIT, prog, arg0, temp, L"x", source, dest, NULL);
			*wcsrchr(dest, L'\"') = L'\0';
			break;
	}
	if(errno)
		return ERROR;
	if(isfile)
	{
		FILE *file = _wfopen(dest, L"r");
		if(file != NULL)
		{
			fclose(file);
			return SUCCESS;
		}
		return FAILED*(FAILED+Pret);
	}
	_wrmdir(dest+1);
	wcscat(dest, L"\"");
	if(errno == ENOTEMPTY)
		return SUCCESS;
	return FAILED*(FAILED+Pret);
}

wchar_t * extractto(const wchar_t source[])
{
	_textcolor(LIGHTGRAY);
	puts("\nExtract to a subfolder (a);\nExtract inside folder containing the archive (b);\nExtract to a specified folder (c)");
	Pret = _getch();
	if((Pret == 0)||(Pret == 224))
		_getch();
	wcscpy(dest, source);
	switch(Pret)
	{
		case 'a':
			if(wcschr(dest, L'.') != NULL)
				wcscpy(wcsrchr(dest, L'.'), L"\"");
			else
				wcscpy(wcsrchr(dest, L'\"'), L"_\"");
			break;
		case 'b':
			wcscpy(wcsrchr(dest, L'\\'), L"\"");
			break;
		case 'c':
			puts("Extract to this folder (full path):");
			dest[0] = L'\"';
			EnterPathToDir(dest+1, MAX_TEMP-4);
			if(dest[1] == L'\0')
				return NULL;
			wcscat(dest, L"\"");
			break;
		default:
			dest[0] = L'\0';
			return NULL;
	}
	putchar('\n');
	return dest;
}

char install(const wchar_t name[])
{
	_textcolor(WHITE);
	puts("For installation: is this program running as administrator? [SPACE]\nFor portable (console) use instructions [any other key]");
	wchar_t comm[wcslen(name)+wcslen(thispath)+20];
	int choice = _getch();
	if(choice == ' ')
	{
		swprintf(comm, wcslen(name)+wcslen(thispath)+20, L"\"\\\"%ls\\%ls\\\" \\\"%%1\\\"\"", thispath, name);
		D = (char) _wspawnlp(_P_WAIT, L"reg.exe", L"reg.exe", L"add", L"\"HKEY_CLASSES_ROOT\\*\\shell\\Process with myUE\\command\"", L"/d", comm, L"/f", NULL);
		puts("Add a context menu item for directories to open IZArc2Go? [SPACE]");
		choice = _getch();
		if(choice == ' ')
		{
			swprintf(comm, wcslen(name)+wcslen(thispath)+20, L"\"%ls\\bin\\IZArc2Go.exe\"", thispath);
			return D + (char) _wspawnlp(_P_WAIT, L"reg.exe", L"reg.exe", L"add", L"\"HKEY_CLASSES_ROOT\\Directory\\shell\\Open IZArc2Go\\command\"", L"/d", comm, L"/f", NULL);
		}
		if((choice == 0)||(choice == 224))
			_getch();
		return D;
	}
	if((choice == 0)||(choice == 224))
		_getch();
	puts("\nmyUE.exe <file to extract> [debug]\n\n<file to extract>: Complete path to the file that needs to be extracted\n                   (including file extention).\ndebug: optional parameter that will result in more standard output.");
	return 0;
}

char Qdir(struct _wfinddata64_t *result, const wchar_t source[], int i, void *data)
{
	if((*result).attrib&_A_SUBDIR)
		D = -1;
	return 0;
}

