#include<iostream>
#include<fstream>
#include<algorithm>
#include<cstring>
#include<time.h>
using namespace std;

int GetSudokuNumber(char * p);
void GetFinalSudoku(int FinalSudoku[][10], int &num, int line, int rol, FILE*sudoku);
void GetRand(int FinalSudoku[][10], int line, int rol);
void WriteIntoFile(int FinalSudoku[][10], FILE*sudoku, int type, int &num, int*lineorder);
void ChangeFinalSugouku(int FinalSudoku[][10], int &num, FILE*sudoku);
int main(int argc, char * argv[])
{
	if (argc != 3)				//参数数目不足，显示异常
	{
		cout << "Missing Parameter" << endl;
		return 0;
	}

	FILE *sudoku;
	int FinalSudoku[10][10] = { 0 };

	sudoku = fopen("sudoku.txt", "w");
	/*生成数独终局*/
	if (!strcmp(argv[1], "-c"))
	{
		int ShuDuNumber = GetSudokuNumber(argv[2]);
		if (ShuDuNumber < 1 || ShuDuNumber>1000000)			//包含非数字字符或超出范围，报错
		{
			cout << "Num Error" << endl;
			return 0;
		}
		srand((unsigned)time(NULL));
		while (ShuDuNumber > 0)
		{
			memset(FinalSudoku, 0, sizeof(FinalSudoku));	//将数独数组置0；
			GetRand(FinalSudoku, 0, 0);			//将三个无关的九宫格用随机数填满
			GetRand(FinalSudoku, 1, 2);
			GetRand(FinalSudoku, 2, 1);
			for (int i = 0; i<3; i++)		//按要求调整第一个九宫格时第一个数为（4+2）%9+1
				for (int j = 0; j < 3; j++)
				{
					if (FinalSudoku[i][j] == 7)
						FinalSudoku[i][j] = FinalSudoku[0][0];
				}
			FinalSudoku[0][0] = 7;
			GetFinalSudoku(FinalSudoku, ShuDuNumber, 0, 0, sudoku);
		}
		fclose(sudoku);
	}
	/*解数独*/
	else if (!strcmp(argv[1], "-s"))
	{
		FILE *puzzle;
		if ((puzzle = fopen(argv[2], "r")) == NULL)								//未打开文件，报错
		{
			cout << "Can't Open the File" << endl;
			return 0;
		}
		int line = 0, rol = 0;
		char ch;
		while ( (ch=fgetc(puzzle))!= EOF)
		{
			if (ch>='0'&&ch<='9')
			{
				FinalSudoku[line][rol] = ch - '0';
			}
			if (line == 8 && rol == 8)							//若已读入一个完整数独，则处理它
			{
				int num = 1;
				GetFinalSudoku(FinalSudoku, num, 0, 0, sudoku);
				line = rol = 0;
			}
			else
			{
				rol++;
				line = line + rol / 9;
				rol %= 9;
			}
		}
		fclose(puzzle);
		fclose(sudoku);
	}
	/*未定义情况报错*/
	else
	{
		cout << "Parameter Error" << endl;					//输入未定义参数
	}

	return 0;
}
void GetRand(int FinalSudoku[][10], int line, int rol)				
{
	//将某个九宫格用随机数填满
	//line rol表示对应宫所在的行和列
	int flag[10] = { 0 }, num;
	for (int i = 3 * line; i <= 3 * line + 2; i++)
	{
		for (int j = 3 * rol; j <= 3 * rol + 2; j++)
		{
			num = rand() % 9 + 1;
			while (flag[num] != 0)
				num = num % 9 + 1;
			flag[num] = 1;
			FinalSudoku[i][j] = num;
		}
	}
}
int GetSudokuNumber(char * p)
{
	int number = 0;
	for (int i = 0; p[i] != '\0'; i++)
	{
		if (!(p[i] >= '0'&&p[i] <= '9'))
		{
			return -1;
		}
		number = number * 10 + p[i] - '0';
	}
	return number;
}
void WriteIntoFile(int FinalSudoku[][10], FILE*sudoku, int type, int &num, int*lineorder)
{
	/*type==0终局正常输入至文件，type==1表明终局将关于主对角线对称变换后输入*/
	char line[170];
	int pos = 0;
	if (num <= 0)
		return;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (type == 0)
				line[pos++] = FinalSudoku[lineorder[i]][j] + '0';
			else
				line[pos++] = FinalSudoku[j][lineorder[i]] + '0';
			if (j<8)
				line[pos++] = ' ';
		}
		line[pos++] = '\n';
	}
	line[pos++] = '\n';
	line[pos++] = '\0';
	fputs(line, sudoku);
	num--;
}
void GetFinalSudoku(int FinalSudoku[][10], int &num, int line, int rol, FILE*sudoku)		//获取一个数独终局
{
	/*num控制生成终局数目，num<=0时不再进行终局生成操作，解数独num控制为1*/
	if (num <= 0)
		return;
	int nextline, nextrol;
	nextrol = rol + 1;
	nextline = line + nextrol / 9;
	nextrol %= 9;
	if (FinalSudoku[line][rol] != 0)	//若这格已有数字则直接进行下一个或输出至文件
	{
		if (line == 8 && rol == 8)
		{
			ChangeFinalSugouku(FinalSudoku, num, sudoku);
		}
		else
			GetFinalSudoku(FinalSudoku, num, nextline, nextrol, sudoku);
	}
	else
	{
		int flag[10] = { 0 };
		for (int i = 0; i < 9; i++)
		{
			if (FinalSudoku[line][i] != 0)					//与同行不同
				flag[FinalSudoku[line][i]] = 1;
			if (FinalSudoku[i][rol] != 0)						//与同列不同
				flag[FinalSudoku[i][rol]] = 1;
		}
		int LineRegion = line / 3, RolRegion = rol / 3;			//与同宫不同
		for (int i = 3 * LineRegion; i <= 3 * LineRegion + 2; i++)
		{
			for (int j = 3 * RolRegion; j <= 3 * RolRegion + 2; j++)
			{
				if (FinalSudoku[i][j] != 0)
					flag[FinalSudoku[i][j]] = 1;
			}
		}
		for (int i = 1; i < 10; i++)
		{
			if (flag[i] == 0)
			{
				FinalSudoku[line][rol] = i;
				if (line == 8 && rol == 8)
				{
					ChangeFinalSugouku(FinalSudoku, num, sudoku);
				}
				else
					GetFinalSudoku(FinalSudoku, num, nextline, nextrol, sudoku);
				FinalSudoku[line][rol] = 0;
			}
		}
	}
}
void ChangeFinalSugouku(int FinalSudoku[][10], int &num, FILE*sudoku)
{
	/*根据已获得的数独终局通过一定变换快速得到相关终局，用num控制数量，num<=0时不再进行
	lineorder表示变换后的行排列顺序*/
	if (num <= 0)
		return;
	int lineorder[9] = { 0,1,2,3,4,5,6,7,8 };
	do {
		do {
			WriteIntoFile(FinalSudoku, sudoku, 0, num, lineorder);
			WriteIntoFile(FinalSudoku, sudoku, 1, num, lineorder);
		} while (next_permutation(lineorder + 6, lineorder + 9) && num);
		lineorder[6] = 6, lineorder[7] = 7, lineorder[8] = 8;
	} while (next_permutation(lineorder + 3, lineorder + 6) && num);
}