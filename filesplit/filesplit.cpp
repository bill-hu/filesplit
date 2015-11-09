// filesplit.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
const int BUF_SIZE=64*1024*1024;
char buf[BUF_SIZE];
const int headLen = 581;
//const int PosfFixLen = 

void getNextWritePos( char * p, int totalfilelen, int skipsize, int* writelen )
{
	static char * splitString ="\r\n}\r\n#endif";
	static int len = strlen(splitString);
	static int len2 = 5;

	p += skipsize;
	for(;skipsize < totalfilelen;p++,skipsize++)
	{
		if(!strncmp(p,splitString,len))
		{
			skipsize += len;
			break;
		}else if(skipsize > 600000)
		{
			if(!strncmp(p,splitString,len2))
			{
				skipsize += len2;
				break;
			}
		}
	}
	*writelen = skipsize;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool bSeek = false;
	if(argc < 2)
	{
		printf("Useage: %s size inputfile",argv[0]);
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
	fseek(fpi,0,SEEK_END);
	int length =ftell(fpi);
	int totalfilelen = length;
	fseek(fpi,0,SEEK_SET);
	while(length > 0)
	{
		int readlen;
		if(length < BUF_SIZE)
			readlen = length;
		else
			readlen = BUF_SIZE;
		int readsize = fread(buf,1,readlen,fpi);
		length -= readsize;
	}
	fclose(fpi);

	length = 0;
	char * p = buf;

	for(int i=1;totalfilelen > 0;i++)
	{
		char filename[1024];
		sprintf(filename,"e:\\OnvifSoap\\soapC%d.cpp",i);
		fpo = fopen(filename,"wb");
		if(NULL == fpo)
		{
			printf("Error open output file :%s",filename);
			fclose(fpi);
			return -3;
		}

		int writelen;
		getNextWritePos(p,totalfilelen,skipsize,&writelen);
		if(i>1)
		{
			fwrite(buf,1,headLen,fpo);
		}
		fwrite(p,1,writelen,fpo);
		p+= writelen;
		totalfilelen -= writelen;
		fclose(fpo);
	}
	printf("\n!OK");
	return 0;
}

