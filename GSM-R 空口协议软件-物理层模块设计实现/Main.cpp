
#include"ChannelCoding.h"
#include"InterleavingAndMapping.h"

int s[16] = { 0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15 };
unsigned int opt[8][57], opt_BCCH[4][148];

/*
int main()
{
	int decodeoutput[250], n = N - 4, i, m, j = 0, jj = 0;
	unsigned int encodeinput[250], wrong[50] = { 0 }, encodeoutput[456];

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
		if (i % 20 == 19 && i != 200)
			printf("\n");
		
	}

	printf("\n");
	printf("����Ϣ���б�������� :\n ");
	for (i = 0; i<(n + 4) * 2; i++)
	{
		printf("%2d", encodeoutput[i]);
		if (i % 40 == 39)
			printf("\n");
		if (i % 2 == 1)
			printf(" ");
		//

	}
	printf("\n");
	printf("����Ϣ���б�������� :\n ");
	for (i = 0; i<(n + 4) * 2; i++)
	{
		printf("%2d", encodeoutput[i]);
		if ( i % 8 == 7)
			printf("\n");
		if (i % 2 == 1)
			printf(" ");
		  //if ( i % 40 == 39)
		
	}
	printf("\n");
	decode(encodeoutput, decodeoutput, N);

	//��֯
	InsideB_inter(encodeoutput);
	mapping_BCCH(opt);
	printf("BCCH��佻֯��\n");
	for (int p = 0; p < 4; p++)
	{
		for (int pp = 0; pp < 148; pp++)
		{
			printf("%2d", opt_BCCH[p][pp]);
		}
		printf("\n");
	}

	printf("�⽻֯������ :\n ");
	for (i = 0; i<(n + 4) * 2; i++)
	{
		printf("%2d", encodeoutput[i]);
		if (i % 40 == 39)
			printf("\n");
		
		//if ( i % 40 == 39)

	}
	printf("\n");
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
	system("color f0");
	system("pause");
	return 0;
}
*/