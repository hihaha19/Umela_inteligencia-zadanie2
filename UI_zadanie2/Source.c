#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

#define GENERAL_BUFFER_SIZE	1000

typedef struct Stvorec {
	int prazdne_miesto;
	int smer_pohybu; //1-dole 2-vpravo 3-hore 4-vlavo
	int predchadzajuce_pohyby[GENERAL_BUFFER_SIZE];
	int pocet_pohybov;
	char predchadzajuci_operator[GENERAL_BUFFER_SIZE];
}STVOREC;

int vypocitaj_cenu(int g) {
	int celkova_cena = -1;

}

//bool chceckCLosed check if node wasnt already visited 
int najdi_susedov(STVOREC* stvorec, int pocet_pohybov, bool mozny_pohyb_dole, bool mozny_pohyb_vpravo, bool mozny_pohyb_hore, bool mozny_pohyb_vlavo) {
	char posledny_operator[] = stvorec[pocet_pohybov - 1].predchadzajuci_operator;
	
	int cena_hore = -1, cena_dole = -1, cena_vlavo = -1, cena_vpravo = -1;
	if (stvorec[pocet_pohybov - 1].prazdne_miesto == 9) {
		if (strcmp(posledny_operator, "dole") != 0) 	mozny_pohyb_hore = true;
		if (strcmp(posledny_operator, "vpravo") != 0) 	mozny_pohyb_vlavo = true;
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 8) {
		if (strcmp(posledny_operator, "vpravo") != 0) mozny_pohyb_vlavo = true;
		if (strcmp(posledny_operator, "dole") != 0)	mozny_pohyb_hore = true;	
		if (strcmp(posledny_operator, "vlavo") != 0) mozny_pohyb_vpravo = true;	
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 7) {
		if (strcmp(posledny_operator, "dole") != 0) mozny_pohyb_hore = true;
		if (strcmp(posledny_operator, "vlavo") != 0) mozny_pohyb_vpravo = true;
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 6) {
		if (strcmp(posledny_operator, "vpravo") != 0) mozny_pohyb_vlavo = true;
		if (strcmp(posledny_operator, "hore") != 0) mozny_pohyb_dole = true;
		if (strcmp(posledny_operator, "dole") != 0) mozny_pohyb_hore = true;
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 5) {
		if (strcmp(posledny_operator, "dole") != 0) mozny_pohyb_hore = true;
		if (strcmp(posledny_operator, "vpravo") != 0) mozny_pohyb_vlavo = true;
		if (strcmp(posledny_operator, "hore") != 0) mozny_pohyb_dole = true;
		if (strcmp(posledny_operator, "vlavo") != 0) mozny_pohyb_vpravo = true;
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 4) {
		if (strcmp(posledny_operator, "dole") != 0) mozny_pohyb_hore = true;
		if (strcmp(posledny_operator, "vlavo") != 0) mozny_pohyb_vpravo = true;
		if (strcmp(posledny_operator, "hore") != 0) mozny_pohyb_dole = true;
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 3) {
		if (strcmp(posledny_operator, "vpravo") != 0) mozny_pohyb_vlavo = true;
		if (strcmp(posledny_operator, "hore") != 0) mozny_pohyb_dole = true;
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 2) {
		if (strcmp(posledny_operator, "hore") != 0) mozny_pohyb_dole = true;
		if (strcmp(posledny_operator, "vlavo") != 0) mozny_pohyb_vpravo = true;
		if (strcmp(posledny_operator, "vpravo") != 0) mozny_pohyb_vlavo = true;
	}

	else if (stvorec[pocet_pohybov - 1].prazdne_miesto == 1) {
		if (strcmp(posledny_operator, "vlavo") != 0) mozny_pohyb_vpravo = true;
		if (strcmp(posledny_operator, "hore") != 0) mozny_pohyb_dole = true;
	}
}

int realokuj_strukturu(STVOREC** stvorec) {
	STVOREC* lokal = *stvorec;
	int pocet_pohybov = 0;

	lokal[0].pocet_pohybov++;
	pocet_pohybov = lokal[0].pocet_pohybov;

	lokal = (STVOREC*)realloc(lokal, lokal[0].pocet_pohybov * sizeof(STVOREC));
	lokal[pocet_pohybov - 1].prazdne_miesto = -1;
	memset(lokal[pocet_pohybov - 1].predchadzajuce_pohyby, 0, GENERAL_BUFFER_SIZE);
	strcpy(lokal[pocet_pohybov - 1].predchadzajuci_operator, 0);
	lokal[pocet_pohybov - 1].smer_pohybu = 0;

	*stvorec = lokal;		
}

int sprav_pohyb() {

}

int main() {
	int start[] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
	int koniec[] = { 1, 2, 3, 4, 6, 8, 7, 5, 0 };
	int aktualny_stav_stvorca[] = { 0 };
	strcpy(aktualny_stav_stvorca, start);
	int prvy_riadok[] = { 1, 2, 3 };
	int druhy_riadok[] = { 4, 5, 6 };
	int treti_riadok[] = {7, 8, 0};
	int pocet_pohybov = 0;
	bool mozny_pohyb_hore = false, mozny_pohyb_dole = false, mozny_pohyb_vlavo = false, mozny_pohyb_vpravo = false;
	bool nema_riesenie = false;
	STVOREC* stvorec;

	realokuj_strukturu(&stvorec);

	while (strcmp(aktualny_stav_stvorca, koniec) != 0 || true == nema_riesenie) {
		pocet_pohybov++;
		najdi_susedov(stvorec, pocet_pohybov, mozny_pohyb_dole, mozny_pohyb_vpravo, mozny_pohyb_hore, mozny_pohyb_vlavo);
		vypocitaj_cenu(pocet_pohybov, aktualny_stav_stvorca, koniec);
		sprav_pohyb(stvorec);
	}
	//0 je zaciatocny uzol

}

//h(n) ktore nie su na pozicii		g(n) hlbka roota	f(n) obe dokopy

//vystup :
/*Found solution:
down
right
up
left
down
right
up
left*/