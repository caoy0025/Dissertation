
//（2,1,5）卷积和viterbi算法解卷积
#include <stdio.h>
#include "math.h"
#include <stdlib.h> 
#include<time.h>
#include"ChannelCoding.h"
extern int s[16];//四个寄存器，16个状态

													  //卷积码编码
int encode(
	unsigned int *symbols,    /*编码输出*/
	unsigned int *data,       /*编码输入*/
	unsigned int nbytes,      /*nbytes=n/16,n为实际输入码字的数目 */
	unsigned int startstate   /*定义初始化状态*/
)
{
	int j;
	unsigned int input, a0 = 0, a1 = 0, a2 = 0, a3 = 0;
	for (j = 0; j < nbytes; j++)
	{
		input = *data;
		data++;
		*symbols = input ^ a0 ^ a2 ^ a3;
		symbols++;
		*symbols = input ^a2^a3;
		symbols++;
		a3 = a2;
		a2 = a1;
		a1 = a0;
		a0 = input;
	}
	return 0;
}

/*********viterbi译码相关*****************/
int trandistance(int m, int stat1, int stat2)         /*符号m与从state1到state2时输出符号的汉明距离,
													  如果state1无法到state2则输出度量值为10000*/
{
	int c;
	int sym, sym1, sym2;
	sym1 = ((stat2 >> 3) & 1) ^ ((stat1 >> 1) & 1) ^ ((stat1) & 1);//1 0 0 1 1
	sym2 = ((stat2 >> 3) & 1) ^ ((stat1 >> 3) & 1) ^ ((stat1 >> 1) & 1) ^ (stat1 & 1);//1 1 0 1 1
	sym = (sym1 << 1) | sym2;
	if ((((stat1 >> 3) & 1) == ((stat2 >> 2) & 1)) && (((stat1 >> 2) & 1) == ((stat2 >> 1) & 1)) && (((stat1 >> 1) & 1) == (stat2 & 1)))
		c = ((m & 1) ^ (sym & 1)) + (((m >> 1) & 1) ^ ((sym >> 1) & 1));//计算欧式距离
	else
		c = 10000;
	return(c);
}


int traninput(int a, int b)         /*状态从a到b时输入卷积码的符号*/
{
	int c;
	if ((((a >> 3) & 1) == ((b >> 2) & 1)) && (((a >> 2) & 1) == ((b >> 1) & 1)) && (((a >> 1) & 1) == (b & 1)))
		c = ((b >> 3) & 1);
	else
		c = -1;
	return(c);
}

int tranoutput(int a, int b)         /*状态从a到b时卷积码输出的符号*/
{
	int c, s1, s2;
	s1 = ((b >> 3) & 1) ^ ((a >> 1) & 1) ^ (a & 1);//1 0 0 1 1
	s2 = ((b >> 3) & 1) ^ ((a >> 3) & 1) ^ ((a >> 1) & 1) ^ (a & 1);//1 1 0 1 1
	if ((((a >> 3) & 1) == ((b >> 2) & 1)) && (((a >> 2) & 1) == ((b >> 1) & 1)) && (((a >> 1) & 1) == (b & 1)))
		c = (s1 << 1) | s2;
	else
		c = -1;
	return(c);
}

void viterbi(int initialstate,                  /*定义解码器初始状态*/
	int *viterbiinput,              /*解码器输入码字序列*/
	int *viterbioutput)           /*解码器输出码字序列*/
{
	struct sta                                     /*定义网格图中每一点为一个结构体,其元素包括*/
	{
		int met;                                  /*转移到此状态累计的度量值*/
		int value;                               /*输入符号 */
		struct sta *last;                     /*及指向前一个状态的指针*/
	};
	struct  sta state[16][N];
	struct  sta *g;
	int i, j, p, q, t, r, u;//,l;

	for (i = 0; i<16; i++)                           /* 初始化每个状态的度量值*/
	{
		for (j = 0; j<N; j++)
			state[i][j].met = 0;
	}

	for (int m = 0; m<16; m++)
	{
		state[m][0].met = trandistance(*viterbiinput, initialstate, s[m]);
		state[m][0].value = traninput(initialstate, s[m]);
		state[m][0].last = NULL;
	}

	for (t = 1; t<N; t++)
	{
		for (p = 0; p<8; p++)
			for (q = 0; q<16; q++)
			{
				if ((trandistance(viterbiinput[t], s[p], s[q]) == 10000) || (trandistance(viterbiinput[t], s[p + 8], s[q]) == 10000))
				{
					//	state[q][t].last=NULL;0

					continue;
				}
				else
				{
					int met1 = state[p][t - 1].met + trandistance(viterbiinput[t], s[p], s[q]);
					int met2 = state[p + 8][t - 1].met + trandistance(viterbiinput[t], s[p + 8], s[q]);
					if (met1>met2)
					{
						state[q][t].met = met2;
						state[q][t].value = traninput(s[p + 8], s[q]);
						state[q][t].last = &state[p + 8][t - 1];
					}
					else
					{
						state[q][t].met = met1;
						state[q][t].value = traninput(s[p], s[q]);
						state[q][t].last = &state[p][t - 1];
					}
				}

			}
	}

	r = state[0][N - 1].met;                  /*找出n步后度量值最小的状态，准备回溯路由*/
	g = &state[0][N - 1];


	for (u = N; u>0; u--)                       /*向前递归的找出最大似然路径 */
	{
		viterbioutput[u - 1] = g->value;
		g = g->last;
	}

	/* for(u=0;u<8;u++)
	*(viterbioutput+u)=state[u][2].met; */    /*此行程序可用于检测第n列的度量值*/
}

void decode(unsigned int *input, int *output, int n)
{
	int viterbiinput[500];
	int j;
	for (j = 0; j<n + 4; j++)
	{
		viterbiinput[j] = (input[j * 2] << 1) | input[j * 2 + 1];
		//printf("%3d",viterbiinput[j]);
	}
	viterbi(s[0], viterbiinput, output);
}


/*
int main()
{
int decodeoutput[250], n = N - 4, i, m, j = 0, jj = 0;
unsigned int encodeinput[250], wrong[50] = { 0 }, encodeoutput[500];

randomize();
for (i = 0; i<n; i++)
	encodeinput[i] = rand() % 2;
encodeinput[n] = 0;
encodeinput[n + 1] = 0;
encodeinput[n + 2] = 0;
encodeinput[n + 3] = 0;

encode(encodeoutput, encodeinput, n + 4, s[0]);
printf("信息序列 :\n");
for (i = 0; i < n + 4; i++)
{
	printf("%2d", encodeinput[i]);
	if (i % 20 == 19)
		printf("\n");
}

printf("\n");
printf("对信息序列编码后的输出 :\n");
for (i = 0; i<(n + 4) * 2; i++)
{
	printf("%2d", encodeoutput[i]);
	if (i % 20 == 19)
		printf("\n");
}
printf("\n");
decode(encodeoutput, decodeoutput, N);


printf("无错误出现时对编码输出viterbi译码得到的序列:\n");
for (int ii = 0; ii < n; ii++)
{
	printf("%2d", decodeoutput[ii]);
	if (ii % 20 == 19)
		printf("\n");
}
printf("\n");
for (i = 0; i<n; i++)
{
	if (encodeinput[i] != decodeoutput[i])
		jj++;
}

printf("译错的个数:%d\n", jj);

printf("请输入出错的个数\n");
scanf("%d", &m);
printf("请输入每个出错所在的位数\n");
for (i = 0; i<m; i++)
{
scanf("%d", &wrong[m]);
if (encodeoutput[wrong[m]] == 0)
encodeoutput[wrong[m]] = 1;
else
encodeoutput[wrong[m]] = 0;
}
printf("输入viterbi解码器的待解码序列(即有误的序列) :\n");
for (i = 0; i<(n + 4) * 2; i++)
{
printf("%2d", encodeoutput[i]);
if (i % 20 == 19)
printf("\n");
}
printf("\n");
*/

/*
decode(encodeoutput, decodeoutput, n + 4);
printf("经viterbi解码后得到的信息序列:\n");
for (i = 0; i<n; i++)
printf("%2d", decodeoutput[i]);
printf("\n");

for (i = 0; i<n; i++)
{
if (encodeinput[i] != decodeoutput[i])
j++;
}
//printf("译错的个数:%d\n", j);

system("pause");
return 0;
}
*/