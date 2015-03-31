// filesplit.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
const int BUF_SIZE=64*1024*1024;
char buf[BUF_SIZE];

int _tmain(int argc, _TCHAR* argv[])
{
	bool bSeek = false;
	if(argc < 4)
	{
		printf("\nSkip the size from begining:\n");
		printf("Useage: filesplit size infile outfile");

		printf("Keep the size from begining:\n");
		printf("Useage: filesplit size infile outfile 1\n");
		return -1;
	}
	

	FILE * fpi;
	FILE * fpo;

	int skipsize = atoi(argv[1]);
	fpi = fopen(argv[2],"rb");
	if(NULL == fpi)
	{
		printf("Error open input file :%s",argv[2]);
		return -2;
	}

	fpo = fopen(argv[3],"wb");
	if(NULL == fpo)
	{
		printf("Error open output file :%s",argv[3]);
		fclose(fpi);
		return -3;
	}

	fseek(fpi,0,SEEK_END);
	int length =ftell(fpi);
	if(argc == 5)
	{
		length = skipsize;
		fseek(fpi,0,SEEK_SET);
	}
	else
	{
		fseek(fpi,skipsize,SEEK_SET);
		length -= skipsize;
	}
	printf("\n!Begin");
	while(length > 0)
	{
		int readlen;
		if(length < BUF_SIZE)
			readlen = length;
		else
			readlen = BUF_SIZE;
		int readsize = fread(buf,1,readlen,fpi);
		if(readsize>0)
		{
			fwrite(buf,1,readsize,fpo);
		}
		length -= readsize;
	}
	fclose(fpi);
	fclose(fpo);
	printf("\n!OK");
	return 0;
}

