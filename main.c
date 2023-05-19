#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#include "managment.c"
#include "data.c"
#include "funphys.c"
#include "funenerg.c"
#include "save.c"
#include "process.c"
#include "setup.c"


int main(int argc, char *argv[]) {
	char c, ss[512];

	setup(argc,argv);

#if D < 0
	if (nm > 4) d = nm;
	else d = DM;
#endif
S:	while (!kbhit())
	{
		process();
		if (forend()) goto E;
	}
	switch (getch())
	{
	case 's':	// показать статус
		show();
		goto S;
	case 'q':	// выход
		snprintf(ss, 512, PPC);
		savev(ss);
		char sc[512];
		snprintf(sc, 512, PPG);
		saveg(sc);
		break;
	case 'p':	// пауза
		show();
		printf("ПАУЗА\n");
		c = getch();
		goto S;
	case 'i':	// сохранение
		show();
		printf("сохранить в > ");
		scanf("%s", ss);
		savev(ss);
		goto S;
	default:
		goto S;
	}
E:
	show();

	return 0;
}