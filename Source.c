#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <time.h>

#define GENERAL_BUFFER_SIZE	1000
#define MAX_POLE 1000000

typedef struct Stvorec {
	int index_prazdneho_miesta;
	int stav[9];
	char smer_pohybu[GENERAL_BUFFER_SIZE];
	int pocet_predchodcov_gx;
	int odhad_ceny_do_ciela;
	int cena_celkova;
	int navstiveny;
	char predchadzajuci_operator[GENERAL_BUFFER_SIZE];
	int sucet_vzdialenosti_od_ciela;
	struct Stvorec* predchodca;
	struct Stvorec* dolava;
	struct Stvorec* doprava;
	struct Stvorec* dole;
	struct Stvorec* hore;
}STVOREC;

typedef struct Open_list{
	int dlzka_aktualna;
	STVOREC* pole[MAX_POLE];
}OPEN_LIST;

typedef struct Close_list {
	int dlzka_aktualna;
	STVOREC* pole[MAX_POLE];
}CLOSE_LIST;

//pri heuristike c. 1 spocita pocet policok, ktore nie su na svojom mieste
int vypocitaj_cenu(int aktualny_stav[], int konecny_stav[]) {
	int rozdielne_pozicie = 0;
	int i = 0;
	while (i < 9) {
		if (aktualny_stav[i] != konecny_stav[i] || (aktualny_stav[i] == 0 && konecny_stav[i] == 0)) rozdielne_pozicie++;
		i++;
	}
	if (rozdielne_pozicie == 0) return rozdielne_pozicie;
	rozdielne_pozicie -=1;
	return rozdielne_pozicie;
}

// premiestni medzeru bud hore, dole, doprava alebo dolava
int skusobny_posun(STVOREC* stvorec, char smer_posunu[], int skusobny_stav[]) {
	int index_cisla_v_aktualnom_poli = -1;
	int index_volneho_policka = stvorec->index_prazdneho_miesta;
	int cislo_pred_presunom = -1;

	if (strcmp(smer_posunu, "dole") == 0) 			index_cisla_v_aktualnom_poli = stvorec->index_prazdneho_miesta + 3;
	else if (strcmp(smer_posunu, "hore") == 0)		index_cisla_v_aktualnom_poli = stvorec->index_prazdneho_miesta - 3;
	else if (strcmp(smer_posunu, "vpravo") == 0) 	index_cisla_v_aktualnom_poli = stvorec->index_prazdneho_miesta + 1;
	else if (strcmp(smer_posunu, "vlavo") == 0) 	index_cisla_v_aktualnom_poli = stvorec->index_prazdneho_miesta - 1;
	else printf("Chyba vo funkcii skusobny_posun\n");

	cislo_pred_presunom = skusobny_stav[index_cisla_v_aktualnom_poli];
	skusobny_stav[index_volneho_policka] = cislo_pred_presunom;
	skusobny_stav[index_cisla_v_aktualnom_poli] = 0;
}

// najde smery (doprava, dolava, hore, dole), do ktorych sa moze posunut sucasny stav
int najdi_susedov(STVOREC* aktualny, STVOREC *predchadzajuci, bool *mozny_pohyb_dole, bool *mozny_pohyb_vpravo, bool *mozny_pohyb_hore, bool *mozny_pohyb_vlavo) {
	char posledny_operator[GENERAL_BUFFER_SIZE];
	if (predchadzajuci != NULL) strcpy(posledny_operator, aktualny->predchadzajuci_operator);

	int cena_hore = -1, cena_dole = -1, cena_vlavo = -1, cena_vpravo = -1;

	if (predchadzajuci == NULL) {
		if (aktualny->index_prazdneho_miesta == 8) {
			*mozny_pohyb_hore = true;
			*mozny_pohyb_vlavo = true;	
		}

		else if (aktualny->index_prazdneho_miesta == 7){
			*mozny_pohyb_hore = true;
			*mozny_pohyb_vlavo = true;
			*mozny_pohyb_vpravo = true;
		}

		else if (aktualny->index_prazdneho_miesta == 6) {
			*mozny_pohyb_hore = true;
			*mozny_pohyb_vpravo = true;	
		}

		else if (aktualny->index_prazdneho_miesta == 5) {
			*mozny_pohyb_hore = true;
			*mozny_pohyb_vlavo = true;
			*mozny_pohyb_dole = true;	
		}

		else if (aktualny->index_prazdneho_miesta == 4) {
			*mozny_pohyb_hore = true;
			*mozny_pohyb_vpravo = true;
			*mozny_pohyb_dole = true;
			*mozny_pohyb_vlavo = true;
		}

		else if (aktualny->index_prazdneho_miesta == 3) {
			*mozny_pohyb_hore = true;
			*mozny_pohyb_vpravo = true;
			*mozny_pohyb_dole = true;
		}

		else if (aktualny->index_prazdneho_miesta == 2) {
			*mozny_pohyb_dole = true;
			*mozny_pohyb_vlavo = true;
		}

		else if (aktualny->index_prazdneho_miesta == 1) {
			*mozny_pohyb_dole = true;
			*mozny_pohyb_vpravo = true;
			*mozny_pohyb_vlavo = true;
		}

		else if (aktualny->index_prazdneho_miesta == 0) {
			*mozny_pohyb_dole = true;
			*mozny_pohyb_vpravo = true;
		}
	}

	else {
		if (aktualny->index_prazdneho_miesta == 8) {
			if (strcmp(aktualny->predchadzajuci_operator, "dole") != 0) {
				*mozny_pohyb_hore = true;
			}

			if (strcmp(aktualny->predchadzajuci_operator, "vpravo") != 0) {
				*mozny_pohyb_vlavo = true;	
			}
		}

		else if (aktualny->index_prazdneho_miesta == 7) {
			if (strcmp(posledny_operator, "vpravo") != 0) {
				*mozny_pohyb_vlavo = true;	
			}

			if (strcmp(aktualny->predchadzajuci_operator, "dole") != 0) {
				*mozny_pohyb_hore = true;	
			}

			if (strcmp(aktualny->predchadzajuci_operator, "vlavo") != 0) {
				*mozny_pohyb_vpravo = true;	
			}
		}

		else if (aktualny->index_prazdneho_miesta == 6) {
			if (strcmp(posledny_operator, "dole") != 0) {
				*mozny_pohyb_hore = true;
			}

			if (strcmp(posledny_operator, "vlavo") != 0) {
				*mozny_pohyb_vpravo = true;	
			}
		}

		else if (aktualny->index_prazdneho_miesta == 5) {
			if (strcmp(posledny_operator, "vpravo") != 0) {
				*mozny_pohyb_vlavo = true;
			}

			if (strcmp(posledny_operator, "hore") != 0) {
				*mozny_pohyb_dole = true;	
			}

			if (strcmp(posledny_operator, "dole") != 0) {
				*mozny_pohyb_hore = true;	
			}
		}

		else if (aktualny->index_prazdneho_miesta == 4) {
			if (strcmp(posledny_operator, "dole") != 0) {
				*mozny_pohyb_hore = true;
			}

			if (strcmp(posledny_operator, "vpravo") != 0) {
				*mozny_pohyb_vlavo = true;	
			}

			if (strcmp(posledny_operator, "hore") != 0) {
				*mozny_pohyb_dole = true;	
			}

			if (strcmp(posledny_operator, "vlavo") != 0) {
				*mozny_pohyb_vpravo = true;	
			}
		}

		else if (aktualny->index_prazdneho_miesta == 3) {
			if (strcmp(posledny_operator, "dole") != 0) {
				*mozny_pohyb_hore = true;
			}

			if (strcmp(posledny_operator, "vlavo") != 0) {
				*mozny_pohyb_vpravo = true;	
			}

			if (strcmp(posledny_operator, "hore") != 0) {
				*mozny_pohyb_dole = true;	
			}
		}

		else if (aktualny->index_prazdneho_miesta == 2) {
			if (strcmp(posledny_operator, "vpravo") != 0) {
				*mozny_pohyb_vlavo = true;	
			}

			if (strcmp(posledny_operator, "hore") != 0) {
				*mozny_pohyb_dole = true;	
			}
		}

		else if (aktualny->index_prazdneho_miesta == 1){
			if (strcmp(posledny_operator, "hore") != 0) {
				*mozny_pohyb_dole = true;
			}

			if (strcmp(posledny_operator, "vlavo") != 0) {
				*mozny_pohyb_vpravo = true;	
			}

			if (strcmp(posledny_operator, "vpravo") != 0) {
				*mozny_pohyb_vlavo = true;		
			}
		}

		else if (aktualny->index_prazdneho_miesta == 0) {
			if (strcmp(posledny_operator, "vlavo") != 0) {
				*mozny_pohyb_vpravo = true;
			}

			if (strcmp(posledny_operator, "hore") != 0) {
				*mozny_pohyb_dole = true;
			}
		}
	}
	return 0;
}

// nastavenie dlzky open listu na 0
int open_list_init(OPEN_LIST *open_list) {
	open_list->dlzka_aktualna = 0;
}

// nastavenie dlzky close listu na 0
int close_list_init(CLOSE_LIST* close) {
	close->dlzka_aktualna = 0;
}

// vytvori dalsi stvorec (stav)
int inicializuj_dalsi_tah(STVOREC** aktualny, STVOREC *predchodca) {
	STVOREC* lokal = *aktualny;
	lokal = (STVOREC*)malloc(1 * sizeof(STVOREC));
	lokal->navstiveny = 0;
	if(predchodca != NULL)	strcpy(lokal->predchadzajuci_operator, predchodca->smer_pohybu);
	lokal->dolava = NULL;
	lokal->doprava = NULL;
	lokal->hore = NULL;
	lokal->dole = NULL;
	lokal->sucet_vzdialenosti_od_ciela = -1;
	lokal->pocet_predchodcov_gx = 0;
	lokal->cena_celkova = 0;
	lokal->predchodca = predchodca;
	lokal->odhad_ceny_do_ciela = MAX_POLE;
	*aktualny = lokal;
}

// z open listu vyberie stav, ktory ma najmensiu hodnotu
int vyber_z_open(OPEN_LIST *open, STVOREC **hodnota, int heuristika) {
	int i = 0, min = MAX_POLE, j, index_vyber;
	int cena_do_ciela = 0;

	if (open->dlzka_aktualna == 0)	{
		*hodnota = NULL;
		return 0;
	}

	while (i < open->dlzka_aktualna) {
		j = open->pole[i]->cena_celkova;	
		if (j <= min) {
			index_vyber = i; min = j;
		if (j == min)			// ak mam viac rovnakych cien, vyberam tu, ktora je najblizsie k cielu
			if (open->pole[index_vyber]->odhad_ceny_do_ciela > open->pole[i]->odhad_ceny_do_ciela)
				index_vyber = i;			
			}
		i++;
		}

	*hodnota = open->pole[index_vyber];
	i = index_vyber;

	while (i < open->dlzka_aktualna) {
		open->pole[i] = open->pole[i + 1];
		i++;
	}
	open->dlzka_aktualna--;
}

//prehladava stavy, pokym nenajde riesenie
int najdi_riesenie(OPEN_LIST* open_list, CLOSE_LIST *close_list, STVOREC *aktualny, int start[], int ciel[], int heuristika) {
	STVOREC* koncovy_stav = (STVOREC*)calloc(1, sizeof(STVOREC));
	memcpy(koncovy_stav->stav, ciel, sizeof(koncovy_stav->stav));
	int dalsi_stav[9];

	if (heuristika == 2)
		aktualny->sucet_vzdialenosti_od_ciela = zisti_vzdialenost_policok_od_ciela(aktualny, koncovy_stav);

	pridaj_do_open(open_list, aktualny);

	close_list_init(close_list);

	while (open_list->dlzka_aktualna != 0) {
		vyber_z_open(open_list, &aktualny, heuristika);
		
		if (memcmp(aktualny->stav, koncovy_stav->stav, sizeof(koncovy_stav->stav)) == 0) 
			break;
		bool mozny_pohyb_hore = false, mozny_pohyb_dole = false, mozny_pohyb_vlavo = false, mozny_pohyb_vpravo = false;
		najdi_susedov(aktualny, aktualny->predchodca, &mozny_pohyb_dole, &mozny_pohyb_vpravo, &mozny_pohyb_hore, &mozny_pohyb_vlavo);
		aktualny->navstiveny = 1;

			if (mozny_pohyb_dole) {
				inicializuj_dalsi_tah(&aktualny->dole, aktualny);
			if (!aktualny->dole->navstiveny) {
				aktualny->dole->navstiveny = 1;
				aktualny->dole->predchodca = aktualny;
				memcpy(dalsi_stav, aktualny->stav, sizeof(aktualny->stav));
				if (aktualny->predchodca != NULL)
					aktualny->dole->pocet_predchodcov_gx = aktualny->pocet_predchodcov_gx + 1;
				else aktualny->dole->pocet_predchodcov_gx = 1;

				strcpy(aktualny->dole->predchadzajuci_operator, "dole");
				skusobny_posun(aktualny, "dole", dalsi_stav);
				memcpy(aktualny->dole->stav, dalsi_stav, sizeof(dalsi_stav));
				aktualny->dole->index_prazdneho_miesta = najdi_medzeru(aktualny->dole);

				if (1 == heuristika) {
					aktualny->dole->odhad_ceny_do_ciela = vypocitaj_cenu(aktualny->dole->stav, ciel);
					aktualny->dole->cena_celkova = aktualny->dole->pocet_predchodcov_gx + aktualny->dole->odhad_ceny_do_ciela;
				}

				else if (2 == heuristika) {
					aktualny->dole->sucet_vzdialenosti_od_ciela = zisti_vzdialenost_policok_od_ciela(aktualny->dole, koncovy_stav);
					aktualny->dole->cena_celkova = aktualny->dole->pocet_predchodcov_gx + aktualny->dole->sucet_vzdialenosti_od_ciela;
				}
				pridaj_do_open(open_list, aktualny->dole);
			}
			
			else
				if (aktualny->cena_celkova + aktualny->dole->pocet_predchodcov_gx < aktualny->dole->cena_celkova)
				{
					aktualny->dole->cena_celkova = aktualny->cena_celkova + aktualny->dole->odhad_ceny_do_ciela;
					aktualny->dole->predchodca = aktualny;
				}
		}
		

		if (mozny_pohyb_vlavo) {
			inicializuj_dalsi_tah(&aktualny->dolava, aktualny);
			if (!aktualny->dolava->navstiveny) {
				aktualny->dolava->navstiveny = 1;
				aktualny->dolava->predchodca = aktualny;
				memcpy(dalsi_stav, aktualny->stav, sizeof(aktualny->stav));
				if (aktualny->predchodca != NULL)
					aktualny->dolava->pocet_predchodcov_gx = aktualny->pocet_predchodcov_gx + 1;
				else aktualny->dolava->pocet_predchodcov_gx = 1;

				strcpy(aktualny->dolava->predchadzajuci_operator, "vlavo");
				skusobny_posun(aktualny, "vlavo", dalsi_stav);
				memcpy(aktualny->dolava->stav, dalsi_stav, sizeof(dalsi_stav));

				if (1 == heuristika) {
					aktualny->dolava->odhad_ceny_do_ciela = vypocitaj_cenu(aktualny->dolava->stav, ciel);
					aktualny->dolava->cena_celkova = aktualny->dolava->pocet_predchodcov_gx + aktualny->dolava->odhad_ceny_do_ciela;
				}
				
				else if (2 == heuristika) {
					aktualny->dolava->sucet_vzdialenosti_od_ciela = zisti_vzdialenost_policok_od_ciela(aktualny->dolava, koncovy_stav);
					aktualny->dolava->cena_celkova = aktualny->dolava->pocet_predchodcov_gx + aktualny->dolava->sucet_vzdialenosti_od_ciela;
				}

				aktualny->dolava->index_prazdneho_miesta = najdi_medzeru(aktualny->dolava);
				pridaj_do_open(open_list, aktualny->dolava);
			}
			else
				if (aktualny->cena_celkova + aktualny->dolava->pocet_predchodcov_gx < aktualny->dolava->cena_celkova)
				{
					aktualny->dolava->cena_celkova = aktualny->cena_celkova + aktualny->dolava->odhad_ceny_do_ciela;
					aktualny->dolava->predchodca = aktualny;
				}
		}


		if (mozny_pohyb_hore)
		{
			inicializuj_dalsi_tah(&aktualny->hore, aktualny);
			if (!aktualny->hore->navstiveny) {
				aktualny->hore->navstiveny = 1;
				aktualny->hore->predchodca = aktualny;
				if (aktualny->predchodca != NULL)
					aktualny->hore->pocet_predchodcov_gx = aktualny->pocet_predchodcov_gx + 1;
				else aktualny->hore->pocet_predchodcov_gx = 1;
				
				memcpy(dalsi_stav, aktualny->stav, sizeof(aktualny->stav));
				skusobny_posun(aktualny, "hore", dalsi_stav);
				memcpy(aktualny->hore->stav, dalsi_stav, sizeof(dalsi_stav));
				strcpy(aktualny->hore->predchadzajuci_operator, "hore");

				if (1 == heuristika) {
					aktualny->hore->odhad_ceny_do_ciela = vypocitaj_cenu(aktualny->hore->stav, ciel);
					aktualny->hore->cena_celkova = aktualny->hore->pocet_predchodcov_gx + aktualny->hore->odhad_ceny_do_ciela;
				}
				
				else if (2 == heuristika) {
					aktualny->hore->sucet_vzdialenosti_od_ciela = zisti_vzdialenost_policok_od_ciela(aktualny->hore, koncovy_stav);
					aktualny->hore->cena_celkova = aktualny->hore->pocet_predchodcov_gx + aktualny->hore->sucet_vzdialenosti_od_ciela;
				}

				aktualny->hore->index_prazdneho_miesta = najdi_medzeru(aktualny->hore);
				pridaj_do_open(open_list, aktualny->hore);
			}
			else
				if (aktualny->cena_celkova + aktualny->hore->pocet_predchodcov_gx < aktualny->hore->cena_celkova)
				{
					aktualny->hore->cena_celkova = aktualny->cena_celkova + aktualny->hore->odhad_ceny_do_ciela;
					aktualny->hore->predchodca = aktualny;
				}
		}
		if (mozny_pohyb_vpravo)
		{
			inicializuj_dalsi_tah(&aktualny->doprava, aktualny);
			if (!aktualny->doprava->navstiveny) {
				aktualny->doprava->navstiveny = 1;
				aktualny->doprava->predchodca = aktualny;
				if (aktualny->predchodca != NULL)
					aktualny->doprava->pocet_predchodcov_gx = aktualny->pocet_predchodcov_gx + 1;
				else aktualny->doprava->pocet_predchodcov_gx = 1;

				strcpy(aktualny->doprava->predchadzajuci_operator, "vpravo");
				memcpy(dalsi_stav, aktualny->stav, sizeof(aktualny->stav));
				skusobny_posun(aktualny, "vpravo", dalsi_stav);
				memcpy(aktualny->doprava->stav, dalsi_stav, sizeof(dalsi_stav));

				if (1 == heuristika) {
					aktualny->doprava->odhad_ceny_do_ciela = vypocitaj_cenu(aktualny->doprava->stav, ciel);
					aktualny->doprava->cena_celkova = aktualny->doprava->pocet_predchodcov_gx + aktualny->doprava->odhad_ceny_do_ciela;
				}

				else if (2 == heuristika) {
					aktualny->doprava->sucet_vzdialenosti_od_ciela = zisti_vzdialenost_policok_od_ciela(aktualny->doprava, koncovy_stav);
					aktualny->doprava->cena_celkova = aktualny->doprava->pocet_predchodcov_gx + aktualny->doprava->sucet_vzdialenosti_od_ciela;
				}

				aktualny->doprava->index_prazdneho_miesta = najdi_medzeru(aktualny->doprava);
				pridaj_do_open(open_list, aktualny->doprava);
			}
			else
				if (aktualny->cena_celkova + aktualny->doprava->pocet_predchodcov_gx < aktualny->doprava->cena_celkova)
				{
					aktualny->doprava->cena_celkova = aktualny->cena_celkova + aktualny->doprava->odhad_ceny_do_ciela;
					aktualny->doprava->predchodca = aktualny;
				}
		}
		pridaj_do_close(close_list, aktualny);
	}

	vypis_operatory_od_zaciatku(aktualny);
}

// vypisuje postupne operatory
int vypis_operatory_od_zaciatku(STVOREC* posledny)
{
	if (posledny == NULL)
		return;

	vypis_operatory_od_zaciatku(posledny->predchodca);

	printf("%s\n", posledny->predchadzajuci_operator);
}

// pridava prehladane stavy do close listu
int pridaj_do_close(CLOSE_LIST *close, STVOREC *hodnota) {
	close->pole[close->dlzka_aktualna] = hodnota;
	close->dlzka_aktualna++;
}

// pridava objavene, ale este nepreskumane stav do open listu
int pridaj_do_open(OPEN_LIST* open, STVOREC *hodnota) {
	if(open->dlzka_aktualna < MAX_POLE)
		open->pole[open->dlzka_aktualna] = hodnota;
	open->dlzka_aktualna++;
}

//hlada 0 (medzeru) v sucasnom stave stvorca
int najdi_medzeru(STVOREC* stvorec) {
	int i = 0;
	while (stvorec->stav[i] != 0) 
		i++;
	
	return i;
}


//zistuje suradnice cisla na urcitom indexe
int zisti_suradnice(int index, int *x, int *y) {
	if (index == 0) {
		*x = 1;
		*y = 1;
	}

	else if (index == 1) {
		*x = 1;
		*y = 2;
	}

	else if (index == 2) {
		*x = 1;
		*y = 3;
	}

	else if (index == 3) {
		*x = 2;
		*y = 1;
	}

	else if (index == 4) {
		*x = 2;
		*y = 2;
	}

	else if (index == 5) {
		*x = 2;
		*y = 3;
	}

	else if (index == 6) {
		*x = 3;
		*y = 1;
	}

	else if (index == 7) {
		*x = 3;
		*y = 2;
	}

	else if (index == 8) {
		*x = 3;
		*y = 3;
	}
	return 0;
}

// vypocita sucet vzdialenosti policok od ich cielovej pozicie
int zisti_vzdialenost_policok_od_ciela(STVOREC* aktualny, STVOREC* koniec) {
	int i = 0, xn = -1, yn = -1, xg = -1, yg = -1, j = 0, celkova_suma = 0;

	while (i < 9) {
		if (aktualny->stav[i] != koniec->stav[i] && aktualny->stav[i] != 0) {		
			zisti_suradnice(i, &xn, &yn);
			while (aktualny->stav[i] != koniec->stav[j]) j++;
			zisti_suradnice(j, &xg, &yg);
			celkova_suma += abs(xn - xg) + abs(yn - yg);
		}
		j = 0;
		i++;
	}

	return celkova_suma;
}

int test1(int start[], int ciel[]) {
	start[0] =  2,	 
	start[1] = 8;
	start[2] = 1;
	start[3] = 0;
	start[4] = 4;
	start[5] = 3;
	start[6] = 7;
	start[7] = 6;
	start[8] = 5;
	ciel[0] = 1;
	ciel[1] = 2;
	ciel[2] = 3;
	ciel[3] = 8;
	ciel[4] = 0;
	ciel[5] = 4;
	ciel[6] = 7;
	ciel[7] = 6;
	ciel[8] = 5;
	
	return 0;
}

int main() {
	clock_t start_cas, end;
	double time_spent = 0;
	int start[9] = {0};
	int koniec[9] = {0};
	int heuristika = 0;
	int i = 0;
	STVOREC* stvorec = NULL;
	OPEN_LIST* open_list = (OPEN_LIST*)calloc(1, sizeof(OPEN_LIST));
	CLOSE_LIST* close_list = (CLOSE_LIST*)calloc(1, sizeof(CLOSE_LIST));

	stvorec = (STVOREC*)calloc(1, sizeof(STVOREC));

	/*printf("Zadajte zaciatocny stav:\n");
	for (i = 0; i < 9; i++) {
		scanf("%d", &start[i]);
	}

	printf("Zadajte cielovy stav:\n");
	for (i = 0; i < 9; i++) {
		scanf("%d", &koniec[i]);
	}*/
	
	test1(&start, &koniec);

	printf("Vyber heuristiky: \n");
	scanf("%d", &heuristika);


	memcpy(stvorec->stav, start, sizeof(stvorec->stav));
	open_list_init(open_list);

	start_cas = clock();
	stvorec->index_prazdneho_miesta = najdi_medzeru(stvorec);


	najdi_riesenie(open_list, close_list, stvorec, start, koniec, heuristika);
	end = clock();
	time_spent = (double)(end - start_cas) / CLOCKS_PER_SEC;
	printf("Cas %f\n", time_spent);

	return 0;
}