// считает энергию от магнитного поля для вихревой нити
double eone1old(struct vortex *a) {
	double v = 0, f = 0, s, t;

	for (int i = 0; i < dsn; i++)
	{
		if (!a[i].zn) continue;
		s = ((a[i].zn == 'o') ? 1 : -1) * e1(a[i].h,a[i].l,a[i].x,a[i].y,a[i].z) - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
	}

	return v;
}

// считает энергию от магнитного поля для всей системы, имеющей n нитей
double tote1old(struct vortex **a, int n) {
	double v = 0, f = 0, s, t;
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < dsn; j++)
		{
			if (!a[i][j].zn) continue;
			s = ((a[i][j].zn == 'o') ? 1 : -1) * e1(a[i][j].h,a[i][j].l,a[i][j].x,a[i][j].y,a[i][j].z) - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
		}

	return v;
}


// считает энергию от магнитного поля для вихревой нити
void *eone1(void *a) {
	double f = 0, s, t;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

	for (int i = 0; i < dsn; i++)
	{
		if (!(*vv).v[i].zn) continue;
		s = (((*vv).v[i].zn == 'o') ? 1 : -1) * e1((*vv).v[i].h,(*vv).v[i].l,(*vv).v[i].x,(*vv).v[i].y,(*vv).v[i].z) - f;
		t = (*vv).e + s;
		f = (t - (*vv).e) - s;
		(*vv).e = t;
	}

	pthread_exit(NULL);
}

// считает энергию от магнитного поля для всей системы, имеющей n нитей
double tote1(struct vortex **a, int n) {
	double ve = 0, f = 0, s, t;
	int i;
	pthread_t *th;
	th = malloc(n * sizeof(pthread_t));
	struct vn12 *vv;
	vv = malloc(n * sizeof(struct vn12));

	for (i = 0; i < n; i++)
	{
		vv[i].v = a[i];
		vv[i].e = 0;
		pthread_create(&th[i], NULL, eone1, (void *)&vv[i]);
	}

	for (i = 0; i < n; i++)
		pthread_join(th[i], NULL);

	for (i = 0; i < n; i++)
	{
		s = vv[i].e - f;
		t = ve + s;
		f = (t - ve) - s;
		ve = t;
	}

	// на всякий случай
	free(vv); free(th);

	return ve;
}





// считает внутреннюю энергию для вихревой нити
double eone2old(struct vortex *a) {
	double v = 0, f = 0, s, t;

	for (int i = 0; i < dsn; i++)
	{
		if (!a[i].zn) continue;
		s = e2(a[i].l,a[i].xi) - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
	}

	return v;
}

// считает внутреннюю энергию для всей системы, имеющей n нитей
double tote2old(struct vortex **a, int n) {
	double v = 0, f = 0, s, t;
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < dsn; j++)
		{
			if (!a[i][j].zn) continue;
			s = e2(a[i][j].l,a[i][j].xi) - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
		}

	return v;
}


// считает внутреннюю энергию для вихревой нити
void *eone2(void *a) {
	double f = 0, s, t;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

	for (int i = 0; i < dsn; i++)
	{
		if (!(*vv).v[i].zn) continue;
		s = e2((*vv).v[i].l,(*vv).v[i].xi) - f;
		t = (*vv).e + s;
		f = (t - (*vv).e) - s;
		(*vv).e = t;
	}

	pthread_exit(NULL);
}

// считает внутреннюю энергию для всей системы, имеющей n нитей
double tote2(struct vortex **a, int n) {
	double ve = 0, f = 0, s, t;
	int i;
	pthread_t *th;
	th = malloc(n * sizeof(pthread_t));
	struct vn12 *vv;
	vv = malloc(n * sizeof(struct vn12));

	for (i = 0; i < n; i++)
	{
		vv[i].v = a[i];
		vv[i].e = 0;
		pthread_create(&th[i], NULL, eone2, (void *)&vv[i]);
	}

	for (i = 0; i < n; i++)
		pthread_join(th[i], NULL);

	for (i = 0; i < n; i++)
	{
		s = vv[i].e - f;
		t = ve + s;
		f = (t - ve) - s;
		ve = t;
	}

	// на всякий случай
	free(vv); free(th);

	return ve;
}





// считает энергию взаимодействия узла нити с другими узлами в той же плоскости (nl -- номер линии; nv -- номер узла в линии; n -- количество нитей всего)
double eone3old(struct vortex **a, int n, int nl, int nv) {
	if (!a[nl][nv].zn) return 0;	// узел не активен (todo: и что делать? такого не должно быть)

	double v = 0, f = 0, s, t, r;
	int i;

	for (i = 0; i < n; i++)
	{
		if (!a[i][nv].zn || i == nl) continue;

		r = rrr(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,a[i][nv].x,a[i][nv].y,a[i][nv].z);
		// todo: добавить обрезку взаимодействия по радиусу
		s = ((a[nl][nv].zn == a[i][nv].zn) ? 1 : -1) * (e3(r,a[nl][nv].l) + e3(r,a[i][nv].l)) - f;
		t = v + s;
		f = (t - v) - s;
		v = t;

#if XG == 1
		if (a[i][nv].x < X / 2)
			r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,-a[i][nv].x,a[i][nv].y,a[i][nv].z);
		else
			r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,2 * X - a[i][nv].x,a[i][nv].y,a[i][nv].z);
		// todo: добавить обрезку взаимодействия по радиусу
		s = ((a[nl][nv].zn == a[i][nv].zn) ? -1 : 1) * (e3(r,a[nl][nv].l) + e3(r,a[i][nv].l)) - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
#endif
#if YG == 1
		if (a[i][nv].y < Y / 2)
			r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,a[i][nv].x,-a[i][nv].y,a[i][nv].z);
		else
			r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,a[i][nv].x,2 * Y - a[i][nv].y,a[i][nv].z);
		// todo: добавить обрезку взаимодействия по радиусу
		s = ((a[nl][nv].zn == a[i][nv].zn) ? -1 : 1) * (e3(r,a[nl][nv].l) + e3(r,a[i][nv].l)) - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
#endif
	}

#if XG == 1
	if (a[nl][nv].x < X / 2)
		r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,-a[nl][nv].x,a[nl][nv].y,a[nl][nv].z);
	else
		r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,2 * X - a[nl][nv].x,a[nl][nv].y,a[nl][nv].z);
	// todo: добавить обрезку взаимодействия по радиусу
	s = -e3(r,a[nl][nv].l) - f;
	t = v + s;
	f = (t - v) - s;
	v = t;
#endif
#if YG == 1
	if (a[nl][nv].y < Y / 2)
		r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,a[nl][nv].x,-a[nl][nv].y,a[nl][nv].z);
	else
		r = rrb(a[nl][nv].x,a[nl][nv].y,a[nl][nv].z,a[nl][nv].x,2 * Y - a[nl][nv].y,a[nl][nv].z);
	// todo: добавить обрезку взаимодействия по радиусу
	s = -e3(r,a[nl][nv].l) - f;
	t = v + s;
	f = (t - v) - s;
	v = t;
#endif

	return v;
}

// считает энергию взаимодействия всех узлов в одинаковой плоскости друг с другом (n -- количество нитей всего)
double tote3old(struct vortex **a, int n) {
	double r, v = 0, f = 0, s, t;
	int i, j, p;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < i; j++)
			for (p = 0; p < dsn; p++)
			{
				if (!a[i][p].zn || !a[j][p].zn) continue;
				r = rrr(a[i][p].x,a[i][p].y,a[i][p].z,a[j][p].x,a[j][p].y,a[j][p].z);
				// todo: добавить обрезку взаимодействия по радиусу
				s = ((a[i][p].zn == a[j][p].zn) ? 1 : -1) * (e3(r,a[i][p].l) + e3(r,a[j][p].l)) - f;
				t = v + s;
				f = (t - v) - s;
				v = t;

#if XG == 1
				if (a[j][p].x < X / 2)
					r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,-a[j][p].x,a[j][p].y,a[j][p].z);
				else
					r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,2 * X - a[j][p].x,a[j][p].y,a[j][p].z);
				// todo: добавить обрезку взаимодействия по радиусу
				s = ((a[i][p].zn == a[j][p].zn) ? -1 : 1) * (e3(r,a[i][p].l) + e3(r,a[j][p].l)) - f; // todo: делить на 2?
				t = v + s;
				f = (t - v) - s;
				v = t;

				if (a[i][p].x < X / 2)
					r = rrb(-a[i][p].x,a[i][p].y,a[i][p].z,a[j][p].x,a[j][p].y,a[j][p].z);
				else
					r = rrb(2 * X - a[i][p].x,a[i][p].y,a[i][p].z,a[j][p].x,a[j][p].y,a[j][p].z);
				// todo: добавить обрезку взаимодействия по радиусу
				s = ((a[i][p].zn == a[j][p].zn) ? -1 : 1) * (e3(r,a[i][p].l) + e3(r,a[j][p].l)) - f; // todo: делить на 2?
				t = v + s;
				f = (t - v) - s;
				v = t;
#endif
#if YG == 1
				if (a[j][p].x < Y / 2)
					r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,a[j][p].x,-a[j][p].y,a[j][p].z);
				else
					r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,a[j][p].x,2 * Y - a[j][p].y,a[j][p].z);
				// todo: добавить обрезку взаимодействия по радиусу
				s = ((a[i][p].zn == a[j][p].zn) ? -1 : 1) * (e3(r,a[i][p].l) + e3(r,a[j][p].l)) - f; // todo: делить на 2?
				t = v + s;
				f = (t - v) - s;
				v = t;

				if (a[i][p].x < Y / 2)
					r = rrb(a[i][p].x,-a[i][p].y,a[i][p].z,a[j][p].x,a[j][p].y,a[j][p].z);
				else
					r = rrb(a[i][p].x,2 * Y - a[i][p].y,a[i][p].z,a[j][p].x,a[j][p].y,a[j][p].z);
				// todo: добавить обрезку взаимодействия по радиусу
				s = ((a[i][p].zn == a[j][p].zn) ? -1 : 1) * (e3(r,a[i][p].l) + e3(r,a[j][p].l)) - f; // todo: делить на 2?
				t = v + s;
				f = (t - v) - s;
				v = t;
#endif
			}

#if XG == 1
		for (p = 0; p < dsn; p++)
		{
			if (!a[i][p].zn) continue;
			if (a[i][p].x < X / 2)
				r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,-a[i][p].x,a[i][p].y,a[i][p].z);
			else
				r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,2 * X - a[i][p].x,a[i][p].y,a[i][p].z);
			// todo: добавить обрезку взаимодействия по радиусу
			s = -e3(r,a[i][p].l) - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
		}
#endif
#if YG == 1
		for (p = 0; p < dsn; p++)
		{
			if (!a[i][p].zn) continue;
			if (a[i][p].y < Y / 2)
				r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,a[i][p].x,-a[i][p].y,a[i][p].z);
			else
				r = rrb(a[i][p].x,a[i][p].y,a[i][p].z,a[i][p].x,2 * Y - a[i][p].y,a[i][p].z);
			// todo: добавить обрезку взаимодействия по радиусу
			s = -e3(r,a[i][p].l) - f;
			t = v + s;
			f = (t - v) - s;
			v = t;
		}
#endif
	}

	return v;
}


// помошник, считающий энергии взаимодействия отдельных вихрей, для энергии взаимодействия узла нити с другими узлами в той же плоскости
void *eone3help(void *a) {
	double r;
	struct vn3 *vv;
	vv = (struct vn3 *)a;


	r = rrr(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,aa[(*vv).i][(*vv).nv].x,aa[(*vv).i][(*vv).nv].y,aa[(*vv).i][(*vv).nv].z);
	// todo: добавить обрезку взаимодействия по радиусу
#if XG || YG
	double f, s, t;
	s = ((aa[(*vv).nl][(*vv).nv].zn == aa[(*vv).i][(*vv).nv].zn) ? 1 : -1) * (e3(r,aa[(*vv).nl][(*vv).nv].l) + e3(r,aa[(*vv).i][(*vv).nv].l));
	t = (*vv).ee[(*vv).i] + s;
	f = (t - (*vv).ee[(*vv).i]) - s;
	(*vv).ee[(*vv).i] = t;
#else
	(*vv).ee[(*vv).i] = ((aa[(*vv).nl][(*vv).nv].zn == aa[(*vv).i][(*vv).nv].zn) ? 1 : -1) * (e3(r,aa[(*vv).nl][(*vv).nv].l) + e3(r,aa[(*vv).i][(*vv).nv].l));
#endif

#if XG == 1
	if (aa[(*vv).i][(*vv).nv].x < X / 2)
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,-aa[(*vv).i][(*vv).nv].x,aa[(*vv).i][(*vv).nv].y,aa[(*vv).i][(*vv).nv].z);
	else
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,2 * X - aa[(*vv).i][(*vv).nv].x,aa[(*vv).i][(*vv).nv].y,aa[(*vv).i][(*vv).nv].z);
	// todo: добавить обрезку взаимодействия по радиусу
	s = ((aa[(*vv).nl][(*vv).nv].zn == aa[(*vv).i][(*vv).nv].zn) ? 1 : -1) * (e3(r,aa[(*vv).nl][(*vv).nv].l) + e3(r,aa[(*vv).i][(*vv).nv].l)) - f; // todo: делить на 2?
	t = (*vv).ee[(*vv).i] + s;
	f = (t - (*vv).ee[(*vv).i]) - s;
	(*vv).ee[(*vv).i] = t;
#endif
#if YG == 1
	if (aa[(*vv).i][(*vv).nv].y < Y / 2)
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,aa[(*vv).i][(*vv).nv].x,-aa[(*vv).i][(*vv).nv].y,aa[(*vv).i][(*vv).nv].z);
	else
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,aa[(*vv).i][(*vv).nv].x,2 * Y - aa[(*vv).i][(*vv).nv].y,aa[(*vv).i][(*vv).nv].z);
	// todo: добавить обрезку взаимодействия по радиусу
	s = ((aa[(*vv).nl][(*vv).nv].zn == aa[(*vv).i][(*vv).nv].zn) ? 1 : -1) * (e3(r,aa[(*vv).nl][(*vv).nv].l) + e3(r,aa[(*vv).i][(*vv).nv].l)) - f; // todo: делить на 2?
	t = (*vv).ee[(*vv).i] + s;
	f = (t - (*vv).ee[(*vv).i]) - s;
	(*vv).ee[(*vv).i] = t;
#endif

	pthread_exit(NULL);
}

// считает энергию взаимодействия узла нити с другими узлами в той же плоскости
void *eone3(void *a) {
#if XG || YG
	double v = 0, r;
#endif
	double f = 0, s, t;
	int i;
	struct vn3 *vv;
	vv = (struct vn3 *)a;
	pthread_t *th;
	th = malloc(no * sizeof(pthread_t));


	for (i = 0; i < no; i++)
	{
		if (i >= (*vv).nl) continue;

		(*vv).i = i;
		(*vv).ee[i] = 0;
		pthread_create(&th[i], NULL, eone3help, (void *)vv);
	}

#if XG == 1
	if (aa[(*vv).nl][(*vv).nv].x < X / 2)
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,-aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z);
	else
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,2 * X - aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z);
	// todo: добавить обрезку взаимодействия по радиусу
	s = -e3(r,aa[(*vv).nl][(*vv).nv].l);
	t = v + s;
	f = (t - v) - s;
	v = t;
#endif
#if YG == 1
	if (aa[(*vv).nl][(*vv).nv].y < Y / 2)
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,aa[(*vv).nl][(*vv).nv].x,-aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z);
	else
		r = rrb(aa[(*vv).nl][(*vv).nv].x,aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z,aa[(*vv).nl][(*vv).nv].x,2 * Y - aa[(*vv).nl][(*vv).nv].y,aa[(*vv).nl][(*vv).nv].z);
	// todo: добавить обрезку взаимодействия по радиусу
	s = -e3(r,aa[(*vv).nl][(*vv).nv].l) - f;
	t = v + s;
	f = (t - v) - s;
	v = t;
#endif

	for (i = 0; i < no; i++)
	{
		if (i >= (*vv).nl) continue;

		pthread_join(th[i], NULL);
	}

	for (i = 0; i < no; i++)
	{
	 	if (i >= (*vv).nl) continue;

	 	s = (*vv).ee[i] - f;
	 	t = (*vv).e + s;
	 	f = (t - (*vv).e) - s;
	 	(*vv).e = t;
	}

	// на всякий случай
	free(th); free((*vv).ee);

	pthread_exit(NULL);
}

// считает энергию взаимодействия всех узлов в одинаковой плоскости друг с другом (n -- количество нитей всего)
double tote3(struct vortex **a, int n) {
	double ve = 0, f = 0, s, t;
	int i;
	pthread_t *th;
	th = malloc((n * dsn) * sizeof(pthread_t));
	struct vn3 *vv;
	vv = malloc((n * dsn) * sizeof(struct vn3));


	aa = a; no = n;
	for (i = 0; i < (n * dsn); i++)
	{
		vv[i].nl = i / dsn;
		vv[i].nv = i % dsn;
		if (!a[vv[i].nl][vv[i].nv].zn) continue;

		vv[i].e = 0;
		vv[i].ee = malloc(n * sizeof(double));
		pthread_create(&th[i], NULL, eone3, (void *)&vv[i]);
	}

	for (i = 0; i < (n * dsn); i++)
	{
		if (!a[vv[i].nl][vv[i].nv].zn) continue;
	 	pthread_join(th[i], NULL);
	}

	for (i = 0; i < (n * dsn); i++)
	{
	 	s = vv[i].e - f;
	 	t = ve + s;
	 	f = (t - ve) - s;
	 	ve = t;
	}

	// на всякий случай
	free(vv); free(th);

	return ve;
}




// проблема!!
// считает энергию взаимодействия одного узла вихря с примесью
double eone4old1(struct vortex *a, int n) {
#if (SNP + CNP) == 0
	return 0;
#endif

	int i, ii;
	double r, rr = X + Y;

	for (ii = i = 0; i < SNP + CNP; i++)
	{
		r = rrr(a[n].x,a[n].y,a[n].z,pr[n][i].x,pr[n][i].y,pr[n][i].z);
		if (r < rr)
		{
			rr = r;
			ii = i;
		}
	}

	return e4(rr,a[n].xi,pr[n][ii].u);
}


// считает энергию взаимодействия вихря с примесью
double eone4old(struct vortex *a) {
	int i, j, ii;
	double r, rr, v = 0, f = 0, s, t;

	for (i = 0; i < dsn; i++)
	{
		if (!a[i].zn) continue;

		for (rr = rrr(a[i].x,a[i].y,a[i].z,pr[i][0].x,pr[i][0].y,pr[i][0].z), ii = 0, j = 1; j < SNP + CNP; j++)
		{
			r = rrr(a[i].x,a[i].y,a[i].z,pr[i][j].x,pr[i][j].y,pr[i][j].z);
			if (r < rr)
			{
				rr = r;
				ii = j;
			}
		}

		s = e4(rr,a[i].xi,pr[i][ii].u) - f;
		t = v + s;
		f = (t - v) - s;
		v = t;
	}

	return v;
}

// считает энергию взаимодействия всех вихрей с примесью в системе, имеющей n нитей
double tote4old(struct vortex **a, int n) {
	int i, j, ii, jj;
	double r, rr, v = 0, f = 0, s, t;

	for (jj = 0; jj < n; jj++)
		for (i = 0; i < dsn; i++)
	 	{
	 		if (!a[jj][i].zn) continue;

			for (rr = rrr(a[jj][i].x,a[jj][i].y,a[jj][i].z,pr[i][0].x,pr[i][0].y,pr[i][0].z), ii = 0, j = 1; j < SNP + CNP; j++)
			{
				r = rrr(a[jj][i].x,a[jj][i].y,a[jj][i].z,pr[i][j].x,pr[i][j].y,pr[i][j].z);
				if (r < rr)
	 			{
		 			rr = r;
		 			ii = j;
		 		}
		 	}

		 	s = e4(rr,a[jj][i].xi,pr[i][ii].u) - f;
		 	t = v + s;
		 	f = (t - v) - s;
		 	v = t;
		}

	return v;
}


// считает энергию взаимодействия вихря с примесью
void *eone4(void *a) {
	int i, j, ii;
	double r, rr, f = 0, s, t;
	struct vn12 *vv;
	vv = (struct vn12 *)a;

	for (i = 0; i < dsn; i++)
	{
		if (!(*vv).v[i].zn) continue;

		for (rr = rrr((*vv).v[i].x,(*vv).v[i].y,(*vv).v[i].z,pr[i][0].x,pr[i][0].y,pr[i][0].z), ii = 0, j = 1; j < SNP + CNP; j++)
		{
			r = rrr((*vv).v[i].x,(*vv).v[i].y,(*vv).v[i].z,pr[i][j].x,pr[i][j].y,pr[i][j].z);
			if (r < rr)
			{
				rr = r;
				ii = j;
			}
		}

		s = e4(rr,(*vv).v[i].xi,pr[i][ii].u) - f;
		t = (*vv).e + s;
		f = (t - (*vv).e) - s;
		(*vv).e = t;
	}

	pthread_exit(NULL);
}

// считает всю энергию взаимодействия всех вихрй с примесью в системе, имеющей n нитей
double tote4(struct vortex **a, int n) {
	double ve = 0, f = 0, s, t;
	int i;
	pthread_t *th;
	th = malloc(n * sizeof(pthread_t));
	struct vn12 *vv;
	vv = malloc(n * sizeof(struct vn12));

	for (i = 0; i < n; i++)
	{
		vv[i].v = a[i];
		vv[i].e = 0;
		pthread_create(&th[i], NULL, eone4, (void *)&vv[i]);
	}

	for (i = 0; i < n; i++)
		pthread_join(th[i], NULL);

	for (i = 0; i < n; i++)
	{
		s = vv[i].e - f;
		t = ve + s;
		f = (t - ve) - s;
		ve = t;
	}

	// на всякий случай
	free(vv); free(th);

	return ve;
}





// считает энергию взаимодействия узла с токами слоя, следующего за n-ым по счёту узлом в нити
double eone5old_next(struct vortex *a, int n) {
	if (!a[n].zn) return 0;

	for (int j, i = 1; i < dsn; i++)
	{
#if ZG == 1
		if (n + i == dsn) return E5Z;
#endif
		j = (n + i) % dsn;
		if (a[j].zn) return e5(rrr(a[n].x,a[n].y,a[n].z,a[j].x,a[j].y,a[j].z),a[n].l);
	}

	return 0; // todo: это как?
}

// считает энергию взаимодействия узла с токами слоя, предыдущего n-ому по счёту узлу в нити
double eone5old_prev(struct vortex *a, int n) {
	if (!a[n].zn) return 0;

	for (int j, i = 1; i < dsn; i++)
	{
#if ZG == 1
		if (n - i == 0) return E5Z;
#endif
		j = (n - i + dsn) % dsn;
		if (a[j].zn) return e5(rrr(a[n].x,a[n].y,a[n].z,a[j].x,a[j].y,a[j].z),a[n].l);
	}

	return 0; // todo: это как?
}

// считает энергию взаимодействия узлов с токами слоёв, следующих за i-ым по счёту узлом в нити
double tote5old_next(struct vortex **a, int n) {
	double v = 0, f = 0, s, t;
	int i, ii, j, jj;

	for (i = 0; i < n; i++)
	{
		for (ii = 0; ii < dsn; ii++)
			for (jj = j = 1; j < dsn; j++)
			{
#if ZG == 1
				if (ii + j == dsn)
				{
					s = E5Z - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
#endif
				jj = (ii + j) % dsn;
				if (a[i][jj].zn)
				{
					s = e5(rrr(a[i][ii].x,a[i][ii].y,a[i][ii].z,a[i][jj].x,a[i][jj].y,a[i][jj].z),a[i][jj].l) - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
			}
	}

	return v;
}

// считает энергию взаимодействия узлов с токами слоёв, предыдущих i-ому по счёту узлу в нити
double tote5old_prev(struct vortex **a, int n) {
	double v = 0, f = 0, s, t;
	int i, ii, j, jj;

	for (i = 0; i < n; i++)
	{
		for (ii = 0; ii < dsn; ii++)
			for (jj = j = 1; j < dsn; j++)
			{
#if ZG == 1
				if (ii - j == 0)
				{
					s = E5Z - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
#endif
				jj = (ii - j + dsn) % dsn;
				if (a[i][jj].zn)
				{
					s = e5(rrr(a[i][ii].x,a[i][ii].y,a[i][ii].z,a[i][jj].x,a[i][jj].y,a[i][jj].z),a[i][jj].l) - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
			}
	}

	return v;
}


// //
// void *eone5(void *a) {
// }

// //
// double tote5(struct vortex **a, int n) {
// }





// считает энергию взаимодействия узла вихря с соседними частями вихревой нити, следующего за n-ым по счёту узлом в нити
double eone6old_next(struct vortex *a, int n) {
	if (!a[n].zn) return 0;

	for (int j, i = 1; i < dsn; i++)
	{
#if ZG == 1
		if (n + i == dsn) return E6Z;
#endif
		j = (n + i) % dsn;
		if (a[j].zn) return e6(rrr(a[n].x,a[n].y,a[n].z,a[j].x,a[j].y,a[j].z),a[n].l);
	}

	return 0; // todo: это как?
}

// считает энергию взаимодействия узла вихря с соседними частями вихревой нити, предыдущего n-ому по счёту узлу в нити
double eone6old_prev(struct vortex *a, int n) {
	if (!a[n].zn) return 0;

	for (int j, i = 1; i < dsn; i++)
	{
#if ZG == 1
		if (n - i == 0) return E6Z;
#endif
		j = (n - i + dsn) % dsn;
		if (a[j].zn) return e6(rrr(a[n].x,a[n].y,a[n].z,a[j].x,a[j].y,a[j].z),a[n].l);
	}

	return 0; // todo: это как?
}

// считает энергию взаимодействия узла вихря с соседними частями вихревой нити, следующих за i-ым по счёту узлом в нити
double tote6old_next(struct vortex **a, int n) {
	double v = 0, f = 0, s, t;
	int i, ii, j, jj;

	for (i = 0; i < n; i++)
	{
		for (ii = 0; ii < dsn; ii++)
			for (jj = j = 1; j < dsn; j++)
			{
#if ZG == 1
				if (ii + j == dsn)
				{
					s = E6Z - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
#endif
				jj = (ii + j) % dsn;
				if (a[i][jj].zn)
				{
					s = e6(rrr(a[i][ii].x,a[i][ii].y,a[i][ii].z,a[i][jj].x,a[i][jj].y,a[i][jj].z),a[i][jj].l) - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
			}
	}

	return v;
}

// считает энергию взаимодействия узла вихря с соседними частями вихревой нити, предыдущих i-ому по счёту узлу в нити
double tote6old_prev(struct vortex **a, int n) {
	double v = 0, f = 0, s, t;
	int i, ii, j, jj;

	for (i = 0; i < n; i++)
	{
		for (ii = 0; ii < dsn; ii++)
			for (jj = j = 1; j < dsn; j++)
			{
#if ZG == 1
				if (ii - j == 0)
				{
					s = E6Z - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
#endif
				jj = (ii - j + dsn) % dsn;
				if (a[i][jj].zn)
				{
					s = e6(rrr(a[i][ii].x,a[i][ii].y,a[i][ii].z,a[i][jj].x,a[i][jj].y,a[i][jj].z),a[i][jj].l) - f;
					t = v + s;
					f = (t - v) - s;
					v = t;
					break;
				}
			}
	}

	return v;
}


// //
// double *eone6(void *a) {
// }

// //
// double tote6(struct vortex **a, int n) {
// }





// считает все стационарные энергии, записаные заранее в vortex *a (vortex *a имеет размер n)
// double tote124(struct vortex *a, int n) {
// 	double r, v = 0, f = 0, s, t;
// 	int i, j;

// 	for (i = 0; i < n; i++)
// 	{
// 		s = a[i].e - f;
// 		t = v + s;
// 		f = (t - v) - s;
// 		v = t;
// 	}

// 	return v;
// }