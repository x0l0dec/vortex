// считает энергию от внешнего магнитного поля для вихря в рамках одного гекса
void *eonee1(void *a) {
	double f = 0, s, t;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

	for (int i = 0; i < may[(*vv).x][(*vv).y].nv; i++)
	{
		s = ((may[(*vv).x][(*vv).y].v[i].zn == 'o') ? 1 : -1) * e1e(may[(*vv).x][(*vv).y].v[i].h, may[(*vv).x][(*vv).y].v[i].l, may[(*vv).x][(*vv).y].v[i].x, may[(*vv).x][(*vv).y].v[i].y) - f;
		t = (*vv).v + s;
		f = (t - (*vv).v) - s;
		(*vv).v = t;
	}

	pthread_exit(NULL);
}

// считает энергию от внешнего магнитного поля для всей системы
double totee1() {
	double v = 0, f = 0, s, t;
	int i, j;
	pthread_t *th;
	th = malloc(nx * ny * sizeof(pthread_t));
	struct vn12 *vv;
	vv = malloc(nx * ny * sizeof(struct vn12));

	for (j = i = 0; i < nx * ny; i++)
	{
		vv[i].v = 0;
		vv[i].x = i / ny;
		vv[i].y = i % ny;
		pthread_create(&th[i], NULL, eonee1, (void *)&vv[i]);

		if (i - j >= CPOT)
		{
			pthread_join(th[j], NULL);
			s = vv[j].v - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
			j++;
		}
	}
	while (j < i)
	{
		pthread_join(th[j], NULL);
		s = vv[j].v - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
		j++;
	}

	// на всякий случай
	free(vv); free(th);

	return v;
}


// считает энергию магнитного поля от тока для вихря в рамках одного гекса
void *eonei1(void *a) {
	double f = 0, s, t;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

	for (int i = 0; i < may[(*vv).x][(*vv).y].nv; i++)
	{
		s = e1i(may[(*vv).x][(*vv).y].v[i].h, may[(*vv).x][(*vv).y].v[i].l, may[(*vv).x][(*vv).y].v[i].x, may[(*vv).x][(*vv).y].v[i].y) - f;
		t = (*vv).v + s;
		f = (t - (*vv).v) - s;
		(*vv).v = t;
	}

	pthread_exit(NULL);
}

// считает энергию магнитного поля от тока для всей системы
double totei1() {
	double v = 0, f = 0, s, t;
	int i, j;
	pthread_t *th;
	th = malloc(nx * ny * sizeof(pthread_t));
	struct vn12 *vv;
	vv = malloc(nx * ny * sizeof(struct vn12));

	for (j = i = 0; i < nx * ny; i++)
	{
		vv[i].v = 0;
		vv[i].x = i / ny;
		vv[i].y = i % ny;
		pthread_create(&th[i], NULL, eonei1, (void *)&vv[i]);

		if (i - j >= CPOT)
		{
			pthread_join(th[j], NULL);
			s = vv[j].v - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
			j++;
		}
	}
	while (j < i)
	{
		pthread_join(th[j], NULL);
		s = vv[j].v - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
		j++;
	}

	// на всякий случай
	free(vv); free(th);

	return v;
}




// считает внутреннюю энергию вихрей в рамках одного гекса
void *eone2(void *a) {
	double f = 0, s, t;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

	for (int i = 0; i < may[(*vv).x][(*vv).y].nv; i++)
	{
		s = e2(may[(*vv).x][(*vv).y].v[i].l, may[(*vv).x][(*vv).y].v[i].xi) - f;
		t = (*vv).v + s;
		f = (t - (*vv).v) - s;
		(*vv).v = t;
	}

	pthread_exit(NULL);
}

// считает внутреннюю энергию вихрей для всей системы
double tote2() {
	double v = 0, f = 0, s, t;
	int i, j;
	pthread_t *th;
	th = malloc(nx * ny * sizeof(pthread_t));
	struct vn12 *vv;
	vv = malloc(nx * ny * sizeof(struct vn12));

	for (j = i = 0; i < nx * ny; i++)
	{
		vv[i].v = 0;
		vv[i].x = i / ny;
		vv[i].y = i % ny;
		pthread_create(&th[i], NULL, eone2, (void *)&vv[i]);

		if (i - j >= CPOT)
		{
			pthread_join(th[j], NULL);
			s = vv[j].v - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
			j++;
		}
	}
	while (j < i)
	{
		pthread_join(th[j], NULL);
		s = vv[j].v - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
		j++;
	}

	// на всякий случай
	free(vv); free(th);

	return v;
}




// считает энергию взаимодействия заданного вихря с другими вихрями в заданном гексе
void *eone33(void *a) {
	double f = 0, s, t, r;
	struct vn3e *vv;
	vv = (struct vn3e *)a;

	for (int i = 0; i < may[(*vv).x][(*vv).y].nv; i++)
	{
		if ((*vv).x == (*vv).nx && (*vv).y == (*vv).ny && i == (*vv).n) continue;

		r = rrr(may[(*vv).x][(*vv).y].v[i].x,may[(*vv).x][(*vv).y].v[i].y, may[(*vv).nx][(*vv).ny].v[(*vv).n].x,may[(*vv).nx][(*vv).ny].v[(*vv).n].y);
		if (may[(*vv).x][(*vv).y].v[i].zn == may[(*vv).nx][(*vv).ny].v[(*vv).n].zn)
			s = e3(r,may[(*vv).x][(*vv).y].v[i].l) + e3(r,may[(*vv).nx][(*vv).ny].v[(*vv).n].l) - f;
		else
			s = -e3(r,may[(*vv).x][(*vv).y].v[i].l) - e3(r,may[(*vv).nx][(*vv).ny].v[(*vv).n].l) - f;
		t = (*vv).v + s;
		f = (t - (*vv).v) - s;
		(*vv).v = t;

#if XG == 1
		if (may[(*vv).x][(*vv).y].v[i].x < X / 2)
			r = rrb(-may[(*vv).x][(*vv).y].v[i].x,may[(*vv).x][(*vv).y].v[i].y, may[(*vv).nx][(*vv).ny].v[(*vv).n].x,may[(*vv).nx][(*vv).ny].v[(*vv).n].y);
		else
			r = rrb(2 * X - may[(*vv).x][(*vv).y].v[i].x,may[(*vv).x][(*vv).y].v[i].y, may[(*vv).nx][(*vv).ny].v[(*vv).n].x,may[(*vv).nx][(*vv).ny].v[(*vv).n].y);

		s = ((may[(*vv).x][(*vv).y].v[i].zn == may[(*vv).nx][(*vv).ny].v[(*vv).n].zn) ? -1 : 1) * (e3(r,may[(*vv).x][(*vv).y].v[i].l) + e3(r,may[(*vv).nx][(*vv).ny].v[(*vv).n].l)) - f;
		t = (*vv).v + s;
		f = (t - (*vv).v) - s;
		(*vv).v = t;
#endif
#if YG == 1
		if (may[(*vv).x][(*vv).y].v[i].y < Y / 2)
			r = rrb(may[(*vv).x][(*vv).y].v[i].x,-may[(*vv).x][(*vv).y].v[i].y, may[(*vv).nx][(*vv).ny].v[(*vv).n].x,may[(*vv).nx][(*vv).ny].v[(*vv).n].y);
		else
			r = rrb(may[(*vv).x][(*vv).y].v[i].x,2 * Y - may[(*vv).x][(*vv).y].v[i].y, may[(*vv).nx][(*vv).ny].v[(*vv).n].x,may[(*vv).nx][(*vv).ny].v[(*vv).n].y);

		s = ((may[(*vv).x][(*vv).y].v[i].zn == may[(*vv).nx][(*vv).ny].v[(*vv).n].zn) ? -1 : 1) * (e3(r,may[(*vv).x][(*vv).y].v[i].l) + e3(r,may[(*vv).nx][(*vv).ny].v[(*vv).n].l)) - f;
		t = (*vv).v + s;
		f = (t - (*vv).v) - s;
		(*vv).v = t;
#endif
	}

	pthread_exit(NULL);
}

// считает энергию взаимодействия вихря с вихрями в этом и соседними гексах
double eone3(int x, int y, int n) {	// x номер гекса, y номер гекса, номер вихря в гексе
	double v = 0, f = 0, s, t;
	int i, j, ii = -1, jj = -1, in = 3, jn = 3;

# if XG == 1
	if (x - 1 < 0) {in--; ii++;}
	if (x + 1 >= nx) in--;
#endif
#if YG == 1
	if (y - 1 < 0) {jn--; jj++;}
	if (y + 1 >= ny) jn--;
#endif

	pthread_t **th;
	th = malloc(in * sizeof(pthread_t *));
	for (i = 0; i < in; i++)
		th[i] = malloc(jn * sizeof(pthread_t));
	struct vn3e **vv;
	vv = malloc(in * sizeof(struct vn3e *));
	for (i = 0; i < in; i++)
		vv[i] = malloc(jn * sizeof(struct vn3e));

	for (i = 0; i < in; i++)
		for (j = 0; j < jn; j++)
		{
			vv[i][j].v = 0;
#if XG == 1
			vv[i][j].x = x + i + ii;
#else
			vv[i][j].x = (x + i + ii >= nx) ? 0 : (x + i + ii < 0) ? nx - 1 : x + i + ii;
#endif
#if YG == 1
			vv[i][j].y = y + j + jj;
#else
			vv[i][j].y = (y + j + jj >= ny) ? 0 : (y + j + jj < 0) ? ny - 1 : y + j + jj;
#endif
			vv[i][j].nx = x;
			vv[i][j].ny = y;
			vv[i][j].n = n;
			pthread_create(&th[i][j], NULL, eone33, (void *)&vv[i][j]);
		}

#if XG == 1
	if (may[x][y].v[n].x < X / 2)
		r = rrb(-may[x][y].v[n].x,may[x][y].v[n].y, may[x][y].v[n].x,may[x][y].v[n].y);
	else
		r = rrb(2 * X - may[x][y].v[n].x,may[x][y].v[n].y, may[x][y].v[n].x,may[x][y].v[n].y);

	s = ((may[x][y].v[n].zn == may[x][y].v[n].zn) ? -1 : 1) * (e3(r,may[x][y].v[n].l) + e3(r,may[x][y].v[n].l)) - f;
	t = v + s;
	f = (t - v) - s;
	v = t;
#endif
#if YG == 1
	if (may[x][y].v[n].y < Y / 2)
		r = rrb(may[x][y].v[n].x,-may[x][y].v[n].y, may[x][y].v[n].x,may[x][y].v[n].y);
	else
		r = rrb(may[x][y].v[n].x,2 * Y - may[x][y].v[n].y, may[x][y].v[n].x,may[x][y].v[n].y);

	s = ((may[x][y].v[n].zn == may[x][y].v[n].zn) ? -1 : 1) * (e3(r,may[x][y].v[n].l) + e3(r,may[x][y].v[n].l)) - f;
	t = v + s;
	f = (t - v) - s;
	v = t;
#endif

	for (i = 0; i < in; i++)
		for (j = 0; j < jn; j++)
		{
			pthread_join(th[i][j], NULL);
			s = vv[i][j].v - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
		}

	// на всякий случай
	free(vv); free(th);

	return v;
}


// считает энергию взаимодействия вихрей друг с другом в гексе и в соседних гексах
void *eone3g(void *a) {
	double f = 0, s, t, r;
	int i, j, p, q, ii = -1, jj = -1, in = 3, jn = 3;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

# if XG == 1
	if ((*vv).x - 1 < 0) {in--; ii++;}
	if ((*vv).x + 1 >= nx) in--;
#endif
#if YG == 1
	if ((*vv).y - 1 < 0) {jn--; jj++;}
	if ((*vv).y + 1 >= ny) jn--;
#endif

	for (p = 0; p < may[(*vv).x][(*vv).y].nv; p++)
	{
#if XG == 1
		i = (*vv).x + ii;
#else
		i = ((*vv).x + ii >= nx) ? 0 : ((*vv).x + ii < 0) ? nx - 1 : (*vv).x + ii;
#endif
#if YG == 1
		j = (*vv).y + jj;
#else
		j = ((*vv).y + jj >= ny) ? 0 : ((*vv).y + jj < 0) ? ny - 1 : (*vv).y + jj;
#endif
		for (; i < in; i++)
			for (; j < jn; j++)
				for (q = 0; q < may[i][j].nv; q++)
				{
					if (i == (*vv).x && j == (*vv).y && q == p) continue;

					r = rrr(may[i][j].v[q].x,may[i][j].v[q].y, may[(*vv).x][(*vv).y].v[p].x,may[(*vv).x][(*vv).y].v[p].y);
					if (may[i][j].v[q].zn == may[(*vv).x][(*vv).y].v[p].zn)
						s = e3(r,may[(*vv).x][(*vv).y].v[p].l) - f;
					else
						s = -e3(r,may[(*vv).x][(*vv).y].v[p].l) - f;
					t = (*vv).v + s;
					f = (t - (*vv).v) - s;
					(*vv).v = t;
				}
	}

	pthread_exit(NULL);
}

// считает энергию взаимодействия вихрей друг с другом во всей системе
double tote3() {
	double v = 0, f = 0, s, t;
	int i, j;
	pthread_t *th;
	th = malloc(nx * ny * sizeof(pthread_t));
	struct vn3e *vv;
	vv = malloc(nx * ny * sizeof(struct vn3e));

	for (j = i = 0; i < nx * ny; i++)
	{
		vv[i].v = 0;
		vv[i].x = i / ny;
		vv[i].y = i % ny;
		pthread_create(&th[i], NULL, eone2, (void *)&vv[i]);

		if (i - j >= CPOT)
		{
			pthread_join(th[j], NULL);
			s = vv[j].v - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
			j++;
		}
	}
	while (j < i)
	{
		pthread_join(th[j], NULL);
		s = vv[j].v - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
		j++;
	}

	// на всякий случай
	free(vv); free(th);

	return v;
}




// считает энергию взаимодействия заданного вихря с примесью в заданном гексе
void *eone44(void *a) {
	double r, ee;
	struct vn3e *vv;
	vv = (struct vn3e *)a;

	for (int i = 0; i < may[(*vv).x][(*vv).y].np; i++)
	{
		r = rrr(may[(*vv).x][(*vv).y].p[i].x,may[(*vv).x][(*vv).y].p[i].y, may[(*vv).nx][(*vv).ny].v[(*vv).n].x,may[(*vv).nx][(*vv).ny].v[(*vv).n].y);
		ee = e4(r,may[(*vv).nx][(*vv).ny].v[(*vv).n].xi,may[(*vv).x][(*vv).y].p[i].u);
		if ((*vv).v > ee) (*vv).v = ee;
	}

	pthread_exit(NULL);
}

// считает энергию взаимодействий вихря с примесью в этом и соседнем гексе
double eone4(int x, int y, int n) {	// x номер гекса, y номер гекса, номер вихря в гексе
	double v = 0;
	int i, j, ii = -1, jj = -1, in = 3, jn = 3;

# if XG == 1
	if (x - 1 < 0) {in--; ii++;}
	if (x + 1 >= nx) in--;
#endif
#if YG == 1
	if (y - 1 < 0) {jn--; jj++;}
	if (y + 1 >= ny) jn--;
#endif

	pthread_t **th;
	th = malloc(in * sizeof(pthread_t *));
	for (i = 0; i < in; i++)
		th[i] = malloc(jn * sizeof(pthread_t));
	struct vn3e **vv;
	vv = malloc(in * sizeof(struct vn3e *));
	for (i = 0; i < in; i++)
		vv[i] = malloc(jn * sizeof(struct vn3e));

	for (i = 0; i < in; i++)
		for (j = 0; j < jn; j++)
		{
			vv[i][j].v = 0;
#if XG == 1
			vv[i][j].x = x + i + ii;
#else
			vv[i][j].x = (x + i + ii >= nx) ? 0 : (x + i + ii < 0) ? nx - 1 : x + i + ii;
#endif
#if YG == 1
			vv[i][j].y = y + j + jj;
#else
			vv[i][j].y = (y + j + jj >= ny) ? 0 : (y + j + jj < 0) ? ny - 1 : y + j + jj;
#endif
			vv[i][j].nx = x;
			vv[i][j].ny = y;
			vv[i][j].n = n;
			pthread_create(&th[i][j], NULL, eone44, (void *)&vv[i][j]);
		}

	for (i = 0; i < in; i++)
		for (j = 0; j < jn; j++)
		{
			pthread_join(th[i][j], NULL);
			if (vv[i][j].v < v) v = vv[i][j].v;
		}

	// на всякий случай
	free(vv); free(th);

	return v;
}


// считает энергию взаимодействия вихрей с примесью для заданного гекса
void *eone4g(void *a) {
	double r, s, f = 0, t, ee, de = 0;
	int i, j, p, q, ii = -1, jj = -1, in = 3, jn = 3;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

# if XG == 1
	if ((*vv).x - 1 < 0) {in--; ii++;}
	if ((*vv).x + 1 >= nx) in--;
#endif
#if YG == 1
	if ((*vv).y - 1 < 0) {jn--; jj++;}
	if ((*vv).y + 1 >= ny) jn--;
#endif

	for (p = 0; p < may[(*vv).x][(*vv).y].nv; p++, de = 0)
	{
#if XG == 1
		i = (*vv).x + ii;
#else
		i = ((*vv).x + ii >= nx) ? 0 : ((*vv).x + ii < 0) ? nx - 1 : (*vv).x + ii;
#endif
#if YG == 1
		j = (*vv).y + jj;
#else
		j = ((*vv).y + jj >= ny) ? 0 : ((*vv).y + jj < 0) ? ny - 1 : (*vv).y + jj;
#endif
		for (; i < in; i++)
			for (; j < jn; j++)
				for (q = 0; q < may[i][j].np; q++)
				{
					r = rrr(may[i][j].p[q].x,may[i][j].p[q].y, may[(*vv).x][(*vv).y].v[p].x,may[(*vv).x][(*vv).y].v[p].y);
					ee = e4(r,may[(*vv).x][(*vv).y].v[p].xi,may[i][j].p[q].u);
					if (de < ee) de = ee;
				}
		s = de - f;
		t = (*vv).v + s;
		f = (t - (*vv).v) - s;
		(*vv).v = t;
	}

	pthread_exit(NULL);
}

// считает энергию взаимодействия вихрей с примесью
double tote4() {
	double v = 0, f = 0, s, t;
	int i, j;
	pthread_t *th;
	th = malloc(nx * ny * sizeof(pthread_t));
	struct vn12 *vv;
	vv = malloc(nx * ny * sizeof(struct vn12));

	for (j = i = 0; i < nx * ny; i++)
	{
		vv[i].v = 0;
		vv[i].x = i / ny;
		vv[i].y = i % ny;
		pthread_create(&th[i], NULL, eone2, (void *)&vv[i]);

		if (i - j >= CPOT)
		{
			pthread_join(th[j], NULL);
			s = vv[j].v - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
			j++;
		}
	}
	while (j < i)
	{
		pthread_join(th[j], NULL);
		s = vv[j].v - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
		j++;
	}

	// на всякий случай
	free(vv); free(th);

	return v;
}





// считает все стационарные энергии, посчитанные ранее
double tote124() {
	double v = 0, f = 0, s, t;
	int i, j, q;

	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			for (q = 0; q < may[i][j].nv; q++)
			{
				s = may[i][j].v[q].e - f;
				t = v + s;
				f = (t - v) - s;
				v = t;
			}

	return v;
}