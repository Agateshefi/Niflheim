#include "pac.h"


int main(int agrc, char* agrv[])
{
	cout << "project��Niflheim-SOFTPAL_ADV_SYSTEM\n���ڽ�������pac��\nby Destiny�λ�� 2016.11.07\n";
	if (agrc != 3)
		cout << "\nUsage:\n\texport:\tfuckpac -e pacfile\n\tpack:\tfuckpac -p oldpacfile\n";
	else
	{
		if (strcmp(agrv[1],"-e")==0)
		{
			PAC pac(agrv[2]);
			cout << "load index...\n";
			for (DWORD i = 0; i < pac.filenum; i++)
				printf("name:%s offset:0x%X size:0x%X\n", pac.findexs[i].filename, pac.findexs[i].offset, pac.findexs[i].size);
			cout << "\nexport...\n";
			if (pac.pacexport())
				printf("all %d files export\n", pac.filenum);
			else
				cout << "��ȡʧ�ܣ�\n";
		}
		else if (strcmp(agrv[1],"-p")==0)
		{
			PAC pac(agrv[2]);
			cout << "load old index...\n";
			for (DWORD i = 0; i < pac.filenum; i++)
				printf("name:%s offset:0x%X size:0x%X\n", pac.findexs[i].filename, pac.findexs[i].offset, pac.findexs[i].size);
			cout << "\npack...\n";
			if (pac.pacpack())
				printf("all %d files pack\n", pac.filenum);
			else
				cout << "���ʧ�ܣ�\n";
		}
		else
			cout << "δ֪������\n";
	}
	return 0;
}