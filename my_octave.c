// Copyright Dragan Dragos Ovidiu 313CAb 2022-2023

#include <stdio.h>
#include <stdlib.h>

void alocare(int ****a, int k, int **linie, int **coloana, int m, int n)
{
	(*a) = (int ***)realloc((*a), (k + 1) * (sizeof(int **)));
	// aloca o noua matrice in cub

	(*a)[k] = (int **)calloc(m, sizeof(int *)); // aloca m linii in matricea k
	for (int j = 0; j < m; j++)
		(*a)[k][j] = (int *)calloc(n, sizeof(int));
	// aloca elemtele de pe coloana

	(*linie) = (int *)realloc((*linie), (k + 1) * sizeof(int));
	(*linie)[k] = m; // pentru a retine nr de linii la fiecare matrice citita

	(*coloana) = (int *)realloc((*coloana), (k + 1) * sizeof(int));
	(*coloana)[k] = n; // pentru a retine nr de coloane la fiecare matr. citita
}

void interschimbare(int ***a, int *linie, int *coloana, int z, int w)
{
	int **aux2 = a[z]; // interschimb matricele
	a[z] = a[w];
	a[w] = aux2;
	int aux3 = linie[z]; // interschimb dimensiunile
	linie[z] = linie[w];
	linie[w] = aux3;
	int aux4 = coloana[z];
	coloana[z] = coloana[w];
	coloana[w] = aux4;
}

void matricea_unitate(int ****a, int **linie, int **coloana, int index)
{
	for (int i = 0; i < (*linie)[index]; i++)
		for (int j = 0; j < (*coloana)[index]; j++)
			if (i == j)
				(*a)[index][i][i] = 1;
			else
				(*a)[index][i][j] = 0;
}

int modulo(int **a, int i, int j)
{
	int y = 10 * 10 * 10 * 10 + 7;

	if (a[i][j] % y < 0)
		a[i][j] = a[i][j] % y + y;
	else
		a[i][j] = a[i][j] % y;

	return a[i][j];
}

void citire(int ****a, int k, int **linie, int **coloana)
{
	int n, m;
	scanf("%d%d", &m, &n); // m linii si n coloane

	alocare(a, k, linie, coloana, m, n);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			scanf("%d", &((*a)[k][i][j]));
}

void dimensiune(int k, int *linie, int *coloana)
{
	int index;
	scanf("%d", &index);

	if (index < k)
		printf("%d %d\n", linie[index], coloana[index]);
	else
		printf("No matrix with the given index\n");
}

void afisare(int ***a, int *linie, int *coloana, int k)
{
	int index;
	scanf("%d", &index);

	if (index >= 0 && index < k) {
		for (int i = 0; i < linie[index]; i++) {
			for (int j = 0; j < coloana[index]; j++)
				printf("%d ", a[index][i][j]);
			printf("\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

void dealocare_matrice(int ***a, int k, int **linie, int **coloana, int index)
{
	if (index < k) {
		for (int j = 0; j < (*linie)[index]; j++)
			free(a[index][j]);
		free(a[index]); // dealoc matricea

		for (int i = index; i < (k - 1); i++)
			a[i] = a[i + 1]; // decalez matricele cu p o pozitie in minus

		for (int i = index; i < (k - 1); i++)
			(*linie)[i] = (*linie)[i + 1]; // decalez elem in vectorul de linii

		(*linie) = (int *)realloc((*linie), (k - 1) * sizeof(int));

		for (int j = index; j < (k - 1); j++)
			(*coloana)[j] = (*coloana)[j + 1]; // decalez elem in vect de col.

		(*coloana) = (int *)realloc((*coloana), (k - 1) * sizeof(int));
	}
}

void redimensionare(int ****a, int k, int **linie, int **coloana, int mat)
{
	int lin, col;
	scanf("%d", &lin);
	int *vlin;
	vlin = (int *)malloc(lin * sizeof(int));

	for (int i = 0; i < lin; i++)
		scanf("%d", &vlin[i]);

	scanf("%d", &col);
	int *vcol;
	vcol = (int *)malloc(col * sizeof(int));

	for (int i = 0; i < col; i++)
		scanf("%d", &vcol[i]);

	if (mat >= 0 && mat < k) {
		alocare(a, k, linie, coloana, lin, col);
		k++;
		for (int i = 0; i < lin; i++)
			for (int j = 0; j < col; j++)
				(*a)[k - 1][i][j] = (*a)[mat][vlin[i]][vcol[j]];
		// am copiat elementele in penultima matrice

		interschimbare(*a, *linie, *coloana, mat, k - 1);
		dealocare_matrice(*a, k, linie, coloana, k - 1);
		k--;

	} else {
		printf("No matrix with the given index\n");
	}
	free(vlin);
	free(vcol);
}

void produs_mat(int ***a, int k, int *linie, int *coloana, int mat1, int mat2)
{
	int y = 10 * 10 * 10 * 10 + 7, produs = 0;

	for (int i = 0; i < linie[mat1]; i++)
		for (int j2 = 0; j2 < coloana[mat2]; j2++) {
			for (int i2 = 0; i2 < linie[mat2]; i2++) {
				produs = (a[mat1][i][i2] * a[mat2][i2][j2]) % y;
				a[k][i][j2] = (a[k][i][j2] + produs) % y;
				// suma de produse
			}
			a[k][i][j2] = modulo(a[k], i, j2);
		}
}

void ordonare(int ***a, int k, int *linie, int *coloana)
{
	int y = 10 * 10 * 10 * 10 + 7;
	long s[k]; // vector cu suma tuturor elem. matricelor

	for (int i = 0; i < k; i++)
		s[i] = 0;

	for (int z = 0; z < k; z++)
		for (int i = 0; i < linie[z]; i++)
			for (int j = 0; j < coloana[z]; j++) {
				s[z] = (s[z] + a[z][i][j]) % y;
				if (s[z] % y < 0)
					s[z] = s[z] % y + y;
				else
					s[z] = s[z] % y;
			}

	for (int i = 0; i < k - 1; i++)
		for (int j = i + 1; j < k; j++)
			if (s[i] > s[j]) {
				int aux1 = s[i]; // interschimb vectorul de sume
				s[i] = s[j];
				s[j] = aux1;
				interschimbare(a, linie, coloana, i, j);
			}
}

void transpusa(int ****a, int k, int **linie, int **coloana)
{
	int index;
	scanf("%d", &index);

	if (index < k) {
		alocare(a, k, linie, coloana, (*coloana)[index], (*linie)[index]);
		k++;
		// formez o matrice ajutatoare transpusa pe pozitia k - 1

		for (int i = 0; i < (*linie)[index]; i++)
			for (int j = 0; j < (*coloana)[index]; j++)
				(*a)[k - 1][j][i] = (*a)[index][i][j];

		interschimbare(*a, *linie, *coloana, index, k - 1);

		dealocare_matrice((*a), k, linie, coloana, k - 1);
		k--;

	} else {
		printf("No matrix with the given index\n");
	}
}

void ridicare_putere(int ****a, int k, int **lin, int **col)
{
	int index, p;
	scanf("%d%d", &index, &p); // citesc matricea si puterea

	if (index >= 0 && index < k) {  // daca exista indexul
		if (p >= 0) {  // daca puterea este pozitiva
			if ((*lin)[index] == (*col)[index]) { //daca e patratica
				alocare(a, k, lin, col, (*lin)[index], (*col)[index]);
				k++;
				// fac o matrice ajutatoare
				matricea_unitate(a, lin, col, k - 1);

				while (p) {
					if (p % 2 != 0) {
						alocare(a, k, lin, col, (*lin)[index], (*col)[index]);
						// aloc o matrice pentru produs
						produs_mat(*a, k, (*lin), (*col), k - 1, index);
						k++;
						int **aux = (*a)[k - 2];
						(*a)[k - 2] = (*a)[k - 1];
						(*a)[k - 1] = aux;
						// interschimb matricele( cea ajutatoare cu produsul)
						dealocare_matrice(*a, k, lin, col, k - 1);
						k--;
						// dealoc matricea veche
					}
					alocare(a, k, lin, col, (*lin)[index], (*col)[index]);
					produs_mat(*a, k, (*lin), (*col), index, index);
					k++;
					interschimbare(*a, (*lin), (*col), index, k - 1);
					// intereschimb pentru ca matrice = matrice * matrice;
					dealocare_matrice(*a, k, lin, col, k - 1);
					k--;
					// dealoc matricea initiala (cea cu valoarea veche)

					p = p / 2;
				}
				for (int i = 0; i < (*lin)[index]; i++)
					for (int j = 0; j < (*col)[index]; j++)
						(*a)[index][i][j] = (*a)[k - 1][i][j];
				// cop in matricea de pe pozitia "index" valorile din ult matr.
				// (cea ajutatoare)
				dealocare_matrice(*a, k, lin, col, k - 1);
				k--;
				// sterg matricea ajutatoare
			} else {
				printf("Cannot perform matrix multiplication\n");
			}
		} else {
			printf("Power should be positive\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

void alocare2(int ***a, int n)
{
	// aloc o singura matrice dinamic
	(*a) = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++)
		(*a)[i] = (int *)malloc(n * sizeof(int)); // aloc dinamic o matrice
}

void eliberare2(int **a, int n)
{
	// dealoc o singura matrice
	for (int j = 0; j < n; j++)
		free(a[j]);
	free(a);
}

void alocare_stras(int ***m1, int ***m2, int ***m3, int ***m4, int n)
{
	// aloc dinamic mai multe matrice simultan
	alocare2(m1, n);
	alocare2(m2, n);
	alocare2(m3, n);
	alocare2(m4, n);
}

void eliberare_stras(int **m1, int **m2, int **m3, int **m4, int **m5, int n)
{
	// dealoc simultan mai multe matrice
	for (int i = 0; i < n; i++) {
		free(m1[i]);
		free(m2[i]);
		free(m3[i]);
		free(m4[i]);
		free(m5[i]);
	}
	free(m1);
	free(m2);
	free(m3);
	free(m4);
	free(m5);
}

int **suma_matrice(int **mat1, int **mat2, int n)
{
	// stochez in mat3 suma a doua matrice
	int **mat3;
	alocare2(&mat3, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			mat3[i][j] = mat1[i][j] + mat2[i][j];
			mat3[i][j] = modulo(mat3, i, j);
		}
	return mat3;
}

int **dif_matrice(int **mat1, int **mat2, int n)
{
	// stochez in mat3 diferenta a doua matrice
	int **mat3;
	alocare2(&mat3, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			mat3[i][j] = mat1[i][j] - mat2[i][j];
			mat3[i][j] = modulo(mat3, i, j);
		}
	return mat3;
}

void cadranul1(int **c, int **m1, int **m4, int **m5, int **m7, int n)
{
	// parcurg cadranul 1 pentru a face diferenta dintre m1 + m4 si m5 +m7
	for (int i = 0; i < n / 2; i++)
		for (int j = 0; j < n / 2; j++) {
			c[i][j] = m1[i][j] + m4[i][j] - m5[i][j] + m7[i][j];
			c[i][j] = modulo(c, i, j);
		}
}

void cadranul2(int **c, int **m3, int **m5, int n)
{
	// parcurg cadranul 2 pentru a face suma dintre m3 si m5
	for (int i = 0; i < n / 2; i++)
		for (int j = n / 2; j < n; j++) {
			c[i][j] = m3[i][j - n / 2] + m5[i][j - n / 2];
			c[i][j] = modulo(c, i, j);
		}
}

void cadranul3(int **c, int **m2, int **m4, int n)
{
	// parcurg cadranul 3 pentru a face suma dintre m2 si m4
	for (int i = n / 2; i < n; i++)
		for (int j = 0; j < n / 2; j++) {
			c[i][j] = m2[i - n / 2][j] + m4[i - n / 2][j];
			c[i][j] = modulo(c, i, j);
		}
}

void cadranul4(int **c, int **m1, int **m2, int **m3, int **m6, int n)
{
	// parcurg cadranul 4 pentru a face suma dintre m3 + 6 si m1 - m2
	int s = 0, d = 0;
	for (int i = n; i < 2 * n; i++) // cadranul 4
		for (int j = n; j < 2 * n; j++) {
			s = m3[i - n][j - n] + m6[i - n][j - n];
			d = m1[i - n][j - n] - m2[i - n][j - n];
			c[i][j] = d + s;
			c[i][j] = modulo(c, i, j);
		}
}

int **strassen(int **mat1, int **mat2, int n)
{
	if (n == 2) { // pentru o matrice de 2 pe 2
		int **c;// am alocat dinamic matricea C
		alocare2(&c, n);

		int m1 = (mat1[0][0] + mat1[1][1]) * (mat2[0][0] + mat2[1][1]);
		int m2 = (mat1[1][0] + mat1[1][1]) * mat2[0][0];
		int m3 = mat1[0][0] * (mat2[0][1] - mat2[1][1]);
		int m4 = mat1[1][1] * (mat2[1][0] - mat2[0][0]);
		int m5 = (mat1[0][0] + mat1[0][1]) * mat2[1][1];
		int m6 = (mat1[1][0] - mat1[0][0]) * (mat2[0][0] + mat2[0][1]);
		int m7 = (mat1[0][1] - mat1[1][1]) * (mat2[1][0] + mat2[1][1]);

		c[0][0] = m1 + m4 - m5 + m7, c[0][1] = m3 + m5; // formez matricea C
		c[1][0] = m2 + m4, c[1][1] = m1 - m2 + m3 + m6;

		return c;

	} else {
		int **a1, **a2, **a3, **a4, **b1, **b2, **b3, **b4, **aux1, **aux2;
		int **aux3, **aux4, **aux5, **aux6, **aux7, **aux8, **aux9, **aux10;

		alocare_stras(&a1, &a2, &a3, &a4, n / 2);// aloc matrice care
		alocare_stras(&b1, &b2, &b3, &b4, n / 2);// formeaza cadrane

		for (int i = 0; i < n / 2; i++) // cadranul 1
			for (int j = 0; j < n / 2; j++) {
				a1[i][j] = mat1[i][j];
				b1[i][j] = mat2[i][j];
			}
		for (int i = 0; i < n / 2; i++) // cadranul 2
			for (int j = n / 2; j < n; j++) {
				a2[i][j - n / 2] = mat1[i][j];
				b2[i][j - n / 2] = mat2[i][j];
			}
		for (int i = n / 2; i < n; i++) // cadranul 3
			for (int j = 0; j < n / 2; j++) {
				a3[i - n / 2][j] = mat1[i][j];
				b3[i - n / 2][j] = mat2[i][j];
			}
		for (int i = n / 2; i < n; i++) // cadranul 4
			for (int j = n / 2; j < n; j++) {
				a4[i - n / 2][j - n / 2] = mat1[i][j];
				b4[i - n / 2][j - n / 2] = mat2[i][j];
			}

		aux1 = suma_matrice(a1, a4, n / 2); // formez matrice auxiliare pentru
		aux2 = suma_matrice(b1, b4, n / 2);// a le putea dealoca la final
		aux3 = suma_matrice(a3, a4, n / 2);
		aux4 = dif_matrice(b2, b4, n / 2);
		aux5 = dif_matrice(b3, b1, n / 2);
		aux6 = suma_matrice(a1, a2, n / 2);
		aux7 = dif_matrice(a3, a1, n / 2);
		aux8 = suma_matrice(b1, b2, n / 2);
		aux9 = dif_matrice(a2, a4, n / 2);
		aux10 = suma_matrice(b3, b4, n / 2);

		int **m1 = strassen(aux1, aux2, n / 2);
		int **m2 = strassen(aux3, b1, n / 2);
		int **m3 = strassen(a1, aux4, n / 2);
		int **m4 = strassen(a4, aux5, n / 2);
		int **m5 = strassen(aux6, b4, n / 2);
		int **m6 = strassen(aux7, aux8, n / 2);
		int **m7 = strassen(aux9, aux10, n / 2);

		int **c; // am alocat dinamic matricea C
		alocare2(&c, n);

		cadranul1(c, m1, m4, m5, m7, n); // formez matrice in functie de cadran
		cadranul2(c, m3, m5, n);
		cadranul3(c, m2, m4, n);
		cadranul4(c, m1, m2, m3, m6, n / 2);

		eliberare_stras(a1, a2, a3, a4, b1, n / 2);// eliberez toate matricele
		eliberare_stras(b2, b3, b4, m1, m2, n / 2);// pe care le am folosit
		eliberare_stras(m3, m4, m5, m6, m7, n / 2);
		eliberare_stras(aux1, aux2, aux3, aux4, aux5, n / 2);
		eliberare_stras(aux6, aux7, aux8, aux9, aux10, n / 2);
		return c;
	}
}

void alocare_C(int ****a, int k, int **linie, int **coloana, int mat)
{
	// aloc dinamic o matrice pentru produsul dintre cele 2 matrice
	// in urma apelarii functiei "strassen", la finalul sirului de matrice
	(*a) = (int ***)realloc((*a), (k + 1) * (sizeof(int **)));
	(*linie) = (int *)realloc((*linie), (k + 1) * sizeof(int));
	(*linie)[k] = (*linie)[mat];
	// pentru a retine nr de linii de la matricea citita
	(*coloana) = (int *)realloc((*coloana), (k + 1) * sizeof(int));
	(*coloana)[k] = (*coloana)[mat];
	// pentru a retine nr de coloane la  matricea citita
}

void eliberare_totala(int ***a, int k, int *linie, int *coloana)
{
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < linie[i]; j++)
			free(a[i][j]); //dealoc liniile matricelor
		free(a[i]); //dealoc matricea
	}
	free(a); //dealoc cubul

	free(linie); // dealoc vectorul de linii
	free(coloana); // dealoc vectorul de coloane
}

int main(void)
{
	char l;
	int k = 0, ***a = NULL, *i = NULL, *j = NULL;

	while (scanf("%c", &l) && l != 'Q') {
		if ((l == ' ') || (l == '\n'))
			continue;

		if (l == 'L') { // operatia L
			citire(&a, k, &i, &j);
			k++;

		} else if (l == 'D') { // operatia D
			dimensiune(k, i, j);

		} else if (l == 'P') { // operatia P
			afisare(a, i, j, k);

		} else if (l == 'C') { // operatia C
			int mat;
			scanf("%d", &mat);
			redimensionare(&a, k, &i, &j, mat);

		} else if (l == 'M') { // operatia M
			int mat1, mat2; //cele 2 matrice citite
			scanf("%d%d", &mat1, &mat2);
			if (mat1 >= 0 && mat1 < k && mat2 >= 0 && mat2 < k) {
				if (j[mat1] == i[mat2]) {
					alocare(&a, k, &i, &j, i[mat1], j[mat2]);
					produs_mat(a, k, i, j, mat1, mat2);
					k++;
				} else {
					printf("Cannot perform matrix multiplication\n");
				}
			} else {
				printf("No matrix with the given index\n");
			}

		} else if (l == 'O') { // operatia O
			ordonare(a, k, i, j);

		} else if (l == 'T') { // operatia T
			transpusa(&a, k, &i, &j);

		} else if (l == 'R') { // operatia R
			ridicare_putere(&a, k, &i, &j);

		} else if (l == 'F') { // operatia F
			int mat;
			scanf("%d", &mat);

			if (mat >= 0 && mat < k) {
				dealocare_matrice(a, k, &i, &j, mat);
				k--;

			} else {
				printf("No matrix with the given index\n");
			}

		} else if (l == 'S') { // operatia S
			int mat1, mat2;
			scanf("%d %d", &mat1, &mat2);

			if (mat1 >= 0 && mat1 < k && mat2 >= 0 && mat2 < k) {
				if (i[mat1] == j[mat2]) {
					alocare_C(&a, k, &i, &j, mat1);
					k++;
					a[k - 1] = strassen(a[mat1], a[mat2], i[mat1]);
				} else {
					printf("Cannot perform matrix multiplication");
				}
			} else {
				printf("No matrix with the given index");
			}
		} else {
			printf("Unrecognized command\n");
		}
	}
	eliberare_totala(a, k, i, j); // operatia Q
}
