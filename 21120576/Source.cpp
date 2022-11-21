#include "Provider.h"


//Ham doc file anh BMP
int readBMPFile(char* filename, BMP& bmp)
{
	// mo file can doc
	fstream fin;
	fin.open(filename, ios::in | ios::binary);
	// kiem tra file co mo duoc hay khong
	if (!fin.is_open())
	{
		return 0;
	}
	// dua con tro vao dau file
	fin.seekg(0, ios_base::beg);
	// doc file header 14 bytes
	fin.read((char*)&bmp.header, 14);
	// doc file dib 40 bytes
	fin.read((char*)&bmp.dib, 40);

	// kiem tra xem kich thuoc dib co thua khong
	if (bmp.dib.dibSize > 40)
	{
		// cap phat vung nho dib thua 
		bmp.pDIBReserved = new char[bmp.dib.dibSize - 40];
		// doc phan thua dib
		fin.read(bmp.pDIBReserved, bmp.dib.dibSize - 40);
	}
	if (bmp.dib.bpp == 8) {
		fin.read((char*)&bmp.colorTable, sizeof(ColorTable8Bits) * 256);
	}
	// cap phat vung nho cho phan pixel data
	bmp.pImageData = new char[bmp.dib.imageSize];

	// doc phan du lieu diem anh
	fin.read(bmp.pImageData, bmp.dib.imageSize);
	fin.close();
	return 1;
}

//Ham ghi anh BMP
int writeBMPFile(char* filename, BMP bmp)
{
	// mo file can ghi
	ofstream fout(filename, ios::binary | ios::out);
	// kiem tra file co mo duoc hay khong
	if (!fout)
	{
		return 0;
	}
	// ghi header
	fout.write((char*)&bmp.header, 14);
	// ghi dib
	fout.write((char*)&bmp.dib, 40);
	// ghi phan du dib
	if (bmp.dib.dibSize > 40)
		fout.write(bmp.pDIBReserved, bmp.dib.dibSize - 40);
	if (bmp.dib.bpp == 8) {
		fout.write((char*)&bmp.colorTable, sizeof(ColorTable8Bits) * 256);
	}
	// ghi pixel data
	fout.write(bmp.pImageData, bmp.dib.imageSize);
	fout.close();
	return 1;
}

//Ham chuyen du lieu diem anh sang mang diem anh 8 bit
Pixel8bits* convertDataToPixArrayFor8(char*& data, DIB& dib) {
	// Bien luu kich thuoc anh
	int size = dib.width * dib.height;

	// Mang luu diem anh
	Pixel8bits* pixels = new Pixel8bits[size];

	// Tinh phan padding 
	int padding = (4 - (dib.width * dib.bpp / 8) % 4) % 4;

	// Bien temp lam con tro tam tro vao data
	char* temp = data;

	//Vong lap gan gia tri trong du lieu diem anh vao mang diem anh
	for (int i = 0; i < dib.height; i++) {
		for (int j = 0; j < dib.width; j++) {
			pixels[i * dib.width + j].B = *(temp++);
		}
		// Cong them padding de temp di qua phan padding
		temp += padding;
	}
	return pixels;
}

// Ham chuyen mang diem anh sang du lieu diem anh 8 bit
char* convertPixArrayToDataFor8(Pixel8bits*& pixels, DIB& dib) {
	// Tinh padding cho anh
	int padding = (4 - (dib.width * dib.bpp / 8) % 4) % 4;

	// Tinh kich thuoc anh
	int size = dib.height * dib.width * (dib.bpp / 8) + (padding)*dib.height;
	// Cap phat mang kich thuoc anh vao bien data
	char* data = new char[size];

	// Bien temp tro vao data
	char* temp = data;

	// Vong lap gan lan luot cac gia tri mau trong mang diem anh cho temp
	for (int i = 0; i < dib.height; i++) {
		for (int j = 0; j < dib.width; j++) {
			*(temp++) = pixels[i * dib.width + j].B;
		}

		temp += padding;

	}
	return data;
}

//Ham chuyen du lieu diem anh sang mang diem anh 24 bit
Pixel24bits* convertDataToPixArrayFor24(char*& data, DIB& dib) {
	// Bien luu kich thuoc anh
	int size = dib.width * dib.height;

	// Mang luu diem anh
	Pixel24bits* pixels = new Pixel24bits[size];

	// Tinh phan padding 
	int padding = (4 - (dib.width * dib.bpp / 8) % 4) % 4;

	// Bien temp lam con tro tam tro vao data
	char* temp = data;

	//Vong lap gan gia tri trong du lieu diem anh vao mang diem anh
	for (int i = 0; i < dib.height; i++) {
		for (int j = 0; j < dib.width; j++) {
			pixels[i * dib.width + j].B = *(temp++);
			pixels[i * dib.width + j].R = *(temp++);
			pixels[i * dib.width + j].G = *(temp++);
		}
		// Cong them padding de temp di qua phan padding
		temp += padding;
	}
	return pixels;
}

// Ham chuyen mang diem anh sang du lieu diem anh 24 bit
char* convertPixArrayToDataFor24(Pixel24bits*& pixels, DIB& dib) {
	// Tinh padding cho anh
	int padding = (4 - (dib.width * dib.bpp / 8) % 4) % 4;

	// Tinh kich thuoc anh
	int size = dib.height * dib.width * (dib.bpp / 8) + padding * dib.height;
	// Cap phat mang kich thuoc anh vao bien data
	char* data = new char[size];

	// Bien temp tro vao data
	char* temp = data;

	// Vong lap gan lan luot cac gia tri mau trong mang diem anh cho temp
	for (int i = 0; i < dib.height; i++) {
		for (int j = 0; j < dib.width; j++) {
			*(temp++) = pixels[i * dib.width + j].B;
			if (dib.bpp != 8) {
				*(temp++) = pixels[i * dib.width + j].R;
				*(temp++) = pixels[i * dib.width + j].G;
			}
		}

		temp += padding;
	}
	return data;
}

//Ham chuyen du lieu diem anh sang mang diem anh 32 bit
//Cach tinh tuong tu nhu ham 24 bit
Pixel32bits* convertDataToPixArrayFor32(char*& data, DIB& dib) {
	int size = dib.width * dib.height;
	Pixel32bits* pixels = new Pixel32bits[size];

	int padding = (4 - (dib.width * dib.bpp / 8) % 4) % 4;
	char* temp = data;
	for (int i = 0; i < dib.height; i++) {
		for (int j = 0; j < dib.width; j++) {
			pixels[i * dib.width + j].B = *(temp++);
			pixels[i * dib.width + j].G = *(temp++);
			pixels[i * dib.width + j].R = *(temp++);
			pixels[i * dib.width + j].A = *(temp++);
		}
		temp += padding * dib.bpp / 8;
	}
	return pixels;
}

//Ham chuyen mang diem anh sang du lieu diem anh 32 bit
//Cach tinh tuong tu nhu ham 24 bit
char* convertPixArrayToDataFor32(Pixel32bits*& pixels, DIB& dib) {
	int padding = (4 - (dib.width * dib.bpp / 8) % 4) % 4;
	int size = dib.height * dib.width * (dib.bpp / 8) + padding * dib.height;
	char* data = new char[size];

	char* temp = data;

	for (int i = 0; i < dib.height; i++) {
		for (int j = 0; j < dib.width; j++) {
			*(temp++) = pixels[i * dib.width + j].B;
			*(temp++) = pixels[i * dib.width + j].G;
			*(temp++) = pixels[i * dib.width + j].R;
			*(temp++) = pixels[i * dib.width + j].A;
		}
		temp += padding;
	}
	return data;
}

//Giai phong bo nho cho mang dong 8 bit
void releasePixArrayFor8(Pixel8bits* pixelarray)
{
	delete[]pixelarray;
	pixelarray = NULL;
}

//Giai phong bo nho cho mang dong 24 bit
void releasePixArrayFor24(Pixel24bits* pixelarray)
{
	delete[]pixelarray;
	pixelarray = NULL;
}

//Giai phong bo nho cho mang dong 32 bit
void releasePixArrayFor32(Pixel32bits* pixelarray)
{
	delete[]pixelarray;
	pixelarray = NULL;
}

//Giai phong bo nho cac con tro cap phat trong struct BMP
void releaseBMP(BMP& bmp) {
	delete[] bmp.pImageData;
	bmp.pImageData = NULL;
}

//Ham chuyen anh 24 bit sang 8 bit
int convert24bitsTo8Bits(BMP srcBmp, BMP& dstBmp) {
	if (srcBmp.dib.bpp != 24 || srcBmp.pImageData == NULL) {
		return 0;
	}
	//dien thong tin quan trong cho anh dest
	dstBmp.header = srcBmp.header;
	dstBmp.dib = srcBmp.dib;
	dstBmp.dib.bpp = 8;
	dstBmp.pDIBReserved = srcBmp.pDIBReserved;
	//tinh padding cho anh
	int padding = (4 - (dstBmp.dib.width * dstBmp.dib.bpp / 8) % 4) % 4;

	//tinh phan kich co anh
	dstBmp.dib.imageSize = (dstBmp.dib.width * dstBmp.dib.height * dstBmp.dib.bpp / 8) + padding * dstBmp.dib.height;

	// luu bang mau cua anh 8 bit
	for (int i = 0; i < 256; i++) {

		dstBmp.colorTable[i].B = i;
		dstBmp.colorTable[i].G = i;
		dstBmp.colorTable[i].R = i;
		dstBmp.colorTable[i].Reserved = 0;
	}

	// Chuyen du lieu diem anh sang mang diem anh 2 chieu
	Pixel24bits* srcPixels = convertDataToPixArrayFor24(srcBmp.pImageData, srcBmp.dib);

	// Cap phat mang 2 chieu cho mang diem anh destination
	Pixel8bits* dstPixels = new Pixel8bits[dstBmp.dib.width * dstBmp.dib.height];
	uint8_t ave;

	//Vong lap tinh diem trung binh tung diem anh
	for (int i = 0; i < dstBmp.dib.height; i++) {
		for (int j = 0; j < dstBmp.dib.width; j++) {
			int index = i * dstBmp.dib.width + j;
			ave = (char)((srcPixels[index].R + srcPixels[index].G + srcPixels[index].B) / 3);
			dstPixels[index].B = ave;
		}
	}

	// chuyen mang 2 chieu diem anh sang du lieu diem anh
	dstBmp.pImageData = convertPixArrayToDataFor8(dstPixels, dstBmp.dib);
	releasePixArrayFor8(dstPixels);
	return 1;
}

//Ham chuyen anh 32 bit sang 8 bit
// Cach tinh tuong tu nhu ham chuyen anh 24 bit
int convert32bitsTo8Bits(BMP srcBmp, BMP& dstBmp) {
	if (srcBmp.dib.bpp != 32 || srcBmp.pImageData == NULL) {
		return 0;
	}
	//dien thong tin quan trong cho anh dest
	dstBmp.header = srcBmp.header;
	dstBmp.dib = srcBmp.dib;
	dstBmp.dib.bpp = 8;
	dstBmp.pDIBReserved = srcBmp.pDIBReserved;
	int padding = (4 - (dstBmp.dib.width * dstBmp.dib.bpp / 8) % 4) % 4;

	dstBmp.dib.imageSize = (dstBmp.dib.width * dstBmp.dib.height * dstBmp.dib.bpp / 8) + padding * dstBmp.dib.height;

	for (int i = 0; i < 256; i++) {

		dstBmp.colorTable[i].B = i;
		dstBmp.colorTable[i].G = i;
		dstBmp.colorTable[i].R = i;
		dstBmp.colorTable[i].Reserved = 0;
	}
	Pixel32bits* srcPixels = convertDataToPixArrayFor32(srcBmp.pImageData, srcBmp.dib);
	Pixel8bits* dstPixels = new Pixel8bits[dstBmp.dib.width * dstBmp.dib.height];
	//char* ave = new char[dstBmp.dib.width * dstBmp.dib.height];
	uint8_t ave;
	for (int i = 0; i < dstBmp.dib.height; i++) {
		for (int j = 0; j < dstBmp.dib.width; j++) {
			int index = i * dstBmp.dib.width + j;
			ave = (char)((srcPixels[index].R + srcPixels[index].G + srcPixels[index].B) / 3);
			dstPixels[index].B = ave;
		}
	}
	dstBmp.pImageData = convertPixArrayToDataFor8(dstPixels, dstBmp.dib);
	releasePixArrayFor8(dstPixels);
	return 1;
}

//Ham zoom anh cho 8 bit
int zoomBMP8bits(BMP srcBmp, BMP& dstBmp, int s) {
	if (srcBmp.dib.bpp != 8 || srcBmp.pImageData == NULL) {
		return 0;
	}
	// Gan cac gia tri can thiet cua anh goc cho anh dich
	dstBmp = srcBmp;

	dstBmp.dib.compression = 0;

	// Chuyen du lieu diem anh sang mang diem anh 8 bit cua anh goc
	Pixel8bits* srcPixels = convertDataToPixArrayFor8(dstBmp.pImageData, dstBmp.dib);

	// Cap phat mang dong pixel cho anh dich
	Pixel8bits* dstPixels = new Pixel8bits[dstBmp.dib.width * dstBmp.dib.height];

	// Bien luu cac buoc di trong mang con hai chieu
	// Mang con hai chieu de tinh trung binh cac diem anh
	int xStep = 0, yStep = 0, index = 0;

	// Bien de tinh trung binh cho tung diem anh
	unsigned int averagePerPixel;
	for (int i = 0; i <= dstBmp.dib.height - s; i += s) {
		for (int j = 0; j <= dstBmp.dib.width - s; j += s) {
			//Gan lai gia tri bang 0 cho bien tinh trung binh
			averagePerPixel = 0;

			//Hai vong lap duoi day de tinh trung binh cho moi mang con 
			// Gan lai buoc di cua x 
			xStep = 0;
			for (int x = i; xStep < s; x++) {

				// Gan lai buoc di cua y
				yStep = 0;
				for (int y = j; yStep < s; y++) {
					averagePerPixel += srcPixels[x * dstBmp.dib.width + y].B;
					yStep++;
				}
				xStep++;
			}

			averagePerPixel = (char)(averagePerPixel / (s * s));

			// Gan gia tri trung binh cho diem anh dich
			dstPixels[index].B = (char)averagePerPixel;

			// Di qua diem anh tiep theo cua anh dich
			index++;

		}
	}
	// Chia kich thuoc anh dich cho s
	dstBmp.dib.height = (dstBmp.dib.height) / s;
	dstBmp.dib.width = (dstBmp.dib.width) / s;

	// Tinh phan padding cua anh dich
	int padding = (4 - (dstBmp.dib.width * dstBmp.dib.bpp / 8) % 4) % 4;

	// Gan gia tri kich thuoc moi vao kich thuoc anh dich
	dstBmp.dib.imageSize = dstBmp.dib.height * dstBmp.dib.width * dstBmp.dib.bpp / 8 + padding * dstBmp.dib.height;

	// Chuyen lai mang diem anh dich sang du lieu diem anh
	dstBmp.pImageData = convertPixArrayToDataFor8(dstPixels, dstBmp.dib);

	// Giai phong bo nho cho mang pixel 8 bit
	releasePixArrayFor8(dstPixels);
	return 1;
}

//Ham zoom anh cho 24 bit
// Tuong tu nhu ham zoom anh 8 bit
int zoomBMP24bits(BMP srcBmp, BMP& dstBmp, int s) {
	if (srcBmp.dib.bpp != 24 || srcBmp.pImageData == NULL) {
		return 0;
	}
	dstBmp = srcBmp;

	Pixel24bits* srcPixels = convertDataToPixArrayFor24(dstBmp.pImageData, dstBmp.dib);

	Pixel24bits* dstPixels = new Pixel24bits[dstBmp.dib.width * dstBmp.dib.height];

	int xStep = 0, yStep = 0, index = 0;

	unsigned int averagePerPixel[3];

	for (int i = 0; i <= dstBmp.dib.height - s; i += s) {
		for (int j = 0; j <= dstBmp.dib.width - s; j += s) {
			averagePerPixel[0] = averagePerPixel[1] = averagePerPixel[2] = 0;
			xStep = 0;
			for (int x = i; xStep < s; x++) {
				yStep = 0;
				for (int y = j; yStep < s; y++) {
					averagePerPixel[0] += (unsigned char)srcPixels[x * dstBmp.dib.width + y].B;
					averagePerPixel[1] += (unsigned char)srcPixels[x * dstBmp.dib.width + y].G;
					averagePerPixel[2] += (unsigned char)srcPixels[x * dstBmp.dib.width + y].R;
					yStep++;
				}
				xStep++;
			}

			averagePerPixel[0] = (unsigned char)(averagePerPixel[0] / (s * s));
			averagePerPixel[1] = (unsigned char)(averagePerPixel[1] / (s * s));
			averagePerPixel[2] = (unsigned char)(averagePerPixel[2] / (s * s));

			dstPixels[index].B = (unsigned char)averagePerPixel[0];
			dstPixels[index].G = (unsigned char)averagePerPixel[1];
			dstPixels[index].R = (unsigned char)averagePerPixel[2];
			index++;
		}
	}

	dstBmp.dib.height = (dstBmp.dib.height) / s;
	dstBmp.dib.width = (dstBmp.dib.width) / s;

	int padding = (4 - (dstBmp.dib.width * dstBmp.dib.bpp / 8) % 4) % 4;

	dstBmp.dib.imageSize = dstBmp.dib.height * dstBmp.dib.width * dstBmp.dib.bpp / 8 + padding * dstBmp.dib.height;

	dstBmp.pImageData = convertPixArrayToDataFor24(dstPixels, dstBmp.dib);
	releasePixArrayFor24(dstPixels);
	return 1;
}

//Ham zoom anh cho 32 bit
// Tuong tu nhu ham zoom anh 8 bit
int zoomBMP32bits(BMP srcBmp, BMP& dstBmp, int s) {
	if (srcBmp.dib.bpp != 32 || srcBmp.pImageData == NULL) {
		return 0;
	}
	dstBmp = srcBmp;

	Pixel32bits* srcPixels = convertDataToPixArrayFor32(dstBmp.pImageData, dstBmp.dib);

	Pixel32bits* dstPixels = new Pixel32bits[dstBmp.dib.width * dstBmp.dib.height];


	int xStep = 0, yStep = 0, index = 0;

	unsigned int averagePerPixel[4];
	for (int i = 0; i <= dstBmp.dib.height - s; i += s) {
		for (int j = 0; j <= dstBmp.dib.width - s; j += s) {
			averagePerPixel[0] = averagePerPixel[1] = averagePerPixel[2] = averagePerPixel[3] = 0;
			xStep = 0;
			for (int x = i; xStep < s; x++) {
				yStep = 0;
				for (int y = j; yStep < s; y++) {
					averagePerPixel[0] += srcPixels[x * dstBmp.dib.width + y].A;
					averagePerPixel[1] += srcPixels[x * dstBmp.dib.width + y].B;
					averagePerPixel[2] += srcPixels[x * dstBmp.dib.width + y].G;
					averagePerPixel[3] += srcPixels[x * dstBmp.dib.width + y].R;
					yStep++;
				}
				xStep++;
			}

			averagePerPixel[0] = (char)(averagePerPixel[0] / (s * s));
			averagePerPixel[1] = (char)(averagePerPixel[1] / (s * s));
			averagePerPixel[2] = (char)(averagePerPixel[2] / (s * s));
			averagePerPixel[3] = (char)(averagePerPixel[3] / (s * s));

			dstPixels[index].A = (char)averagePerPixel[0];
			dstPixels[index].B = (char)averagePerPixel[1];
			dstPixels[index].G = (char)averagePerPixel[2];	
			dstPixels[index].R = (char)averagePerPixel[3];

			index++;
		}
	}

	dstBmp.dib.height /= s;
	dstBmp.dib.width /= s;

	int padding = (4 - (dstBmp.dib.width * dstBmp.dib.bpp / 8) % 4) % 4;

	dstBmp.dib.imageSize = dstBmp.dib.height * dstBmp.dib.width * dstBmp.dib.bpp / 8 + padding * dstBmp.dib.height;

	dstBmp.pImageData = convertPixArrayToDataFor32(dstPixels, dstBmp.dib);

	releasePixArrayFor32(dstPixels);
	return 1;
}



