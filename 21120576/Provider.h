#pragma once
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

struct Header
{
	char signature[2];
	int fileSize; // kich thuoc file
	int reserved; // phan danh rieng
	int dataOffset; // dia chi phan bat dau luu du lieu diem anh
};

struct Pixel8bits {
	uint8_t B;
};

struct Pixel24bits {
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

struct Pixel32bits {
	uint8_t A;
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

// Bang mau anh 8 bit
struct ColorTable8Bits {
	uint8_t B;
	uint8_t G;
	uint8_t R;
	uint8_t Reserved;
};

struct DIB
{
	int dibSize; // kich thuoc DIB = 40 byte
	int width; // so pixel theo chieu rong
	int height; // so pixel theo chieu dai
	short planes; // so lop mau 
	short bpp; // so bit trong 1 pixel

	int compression; // cach nen anh
	int imageSize; // kich thuoc phan pixel data
	int xpPM; // do phan giai theo phuong ngang 
	int ypPM; // do phan giai theo phuong thang dung

	int colorsUsed; // so mau su dung
	int importantColor; // so mau quan trong
	ColorTable8Bits colorMap[256]; // Bang mau cua anh
};

struct BMP
{
	Header header;
	DIB dib;
	ColorTable8Bits colorTable[256];
	char* pDIBReserved; // con tro toi vung nho luu phan du cua DIB
	char* pImageData; // con tro toi vung nho luu pixel data
};

// Khai bao ham chuyen du lieu diem anh sang mang pixel 8 bit
Pixel8bits* convertDataToPixArrayFor8(char*& data, DIB& dib);

// Khai bao ham chuyen mang pixel sang du lieu diem anh 8 bit
char* convertPixArrayToDataFor8(Pixel8bits*& pixels, DIB& dib);

// Khai bao ham chuyen du lieu diem anh sang mang pixel 24 bit
Pixel24bits* convertDataToPixArrayFor24(char*& data, DIB& dib);

// Khai bao ham chuyen mang pixel sang du lieu diem anh 24 bit
char* convertPixArrayToDataFor24(Pixel24bits*& pixels, DIB& dib);

// Khai bao ham chuyen du lieu diem anh sang mang pixel 32 bit
Pixel32bits* convertDataToPixArrayFor32(char*& data, DIB& dib);

// Khai bao ham chuyen mang pixel sang du lieu diem anh 32 bit
char* convertPixArrayToDataFor32(Pixel32bits*& pixels, DIB& dib);

// Khai bao ham doc file bmp
int readBMPFile(char* filename, BMP& bmp);

// Khai bao ham ghi file bmp
int writeBMPFile(char* filename, BMP bmp);

// Khai bao ham chuyen anh 24 bit sang 8 bit
int convert24bitsTo8Bits(BMP srcBmp, BMP& dstBmp);

// Khai bao ham chuyen anh 32 bit sang 8 bit
int convert32bitsTo8Bits(BMP srcBmp, BMP& dstBmp);

// Khai bao ham thu nho 8 bit
int zoomBMP8bits(BMP srcBmp, BMP& dstBmp, int s);

// Khai bao ham thu nho 24 bit
int zoomBMP24bits(BMP srcBmp, BMP& dstBmp, int s);

// Khai bao ham thu nho 32 bit
int zoomBMP32bits(BMP srcBmp, BMP& dstBmp, int s);

// Khai bao ham giai phong bo nho 8 bit
void releasePixArrayFor8(Pixel8bits* pixelarray);

// Khai bao ham giai phong bo nho 24 bit
void releasePixArrayFor24(Pixel24bits* pixelarray);

// Khai bao ham giai phong bo nho 32 bit
void releasePixArrayFor32(Pixel32bits* pixelarray);

// Khai bao ham giai phong bo nho cac con tro cap phat trong struct BMP
void releaseBMP(BMP& bmp);
