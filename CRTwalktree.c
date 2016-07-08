/*#include <stdio.h>
#include <io.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>*/

int _walktree(const char bron[], const unsigned short int max_array_bron, const char zoek[], char functie(struct _finddata64_t *object, const char map[], int teller, void *data), const char TODO, int i, void *data)
{
	struct _finddata64_t result;
	intptr_t handle;
	if(TODO == w_CHECK)
	{
		errno = 0;
		if(handle = _findfirst64(bron, &result), handle == -1)
			return 0;
		else
		{
			functie(&result, bron, 1, data);
			_findclose(handle);
			return 1;
		}
	}
	if(i < 0)
		return i;
	if((TODO == w_COMPLETE)&&(strlen(bron) >= max_array_bron-1))
		return -1;
	if((TODO == w_WALK)&&(strlen(bron) >= max_array_bron-2))
		return -1;
	if(TODO == w_WALK)
		handle = max_array_bron;
	else
		handle = strlen(bron)+3;																	//('\\') en '*' moet nog bij copy kunnen (en '\0')
	char copy[handle];
	strcpy(copy, bron);
	if(TODO == w_COMPLETE)																			/*if((TODO == COMPLETE)||(*(strrchr(copy, '\0')-1) == '\\'))*/
		strcat(copy, "*");
	else
		strcat(copy, "\\*");
	errno = 0;
	handle = _findfirst64(copy, &result);
	if(strchr(copy, '\\') == NULL)
		return 0;
	*strrchr(copy, '\\') = '\0';
	while((result.attrib&_A_SUBDIR)&&((!strcmp(result.name, "."))||(!strcmp(result.name, "..")))&&(!errno))	//de virtuele mappen "." en ".." (bovenliggenden) mogen niet in aanmerking genomen worden
		_findnext64(handle, &result);
	while(!errno)
	{
		if(_stristr(result.name, zoek) != NULL)
		{
			if((TODO != w_DIR)&&(strlen(copy) + 1 + strlen(result.name) >= max_array_bron))							//'\\' moet er nog bij kunnen (en '\0')
			{
				if(!(i%2))
					++i;
			}
			else
			{
				i+=2;
				if(!functie(&result, copy, i, data))
				{
					i*=-1;
					_findclose(handle);
					return i;
				}
			}
		}
		if((TODO == w_WALK)&&(result.attrib&_A_SUBDIR))											//recursief oproepen van deze functie om onderliggende mappen te doorzoeken
		{
			if((strlen(copy) + 3 + strlen(result.name) >= max_array_bron))											//'\\' en {'\\' en '*'} moet er nog bij kunnen (en '\0')
			{
				if(!(i%2))
					++i;
			}
			else
			{
				sprintf(strrchr(copy, '\0'), "\\%s", result.name);
				i = _walktree(copy, max_array_bron, zoek, functie, TODO, i, data);
				if(i < 0)
				{
					_findclose(handle);
					return i;
				}
				errno = 0;
				*strrchr(copy, '\\') = '\0';
			}
		}
		_findnext64(handle, &result);
	}
	if(handle != -1)
		_findclose(handle);
	//errno = 0;
	return i;
}

char _w_R1(struct _finddata64_t *object, const char folder[], int i, void *data)
{
	return 1;
}

int wwalktree(const wchar_t bron[], const unsigned short int max_array_bron, const wchar_t zoek[], char functie(struct _wfinddata64_t *object, const wchar_t map[], int teller, void *data), const char TODO, int i, void *data)
{
	struct _wfinddata64_t result;
	intptr_t handle;
	if(TODO == w_CHECK)
	{
		errno = 0;
		if(handle = _wfindfirst64(bron, &result), handle == -1)
			return 0;
		else
		{
			functie(&result, bron, 1, data);
			_findclose(handle);
			return 1;
		}
	}
	if(i < 0)
		return i;
	if((TODO == w_COMPLETE)&&(wcslen(bron) >= max_array_bron-1))
		return -1;
	if((TODO == w_WALK)&&(wcslen(bron) >= max_array_bron-2))
		return -1;
	if(TODO == w_WALK)
		handle = max_array_bron;
	else
		handle = wcslen(bron)+3;																	//('\\') en '*' moet nog bij copy kunnen (en '\0')
	wchar_t copy[handle];
	wcscpy(copy, bron);
	if(TODO == w_COMPLETE)																			/*if((TODO == COMPLETE)||(*(strrchr(copy, '\0')-1) == '\\'))*/
		wcscat(copy, L"*");
	else
		wcscat(copy, L"\\*");
	errno = 0;
	handle = _wfindfirst64(copy, &result);
	if(wcsrchr(copy, L'\\') == NULL)
		return 0;
	*wcsrchr(copy, L'\\') = L'\0';
	while((result.attrib&_A_SUBDIR)&&((!wcscmp(result.name, L"."))||(!wcscmp(result.name, L"..")))&&(!errno))	//de virtuele mappen "." en ".." (bovenliggenden) mogen niet in aanmerking genomen worden
		_wfindnext64(handle, &result);
	while(!errno)
	{
		if(_wcsistr(result.name, zoek) != NULL)
		{
			if((TODO != w_DIR)&&(wcslen(copy) + 1 + wcslen(result.name) >= max_array_bron))							//'\\' moet er nog bij kunnen (en '\0')
			{
				if(!(i%2))
					++i;
			}
			else
			{
				i+=2;
				if(!functie(&result, copy, i, data))
				{
					i*=-1;
					_findclose(handle);
					return i;
				}
			}
		}
		if((TODO == w_WALK)&&(result.attrib&_A_SUBDIR))											//recursief oproepen van deze functie om onderliggende mappen te doorzoeken
		{
			if((wcslen(copy) + 3 + wcslen(result.name) >= max_array_bron))											//'\\' en {'\\' en '*'} moet er nog bij kunnen (en '\0')
			{
				if(!(i%2))
					++i;
			}
			else
			{
				swprintf(wcsrchr(copy, L'\0'), max_array_bron, L"\\%ls", result.name);
				i = walktree(copy, max_array_bron, zoek, functie, TODO, i, data);
				if(i < 0)
				{
					_findclose(handle);
					return i;
				}
				errno = 0;
				*wcsrchr(copy, L'\\') = L'\0';
			}
		}
		_wfindnext64(handle, &result);
	}
	if(handle != -1)
		_findclose(handle);
	//errno = 0;
	return i;
}

char ww_R1(struct _wfinddata64_t *object, const wchar_t folder[], int i, void *data)
{
	return 1;
}
