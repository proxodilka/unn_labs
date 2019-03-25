#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <limits>
#include <windows.h>
#include "my_queue.h"
#define DEBUG 0

using namespace std; 

vector<char> restricted_symb = { ',','-','—','!','?','.','"','\'','(',')', ' ', '\n', '\t' };
vector<string> intro_menu = { "Ввести из файла", "Ввести вручную", "Назад" },
			main_menu = { "Просмотреть очередь", "Добавить в очередь", "Исключить из очереди", "Посмотреть первого в очереди"
						  , "Посмотреть последнего в очереди", "Очистить очередь", "Разделить очередь на две", "Назад" },
			select_queue = { "", "", "Соеденить две очереди в одну" ,"Назад" };
queue *q, *q_first, *q_second;
bool many_queues_defined=0;
fstream fin;

void init();

bool decision(string question);

int menu(vector<string> &variants, string title = "Выберите действие");

void connect_file(fstream &file, char type);

void fill_queue(queue *q, istream &from);

void error_exit(int error_code);

int main()
{
	init();
	//=====================================================intro_menu====================================================================================//
	while (1) {
		system("CLS");
		cin.clear();
		int answer = menu(intro_menu, "Надо заполнить очередь, как это сделать?");
		if (answer == -1) {
			exit(0);
		}
		else if (answer == 1) {
			connect_file(fin, 'r');
			fill_queue(q, fin);
			fin.close();
		}
		else if (answer == 2) {
			cout << "Вводите здесь, когда закончите нажмите CTRL+Z\n";
			cin.clear();
			cin.ignore();
			fill_queue(q, cin);
		}
		//================================================================menu_for_many_queues==============================================================//
		while (1) {
			cin.clear();
			system("CLS");
			if (many_queues_defined) {
				int answer = menu(select_queue, "Отныне у нас две очереди, какой же нам теперь воспользоваться?");
				if (answer == -1) {
					queue_clear(q_first);
					queue_clear(q_second);
					break;
				}
				else if (answer == 1) {
					queue_clear(q);
					queue_cpy(q_first, q);
					cout << "Выбранна первая очередь\n";
				}
				else if (answer == 2) {
					queue_clear(q);
					queue_cpy(q_second, q);
					cout << "Выбранна вторая очередь\n";
				}
				else if (answer == 3) {
					queue_clear(q);
					node *node_from_first_queue = q_first->first;
					node *node_from_second_queue = q_second->first;
					while (node_from_first_queue && node_from_second_queue) {
						queue_push(q, node_from_second_queue->value);
						queue_push(q, node_from_first_queue->value);
						node_from_first_queue = node_from_first_queue->next;
						node_from_second_queue = node_from_second_queue->next;
					}
					while (node_from_second_queue) {
						queue_push(q, node_from_second_queue->value);
						node_from_second_queue = node_from_second_queue->next;
					}
					while (node_from_first_queue) {
						queue_push(q, node_from_first_queue->value);
						node_from_first_queue = node_from_first_queue->next;
					}
					queue_clear(q_first);
					queue_clear(q_second);
					many_queues_defined = 0;
				}
			}
			//=============================================================main_menu========================================================================//
			while (1) {
				system("CLS");
				cin.clear();
				int answer = menu(main_menu);
				if (answer == -1) {
					queue_clear(q);
					fin.close();
					break;
				}
				else if (answer == 1) {
					cout << "Наша очередь:\n";
					queue_print(q, "\t", '\n', 1);
				}
				else if (answer == 2) {
					cout << "Кого будем добавлять?\n";
					string string_to_add;
					cin.clear();
					cin.ignore();
					getline(cin, string_to_add);
					queue_push(q, string_to_add);
					cout << "Готово!\n";
				}
				else if (answer == 3) {
					string deleted_value = queue_pop(q);
					if (deleted_value == "")
						cout << "Так ведь очередь пуста...\n";
					else
						cout << "Успешно удалили элемент: " << deleted_value << "\n";
				}
				else if (answer == 4) {
					if (q->size == 0)
						cout << "Очередь пуста!\n";
					else
						cout << "Первый в очереди у нас: " << q->first->value << "\n";
				}
				else if (answer == 5) {
					if (q->size == 0)
						cout << "Очередь пуста!\n";
					else
						cout << "Последний в очереди: " << q->last->value << "\n";
				}
				else if (answer == 6) {
					queue_clear(q);
					cout << "Теперь очередь пуста!\n";
				}
				else if (answer == 7) {
					node *current_queue_node = q->first;
					queue_clear(q_first);
					queue_clear(q_second);
					int i = 0;
					while(current_queue_node) {
						if ((i + 1) % 2 == 1)
							queue_push(q_second, current_queue_node->value);
						else
							queue_push(q_first, current_queue_node->value);
						current_queue_node = current_queue_node->next;
						i++;
					}
					many_queues_defined = 1;
					queue_clear(q);
					break;
				}
				system("pause");
			}
			//=============================================================main_menu_end========================================================================//
			if (!many_queues_defined)
				break;
		}
		//================================================================menu_for_many_queues_end==============================================================//
		many_queues_defined = 0;
	}
	//=====================================================intro_menu_end====================================================================================//
}

void init() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	q = new queue;
	q_first = new queue;
	q_second = new queue;
	if (!q || !q_first || !q_second)
		error_exit(-3);
}

void fill_queue(queue *q, istream &from) {
	string current_string;
	from.clear();
	while (getline(from, current_string)) {
		queue_push(q, current_string);
	}
	if (DEBUG) cout << "queue has been filled\n";
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
	for (int i = 0; i < variants.size(); i++) {
		cout << '\t' << i + 1 << ". " << variants[i] << "\n";
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
	return answer == variants.size() ? -1 : answer;
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

void error_exit(int error_code) {
	cout << "\nОШИБКА: Видимо дерево сломанно, до новых встреч\n";
	if (DEBUG) cout << error_code;
	exit(error_code);
}


