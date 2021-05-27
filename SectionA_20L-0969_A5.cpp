#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;
int maxlength = 0;

int StrLength(char* str)// finds the length of a string
{
	if (str == NULL)
		return 0;

	int len = 0, count = 0;
	for (; str[len] != '\0'; len++, count++)
	{
		if (str[len] == ' ')
			count--;
	}

	if (count > maxlength)
		maxlength = len;

	return count;
}

//used to input the grid 
char** InputGrid(int rows, int cols, string input)
{
	int length = 0, count = 0;
	char* temp = new char[80];
	ifstream fin(input);

	if (!fin)
	{
		cout << endl << "File not found, returning to homescreen!" << endl;
		system("pause");
		cout << endl << endl;
		return NULL;
	}

	char** arr = new char* [rows];

	for (int i = 0; i < rows; i++)
	{
		fin.getline(temp, 80, '\n');
		length = StrLength(temp);
		count = 0;

		arr[i] = new char[length + 1];
		for (int j = 0; temp[j] != '\0'; j++)
		{
			if (temp[j] != ' ')
			{
				arr[i][count] = temp[j];
				count++;
			}
		}
		arr[i][length] = '\0';
	}
	fin.close();

	return arr;
}

//different from input words because it also inputs rows, cols and number of words on top of txt file used for search part of code
char** InputSearchList(int& numofwords, int& rows, int& cols, string input)
{
	int length = 0, count = 0;
	ifstream fin(input);

	if (!fin)
	{
		cout << endl << "File not found, returning to homescreen!" << endl;
		system("pause");
		cout << endl << endl;
		return NULL;
	}

	char* temp = new char[80];

	fin >> rows;
	fin >> cols;
	fin >> numofwords;
	fin.ignore();
	char** arr = new char* [numofwords];

	for (int i = 0; i < numofwords; i++)
	{
		fin.getline(temp, 80, '\n');
		length = StrLength(temp);
		arr[i] = new char[length + 1];
		count = 0;

		for (int j = 0; temp[j] != '\0'; j++)
		{
			if (temp[j] != ' ')
			{
				arr[i][count] = temp[j];
				count++;
			}
		}

		arr[i][length] = '\0';
	}

	fin.close();
	delete[] temp;
	return arr;
}

//used to input words from given file
char** InputWordList(int& numofwords, string inputfile)
{
	numofwords = 0;
	int length = 0, count = 0, spacecount = 0;
	ifstream fin(inputfile);

	if (!fin)
	{
		cout << endl << "File not found, returning to homescreen!" << endl;
		system("pause");
		cout << endl << endl;
		return NULL;
	}

	char* temp = new char[80];

	do
	{
		fin.getline(temp, 80, '\n');
		numofwords++;
	} while (!fin.eof());

	char** arr = new char* [numofwords];
	fin.clear();
	fin.seekg(0);

	for (int i = 0; i < numofwords; i++)
	{
		fin.getline(temp, 80);
		length = StrLength(temp);

		arr[i] = new char[length + 1];

		count = 0;
		for (int j = 0; temp[j] != '\0'; j++)
		{
			if (temp[j] != ' ')
			{
				arr[i][count] = temp[j];
				count++;
			}
		}

		arr[i][length] = '\0';
	}

	delete[] temp;
	fin.close();

	return arr;
}

//capatalizes all words making it easier to check and put in grid
void Capitalize(char** arr, int numofwords)
{
	for (int i = 0; i < numofwords; i++)
	{
		for (int j = 0; arr[i][j] != '\0'; j++)
		{
			if (arr[i][j] >= 'a' && arr[i][j] <= 'z')
				arr[i][j] = arr[i][j] - 32;
		}
	}

	return;
}

//makes grid of given size and assigns all value as \0
char** GridMaker(int numofwords, int rows, int cols)
{
	if(maxlength > rows || maxlength > cols)
	{
		cout << endl << "Grid cannot be formed, press any key to return to the home screen" << endl;
		system("pause");
		cout << endl << endl;
		return NULL;
	}

	char** Grid = new char* [rows];
	for (int i = 0; i < rows; i++)
	{
		Grid[i] = new char[cols + 1];
		Grid[i][cols] = '\0';

		for (int j = 0; j < cols; j++)
		{
			Grid[i][j] = '\0';
		}
	}

	return Grid;
}

//checks if placing a word in certain direction is possible or not
bool leftcheck(int r, int c, char* word, char** grid)
{
	int length = 0;
	length = StrLength(word);

	if (c - length + 1 < 0)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r][c - i] != '\0' && grid[r][c - i] != word[i])
			return false;

	return true;
}

bool rightcheck(int r, int c, char* word, char** grid, int cols)
{
	int length = 0;
	length = StrLength(word);

	if (c + length > cols)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r][c + i] != '\0' && grid[r][c + i] != word[i])
			return false;

	return true;
}

bool upcheck(int r, int c, char* word, char** grid)
{
	int length = 0;
	length = StrLength(word);

	if (r - length + 1 < 0)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r - i][c] != '\0' && grid[r - i][c] != word[i])
			return false;

	return true;
}

bool downcheck(int r, int c, char* word, char** grid, int rows)
{
	int length = 0;
	length = StrLength(word);

	if (r + length > rows)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r + i][c] != '\0' && grid[r + i][c] != word[i])
			return false;

	return true;
}

bool uprightcheck(int r, int c, char* word, char** grid, int cols)
{
	int length = 0;
	length = StrLength(word);

	if (r - length + 1 < 0 || c + length > cols)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r - i][c + i] != '\0' && grid[r - i][c + i] != word[i])
			return false;

	return true;
}

bool upleftcheck(int r, int c, char* word, char** grid)
{
	int length = 0;
	length = StrLength(word);

	if (r - length + 1 < 0 || c - length + 1 < 0)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r - i][c - i] != '\0' && grid[r - i][c - i] != word[i])
			return false;

	return true;
}

bool downrightcheck(int r, int c, char* word, char** grid, int rows, int cols)
{
	int length = 0;
	length = StrLength(word);

	if (r + length - 1 > rows || c + length > cols)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r + i][c + i] != '\0' && grid[r + i][c + i] != word[i])
			return false;

	return true;
}

bool downleftcheck(int r, int c, char* word, char** grid, int rows)
{
	int length = 0;
	length = StrLength(word);

	if (r + length > rows || c - length + 1 < 0)
		return false;

	for (int i = 0; i < length; i++)
		if (grid[r + i][c - i] != '\0' && grid[r + i][c - i] != word[i])
			return false;

	return true;
}

// following conditions used to place the word into grid after check comes true
void placeleft(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r][c - i] = word[i];
	}

	return;
}

void placeright(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r][c + i] = word[i];
	}

	return;
}

void placeup(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r - i][c] = word[i];
	}

	return;
}

void placedown(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r + i][c] = word[i];
	}

	return;
}

void placeupright(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r - i][c + i] = word[i];
	}

	return;
}

void placeupleft(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r - i][c - i] = word[i];
	}

	return;
}

void placedownright(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r + i][c + i] = word[i];
	}

	return;
}

void placedownleft(int r, int c, char** grid, char* word)
{
	int length = StrLength(word);

	for (int i = 0; i < length; i++)
	{
		grid[r + i][c - i] = word[i];
	}

	return;
}
 //used to put the words into the grid
void FitWords(char** wordlist, char** grid, int numofwords, int rows, int cols)
{
	bool check = false;;
	int randomrow = 0, randomcol = 0, temp = 0, count = 0;

	for (int i = 0; i < numofwords;)
	{
		randomrow = rand() % rows; //random col and row picked after which random number is used to see how to place word starting from random row and random col
		randomcol = rand() % cols;
		temp = rand() % 8;

		switch (temp)
		{
		case 0:

			check = leftcheck(randomrow, randomcol, wordlist[i], grid);
			if (check == true)
			{
				placeleft(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;

		case 1:
			check = rightcheck(randomrow, randomcol, wordlist[i], grid, cols);
			if (check == true)
			{
				placeright(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;

		case 2:
			check = upcheck(randomrow, randomcol, wordlist[i], grid);
			if (check == true)
			{
				placeup(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;

		case 3:
			check = downcheck(randomrow, randomcol, wordlist[i], grid, rows);
			if (check == true)
			{
				placedown(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;

		case 4:
			check = upleftcheck(randomrow, randomcol, wordlist[i], grid);
			if (check == true)
			{
				placeupleft(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;

		case 5:
			check = downleftcheck(randomrow, randomcol, wordlist[i], grid, rows);
			if (check == true)
			{
				placedownleft(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;

		case 6:
			check = uprightcheck(randomrow, randomcol, wordlist[i], grid, cols);
			if (check == true)
			{
				placeupright(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;

		case 7:
			check = downrightcheck(randomrow, randomcol, wordlist[i], grid, rows, cols);
			if (check == true)
			{
				placedownright(randomrow, randomcol, grid, wordlist[i]);
				i++;
				count = 0;
			}
			else
				count++;
			break;
		}

		if (count == 5000)
		{
			count = 0;
			cout << wordlist[i] << " could not be entered into grid due to an unexpeted error!" << endl;
			i++;
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j] == '\0')
				grid[i][j] = (char)((rand() % 26) + 65);
		}
	}

	return;
}

//following conditions check each specific direction a word could be and if the words is found then a true is returned
bool checkup(int len, int row, int col, char* str, char** grid, int size)
{
	if (row - len + 1 < 0)
		return false;

	for (int i = 0; i < len; i++)
		if (grid[row - i][col] != str[i])
			return false;

	return true;
}

bool checkdown(int len, int row, int col, char* str, char** grid, int size)
{

	if (row + len - 1 > size)
		return false;

	for (int i = 0; i < len; i++)
		if (grid[row + i][col] != str[i])
			return false;

	return true;
}

bool checkleft(int len, int row, int col, char* str, char** grid, int size)
{
	if (col - len + 1 < 0)
		return false;

	for (int i = 0; i < len; i++)
		if (grid[row][col - i] != str[i])
			return false;

	return true;
}

bool checkright(int len, int row, int col, char* str, char** grid, int size)
{
	for (int i = 0; i < len; i++)
		if (grid[row][col + i] != str[i])
			return false;

	return true;
}

bool checkupleft(int len, int row, int col, char* str, char** grid, int size)
{
	if (col - len + 1 < 0 || row - len + 1 < 0)
		return false;

	for (int i = 0; i < len; i++)
		if (grid[row - i][col - i] != str[i])
			return false;

	return true;
}

bool checkupright(int len, int row, int col, char* str, char** grid, int size)
{
	if (row - len + 1 < 0)
		return false;

	for (int i = 0; i < len; i++)
		if (grid[row - i][col + i] != str[i])
			return false;

	return true;
}

bool checkdownleft(int len, int row, int col, char* str, char** grid, int size)
{
	if (col - len + 1 < 0 || row + len - 1 > size)
		return false;

	for (int i = 0; i < len; i++)
		if (grid[row + i][col - i] != str[i])
			return false;

	return true;
}

bool checkdownright(int len, int row, int col, char* str, char** grid, int size)
{
	if (row + len - 1 > size)
		return false;

	for (int i = 0; i < len; i++)
		if (grid[row + i][col + i] != str[i])
			return false;

	return true;
}

//finds the word in the grid and the displays and outputs it into a file
void FindWords(char** wordlist, char** grid, int numofwords, int rows, int cols, string output)
{
	bool check = false;
	int length = 0;

	ofstream fout(output);

	for (int l = 0; l < numofwords; l++)
	{
		check = false;
		for (int i = 0; i < rows && check == false; i++)
		{
			for (int j = 0; j < cols && check == false; j++)
			{
				length = StrLength(wordlist[l]);

				if (checkup(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i - length + 1 << ", " << j << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i - length + 1 << ", " << j << "]" << endl << endl;
					check = true;
				}

				else if (checkdown(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i + length - 1 << ", " << j << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i + length - 1 << ", " << j << "]" << endl << endl;
					check = true;
				}

				else if (checkright(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i << ", " << j + length - 1 << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i << ", " << j + length - 1 << "]" << endl << endl;
					check = true;
				}

				else if (checkleft(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i << ", " << j - length + 1 << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i << ", " << j - length + 1 << "]" << endl << endl;
					check = true;
				}

				else if (checkupright(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i - length + 1 << ", " << j + length << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i - length + 1 << ", " << j + length << "]" << endl << endl;
					check = true;
				}

				else if (checkupleft(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i - length + 1 << ", " << j - length + 1 << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i - length + 1 << ", " << j - length + 1 << "]" << endl << endl;
					check = true;
				}

				else if (checkdownright(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i + length - 1 << ", " << j + length - 1 << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i + length - 1 << ", " << j + length - 1 << "]" << endl << endl;
					check = true;
				}

				else if (checkdownleft(length, i, j, wordlist[l], grid, rows))
				{
					cout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i + length - 1 << ", " << j - length + 1 << "]" << endl << endl;
					fout << wordlist[l] << "   " << "[" << i << ", " << j << "]" << " , " << "[" << i + length - 1 << ", " << j - length + 1 << "]" << endl << endl;
					check = true;
				}
			}
		}
		if (check == false)
		{
			cout << wordlist[l] << " WORD NOT FOUND" << endl << endl;
			fout << wordlist[l] << " WORD NOT FOUND" << endl << endl;
		}
	}

	fout.close();
	cout << endl << "SEARCH SUCCESSFUL!" << endl;
	system("pause");
	cout << endl << endl;
	return;
}

void SortList(char** list, const int& size) //sorts words making the biggest word come first
{
	char* temp;
	int len1, len2;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size - 1 - i; j++)
		{
			len1 = StrLength(list[j]);
			len2 = StrLength(list[j + 1]);

			if (len1 < len2)
			{
				temp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = temp;
			}
		}
	}
}

void OutputGrid(char** grid, int rows, int cols, string outputfile)//used to put the grid into a txt file
{
	if (grid == NULL)
		return;

	ofstream fout(outputfile);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			fout << grid[i][j] << ' ';

		if (i + 1 < rows) 
			fout << endl;
	}

	fout.close();

	cout << endl << "GRID SUCCESSFULLY CREATED!" << endl;
}


int main()// contains the menu for the whole program
{
	srand((unsigned)time(0));
	char choice;
	cout << "-----------------------------------------------------PROGRAM START-----------------------------------------------------" << endl << endl;
	cout << endl << endl;

	for (int i = 0; 1; i++)
	{
		char** wordlist, ** grid;
		string inputfile, outputfile, gridinput, parameterinput;
		int rows = 0, cols = 0, numofwords = 0;

	menu:

		cout << "1) Create Grid (Type C)" << endl;
		cout << "2) Search Words from Grid (Type S)" << endl;
		cout << "3) Quit (Type Q)" << endl;

		cout << "Your Choice: ";
		cin >> choice;

		cout << endl;

		switch (choice)
		{

		case 'C':
		{
			char** wordlist, **grid;
			string inputfile, outputfile;
			int rows = 0, cols = 0, numofwords = 0;

			cout << endl << "-----------------------------------------------------GRID CREATION----------------------------------------------------" << endl;

			cout << "Enter name of input file where words are stored(add .txt in the end): ";
			cin >> inputfile;

			cout << "Enter number of words in file: ";
			cin >> numofwords;

			cout << "Enter number of rows the grid should have: ";
			cin >> rows;

			cout << "Enter number of columns the grid should have: ";
			cin >> cols;

			cout << "Enter name of output file where grid is to be stored(add .txt in the end): ";
			cin >> outputfile;

			wordlist = InputWordList(numofwords, inputfile);

			if (!wordlist)//go to statemnts to take you back to start if file is not found
				goto menu;

			Capitalize(wordlist, numofwords);

			grid = GridMaker(numofwords, rows, cols);

			if (!grid)
				goto menu;

			SortList(wordlist, numofwords);

			FitWords(wordlist, grid, numofwords, rows, cols);

			OutputGrid(grid, rows, cols, outputfile);

			if (wordlist != NULL)
			{
				for (int i = 0; i < numofwords; i++)
					delete[] wordlist[i];
				delete[] wordlist;

			}

			if (grid != NULL)
			{
				for (int i = 0; i < numofwords; i++)
				{
					if (grid[i])
						delete[] grid[i];
				}

				delete[] grid;

			}

			system("pause");//used to simultate requirement of key press to continue

			break;
		}

		case 'S':

			cout << endl << "-----------------------------------------------------GRID SEARCH----------------------------------------------------" << endl;

			cout << "Enter name of input file which contains grid(add .txt in the end): ";
			cin >> gridinput;

			cout << "Enter name of file with parameters and input to be searched(add .txt in the end): ";
			cin >> parameterinput;

			cout << "Enter name where the output is to be stored(add .txt in the end): ";
			cin >> outputfile;

			wordlist = InputSearchList(numofwords, rows, cols, parameterinput);

			if (!wordlist)
				goto menu;

			grid = InputGrid(rows, cols, gridinput);

			if (!grid)
				goto menu;

			Capitalize(wordlist, numofwords);

			FindWords(wordlist, grid, numofwords, rows, cols, outputfile);

			break;

			system("pause");

		case 'Q':
		{
			cout << endl << "---------Q PRESSED, PROGRAM TERMINATED!---------" << endl;
			return 0;
			break;
		}

		default://any input other then capital s c and q will be dealt with here
		{
			cout << endl << "ERROR! INVALID INPUT! PLEASE CHOOSE FROM THREE GIVEN OPTIONS AGAIN" << endl;
			break;
		}

		}

		cout << endl << endl;

	}

}