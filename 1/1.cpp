#include "pch.h"
#include <iostream>
#include <string.h>
#include<iomanip>
using namespace std;
//判断字符函数的声明，函数写在了后面 
int j_num(char ch);	  //判断数字 
int j_alpha(char l);  //判断字母 
int j_key(char sign[]); //判断保留字 

void deal_alpha(FILE *fp, char now) //处理字母 
{
	char alpha[15] = { '\0' };  //初始化数组
	char cur;   //当前字符 
	int i = 0, j;

	alpha[i] = now;  //存入数组
	i++;
	cur = fgetc(fp);  //获取字符，随后指针fp指向下一个字符 

	while (j_alpha(cur))  //找出单词,存入字符数组 
	{
		alpha[i] = cur;  //存入数组 
		i++;
		cur = fgetc(fp);  //再获取一个字符 
	}                     //直到字符不是数字或字符 

	fseek(fp, -1, 1);     //回退一个字符，回退指针 
	j = j_key(alpha);       //判断单词是保留字还是字母 
	if (j == 1)            //如果是保留字,1 
		cout << "(1," << setw(3) << alpha << ")\n";

	else  //如果是字母,2
		cout << "(2," << setw(3) << alpha << ")\n";
}

void deal_num(FILE *fp, char now) //处理数字 
{
	char num[10] = { '\0' }; //初始化数组
	char cur_num;
	int i = 0;
	num[i] = now;
	i++;
	cur_num = fgetc(fp);

	while (j_num(cur_num))  //找到完整的数字，比如12 
	{
		num[i] = cur_num;
		i++;
		cur_num = fgetc(fp);
	}
	fseek(fp, -1, 1);              //回退字符，回退指针 
	cout << "(3," << setw(3) << num << ")\n"; //数字,3 
}

void deal_symbol(FILE *fp, char now)  //处理符号 
{
	char sym[5] = { '\0' }; //初始化数组
	char cur;
	int i = 1;
	sym[0] = now;
	switch (now)
	{
	case'(':  //分隔符,5
	case')':
	case'{':
	case'}':
	case',':
	case';':
	case'[':
	case']':
	case'"':
		cout << "(5," << setw(3) << sym << ")\n";
		break;
		//运算符,4 
	case'*':
	case'/':
	case'!':
	case'=':
		cur = fgetc(fp);
		sym[i] = cur;
		if (cur == '=')  //!=,==,/=,*= 
			cout << "(4," << setw(3) << sym << ")\n";

		else
		{
			fseek(fp, -1, 1);
			cout << "(4," << setw(3) << sym[0] << ")\n";
		}
		break;
	case'+':
		cur = fgetc(fp);
		sym[i] = cur;
		if (cur == '+')  //++
			cout << "(4," << setw(3) << sym << ")\n";

		else   //+
		{
			fseek(fp, -1, 1);  //回退字符 
			cout << "(4," << setw(3) << sym[0] << ")\n";
		}
		break;

	case'-':
		cur = fgetc(fp);
		sym[i] = cur;
		if (cur == '-')   //--
			cout << "(4," << setw(3) << sym << ")\n";


		else //-
		{
			fseek(fp, -1, 1); //退字符 
			cout << "(4," << setw(3) << sym[0] << ")\n";
		}
		break;



	case'<':
		cur = fgetc(fp);
		sym[i] = cur;
		if (cur == '=')  //<=
			cout << "(4," << setw(3) << sym << ")\n";

		else if (cur == '<') //<<
		{
			cout << "(4," << setw(3) << sym << ")\n";
		}

		else  //<
		{
			fseek(fp, -1, 1); //退字符 
			cout << "(4," << setw(3) << sym[0] << ")\n";
		}
		break;

	case'>':
		cur = fgetc(fp);
		sym[i] = cur;
		if (cur == '=')  //>=
			cout << "(4," << setw(3) << sym << ")\n";

		else if (cur == '>') //>>
		{
			cout << "(4," << setw(3) << sym << ")\n";
		}

		else   //>
		{
			fseek(fp, -1, 1); //退字符 
			cout << "(4," << setw(3) << sym[0] << ")\n";
		}
		break;

	default:  //无法识别 
		cout << "无法识别该字符" << "\n";
		break;
	}
}

int j_alpha(char l)   //判断是否为字母
{
	if (((l >= 'A') && (l <= 'Z')) || ((l >= 'a') && (l <= 'z')))  //如果是字母 
		return 1;
	else
		return 0;
}

int j_key(char sign[]) { //判断保留字 
	int i;
	char key[][15] = { "main","const","if","else","while","int","for","return","void","double","float","char","printf","scanf" };  //设置的保留字，1

	for (i = 0; i <= 13; i++)
	{
		if ((strcmp(key[i], sign)) == 0) //将单词与保留字作对比 
		{
			return 1;
		}
	}
	return 0;
}
int j_num(char ch)   //判断是否为数字
{
	if (ch >= '0'&&ch <= '9')
		return 1;
	else
		return 0;
}

void scan(FILE *fp)   //扫描符号
{
	char cur;   //当前字符 
	cur = fgetc(fp);        //获取字符，指针fp并自动指向下一个字符
	if (j_num(cur))    //如果字符是数字          
		deal_num(fp, cur);

	else if (j_alpha(cur))   //如果字符是字母        
		deal_alpha(fp, cur);

	else         //字符是符号 
		deal_symbol(fp, cur);
}


int main()
{
	FILE *fp;
	char ch;

	if ((fp = fopen("C:\\test\\in.txt", "r")) != NULL)  //读取文件内容
		cout << "文件打开成功!" << "\n";
	else
	{
		cout << "文件无法打开" << "\n";
	}
	cout << "文法分析如下" << "\n";
	while (1)
	{
		ch = fgetc(fp);
		if (ch == EOF) //如果到了文件末尾，则结束分析 
			break;
		else if (ch == '\n' || ch == ' ' || ch == '\t') //忽略空格，空白，和换行
			continue;

		else 
		{
			fseek(fp, -1, 1);   //回退字符到开头，开始识别文法 
			scan(fp);
		}
	}
	fclose(fp); //关闭文件 
	return 0;
}
