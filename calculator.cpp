#include<cstdlib>
#include<cstdio>
#include<windows.h>
#include<iostream>
using namespace std;
#include<cstring>
#include<cmath>



static int size = 0;


////////////////////////////////////////////////////////Node
template<class Type>
class Node
{
public:
	Type data;
	Node<Type>* next;
	Node(const Type& num, Node<Type>* link = NULL);
	Node(Node<Type>* link = NULL);
};


template<class Type>
Node<Type>::Node(const Type& num, Node<Type>* link)
{
	data = num;
	next = link;
}

template<class Type>
Node<Type>::Node(Node<Type>* link)
{
	next = link;
}





////////////////////////////////////////////////////////LinkStack
template<class Type>
class LinkStack
{
private:
	Node<Type>* top;
	int currSize;
public:
	LinkStack();
	~LinkStack();
	void Clear();
	Type Pop();
	void Push(const Type& num);
	Type getValue();
	int Length();
};



template<class Type>
LinkStack<Type>::LinkStack()
{
	top = NULL;
	currSize = size;
}

template<class Type>
LinkStack<Type>::~LinkStack()
{
	Clear();
}


template<class Type>
void LinkStack<Type>::Clear()
{
	Node<Type>* temp;
	while (top != NULL)
	{
		temp = top;
		top = top->next;
		delete temp;
	}
}


template<class Type>
Type LinkStack<Type>::Pop()
{
	if (Length() == 0)
	{
		cout << "存在错误，计算器将在3s后退出" << endl;
		Sleep(3000);
		exit(1);
	}
	else
	{
		Type temp = top->data;
		Node<Type>* removeNode = top;
		top = top->next;
		delete removeNode;
		currSize--;
		return temp;
	}

}


template<class Type>
void LinkStack<Type>::Push(const Type& num)
{
	top = new Node<Type>(num, top);
	currSize++;
}


template<class Type>
Type LinkStack<Type>::getValue()
{
	return top->data;
}



template<class Type>
int LinkStack<Type>::Length()
{
	return currSize;
}



////////////////////////////////////////////////////Calculator
class Calculator
{
private:

	LinkStack<char>* optr;
	LinkStack<double>* opnd;
public:
	Calculator();
	LinkStack<double>* getOpndstack();
	double DoOperator(double x, char op, double y);
	void GetChar(char& ch);
	bool compare_Prior(char ch);
	int Judge_kinds(char ch);
	void Run();
	bool isOperator(char& ch);
	double CharToDouble(char* address);
	void Clear();

};



double Calculator::CharToDouble(char* address)//假设数字不超过100位 
{
	int digit[100];
	double sum = 0;
	int dot = 0;
	int temp = 0;
	sum = digit[size] = *address - 48;
	while (!isOperator(*(address + size + 1)))
	{
		if (*(address + (size + 1)) == '.')
		{
			sum = 0;
			for (int i = size++; i >= 0; i--)
			{
				sum += digit[size - 1 - i] * pow(10, i);
			}
			dot = 1;
			continue;
		}
		if (dot == 0)
		{
			digit[size + 1] = *(address + (size + 1)) - 48;
			size++;
		}
		else
		{
			digit[99 + (temp--)] = *(address + (++size)) - 48;
		}
	}

	if (dot == 1)
	{
		size++;
		for (int i = temp; i<0; i++)
		{
			sum += digit[100 + i] * pow(10, i);
		}
	}
	else if (dot == 0)
	{
		sum = 0;
		for (int i = size++; i >= 0; i--)
		{
			sum += digit[size - 1 - i] * pow(10, i);
		}
	}
	return sum;
}






Calculator::Calculator()
{
	opnd = new LinkStack<double>();
	optr = new LinkStack<char>();
}




LinkStack<double>* Calculator::getOpndstack()
{
	return opnd;
}


double Calculator::DoOperator(double x, char op, double y)
{
	if(y==0&&op=='/')
	{
		cout << "除数为0，运算符不符合规范,计算器将在3s后退出";
		Sleep(3000);
		exit(-1);
	}
	double res;
	int kinds = Judge_kinds(op);
	switch (kinds)
	{
	case 1:
	{
			  res = x + y;
			  return res;
	}
	case 2:
	{
			  res = x - y;
			  return res;
	}
	case 3:
	{
			  res = x*y;
			  return res;
	}
	case 4:
	{
			  res = x / y;
			  return res;
	}
	default:
	{
			   cout << "运算符不符合规范,计算器将在3s后退出";
			   Sleep(3000);
			   exit(-1);
	}
	}
}







void Calculator::GetChar(char& ch)
{

	if (optr->Length() == 0 && ch != '=')
	{
		if (opnd->Length() == 0)
		{
			if (ch == '-' || ch=='+')
			{
				opnd->Push(0);
				optr->Push(ch);
			}
			else
			{
				optr->Push(ch);
			}
		}
		else
		{
			optr->Push(ch);
		}
	}
	else if ((ch == '-' ||ch== '+') && optr->getValue() == '(')
	{
		opnd->Push(0);
		optr->Push(ch);

	}
	else
	{
		int flag = Judge_kinds(ch);
		if (flag == 6)//遇到“=”结束运算符 
		{

			if (optr->Length() == 0)
			{
				return;
			}
			while (optr->Length() != 0)
			{
				char operators = optr->Pop();
				double y = opnd->Pop();
				double x = opnd->Pop();
				double res = DoOperator(x, operators, y);
				opnd->Push(res);
			}
		}
		else if (flag == -1)//即运算符为（，无条件入栈
		{
			optr->Push(ch);
		}
		else if (flag == 0)//即运算符为），操作数栈出两个数，和这个运算符相作用，将计算结果放入操作数栈中 
		{

			char operators = optr->Pop();
			while (operators != '(')
			{
				double y = opnd->Pop();
				double x = opnd->Pop();
				double res = DoOperator(x, operators, y);
				opnd->Push(res);
				operators = optr->Pop();
			}
		}
		else
		{
			bool prior = compare_Prior(ch);
			if (prior)
			{
				optr->Push(ch);
			}
			else
			{
				while (!prior)
				{
					double y = opnd->Pop();
					double x = opnd->Pop();
					char oper = optr->Pop();
					double res = DoOperator(x, oper, y);
					opnd->Push(res);
					prior = compare_Prior(ch);
				}
				optr->Push(ch);
			}

		}
	}
}





bool Calculator::isOperator(char& ch)
{
	switch (ch)
	{
	case'+':
	case'-':
	case'*':
	case'/':
	case'(':
	case')':
	case'=':
	{
			   return 1;
	}
	default:
	{
			   return 0;
	}
	}
}





bool Calculator::compare_Prior(char ch)
{
	if (optr->Length() == 0)
	{
		return 1;
	}
	char temp = optr->getValue();
	int a, b;
	a = Judge_kinds(temp);
	b = Judge_kinds(ch);
	if (a == -1)
	{
		return 1;
	}
	if (a <= 2 && b <= 2 || a >= 3 && b >= 3 || a >= 3 && b <= 2)//即两个运算符都是加减或者都是乘除或者前者优先级高于后者，不能进栈，返回0 
	{
		return 0;//即该运算符优先级和栈顶相比较低，不能进栈  
	}
	else
	{
		return 1;//即该运算符优先级较高，可以进栈 
	}

}


int Calculator::Judge_kinds(char ch)
{
	if (ch == '(')
	{
		return -1;
	}
	if (ch == ')')
	{
		return 0;
	}
	if (ch == '+')
	{
		return 1;
	}
	if (ch == '-')
	{
		return 2;
	}
	if (ch == '*')
	{
		return 3;
	}
	if (ch == '/')
	{
		return 4;
	}
	if (ch == '=')
	{
		return 6;
	}
}



void Calculator::Run()
{
	cout << "=" << opnd->Pop() << endl;
}

void Calculator::Clear()
{
	opnd->Clear();
	optr->Clear();
}



void MainWindow()
{
	cout << "   =========================================================================" << endl;
	cout << "   |**|          ===================Calculator=================         |**|" << endl;
	cout << "   |**|=================================================================|**|" << endl;
	cout << "   |**|                    Welcome to Simple Calculator                 |**|" << endl;
	cout << "   |**|                            You can use                          |**|" << endl;
	cout << "   |**|                            + - * / ( )                          |**|" << endl;
	cout << "   |**|                      to conduct your operation  	        |**|" << endl;
	cout << "   |**|                           end with '='                          |**|" << endl;
	cout << "   =========================================================================" << endl;
	cout << "   |**|          ===================&&&&&&=================             |**|" << endl;
	cout << "   |**|=================================================================|**|" << endl;
}

int main(void)
{
	Calculator calculator;
	char temp[100];
	while (1)
	{
		MainWindow();
		cout << "Input:";
		cin >> temp;
		int bit = strlen(temp);

		for (int i = 0, flag = 0; i<bit; i++)
		{
			i += size;
			size = 0;
			flag = calculator.isOperator(temp[i]);
			if (!flag)
			{
				double turnout = calculator.CharToDouble(&temp[i]);
				calculator.getOpndstack()->Push(turnout);
			}
			else
			{
				calculator.GetChar(temp[i]);
				size++;
			}
			size--;
		}
		calculator.Run();
		calculator.Clear();
		cout << endl << endl << "Continue? if yes,press 'y';else press 'N'" << endl;
		char  switchkey;
		while (1)
		{
			cin >> switchkey;
			switchkey = toupper(switchkey);
			if (switchkey == 'N')
			{
				exit(-1);
			}
			else if (switchkey == 'Y')
			{
				calculator.Clear();
				system("cls");
				break;
			}
			else
			{
				fflush(stdin);
				cout << "输入不符合规范，请重新选择" << endl;
			}
		}

	}
}
