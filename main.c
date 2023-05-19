#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#include "managment.c"
#include "data.c"
#include "funcphys.c"
#include "funcenerg.c"
#include "qcount.c"
#include "setup.c"
#include "out.c"


int main(int argc, char *argv[]) {
	char ss[512];


	// генератор случайного, добавляем примеси, 
	setup(argc,argv);

	// запись основных настроек в отдельны файл
	snprintf(ss, 512, "pref.txt");
	savepref(ss);

	// запись примесей в отдельный файл
	snprintf(ss, 512, "primcoord.txt");
	saveprim(ss);




	// int j, i;
	// v[0][0] = realloc(v[0][0], dsn * sizeof(struct vortex));

	// v[0] = realloc(v[0], 10 * sizeof(struct vortex *));
	// for (i = 0; i < 10; i++)
	// 	v[0][i] = malloc(dsn * sizeof(struct vortex));

	// nm = 1;
	// nn[0] = 10;

	// double w = 1.32471795724474602596, a1, a2, x, y, u;

	// a1 = 1.0 / w;
	// a2 = 1.0 / (w * w);

	// for (i = 0; i < 10; i++)
	// {
	// 	x = ((double)rand() - (double)rand()) / RAND_MAX * RP;
	// 	y = ((double)rand() - (double)rand()) / RAND_MAX * RP;
	// 	w = ((0.5 + a1 * i) - (int)(0.5 + a1 * i));
	// 	x = ((x + w > 1) ? (x + w - 1) : ((x + w < 0) ? (x + w + 1) : (x + w))) * X;
	// 	w = ((0.5 + a2 * i) - (int)(0.5 + a2 * i));
	// 	y = ((y + w > 1) ? (y + w - 1) : ((y + w < 0) ? (y + w + 1) : (y + w))) * Y;
	// 	u = (double)rand() / RAND_MAX * m(U01 - U02) + U02;

	// 	for (j = 0; j < dsn; j++)
	// 	{
	// 		v[0][i][j].x = x;
	// 		v[0][i][j].y = y;
	// 		v[0][i][j].z = j * SD;
	// 		v[0][i][j].zn = 'o';
	// 	}
	// }

	// snprintf(ss, 512, "vortex.txt");
	// savev(ss);

	// return 0;






	/////////////////////////////////////////////////////
	// problem:
	// 1. слишком много памяти выделяется в tote3, можно оптимизировать и время
	// 2. паралельность в tote3 не работает
	// 3. отражения не работают
	// 4. параметры и количество примесей одинаковы на всех плоскостях?


	// начальная энергия
	e = malloc(2 * sizeof(double));
	e[1] = e[0] = tote1(v[nm],nn[nm]) + tote2(v[nm],nn[nm]) + tote3old(v[nm],nn[nm]) + tote4(v[nm],nn[nm]) + tote5old_next(v[nm],nn[nm]); // + tote6old_next(v[nm],nn[nm]);

	int i, j; e0 = 0;
	for (i = 0; i < nn[nm]; i++)
		for (j = 0; j < dsn; j++)
			e0 = e0 + v[nm][i][j].e + v[nm][i][j].ez;

	printf("e0: %.16f\ntt: %.16f\n", e[nm], e0 + tote3old(v[nm],nn[nm]));

#if D < 0
	if (nn[nm] > DM) d = nn[nm];
	else d = DM;
#endif

	// основной цикл
S:	while (!kbhit())
	{
		trtr();

		// условие на проверку только каждого физического шага
#if D < 0
		if (k > d + dk)
		{
			dk = k;
			d = (nn[nm] < DM) ? DM : nn[nm];
#else
		if (k > D + dk)
		{
			dk = k;
#endif
			if (stopr()) goto E;
		}
	}
	switch (getch())
	{
		case 's': // остановка и выход из программы
			// todo:
			// show();
			goto S;
		case 'q':
			// todo: сделать сохранение посчитаного до следующего програмного шага
			break;
		case 'p':
			// todo: сделать паузу, скушать твикс
			goto S;
		case 'i':
			// todo: сделать сохранение
			goto S;
		default:
			goto S;
	}


E:	// запись расположения вихрей в отдельный файл
	snprintf(ss, 512, "vortex2.txt");
	savev(ss);

//todo:
//	show();

	e0 = 0;
	for (i = 0; i < nn[nm]; i++)
		for (j = 0; j < dsn; j++)
			e0 = e0 + v[nm][i][j].e + v[nm][i][j].ez;

	printf("---\ne0: %.16f\ntt: %.16f\nt+: %.16f\n", e[nm], tote1(v[nm],nn[nm]) + tote2(v[nm],nn[nm]) + tote3old(v[nm],nn[nm]) + tote4(v[nm],nn[nm]) + tote5old_next(v[nm],nn[nm]), e0 + tote3old(v[nm],nn[nm]));


	return 0;
}
