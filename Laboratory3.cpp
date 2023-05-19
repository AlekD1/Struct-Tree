#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <cstring>
using namespace std;
/*Описание структуры*/
struct marsh {
	int id, number;
	char nap[15], kop[15];
};

struct tree {
	marsh inf;
	tree* right;
	tree* left;
};
/*Прототипы функций*/
const int size_marsh = sizeof(marsh);
marsh vvod_st();
tree* tree_org(), * read_file(FILE* f), * addtree(tree* top, const marsh& newtree_st), * tree_delete(tree* top, int s);
void save_tree(tree* top, FILE* f), free_memory(tree* top), prosmotr(tree* top), tree_output(tree* top, int otstup);
int most_right(tree* top, int level);
void pechat(tree* top, char* q);
/*Главная функция*/
int main() {
	int c;
	int s;
	char q[10];
	setlocale(LC_ALL, "Russian");
	tree* top = NULL;
	FILE* f = fopen("data.dat", "rb+");
	if (f == NULL) {
		f = fopen("data.dat", "wb+");
		cout << "Файл создан" << endl;
		if (f == NULL) {
			cout << "Неудалось открыть файл";
			exit(1);
		}
	}
	fseek(f, 0, 0);
	while (1) {
		_getch();
		system("cls");
		cout << "Меню" << endl <<
			"1.Организовать дерево" << endl <<
			"2.Просмотреть дерево" << endl <<
			"3.Отобразить структуру дерева" << endl <<
			"4.Добавить узел в дерево" << endl <<
			"5.Исключить узел из дерева" << endl <<
			"6.Сохранить в файл" << endl <<
			"7.Загрузить дерево из файла" << endl <<
			"8.Найти уровень самого правого элемента" << endl <<
			"9.Печать информации о маршрутах" << endl;
		cin >> c;
		switch (c) {
		case 0: fclose(f); free_memory(top); return 0;
		case 1: top = tree_org(); break;
		case 2: prosmotr(top); break;
		case 3: tree_output(top, 0); break;
		case 4: addtree(top, vvod_st()); break;
		case 5: {
			puts("Введите номер маршрута");
			scanf("%d", &s);
			tree_delete(top, s); break;
		}
		case 6: save_tree(top, f); cout << "Список сохранен"; break;
		case 7: top = read_file(f); break;
		case 8: {int rmost = most_right(top, 0);
			if (rmost == -1) printf("Дерево пустое\n");
			else printf("Уровень самого правого элемента: %d\n", rmost);
			break; }
		case 9: {
			puts("Введите название маршрута");
			cin >> q;
			pechat(top, q);
			break;
		}
		default: break;
		}
	}
}
/*Создание узла*/
tree* addtree(tree* top, const marsh& newtree_st) {
	if (!top) {
		top = (tree*)malloc(sizeof(tree));
		top->inf = newtree_st;
		top->left = NULL;
		top->right = NULL;
	}
	else {
		if (top->inf.number > newtree_st.number) {
			top->left = addtree(top->left, newtree_st);
		}
		else {
			top->right = addtree(top->right, newtree_st);
		}
	}
	return top;
}
/*Структура дерева*/
void tree_output(tree* top, int otstup) {
	if (top) {
		otstup += 3;
		tree_output(top->right, otstup);
		for (int i = 0; i < otstup; i++) cout << " ";
		cout << top->inf.number << endl;
		tree_output(top->left, otstup);
	}
}
/*Ввод данных студента*/
marsh vvod_st() {
	marsh d;
	puts("Введите порядковый номер");
	scanf("%d", &d.id);
	puts("Введите номер маршрута");
	scanf("%d", &d.number);
	puts("Введите начальный пункт");
	scanf("%s", d.nap);
	puts("Введите конечный пункт");
	scanf("%s", d.kop);
	return d;
}

/*Создать дерево*/
tree* tree_org() {
	int answer;
	tree* top = NULL;
	while (1) {
		top = addtree(top, vvod_st());
		cout << "0. Закончить" << endl << "1. Продолжить ввод" << endl;
		scanf("%d", &answer);
		if (answer == 0) return top;
	}
	return top;
}
/*Просмотреть дерево*/
void prosmotr(tree* top) {
	if (top) {
		printf("| %2d | %4d | %-15s | %-15s |\n",
			top->inf.id, top->inf.number, top->inf.nap, top->inf.kop);
		prosmotr(top->left);
		prosmotr(top->right);
	}
}

/*Просмотреть дерево*/
void pechat(tree* top, char* q) {
	if (top == NULL) printf("");
	else {
		if ((strcmp(top->inf.nap, q) == 0) || (strcmp(top->inf.kop, q) == 0)) {
			printf("| %2d | %4d | %-15s | %-15s |\n",
				top->inf.id, top->inf.number, top->inf.nap, top->inf.kop);
		}
		pechat(top->left, q);
		pechat(top->right, q);
	}
}

/*Удаление узла дерева*/
tree* tree_delete(tree* top, int s) {
	if (top == NULL) return top;
	if (top->inf.number == s) {
		tree* tmp;
		if (top->right == NULL) tmp = top->left;
		else {
			tree* ptr = top->right;
			if (ptr->left == NULL) {
				ptr->left = top->left;
				tmp = ptr;
			}
			else {
				tree* pmin = ptr->left;
				while (pmin->left != NULL) {
					ptr = pmin;
					pmin = ptr->left;
				}
				ptr->left = pmin->right;
				pmin->left = top->left;
				pmin->right = top->right;
				tmp = pmin;
			}
		}
		delete top;
		return tmp;
	}
	else if (top->inf.number == s) top->left = tree_delete(top->left, s);
	else top->right = tree_delete(top->right, s);
	return top;
}
/*Поиск глубины самого правого элемента*/
int most_right(tree* top, int level) {
	if (top == NULL) return -1; // если дерево пустрое
	else if (top->right == NULL) return level;
	else {
		return most_right(top->right, level + 1);
	}
}

/*Сохранение в файл*/
void save_tree(tree* top, FILE* f) {
	if (top) {
		fwrite(&top->inf, size_marsh, 1, f);
		if (top->left != NULL) save_tree(top->left, f);
		if (top->right != NULL) save_tree(top->right, f);
	}
}
/*Чтение с файла*/
tree* read_file(FILE* f) {
	fseek(f, 0, 0);
	rewind(f);
	tree* top = NULL;
	marsh d;
	while (!feof(f)) {
		if (fread(&d, size_marsh, 1, f) != 0) {
			top = addtree(top, d);
		}
	}
	cout << "Список скопирован";
	return top;
}
/*Освобождение памяти*/
void free_memory(tree* top) {
	if (top) {
		if (top->left != NULL) free_memory(top->left);
		if (top->right != NULL) free_memory(top->right);
		delete top;
	}
}
