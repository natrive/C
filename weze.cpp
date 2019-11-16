//---------------------------
#pragma hdrstop
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define WIN 1

#if (WIN > 0)
#include <windows.h>
// kolory wed³ug: http://stackoverflow.com/questions/17125440/c-win32-console-color

#define KOLOR_DARKBLUE   (FOREGROUND_BLUE)
#define KOLOR_DARKRED    (FOREGROUND_RED)
#define KOLOR_RED        (FOREGROUND_RED   | FOREGROUND_INTENSITY)
#define KOLOR_DARKGREEN  (FOREGROUND_GREEN)
#define KOLOR_GREEN      (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define KOLOR_DARKGREY   (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define KOLOR_DARKYELLOW (FOREGROUND_RED   | FOREGROUND_GREEN)

static HANDLE hStdOut;
//---------------------------------------------------------------------------
//! Ustaw kolor tekstu - kolor zdefiniowany w windows.h
void SetTekstKolor(short kolor)
{
  SetConsoleTextAttribute(hStdOut, kolor);
} // SetTekstKolor

//---------------------------------------------------------------------------
//! Drukuj tekst wg bie¿¹cego atrybutu
void PutZnak(char* znak)
{
  unsigned long count;

  WriteConsole(hStdOut, znak, 1, &count, NULL);
} // PutZnak

//---------------------------------------------------------------------------
void SetKursorPoz(short x, short y)
{
  COORD coord = {0,0};
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(hStdOut, coord);
} // SetKursorPoz

//---------------------------------------------------------------------------
void PrzygotujEkran(void)
{
  system("cls");
  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  SetTekstKolor(KOLOR_DARKGREY);
} // PrzygotujEkran

#else //=====================================================================

// http://cc.byexamples.com/2007/01/20/print-color-string-without-ncurses/
// http://linux.byexamples.com/archives/184/print-text-in-colors-with-a-simple-command-line/

#define KOLOR_DARKBLUE   (char*)"\33[01;34m"
#define KOLOR_DARKRED    (char*)"\33[22;31m"
#define KOLOR_RED        (char*)"\33[01;21m"
#define KOLOR_DARKGREEN  (char*)"\33[22;32m"
#define KOLOR_GREEN      (char*)"\33[01;32m"
#define KOLOR_DARKGREY   (char*)"\33[01;30m"
#define KOLOR_DARKYELLOW (char*)"\33[01;33m"

//---------------------------------------------------------------------------
//! Ustaw kolor tekstu - kolor zdefiniowany w windows.h
void SetTekstKolor(char* kolor)
{
  printf(kolor);
} // SetTekstKolor

//---------------------------------------------------------------------------
//! Drukuj tekst wg bie¿¹cego atrybutu
void PutZnak(char* znak)
{
  putchar((int)znak[0]);
} // PutZnak

//---------------------------------------------------------------------------
void SetKursorPoz(short x, short y)
{
  printf("\033[2J\033[1;1H"); // wyczyœæ + górny lewy róg
} // SetKursorPoz

//---------------------------------------------------------------------------
void PrzygotujEkran(void)
{
  printf("\033[2J\033[1;1H"); // wyczyœæ + górny lewy róg
  SetTekstKolor(KOLOR_DARKGREY);
} // PrzygotujEkran

#endif



//---------------------------------------------------------------------------
//! G³ówne wejœcie do programu
int main(int argc, char* argv[])
{
	short a = 0, i = 0, j = 0, obrot = -1;
	char pom = 'A', koniec = ' ';
	const char* znak = &pom;
	PrzygotujEkran();
	while (koniec != 'q')
	{
		koniec = getchar();
		SetKursorPoz(i, a);
		PutZnak((char*)znak);
		
		gora:
		if (i == j && a == j && obrot == 1)
		{
			j++;
			obrot = -1;
			i++;
			a++;
			pom = znak[0] + 1;
		}
		else if (i <20-j && a == 0+j)
		{
			i++;
		}

		prawo:
		if (i == 20-j && a == 0+j)
		{
			i--;
			pom = znak[0] + 1;
			a++;
		}
		else if (a > 0+j && a < 21-j && i == 19-j)
		{
			pom = znak[0] + 1;
			a++;
		}

		dol:
		if (a == 20 - j && i == 19 - j)
		{
			pom = znak[0] - 1;
			i--;
			a--;
		}
		else if (a == 19 - j && i > -1 + j && i < 19 - j)
		{
			i--;
		}

		lewo:
		if (i == -1+j && a == 19-j)
		{
			pom = znak[0] - 1;
			a--;
			i++;
		}
		else if (i == 0+j && a > 0+j && a < 19-j)
		{
			pom = znak[0] - 1;
			a--;
			obrot = 1;
		}

	}
  
// return 0;
} // main


// eof: weze.cpp

