// программа показывает статус при нажатии 's'
// void show() {
// 	system("clear");
// 	printf("шагов: %d / %llu\nвихрей: %d\ntotal: %.16f\n+++++: %.16f\n", nm, k, nn[nm], tote1(v[nm],nn[nm]) + tote2(v[nm],nn[nm]) + tote3old(v[nm],nn[nm]) + tote4(v[nm],nn[nm]) + tote5old_next(v[nm],nn[nm]) + tote6old_next(v[nm],nn[nm]), e[nm]);
// 	fflush(stdout);
// }

// // создаёт случайный новый вихрь в vortex *a (vortex *a имеет размеры n)
// struct vortex *new(struct vortex *a, int *n) {
// 	char zn = rand() % 2 ? 'o' : 'x';
// 	double x, y;

// #if XG & 2
// 	x = (double)rand() / RAND_MAX * (BRN - DRN) + (rand() % 2 ? DRN : X - L0);
// #else
// 	x = (double)rand() / RAND_MAX * X;
// #endif
// #if XG & 1
// 	y = (double)rand() / RAND_MAX * (BRN - DRN) + (rand() % 2 ? DRN : Y - L0);
// #else
// 	y = (double)rand() / RAND_MAX * Y;
// #endif

// 	if (check(a,*n,x,y)) return 0;

// 	a = realloc(a, (*n + 1) * sizeof(struct vortex));

// 	a[*n].x = x;
// 	a[*n].y = y;
// 	a[*n].zn = zn;
// 	a[*n].t = temp(x,y);
// 	a[*n].h = pole(x,y);
// 	a[*n].l = lamb(a[*n].t);
// 	a[*n].xi = ksii(a[*n].t);
// 	a[*n].e = ((a[*n].zn == 'o') ? 1 : -1) * e1(a[*n].h,a[*n].l,a[*n].x,a[*n].y) + e2(a[*n].l,a[*n].xi) + eone4(a,*n);

// 	*n = *n + 1;

// 	return a;
// }

// ветка основного алгорима, распределение подпроцессов
void trtr() {
	double s, g;
	int i, j;

	e0 = 0;
	k++;

//	i = rand() % RND;
	i = 2;
	if (i != 0 && nn[nm] != 0)
	{
		switch (i)
		{
			case 0:	// удаление вихря
			case 1:	// добавление вихря
// 				struct vortex *a;

// //				v[nm] = new(v[nm],&nn[nm]);
// 				a = new(v[nm],&nn[nm]);
// 				if (!a) break;
// 				v[nm] = a;

// 				e0 = eone3(v[nm],nn[nm]) + v[nm][nn[nm] - 1].e;

// 				g = (double)TA / (nn[nm] + 1) * exp(-e0 / v[nm][nn[nm] - 1].t);
// 				if (g < 1)
// 				{
// 					if ((double)rand() / RAND_MAX > g)
// 					{
// 						v[nm] = realloc(v[nm], nn[nm] * sizeof(struct vortex));
// 						nn[nm]--;
// 						e0 = 0;
// 					}
// 				}
// 				break;
			case 2:	// перемещение вихрей
				double x, y, t, h, l, xi, gz;

				i = rand() % nn[nm];
				do {
					j = rand() % dsn;
				} while (!v[nm][i][j].zn);

				do {
					x = (double)rand() / RAND_MAX * 2 * DP * v[nm][i][j].l - DP * v[nm][i][j].l;
					y = (double)rand() / RAND_MAX * 2 * DP * v[nm][i][j].l - DP * v[nm][i][j].l;
				} while (x * x + y * y > DP * v[nm][i][j].l);
// #if XG & 2
// 				if (v[nm][i][j].x + x < 0)
// 					break;
// 				else
// 					if (v[nm][i][j].x + x >= X)
// 						break;
// 					else
// 						x = v[nm][i][j].x + x;
// #else
				x = v[nm][i][j].x + x;
				x = (x < 0) ? (X + x) : ((x >= X) ? (x - X) : x);
// #endif
// #if XG & 1
// 				if (v[nm][i][j].y + y < 0)
// 					break;
// 				else
// 					if (v[nm][i][j].y + y >= Y)
// 						break;
// 					else
// 						y = v[nm][i][j].y + y;
// #else
				y = v[nm][i][j].y + y;
				y = (y < 0) ? (Y + y) : ((y >= Y) ? (y - Y) : y);
// #endif
				if (check(v[nm],nn[nm],x,y,v[nm][i][j].z)) break;


				e0 = eone3old(v[nm],nn[nm],i,j) + v[nm][i][j].e + v[nm][i][j].ez;
				t = v[nm][i][j].t; v[nm][i][j].t = temp(x,y,v[nm][i][j].z);
				h = v[nm][i][j].h; v[nm][i][j].h = pole(x,y,v[nm][i][j].z);
				g = v[nm][i][j].x; v[nm][i][j].x = x; x = g;
				g = v[nm][i][j].y; v[nm][i][j].y = y; y = g;
				l = v[nm][i][j].l; v[nm][i][j].l = lamb(v[nm][i][j].t);
				xi = v[nm][i][j].xi; v[nm][i][j].xi = ksii(v[nm][i][j].t);
				g = v[nm][i][j].e; gz = v[nm][i][j].ez;
				v[nm][i][j].e = ((v[nm][i][j].zn == 'o') ? 1 : -1) * e1(v[nm][i][j].h,v[nm][i][j].l,v[nm][i][j].x,v[nm][i][j].y,v[nm][i][j].z) + e2(v[nm][i][j].l,v[nm][i][j].xi) + eone4old1(v[nm][i],j);
				v[nm][i][j].ez = eone5old_next(v[nm][i],j); // + eone6old_next(v[nm][i],j);

				e0 = eone3old(v[nm],nn[nm],i,j) + v[nm][i][j].e + v[nm][i][j].ez - e0;
			//	printf("%.16f -- ", e0);
				if (e0 > 0)
				{
					if ((double)rand() / RAND_MAX > (double)exp(-e0 / v[nm][i][j].t))
					{
						e0 = 0;
						v[nm][i][j].x = x;
						v[nm][i][j].y = y;
						v[nm][i][j].t = t;
						v[nm][i][j].h = h;
						v[nm][i][j].l = l;
						v[nm][i][j].xi = xi;
						v[nm][i][j].e = g;
						v[nm][i][j].ez = gz;
					}
				}

			//	printf("%.16f\n", e0);

				break;
			case 3:	// аннигиляция вихрей противоположного знака
			case 4:	// перезацепление вихрей
			case 5:	// 
		}
	}

	s = e0 - ff;
	g = e[nm] + s;
	ff = (g - e[nm]) - s;
	e[nm] = g;
}


// char qqq = 0;
// double ff = 0, fs = 0;
// double tempnm = 0, tempcikl = 0;
// FILE *geex;

// увеличение содержимого после каждого физического шага и условие выхода
char stopr() {
	int i, j;


	nm++;

	nn = realloc(nn, (nm + 1) * sizeof(int));
	nn[nm] = nn[nm - 1];

	v = realloc(v, (nm + 1) * sizeof(struct vortex **));
	v[nm] = malloc(nn[nm] * sizeof(struct vortex *));
	for (i = 0; i < nn[nm]; i++)
	{
		v[nm][i] = malloc(dsn * sizeof(struct vortex));

		for (j = 0; j < dsn; j++)
		{
			v[nm][i][j].x = v[nm - 1][i][j].x;
			v[nm][i][j].y = v[nm - 1][i][j].y;
			v[nm][i][j].z = v[nm - 1][i][j].z;
			v[nm][i][j].zn = v[nm - 1][i][j].zn;
			v[nm][i][j].t = v[nm - 1][i][j].t;
			v[nm][i][j].h = v[nm - 1][i][j].h;
			v[nm][i][j].l = v[nm - 1][i][j].l;
			v[nm][i][j].xi = v[nm - 1][i][j].xi;
			v[nm][i][j].e = v[nm - 1][i][j].e;
			v[nm][i][j].ez = v[nm - 1][i][j].ez;
		}
	}

	e = realloc(e, (nm + 1) * sizeof(double));
 	e[nm] = e[nm - 1];


 	// условия выхода
 	if (nm >= N) return 1;	// выход

 	return 0;


 	// 	// todo: fail: потом заметить (некоторое время надо считать без сохранения)
// #if N > 0
// 		tempnm++;
// 		if (tempnm < 20000) return 0;

// 		tempcikl++;
// 		if (tempcikl < 20) return 0;
// 		else tempcikl = 0;
// #endif
// 		fprintf(geex, "%.8f;", gex(v[nm],nn[nm],L0));

//	-------------

// 		me = realloc(me, (nm + 1) * sizeof(double));
// 		for (me[nm - 1] = i = 0; i < nm; i++)
// 		{
// 			s = e[i] - fs;
// 			g = me[nm - 1] + s;
// 			fs = (g - me[nm - 1]) - s;
// 			me[nm - 1] = g;
// 		}
// 		me[nm - 1] = me[nm - 1] / nm;

// 		// s -- минимальное значение
// 		// g -- максимальное значение
// 		for (s = g = me[nm - 1], i = (int)((1 - G) * nm); i < nm; i++)
// 		{
// 			if (s > me[i]) s = me[i];
// 			if (g < me[i]) g = me[i];
// 		}
// 		pog = m(((g - s) / 2) / e[nm]);
// 		if (!pog) pog = 1;

// #if N > 0
// 		// условия выхода, если задано минимальное колличество физических шагов
// 		if (nm > (double)N / 2 && !tm)
// 		{
// 			tm = 1;
// 			show();
// 			for (i = 0; i < nn[nm]; i++)
// 			{
// 				v[nm][i].t = temp(v[nm][i].x,v[nm][i].y);
// 				v[nm][i].h = pole(v[nm][i].x,v[nm][i].y);
// 				v[nm][i].l = lamb(v[nm][i].t);
// 				v[nm][i].xi = ksii(v[nm][i].t);
// 				v[nm][i].e = ((v[nm][i].zn == 'o') ? 1 : -1) * e1(v[nm][i].h,v[nm][i].l,v[nm][i].x,v[nm][i].y) + e2(v[nm][i].l,v[nm][i].xi) + eone4(v[nm],i);
// 			}
// 			e[nm] = tote1(v[nm],nn[nm]) + tote2(v[nm],nn[nm]) + tote3(v[nm],nn[nm]) + tote4(v[nm],nn[nm]);
// 		}

// 		if (nm > N) qqq = 1; // заканчиваем работу
// #else
// 		// условия выхода, если задана минимальная погрешность
// //		if (pog <= P && !q)
// //		q = 2 * nm;
// //		if (q && q < nm)
// //			if (pog > P)
// //				q = 0;
// //			else
// //				return 1;

// //		if (nm > 60)
// //			qqq = 1;

// 		if (pog <= P && !qq && !tm)
// 			qq = 2 * nm;
// 		if (qq && qq < nm && !tm)
// 			if (pog > P)
// 				qq = 0;
// 			else {tm = 1; qq = 0; show();}
// 		if (!qq && tm)
// 		{
// 			qq = 2 * nm;
// 			for (i = 0; i < nn[nm]; i++)
// 			{
// 				v[nm][i].t = temp(v[nm][i].x,v[nm][i].y);
// 				v[nm][i].h = pole(v[nm][i].x,v[nm][i].y);
// 				v[nm][i].l = lamb(v[nm][i].t);
// 				v[nm][i].xi = ksii(v[nm][i].t);
// 				v[nm][i].e = ((v[nm][i].zn == 'o') ? 1 : -1) * e1(v[nm][i].h,v[nm][i].l,v[nm][i].x,v[nm][i].y) + e2(v[nm][i].l,v[nm][i].xi) + eone4(v[nm],i);
// 			}
// 			e[nm] = tote1(v[nm],nn[nm]) + tote2(v[nm],nn[nm]) + tote3(v[nm],nn[nm]) + tote4(v[nm],nn[nm]);
// 		}
// 		if (qq && qq < nm && tm)
// 			qqq = 1; // заканчиваем работу

// 		if (nm > 10000)
// 			qqq = 1; // слишком много
// 		// todo: иногда погрешность может быть больше 100
// //		if (isnan(pog) || pog > 100)
// //			qqq = 1; // заканчиваем работу
// #endif
// 	}

// 	return 0;
// }
}


/* ветка основного алгоритма */
//void *tree() {
	// int i;
	// double g, gz, x, y, t, h, l, xi;



	// 	switch (i)
	// 	{
// 			case 0: // удаление
// 				i = rand() % nn[nm];

// #if XG & 2
// 	if ((v[nm][i].x <= X - L0) && (v[nm][i].x >= L0)) break;
// #endif
// #if XG & 1
// 	if ((v[nm][i].y <= Y - L0) && (v[nm][i].y >= L0)) break;
// #endif

// 				swapvi(v[nm],i,nn[nm] - 1);

// 				e0 = -(eone3(v[nm],nn[nm]) + v[nm][nn[nm] - 1].e);

// 				g = nn[nm] / TA * (double)nn[nm] * exp(-e0 / v[nm][nn[nm] - 1].t);
// 				if (g < 1)
// 				{
// 					if ((double)rand() / RAND_MAX <= g)
// 					{
// 						v[nm] = realloc(v[nm], nn[nm] * sizeof(struct vortex));
// 						nn[nm]--;
// 					}
// 					else e0 = 0;
// 				}
// 				else
// 				{
// 					v[nm] = realloc(v[nm], nn[nm] * sizeof(struct vortex));
// 					nn[nm]--;
// 				}
// 				break;
// 			case 2: // анигиляция
// 				int j, xx, yy;

// 				for (i = xx = yy = 0; i < nn[nm]; i++)
// 					if (v[nm][i].zn == 'o') yy++;
// 					else xx++;

// 				if (xx && yy)
// 				{
// 					i = rand() % xx;
// 					j = rand() % yy;
// 					for (int tt = yy = xx = 0; tt < nn[nm]; tt++)
// 						if (v[nm][tt].zn == 'o')
// 						{
// 							if (yy == j) j = tt;
// 							yy++;
// 						}
// 						else
// 						{
// 							if (xx == i) i = tt;
// 							xx++;
// 						}

// 					if ("возможна аннигиляция одного вихря", 421) // erro 421

// 					if (rrr(v[nm][i].x,v[nm][i].y,v[nm][j].x,v[nm][j].y) <= DS)
// 					{
// 						e0 = -(v[nm][i].e + v[nm][j].e);

// 						swapvi(v[nm],i,nn[nm] - 1);
// 						e0 = e0 - eone3(v[nm],nn[nm]);

// 						v[nm] = realloc(v[nm], nn[nm] * sizeof(struct vortex));
// 						nn[nm]--;

// 						swapvi(v[nm],(j == nn[nm]) ? i : j,nn[nm] - 1);
// 						e0 = e0 - eone3(v[nm],nn[nm]);

// 						v[nm] = realloc(v[nm], nn[nm] * sizeof(struct vortex));
// 						nn[nm]--;
// 					}
// 				}
// 		}
// }