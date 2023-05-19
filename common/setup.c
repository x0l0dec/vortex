// программа показывает статус
void show() {
	system("clear");
	printf("шагов: %llu / %llu\nвихрей: %d\ntotal: %.16f\n+++++: %.16f\nH: %.1d\n", nn, k, nm, totee1() + totei1() + tote2() + tote3() + tote4(), e, HE);
	fflush(stdout);
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
	char c, x = 0, p = 0, zn = 'o';
	int i = 0, j = 0;
	double tx = 0, ty = 0;
	FILE *inp;


	inp = fopen(ss, "r");

	while ((c = fgetc(inp)) != EOF)
	{
		if (c == '\n')
		{
			j = (int)(ty / ry);
			//if (i > ny) если вылезло за разбиение ( if (ty > Y) ) ???

			may[i][j].v[may[i][j].nv].zn = zn;
			may[i][j].v[may[i][j].nv].x = tx;
			may[i][j].v[may[i][j].nv].y = ty;

			may[i][j].v[may[i][j].nv].t = temp(tx,ty);
			may[i][j].v[may[i][j].nv].h = pole(tx,ty);
			may[i][j].v[may[i][j].nv].hi = poli(tx,ty);
			may[i][j].v[may[i][j].nv].l = lamb(may[i][j].v[may[i][j].nv].t);
			may[i][j].v[may[i][j].nv].xi = ksii(may[i][j].v[may[i][j].nv].t);
			may[i][j].v[may[i][j].nv].e = ((may[i][j].v[may[i][j].nv].zn == 'o') ? 1 : -1) * (e1e(may[i][j].v[may[i][j].nv].h,may[i][j].v[may[i][j].nv].l,tx,ty) + e1i(may[i][j].v[may[i][j].nv].hi,may[i][j].v[may[i][j].nv].l,tx,ty)) + e2(may[i][j].v[may[i][j].nv].l,may[i][j].v[may[i][j].nv].xi) + eone4(i,j,may[i][j].nv);

			if (zn == 'o') nmo++;
			else nmx++;
			nm++;
			may[i][j].nv++;
			may[i][j].v = realloc(may[i][j].v, (may[i][j].nv + 1) * sizeof(struct vortex));

			x = 0;
			p = 0;
			tx = ty = 0;

			continue;
		}
		if (c == ',')
		{
			i = (int)(tx / rx);
			//if (i > nx) если вылезло за разбиение ( if (tx > X) ) ???

			x++;
			p = 0;

			continue;
		}
		if (c == '.')
		{
			if (!p) {p = 1; continue;}
			else continue; // todo: во вводе много точек
		}
		if (c == 'o' || c == 'x')
		{
			zn = c;
			continue;
		}
		if ('0' <= c && c <= '9')
		{
			if (x == 0)
			{
				if (p)
				{
					tx = tx + (c - '0') * pow(10,-p);
					p++;
				}
				else
					tx = 10 * tx - '0';
			}
			if (x == 1)
			{
				if (p)
				{
					ty = ty + (c - '0') * pow(10,-p);
					p++;
				}
				else
					ty = 10 * ty - '0';
			}
		}
	}
	fclose(inp);
}

// читает примеси из внешнего файла
void takep(char *ss) {
	char c, x = 0, p = 0;
	int i = 0, j = 0;
	double tx = 0, ty = 0;
	FILE *inp;


	inp = fopen(ss, "r");

	while ((c = fgetc(inp)) != EOF)
	{
		if (c == '\n')
		{
			may[i][j].p[may[i][j].np].x = tx;
			may[i][j].p[may[i][j].np].y = ty;

			may[i][j].np++;
			may[i][j].p = realloc(may[i][j].p, (may[i][j].np + 1) * sizeof(struct prim));

			x = 0;
			p = 0;
			tx = ty = 0;

			continue;
		}
		if (c == ',')
		{
			if (x == 0) i = (int)(tx / rx);	//if (i > nx) если вылезло за разбиение ( if (tx > X) ) ???
			if (x == 1) j = (int)(ty / ry);	//if (i > ny) если вылезло за разбиение ( if (ty > Y) ) ???

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
			if (x == 0)
			{
				if (p)
				{
					tx = tx + (c - '0') * pow(10,-p);
					p++;
				}
				else
					tx = 10 * tx - '0';
			}
			if (x == 1)
			{
				if (p)
				{
					ty = ty + (c - '0') * pow(10,-p);
					p++;
				}
				else
					ty = 10 * ty - '0';
			}
			if (x == 2)
			{
				if (p)
				{
					may[i][j].p[may[i][j].np].u = may[i][j].p[may[i][j].np].u + (c - '0') * pow(10,-p);
					p++;
				}
				else
					may[i][j].p[may[i][j].np].u = 10 * may[i][j].p[may[i][j].np].u - '0';
			}
		}
	}
	fclose(inp);
}




// функция, запускающаяся один раз в начале работы программы
void setup(int argc, char *argv[]) {
	char ssd[512], ssp[512];
	int i, j;


	// генератор случайного
	srand(clock());

	// главная структура, в которой всё содержится
	may = malloc(nx * sizeof(struct gr4 *));
	for (i = 0; i < nx; i++)
	{
		may[i] = malloc(ny * sizeof(struct gr4));
		for (j = 0; j < ny; j++)
		{
			may[i][j].v = malloc(sizeof(struct vortex));
			may[i][j].nv = 0;
			may[i][j].p = malloc(sizeof(struct prim));
			may[i][j].np = 0;
		}
	}


	ssd[0] = 0; ssp[0] = 0;
	inflag(argc,argv,ssd,ssp);

	if (ssp[0])	// есть файл с входными примесями
		takep(ssp);
	else		// нет фала с входными примесями
	{
		// распределение примесей
		double w = 1.32471795724474602596, a1, a2, tx, ty;

		a1 = 1.0 / w;
		a2 = 1.0 / (w * w);

		for (int p = 0; p < NP; p++)
		{
			tx = ((double)rand() - (double)rand()) / RAND_MAX * RP;
			ty = ((double)rand() - (double)rand()) / RAND_MAX * RP;
			w = ((0.5 + a1 * i) - (int)(0.5 + a1 * i));
			tx = ((tx + w > 1) ? (tx + w - 1) : ((tx + w < 0) ? (tx + w + 1) : (tx + w))) * X;
			w = ((0.5 + a2 * i) - (int)(0.5 + a2 * i));
			ty = ((ty + w > 1) ? (ty + w - 1) : ((ty + w < 0) ? (ty + w + 1) : (ty + w))) * Y;

			i = (int)(tx / rx);
			j = (int)(ty / ry);

			may[i][j].p[may[i][j].np].x = tx;
			may[i][j].p[may[i][j].np].y = ty;
			may[i][j].p[may[i][j].np].u = upp(tx,ty);

			may[i][j].np++;
			may[i][j].p = realloc(may[i][j].p, (may[i][j].np + 1) * sizeof(struct prim));
		}
	}

	if (ssd[0])	// есть файл с распределением вихрей
		takev(ssd);
	e = totee1() + totei1() + tote2() + tote3() + tote4();
}