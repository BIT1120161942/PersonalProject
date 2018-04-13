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
	if (argc != 3)				//������Ŀ���㣬��ʾ�쳣
	{
		cout << "Missing Parameter" << endl;
		return 0;
	}

	FILE *sudoku;
	int FinalSudoku[10][10] = { 0 };

	sudoku = fopen("sudoku.txt", "w");
	/*���������վ�*/
	if (!strcmp(argv[1], "-c"))
	{
		int ShuDuNumber = GetSudokuNumber(argv[2]);
		if (ShuDuNumber < 1 || ShuDuNumber>1000000)			//�����������ַ��򳬳���Χ������
		{
			cout << "Num Error" << endl;
			return 0;
		}
		srand((unsigned)time(NULL));
		while (ShuDuNumber > 0)
		{
			memset(FinalSudoku, 0, sizeof(FinalSudoku));	//������������0��
			GetRand(FinalSudoku, 0, 0);			//�������޹صľŹ��������������
			GetRand(FinalSudoku, 1, 2);
			GetRand(FinalSudoku, 2, 1);
			for (int i = 0; i<3; i++)		//��Ҫ�������һ���Ź���ʱ��һ����Ϊ��4+2��%9+1
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
	/*������*/
	else if (!strcmp(argv[1], "-s"))
	{
		FILE *puzzle;
		if ((puzzle = fopen(argv[2], "r")) == NULL)								//δ���ļ�������
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
			if (line == 8 && rol == 8)							//���Ѷ���һ������������������
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
	/*δ�����������*/
	else
	{
		cout << "Parameter Error" << endl;					//����δ�������
	}

	return 0;
}
void GetRand(int FinalSudoku[][10], int line, int rol)				
{
	//��ĳ���Ź��������������
	//line rol��ʾ��Ӧ�����ڵ��к���
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
	/*type==0�վ������������ļ���type==1�����վֽ��������Խ��߶ԳƱ任������*/
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
void GetFinalSudoku(int FinalSudoku[][10], int &num, int line, int rol, FILE*sudoku)		//��ȡһ�������վ�
{
	/*num���������վ���Ŀ��num<=0ʱ���ٽ����վ����ɲ�����������num����Ϊ1*/
	if (num <= 0)
		return;
	int nextline, nextrol;
	nextrol = rol + 1;
	nextline = line + nextrol / 9;
	nextrol %= 9;
	if (FinalSudoku[line][rol] != 0)	//���������������ֱ�ӽ�����һ����������ļ�
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
			if (FinalSudoku[line][i] != 0)					//��ͬ�в�ͬ
				flag[FinalSudoku[line][i]] = 1;
			if (FinalSudoku[i][rol] != 0)						//��ͬ�в�ͬ
				flag[FinalSudoku[i][rol]] = 1;
		}
		int LineRegion = line / 3, RolRegion = rol / 3;			//��ͬ����ͬ
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
	/*�����ѻ�õ������վ�ͨ��һ���任���ٵõ�����վ֣���num����������num<=0ʱ���ٽ���
	lineorder��ʾ�任���������˳��*/
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