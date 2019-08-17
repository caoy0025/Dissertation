#include"stdio.h"
#include"math.h"
#include <stdlib.h>
#include <time.h>
#include"interleavingAndMapping.h"

extern short input[B];
extern unsigned int opt[8][57],opt_BCCH[4][148];
unsigned int b[8][B / 8], output_i[8][B / 8], output_BCCH[4][148];
/*
short FB[148] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
*/

void mapping_BCCH(unsigned int output_i[8][57])
{
	int k;
	for (int i = 0; i < 4; i++)
	{
		output_BCCH[i][0] = 0;
		output_BCCH[i][1] = 0;
		output_BCCH[i][2] = 0;
		for (k = 3; k < 60; k++)
		{ 
			output_BCCH[i][k] = output_i[i][k - 3];
		}
		output_BCCH[i][60] = 0;
		output_BCCH[i][61] = 0;
		output_BCCH[i][62] = 0;
		output_BCCH[i][63] = 1;
		output_BCCH[i][64] = 0;
		output_BCCH[i][65] = 0;
		output_BCCH[i][66] = 1;
		output_BCCH[i][67] = 0;
		output_BCCH[i][68] = 1;
		output_BCCH[i][69] = 1;
		output_BCCH[i][70] = 1;
		output_BCCH[i][71] = 0;
		output_BCCH[i][72] = 0;
		output_BCCH[i][73] = 0;
		output_BCCH[i][74] = 0;
		output_BCCH[i][75] = 1;
		output_BCCH[i][76] = 0;
		output_BCCH[i][77] = 0;
		output_BCCH[i][78] = 0;
		output_BCCH[i][79] = 1;
		output_BCCH[i][80] = 0;
		output_BCCH[i][81] = 0;
		output_BCCH[i][82] = 1;
		output_BCCH[i][83] = 0;
		output_BCCH[i][84] = 1;
		output_BCCH[i][85] = 1;
		output_BCCH[i][86] = 1;
		output_BCCH[i][87] = 0;
		for (k = 88; k < 145; k++)
		{
			output_BCCH[i][k] = output_i[i + 4][k - 88];
		}
		output_BCCH[i][145] = 0;
		output_BCCH[i][146] = 0;
		output_BCCH[i][147] = 0;
		for (int ii = 0; ii < 4; ii++)
			for (int iii = 0; iii < 148; iii++)
				opt_BCCH[ii][iii] = output_BCCH[ii][iii];
	}
}


int InsideB_inter(unsigned int ip[B]) { 
	unsigned int c[B];
	unsigned int op[8][57];
	int i = 0;
	for (i = 0; i < B; i++)
	{
		c[i] = ip[i];
		
	}
	printf("\n\n快内交织后：\n");
	i = 0;

	for (int j = 0; j < 8; j++)
	{
		for (int jj = 0; jj < B / 8; jj++)
		{
			op[j][jj] = c[i];
			opt[j][jj] = op[j][jj];
			i = i + 8;
			printf("%2d", op[j][jj]);
			if (jj == B / 8 - 1)
				i = j + 1;
		}
		printf("\n");
	}
	printf("\n");
	return 0; 
}

int deInsideB_inter(unsigned int ip[B]) {
	unsigned int c[B];
	unsigned int op[8][57];
	int i = 0;
	for (i = 0; i < B; i++)
	{
		c[i] = ip[i];

	}
	printf("\n解块间交织后：\n");
	i = 0;

	for (int j = 0; j < 8; j++)
	{
		for (int jj = 0; jj < B / 8; jj++)
		{
			op[j][jj] = c[i];
			opt[j][jj] = op[j][jj];
			i = i + 8;
			printf("%2d", op[j][jj]);
			if (jj == B / 8 - 1)
				i = j + 1;
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

/*
int main()
{
	srand((unsigned)time(NULL));
	int i = 0;
	unsigned int a[B];

	//赋值
	printf("随机序列为：\n");
	for (i = 0; i < B; i++)
	{
		a[i] = rand() % 2;
		printf("%2d", a[i]);
		if (i % 8 == 7)
			printf("\n");
	}
	printf("\n\n快内交织后：\n");
	i = 0;

	for (int j = 0; j < 8; j++)
	{
		for (int jj = 0; jj < B / 8; jj++)
		{
			b[j][jj] = a[i];
			i = i + 8;
			printf("%2d", b[j][jj]);
			if (jj == B / 8 - 1)
				i = j + 1;
		}
		printf("\n");
	}
	printf("\n");
	mapping_BCCH(b);
	for (int p = 0; p < 4; p++)
	{
		for (int pp = 0; pp < 148; pp++)
		{
			printf("%2d", output_BCCH[p][pp]);
		}
		printf("\n");
	}
	system("pause");
	return 0;
}
*/