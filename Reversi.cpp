#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>
#include <regex>
#include <stdlib.h>
#include <windows.h>

using namespace std;

string vartotojasIvede = "start";
int xIvedimas = 0;
int yIvedimas = 0;

int aplinkiniaiPosPakytimai[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };


void pradetiZaidima(char lentele[8][8]);
void SpausdintiLentele(char naujalentele[8][8], int xIvedimas_C = -1, int yIvedimas_C = -1, string ivedimas = ::vartotojasIvede);
void KasLaimejo(char(&lentele)[8][8]);
void AtspasdintKurJudet(char lentele[8][8], char zaidejas);
void pakeisti(char(lentele)[8][8], int eilute, int stulpelis, char zaidejas);
bool PatikrintiIrGaliJudet(char lentele[8][8], string ivedimas, char zaidejas);
vector< vector<int>> KurJudet(char lentele[8][8], char zaidejas);
vector< vector<int>> GautiKurJudet(char lentele[8][8], char zaidejas);

void pradetiZaidima(char lentele[8][8])
{
	cout << "   A  B  C  D  E  F  G  H" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << (i) << "  ";
		for (int j = 0; j < 8; j++)
		{
			lentele[i][j] = '-';
			lentele[3][3] = 'O';
			lentele[3][4] = 'X';
			lentele[4][3] = 'X';
			lentele[4][4] = 'O';

			cout << lentele[i][j] << "  ";
		}
		cout << endl;
	}

}


void SpausdintiLentele(char naujalentele[8][8], int xIvedimas_C, int yIvedimas_C, string ivedimas)
{

	cout << endl;

	if (xIvedimas_C == -1 || yIvedimas_C == -1)
	{
		xIvedimas = string("abcdefgh").find(ivedimas[0]);
		yIvedimas = string("01234567").find(ivedimas[1]);
		naujalentele[yIvedimas][xIvedimas] = 'O';
		pakeisti(naujalentele, xIvedimas, yIvedimas, 'O');
	}
	else
	{
		naujalentele[yIvedimas_C][xIvedimas_C] = 'X';
		pakeisti(naujalentele, xIvedimas_C, yIvedimas_C, 'X');
	}


	for (int i = 0; i < 8; i++)
	{
		cout << (i) << "  ";
		for (int j = 0; j < 8; j++)
		{
			cout << naujalentele[i][j] << "  ";
		}
		cout << endl;

	}
}


int patikrintiDabartiniRezultata(char lentele[8][8], char Role) {
	int total = 0;
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			if (lentele[i][j] == Role)
				total += 1;

	return total;
}


void KasLaimejo(char(&lentele)[8][8]) {
	int zaidejo_taskai = patikrintiDabartiniRezultata(lentele, 'O');
	int Varzovo_taskai = patikrintiDabartiniRezultata(lentele, 'X');

	cout << "Jus surinkote: " << zaidejo_taskai << " T " << endl; ;
	cout << "Varzovas surinko: " << Varzovo_taskai << " T " << endl;
	if (zaidejo_taskai == Varzovo_taskai) {
		cout << "Lygiasias!";
		return;
	}

	cout << ((zaidejo_taskai > Varzovo_taskai) ? "JUS LAIMEJOTE" : "VARZOVAS LAIMEJO");

}


bool TikrintTrajektorija(char lentele[8][8], char charPadetis, int eilute, int stulpelis, int trajectory[], char zaidejas)
{
	char kitasZaidejas = (zaidejas == 'O') ? 'X' : 'O';
	if (charPadetis == kitasZaidejas) {

		int dabartineEilute = eilute + trajectory[0];
		int dabartinisStulpelis = stulpelis + trajectory[1];
		while (charPadetis == kitasZaidejas) {
			dabartineEilute += trajectory[0];
			dabartinisStulpelis += trajectory[1];
			if (dabartineEilute > 7 || dabartineEilute < 0 || dabartinisStulpelis > 7 || dabartineEilute < 0)
				break;
			charPadetis = lentele[dabartineEilute][dabartinisStulpelis];
		}
		if (charPadetis == zaidejas)
			return true;
	}
	return false;
}


vector< vector<int>> ListPakeisti(char(lentele)[8][8], int eilute, int stulpelis, char zaidejas, int trajectory[], vector< vector<int>> d_to_pakeisti)
{
	char kitasZaidejas = (zaidejas == 'O') ? 'X' : 'O';
	eilute = eilute + trajectory[0];
	stulpelis = stulpelis + trajectory[1];
	zaidejas = lentele[stulpelis][eilute];


	while (zaidejas == kitasZaidejas) {
		vector<int> d = { stulpelis, eilute };
		d_to_pakeisti.push_back(d);
		eilute += trajectory[0];
		stulpelis += trajectory[1];

		zaidejas = lentele[stulpelis][eilute];
	}
	return d_to_pakeisti;
}


void pakeisti(char(lentele)[8][8], int eilute, int stulpelis, char zaidejas) {
	cout << "   A  B  C  D   E  F  G  H" << endl;
	vector< vector<int>> d_to_pakeisti;

	char kitasZaidejas = (zaidejas == 'O') ? 'X' : 'O';


	for (auto deltas : aplinkiniaiPosPakytimai) {

		int dabartineEilute = eilute + deltas[0];
		int dabartinisStulpelis = stulpelis + deltas[1];


		if (dabartineEilute > 7 || dabartineEilute < 0 || dabartinisStulpelis > 7 || dabartinisStulpelis < 0)
			continue;


		char charPadetis = lentele[dabartinisStulpelis][dabartineEilute];


		bool pakeisti_this_direction = false;


		if (TikrintTrajektorija(lentele, charPadetis, eilute, stulpelis, deltas, zaidejas)) {
			d_to_pakeisti = ListPakeisti(lentele, eilute, stulpelis, zaidejas, deltas, d_to_pakeisti);
		}
	}

	for (auto pos : d_to_pakeisti)
		lentele[pos[0]][pos[1]] = zaidejas;
}


bool ArPakeistiGalim(char lentele[8][8], int eilute, int stulpelis, char zaidejas) {
	char kitasZaidejas = (zaidejas == 'O') ? 'X' : 'O';

	for (auto deltas : aplinkiniaiPosPakytimai) {

		if (eilute + deltas[0] > 7 || eilute + deltas[0] < 0 || stulpelis + deltas[1] > 7 || stulpelis + deltas[1] < 0) {
			continue;
		}

		char charPadetis = lentele[eilute + deltas[0]][stulpelis + deltas[1]];

		if (TikrintTrajektorija(lentele, charPadetis, eilute, stulpelis, deltas, zaidejas)) return true;
	}

	return false;
}


bool PatikrintiIrGaliJudet(char lentele[8][8], string ivedimas, char zaidejas) {
	string eilute = "abcdefgh";
	string stulpelis = "01234567";

	if (ivedimas == "start" || ivedimas == "Start" || ivedimas == "exit" || ivedimas == "Exit") {
		vartotojasIvede = ivedimas;
		return true;
	}

	if (ivedimas.length() != 2) {
		return false;
	}

	char eiluteSimbolis = ivedimas[0];
	char stulpelisSimbolis = ivedimas[1];

	if (eilute.find(eiluteSimbolis) == string::npos || stulpelis.find(stulpelisSimbolis) == string::npos) {
		return false;
	}

	int eiluteIndeksas = eilute.find(eiluteSimbolis);
	int stulpelisIndeksas = stulpelis.find(stulpelisSimbolis);

	if (lentele[stulpelisIndeksas][eiluteIndeksas] != '-') {
		return false;
	}

	vector<vector<int>> sarasas = KurJudet(lentele, zaidejas);
	if (find(sarasas.begin(), sarasas.end(), vector<int>{eiluteIndeksas, stulpelisIndeksas}) == sarasas.end()) {
		return false;
	}

	return true;
}


vector< vector<int>> KurJudet(char lentele[8][8], char zaidejas) {

	vector< vector<int>> sarasas;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {


			if (lentele[i][j] == '-') {


				if (ArPakeistiGalim(lentele, i, j, zaidejas)) {

					vector<int> move = { j, i };
					sarasas.push_back(move);
				}
			}
		}
	}


	return sarasas;
}


vector< vector<int>> GautiKurJudet(char lentele[8][8], char zaidejas) {
	return KurJudet(lentele, zaidejas);
}


void AtspasdintKurJudet(char lentele[8][8], char zaidejas) {
	int zaidejas_total = patikrintiDabartiniRezultata(lentele, 'O');
	int Computer_total = patikrintiDabartiniRezultata(lentele, 'X');
	cout << "                                                                               Jus surinkote:    " << zaidejas_total << " T " << endl;
	cout << "                                                                               Varzovas surinko: " << Computer_total << " T " << endl;
	cout << endl; 
	string stulpelis = "abcdefgh";
	string ivedimas = "";
	string role = (zaidejas == 'O') ? "Jus" : "Varzovas";
	cout << endl;
	cout << role << " galite pasirinkiti is situ:" << endl;
	auto v = GautiKurJudet(lentele, zaidejas);
	for (const auto& vec : v) {
		cout << "(" << stulpelis.at(vec[0]) << "," << vec[1] << ")  ";
	}
	cout << endl;

}


int main()
{
	char lentele[8][8] = { ' ' };
	vector< vector<int>> sarasas;

	pradetiZaidima(lentele);
	while (vartotojasIvede != "Exit" && vartotojasIvede != "exit")
	{
		sarasas = KurJudet(lentele, 'O');
		AtspasdintKurJudet(lentele, 'O');

		cout << "Zaidejas pasirinko: ";
		cin >> vartotojasIvede;

		if (vartotojasIvede == "Exit" || vartotojasIvede == "exit") {
			break;
		}

		while (!PatikrintiIrGaliJudet(lentele, vartotojasIvede, 'O'))
		{
			cout << "Netinkamas pasirinkimas, bandykite dar karta: ";
			cin >> vartotojasIvede;
		}

		SpausdintiLentele(lentele);
		vector< vector<int>> sarasasVarzovo;
		sarasasVarzovo = KurJudet(lentele, 'X');
		AtspasdintKurJudet(lentele, 'X');

		try
		{
			int randomInt;
			if (sarasasVarzovo.size() > 1) 
				randomInt = rand() % (sarasasVarzovo.size());
			else 
				randomInt = 0;
			cout << "Varzovas pasirinko: (" << (char)('A' + sarasasVarzovo[randomInt][0]) << "," << sarasasVarzovo[randomInt][1] << ")" << endl;
			Sleep(2000);
			system("cls");
			SpausdintiLentele(lentele, sarasasVarzovo[randomInt][0], sarasasVarzovo[randomInt][1]);
		}
		catch (exception e)
		{
			cout << "Siuo metu varzovas neturi pasirinkimo, dabar jusu eile. ";
			continue;
		}
	}

	KasLaimejo(lentele);
	cout << endl;

	return 0;
}
