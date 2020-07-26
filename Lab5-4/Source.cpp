#include <conio.h>
#include <omp.h>
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>

#pragma comment (lib, "Winmm.lib")

//Nстуд. билета = 530732
//Размер блока = 5307320
//Число блоков = 189
//Динамический TLS

const unsigned int N = 1000000000;
const unsigned int BlockSize = 5307320;
const unsigned int BlockNumber = 189;
const unsigned int TLSCellCount = 1088;
const unsigned int ThreadCount = 6;

//Общая сумма
double Sum = 0;

void main(void)
{
	setlocale(LC_ALL, "Russian");

	//Устанавливем число потоков
	omp_set_num_threads(ThreadCount);

	//Начальное значение счётчика времени
	unsigned long Time = timeGetTime();

	//Вычисленная блочная сумма
	double BlockSum = 0;

	#pragma omp parallel private(BlockSum) \
	shared(Sum)
	{
		BlockSum = 0;

		#pragma omp for schedule (static, BlockSize)
		for (int i = 0; i < N; i++)
		{
			double x = (i + 0.5) / (double)N;
			BlockSum += 4. / (double)(1 + x*x);
		};

		#pragma omp critical(Sum)
		{
			Sum += BlockSum;
		};
	};

#pragma omp barrier

	//Вычисляем общее время обработки
	Time = timeGetTime() - Time;
	std::cout << "Время обработки: " << Time << " мс." << std::endl;

	//Полученное значение pi
	double pi = Sum / (double)N;
	std::cout << "pi = " << (unsigned int)pi << '.';

	for (unsigned long i = 0; i < 52; i++)
	{
		pi -= (unsigned int)pi;
		pi *= 10;
		std::cout << (unsigned int)pi;
	};

	getch();
}