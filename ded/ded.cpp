#include <bits/stdc++.h>
using namespace std;

//функция для сжатия и вывода
void format (vector<bool> &bi, ofstream &out) {
	string d; //буфер, в котором будет хранится нужная информация для вывода, кол-во повторяющихся битов

	bool cur; //нынешний повторяющийся символ
	int count = 0; //счетчик того, сколько раз бит повторился
	bool block = false; //начали ли мы рассматривать строку 

	for (int i = 0; i < bi.size(); ++i) {
		//необходимое условие. нужно для того, чтобы программа корректно работала, если cur еще не определен
		if (!block) {
			block = true;
			cur = bi[i];
			count++;
			continue;
		}

		if (cur != bi[i]) { //если ряд повторяющихся не совпадает с последним
			if (count >= 6) { //если нам оптимально сжимать
				
				out << ':';

				//кооректно готовлю число повторений к выводу
				while (count > 0) {
					d += char(count % 10 + '0');
					count /= 10;
				}
				//переворачиваю, т.к. изначально записано в обратном порядке
				reverse(d.begin(), d.end());
				out << d;
				out << ',';
				out << char(cur + '0');
				out << ':';
				//чищу буфер для дальнейшей работы
				d.clear();

			} else {
				//если не оптимально сжимать, просто вывожу биты
				for (int j = 0; j < count; ++j)
					out << char(cur + '0');
				
			}
			//готовлю программу к новому блоку повторяющихся битов
			cur = bi[i];
			count = 1;
		} else 
			//отмечаю, что кол-во повторяющихся битов увеличилось
			count++;
	}

	//вывожу то, что не вывел. аналогично с кодом выше

	if (count >= 6) {
		out << ':';
		while (count > 0) {
			d += char(count % 10 + '0');
			count /= 10;
		}
		reverse(d.begin(), d.end());
		out << d;
		out << ',';
		out << char(cur + '0');
		out << ':';
		d.clear();
		
	} else {
	
		for (int j = 0; j < count; ++j)
			out << char(cur + '0');
				
	}

}

int main (int argc, char *argv[]) {

	//проверка на наличие аргументов
	if (argc == 1) {
		cout << "error: need arguments";
		return 0;
	}

	string puth; //полный путь до файла

	//считываю путь
	for (int i = 1; i < argc; ++i) {
		int dl = strlen(argv[i]); //вычисляю длину каждого аргумента
		for (int j = 0; j < dl; ++j) {
			puth += argv[i][j];
			if (argv[i][j] == '\\')
				puth += '\\';
		}
		puth += ' ';
	}
	puth.pop_back(); //удаляю лишний пробел
	ifstream in(puth.c_str(), ios_base::in); //открываю файл для чтения

	string puth_out = puth + ".ded"; //полный путь до сжатого файла
	ofstream out(puth_out.c_str(), ios_base::out | ios_base::trunc); //открываю файл для записи сжатого файла

	ifstream cfg_in("ded.cfg", ios_base::in); //открываю файл-конфиг для чтения

	int by_co; //по сколько байт хранить в оперативной памяти для сжатия

	//считываю и обрабатываю информацию из ded.cfg
	string s;
	while (cfg_in >> s) {

		int x = s.find("by_co");
		if (x != string::npos) {
			string d = s.substr(6, s.size() - 6 + 1);
			by_co = atoi(d.c_str());
			continue;
		}
	}


	vector<bool> bi; //массив, в которой хранятся биты
	bi.reserve(by_co); //резервирую память для by_co битов

	char o;

	while (in >> o) {

		bi.push_back(o - '0');

		//чищу буфер битов
		if (bi.size() == by_co) {
			format(bi, out);
			bi.clear();
		}
	}

	//вывожу остаток битов

	if (bi.size() > 0)
		format(bi, out);	
	//закрываю файлы
	in.close();
	out.close();
	cfg_in.close();


	return 0;
}