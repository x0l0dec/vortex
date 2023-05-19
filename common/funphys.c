// модуль
double m(double i) {
	return i > 0 ? i : -i;
}

// меняет местами два double
void swapch(double *a, double *b) {
	double c = *a; *a = *b; *b = c;
}

// распределение глубины примесей в зависимости от их местоположения
double upp(double x, double y) {
	return (double)rand() / RAND_MAX * m(U0L - U0M) + U0M;	// случайное распределение
//	return exp(-pow(((double)rand() / RAND_MAX - PMAT) / PDIS,2) / 2) / (PDIS * pow(2 * M_PI, 0.5));	// гауссово распределение
}

// внешнее магнитное поле He(x,y)
double pole(double x, double y) {
	return HE;
}

// магнитное поле от тока Hi(x,y)
double poli(double x, double y) {
	return HI;
}

// температура T(x,y)
double temp(double x, double y) {
	return T;
}

// глубина проникновения λ(T)
double lamb(double t) {
	return L;
}

// длина когерентности ξ(T)
double ksii(double t) {
	return K;
}

// энергия взаимодействия с внешнем магнитным полем E1(H,λ,x,y)
double e1e(double h, double l, double x, double y) {
#if (XG == 0 && YG == 0)
	return EE1;
#elif (XG == 1 && YG == 0)
	return EEX;
#elif (XG == 0 && YG == 1)
	return EEY;
#endif
}

// энергия взаимодействия с магнитным полем от тока E1(H,λ,x,y)
double e1i(double h, double l, double x, double y) {
#if (XG == 0 && YG == 0)
	return EI1;
#elif (XG == 1 && YG == 0)
	return EIX;
#elif (XG == 0 && YG == 1)
	return EIY;
#endif
}

// собственая энергия вихря E2(λ,ξ)
double e2(double l, double xi) {
	return E2;
}

// энергия взаимодействия вихря с другим вихрем E3(r,λ)
double e3(double r, double l) {
	return E3;
}

// энергия взаимодействия вихря с примесью E4(r,ξ,U)
double e4(double r, double xi, double u) {
	return E4;
}

// определяет минимальное расстояние между двумя точками с учётом стенок и границ системы
double rrr(double x1, double y1, double x2, double y2) {
	double x = m(x1 - x2), y = m(y1 - y2);

#if XG == 0
	x = (x < X - x) ? x : X - x;
#endif
#if YG == 0
	y = (y < Y - y) ? y : Y - y;
#endif

	return pow(x * x + y * y, 0.5);
}

// определяет минимальное расстояние между двумя точками без учёта стенок и границ системы
double rrb(double x1, double y1, double x2, double y2) {
	double x = m(x1 - x2), y = m(y1 - y2);

	return pow(x * x + y * y, 0.5);
}

// проверяет есть ли вихрь рядом с указанными координатами
char check(double x, double y) {
	int i = (int)(x / rx);
	int j = (int)(y / ry);

	for (int q = 0; q < may[i][j].nv; q++)
		if (rrr(may[i][j].v[q].x,may[i][j].v[q].y,x,y) <= DR) return 1;

	return 0;
}

// меняет два вихря местами
void swapvv(int i, int j, int q1, int q2) {
	char q;
	double u;

	u = may[i][j].v[q1].x; may[i][j].v[q1].x = may[i][j].v[q2].x; may[i][j].v[q2].x = u;
	u = may[i][j].v[q1].y; may[i][j].v[q1].y = may[i][j].v[q2].y; may[i][j].v[q2].y = u;
	q = may[i][j].v[q1].zn; may[i][j].v[q1].zn = may[i][j].v[q2].zn; may[i][j].v[q2].zn = q;
	u = may[i][j].v[q1].t; may[i][j].v[q1].t = may[i][j].v[q2].t; may[i][j].v[q2].t = u;
	u = may[i][j].v[q1].h; may[i][j].v[q1].h = may[i][j].v[q2].h; may[i][j].v[q2].h = u;
	u = may[i][j].v[q1].hi; may[i][j].v[q1].hi = may[i][j].v[q2].hi; may[i][j].v[q2].hi = u;
	u = may[i][j].v[q1].l; may[i][j].v[q1].l = may[i][j].v[q2].l; may[i][j].v[q2].l = u;
	u = may[i][j].v[q1].xi; may[i][j].v[q1].xi = may[i][j].v[q2].xi; may[i][j].v[q2].xi = u;
	u = may[i][j].v[q1].e; may[i][j].v[q1].e = may[i][j].v[q2].e; may[i][j].v[q2].e = u;
}