#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct obraz
{
	char typ[3];
	char nazwa[100];
	int  szer;
	int wys;
	int glebia;
	int **tablica;
};

void wczytaj(struct obraz **obraz, struct obraz **obrazek, int *ilosc)
{
	FILE *plik;
	char nazwa[100];
	int i;
	printf("Podaj nazwe pliku: ");
	scanf("%s", nazwa);
	plik = fopen(nazwa, "r");

	while (plik == NULL)
	{
		printf("Podaj inna nazwe pliku: ");
		scanf("%s", nazwa);
		plik = fopen(nazwa, "r");
	}

	(*ilosc)++;
	*obraz = realloc(*obraz, *ilosc * sizeof *(*obraz));//TABLICA OBRAZOW
	*obrazek = &(*obraz)[*ilosc - 1];
	strcpy((**obrazek).nazwa, nazwa);
	fscanf(plik, "%s\n", &(**obrazek).typ);

	while (fgetc(plik) == '#')
	{
		while (fgetc(plik) != '\n');
	}
	fseek(plik, -1, SEEK_CUR);
	fscanf(plik, "%d %d\n", &(**obrazek).szer, &(**obrazek).wys);
	(**obrazek).tablica = malloc((**obrazek).szer * sizeof(int));

	if ((**obrazek).tablica != NULL)
	{
		for (i = 0; i < (**obrazek).szer; i++)
		{
			(**obrazek).tablica[i] = malloc(((**obrazek).wys + 1) * sizeof(int));

		}
	}

	while (fgetc(plik) == '#')//jestli jest # to skanuj az nie bedzie konca linii
	{
		while (fgetc(plik) != '\n');
	}
	fseek(plik, -1, SEEK_CUR);
	fscanf(plik, "%d\n", &(**obrazek).glebia);
	for (int i = 0; i < (**obrazek).wys; i++)
	{
		if (fgetc(plik) == '#')
		{
			while (fgetc(plik) != '\n');
			i = i - 1;
		}
		else
		{
			fseek(plik, -1, SEEK_CUR);
			for (int j = 0; j < (**obrazek).szer; j++)
			{
				fscanf(plik, "%d ", &(**obrazek).tablica[j][i]);//ZAPELNIENIE TABLICY
			}
		}
	}
	fclose(plik);
}

void zapisz(struct obraz *obrazek)
{
	FILE *plik;
	char nazwadozapisu[100];
	printf("podaj nazwe pod jaka chcesz zapisac plik\n");
	scanf("%s", nazwadozapisu);
	plik = fopen(nazwadozapisu, "w");

	fprintf(plik, "%s \n %d %d \n %d \n", obrazek->typ, obrazek->szer, obrazek->wys, obrazek->glebia);
	for (int i = 0; i < obrazek->wys; i++)
	{

		for (int j = 0; j < obrazek->szer; j++)
		{
			fprintf(plik, "%d ", obrazek->tablica[j][i]);
		}
		fprintf(plik, "\n");
	}
	fclose(plik);


}

void negatyw(struct obraz *obrazek)
{
	int i, j;


	for (i = 0; i < obrazek->wys; i++)
	{
		for (j = 0; j < obrazek->szer; j++)
		{
			obrazek->tablica[j][i] = obrazek->glebia - obrazek->tablica[j][i];
		}
	}

}





void progowanie(struct obraz *obrazek)
{
	int prog;
	printf("podaj wartosc progu");
	scanf("%d", &prog);

	for (int i = 0; i < obrazek->wys; i++)
	{
		for (int j = 0; j < obrazek->szer; j++)
		{
			if (obrazek->tablica[j][i] < prog)
				obrazek->tablica[j][i] = 0;
			else
				obrazek->tablica[j][i] = obrazek->glebia;
		}
	}
}


void pieprzsol(struct obraz *obrazek)
{

	int i, j;
	int k, l, ilosc;
	ilosc = rand() % (obrazek->szer * obrazek->wys) * 10;
	for (i = 0; i < ilosc; i++)
	{
		k = rand() % obrazek->szer;
		l = rand() % obrazek->wys;
		if (rand() % 2 == 0)
			obrazek->tablica[k][l] = 0;
		else
			obrazek->tablica[k][l] = obrazek->glebia;
	}
}
void drukuj(struct obraz *obrazek)
{
	printf("%s\n", obrazek->typ);
	printf("%d %d\n", obrazek->szer, obrazek->wys);
	printf("%d\n", obrazek->glebia);

	for (int i = 0; i < obrazek->wys; i++)
	{
		for (int j = 0; j < obrazek->szer; j++)
		{
			printf("%d ", obrazek->tablica[j][i]);
		}
		printf("\n");
	}
}

void odbicie(int *ilosc, struct obraz **wsk, struct obraz **obrazek)
{
	int x = 0, i, j, temp;

	printf("Jesli chcesz odbic obraz wzgledem osi X nacisnij 0, a jesli wzgledem osi y naciœnij 1\n");
	scanf("%d", &x);

	if (x == 0)
	{
		for (j = 0; j < (*wsk)->wys; j++)
			for (i = 0; i < (*wsk)->szer / 2; i++)
			{
				temp = (*wsk)->tablica[i][j];
				(*wsk)->tablica[i][j] = (*wsk)->tablica[(*wsk)->szer - i - 1][j];
				(*wsk)->tablica[(*wsk)->szer - i - 1][j] = temp;

			}
		printf("pomyslnie odbito obrazek \n");
	}
	else if (x == 1)
	{
		for (i = 0; i < (*wsk)->szer; i++)
			for (j = 0; j < (*wsk)->wys / 2; j++)
			{
				temp = (*wsk)->tablica[i][j];
				(*wsk)->tablica[i][j] = (*wsk)->tablica[i][(*wsk)->wys - j - 1];
				(*wsk)->tablica[i][(*wsk)->wys - j - 1] = temp;
			}
		printf("pomyslnie odbito obrazek \n");
	}
	else
	{
		printf("nie udalo sie odbic, sprobuj ponownie\n");
	}
}

void obrot(int *ilosc, struct obraz **wsk, struct obraz **obrazek, int *ileStopni)
{
	int i, j;
	while (*ileStopni != 0)
	{
		(*obrazek) = realloc(*obrazek, ++(*ilosc) * sizeof(*(*obrazek)));
		strcpy((*obrazek)[*ilosc - 1].nazwa, (*wsk)->nazwa);
		strcpy((*obrazek)[*ilosc - 1].typ, (*wsk)->typ);
		(*obrazek)[*ilosc - 1].szer = (*wsk)->wys;
		(*obrazek)[*ilosc - 1].wys = (*wsk)->szer;
		(*obrazek)[*ilosc - 1].glebia = (*wsk)->glebia;
		(*obrazek)[*ilosc - 1].tablica = calloc((*obrazek)[*ilosc - 1].szer, sizeof(int *));


		for (i = 0; i < (*obrazek)[*ilosc - 1].szer; i++)
		{
			(*obrazek)[*ilosc - 1].tablica[i] = calloc(((*obrazek)[*ilosc - 1].wys + 1), sizeof(int));
		}

		for (j = 0; j < (*obrazek)[*ilosc - 1].wys; j++)
		{
			for (i = 0; i < (*obrazek)[*ilosc - 1].szer; i++)
				(*obrazek)[*ilosc - 1].tablica[i][j] = (*wsk)->tablica[(*wsk)->szer - j - 1][i];//kopia na obrocony
		}
		for (j = 0; j < (*wsk)->szer; j++)
		{
			free((*wsk)->tablica[j]);
		}
		free((*wsk)->tablica);
		(*obrazek)[*ilosc - 2] = (*obrazek)[*ilosc - 1];
		*obrazek = realloc(*obrazek, --(*ilosc) * sizeof(*(*obrazek)));
		*wsk = &(*obrazek)[*ilosc - 1];
		(*ileStopni)--;
	}
}

void gauss(int *ilosc, struct obraz **wsk, struct obraz **obrazek)
{
	int srednia, i, j;
	(*obrazek) = realloc(*obrazek, ++(*ilosc) * sizeof(*(*obrazek)));
	strcpy((*obrazek)[*ilosc - 1].nazwa, (*wsk)->nazwa);
	strcpy((*obrazek)[*ilosc - 1].typ, (*wsk)->typ);
	(*obrazek)[*ilosc - 1].szer = (*wsk)->szer;
	(*obrazek)[*ilosc - 1].wys = (*wsk)->wys;
	(*obrazek)[*ilosc - 1].glebia = (*wsk)->glebia;
	(*obrazek)[*ilosc - 1].tablica = calloc((*obrazek)[*ilosc - 1].szer, sizeof(int *));

	for (i = 0; i < (*obrazek)[*ilosc - 1].szer; i++)
	{
		(*obrazek)[*ilosc - 1].tablica[i] = calloc(((*obrazek)[*ilosc - 1].wys + 1), sizeof(int));
	}

	for (i = 0; i < (*wsk)->szer; i++)
		for (j = 0; j < (*wsk)->wys; j++)
			(*obrazek)[*ilosc - 1].tablica[i][j] = (*wsk)->tablica[i][j];

	*wsk = &(*obrazek)[*ilosc - 1];
	for (i = 0; i < (*wsk)->szer - 3; i++)
		for (j = 0; j < (*wsk)->wys - 3; j++)
		{
			srednia = 0;
			srednia = (*wsk)->tablica[i][j] + (*wsk)->tablica[i + 1][j] * 2 + (*wsk)->tablica[i + 2][j] + (*wsk)->tablica[i][j + 1] * 2 + (*wsk)->tablica[i + 1][j + 1] * 4 + (*wsk)->tablica[i + 2][j + 1] * 2 + (*wsk)->tablica[i][j + 2] + (*wsk)->tablica[i + 1][j + 2] * 2 + (*wsk)->tablica[i + 2][j + 2];
			srednia = srednia / 16;
			(*obrazek)[*ilosc - 2].tablica[i + 1][j + 1] = srednia;
		}
	for (j = 0; j < (*wsk)->szer; j++)
		free((*wsk)->tablica[j]);
	free((*wsk)->tablica);
	*obrazek = realloc(*obrazek, --(*ilosc) * sizeof(*(*obrazek)));
	*wsk = &(*obrazek)[*ilosc - 1];
}

void medianowy(int *ilosc, struct obraz **wsk, struct obraz **obrazek)
{
	int mediana[9], i, j, k, l, temp;
	(*obrazek) = realloc(*obrazek, ++(*ilosc) * sizeof(*(*obrazek)));
	strcpy((*obrazek)[*ilosc - 1].nazwa, (*wsk)->nazwa);
	strcpy((*obrazek)[*ilosc - 1].typ, (*wsk)->typ);
	(*obrazek)[*ilosc - 1].szer = (*wsk)->szer;
	(*obrazek)[*ilosc - 1].wys = (*wsk)->wys;
	(*obrazek)[*ilosc - 1].glebia = (*wsk)->glebia;
	(*obrazek)[*ilosc - 1].tablica = calloc((*obrazek)[*ilosc - 1].szer, sizeof(int *));

	for (i = 0; i < (*obrazek)[*ilosc - 1].szer; i++)
	{
		(*obrazek)[*ilosc - 1].tablica[i] = calloc(((*obrazek)[*ilosc - 1].wys + 1), sizeof(int));

	}

	for (i = 0; i < (*wsk)->szer; i++)
		for (j = 0; j < (*wsk)->wys; j++)
			(*obrazek)[*ilosc - 1].tablica[i][j] = (*wsk)->tablica[i][j];

	*wsk = &(*obrazek)[*ilosc - 1];
	for (i = 0; i < (*wsk)->szer - 3; i++)
		for (j = 0; j < (*wsk)->wys - 3; j++)
		{

			mediana[0] = (*wsk)->tablica[i][j];
			mediana[1] = (*wsk)->tablica[i + 1][j];
			mediana[2] = (*wsk)->tablica[i + 2][j];
			mediana[3] = (*wsk)->tablica[i][j + 1];
			mediana[4] = (*wsk)->tablica[i + 1][j + 1];
			mediana[5] = (*wsk)->tablica[i + 2][j + 1];
			mediana[6] = (*wsk)->tablica[i][j + 2];
			mediana[7] = (*wsk)->tablica[i + 1][j + 2];
			mediana[8] = (*wsk)->tablica[i + 2][j + 2];

			k = 8;
			do
			{
				for (l = 0; l < k; l++)
				{
					if (mediana[l] > mediana[l + 1])
					{
						temp = mediana[l];
						mediana[l] = mediana[l + 1];
						mediana[l + 1] = temp;
					}
				}
				k--;
			} while (k > 1);

			(*obrazek)[*ilosc - 2].tablica[i + 1][j + 1] = mediana[4];
		}
	for (j = 0; j < (*wsk)->szer; j++)
		free((*wsk)->tablica[j]);
	free((*wsk)->tablica);
	*obrazek = realloc(*obrazek, --(*ilosc) * sizeof(*(*obrazek)));
	*wsk = &(*obrazek)[*ilosc - 1];
}

int main()
{
	struct obraz *obraz;
	struct obraz *obrazek = NULL;
	int ilosc = 0, i, j, k = 0, w, aktywny;
	obraz = &obrazek[ilosc];
	int x = 0;
	int koniec = 0;
	printf(" lista obrazow: \n  1.pies.pgm \n");
	wczytaj(&obraz, &obrazek, &ilosc);

	do
	{
		system("cls");
		printf("MENU : \n 1. wczytaj obraz \n 2. zapisz obraz \n 3. stworz histogram i zapisz w pliku csv \n 4. wydrukuj w programie\n 5. Progowanie \n 6. Negatyw \n 7. Pieprz i sol \n 8. obroc o kat k*90 stopni \n 9. odbij wzgledem osi \n 10. filtr Gaussa \n 11.filtr medianowy\n 12.wybierz aktywny\n 13.usun\n 14.koniec\n");
		scanf("%d", &w);
		while (w > 14 || w < 1)
		{
			printf("Wybierz poprawna opcje \n");
			scanf("%d", &w);
		}
		switch (w)
		{
		case 1:
			printf(" lista obrazow: \n  1. ");
			wczytaj(&obraz, &obrazek, &ilosc);
			printf(" wczytano z pliku\n");
			system("pause");
			break;
		case 2:
			zapisz(obrazek);
			printf(" zapisano do pliku\n");
			system("pause");
			break;
		case 3:
			histogram(obrazek);
			printf(" zapisano histogram \n");
			system("pause");
			break;
		case 4:
			drukuj(obrazek);
			printf("wydrukowano obrazek \n");
			system("pause");
			break;
		case 5:
			progowanie(obrazek);
			printf("zastosowano progowanie \n");
			system("pause");
			break;
		case 6:

			negatyw(obrazek);
			printf("zastosowano negatyw \n");
			system("pause");
			break;
		case 7:
			pieprzsol(obrazek);
			printf("wprowadzono zaklocenia pieprz i sol \n");
			system("pause");
			break;
		case 8:
			printf("podaj krotnosc k * 90 stopni o jaka chcesz obrocic obrazek\n");
			scanf("%d", &k); //krotnosc 90
			obrot(&ilosc, &obraz, &obrazek, &k);
			printf("pomyslnie obrocono obrazek \n");
			system("pause");
			break;
		case 9:
			odbicie(&ilosc, &obraz, &obrazek);
			system("pause");
			break;
		case 10:
			gauss(&ilosc, &obraz, &obrazek);
			printf("pomyslnie zastosowano filrt gaussa  \n");
			system("pause");
			break;
		case 11:
			medianowy(&ilosc, &obraz, &obrazek);
			printf("pomyslnie zastosowano filrt medianowy \n");
			system("pause");
			break;
		case 12:
			for (i = 0; i < ilosc; i++)
			{
				obrazek = &obraz[i];
				printf("%d %s\n", i + 1, (*obrazek).nazwa);
			}
			do
			{
				printf("Podaj ktory obraz ma byc aktywny\n");
				scanf("%d", &aktywny);
			} while (aktywny < 1 || aktywny > ilosc);
			aktywny--;
			obrazek = &obraz[aktywny];
			break;

		case 13:
			for (i = 0; i < ilosc; i++)
			{
				obrazek = &obraz[i];
				printf("%d %s\n", i + 1, (*obrazek).nazwa);
			}
			if (ilosc != 1)
			{
				printf("\nKtory obraz ma byc usuniety?\n");
				scanf("%d", &aktywny);
				while (aktywny < 1 || aktywny > ilosc)
				{
					printf("\nKtory obraz ma byc usuniety?\n");
					scanf("%d", &aktywny);
				}
				obrazek = &(obraz)[aktywny - 1];
				for (j = 0; j < obrazek->szer; j++)
					free((obrazek)->tablica[j]);
				free((obrazek)->tablica);
				for (i = aktywny - 1; i < ilosc - 1; i++)
					(obraz)[i] = obraz[i + 1];
				obraz = realloc(obraz, --ilosc * sizeof(*obraz));
			}
			obrazek = &(obraz)[0];
			break;
		case 14:

			for (i = ilosc - 1; i >= 0; i--)
			{
				obrazek = &obraz[i];
				for (j = 0; j < obrazek->szer; j++)
					free(obrazek->tablica[j]);
				free(obrazek->tablica);
				obraz = realloc(obraz, i * sizeof(*obraz));
			}

			koniec++;








			break;
		}


	} while (koniec == 0);
	//drukuj(obrazek);
	//progowanie(obrazek);
	//histogram(obrazek);
	//negatyw(obrazek);
	//pieprzsol(obrazek);
	//odbicie(obrazek);
	//filtrgaussa(obrazek);
	//odbicie(&ilosc, &obraz, &obrazek);
	//scanf("%d", &k); //krotnosc 90
	//obrot(&ilosc, &obraz, &obrazek, &k);
	//gauss(&ilosc, &obraz, &obrazek);
	//medianowy(&ilosc, &obraz, &obrazek);
	//zapisz(obrazek); 
	//wczytaj(&obraz, &obrazek, &ilosc);
	//zapisz(obrazek);
}