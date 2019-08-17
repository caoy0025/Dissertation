#include <stdio.h>  
#pragma warning(disable:4996)

#include"ChannelCoding.h"
#include"InterleavingAndMapping.h"

/*
int main()
{

	//下面是读数据，将读到的数据存到数组a[10]中，并且打印到控制台上  
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
	getchar();//等待  

	return 1;
}
*/