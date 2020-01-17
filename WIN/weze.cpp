//---------------------------
#pragma hdrstop
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WIN 1

#if (WIN > 0)
#include <windows.h>
// kolory wed�ug: http://stackoverflow.com/questions/17125440/c-win32-console-color

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
//! Drukuj tekst wg bie��cego atrybutu
void PutZnak(char znak)
{
  unsigned long count;

  WriteConsole(hStdOut, &znak, 1, &count, NULL);
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
//! Drukuj tekst wg bie��cego atrybutu
void PutZnak(char* znak)
{
  putchar((int)znak[0]);
} // PutZnak

//---------------------------------------------------------------------------
void SetKursorPoz(short x, short y)
{
  printf("\033[2J\033[1;1H"); // wyczy�� + g�rny lewy r�g
} // SetKursorPoz

//---------------------------------------------------------------------------
void PrzygotujEkran(void)
{
  printf("\033[2J\033[1;1H"); // wyczy�� + g�rny lewy r�g
  SetTekstKolor(KOLOR_DARKGREY);
} // PrzygotujEkran

#endif

#define ROZMIAR 20 // okre�la d�ugo�� boku rysowanego kwadratu

/*
	AAA 
	BBB  => wyniki ko�cowy dla ROZMIAR = 3
	CCC 
*/

//---------------------------------------------------------------------------
//! G��wne wej�cie do programu
int main(int argc, char* argv[])
{
	short
		wiersz = 0, 
		kolumna = 0, 
		iteracje = 0; // zmienna pomocnicza "zacie�niaj�ca" wn�trze kwadratu, zwi�ksza si� po dotarciu do lewego g�rnego rogu

	char koniec = ' '; // wczytywany znak, gdy 'q' koniec p�tli

	bool obrot = false; // kontrola, czy zosta�o wykonane pe�ne przej�cie 

	PrzygotujEkran(); // przygotowanie mapy, do kt�rej b�d� wpisywane znaki - na odpowiednich wsp�rz�dnych

	// p�tla ustawia znak w pozycji okre�lonej przez wsp�rz�dne 'kolumna' oraz 'wiersz', a nast�pnie zmienia warto�� wsp�rz�dnych w zale�no�ci od ich obecnej warto�ci
	while (koniec != 'q')
	{
		koniec = getchar();
		SetKursorPoz(kolumna, wiersz);
		PutZnak('A'+wiersz);
		
		// przypisz warto�ci dla rysowania gornej krawedzi
		// lewy g�rny r�g
		if (kolumna == iteracje && wiersz == iteracje && obrot == true)
		{
			iteracje++;
			obrot = false;
			kolumna++;
			wiersz++;
		}
		// g�ra
		else if (kolumna < ROZMIAR - iteracje && wiersz == iteracje)
		{
			kolumna++;
		}
		// g�rna kraw�d�

		// przypisz warto�ci dla rysowania prawej krawedzi
		// prawy g�rny r�g
		if (kolumna == ROZMIAR - iteracje && wiersz == iteracje)
		{
			kolumna--;
			wiersz++;
		}
		// prawo
		else if (wiersz > iteracje && wiersz < ROZMIAR+1 - iteracje && kolumna == ROZMIAR-1 - iteracje)
		{
			wiersz++;
		}
		// prawa kraw�d�

		//przypisz warto�ci dla rysowania dolnej kraw�dzi
		// prawy dolny r�g
		if (wiersz == ROZMIAR - iteracje && kolumna == ROZMIAR-1 - iteracje)
		{
			kolumna--;
			wiersz--;
		}
		// d�
		else if (wiersz == ROZMIAR-1 - iteracje && kolumna > -1 + iteracje && kolumna < ROZMIAR-1 - iteracje)
		{
			kolumna--;
		}
		// dolna krawedz

		// przypisz wartosci dla rysowania lewej kraw�dzi
		// lewy dolny r�g
		if (kolumna == -1 + iteracje && wiersz == ROZMIAR-1 - iteracje)
		{
			wiersz--;
			kolumna++;
			obrot = true;
		}
		// lewo
		else if (kolumna == iteracje && wiersz > iteracje && wiersz < ROZMIAR-1 - iteracje)
		{
			wiersz--;
		}
		// lewa kraw�d�
	}
	return 0;
} 
