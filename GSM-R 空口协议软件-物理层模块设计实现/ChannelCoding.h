#include <stdio.h>
#include "math.h"
#include <stdlib.h> 
#include<time.h>
#define N 228	//�ܱ��볤��
#define randomize() srand((unsigned)time(NULL))


int encode(
	unsigned int *symbols,    /*�������*/
	unsigned int *data,       /*��������*/
	unsigned int nbytes,      /*nbytes=n/16,nΪʵ���������ֵ���Ŀ */
	unsigned int startstate   /*�����ʼ��״̬*/
);
int trandistance(int m, int stat1, int stat2);
int traninput(int a, int b);
int tranoutput(int a, int b);
void viterbi(int initialstate,                  /*�����������ʼ״̬*/
	int *viterbiinput,              /*������������������*/
	int *viterbioutput           /*�����������������*/
);
void decode(unsigned int *input, int *output, int n);
