#include "stdafx.h"	
#include <iostream>	
#include <time.h>	
#include <windows.h>	
#include <conio.h>	

using namespace std;	
	
// N - розмір поля; M - кількість мін	
#define N 10	
#define M 10		
	
int matrix[N][N];		
bool open[N][N];				
	
// перевіряє на наявність міни ; вихід за межі таблиці видає false	
bool mine(int i, int j){	
	if ((i >= 0) && (i<N)){	
		if ((j >= 0) && (j<N)){	
			if (matrix[i][j] == -1) return true;	
		}	
	}	
	return false;	
}	
	
// перевіряє на пустоту (true), вихід за межі таблиці видає  false	
bool empty(int i, int j){	
	if ((i >= 0) && (i<N)){	
		if ((j >= 0) && (j<N)){	
			if (matrix[i][j] == 0) return true;	
		}	
	}	
	return false;		
}		
		
// рекурсивна функція, яка відкриває точку попадання	
void clean(int i, int j){	
	// перевіремо на те що ми в масиві	
	if ((i >= 0) && (i<N)){	
		if ((j >= 0) && (j<N)){	
			// перевіримо чи не було це поле раніше відкрите	
			if (!open[i][j]){	
				// відкриваємо	
				open[i][j] = true;	
				// якщо поле пусте (=0), просто покриваємо всіх його сусідів	
				if (matrix[i][j] == 0){	
					clean(i - 1, j - 1);	
					clean(i - 1, j);	
					clean(i - 1, j + 1);	
					clean(i, j - 1);	
					clean(i, j + 1);	
					clean(i + 1, j - 1);	
					clean(i + 1, j);	
					clean(i + 1, j + 1);	
				}	
				// якщо не пусте (!=0) відкриваємо тільки пусті (=0) сусіди	
				else{	
					if (empty(i - 1, j - 1)) clean(i - 1, j - 1);	
					if (empty(i - 1, j)) clean(i - 1, j);	
					if (empty(i - 1, j + 1)) clean(i - 1, j + 1);	
					if (empty(i, j - 1)) clean(i, j - 1);	
					if (empty(i, j + 1)) clean(i, j + 1);	
					if (empty(i + 1, j - 1)) clean(i + 1, j - 1);	
					if (empty(i + 1, j)) clean(i + 1, j);	
					if (empty(i + 1, j + 1)) clean(i + 1, j + 1);	
				}	
			}	
		}	
	}	
}	
	
// малюєио "міну" - зірочкою червоного кольору	
void coutmine(HANDLE hConsole){	
	SetConsoleTextAttribute(hConsole, 12);  // red text	
	cout << "* ";	
	SetConsoleTextAttribute(hConsole, 7);   // white text	
}	
	
// рисует минное поле с учетом открытых и закрытых полей	
// и вспомогательные оси	
void draw_matrix(HANDLE hConsole){	
	SetConsoleTextAttribute(hConsole, 6);  // dark yellow text	
	cout << "  A B C D E F G H I J\n";	
	SetConsoleTextAttribute(hConsole, 7);  // white text	
	for (int x = 0; x<N; x++){	
		SetConsoleTextAttribute(hConsole, 6);  // dark yellow text	
		cout << x << " ";	
		SetConsoleTextAttribute(hConsole, 7);  // white text	
		for (int y = 0; y<N; y++){	
			if (open[x][y]){	
				SetConsoleTextAttribute(hConsole, 8);  // gray text	
				if (matrix[x][y] == -1) coutmine(hConsole);	
				else if (matrix[x][y] == 0) cout << ". ";	
				else cout << matrix[x][y] << " ";	
				SetConsoleTextAttribute(hConsole, 7);  // white text	
			}	
			else{	
				
				cout << "# ";	
					
			}	
		}	
		cout << "\n";	
	}	
}	
	
// функция завершает игру, выведя одну из двух надписей "Loser!" или "Winner!"
void fin(HANDLE hConsole, bool loser){	
	COORD coord;	
	coord.X = 33;	
	coord.Y = 10;	
		system("cls");
		draw_matrix(hConsole);
	SetConsoleCursorPosition(hConsole, coord);
	if (loser){
		SetConsoleTextAttribute(hConsole, 12);  // red text
		cout << "L O S E R ! ! !";
			SetConsoleTextAttribute(hConsole, 7);  // white text
	}
		else{
		SetConsoleTextAttribute(hConsole, 10);  // red text
		cout << "W I N N E R ! ! !";
		SetConsoleTextAttribute(hConsole, 7);  // white text
	}
	cout << "\n\n\n\n";
	_getch();
}

// в случае проигрыша эта функция откроет все мины
void openmines(){
	for (int i = 0; i<N; i++){
		for (int j = 0; j<N; j++){
			if (matrix[i][j] == -1) open[i][j] = true;
		}
	}
}

// проверяет, все ли поле открыто, кроме мин (таково условие победы =)
bool checkwin(){
	for (int x = 0; x<N; x++){
		for (int y = 0; y<N; y++){
			if ((matrix[x][y] != -1) && (!open[x][y])) return false;
		
		}

	}

	return true;
}	
	
	
int _tmain(int argc, _TCHAR* argv[])
{
	int i, j, k = 0;
	char s[3];
	// хэндл экна необходим для рисования цветного текста
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// инициализация
	setlocale(0, "rus");
	srand((int)time(NULL));
	
	// все чистим
	for (int c = 0; c<100; c++){ matrix[c / 10][c % 10] = 0; open[c / 10][c % 10] = false; }
	// заполняем массив поля минами
	for (int c = 0; c<M; c++){
		do{
			i = rand() % N; j = rand() % N;
		} while (matrix[i][j] != 0);
		matrix[i][j] = -1;
	}
	// заполняем массив поля цифрами
	for (i = 0; i<N; i++){
		for (j = 0; j<N; j++){
			if (matrix[i][j] != -1){
				k = 0;
				if (mine(i - 1, j - 1)) k++;
				if (mine(i - 1, j)) k++;
				if (mine(i - 1, j + 1)) k++;
				if (mine(i, j - 1)) k++;
				if (mine(i, j + 1)) k++;
				if (mine(i + 1, j - 1)) k++;
				if (mine(i + 1, j)) k++;
				if (mine(i + 1, j + 1)) k++;
				matrix[i][j] = k;
			}
		}
	}
		
	// главный игровой цикл
	while (true){
		// чистим экран от старого рисунка
		system("cls");
		// рисуем поле
		draw_matrix(hConsole);
		cout << "\n";
		// запрашиваем координаты удара
		cout << "Введите координаты удара (C3): ";
		cin >> s;
		// переводим координаты в цифровой вид
		if ((s[0] >= 65) && (s[0] <= 74)) j = s[0] - 65;        // буква в промежутке от A до J
		else if ((s[0] >= 97) && (s[0] <= 106)) j = s[0] - 97;  // буква в промежутке от a до j
		else continue;                              // если введены неверные значения, возврат в начало цикла
		if ((s[1] >= 48) && (s[1] <= 57)) i = s[1] - 48;
		else continue;
		// далее проверяем все восемь окрестных полей на пустые клетки
		// и если надо показываем некий кусок поля (ну, много пустых клеток, например)
		clean(i, j);
		
		if (mine(i, j)){ openmines(); fin(hConsole, true); break; }  // программа покидает цикл в случае проигрыша
		if (checkwin()){ fin(hConsole, false); break; }                 // или победы
	}
	
	system("PAUSE")
	;
	return 0;

}