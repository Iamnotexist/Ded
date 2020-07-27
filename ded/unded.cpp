#include <bits/stdc++.h>
using namespace std;


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

	//получаю полный путь до "разжатого файла, путем удаления .ded"
	string puth_out = puth;
	for (int i = 0; i < 4; ++i)
		puth_out.pop_back();

	ofstream out(puth_out.c_str(), ios_base::out | ios_base::trunc); //открываю файл для записи разжатого файла

	//далее я делаю проверку, насколько много данных будет после архивации 
	//если размер файла будет слишком большой, выведется сообщение с предупреждением

	int sz = 0; //тут считается колво символов, которое будет после разжатия
	bool big = false; //слишком ли большой файл.


	char o;
	while (in >> o) {

		if (sz >= (int)1e8) { // 10 в 8 символов - много символов
			big = true;	
			break;
		}

		if (o == ':') {
			string d; //буфер, в котором хранится число в виде строки
			while (true) {
				in >> o; 
				if (o == ',') 
					break;
				d += o;
			}

			if (d.size() > 7) { // 10 в 8 символов - много символов
				big = true;
				break;
			}

			sz += atoi(d.c_str()); //добавляю информацию о количестве символов

			if (sz >= (int)1e8) { // 10 в 8 символов - много символов
				big = true;
				break;
			}

			//пропускаю ненужные символы
			while (true) {
				in >> o;
				if (o == ':')
					break;
			}
		} else
			sz++; //отмечаю, что колво символов увеличилось на 1
	}

	//вывожу предупреждение о том, что файл может быть очень большим
	//завершаю программу, если пользователь испугался

	if (big) {
		cout << "WARNING! THIS FILE CAN BE BOMB! YOU SHURE THAT YOU WANT unded THIS FILE? [Y/N]" << endl;
		cin >> o;
		if (o != 'Y') {
			in.close();
			out.close();
			return 0;
		}
	}


	//переоткрываю файл

	in.close();
	in.open(puth.c_str(), ios_base::in);

	//разжимаю файл
	while (in >> o) {

		if (o == ':') {
			string d;
			while (true) {
				in >> o;
				if (o == ',')
					break;
				d += o;
			}
			int col = atoi(d.c_str()); //количество символов, которое нужно вывести
			in >> o; //символ, который нужно выводить

			//вывожу этот символ столько раз, сколько нужно
			for (int i = 0; i < col; ++i)
				out << o;
			in >> o; // тут находится :
			continue;
		} else
			out << o; //просто вывожу символ
	}


	in.close();
	out.close();




	return 0;
} 