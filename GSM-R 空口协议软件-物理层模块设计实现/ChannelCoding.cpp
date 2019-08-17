
//��2,1,5�������viterbi�㷨����
#include <stdio.h>
#include "math.h"
#include <stdlib.h> 
#include<time.h>
#include"ChannelCoding.h"
extern int s[16];//�ĸ��Ĵ�����16��״̬

													  //��������
int encode(
	unsigned int *symbols,    /*�������*/
	unsigned int *data,       /*��������*/
	unsigned int nbytes,      /*nbytes=n/16,nΪʵ���������ֵ���Ŀ */
	unsigned int startstate   /*�����ʼ��״̬*/
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

/*********viterbi�������*****************/
int trandistance(int m, int stat1, int stat2)         /*����m���state1��state2ʱ������ŵĺ�������,
													  ���state1�޷���state2���������ֵΪ10000*/
{
	int c;
	int sym, sym1, sym2;
	sym1 = ((stat2 >> 3) & 1) ^ ((stat1 >> 1) & 1) ^ ((stat1) & 1);//1 0 0 1 1
	sym2 = ((stat2 >> 3) & 1) ^ ((stat1 >> 3) & 1) ^ ((stat1 >> 1) & 1) ^ (stat1 & 1);//1 1 0 1 1
	sym = (sym1 << 1) | sym2;
	if ((((stat1 >> 3) & 1) == ((stat2 >> 2) & 1)) && (((stat1 >> 2) & 1) == ((stat2 >> 1) & 1)) && (((stat1 >> 1) & 1) == (stat2 & 1)))
		c = ((m & 1) ^ (sym & 1)) + (((m >> 1) & 1) ^ ((sym >> 1) & 1));//����ŷʽ����
	else
		c = 10000;
	return(c);
}


int traninput(int a, int b)         /*״̬��a��bʱ��������ķ���*/
{
	int c;
	if ((((a >> 3) & 1) == ((b >> 2) & 1)) && (((a >> 2) & 1) == ((b >> 1) & 1)) && (((a >> 1) & 1) == (b & 1)))
		c = ((b >> 3) & 1);
	else
		c = -1;
	return(c);
}

int tranoutput(int a, int b)         /*״̬��a��bʱ���������ķ���*/
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

void viterbi(int initialstate,                  /*�����������ʼ״̬*/
	int *viterbiinput,              /*������������������*/
	int *viterbioutput)           /*�����������������*/
{
	struct sta                                     /*��������ͼ��ÿһ��Ϊһ���ṹ��,��Ԫ�ذ���*/
	{
		int met;                                  /*ת�Ƶ���״̬�ۼƵĶ���ֵ*/
		int value;                               /*������� */
		struct sta *last;                     /*��ָ��ǰһ��״̬��ָ��*/
	};
	struct  sta state[16][N];
	struct  sta *g;
	int i, j, p, q, t, r, u;//,l;

	for (i = 0; i<16; i++)                           /* ��ʼ��ÿ��״̬�Ķ���ֵ*/
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

	r = state[0][N - 1].met;                  /*�ҳ�n�������ֵ��С��״̬��׼������·��*/
	g = &state[0][N - 1];


	for (u = N; u>0; u--)                       /*��ǰ�ݹ���ҳ������Ȼ·�� */
	{
		viterbioutput[u - 1] = g->value;
		g = g->last;
	}

	/* for(u=0;u<8;u++)
	*(viterbioutput+u)=state[u][2].met; */    /*���г�������ڼ���n�еĶ���ֵ*/
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
printf("��Ϣ���� :\n");
for (i = 0; i < n + 4; i++)
{
	printf("%2d", encodeinput[i]);
	if (i % 20 == 19)
		printf("\n");
}

printf("\n");
printf("����Ϣ���б�������� :\n");
for (i = 0; i<(n + 4) * 2; i++)
{
	printf("%2d", encodeoutput[i]);
	if (i % 20 == 19)
		printf("\n");
}
printf("\n");
decode(encodeoutput, decodeoutput, N);


printf("�޴������ʱ�Ա������viterbi����õ�������:\n");
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

printf("���ĸ���:%d\n", jj);

printf("���������ĸ���\n");
scanf("%d", &m);
printf("������ÿ���������ڵ�λ��\n");
for (i = 0; i<m; i++)
{
scanf("%d", &wrong[m]);
if (encodeoutput[wrong[m]] == 0)
encodeoutput[wrong[m]] = 1;
else
encodeoutput[wrong[m]] = 0;
}
printf("����viterbi�������Ĵ���������(�����������) :\n");
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
printf("��viterbi�����õ�����Ϣ����:\n");
for (i = 0; i<n; i++)
printf("%2d", decodeoutput[i]);
printf("\n");

for (i = 0; i<n; i++)
{
if (encodeinput[i] != decodeoutput[i])
j++;
}
//printf("���ĸ���:%d\n", j);

system("pause");
return 0;
}
*/