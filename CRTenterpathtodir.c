char eptd_todo = eptd_DIR;
int eptd_nodir, eptd_take, eptd_list = 0;

void _EnterPathToDir(char to[], int max)
{
#ifdef BLACKBCKGR
	_textcolor(LIGHTGRAY);
#else
	_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
	puts("(no trailing '\\' necessary, '\\' may be replaced by '<' and path completion with [ENTER] possible)");
#else
	puts("(eindigende '\\' niet nodig, '\\' mag vervangen worden door '<' en padaanvulling met [ENTER] mogelijk)");
#endif
	int i, k = _wherey();
	eptd_todo = eptd_START;
	do
	{
		if(eptd_todo == eptd_NODIR)
		{
			while(k != _wherey())
			{
				_gotoxy(1, _wherey()-1);
				_clreol();
			}
			_textcolor(LIGHTRED);
#ifdef ENGLISH
			puts("The path entered is the path to a file, not a directory.\nTyp another one or [ENTER] to abort.");
#else
			puts("Het opgegeven pad is er een naar een bestand, geen map.\nGeef een nieuwe in of [ENTER] om af te breken.");
#endif
		}
		if((eptd_todo == eptd_START)||(eptd_todo == eptd_NODIR))
		{
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			while(*(strrchr(fgets(to, max, stdin), '\0')-1) != 10)
			{
				while(*(strrchr(fgets(to, max, stdin), '\0')-1) != 10);
				while(k != _wherey())
				{
					_gotoxy(1, _wherey()-1);
					_clreol();
				}
				_textcolor(LIGHTRED);
#ifdef ENGLISH
				puts("The given path is too long to process.\nTyp another one or [ENTER] to abort.");
#else
				puts("Het opgegeven pad is te lang om te verwerken.\nGeef een nieuwe in of [ENTER] om af te breken.");
#endif
#ifdef BLACKBCKGR
				_textcolor(WHITE);
#else
				_textcolor(BLACK);
#endif
			}
			*(strrchr(to, '\0')-1) = '\0';
			if(to[0] == '\0')
				return;
			if((to[1] == ':')&&(to[2] == '\0'))
				return;
			for(i=0; to[i] != '\0'; i++)
			{
				if(to[i] == '<')
					to[i] = '\\';
			}
		}
		eptd_todo = eptd_LISTdir;
		eptd_nodir = 0;
		i = _walktree(to, max, "", _eptd_found, w_COMPLETE, 0, (void *)to);
		if(!i)
		{
			_textcolor(LIGHTRED);
			perror(NULL);
			to[0] = '\0';
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			return;
		}
		if(i%2)
		{
#ifdef BLACKBCKGR
			_textcolor(LIGHTGRAY);
#else
			_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
			puts("((The search also encountered paths that are too long to process))");
#else
			puts("((Er werden ook paden gevonden die te lang zijn om te verwerken))");
#endif
		}
		i = (i/2)-2*(i/2)*(i<0);
		if(i == 1)
			eptd_take = 'a';
		else
		{
#ifdef BLACKBCKGR
			_textcolor(LIGHTCYAN);
#else
			_textcolor(LIGHTBLUE);
#endif
			printf("[ESC]: ");
#ifdef BLACKBCKGR
			_textcolor(LIGHTGRAY);
#else
			_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
			puts("Cancel");
#else
			puts("Annuleren");
#endif
			if(i == 26*(eptd_list+1)+eptd_nodir)
			{
#ifdef BLACKBCKGR
				_textcolor(LIGHTCYAN);
#else
				_textcolor(LIGHTBLUE);
#endif
				printf("[TAB]: ");
#ifdef BLACKBCKGR
				_textcolor(LIGHTGRAY);
#else
				_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
				puts("Show next series of found paths");
#else
				puts("Volgende reeks gevonden paden");
#endif
			}
			eptd_take = _getch();
			if((eptd_take == 224)||(eptd_take == 0))
				_getch();
		}
		if((eptd_take == 9)&&(!(i - 26*(eptd_list+1) - eptd_nodir)))
		{
			eptd_list++;
			for(eptd_take=0; eptd_take != 28; eptd_take++)
			{
				_gotoxy(1, _wherey()-1);
				_clreol();
			}
		}
		else
		{
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			if((eptd_take >= 'a')&&(eptd_take <= i - 26*eptd_list - eptd_nodir + 'a' - 1))
			{
				eptd_todo = eptd_TAKEdir;
				eptd_nodir = 0;
				_walktree(to, max, "", _eptd_found, w_COMPLETE, 0, (void *)to);
			}
			else
			{
				to[0] = '\0';
				return;
			}
			eptd_list = 0;
			while(k != _wherey())
			{
				_gotoxy(1, _wherey()-1);
				_clreol();
			}
			printf("%s", to);
			if(*(strrchr(fgets(strrchr(to, '\0'), max-strlen(to), stdin), '\0')-1) != 10)
			{
				_textcolor(LIGHTRED);
#ifdef ENGLISH
				puts("The given path is too long to process.");
#else
				puts("Het opgegeven pad is te lang om te verwerken.");
#endif
				while(*(strrchr(fgets(to, max, stdin), '\0')-1) != 10);
				to[0] = '\0';
#ifdef BLACKBCKGR
				_textcolor(WHITE);
#else
				_textcolor(BLACK);
#endif
				return;
			}
			*(strrchr(to, '\0')-1) = '\0';
			for(i=0; to[i] != '\0'; i++)
			{
				if(to[i] == '<')
					to[i] = '\\';
			}
		}
		eptd_todo = eptd_CHECKdir;
	} while((!_walktree(to, 0, "", _eptd_found, w_CHECK, 0, (void *)to))||(eptd_todo == eptd_NODIR));
	return;
}

char _eptd_found(struct _finddata64_t *result, const char source[], int i, void *to)
{
	i = (i/2) - 2*(i/2)*(i<0) + 'a' - 1;
	if(eptd_todo == eptd_CHECKdir)
	{
		if((*result).attrib&_A_SUBDIR)
			eptd_todo == eptd_DIR;
		else
			eptd_todo == eptd_NODIR;
		return 0;
	}
	if(eptd_todo == eptd_LISTdir)
	{
		if(!((*result).attrib&_A_SUBDIR))
		{
			eptd_nodir++;
			return 1;
		}
		i -= eptd_list*26;
		i -= eptd_nodir;
		if(i >= 'a')
		{
#ifdef BLACKBCKGR
			_textcolor(LIGHTCYAN);
#else
			_textcolor(LIGHTBLUE);
#endif
			printf("%c: ", i);
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			puts((*result).name);
		}
		if(i == 'z')
			return 0;
		return 1;
	}
	if(eptd_todo == eptd_TAKEdir)
	{
		if(!((*result).attrib&_A_SUBDIR))
		{
			eptd_nodir++;
			return 1;
		}
		i -= eptd_list*26;
		i -= eptd_nodir;
		if(eptd_take == i)
		{
			sprintf((char *)to, "%s\\%s", source, (*result).name);
			return 0;
		}
		return 1;
	}
	return 0;
}

void wEnterPathToDir(wchar_t path_out[], int max)
{
	char to[max];
#ifdef BLACKBCKGR
	_textcolor(LIGHTGRAY);
#else
	_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
	puts("(no trailing '\\' necessary, '\\' may be replaced by '<' and path completion with [ENTER] possible)");
#else
	puts("(eindigende '\\' niet nodig, '\\' mag vervangen worden door '<' en padaanvulling met [ENTER] mogelijk)");
#endif
	int i, k = _wherey();
	eptd_todo = eptd_START;
	do
	{
		if(eptd_todo == eptd_NODIR)
		{
			while(k != _wherey())
			{
				_gotoxy(1, _wherey()-1);
				_clreol();
			}
			_textcolor(LIGHTRED);
#ifdef ENGLISH
			puts("The path entered is the path to a file, not a directory.\nTyp another one or [ENTER] to abort.");
#else
			puts("Het opgegeven pad is er een naar een bestand, geen map.\nGeef een nieuwe in of [ENTER] om af te breken.");
#endif
		}
		if((eptd_todo == eptd_START)||(eptd_todo == eptd_NODIR))
		{
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			while(*(strrchr(fgets(to, max, stdin), '\0')-1) != 10)
			{
				while(*(strrchr(fgets(to, max, stdin), '\0')-1) != 10);
				while(k != _wherey())
				{
					_gotoxy(1, _wherey()-1);
					_clreol();
				}
				_textcolor(LIGHTRED);
#ifdef ENGLISH
				puts("The given path is too long to process.\nTyp another one or [ENTER] to abort.");
#else
				puts("Het opgegeven pad is te lang om te verwerken.\nGeef een nieuwe in of [ENTER] om af te breken.");
#endif
#ifdef BLACKBCKGR
				_textcolor(WHITE);
#else
				_textcolor(BLACK);
#endif
			}
			*(strrchr(to, '\0')-1) = '\0';
			if(to[0] == '\0')
			{
				path_out[0] = L'\0';
				return;
			}
			if((to[1] == ':')&&(to[2] == '\0'))
			{
				mbstowcs(path_out, to, max);
				return;
			}
			for(i=0; to[i] != '\0'; i++)
			{
				if(to[i] == '<')
					to[i] = '\\';
			}
		}
		eptd_todo = eptd_LISTdir;
		eptd_nodir = 0;
		mbstowcs(path_out, to, max);
		i = wwalktree(path_out, max, L"", weptd_found, w_COMPLETE, 0, (void *)to);
		if(!i)
		{
			_textcolor(LIGHTRED);
			perror(NULL);
			path_out[0] = L'\0';
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			return;
		}
		if(i%2)
		{
#ifdef BLACKBCKGR
			_textcolor(LIGHTGRAY);
#else
			_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
			puts("((The search also encountered paths that are too long to process))");
#else
			puts("((Er werden ook paden gevonden die te lang zijn om te verwerken))");
#endif
		}
		i = (i/2)-2*(i/2)*(i<0);
		if(i == 1)
			eptd_take = 'a';
		else
		{
#ifdef BLACKBCKGR
			_textcolor(LIGHTCYAN);
#else
			_textcolor(LIGHTBLUE);
#endif
			printf("[ESC]: ");
#ifdef BLACKBCKGR
			_textcolor(LIGHTGRAY);
#else
			_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
			puts("Cancel");
#else
			puts("Annuleren");
#endif
			if(i == 26*(eptd_list+1)+eptd_nodir)
			{
#ifdef BLACKBCKGR
				_textcolor(LIGHTCYAN);
#else
				_textcolor(LIGHTBLUE);
#endif
				printf("[TAB]: ");
#ifdef BLACKBCKGR
				_textcolor(LIGHTGRAY);
#else
				_textcolor(DARKGRAY);
#endif
#ifdef ENGLISH
				puts("Show next series of found paths");
#else
				puts("Volgende reeks gevonden paden");
#endif
			}
			eptd_take = _getch();
			if((eptd_take == 224)||(eptd_take == 0))
				_getch();
		}
		if((eptd_take == 9)&&(!(i - 26*(eptd_list+1) - eptd_nodir)))
		{
			eptd_list++;
			for(eptd_take=0; eptd_take != 28; eptd_take++)
			{
				_gotoxy(1, _wherey()-1);
				_clreol();
			}
		}
		else
		{
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			if((eptd_take >= 'a')&&(eptd_take <= i - 26*eptd_list - eptd_nodir + 'a' - 1))
			{
				eptd_todo = eptd_TAKEdir;
				eptd_nodir = 0;
				mbstowcs(path_out, to, max);
				wwalktree(path_out, max, L"", weptd_found, w_COMPLETE, 0, (void *)to);
			}
			else
			{
				path_out[0] = L'\0';
				return;
			}
			eptd_list = 0;
			while(k != _wherey())
			{
				_gotoxy(1, _wherey()-1);
				_clreol();
			}
			printf("%s", to);
			if(*(strrchr(fgets(strrchr(to, '\0'), max-strlen(to), stdin), '\0')-1) != 10)
			{
				_textcolor(LIGHTRED);
#ifdef ENGLISH
				puts("The given path is too long to process.");
#else
				puts("Het opgegeven pad is te lang om te verwerken.");
#endif
				while(*(strrchr(fgets(to, max, stdin), '\0')-1) != 10);
				path_out[0] = L'\0';
#ifdef BLACKBCKGR
				_textcolor(WHITE);
#else
				_textcolor(BLACK);
#endif
				return;
			}
			*(strrchr(to, '\0')-1) = '\0';
			for(i=0; to[i] != '\0'; i++)
			{
				if(to[i] == '<')
					to[i] = '\\';
			}
		}
		eptd_todo = eptd_CHECKdir;
	} while((mbstowcs(path_out, to, max), !wwalktree(path_out, 0, L"", weptd_found, w_CHECK, 0, (void *)to))||(eptd_todo == eptd_NODIR));
	return;
}

char weptd_found(struct _wfinddata64_t *result, const wchar_t source[], int i, void *to)
{
	i = (i/2) - 2*(i/2)*(i<0) + 'a' - 1;
	if(eptd_todo == eptd_CHECKdir)
	{
		if((*result).attrib&_A_SUBDIR)
			eptd_todo == eptd_DIR;
		else
			eptd_todo == eptd_NODIR;
		return 0;
	}
	if(eptd_todo == eptd_LISTdir)
	{
		if(!((*result).attrib&_A_SUBDIR))
		{
			eptd_nodir++;
			return 1;
		}
		i -= eptd_list*26;
		i -= eptd_nodir;
		if(i >= 'a')
		{
#ifdef BLACKBCKGR
			_textcolor(LIGHTCYAN);
#else
			_textcolor(LIGHTBLUE);
#endif
			printf("%c: ", i);
#ifdef BLACKBCKGR
			_textcolor(WHITE);
#else
			_textcolor(BLACK);
#endif
			printf("%ls\n", (*result).name);
		}
		if(i == 'z')
			return 0;
		return 1;
	}
	if(eptd_todo == eptd_TAKEdir)
	{
		if(!((*result).attrib&_A_SUBDIR))
		{
			eptd_nodir++;
			return 1;
		}
		i -= eptd_list*26;
		i -= eptd_nodir;
		if(eptd_take == i)
		{
			sprintf((char *)to, "%ls\\%ls", source, (*result).name);
			return 0;
		}
		return 1;
	}
	return 0;
}
