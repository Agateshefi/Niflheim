#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <direct.h>
#include <png.h>

using namespace std;

#pragma pack(1)

typedef struct font_s
{
	//��ʵ����GetGlyphOutline��GGO_GRAY8_BITMAP
	DWORD offset;		//��ģ���ڵ�ַ
	DWORD gmBlackBoxX;	//��ģ��
	DWORD gmBlackBoxY;	//��ģ��
	DWORD x;			//gmptGlyphOrigin.x
	DWORD y;			//gmptGlyphOrigin.y����������
	DWORD gmCellIncX;	//�����
	DWORD size;			//��ģռ���ֽ�
	DWORD width;		//��size / gmBlackBoxY�õ���ת��ͼƬ��Ŀ�
}font_t;

struct char_s
{
	//��֪��ɶ���µ�
	DWORD height;
	DWORD width;
	DWORD font_type;
};

#pragma pack()

class FONT
{
public:
	FONT(string fontname);
	bool font2png(DWORD i);
	BYTE* makefont(WCHAR chText, DWORD i);
	~FONT();
	FILE *fp;
	string dirname;
	DWORD count = 0x6D92;
	DWORD tbl_start = 2199;
	vector<font_t> findexs;
private:
	bool ReadIndex(string fontname);
};