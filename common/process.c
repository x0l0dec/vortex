// условие остановки
char forend() {
	char ss[512], sg[512];
	double s, g;

	s = e0 - ff;
	g = e + s;
	ff = (g - e) - s;
	e = g;

	k++;
#if D < 0
	if (k > d + dk + DZP)
	{
		dk = k;
		d = (nm < DM) ? DM : nm;
#else
	if (k > D + dk + DZP)
	{
#endif
		nn++;
		if (SPC < nn) return 0;

		snprintf(ss, 512, PPC);
		savev(ss);

		// условие выхода
		if (nn > N)
		{
			snprintf(sg, 512, PPG);
			saveg(sg);

			return 1;
		}
	}
}

// подпроцессы основного алгоритма
void process() {
	int i, j, q;
	double x, y, g, ei = 0, ed;

	e0 = 0;
	i = rand() % RND;
	switch (i)
	{
	case 0:		// удаление
		if (nm == 0) break;
		i = rand() % nx;
		j = rand() % ny;
		q = rand() % may[i][j].nv;

#if XG == 1
		if ((may[i][j].v[q].x <= X - L0) && (may[i][j].v[q].x >= L0)) break;
#endif
#if YG == 1
		if ((may[i][j].v[q].y <= Y - L0) && (may[i][j].v[q].y >= L0)) break;
#endif

		e0 = -(eone3(may[i][j].v[q].x,may[i][j].v[q].y,q) + may[i][j].v[q].e);

		g = nm / TA * (double)nm * exp(-e0 / may[i][j].v[q].t);
		if (g < 1)
		{
			if ((double)rand() / RAND_MAX <= g)
			{
				if (may[i][j].v[q].zn == 'o') nmo--;
				else nmx--;
				swapvv(i,j,q,may[i][j].nv - 1);
				may[i][j].v = realloc(may[i][j].v, may[i][j].nv * sizeof(struct vortex));
				may[i][j].nv--; nm--;
			}
			else e0 = 0;
		}
		else
		{
			if (may[i][j].v[q].zn == 'o') nmo--;
			else nmx--;
			swapvv(i,j,q,may[i][j].nv - 1);
			may[i][j].v = realloc(may[i][j].v, may[i][j].nv * sizeof(struct vortex));
			may[i][j].nv--; nm--;
		}
		break;
	case 1: 	// создание
#if XG == 1
		x = (double)rand() / RAND_MAX * (BRN - DRN) + (rand() % 2 ? DRN : X - L0);
#else
		x = (double)rand() / RAND_MAX * X;
#endif
#if YG == 1
		y = (double)rand() / RAND_MAX * (BRN - DRN) + (rand() % 2 ? DRN : Y - L0);
#else
		y = (double)rand() / RAND_MAX * Y;
#endif

		if (check(x,y)) break;

		i = (int)(x / rx);
		j = (int)(y / ry);

		may[i][j].v = realloc(may[i][j].v, (may[i][j].nv + 1) * sizeof(struct vortex));

		may[i][j].v[may[i][j].nv].x = x;
		may[i][j].v[may[i][j].nv].y = y;
		may[i][j].v[may[i][j].nv].zn = rand() % 2 ? 'o' : 'x';
		may[i][j].v[may[i][j].nv].t = temp(x,y);
		may[i][j].v[may[i][j].nv].h = pole(x,y);
		may[i][j].v[may[i][j].nv].hi = poli(x,y);
		may[i][j].v[may[i][j].nv].l = lamb(may[i][j].v[may[i][j].nv].t);
		may[i][j].v[may[i][j].nv].xi = ksii(may[i][j].v[may[i][j].nv].t);
		may[i][j].v[may[i][j].nv].e = e1e(may[i][j].v[may[i][j].nv].h,may[i][j].v[may[i][j].nv].l,x,y) + e1i(may[i][j].v[may[i][j].nv].hi,may[i][j].v[may[i][j].nv].l,x,y) + e2(may[i][j].v[may[i][j].nv].l,may[i][j].v[may[i][j].nv].xi) + eone4(i,j,may[i][j].nv);

		e0 = eone3(may[i][j].v[may[i][j].nv].x,may[i][j].v[may[i][j].nv].y,may[i][j].nv) + may[i][j].v[may[i][j].nv].e;

		if (may[i][j].v[may[i][j].nv].zn == 'o') nmo++;
		else nmx++;
		nm++;
		may[i][j].nv++;

		g = (double)TA / (may[i][j].nv + 1) * exp(-e0 / may[i][j].v[may[i][j].nv - 1].t);
		if (g < 1)
		{
			if ((double)rand() / RAND_MAX > g)
			{
				if (may[i][j].v[may[i][j].nv].zn == 'o') nmo--;
				else nmx--;
				may[i][j].v = realloc(may[i][j].v, may[i][j].nv * sizeof(struct vortex));
				may[i][j].nv--; nm--;
				e0 = 0;
			}
		}
		break;
	case 2: 	// анигиляция
		if (!nmx || !nmo) break;

		int jo = 0, io = rand() / nmo, iio = -1, jjo = -1, nno = -1;
		int jx = 0, ix = rand() / nmx, iix = -1, jjx = -1, nnx = -1;

		for (i = 0; i < nx; j++)
			for (j = 0; j < ny; j++)
				for (q = 0; q < may[i][j].nv; q++)
				{
					if (may[i][j].v[q].zn == 'o') jo++;
					if (may[i][j].v[q].zn == 'x') jx++;

					if (jo == io) {iio = i; jjo = j; nno = q;}
					if (jx == ix) {iix = i; jjx = j; nnx = q;}

					if (nno >= 0 && nnx >= 0)
					{
						double r = rrr(may[iio][jjo].v[nno].x,may[iio][jjo].v[nno].y, may[iix][jjx].v[nnx].x,may[iix][jjx].v[nnx].y);
						if (r <= DS)
						{
							e0 = -(may[iio][jjo].v[nno].e + may[iix][jjx].v[nnx].e + eone3(iio,jjo,nno) + eone3(iix,jjx,nnx) - e3(r,may[iio][jjo].v[nno].l) - e3(r,may[iix][jjx].v[nnx].l)); // проблема с энергиями

							swapvv(iio,jjo,nno,may[iio][jjo].nv - 1);
							swapvv(iix,jjx,nnx,may[iix][jjx].nv - 1);

							may[iio][jjo].v = realloc(may[iio][jjo].v, sizeof(struct vortex));
							may[iio][jjo].nv--;
							may[iix][jjx].v = realloc(may[iix][jjo].v, sizeof(struct vortex));
							may[iix][jjx].nv--;
							nmo--; nmx--; nm--;
						}
						else
							goto BB;
					}
				}
		break;
	default:	// перемещение
		if (nm == 0) break;
		i = rand() % nx;
		j = rand() % ny;
		q = rand() % may[i][j].nv;

		do {
			x = (double)rand() / RAND_MAX * 2 * DP * may[i][j].v[q].l - DP * may[i][j].v[q].l;
			y = (double)rand() / RAND_MAX * 2 * DP * may[i][j].v[q].l - DP * may[i][j].v[q].l;
		} while (x * x + y * y > DP * may[i][j].v[q].l);

#if XG == 1
		if (may[i][j].v[q].x + x < 0)
			break;
		else
			if (may[i][j].v[q].x + x >= X)
				break;
			else
				x = may[i][j].v[q].x + x;
#else
		x = (may[i][j].v[q].x + x < 0) ? (X + may[i][j].v[q].x + x) : ((may[i][j].v[q].x + x >= X) ? (may[i][j].v[q].x + x - X) : (may[i][j].v[q].x + x));
#endif
#if YG == 1
		if (may[i][j].v[q].y + y < 0)
			break;
		else
			if (may[i][j].v[q].y + y >= Y)
				break;
			else
				y = may[i][j].v[q].y + y;
#else
		y = (may[i][j].v[q].y + y < 0) ? (Y + may[i][j].v[q].y + y) : ((may[i][j].v[q].y + y >= Y) ? (may[i][j].v[q].y + y - Y) : (may[i][j].v[q].y + y));
#endif
		if (check(may[i][j].v[q].x,may[i][j].v[q].y)) break;

#if XG == 1
		if (may[i][j].v[q].x < X / 2)
			ei = e1i(may[i][j].v[q].hi,lamb(temp(X / 2 - 0.000001,may[i][j].v[q].y)),X / 2 - 0.000001,may[i][j].v[q].y);
		else
			ei = e1i(may[i][j].v[q].hi,lamb(temp(X / 2 + 0.000001,may[i][j].v[q].y)),X / 2 + 0.000001,may[i][j].v[q].y);
#endif
#if YG == 1
		if (may[i][j].v[q].y < Y / 2)
			ei = e1i(may[i][j].v[q].hi,lamb(temp(may[i][j].v[q].x,Y / 2 - 0.000001)),may[i][j].v[q].x,Y / 2 - 0.000001);
		else
			ei = e1i(may[i][j].v[q].hi,lamb(temp(may[i][j].v[q].x,Y / 2 + 0.000001)),may[i][j].v[q].x,Y / 2 + 0.000001);
#endif
		ed = eone3(may[i][j].v[q].x,may[i][j].v[q].y,q) + may[i][j].v[q].e - ei;
		e0 = eone3(may[i][j].v[q].x,may[i][j].v[q].y,q) + may[i][j].v[q].e;

		double t, h, hi, l, xi;
		t = may[i][j].v[q].t; may[i][j].v[q].t = temp(x,y);
		h = may[i][j].v[q].h; may[i][j].v[q].h = pole(x,y);
		hi = may[i][j].v[q].hi; may[i][j].v[q].hi = poli(x,y);
		g = may[i][j].v[q].x; may[i][j].v[q].x = x; x = g;
		g = may[i][j].v[q].y; may[i][j].v[q].y = y; y = g;
		l = may[i][j].v[q].l; may[i][j].v[q].l = lamb(may[i][j].v[q].t);
		xi = may[i][j].v[q].xi; may[i][j].v[q].xi = ksii(may[i][j].v[q].t);
		g = may[i][j].v[q].e; may[i][j].v[q].e = ((may[i][j].v[q].zn == 'o') ? 1 : -1) * (e1e(may[i][j].v[q].h,may[i][j].v[may[i][j].nv].l,x,y) + e1i(may[i][j].v[q].hi,may[i][j].v[q].l,x,y)) + e2(may[i][j].v[q].l,may[i][j].v[q].xi) + eone4(i,j,may[i][j].nv);

#if XG == 1
		if (may[i][j].v[q].x < X / 2)
			ei = e1i(may[i][j].v[q].hi,lamb(temp(X / 2 - 0.000001,may[i][j].v[q].y)),X / 2 - 0.000001,may[i][j].v[q].y);
		else
			ei = e1i(may[i][j].v[q].hi,lamb(temp(X / 2 + 0.000001,may[i][j].v[q].y)),X / 2 + 0.000001,may[i][j].v[q].y);
#endif
#if YG == 1
		if (may[i][j].v[q].y < Y / 2)
			ei = e1i(may[i][j].v[q].hi,lamb(temp(may[i][j].v[q].x,Y / 2 - 0.000001)),may[i][j].v[q].x,Y / 2 - 0.000001);
		else
			ei = e1i(may[i][j].v[q].hi,lamb(temp(may[i][j].v[q].x,Y / 2 + 0.000001)),may[i][j].v[q].x,Y / 2 + 0.000001);
#endif

		ed = eone3(may[i][j].v[q].x,may[i][j].v[q].y,q) + may[i][j].v[q].e - ed - ei;
		e0 = eone3(may[i][j].v[q].x,may[i][j].v[q].y,q) + may[i][j].v[q].e - e0;
		if (ed > 0)
		{
			if ((double)rand() / RAND_MAX > (double)exp(-ed / may[i][j].v[q].t))
			{
				e0 = 0;
				may[i][j].v[q].x = x;
				may[i][j].v[q].y = y;
				may[i][j].v[q].t = t;
				may[i][j].v[q].h = h;
				may[i][j].v[q].hi = hi;
				may[i][j].v[q].l = l;
				may[i][j].v[q].xi = xi;
				may[i][j].v[q].e = g;
			}
		}
	}
BB:
}