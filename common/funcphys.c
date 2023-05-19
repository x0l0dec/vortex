// модуль
double m(double i) {
	return i > 0 ? i : -i;
}

// todo: добавить сюда вектор
// внешнее магнитное поле H(x,y,z)
double pole(double x, double y, double z) {
	return H;
}

// температура T(x,y,z)
double temp(double x, double y, double z) {
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

// todo: возможно нужно добавить ZG
// энергия взаимодействия с магнитным полем E1(H,λ,x,y,z)
double e1(double h, double l, double x, double y, double z) {
	// XG == 1 && YG == 1 непонятно как считать
#if (XG == 0 && YG == 0)
	return E1;
#elif (XG == 1 && YG == 0)
	return E1Y;
#elif (XG == 0 && YG == 1)
	return E1X;
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

// энергия взаимодействия вихря с токами от вихрей другого слоя E5(r,l), здесь под r имеется ввиду расстояние до соседа в нижней вихревой плоскости
double e5(double r, double l) {
	return E5;
}

// энергия взаимодействия вихря с соседними частями вихревой нити E6(r,l), здесь подд r имеется ввиду расстояние до соседа в нижней вихревой плоскости
double e6(double r, double l) {
	return E6;
}

// определяет минимальное расстояние между двумя точками с учётом стенок и границ системы
double rrr(double x1, double y1, double z1, double x2, double y2, double z2) {
	double x = m(x1 - x2), y = m(y1 - y2), z = m(z1 - z2);

#if XG == 0
	x = (x < X - x) ? x : X - x;
#endif
#if YG == 0
	y = (y < Y - y) ? y : Y - y;
#endif
#if ZG == 0
	z = (z < Z - z) ? z : Z - z;
#endif

	return pow(x * x + y * y + z * z, 0.5);
}

// определяет минимальное расстояние между двумя точками без учёта стенок и границ системы
double rrb(double x1, double y1, double z1, double x2, double y2, double z2) {
	double x = m(x1 - x2), y = m(y1 - y2), z = m(z1 - z2);

	return pow(x * x + y * y + z * z, 0.5);
}

// проверяет, есть ли в vortex *a вихрь рядом с координатами (x,y,z) (vortex *a имеет размер n)
char check(struct vortex **a, int n, double x, double y, double z) {
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < dsn; j++)
			if (rrr(a[i][j].x,a[i][j].y,a[i][j].z,x,y,z) <= DR) return 1;

	return 0;
}