/*
���ڽ��BH��pac�ļ�
made by Darkness-TX
2016.12.01
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <Windows.h>
#include <zlib.h>
#include <locale.h>
#include "BH_huffman_comp.h"

typedef unsigned char  unit8;
typedef unsigned short unit16;
typedef unsigned int   unit32;

unit32 FileNum = 0;//���ļ�������ʼ����Ϊ0

struct header
{
	unit8 magic[4];//PAC\0
	unit32 num;
	unit32 mode;//BH����4
}pac_header;

struct index
{
	unit8 name[64];//�ļ���
	unit32 Offset;//�ļ�ƫ��
	unit32 FileSize;//��ѹ��С
	unit32 ComSize;//δ��ѹ��С
}Index[7000];

void ReadIndex(char *fname)
{
	FILE *src, *dst;
	unit32 ComSize = 0, UncomSize = 0, i = 0;
	unit8 *cdata, *udata, dstname[200];
	src = fopen(fname, "rb");
	sprintf(dstname, "%s_INDEX", fname);
	fread(pac_header.magic, 4, 1, src);
	if (strncmp(pac_header.magic, "PAC\0", 4) != 0)
	{
		printf("�ļ�ͷ����PAC\0!\n");
		fclose(src);
		system("pause");
		exit(0);
	}
	else
	{
		fread(&pac_header.num, 4, 1, src);
		fread(&pac_header.mode, 4, 1, src);
		printf("%s filenum:%d mode:%d\n\n", fname, pac_header.num, pac_header.mode);
		if (pac_header.mode != 4)
		{
			printf("����ģʽ4��\n");
			system("pause");
			exit(0);
		}
		else
		{
			fseek(src, -4, SEEK_END);
			fread(&ComSize, 4, 1, src);
			fseek(src, -4 - ComSize, SEEK_END);
			cdata = malloc(ComSize);
			fread(cdata, ComSize, 1, src);
			for (i = 0; i < ComSize; i++)
				cdata[i] = ~cdata[i];
			UncomSize = 76 * pac_header.num;
			udata = malloc(UncomSize);
			huffman_compress(udata, &UncomSize, cdata, ComSize);
			dst = fopen(dstname, "wb");
			fwrite(udata, UncomSize, 1, dst);
			free(cdata);
			fclose(dst);
			fclose(src);
			for (i = 0; i < pac_header.num; i++)
				memcpy(&Index[i], &udata[i * 76], 76);
			free(udata);
		}
	}
}

void packFile(char *fname)
{
	FILE *src, *dst;
	unit8 *cdata, *udata;
	unit32 filesize, compsize;
	src = fopen(fname, "rb");
	fseek(src, 0, SEEK_END);
	filesize = ftell(src);
	compsize = filesize * 4;
	udata = malloc(filesize);
	cdata = malloc(compsize);
	huffman_compress(cdata, &compsize, udata, filesize);
	printf("%X %X", compsize, filesize);
	//for (unit32 i = 0; i < compsize; i++)
	//	cdata[i] = ~cdata[i];
	dst = fopen("1.bin", "wb");
	fwrite(cdata, 1, compsize, dst);
	fclose(dst);
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "chs");
	printf("project��Niflheim-BALDR HEART\n���ڷ��BH��pac�ļ���\n��pac�ļ��ϵ������ϡ�\nby Darkness-TX 2016.12.01\n\n");
	//ReadIndex(argv[1]);
	packFile(argv[1]);
	printf("����ɣ����ļ���%d\n", FileNum);
	system("pause");
	return 0;
}