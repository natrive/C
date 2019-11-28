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

#define ROZMIAR 20 // okreœla d³ugoœæ boku rysowanego kwadratu

/*
	AAA 
	BBB  => wyniki koñcowy dla ROZMIAR = 3
	CCC 
*/

//---------------------------------------------------------------------------
//! G³ówne wejœcie do programu
int main(int argc, char* argv[])
{
	short
		wiersz = 0, 
		kolumna = 0, 
		iteracje = 0; // zmienna pomocnicza "zacieœniaj¹ca" wnêtrze kwadratu, zwiêksza siê po dotarciu do lewego górnego rogu

	char koniec = ' '; // wczytywany znak, gdy 'q' koniec pêtli

	bool obrot = false; // kontrola, czy zosta³o wykonane pe³ne przejœcie 

	PrzygotujEkran(); // przygotowanie mapy, do której bêd¹ wpisywane znaki - na odpowiednich wspó³rzêdnych

	// pêtla ustawia znak w pozycji okreœlonej przez wspó³rzêdne 'kolumna' oraz 'wiersz', a nastêpnie zmienia wartoœæ wspó³rzêdnych w zale¿noœci od ich obecnej wartoœci
	while (koniec != 'q')
	{
		koniec = getchar();
		SetKursorPoz(kolumna, wiersz);
		PutZnak('A'+wiersz);
		
		// przypisz wartoœci dla rysowania gornej krawedzi
		// lewy górny róg
		if (kolumna == iteracje && wiersz == iteracje && obrot == true)
		{
			iteracje++;
			obrot = false;
			kolumna++;
			wiersz++;
		}
		// góra
		else if (kolumna < ROZMIAR - iteracje && wiersz == iteracje)
		{
			kolumna++;
		}
		// górna krawêdŸ

		// przypisz wartoœci dla rysowania prawej krawedzi
		// prawy górny róg
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
		// prawa krawêdŸ

		//przypisz wartoœci dla rysowania dolnej krawêdzi
		// prawy dolny róg
		if (wiersz == ROZMIAR - iteracje && kolumna == ROZMIAR-1 - iteracje)
		{
			kolumna--;
			wiersz--;
		}
		// dó³
		else if (wiersz == ROZMIAR-1 - iteracje && kolumna > -1 + iteracje && kolumna < ROZMIAR-1 - iteracje)
		{
			kolumna--;
		}
		// dolna krawedz

		// przypisz wartosci dla rysowania lewej krawêdzi
		// lewy dolny róg
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
		// lewa krawêdŸ
	}
	return 0;
} 
