#include <bits/stdc++.h>
#include <direct.h>
#include <Windows.h>
#include <windows.h>
using namespace std;

#define MAX_PATH_CUR_DIR 1000 //максимальный путь до директории с нужным файлом - 1

//проверка существования файла
bool _file_is_exist (string file);

//проверка существования директории
bool _dir_is_exist (string dir);

unsigned char kod[256]; //здесь содержится информация, чтобы записать символ таким, каким он был изначально

vector<bool> buf; //буффер битов для вывода
void _buf_add (bool a, ofstream &out); //фунцкия добавления в буфер и вовода байта


int main (int argc, char *argv[]) {

	char path_cur_dir[MAX_PATH_CUR_DIR]; //здесь будет хранится путь до директории нужного файла
	_getcwd(path_cur_dir, sizeof(path_cur_dir));

	string pcd = path_cur_dir; //для упрощения работы

	//проверяю наличие аргументов
	if (argc == 1) {
		cout << "\nerror_unded: add arguments\n";
		return 0;
	}

	//проверяю, существует ли файл
	if (!_file_is_exist(argv[1])) {
		cout << "\nerror_unded: file not exist\n";
		return 0;
	}

	//открываю файл для чтения информации
	ifstream in(std::string(argv[1]).c_str(), ios_base::binary);

	fstream file(std::string(argv[1]).c_str()); //узнаю размер сжатого файла
	file.seekg(0, ios::end);
	long long file_sz = file.tellg();
	file.close();


	//для вывода
	ofstream out;

	if (argc >= 3) { //если файл нужно записать в определенную директорию
		string d; //читаю директорию
		for (int i = 2; i < argc; ++i) {
			d += argv[i];
			d += ' ';
		}
		d.pop_back(); //удаляю лишний пробел

		if (d.back() == '\\') //из-за реализации, бэк-слэша в конце быть не должно 
			d.pop_back();

		if (!_dir_is_exist(d)) {
			cout << "\nerror_unded: directory not exist\n";
			return 0;
		}

		if (d.back() != '\\')
			d += '\\';

		d += string(argv[1]);
		for (int i = 0; i < 4; ++i) //"удаляю .ded"
			d.pop_back();

		if (_file_is_exist(d)) { //если такой файл уже существует
			cout << "\nFile " << d << " is exist." << " You sure want re-write file? [Y/N]\n";
			
			bool _ned = false;
			while (!_ned) {
				char o;
				cin >> o;

				if (o == 'Y') {
					_ned = true;
					break;
				}

				if (o == 'N') 
					return 0;

				if (!_ned) 
					cout << "\nFile " << d << " is exist." << " You sure want re-write file? [Y/N]\n";
			}
		}

		out.open(d.c_str(), ios_base::out | ios_base::trunc);
	} else { //если файл нужно записать в эту же директорию

		string d = string(argv[1]);
		for (int i = 0; i < 4; ++i) //"удаляю .ded"
			d.pop_back();

		if (_file_is_exist(d)) { //если такой файл уже существует
			cout << "\nFile " << d << " is exist." << " You sure want re-write file? [Y/N]\n";
			
			bool _ned = false;
			while (!_ned) {
				char o;
				cin >> o;

				if (o == 'Y') {
					_ned = true;
					break;
				}

				if (o == 'N') 
					return 0;

				if (!_ned) 
					cout << "\nFile " << d << " is exist." << " You sure want re-write file? [Y/N]\n";
			}
		}

		out.open(d.c_str(), ios_base::out | ios_base::trunc);
	}


	//читаю информацию для записи изначальных символов
	for (int i = 1; i < 256; ++i) {
		unsigned char a, b;
		in.read((char*)&a, sizeof(a));
		in.read((char*)&b, sizeof(b));

		kod[a] = b;
	}


	//читаю колво нулей
	long long c0 = 0;

	for (int i = 0; i < 8; ++i) {
		unsigned char o;
		in.read((char*)&o, sizeof(o));

		for (int j = 0; j < 8; ++j) 
			c0 += ((((o & (1ull << j)) > 0ull)) << (i * 8ull + j)); 
	}

	long long ufile = (((file_sz - 255 * 2 - 8) * 2) + c0) / 8; //примерный размер файла после разжатия 

	//оповещаю о примерном размере
	while (true) {
		cout << "\nAfter unded, you will get file ~" << ufile << " byte. You sure want unded? [Y/N]\n";

		char su_u;
		cin >> su_u;

		if (su_u == 'Y')
			break;

		if (su_u == 'N')
			return 0;
	}

	unsigned char kon = 0; //для вывода символов
	bool biter = false; //первая половина байта или вторая



	while (true) { //начинаю считывание
		unsigned char o;
		in.read((char*)&o, sizeof(o));
		if (in.eof())
			break;
		//проверяю первую половину байта

		int zz = o % 16; //добавляю в буффер биты для вывода
		for (int i = 0; i < zz; ++i) {
			_buf_add(0, out);
			c0--;
		}
		_buf_add(1, out);

		zz = o / 16;


		if (zz == 15)
			break;

		for (int i = 0; i < zz; ++i) {
			_buf_add(0, out);
			c0--;
		}
		_buf_add(1, out);

	}

	for (int i = 0; i < c0; ++i)
		_buf_add(0, out);

	out.close();
	in.close();


	return 0;
}


//проверяю, существует ли файл 
bool _file_is_exist (std::string file) {
	std::ifstream _in(file, std::ios_base::in);
	return _in.is_open();
}

//проверяю, существует ли директория
bool _dir_is_exist (string dir) {

	TCHAR *tdir = new TCHAR[dir.size() + 1];
	for (int i = 0; i < dir.size(); ++i)
		tdir[i] = dir[i];
	tdir[dir.size()] = '\0';

	DWORD flag = GetFileAttributes(tdir);
	if (flag == 0xFFFFFFFFUL) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			return false;
	}

	if (!(flag & FILE_ATTRIBUTE_DIRECTORY))
		return false;

	return true;
}

//функция для вывода байта ъ
void _buf_add (bool a, ofstream &out) {
	buf.push_back(a);

	if (buf.size() == 8) {

		unsigned char o = 0;
		for (int i = 0; i < 8; ++i)
			o += (buf[i] << i);

		out << kod[o];

		buf.clear();
	}
}