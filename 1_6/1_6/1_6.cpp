`//Гребнева Дарья, ПС-21, задача 1.6, среда разработки Visual Studio 2017

/*Имеется N костей игры домино.На каждой кости имеется два числа(каждое от 0 до 6).
Требуется написать программу, которая будет определять максимальное число, составленное из
цифр на костях цепочки, составленной по правилам домино из имеющихся костей.

Ввод.Первая строка входного файла содержит целое число N – количество костей(2  N  20).
Следующие N строк содержат два целых числа X и Y(0  X, Y  6), разделенные пробелом.

Вывод.В выходной файл необходимо вывести максимальное целое число, цифры которого
соответствуют значению костей в цепочке. */

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int START_DIG = -1;

struct Domino
{
	int firstNumber;
	int secondNumber;
	bool isExists = false;
};

//Проверка состоит ли строка из нулей
bool IsZeroString(string str)
{
	int checkZer = 0;
	for (int i = 0; i < str.size(); ++i)
	{
		if (str[i] == '0')
		{
			++checkZer;
		}
	}
	if (checkZer == str.size())
	{
		return true;
	}
	return false;
}

//Объединение чисел с одной доминошки в строку
string MergeNumbersToString(int firstNum, int secondNum)
{
	return (to_string(firstNum) + to_string(secondNum));
}

//Сравнение длин получившихся строк
bool CompareStrings(string  s1, string s2)
{
	//Проверка на строку, состоящую из нулей
	if (s1 != "")
	{
		if (IsZeroString(s1) && (!IsZeroString(s2)))
		{
			return true;
		}
		if ((!IsZeroString(s1)) && (IsZeroString(s2)))
		{
			return false;
		}
	}
	if (s1.size() < s2.size())
	{
		return true;
	}
	else if (s1.size() > s2.size())
	{
		return false;
	}
	else if (s1 < s2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Проверка на соединение доминошек
bool CanConnectDomino(int & num, int & prevDig)
{
	if ((num == prevDig) || (prevDig == START_DIG))
	{
		return true;
	}
	return false;
}

void ChangeResultStr(string & resultStr, string & currStr)
{
	//если строка resultStr меньше чем currStr - текущая, то в строку результата записываем текущую 
	if (CompareStrings(resultStr, currStr))
	{
		resultStr = currStr;
	}
}

//Рекурсия по доминошкам
string FindMaxDominos(vector<Domino> & dominos, int prevDig, string & currStr)
{
	string resultStr;
	bool wasThatDomino = false;
	
	for (Domino & domino : dominos)
	{
		wasThatDomino = false;
		if (domino.isExists)
		{
			wasThatDomino = true;
		}

		if (!wasThatDomino)
		{
			if (CanConnectDomino(domino.firstNumber, prevDig))
			{
				string currStr = MergeNumbersToString(domino.firstNumber, domino.secondNumber);
				domino.isExists = true;
				currStr += FindMaxDominos(dominos, domino.secondNumber, currStr);
				domino.isExists = false;
				ChangeResultStr(resultStr, currStr);
			}
			if (CanConnectDomino(domino.secondNumber, prevDig))
			{
				string currStr = MergeNumbersToString(domino.secondNumber, domino.firstNumber);
				domino.isExists = true;
				currStr += FindMaxDominos(dominos, domino.firstNumber, currStr);
				domino.isExists = false;
				ChangeResultStr(resultStr, currStr);
			}
		}
	}

	return resultStr;
}

int main()
{
	ifstream fin("input.txt");

	std::ofstream output("output.txt");

	Domino domino;
	vector<Domino> vectorDominos;

	if (fin.is_open()) 
	{

		int N;
		fin >> N;

		//проверка условия 2 <= N <= 20
		if ((N < 2) || (N > 20))
		{ 
			output << "Your number of dominos must be in the range 2 <= N <= 20" << endl;
			return 1;
		}

		for (int i = 0; i < N; ++i)
		{
			if (!fin.eof())
			{
				int x;
				int y;
				fin >> x >> y;

				//Проверка вхождения в диапазон от 0 до 6 чисел на домино
				if (((x < 0) || (x > 6)) || ((y < 0) || (y > 6)))
				{
					output << "Your number of digits of dominos must be 0 <= x <= 6" << endl;
					return 1;
				}

				//Добавляем в вектор доминошек все доминошки
				domino.firstNumber = x;
				domino.secondNumber = y;
				vectorDominos.push_back(domino);
			}
		}

		//Если указанное число доминошек не равно числу перечисленных
		if (vectorDominos.size() < N) 
		{			
			output << "Your number of dominos is not equal to the Domino number listed" << endl;
			return 1;
		}
	}
	else 
	{
		output << "File cannot be found or opened" << endl;
		return 1;
	}

	string start = "";
	string result = FindMaxDominos(vectorDominos, START_DIG, start);

	//Если среди доминошек были одни нули
	if (IsZeroString(result))
	{
		result = "0";
	}

	output << result << endl;
	return 0;
}