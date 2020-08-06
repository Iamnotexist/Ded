#include <bits/stdc++.h>
#include <direct.h>
#include <windows.h>
#include <Windows.h>

using namespace std;

#define MAX_PATH_CUR_DIR 1000 //максимальный путь до директории с нужным файлом - 1

//проверка существования файла
bool _file_is_exist (string file);

//проверка существования директории
bool _dir_is_exist (string dir);

//своя сортировка (немного измененная, специально для данной программы)
void msort (int *b, int *e);

stack<int> zs; //стек значений для msort 
stack<char> ss; //стек символов

unsigned char kod[256], //здесь я храню какой символ соответствует каждому стандартному
	 		  dok[256], //обратное kod
	 		  pri[256],  //приоритет назначения 
	 	      sym[256], //массив, изначально в котором индекс будет равен значению для каждого элемента
	 		  poi = 0; //номер последнего неназначенного символа

int kol[256]; //количество встреченных символов	 

int main (int argc, char *argv[]) {

	char path_cur_dir[MAX_PATH_CUR_DIR]; //здесь будет хранится путь до директории нужного файла
	_getcwd(path_cur_dir, sizeof(path_cur_dir));

	string pcd = path_cur_dir; //для упрощения работы

	//проверяю наличие аргументов
	if (argc == 1) {
		cout << "\nerror_ded: add arguments\n";
		return 0;
	}

	//проверяю, существует ли файл
	if (!_file_is_exist(argv[1])) {
		cout << "\nerror_ded: file not exist\n";
		return 0;
	}

	//открываю файл для чтения информации
	ifstream in(std::string(argv[1]).c_str(), ios_base::in);

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
			cout << "\nerror_ded: directory not exist\n";
			return 0;
		}

		if (d.back() != '\\')
			d += '\\';

		d += string(argv[1]) + ".ded";

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

		out.open(d.c_str(), ios_base::trunc | ios_base::binary);
	} else { //если файл нужно записать в эту же директорию

		string d = string(argv[1]) + ".ded";

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

		out.open(d.c_str(), ios_base::trunc | ios_base::binary);
	}

	//составляю приоритет
	for (int i = 0; i < 256; ++i)
		pri[i] = i;

	//сортирую массив. Левее будут элементы с меньшим количеством единиц 
	sort(pri, pri + 256, 
			[&](int a, int b) {

				int c_a = 0, //колво единиц в записи a
					c_b = 0; //колво единиц в записи b

				for (int i = 0; i < 8; ++i) {
					c_a += ((a & (1 << i)) > 0); //проверяю каждый бит на то, стоит ли в нем единица
					c_b += ((b & (1 << i)) > 0);
				}

				return c_a < c_b;
			} 
		);

	//сдвигаю числа, так как 00000000 - не может быть кодом символа в данном алгоритме
	for (int i = 1; i < 256; ++i)
		pri[i - 1] = pri[i];

	//считаю колво раз, которое встречался каждый символ в файле 
	while (!in.eof()) {
		unsigned char o = in.get();
		if (in.eof())
			break;

		kol[o]++;
	}	

	//заполняю sym
	for (int i = 0; i < 256; ++i)
		sym[i] = i;

	//сортирую символы. левее те, что встречаются раньше. Так же сохраняю в sym, какой символ стоит на какой позиции в kol
	msort(kol, kol + 256);

	for (int i = 0; i < 256; ++i) //каждому назначению назначаю символ
		dok[sym[i]] = pri[i];

	for (int i = 0; i < 255; ++i) //каждому символу назначаю новый (кроме 00000000)
		kod[pri[i]] = sym[i];


	//выписываю информацию для разжимания
	for (int i = 1; i < 256; ++i) {
		unsigned char di = i, dk = kod[i];
		out.write((char*)&di, sizeof(di));
		out.write((char*)&dk, sizeof(dk));
	}


	unsigned long long c0 = 0; //счетчик нулей
	for (int i = 0; i < 256; ++i) { //считаю колво нулей
		int u = 0; //колво нулей в одном символе 
		for (int j = 0; j < 8; ++j)
			u += ((dok[sym[i]] & (1 << j)) == 0);
		c0 += u * kol[i];	
	}


	//вывожу 8 байт - информацию о количестве нулей
	for (int i = 0; i < 8; ++i) {
		unsigned char u = 0;
		for (int j = 0; j < 8; ++j) 
			u += ((((1ull << (j + 8 * i)) & c0) > 0) << j);

		out.write((char*)&u, sizeof(u));
	}

	//далее по 4 бита вывожу, сколько нулей до каждой единицы

	int k0 = 0; //сколько нулей до единицы
	unsigned char ou = 0; //символ, который я буду каждый раз выводить

	in.close();
	in.open(std::string(argv[1]).c_str(), ios_base::in);

	bool bitter = false; //если false - первая половина байта, иначе правая

	while (!in.eof()) {
		unsigned char o = in.get();
		if (in.eof())
			break;

		o = dok[o]; //другое обозначение для этого сивола

		vector<bool> d; //чтобы перевести символ в битовую запись 

		for (int i = 0; i < 8; ++i)
			d.push_back(((o & (1 << i)) > 0));


		for (int i = 0; i < 8; ++i) {

			if (!d[i]) {
				k0++;
				continue;
			}

			if (!bitter) {
				ou += k0;
				k0 = 0;
				bitter = !bitter;
				continue;
			}

			ou += (k0 << 4);
			bitter = !bitter;
			k0 = 0;
			out.write((char*)&ou, sizeof(ou));
			ou = 0;
		}

	}

	if (bitter) {
		ou += (15 << 4);
		out.write((char*)&ou, sizeof(ou));
	}

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

//bubble-sort на полуинтервалах с небольшими изменениями
//изменения в том, что я меняю элементы не только в kol, но и в sym
void msort (int *b, int *e) {

	if (b + 1 == e)
		return;

	int *bl = b, *el = kol + ((b - kol) + (e - kol)) / 2, //концы левого отрезка
		 *br = kol + ((b - kol) + (e - kol)) / 2, *er = e; //концы правого отрезка	 

	msort(bl, el); 
	msort(br, er);

	while (bl != el || br != er) {

		if (bl == el) {

			zs.push((*br));
			ss.push(sym[br - kol]);

			br++;
			continue;
		}

		if (br == er) {
			
			zs.push((*bl));
			ss.push(sym[bl - kol]);

			bl++;
			continue;
		}

		if ((*bl) > (*br)) {

			zs.push((*bl));
			ss.push(sym[bl - kol]);
			
			bl++;
		} else {

			zs.push((*br));
			ss.push(sym[br - kol]);

			br++;
		}

	}	 


	while (!zs.empty()) {

		sym[zs.size() - 1 + b - kol] = ss.top();
		kol[zs.size() - 1 + b - kol] = zs.top();

		zs.pop();
		ss.pop();
	}

}