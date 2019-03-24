#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <set>
#include <locale>
#include <windows.h>
#include "my_tree.h"
#define DEBUG 0

vector<char> restricted_symb = { ',','-','—','!','?','.','"','\'','(',')', ' ', '\n', '\t' };
vector<string> intro_menu = { "Ввести текст файла", "Ввести текст вручную", "Назад" },
			   main_menu = { "Просмотреть дерево", "Найти элемент", "Добавить слово", "Добавить ещё текста", "Выкинуть одно слово", "Стереть слово", "Удалить часть текста"
							, "Очистить дерево", "Назад" };
fstream fin;
tree *t;

int my_compare(string first, string second);

void separate_string(string &s);

void init();

bool decision(string question);

int menu(vector<string> &variants, string title = "Выберите действие");

void connect_file(fstream &file, char type);

void fill_tree(tree *t, istream &from);

void erase_text_from_tree(tree *t, istream &from);

void error_exit(int error_code);

int main()
{
	init();
	//==========================================intro_menu===================================================//
	while (1) {
		system("CLS);
		cin.clear();
		int answer = menu(intro_menu);
		if (answer == -1) {
			exit(0);
		}
		else if (answer == 1) {
			connect_file(fin, 'r');
			fill_tree(t, fin);
			fin.close();
		}
		else if (answer == 2) {
			cout << "Начинайте вводить текст, когда закончите нажмите CTRL+Z\n";
			fill_tree(t, cin);
		}
		//=============================================main_menu================================================//
		while (1) {
			system("CLS);
			cin.clear();
			int answer = menu(main_menu);
			if (answer == -1) {
				tree_clear;
				fin.close();
				break;
			}
			else if (answer == 1) {
				cout << "Наше дерево:\n";
				tree_print(t,"\t");
			}
			else if (answer == 2) {
				cout << "Какое слово хотите найти?\n>";
				string string_to_find;
				cin >> string_to_find;
				tree *findable_tree = tree_find(t, string_to_find);
				if (findable_tree == nullptr) {
					cout << "В дереве нет такого элемента\n";
				}
				else {
					cout << "Найденный элемент:\n\t" << findable_tree->value << " : " << findable_tree->count << "\n";
				}
			}
			else if (answer == 3) {
				cout << "Какое слово будем добавлять?\n>";
				string string_to_add;
				cin >> string_to_add;
				tree_insert(t, string_to_add);
				tree *findable_tree = tree_find(t, string_to_add);
				if (findable_tree == nullptr) {
					error_exit(-1);
				}
				else{
					cout << "Элемент успешно добавлен!\n\t" << findable_tree->value << " : " << findable_tree->count << "\n";
				}
			}
			else if (answer == 4) {
				int answer = menu(intro_menu);
				if (answer == 1) {
					connect_file(fin, 'r');
					fill_tree(t, fin);
					fin.close();
				}
				else if (answer == 2) {
					cout << "Начинайте вводить текст, когда закончите нажмите CTRL+Z\n";
					fill_tree(t, cin);
				}
				cout << "Текст успешно добавлен!\n";
			}
			else if (answer == 5) {
				cout << "Какое слово выкидывать?\n>";
				string string_to_kick;
				cin >> string_to_kick;
				tree_kick(t, string_to_kick);
				cout << "Готово!\n";
			}
			else if (answer == 6) {
				cout << "Какое слово будет стирать?\n>";
				string string_to_erase;
				cin >> string_to_erase;
				tree_erase(t, string_to_erase);
				cout << "Готово!\n";
			}
			else if (answer == 7) {
				int answer = menu(intro_menu);
				if (answer == 1) {
					connect_file(fin, 'r');
					erase_text_from_tree(t, fin);
					fin.close();
				}
				else if (answer == 2) {
					cout << "Начинайте вводить текст, когда закончите нажмите CTRL+Z\n";
					erase_text_from_tree(t, cin);
				}
				cout << "Текст был успешно удалён!\n";
			}
			else if (answer == 8) {
				tree_clear(t);
				if (t->size == 0)
					cout << "Дерево успешно очищено!\n";
				else
					error_exit(-2);
			}
			system("pause");
		}
		//=================================================main_menu_end===============================================================//
	}
	//====================================================intro_menu_end===============================================================//
	return 0;
}

void init() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	t = new tree;
	if (!t)
		error_exit(-3);
	tree_set_compare_function(my_compare);
}

void fill_tree(tree *t, istream &from) {
	string current_string;
	while(1) {
		from >> current_string;
		if (from.eof())
			break;
		separate_string(current_string);
		if (current_string!="")
			tree_insert(t, current_string);
	}
	if (DEBUG) cout << "tree has been filled\n";
};

void erase_text_from_tree(tree *t, istream &from) {
	string current_string;
	while (1) {
		from >> current_string;
		if (from.eof())
			break;
		separate_string(current_string);
		if (current_string != "")
			tree_erase(t, current_string);
	}
	if (DEBUG) cout << "text has been erased\n";
};

void connect_file(fstream &file, char type) {
	bool file_ok = 0;
	cout << "Введите имя файла:\n>";
	string file_name;
	while (!file_ok) {
		cin >> file_name;
		if (type == 'r') {
			file.open(file_name, ios_base::in);
			if (!file.is_open()) {
				cout << "Такого файла нет в дирректории, попробуйте ещё раз:\n>";
				file_ok = 0;
			}
			else
				file_ok = 1;
		}
		else if (type == 'w') {
			file.open(file_name, ios_base::out);
			if (file.is_open()) {
				if (decision("Такой файл уже существует, перезаписать?"))
					file_ok = 1;
				else
					file_ok = 0;
			}
		}
	}
	if (DEBUG) cout << "file has been connected\n";
}

int menu(vector<string> &variants, string title) {
	cout << title << ":\n";
	for (int i = 0; i<variants.size(); i++) {
		cout << '\t' << i+1 << ". " << variants[i] << "\n";
	}
	cout << ">";
	int answer = 0;
	while (answer == 0) {
		cin >> answer;
		if (answer<1 || answer>variants.size()) {
			answer = 0;
			cout << "Неправильно набран номер, попробуйте ещё раз.\n>";
		}
	}
	return answer==variants.size()?-1:answer;
}

void separate_string(string &s) {
	for (char symb : restricted_symb) {
		bool symb_founded = 1;
		while (symb_founded) {
			symb_founded = 0;
			int restricted_symb_position = s.find(symb);
			if (restricted_symb_position != string::npos) {
				symb_founded = 1;
				s.erase(restricted_symb_position, 1);
			}
		}
	}
}

bool decision(string question) {
	string answer;
	while (1) {
		cout << question << " (y/n)\n>";
		cin >> answer;
		if (answer == "y" || answer == "Y")
			return 1;
		else if (answer == "n" || answer == "N")
			return 0;
		cout << "Не ясно ... ещё раз. ";
	}
	
}

int my_compare(string first, string second) {
	for (int i = 0; i < first.size(); i++) {
		first[i] = tolower(first[i]);
	}
	for (int i = 0; i < second.size(); i++) {
		second[i] = tolower(second[i]);
	}
	for (int i = 0; i < min(first.size(), second.size()); i++) {
		if (first[i] > second[i])
			return 1;
		else if (first[i] < second[i])
			return -1;
	}
	if (first.size() > second.size())
		return 1;
	else if (first.size() < second.size())
		return -1;
	else
		return 0;
}

void error_exit(int error_code) {
	cout << "\nОШИБКА: Видимо дерево сломанно, до новых встреч\n";
	if (DEBUG) cout << error_code;
	exit(error_code);
}

