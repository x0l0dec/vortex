// сохраняет на диск в указанный файл информацию о настройках
void savepr(char *ss) {
	char s[508];
	FILE *out;

	snprintf(s, 508, "out/%s", ss);
	out = fopen(s, "w");

	fprintf(out, "X,Y: %.8f,%.8f\n", X, Y);
	fprintf(out, "T: %f\n", T);
	fprintf(out, "HE: %d\n", HE);
	fprintf(out, "HI: %d\n", HI);
//	fprintf(out, "\n", );	// другая информация

	fflush(out);
	fclose(out);
}

// сохраняет примеси на диск в указанный файл
void savep(char *ss) {
	char s[508];
	int i, j, q;
	FILE *out;

	snprintf(s, 508, "out/%s", ss);
	out = fopen(s, "w");

	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			for (q = 0; q < may[i][j].np; q++)
				fprintf(out, "%.8f,%.8f, %.4f\n", may[i][j].p[q].x, may[i][j].p[q].y, may[i][j].p[q].u);

	fflush(out);
	fclose(out);
}

// сохраняет вихри на диск в указанный файл
void savev(char *ss) {
	char s[508];
	int i, j, q;
	FILE *out;

	snprintf(s, 508, "out/%s", ss);
	out = fopen(s, "w");

	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			for (q = 0; q < may[i][j].nv; q++)
				fprintf(out, "%c, %.8f,%.8f\n", may[i][j].v[q].zn, may[i][j].v[q].x, may[i][j].v[q].y);

	fflush(out);
	fclose(out);
}

// сохраняет инфорамацию для гистограммы на диск
void saveg(char *ss) {
	char s[508];
	FILE *out;

	snprintf(s, 508, "out/%s", ss);
	out = fopen(s, "a+t");

	fprintf(out, "%.1d %.16f %d %d %d\n", HE, HE - (nmo - nmx) * EF / (X * Y), nm, nmo, nmx);

	fflush(out);
	fclose(out);
}