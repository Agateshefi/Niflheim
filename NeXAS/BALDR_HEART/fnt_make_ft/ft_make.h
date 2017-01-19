#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <direct.h>
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

#pragma pack(1)

typedef struct _CharBitmap
{
	DWORD bmp_width;  //λͼ��
	DWORD bmp_height; //λͼ��
	DWORD bearingX;   //����ˮƽ�ı����У����Ǵӵ�ǰ���λ�õ�����ͼ������ߵı߽��ˮƽ����
	DWORD bearingY;   //����ˮƽ�ı����У����Ǵӵ�ǰ���λ�ã�λ�ڻ��ߣ�������ͼ�����ϱߵı߽��ˮƽ���롣 
	DWORD Advance;    //����ˮƽ�ı����У���������Ϊ�ַ�����һ���ֱ�����ʱ�����������ӱ�λ�õ�ˮƽ����
	BYTE* bmpBuffer; //��������
}CharBitmap;

#pragma pack()

class FT_Make
{
public:
	FT_Make(string font_path, DWORD font_height);
	CharBitmap GetCharBitmap(WCHAR *wchar);
	~FT_Make();
private:
	FT_Library library;
	FT_Face face;
	FT_Error error;
	bool FT_Init(string font_path, DWORD font_height);
};