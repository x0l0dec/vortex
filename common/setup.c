// распределяет в системе столбчатые примеси
void *defectcol() {
	int i, j;
	double w = 1.32471795724474602596, a1, a2, x, y, u;

	a1 = 1.0 / w;
	a2 = 1.0 / (w * w);

	for (i = SNP; i < SNP + CNP; i++)
	{
		x = ((double)rand() - (double)rand()) / RAND_MAX * RP;
		y = ((double)rand() - (double)rand()) / RAND_MAX * RP;
		w = ((0.5 + a1 * i) - (int)(0.5 + a1 * i));
		x = ((x + w > 1) ? (x + w - 1) : ((x + w < 0) ? (x + w + 1) : (x + w))) * X;
		w = ((0.5 + a2 * i) - (int)(0.5 + a2 * i));
		y = ((y + w > 1) ? (y + w - 1) : ((y + w < 0) ? (y + w + 1) : (y + w))) * Y;
//		u = exp(-pow(((double)rand() / RAND_MAX - PMAT) / PDIS,2) / 2) / (PDIS * pow(2 * M_PI, 0.5));
		u = (double)rand() / RAND_MAX * m(U01 - U02) + U02;
		for (j = 0; j < dsn; j++)
		{
			pr[j][i].x = x;
			pr[j][i].y = y;
			pr[j][i].z = j * SD;
			pr[j][i].u = u;
		}
	}

	pthread_exit(NULL);
}

//todo: сделать разную плотность случайных примесей между слоями
// распределяет в системе случайные примеси
void *defect(void *zn) {
	int i, *j = (int *)zn;
	double w = 1.32471795724474602596, a1, a2;

	a1 = 1.0 / w;
	a2 = 1.0 / (w * w);

	for (i = 0; i < SNP; i++)
	{
		pr[*j][i].x = ((double)rand() - (double)rand()) / RAND_MAX * RP;
		pr[*j][i].y = ((double)rand() - (double)rand()) / RAND_MAX * RP;
		w = ((0.5 + a1 * i) - (int)(0.5 + a1 * i));
		pr[*j][i].x = ((pr[*j][i].x + w > 1) ? (pr[*j][i].x + w - 1) : ((pr[*j][i].x + w < 0) ? (pr[*j][i].x + w + 1) : (pr[*j][i].x + w))) * X;
		w = ((0.5 + a2 * i) - (int)(0.5 + a2 * i));
		pr[*j][i].y = ((pr[*j][i].y + w > 1) ? (pr[*j][i].y + w - 1) : ((pr[*j][i].y + w < 0) ? (pr[*j][i].y + w + 1) : (pr[*j][i].y + w))) * Y;
		pr[*j][i].z = *j * SD;
//		pr[*j][i].u = exp(-pow(((double)rand() / RAND_MAX - PMAT) / PDIS,2) / 2) / (PDIS * pow(2 * M_PI, 0.5));
		pr[*j][i].u = (double)rand() / RAND_MAX * m(U01 - U02) + U02;
	}

	pthread_exit(NULL);
}


// читает флаги из командной строки
void inflag(int argc, char *argv[], char *ssd, char *ssp) {
	char cd = 1, cp = 1, ch = 1;
	int i, j;

	for (i = 1; i < argc; i++)
	{
		// todo: help
		if (ch && ((argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == 'e' && argv[i][3] == 'l' && argv[i][4] == 'p' && argv[i][5] == '\0') || (argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] == 'h' && argv[i][3] == 'e' && argv[i][4] == 'l' && argv[i][5] == 'p' && argv[i][6] == '\0') || (argv[i][0] == 'h' && argv[i][1] == 'e' && argv[i][2] == 'l' && argv[i][3] == 'p' && argv[i][4] == '\0'))) // help
		{
			ch = 0;
			printf("используются следующие ключи:\
				\n  -d    -- для задания начальной конфигурации расположения вихрей\
				\n  -p    -- для задания начальной конфигурации расположения примесей\
				\n  -help -- для выхова этой справки\
				\n");

			// todo: запрошен help
		}
		if (cd && (argv[i][0] == '-' && argv[i][1] == 'd')) // вихри
		{
			i++; j = 0; cd = 0;
			while (argv[i][j] != '\0')
			{
				ssd[j] = argv[i][j];
				j++;
			}
			ssd[j] = '\0';
		}
		if (cp && (argv[i][0] == '-' && argv[i][1] == 'p')) // примеси
		{
			i++; j = 0; cp = 0;
			while (argv[i][j] != '\0')
			{
				ssp[j] = argv[i][j];
				j++;
			}
			ssp[j] = '\0';
		}
	}
}

// читает вихри из внешнего файла
void takev(char *ss) {
	char c, x = 0, p = 0;
	int i = 0, j;
	FILE *inp;


	inp = fopen(ss, "r");

	nm = -1;
	while ((c = fgetc(inp)) != EOF)
	{
		if (c == '=')
		{
			c = fgetc(inp);

			nm++;
			v = realloc(v, (nm + 1) * sizeof(struct vortex **));
			v[nm] = malloc(sizeof(struct vortex *));
			v[nm][nn[nm]] = malloc(sizeof(struct vortex));

			nn = realloc(nn, (nm + 1) * sizeof(int));
			nn[nm] = 0;

			continue;
		}
		if (c == '\n')
		{
			nn[nm]++;
			v[nm] = realloc(v[nm], (nn[nm] + 1) * sizeof(struct vortex *));
			v[nm][nn[nm]] = malloc(sizeof(struct vortex));
			i = 0;

			continue;
		}
		if (c == ';')
		{
			v[nm][nn[nm]][i].t = temp(v[nm][nn[nm]][i].x,v[nm][nn[nm]][i].y,v[nm][nn[nm]][i].z);
			v[nm][nn[nm]][i].h = pole(v[nm][nn[nm]][i].x,v[nm][nn[nm]][i].y,v[nm][nn[nm]][i].z);
			v[nm][nn[nm]][i].l = lamb(v[nm][nn[nm]][i].t);
			v[nm][nn[nm]][i].xi = ksii(v[nm][nn[nm]][i].t);
			v[nm][nn[nm]][i].e = ((v[nm][nn[nm]][i].zn == 'o') ? 1 : -1) * e1(v[nm][nn[nm]][i].h,v[nm][nn[nm]][i].l,v[nm][nn[nm]][i].x,v[nm][nn[nm]][i].y,v[nm][nn[nm]][i].z) + e2(v[nm][nn[nm]][i].l,v[nm][nn[nm]][i].xi) + eone4old1(v[nm][nn[nm]],i);
			v[nm][nn[nm]][i].ez = eone5old_next(v[nm][nn[nm]],i); // + eone6old_next(v[nm][nn[nm]],i);

			x = 0;
			i++;

			v[nm][nn[nm]] = realloc(v[nm][nn[nm]], (i + 1) * sizeof(struct vortex));

			p = 0;

			continue;
		}
		if (c == ',')
		{
			x++;
			p = 0;

			continue;
		}
		if (c == '.')
		{
			if (!p) {p = 1; continue;}
			else continue; // todo: во вводе много точек
		}
		if ('0' <= c && c <= '9')
		{
			if (x == 1)
			{
				if (p)
				{
					v[nm][nn[nm]][i].x = v[nm][nn[nm]][i].x + (c - '0') * pow(10,-p);
					p++;
				}
				else
					v[nm][nn[nm]][i].x = 10 * v[nm][nn[nm]][i].x + c - '0';
			}
			if (x == 2)
			{
				if (p)
				{
					v[nm][nn[nm]][i].y = v[nm][nn[nm]][i].y + (c - '0') * pow(10,-p);
					p++;
				}
				else
					v[nm][nn[nm]][i].y = 10 * v[nm][nn[nm]][i].y + c - '0';
			}
			if (x == 3)
			{
				if (p)
				{
					v[nm][nn[nm]][i].z = v[nm][nn[nm]][i].z + (c - '0') * pow(10,-p);
					p++;
				}
				else
					v[nm][nn[nm]][i].z = 10 * v[nm][nn[nm]][i].z + c - '0';
			}

			continue;
		}
		if (c == 'o' || c == 'x')
			v[nm][nn[nm]][i].zn = c;
	}
	fclose(inp);


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
}

// читает примеси из внешнего файла
void takep(char *ss) {
	char c;
	FILE *inp;


	inp = fopen(ss, "r");

	pr = malloc(dsn * sizeof(struct prim *));
	for (i = 0; i < dsn; i++)
		pr[i] = malloc((SNP + CNP) * sizeof(struct prim));
	while ((c = fgetc(inp)) != EOF)
	{
	}
}



// функция, запускающаяся один раз в начале работы программы
void setup(int argc, char *argv[]) {
	char ssd[512], ssp[512];
	int i, *j;
	pthread_t *th;


	// генератор случайного
	srand(clock());

	// тут будут наши вихри (v[время][номер линии][номер вихря])
	v = malloc(sizeof(struct vortex **));
	v[0] = malloc(sizeof(struct vortex *));
	v[0][0] = malloc(sizeof(struct vortex));
	// количество линий на каждом шаге
	nn = malloc(sizeof(int));


	ssd[0] = 0; ssp[0] = 0;
	inflag(argc,argv,ssd,ssp);

	if (ssp[0])	// есть файл с входными примесями
		takep(ssp);
	else	// нет файла с входными примесями
	{
		// распределение примесей выполняется паралельно
		th = malloc((dsn + 1) * sizeof(pthread_t));
		j = malloc((dsn + 1) * sizeof(int));

		for (i = 0; i < dsn; i++)
			j[i] = i;

		// случайные примеси
		pr = malloc(dsn * sizeof(struct prim *));
		for (i = 0; i < dsn; i++)
			pr[i] = malloc((SNP + CNP) * sizeof(struct prim));
		pthread_create(&th[0], NULL, defectcol, NULL);
		for (i = 0; i < dsn; i++)
			pthread_create(&th[i + 1], NULL, defect, (void *)&j[i]);
		pthread_join(th[0], NULL);
		for (i = 1; i < dsn + 1; i++)
			pthread_join(th[i], NULL);
	}

	if (ssd[0]) // есть файл с входными вихрями
		takev(ssd);
	else	// нет файла с входными вихрями
	{
		nm = 0;
		nn[0] = 0;
	}
}