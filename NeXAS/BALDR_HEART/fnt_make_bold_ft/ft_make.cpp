#include "ft_make.h"

FT_Make::FT_Make(string font_path, DWORD font_height)
{
	if (!FT_Init(font_path, font_height))
	{
		cout << "ft Init fail!\n";
		exit(0);
	}
}

bool FT_Make::FT_Init(string font_path, DWORD font_height)
{
	FT_Error error;
	ifstream infile;
	infile.open(font_path, ios::in | ios::binary | ios::ate);
	if (!infile.is_open())
	{
		cout << font_path + " open fail!\n";
		exit(0);
	}
	DWORD filesize = (DWORD)infile.tellg();
	infile.seekg(0, ios::beg);
	char* buff = new char[filesize];
	infile.read(buff, filesize);
	infile.close();
	error = FT_Init_FreeType(&library);
	if (error)
	{
		cout << "Init_FreeType Error!\n";
		return false;
	}
	cout << "read ttf...\n";
	error = FT_New_Memory_Face(library, (BYTE *)buff, filesize, 0, &face);
	if (error)
	{
		cout << "New_Memory_Face Error!\n";
		return false;
	}
	error = FT_Set_Char_Size(face, font_height * 64, font_height * 64, 0, 0);
	if (error)
	{
		cout << "Set_Char_Size Error!\n";
		return false;
	}
	error = FT_Set_Pixel_Sizes(face, font_height, font_height);
	if (error)
	{
		cout << "Set_Pixel_Sizes Error!\n";
		return false;
	}
	return true;
}

CharBitmap FT_Make::GetCharBitmap(WCHAR wchar)
{
	FT_GlyphSlot slot = face->glyph;
	FT_Error error;
	FT_Bitmap bmp;
	CharBitmap cbmp;
	error = FT_Load_Char(face, wchar, FT_LOAD_RENDER);
	if (error)
	{
		cout << "Load_Char Error!\n";
		exit(0);
	}
	bmp = slot->bitmap;
	cbmp.bmp_width = bmp.width;
	cbmp.bmp_height = bmp.rows;
	cbmp.bearingX = slot->bitmap_left;
	cbmp.bearingY = slot->bitmap_top;
	cbmp.Advance = slot->advance.x / 64;
	cbmp.bmpBuffer = bmp.buffer;
	return cbmp;
}

FT_Make::~FT_Make()
{
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

BYTE* BuildOutline(DWORD width, DWORD height, BYTE* data, bool do_delete)
{
	DWORD i = 0;
	BYTE *odata, *udata, *ddata;
	height += 2;
	width += 2;
	odata = new BYTE[width*height];//�������ݣ���ʼ״̬��dataһ��
	udata = new BYTE[width*height];//��ʼ״̬Ϊ����ƫ��һ���ص�odata
	ddata = new BYTE[width*height];//��ʼ״̬Ϊ����ƫ��һ���ص�odata
	memset(odata, 0, width*height);
	memset(udata, 0, width*height);
	memset(ddata, 0, width*height);
	//���������ݷŵ����������º�����ƫ��һ����
	for (i = 0; i < height - 2; i++)
		memcpy(odata + width + i*width + 1, data + i*(width - 2), width - 2);
	//����ƫ�ƺ������
	memcpy(udata, odata + width, width*height - width);//����
	memcpy(ddata + width, odata, width*height - width);//����
	//��ʼ����������ݣ��㷨�������Գ����ģ�
	//����ƫ�ƺ�����ݺϲ�
	for (i = 0; i < width*height; i++)
	{
		if (udata[i] != 0xFF)
		{
			if (ddata[i] != 0xFF)
			{
				if (udata[i] + ddata[i] >= 0xFF)
					udata[i] = 0xFF;
				else
					udata[i] += ddata[i];
			}
			else
				udata[i] = 0xFF;
		}
	}
	//����׶��м�����һ�Σ�����Ȥ����ע�ͺ��������ƫ�Ʋ���ֱ�����png�������ɵ�ͼƬ����ô����
	for (i = 0; i < width*height; i++)
	{
		if (odata[i] != 0xFF)
		{
			if (udata[i] != 0xFF)
			{
				if (odata[i] + udata[i] >= 0xFF)
					odata[i] = 0xFF;
				else
					odata[i] += udata[i];
			}
			else
				odata[i] = 0xFF;
		}
	}
	//��ʼ��������ƫ��һ���ز���
	memset(udata, 0, width*height);
	memset(ddata, 0, width*height);
	//���洦�����������������ƫ��һ������
	for (i = 0; i < height; i++)
	{
		memcpy(udata + i * width, odata + i * width + 1, width - 2);
		memcpy(ddata + i * width + 2, odata + i * width + 1, width - 2);
	}
	//���Һϲ�
	for (i = 0; i < width*height; i++)
	{
		if (udata[i] != 0xFF)
		{
			if (ddata[i] != 0xFF)
			{
				if (udata[i] + ddata[i] >= 0xFF)
					udata[i] = 0xFF;
				else
					udata[i] += ddata[i];
			}
			else
				udata[i] = 0xFF;
		}
	}
	//�м�����һ��
	for (i = 0; i < width*height; i++)
	{
		if (odata[i] != 0xFF)
		{
			if (udata[i] != 0xFF)
			{
				if (odata[i] + udata[i] >= 0xFF)
					odata[i] = 0xFF;
				else
					odata[i] += udata[i];
			}
			else
				odata[i] = 0xFF;
		}
	}
	delete[] udata;
	delete[] ddata;
	if (do_delete)
		delete[] data;
	return odata;
}

void WritePng(FILE *pngfile, DWORD width, DWORD height, DWORD p_count, BYTE* data)
{
	png_structp png_ptr;
	png_infop info_ptr;
	DWORD i = 0;
	BYTE *dst, *src, *odata = NULL;
	width += p_count * 2;
	height += p_count * 2;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		printf("PNG��Ϣ����ʧ��!\n");
		exit(0);
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		printf("info��Ϣ����ʧ��!\n");
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		exit(0);
	}
	png_init_io(png_ptr, pngfile);
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	dst = new BYTE[width*height * 4];//��������
	src = new BYTE[width*height];//��ʼ��������
	memset(src, 0, width*height);
	for (i = 0; i < height - p_count * 2; i++)
		memcpy(src + p_count * width/*��ʼ����ƫ������*/ + i*width + p_count/*����ƫ�ƶ�������*/, data + i*(width - p_count * 2), width - p_count * 2);
	for (i = p_count; i > 0; i--)
		if (i == p_count)
			odata = BuildOutline(width - i * 2, height - i * 2, data, false);
		else
			odata = BuildOutline(width - i * 2, height - i * 2, odata, true);
	//���
	for (i = 0; i < width*height; i++)
	{
		dst[i * 4] = src[i];
		dst[i * 4 + 1] = src[i];
		dst[i * 4 + 2] = src[i];
		dst[i * 4 + 3] = odata[i];
	}
	for (i = 0; i < height; i++)
		png_write_row(png_ptr, dst + i*width * 4);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	delete[] src;
	delete[] dst;
	delete[] odata;
}