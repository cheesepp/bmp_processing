#include "Provider.h"

using namespace std;

//Luu tru duong dan file
char filename_in[512], filename_out[512];

//Ham gioi thieu chuong trinh, thanh vien
void welcome() {
	cout << endl;
	cout << " ============================ BAI THU HOACH - BAI TAP BMP ==========================\n";
	cout << "                          Giang vien huong dan: Pham Minh Hoang                     \n";
	cout << "                             Ho va ten: Tran Dinh Nhat Tri                          \n";
	cout << "                                     Lop: 21CTT5                                    \n";
	cout << "                                    MSSV: 21120576                                  \n";
	cout << " ===================================================================================\n";
}

//Ham nhap dia chi doc
void readInput() {
	cout << "\nNhap vao dia chi doc file .bmp (VD: D:/lena_inp.bmp): ";
	cin.getline(filename_in, 512);
}

//Ham nhap dia chi xuat
void writeInput() {
	cout << "\nNhap vao dia chi xuat file .bmp (VD: D:/lena_out.bmp): ";
	cin.getline(filename_out, 512);
}

//Ham xu ly convert anh 24/32 BPP sang 8 BPP
void convertBMPto8(char* fileNameInput, char* fileNameOutput) {
	/*Khai bao cac cau truc de luu tru cac
	cau truc trong file BMP duoc nhap */
	BMP bmp;

	//Luu tru phan Dib cua file BMP
	readBMPFile((char*)fileNameInput, bmp);

	// Kiem tra anh da doc la 32 hay 24 bit
	if (bmp.dib.bpp == 24) {
		//Khai bao cau truc file BMP 24bpp de doc
		BMP bmp24To8;

		//Xuat file 8bpp theo duong dan
		convert24bitsTo8Bits(bmp, bmp24To8);

		//Ghi anh da chuyen vao file dich
		writeBMPFile((char*)fileNameOutput, bmp24To8);

		// Giai phong bo nho
		releaseBMP(bmp);
		releaseBMP(bmp24To8);
		cout << "\nChuyen doi anh sang 8 bit thanh cong!\n";
	}
	//Cac lenh trong ham tuong tu nhu 24 bit
	else if (bmp.dib.bpp == 32) {

		BMP bmp32To8;

		convert32bitsTo8Bits(bmp, bmp32To8);

		writeBMPFile((char*)fileNameOutput, bmp32To8);

		releaseBMP(bmp);
		releaseBMP(bmp32To8);

		cout << "\nChuyen doi anh sang 8 bit thanh cong!\n";
	}
	else {

		cout << "Khong phai anh 24 hoac 32 bit! Vui long nhap lai";

	}
}

//Ham zoom anh BMP theo yeu cau
void zoomBMP(char* fileNameInput, char* fileNameOutput, int S) {

	BMP srcBmp;
	readBMPFile((char*)fileNameInput, srcBmp);
	BMP dstBmp;
	//Kiem tra anh bao nhieu bit
	if (srcBmp.dib.bpp == 8) {
		// Ham zoom anh 8 bit
		zoomBMP8bits(srcBmp, dstBmp, S);

		//Ghi anh da zoom vao file dich
		writeBMPFile((char*)fileNameOutput, dstBmp);

		//Giai phong bo nho du lieu BMP
		releaseBMP(srcBmp);
		releaseBMP(dstBmp);
		cout << "\nZoom anh thanh cong!\n";
	}
	
	// Cac lenh trong ham tuong tu nhu 8 bit
	else if (srcBmp.dib.bpp == 24) {

		// Ham zoom anh 24 bit
		zoomBMP24bits(srcBmp, dstBmp, S);

		//Ghi anh da zoom vao file dich
		writeBMPFile((char*)fileNameOutput, dstBmp);

		//Giai phong bo nho du lieu BMP
		releaseBMP(srcBmp);
		releaseBMP(dstBmp);
		cout << "\nZoom anh thanh cong!\n";
	}

	else if (srcBmp.dib.bpp == 32) {

		// Ham zoom anh 32 bit
		zoomBMP32bits(srcBmp, dstBmp, S);

		//Ghi anh da zoom vao file dich
		writeBMPFile((char*)fileNameOutput, dstBmp);

		releaseBMP(srcBmp);
		releaseBMP(dstBmp);

		cout << "\nZoom anh thanh cong!\n";
	}
	else {

		cout << "Anh nay khong thuoc dang 8 bit hoac 24 bit hoac 32 bit!";

	}
}

// Ham main thuc thi chuong trinh
int main(int argc, char* argv[])
{
	// Ham xin chao
	welcome();

	//Dung de chay chuong trinh console
	if (argc == 1) {
		//Menu chuong trinh
		cout << "\n ========================= MENU ========================:\n";
		cout << "\n Moi ban nhan cac phim sau de thuc hien chuong trinh:    \n";
		cout << "     Phim 1: Chuyen doi anh 24 hoac 32 bit sang anh 8 bit  \n";
		cout << "     Phim 2: Thu nho anh 8 hoac 24 hoac 32 bit          \n";
		cout << "     Phim 0: Thoat chuong trinh \n";
		cout << "\n Vui long nhap: ";

		int choice = -1; cin >> choice;
		while (choice < 0 && choice > 2) {
			cout << "Lua chon khong hop le. Xin nhap lai.\n";
			cin >> choice;
		}

		if (choice == 0)
			return 0;

		cin.ignore();

		readInput();
		writeInput();

		if (choice == 1) {
			convertBMPto8((char*)filename_in, (char*)filename_out);
		}

		if (choice == 2) {
			cout << "\nNhap so de thu nho (vi du 2): ";
			int S; 
			cin >> S;

			zoomBMP((char*)filename_in, (char*)filename_out, S);
		}

		cout << "\n================ HOAN THANH YEU CAU =====================\n";
	}

	//Error handler - Xu ly khi nhap sai
	if (argc > 1 && argc < 4 || argc > 5) {
		cout << "Lenh Nhap Vao Khong Hop Le!\n";
		cout << "Cau truc cau lenh: 21120576.exe -conv <Input Path> <Output Path>\n";
		cout << "21120576.exe -zoom <Input Path> <Output Path> <ratio>\n";
		return 0;
	}

	//Ham xu ly argv -conv
	if (argc == 4) {

		//Kiem tra argv co dung yeu cau hay khong
		if (strcmp(argv[1], "-conv") != 0) {
			cout << "Lenh Nhap Vao Khong Hop Le !\n";
			cout << "Cau truc cau lenh: 21120576.exe -conv <Input Path> <Output Path>\n";
			return 0;
		}

		convertBMPto8(argv[2], argv[3]);
	}

	//Ham xu ly argv - zoom
	if (argc == 5) {

		//Error handler - Khu vuc xu ly loi

		//Kiem tra argv co dung yeu cau hay khong
		if (strcmp(argv[1], "-zoom") != 0) {
			cout << "Lenh Nhap Vao Khong Hop Le !\n";
			cout << "Cau truc cau lenh: 21120576.exe -zoom <Input Path> <Output Path> S\n";
			return 0;
		}

		//Chuyen S tu char sang int
		int S = atoi(argv[4]);

		zoomBMP(argv[2], argv[3], S);
	}

	return 0;
}