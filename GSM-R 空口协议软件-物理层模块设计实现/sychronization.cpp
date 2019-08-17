//同步
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#pragma warning(disable:4996)

#define NN 10
#define AN 17066
void xcorr(float *r, short *x, short *y, int N);



int main()
{
	short x[AN];

	FILE *fpRead = fopen("data.txt", "r");
	if (fpRead == NULL)
	{
		return 0;
	}
	for (int i = 0; i<AN; i++)
	{
		fscanf(fpRead, "%d ", &x[i]);
	}
	getchar();//等待  
 short y[64] = { 1, -1, 1, 1, 1, -1, -1, 1, -1, 1, 1, -1, -1, -1, 1, -1, -1, -1, -1, -1, -1, 1, -1,
						-1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, 1, -1, -1, -1,
						 1, -1, 1, -1, 1, 1, 1, -1, 1, 1, -1, -1, -1, -1, 1, 1, -1, 1, 1
};
 short z[10];
 short *p = x;
float r[2*NN-1] = { 0 };
float rmax = 0;
int rn=0;
int delay=0;
srand((unsigned)time(NULL));


for (int i = 0; i < AN; i++)
{
	x[i] = rand() % 2;
	if (x[i] == 0)
		x[i] = x[i]-1;
}

printf("训练序列：\n");
for (int j = 0; j < 64; j++)
{
	if (y[j] == -1)
		y[j] = 0;
	printf("%5d",y[j]);
	if (j % 20 == 19)
		printf("\n");

}

for (int ii = 0; ii < AN - NN; ii = ii++)
{
for (int j = 0; j < NN; j++) {
z[j] = *(p + ii + j);
}
xcorr(r, z, y, NN);

//printf("%d %f\n", delay, r[delay + NN - 1]);
if (r[delay + NN - 1] > rmax)
{
rmax = r[delay + NN - 1];
rn = ii;
}
}

//在命令框中显示结果

printf("\n输入序列：");
printf("\n");

for (int i = 0; i < AN; i++)
{	
	printf("%5d", x[i]);
	if (i % 20 == 19)
		printf("\n");
}

printf("\n\n");
printf("最大相关度rmax=");
printf("%f\n", rmax);
//printf("同步位置rn= ");
//printf("%2d\n", rn);

system("color f0");
system("pause");

return 0;
}





void xcorr(float *r, short *x, short *y, int N)
{
	float sxy;
	int    delay, i, j;

	for (delay = -N + 1; delay < N; delay++)
	{
		//Calculate the numerator
		sxy = 0;
		for (i = 0; i<N; i++)
		{
			j = i + delay;
			if ((j < 0) || (j >= N))  //The series are no wrapped,so the value is ignored
				continue;
			else
				sxy += (x[i] * y[j]);
		}

		//Calculate the correlation series at "delay"
		r[delay + N - 1] = sxy;
	}
}
