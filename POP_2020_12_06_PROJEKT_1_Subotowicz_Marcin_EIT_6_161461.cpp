// POP_2020_12_06_PROJEKT_1_Subotowicz_Marcin_EIT_6_161461

#include <iostream>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std; 

const int MAX_KART = 52;
const int MAX_KOLOR = 13;
const int KARTY_NA_RECE = 5;
const int szerokosc_karty = 7;
const int dlugosc_karty = 10;
const int WYNIKI = 1;
const int WYNIK = 5;
const int PUNKTY_PARA = 100;
const int PUNKTY_TROJKA = 10000;
const int PUNKTY_STRIT = 30000;
const int PUNKTY_KOLOR = 80000;
const int PUNKTY_FULL = 600000;
const int PUNKTY_KARETA = 5000000;
const int PUNKTY_POKER = 20000000;

enum kolory {serce = 1, wino= 2, trefl = 3, dzwonek = 4};

struct karta {
	int symbol;
	kolory kolor;	
};

stack <karta> stos_kart;

void rysowanie_kart (int symbol, kolory kolor, int linia);
void wymiana_kart (karta *tablica);
void wyswietlanie_reki_gracza (karta *gracz);
void komputer_gracz (karta *tablica);
int	sprawdzanie_wyniku (karta *tablica);
void wiersz_1 ();
void wiersz_2 (int symbol_karty, int linia);
void wiersz_3 (kolory kolor);
void wiersz_4 (kolory kolor);
void wiersz_5 (kolory kolor);
void wiersz_6 (kolory kolor);
void wiersz_7 (kolory kolor);
void wiersz_8 (kolory kolor);

int main ()
{
	srand (time(NULL));
	karta tab_kart[MAX_KART];
	
	int sym, k = 1; // Incijalizacja tablicy, z której bêd¹ losowane karty do stosu
	for (int i = 0 ; i < 4 ; i++)
	{
		sym = 2;
		for (int j = 0 + (MAX_KOLOR * i) ; j < (i+1) * MAX_KOLOR ; j++)
		{
			tab_kart[j].symbol = sym;
			tab_kart[j].kolor = static_cast<kolory>(i+1);
			sym++;
		}
		k++;
	}
	
	karta buff; //Losowanie kart z tablicy do stosu (tasowanie kart)
	int zm_losujaca;
	for (int i = 0 ; i < MAX_KART ; i++)
	{
		zm_losujaca = rand() % (MAX_KART - i);
		stos_kart.push(*(tab_kart+zm_losujaca));
		for (int j = 0 ; j < (MAX_KART - zm_losujaca - i) ; j++)
		{
			buff = tab_kart[zm_losujaca+j];
			tab_kart[zm_losujaca+j] = tab_kart[zm_losujaca+j+1];
			tab_kart[zm_losujaca+j+1] = buff;
		}	
	}
	
	//cout << "Wielkosc stosu: "<< stos_kart.size() << endl; Opcjonalna kontrola stosu
	
	int ilosc_graczy;
	cout <<"Witaj w grze Poker!\n";
	bool poprawna_ilosc_graczy; // Podanie ilosci graczy wraz ze sprawdzeniem czy podano poprawna ilosc graczy
	do 
	{
		poprawna_ilosc_graczy = false;
		cout << "Podaj liczbe graczy (min 2, max 4): \n";
		cin >> ilosc_graczy;
		if (2 > ilosc_graczy || ilosc_graczy > 4) 
		{
			poprawna_ilosc_graczy = true;
			cout << "Podales niepoprawna ilosc graczy\n";
		}
	}while (poprawna_ilosc_graczy);
	
	
	karta Gracze[ilosc_graczy][KARTY_NA_RECE+WYNIKI]; //Inicjalizacja tablicy przechowujacej karty zawodnikow oraz wynik
	for (int i = 0 ; i < KARTY_NA_RECE ; i++)
	{
		for (int j = 0 ; j < ilosc_graczy ; j++)
		{
			Gracze[j][i] = stos_kart.top();
			stos_kart.pop();	
		}	
	}
	
	cout << "Etap wymiany kart...\n";
	for (int i = 0 ; i < ilosc_graczy ; i++)
	{
		if (i == 0) 
		{
			komputer_gracz (Gracze[i]);
		}
		else
		{
			cout << "Tura gracza " << i+1 << ": " << endl;
			wyswietlanie_reki_gracza (Gracze[i]);
			wymiana_kart (Gracze[i]);
		}
		cout << endl << endl << endl << endl << endl << endl << endl;
	}
	
	for (int i = 0 ; i < ilosc_graczy ; i++) // Wypelnienie 6 kolumny w tablicy Gracze wynikiem poszczegolnych graczy
	{
		sprawdzanie_wyniku (Gracze[i]);
	}
	
	int zwyciezca = 0;
	for (int i = 0 ; i < ilosc_graczy-1 ; i++) // Wylonienie zwyciescy
	{
		if (Gracze[i][WYNIK].symbol < Gracze[i+1][WYNIK].symbol) zwyciezca = i+1;
	}
	
	cout << "Koncowe wyniki! Karty graczy: \n"; // Wyswietlenie kart wszystkich graczy i zwyciezcy
	for (int i = 0 ; i < ilosc_graczy ; i++)
	{
		cout << "Gracz " << i+1 << ": " << endl;
		wyswietlanie_reki_gracza (Gracze[i]);
		cout << endl;
	}
		
	cout << "Zwyciesca jest gracz z numerem " << zwyciezca+1 << "!!!" <<endl;
	return 0;		
}

int	sprawdzanie_wyniku (karta *tablica)
{
	bool strit, kolor, trojka;  
	int pary = 0, wieksza_dwojka = 0, symbol_pary = 0, symbol_trojki = 1;
	
	karta tablica_pomocnicza[KARTY_NA_RECE+WYNIK];
	for (int j = 0 ; j < KARTY_NA_RECE ; j++)
	{
		tablica_pomocnicza[j] = tablica[j];
	}
	
	karta buff;
	
	for (int i = 0; i < KARTY_NA_RECE-1; i++) //Sortowanie tablicy pomocniczej w calu sprawdzania indywidualnych wynikow
    {
        for ( int j = 0; j < KARTY_NA_RECE-1-i; j++)
        {
            if (tablica_pomocnicza[KARTY_NA_RECE-1-j].symbol < tablica_pomocnicza[KARTY_NA_RECE-2-j].symbol)
			{
				buff = tablica_pomocnicza[KARTY_NA_RECE-2-j];
				tablica_pomocnicza[KARTY_NA_RECE-2-j] = tablica_pomocnicza[KARTY_NA_RECE-1-j];
				tablica_pomocnicza[KARTY_NA_RECE-1-j] = buff;
			}
        }
	}
	
	tablica[WYNIK].symbol = tablica[4].symbol; // Sprawdzanie najwyzszej karty
	
	for (int i = 0; i < KARTY_NA_RECE-1; i++) //Sprawdzanie czy jest dwojka lub dwie dwojki
	{
		if (tablica_pomocnicza[i].symbol == tablica_pomocnicza[i+1].symbol) 
		{
			wieksza_dwojka = i;
			symbol_pary = tablica_pomocnicza[i].symbol;
			if (pary == 1) pary += 7;
			else pary++;
			i++;
		}
	}
	tablica[WYNIK].symbol = PUNKTY_PARA*pary*tablica_pomocnicza[wieksza_dwojka].symbol;
	
	for (int i = 0; i < KARTY_NA_RECE-2; i++) //Sprawdzanie czy jest trojka
	{
		if (tablica_pomocnicza[i].symbol == tablica_pomocnicza[i+1].symbol == tablica_pomocnicza[i+2].symbol) 
		{
			
			tablica[WYNIK].symbol = PUNKTY_TROJKA*tablica_pomocnicza[i+1].symbol;
			trojka++;
			symbol_trojki = tablica_pomocnicza[i].symbol;
			break;
		}
	}	
	
	for (int i = 0; i < KARTY_NA_RECE-1; i++) // Sprawdzanie czy jest strit
	{
		if (!(tablica_pomocnicza[i].symbol == tablica_pomocnicza[i+1].symbol + 1)) 
		{
		strit = false;
		break;	
		}
		strit = true;	
	}
	if (strit) tablica[WYNIK].symbol = PUNKTY_STRIT*tablica_pomocnicza[4].symbol;
	
	for (int i = 0; i < KARTY_NA_RECE-1; i++) //Sprawdzanie czy jest kolor
	{
		if (!(tablica_pomocnicza[i].kolor == tablica_pomocnicza[i+1].kolor )) 
		{
			kolor = false;
			break;	
		}
		kolor = true;	
	}
	if (kolor) tablica[WYNIK].symbol = PUNKTY_KOLOR*tablica_pomocnicza[4].symbol;
	
	if (pary == 1 && trojka == 2 && !(symbol_trojki == symbol_pary)) tablica[WYNIK].symbol = PUNKTY_FULL*symbol_trojki; // Sprawdzanie czy jest full
	
	for (int i = 0; i < 2; i++) //Sprawdzanie czy jest kareta
	{
		if (tablica_pomocnicza[i].symbol == tablica_pomocnicza[i+1].symbol == tablica_pomocnicza[i+2].symbol == tablica_pomocnicza[i+3].symbol) 
		{
			tablica[WYNIK].symbol = PUNKTY_KARETA*tablica_pomocnicza[i+1].symbol;
			break;
		}
	}
	
	if (kolor && strit ) tablica[WYNIK].symbol = PUNKTY_POKER*tablica_pomocnicza[4].symbol; // Sprawdzanie czy jest poker lub poker krolewski
}

void wymiana_kart (karta *tablica)
{
	int ile;
	bool poprawna_ilosc_kart;
	do
	{
		poprawna_ilosc_kart = false;
		cout << "Ile kart chcesz wymienic: ";
		cin >> ile;
		if (ile < 0 || ile > 5)
		{
			cout << "\nPodano niepoprawna ilosc kart do wymiany!\n";
			poprawna_ilosc_kart = true;		
		}
	}while (poprawna_ilosc_kart);
	int indeks, blad;
	int tab_wymiana[ile];
	for (int i = 0; i < ile ; i++)
	{
		tab_wymiana[i] = 0;
	}
	cout << "Podaj numery kart, ktore chcesz wymienic: \n";
	for (int i = 0; i < ile ; i++)
	{
		blad = 0;
		cout << "Karta nr " << i+1 << ": ";
		cin >> indeks;
		for (int j = 0; j < i ; j++ ) 
		{
			if (tab_wymiana[j] == indeks) 
			{
				cout << "Podano juz wybrany numer karty! Prosze poprawic: " << endl;
				i--;
				blad = 1;
				break;
			}
			if (!(indeks > 0 && indeks < 6)) 
			{
				cout << "Podano niedozowlony numer karty (od 1 do 5): " << endl;
				i--;
				blad = 1;
				break;
			}
		}
		if (!blad) tab_wymiana[i] = indeks;
	}
	for (int i = 0; i < ile ; i++)	
	{
		indeks = tab_wymiana[i];
		tablica[indeks-1] = stos_kart.top();
		stos_kart.pop();
	}
	
	//cout << "Wielkosc stosu: "<< stos_kart.size() << endl; Opcjonalna kontrola stosu
}

void komputer_gracz (karta *tablica)
{
	karta komputer[2][KARTY_NA_RECE];
	
	for (int i = 0 ; i < KARTY_NA_RECE ; i++)
	{
		komputer[0][i] = tablica[i];
		komputer[1][i].symbol = i;
	}
	
	karta buff, buff_indeks;
	
	for (int i = 0; i < KARTY_NA_RECE-1; i++) //Sortowanie tablicy komputer, by mozna bylo ocenic najlepszy ruch
    {
        for ( int j = 0; j < KARTY_NA_RECE-1-i; j++)
        {
            if (komputer[0][KARTY_NA_RECE-1-j].symbol < komputer[0][KARTY_NA_RECE-2-j].symbol)
			{
				buff = komputer[0][KARTY_NA_RECE-2-j];
				komputer[0][KARTY_NA_RECE-2-j] = komputer[0][KARTY_NA_RECE-1-j];
				komputer[0][KARTY_NA_RECE-1-j] = buff;
				
				buff_indeks = komputer[1][KARTY_NA_RECE-2-j];
				komputer[1][KARTY_NA_RECE-2-j] = komputer[1][KARTY_NA_RECE-1-j];
				komputer[1][KARTY_NA_RECE-1-j] = buff_indeks;
			}
        }
	}
	
	int id_dwojki, id_trojki; //Algorytm wyboru ruchu przez komputer
	bool dwojka, trojka, strit, kareta, kolor;
	for (int i = 0; i < KARTY_NA_RECE; i++)
	{
		if (i < KARTY_NA_RECE-1 && komputer[0][i].symbol == komputer[0][i+1].symbol) 
		{
			id_dwojki = i;
			i++;
			dwojka = true;
		}
		
		if (i < KARTY_NA_RECE-2 && komputer[0][i].symbol == komputer[0][i+1].symbol == komputer[0][i+2].symbol) 
		{
			trojka++;
			id_trojki = i;
			trojka = true;
		}
	}
	
	for (int i = 0; i < KARTY_NA_RECE-1; i++)
	{
		if (!(komputer[0][i].symbol == komputer[0][i+1].symbol + 1)) 
		{
		strit = false;
		break;	
		}
		strit = true;	
	}
	
	for (int i = 0; i < KARTY_NA_RECE-1; i++)
	{
		if (!(komputer[0][i].kolor == komputer[0][i+1].kolor )) 
		{
			kolor = false;
			break;	
		}
		kolor = true;	
	}
	
	for (int i = 0; i < 2; i++)
	{
		if (komputer[0][i].symbol == komputer[0][i+1].symbol == komputer[0][i+2].symbol == komputer[0][i+3].symbol) 
		{
			kareta = true;
			break;
		}
	}
	
	if (dwojka && !trojka) 
	{
		for ( int i = 0; i < KARTY_NA_RECE ; i++)
		{
			if (!(i == id_dwojki || i == id_dwojki+1))
			{
				tablica[i] = stos_kart.top();
				stos_kart.pop();
			}
		}
	}
	
	if (!dwojka && trojka) 
	{
		for ( int i = 0; i < KARTY_NA_RECE ; i++)
		{
			if (!(i == id_trojki || i == id_trojki+1 || i == id_trojki+2))
			{
				tablica[i] = stos_kart.top();
				stos_kart.pop();
			}
		}
	}
	if (!(dwojka && trojka && strit && kolor && kareta))
	{
		for ( int i = 0; i < KARTY_NA_RECE ; i++)
		{
			if (!(i == komputer[1][4].symbol))
			{
				tablica[i] = stos_kart.top();
				stos_kart.pop();
			}
		}
	}
}

void wyswietlanie_reki_gracza (karta *gracz)
{
	for (int i = 0 ; i < dlugosc_karty ; i++)
	{
		for (int j = 0 ; j < KARTY_NA_RECE ; j++)
		{
			rysowanie_kart(gracz[j].symbol, gracz[j].kolor, i);	
			if (!(j == KARTY_NA_RECE-1))cout << " ";
			
		}
		cout << endl;	
	}
}

void rysowanie_kart (int symbol, kolory kolor, int linia)
{	
	if (linia == 0 || linia == 9) wiersz_1 ();
	else if (linia == 1 || linia == 8) wiersz_2 (symbol, linia);
	else if (linia == 2) wiersz_3 (kolor);
	else if (linia == 3) wiersz_4 (kolor);
	else if (linia == 4) wiersz_5 (kolor);
	else if (linia == 5) wiersz_6 (kolor);
	else if (linia == 6) wiersz_7 (kolor);
	else if (linia == 7) wiersz_8 (kolor);	
}

void wiersz_1 ()
{
	cout << "-------"; 
}

void wiersz_2 (int symbol_karty, int linia)
{
	string znak;
	if (symbol_karty == 11) znak = 'J';
	else if (symbol_karty == 12) znak = 'D';
	else if (symbol_karty == 13) znak = 'K';
	else if (symbol_karty == 14) znak = 'A';	
	else znak = to_string(symbol_karty);
	if (symbol_karty != 10)
	{
		if (linia == 1) cout << "|" << znak << "    " << "|" ;
		else cout << "|" << "    " << znak << "|" ;
	}
	else
	{
		if (linia == 1) cout << "|" << znak << "   " << "|" ;
		else cout << "|" << "   " << znak << "|" ;
	}
}

void wiersz_3 (kolory kolor)
{
	if (kolor == 1) cout << "|" << "     " << "|"; // Serce
	if (kolor == 2) cout << "|" << "  " << "#" << "  " << "|"; //Wino
	if (kolor == 3) cout << "|" << " " << "###" << " " << "|"; //Trefl 
	if (kolor == 4) cout << "|" << "  " << "#" << "  " << "|";// Dzwonek
}

void wiersz_4 (kolory kolor)
{
	if (kolor == 1) cout << "|" << " " << "#" << " " << "#" << " " << "|"; // Serce
	if (kolor == 2) cout << "|" << " " << "###" << " " << "|"; //Wino
	if (kolor == 3) cout << "|" << "  " << "#" << "  " << "|"; //Trefl 
	if (kolor == 4) cout << "|" << " " << "###" << " " << "|";// Dzwonek
}

void wiersz_5 (kolory kolor)
{
	if (kolor == 1) cout << "|" << "#####" << "|"; // Serce
	if (kolor == 2) cout << "|" << "#####" << "|"; //Wino
	if (kolor == 3) cout << "|" << "#" << " " << "#" << " " << "#" << "|"; //Trefl 
	if (kolor == 4) cout << "|" << "#####" << "|";// Dzwonek
}

void wiersz_6 (kolory kolor)
{
	if (kolor == 1) cout << "|" << "#####" << "|"; // Serce
	if (kolor == 2) cout << "|" << "#####" << "|"; //Wino
	if (kolor == 3) cout << "|" << "#####" << "|"; //Trefl 
	if (kolor == 4) cout << "|" << "#####" << "|";// Dzwonek
}

void wiersz_7 (kolory kolor)
{
	if (kolor == 1) cout << "|" << " " << "###" << " " << "|"; // Serce
	if (kolor == 2) cout << "|" << "#" << " " << "#" << " " << "#" << "|"; //Wino
	if (kolor == 3) cout << "|" << "#" << " " << "#" << " " << "#" << "|"; //Trefl 
	if (kolor == 4) cout << "|" << " " << "###" << " " << "|";// Dzwonek
}

void wiersz_8 (kolory kolor)
{
	if (kolor == 1) cout << "|" << "  " << "#" << "  " << "|"; // Serce
	if (kolor == 2) cout << "|" << "  " << "#" << "  " << "|"; //Wino
	if (kolor == 3) cout << "|" << "  " << "#" << "  " << "|"; //Trefl 
	if (kolor == 4) cout << "|" << "  " << "#" << "  " << "|";// Dzwonek
}

