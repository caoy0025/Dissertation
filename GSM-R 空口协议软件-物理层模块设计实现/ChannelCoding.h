#include <stdio.h>
#include "math.h"
#include <stdlib.h> 
#include<time.h>
#define N 228	//总编码长度
#define randomize() srand((unsigned)time(NULL))


int encode(
	unsigned int *symbols,    /*编码输出*/
	unsigned int *data,       /*编码输入*/
	unsigned int nbytes,      /*nbytes=n/16,n为实际输入码字的数目 */
	unsigned int startstate   /*定义初始化状态*/
);
int trandistance(int m, int stat1, int stat2);
int traninput(int a, int b);
int tranoutput(int a, int b);
void viterbi(int initialstate,                  /*定义解码器初始状态*/
	int *viterbiinput,              /*解码器输入码字序列*/
	int *viterbioutput           /*解码器输出码字序列*/
);
void decode(unsigned int *input, int *output, int n);
