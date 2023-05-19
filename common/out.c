// сохраняет на диск информацию об основных настройках расчёта
// X,Y,Z	// размер
// dsn		// количество слоёв
void savepref(char *ss) {
	char s[512];
	FILE *out;

	snprintf(s, 512, "out/%s", ss);
	out = fopen(s, "w");

	fprintf(out, "%.8f,%.8f,%.8f\n", X, Y, Z);
	fprintf(out, "%d\n", dsn);

	fflush(out);
	fclose(out);
}

// сохраняет примеси на диск в файл out/*ss в формате:
// x1,y1,z1,u1
// x2,y2,z2,u2
// ...
void saveprim(char *ss) {
	char s[512];
	int i, j;
	FILE *out;

	snprintf(s, 512, "out/%s", ss);
	out = fopen(s, "w");

	for (j = 0; j < dsn; j++)
		for (i = 0; i < SNP + CNP; i++)
			fprintf(out, "%.8f,%.8f,%.8f,%.2f\n", pr[j][i].x, pr[j][i].y, pr[j][i].z, pr[j][i].u);

	fflush(out);
	fclose(out);
}

// сохраняет расположение вихрей на диск в файл out/*ss в формате:
// = (шаг)
// zn1,x1,y1,z1;zn2,x2,y2,z2, ... x количество слоёв
// ... x количество линий
// = (новый шаг)
// ...		// X количество шагов
// ...
void savev(char *ss) {
	char s[512];
	int i, j, u;
	FILE *out;

	snprintf(s, 512, "out/%s", ss);
	out = fopen(s, "w");

	for (j = 0; j < nm; j++)
	{
		fprintf(out, "=\n");
		for (i = 0; i < nn[j]; i++)
		{
			for (u = 0; u < dsn; u++)
				fprintf(out, "%c,%.8f,%.8f,%.8f;", v[j][i][u].zn, v[j][i][u].x, v[j][i][u].y, v[j][i][u].z);
			fprintf(out, "\n");
		}
	}

	fflush(out);
	fclose(out);
}


// сохраняет всю информацию на диск в файл с названием *ss
/*
void save(char *ss) {
	int i, j;
	FILE *out;

	out = fopen(ss, "w");
	fprintf(out, "δE = %.4f эВ\n* : %.16f\n", m(pog * e[nm]), e[0]);
	for (i = 0; i < nn[0]; i++)
		fprintf(out, "%c(%.8f;%.8f) ", v[0][i].zn, v[0][i].x, v[0][i].y);
	fprintf(out, "\n");
	for (i = 0; i < nm; i++)
	{
		fprintf(out, "%lld : %.16f\n", (unsigned long long)i, e[i]);
		for (j = 0; j < nn[i]; j++)
			fprintf(out, "%c(%.8f;%.8f) ", v[i][j].zn, v[i][j].x, v[i][j].y);
		fprintf(out, "\n");
	}
	fflush(out);
	fclose(out);
}
*/