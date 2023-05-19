// постоянное поле
#define H	400		// H	: эрстед	// внешнее магнитное поле

// todo: как это зависит от направления тока в 3D?
// todo: I
double hi = 0;		// Hi	: эрстед	// поле, связанное с током


// постоянная температура
//#define T	(z / 1.62e-06 * 6 + 1.0) / 11606	// T	: K		// температура в проводнике
#define T	1.0 / 11606	// T	: K		// T	: K		// температура в проводнике



// формулы расчёта энергий
// todo: как считать энергию магнитного поля в 3D?
double m1 = -EF / (4 * M_PI) * DD / 1.6e-12;
#define E1	m1 * h								// E1(H,λ,x,y,z)	: эВ	энергия от магнитного поля если все границы переодичны
#define E1X	m1 * (h * (1 - cosh((x - (X / 2)) / l) / cosh(X / (2 * l))) + hi * (sinh((x - (X / 2)) / l) / sinh(X / (2 * l)) + ((x > X / 2) ? -1 : 1) ))		// E1(H,λ,x,y,z)	: эВ	энергия от магнитного поля если границы не периодичны по X
#define E1Y	m1 * (h * (1 - cosh((y - (Y / 2)) / l) / cosh(Y / (2 * l))) + hi * (sinh((y - (Y / 2)) / l) / sinh(Y / (2 * l)) + ((y > Y / 2) ? -1 : 1) ))		// E1(H,λ,x,y,z)	: эВ	энергия от магнитного поля если границы не периодичны по Y

double m2 = (EF * EF / (16 * M_PI * M_PI)) * DD / 1.6e-12;
#define E2	m2 * (log(l / xi) + 0.52) / (l * l)			// E2(λ,ξ)	: эВ	// собственная энергия вихря

double m3 = (EF * EF / (16 * M_PI * M_PI)) * DD / 1.6e-12;
#define E3	m3 * bessk(0, r / l) / (l * l)				// E3(r,λ) 	: эВ	// энергия взаимодействия типа вихрь-вихрь	: (194)

#define E4	-u * exp(-r / (2 * xi)) / (r / xi + 1)			// E4(r,ξ)	: эВ	// энергия взаимодействия типа примесь-вихрь

double m5 = 2 * (EF * EF / (16 * M_PI * M_PI)) * SD / 1.6e-12;
double m51 = 2 * (EF * EF / (16 * M_PI * M_PI)) * SD / 1.6e-12 * 0.5772;
#define E5	m51 + m5 * log(0.5 * r / l) + m5 * bessk(0, r / l)	// E5(r,l)	: эВ	// энергия взаимодействия вихря с токами от вихрей другого слоя

#define E5Z	0								// 		: эВ	// энергия взаимодействия вихря с токами от вихрей без слоя (работает при ZG == 1)

double m61 = G * SD;
double m62 = 9 * G * SD;
double m63 = 2 * G * SD;
double m64 = 1 / (4 * G * G * SD * SD);
#define E6 	m2 * (1 + log(l / SD)) * ((r <= m63) ? (m64 * r * r * log(m62 / r)) : ((r / m61) - 0.5)) 	// E6(r,l)	: эВ	// энергия взаимодействия вихря с соседними частями вихревой нити

#define E6Z	0								//		: эВ	// энергия взаимодействия вихря с отсутсвующими частями вихревой нити (работает при ZG == 1)


#define TA	1		// τ (коэффициент для подкручивания вероятности в энергиях)


// примеси
#define SNP	100		// количество случайных примесей в каждом слое	: штук
#define CNP	60		// количество столбчатых примесей в системе	: штук
#define RP	0.04		// отвечает за хаотичность примесей		: безразм. (чем коэфициент меньше, тем упорядоченней будут вести себя вихри)
//todo: не применяется нигде
#define CP	1		// 0 - считать взаимодействие всех примесей с вихрем; 1 - считать взаимодействие ТОЛЬКО ближайшей примеси с вихрем


// расстояния
#define DP	0.1		// максимальный радиус перемещения вихря (от λ)				: см
#define DS	10 * K0	// максимальное расстояние на котором допустима аннигиляция		: см
#define DR	K0		// минимальное расстояние на которое подходят вихри			: см
#define BRN	L0		// максимальное расстояние от стенки, на котором рождаются вихри	: см
#define DRN	0.4 * L0	// минимальное расстояние от стенки, на котором рождаются вихри	: см


// шаги
#define D	-1		// длина времени измерений (сколько програмных шагов в одном физическом); при -1 будет пытаться ориентироваться на количество вихрей (ЭТО НЕ БУДЕТ ФИКСИРОВАННАЯ ВЕЛИЧИНА)
#define DM	10		// минимальное колличество програмных шагов в одном физическом (используется если D == -1)
#define N	2000		// количество физических шагов, которые должна работать программа
#define SPC	2000		// количество первых физических шагов в течение которых будет происходить первоначальный расчёт параметров
#define DZP	20		// количество физических шагов, на одну запись в файле


// границы (YG и XG не должны быть одновременно равны 1)
#define XG	0		// определяет характер границ по X (0 -- переодичные границы, 1 -- непереодичные границы)
#define YG	0		// определяет характер границ по Y (0 -- переодичные границы, 1 -- непереодичные границы)
#define ZG	0		// определяет характер границ по Z (0 -- переодичные границы, 1 -- непереодичные границы)


// выход из программы, погрешность
#define P	0.1		// допустимая погрешность (0.05 ~ 0.1) (лучше ставить 0.1)
#define PG	0.5		// часть массива с конца, берётся для разбиения значений в погрешности
