#include "Calculator.h"


namespace NS_Calculator
{


	const string Exception::exceptionDetail[ExceptionType_End] =
	{
		//计算器类相关
		"UnknownSymbol",
		"BracketMismatching",
		"NoOperand",
		"NoOperator",
		"SyntaxError",
		"MathError",
		"ExpressionNotSet",
		"ValueOverflow",
		//控制台相关
		"UnknownCommand",
		"UnknownOption",
		"ExpectMoreOption",
		"ArgumentMismatching",
		"MutexOptionExists",
		"KeyOptionNotFound",
		//其他
		"OtherException"
	};

	Exception::Exception(const Exception::ExceptionType& _exceptionType)
		:exceptionType(_exceptionType)
	{

	}


	Exception::~Exception()
	{

	}

	const char* Exception::what()const
	{
		return exceptionDetail[this->exceptionType].c_str();
	}


































	//类的私有静态常量定义
	const int Calculator::offset = 6;
	const char* Calculator::operatorsIdentifier[Calculator::Operator_End] =
	{ " ","+","-","*","/","^","!","%","abs","log","sin","cos","tan","arcsin","arccos","arctan","cot","arr","com","(",")","a","n" };
	const int Calculator::operatorsIdentifierLength[Calculator::Operator_End] =
	{ -1,1,1,1,1,1,1,1,3,3,3,3,3,6,6,6,3,3,3,1,1,-1,-1 };
	const int Calculator::priority[Calculator::Operator_End] =
	{ 10,0,0,1,1,3,5,1,4,2,2,2,2,2,2,2,2,2,2,10,10,9,9 };//优先级
	const int Calculator::operatorsType[Calculator::Operator_End] =
	{ 0,2,2,2,2,2,1,2,1,2,1,1,1,1,1,1,1,2,2,0,0,1,1 };//操作符目数
	const Calculator::Operand_index Calculator::constantsValue[Calculator::Constant_End] =
	{ 3.1415926535897932384626,2.71828183 };//内置的两个常量
	const int Calculator::operatorsPosition[Calculator::Operator_End] =
	{ 0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,-1,-1,0,0 };//操作符位置(0代表前置,1代表后置或中置)
	const Calculator::Operand_index Calculator::angleUnitsConvertConstant = 180 / (Calculator::constantsValue[Calculator::PI]);//角单位转换常量



	Calculator::Calculator()
		: result(0)
		, expression(nullptr)
		, formatMode(Fix)
		, precision(2)
		, angleUnit(Rad)
		, displayDigitSeparator(false)
	{

	}

	Calculator::~Calculator()
	{
		if (expression != nullptr)//检查
			delete expression;//释放
	}

	Calculator::Operand_index Calculator::calculate(const std::string& _expression)//计算
	{
		try
		{
			setExpression(_expression);//设置新字符串
			_parseString();//分析字符串得到中缀表达式
			_convertToPostfix();//转换为后缀表达式
			result = _calculatePostfix();//计算结果
		}
		catch (Exception & e)
		{
			//		cout<<e.what();
			throw e;
		}

		return result;
	}

	Calculator::Operand_index Calculator::calculate()//计算
	{
		try
		{
			//		setExpression(_expression);//设置新字符串
			if (expression == nullptr)
				throw Exception(Exception::ExpressionNotSet);//抛出异常
			_parseString();//分析字符串得到中缀表达式
			_convertToPostfix();//转换为后缀表达式
			this->result = _calculatePostfix();//计算结果
		}
		catch (Exception & e)
		{
			//		cout<<e.what();
			throw e;
		}

		return result;
	}

	void Calculator::setFormat(const FormatMode& _format, const int& _precision)
	{
		this->formatMode = _format;
		this->precision = _precision;
	}

	void Calculator::setPrecision(const int& _precision)
	{
		if (_precision >= 0)
			this->precision = _precision;
	}

	void Calculator::setAngleUnit(const AngleUnit& unit)
	{
		this->angleUnit = unit;
	}

	void Calculator::setDisplayDigitSeparator(const bool& displayState)
	{
		this->displayDigitSeparator = displayState;
	}

	Calculator::Operand_index Calculator::getResult()const
	{
		return result;//返回计算结果
	}

	//获取格式化的结果字符串
	string Calculator::getFormattedResult()const
	{

		string result;//结果字符串

		stringstream strStream;

		switch (formatMode)//检查输出模式
		{
		case Fix:
			strStream << fixed << setprecision(precision);
			//		strStream<<setiosflags(ios::fixed);//相同功能
			break;
		case Sci:
			strStream << scientific << setprecision(precision);
			break;
		case Norm:
			break;
		}

		strStream << this->result;
		result = strStream.str();//获取流中内容

		if (displayDigitSeparator)//检查是否显示分隔符
		{
			int i, size = result.size();
			for (i = 0; i < size; i++)
			{
				if (result[i] == '.' || result[i] == 'e')
					break;
			}
			i -= 3;
			for (i; i > 0; i -= 3)
			{
				result.insert(i, 1, ',');//插入一个字符
			}
		}

		return result;
	}

	bool Calculator::setExpression(const string& tarExpression)//设置表达式
	{
		int length = tarExpression.size();

		try
		{
			if (expression != nullptr)//当前表达式非空
				delete expression;//删除数据
			expression = new char[length + 1];//申请内存空间
		}
		catch (exception & e)
		{
			expression = nullptr;
			throw e;//抛出异常
		}

		if (tarExpression.copy(expression, length) != length)
		{
			//复制不成功
			delete expression;//删除申请的内存
			expression = nullptr;
			return false;//错误退出
		}
		expression[length] = '\0';
		return true;
	}

	string Calculator::getInfixExpression()//获取中缀表达式
	{
		//	using namespace std;
		Expression::iterator iter, iter_end;
		string str;
		stringstream ss;
		ss << fixed << setprecision(2);
		iter = infixExpression.begin();
		iter_end = infixExpression.end();

		while (iter != iter_end)
		{
			if (iter->_operator == None)
				//			str.append(std::to_string(iter->_operand)+",");
				ss << iter->_operand << ",";
			else
			{
				ss << operatorsIdentifier[iter->_operator] << ",";
			}
			iter++;//步进
		}
		str = ss.str();
		return str;
	}

	string Calculator::getPostfixExpression()//获取中缀表达式
	{
		//	using namespace std;
		Expression::iterator iter, iter_end;
		string str;
		stringstream ss;
		ss << fixed << setprecision(2);
		iter = postfixExpression.begin();
		iter_end = postfixExpression.end();

		while (iter != iter_end)
		{
			if (iter->_operator == None)
				//			str.append(std::to_string(iter->_operand)+",");
				ss << iter->_operand << ",";
			else
			{
				ss << operatorsIdentifier[iter->_operator] << ",";
			}
			iter++;//步进
		}
		str = ss.str();
		return str;
	}




	///内部函数

	void Calculator::_jumpToNext(int& index)
	{
		char c = expression[index];
		while (c != '\0')
		{
			if ((c >= '0' && c <= '9') || c == '.')
				c = expression[++index];
			else
				break;//退出
		}
	}

	bool Calculator::_parseString()//分析字符串
	{
		int index = 0;//当前检索位置
	//    int flag=1;//,不再实时监测正负号
		bool lastIsOperator = 1;//上一个是操作符
		Node crtNode;
		Constant_index constant;//
		crtNode._operator = None;
		if (!this->infixExpression.empty())//中缀表达式非空
			this->infixExpression.clear();//清空
		int length = strlen(this->expression);

		while (index < length)//主循环
		{
			if (expression[index] == ' ')//遇到空格
			{
				index++;
				continue;//忽略
			}
			crtNode._operator = _scanOperator(index);//扫描操作符
			if (crtNode._operator == None)//未检测到操作符
			{
				//操作数
				constant = _scanConstant(index);//扫描常量
				if (constant == NotConstant)//不是常量
				{
					try
					{
						//尝试提取数值
						crtNode._operand = std::stod(expression + index);
					}
					catch (std::exception & e)
					{
						//读取数值失败
						throw Exception(Exception::UnknownSymbol);//抛出异常(未知符号)
					}
				}
				else
				{
					//检查到常量
					if (constant == Ans)
						crtNode._operand = result;
					else
						crtNode._operand = constantsValue[constant];
				}

				//没有异常

				this->infixExpression.push_back(crtNode);//节点添加到末尾
				lastIsOperator = 0;
				_jumpToNext(index);//步进
			}
			else//操作符
			{
				//操作符
				if (crtNode._operator == Minu && lastIsOperator)//读取到减号
					crtNode._operator = Neg;//转换为负号
				if (crtNode._operator == Plus && lastIsOperator)//读取到加号
					crtNode._operator = Act;//转换为正号
				this->infixExpression.push_back(crtNode);//节点添加到末尾
				if (crtNode._operator != Fact && crtNode._operator != BraR)//需要检查是不是后缀运算符
					lastIsOperator = 1;
			}
		}
		return true;//正常结束
	}

	bool Calculator::_convertToPostfix()//转换为后缀表达式
	{
		if (!this->postfixExpression.empty())//非空
			postfixExpression.clear();//清空

		bool lastIsOperand = false;//上一个是否是操作数的标记
		bool lastIsPrepositiveOperator = false;//上一个是否是前置操作符的标记

		Expression::iterator iter, iter_end;//指向节点的迭代器
		iter = infixExpression.begin();//获取起始迭代器
		iter_end = infixExpression.end();//获取末尾迭代器

		Node tempOperatorNode;//临时OP节点
		OperatorStack opStack;//操作符栈

		while (iter != iter_end)//主循环
		{
			if ((iter)->_operator == None)//类型为操作数
			{
				postfixExpression.push_back(*iter);//直接置入目标列表
				lastIsOperand = true;//标记为数值
				lastIsPrepositiveOperator = false;
			}
			else//类型为操作符
			{
				if (iter->_operator == BraR)//右括号,当做数值看待
				{
					while (true)
					{
						if (opStack.empty())//栈为空
							throw Exception(Exception::BracketMismatching);//抛出异常(括号不匹配)
						tempOperatorNode = opStack.top();//获取栈顶元素
						opStack.pop();//出栈
						if (tempOperatorNode._operator == BraL)//弹出的是左括号
							break;
						else
							postfixExpression.push_back(tempOperatorNode);//置入目标队列
					}//while
					lastIsOperand = true;//标记为数值
				}
				else if (iter->_operator == BraL)//左括号,当做数值看待
				{
					lastIsOperand = true;//标记为数值
					opStack.push(*iter);//置入栈
				}
				else//非括号
				{
					//进行语法检查

					if (!lastIsOperand && operatorsPosition[iter->_operator] == 1)
					{
						throw Exception(Exception::SyntaxError);//抛出异常(语法错误)
					}

					//语法检查结束

					if (!opStack.empty())//栈不为空
					{
						//逐个弹出栈中比将要入栈的OP优先级高(或一样高)的OP
						while ((!opStack.empty()) && priority[opStack.top()._operator] >= priority[iter->_operator] && opStack.top()._operator != iter->_operator)
						{
							tempOperatorNode = opStack.top();//获取栈顶元素
							if (tempOperatorNode._operator != BraL)
								postfixExpression.push_back(tempOperatorNode);//置入目标队列
							else
								break;
							opStack.pop();//出栈
						}//while
					}

					opStack.push(*iter);//置入栈
					if (operatorsPosition[iter->_operator] == 0)//前置运算符
					{
						//lastIsPrepositiveOperator = true;//标记为前置操作符
						lastIsOperand = false;//标记为操作符
					}
					else//其他
					{
						lastIsOperand = true;//标记为数值
					}
				}
			}
			iter++;//步进
		}//while

		//语法检查
		if (!lastIsOperand)
		{
			throw Exception(Exception::SyntaxError);//抛出异常(语法错误)
		}

		while (!opStack.empty())//将栈中剩余操作符弹出
		{
			tempOperatorNode = opStack.top();//获取栈顶元素
			if (tempOperatorNode._operator == BraL)
				throw Exception(Exception::BracketMismatching);//抛出异常(括号不匹配)
			opStack.pop();//出栈
			postfixExpression.push_back(tempOperatorNode);//置入目标队列
		}
		return true;
	}

	Calculator::Operator_index Calculator::_scanOperator(int& index)//扫描操作符,未找到返回None
	{
		//长操作符
		for (int i = 1; i < Operator_End; i++)
		{
			if (_operatorCopmare(this->expression + index, i))
				return index += operatorsIdentifierLength[i], i;
		}
		return None;//非操作符(不步进)
	}

	Calculator::Constant_index Calculator::_scanConstant(int& index)//扫描常量,步进index
	{
		if (strncmp(this->expression + index, "PI", 2) == 0)
		{
			index += 2;//步进两位
	//		return constantsValue[PI];
			return PI;
		}
		else if (expression[index] == 'e')
		{
			index++;
			//		return constantsValue[NaLo];
			return NaLo;
		}
		else if (strncmp(this->expression + index, "ANS", 2) == 0)
		{
			index += 3;
			return Ans;
		}
		else
			return NotConstant;//返回非常量标记
	}

	Calculator::Operand_index Calculator::_calculatePostfix()//计算后缀表达式结果
	{
		OperandStack operandStack;//操作数栈
		Calculator::Operand_index operand1 = 0, operand2 = 0;

		Expression::iterator iter, iter_end;
		Node crtNode;
		iter = postfixExpression.begin();//获取起始迭代器
		iter_end = postfixExpression.end();//获取末尾迭代器

		while (iter != iter_end)//主循环
		{
			crtNode = *iter;
			if (crtNode._operator == None)//若遇到操作数
			{
				//操作数
				operand1 = iter->_operand;
				operandStack.push(operand1);//操作数入栈
			}
			else//若遇到操作符
			{
				//操作符
				//根据运算符类型(单,双目)从栈中获取操作数
				if (operatorsType[crtNode._operator] == 1)//单目运算符
				{
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//抛出异常(无操作数)
					operand1 = operandStack.top();//获取栈顶元素
					operandStack.pop();//出栈
				}
				else//双目运算符
				{
					//获取第二个操作数
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//抛出异常(无操作数)
					operand2 = operandStack.top();//获取栈顶元素
					operandStack.pop();//出栈

					//获取第一个操作数
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//抛出异常(无操作数)
					operand1 = operandStack.top();//获取栈顶元素
					operandStack.pop();//出栈

				}
				try
				{
					operand1 = _getValue(operand1, operand2, crtNode._operator);//计算结果
				}
				catch (Exception & e)
				{
					//发生异常
					throw e;//抛出
				}
				operandStack.push(operand1);//操作数入栈
			}

			iter++;
		}
		if (operandStack.size() > 1)
			throw Exception(Exception::NoOperator);//抛出异常(无操作符)
		return operand1;

	}

	Calculator::Operand_index Calculator::_getValue(const Calculator::Operand_index& operand1, const Calculator::Operand_index& operand2, const Operator_index& op)
	{
		switch (op)
		{
		case Plus:
			return operand1 + operand2;
		case Minu:
			return operand1 - operand2;
		case Mult:
			return operand1 * operand2;
		case Divi:
		{
			if (operand2 == 0)//除数为0
				throw Exception(Exception::MathError);//抛出异常(数学错误)
			return operand1 / operand2;
		}
		case Pow:
			return std::pow(operand1, operand2);
		case Fact://阶乘
		{
			Calculator::Operand_index temp = ceil(operand1);//向零取整
			if (temp > 1754)//检查计算极限值
				throw Exception(Exception::ValueOverflow);//抛出异常(数值溢出)
			if (temp != operand1 || operand1 < 0)
				throw Exception(Exception::MathError);//抛出异常(数学错误)
			if (temp < 0)
				throw Exception(Exception::MathError);//抛出异常(数学错误)
			Calculator::Operand_index res = 1.0, i;
			for (i = 2.0; i <= temp; i++)
				res *= i;
			return res;
		}
		case Mod:
		{
			long long operand_1 = ceil(operand1);
			long long operand_2 = ceil(operand2);
			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2)
				throw Exception(Exception::MathError);//抛出异常(数学错误)
			return (Calculator::Operand_index)(operand_1 % operand_2);
		}
		case Log:
		{
			if (operand1 == 0)
				throw Exception(Exception::MathError);//抛出异常(数学错误)
			return std::log(operand2) / std::log(operand1);//换底公式(operand1是底数)
		}
		case Sin:
		{
			Calculator::Operand_index operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//转换为弧度
			return std::sin(operand_1);
		}
		case Cos:
		{
			Calculator::Operand_index operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//转换为弧度
			return std::cos(operand_1);
		}
		case Tan:
		{
			Calculator::Operand_index operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//转换为弧度
			if (operand1 == constantsValue[PI] / 2)//如果值等于PI/2
				throw Exception(Exception::MathError);//抛出异常(数学错误)
			return std::tan(operand_1);
		}
		case ASin:
		{
			Calculator::Operand_index res;
			if (operand1 > 1.0 || operand1 < -1.0)
				throw Exception(Exception::MathError);
			res = std::asin(operand1);
			if (angleUnit == Deg)
				res *= angleUnitsConvertConstant;
			return res;
		}
		case ACos:
		{
			Calculator::Operand_index res;
			if (operand1 > 1.0 || operand1 < -1.0)
				throw Exception(Exception::MathError);
			res = std::acos(operand1);
			if (angleUnit == Deg)
				res *= angleUnitsConvertConstant;
			return res;
		}
		case ATan:
		{
			Calculator::Operand_index res;
			res = std::atan(operand1);
			if (angleUnit == Deg)
				res *= angleUnitsConvertConstant;
			return res;
		}
		case Cot:
		{
			Calculator::Operand_index operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//转换为弧度
			if (operand1 == 0)//如果值等于0
				throw Exception(Exception::MathError);//抛出异常(数学错误)
			return 1.0 / std::tan(operand_1);
		}
		case Arra://排列数
		{
			Calculator::Operand_index operand_1 = ceil(operand1);
			Calculator::Operand_index operand_2 = ceil(operand2);

			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2)
				throw Exception(Exception::MathError);//抛出异常(数学错误)

			Calculator::Operand_index temp = operand_1, res = 1, count = 0;

			for (; count < operand_2; count++)
			{
				res *= temp;
				temp -= 1.0;
			}
			return res;
		}
		case Comb:
		{
			Calculator::Operand_index operand_1 = ceil(operand1);
			Calculator::Operand_index operand_2 = ceil(operand2);
			Calculator::Operand_index temp = operand_1, res = 1, count = 0, fact = 1.0;
			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2)
				throw Exception(Exception::MathError);//抛出异常(数学错误)

			for (; count < operand_2; count++)
			{
				res *= temp;
				temp -= 1.0;
			}
			temp = operand_2;
			for (Calculator::Operand_index i = 2.0; i <= temp; i++)
				fact *= i;
			res /= fact;
			return res;
		}
		case Act:
			return operand1;
		case Neg:
			return -operand1;
		case Abs:
			return operand1 < 0 ? -operand1 : operand1;
		default:
			return 0;
		}
	}

	bool Calculator::_operatorCopmare(const char* str, const Operator_index& op)noexcept
	{
		int i;
		char c;
		bool res = false;
		int len = operatorsIdentifierLength[op];

		for (i = 0; i < len; i++)
		{
			if (str[i] == '\0')//结束
			{
				break;
			}
			c = operatorsIdentifier[op][i];
			if (str[i] == c || str[i] == c - 32)//不区分大小写
				continue;
			else
			{
				break;
			}
		}
		if (i == len)
			return true;
		else
			return false;
	}


}



