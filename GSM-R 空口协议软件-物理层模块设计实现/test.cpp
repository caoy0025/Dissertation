#include <stdio.h>  
#pragma warning(disable:4996)

#include"ChannelCoding.h"
#include"InterleavingAndMapping.h"

/*
int main()
{

	//�����Ƕ����ݣ������������ݴ浽����a[10]�У����Ҵ�ӡ������̨��  
	int a[400] = { 0 };
	FILE *fpRead = fopen("data.txt", "r");
	if (fpRead == NULL)
	{
		return 0;
	}
	for (int i = 0; i<400; i++)
	{
		fscanf(fpRead, "%d ", &a[i]);
		printf("%d ", a[i]);
	}
	getchar();//�ȴ�  

	return 1;
}
*/