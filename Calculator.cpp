#include "Calculator.h"


namespace NS_Calculator
{


	const string Exception::exceptionDetail[ExceptionType_End] =
	{
		//�����������
		"UnknownSymbol",
		"BracketMismatching",
		"NoOperand",
		"NoOperator",
		"SyntaxError",
		"MathError",
		"ExpressionNotSet",
		"ValueOverflow",
		//����̨���
		"UnknownCommand",
		"UnknownOption",
		"ExpectMoreOption",
		"ArgumentMismatching",
		"MutexOptionExists",
		"KeyOptionNotFound",
		//����
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


































	//���˽�о�̬��������
	const int Calculator::offset = 6;
	const char* Calculator::operatorsIdentifier[Calculator::Operator_End] =
	{ " ","+","-","*","/","^","!","%","abs","log","sin","cos","tan","arcsin","arccos","arctan","cot","arr","com","(",")","a","n" };
	const int Calculator::operatorsIdentifierLength[Calculator::Operator_End] =
	{ -1,1,1,1,1,1,1,1,3,3,3,3,3,6,6,6,3,3,3,1,1,-1,-1 };
	const int Calculator::priority[Calculator::Operator_End] =
	{ 10,0,0,1,1,3,5,1,4,2,2,2,2,2,2,2,2,2,2,10,10,9,9 };//���ȼ�
	const int Calculator::operatorsType[Calculator::Operator_End] =
	{ 0,2,2,2,2,2,1,2,1,2,1,1,1,1,1,1,1,2,2,0,0,1,1 };//������Ŀ��
	const Calculator::Operand_index Calculator::constantsValue[Calculator::Constant_End] =
	{ 3.1415926535897932384626,2.71828183 };//���õ���������
	const int Calculator::operatorsPosition[Calculator::Operator_End] =
	{ 0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,-1,-1,0,0 };//������λ��(0����ǰ��,1������û�����)
	const Calculator::Operand_index Calculator::angleUnitsConvertConstant = 180 / (Calculator::constantsValue[Calculator::PI]);//�ǵ�λת������



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
		if (expression != nullptr)//���
			delete expression;//�ͷ�
	}

	Calculator::Operand_index Calculator::calculate(const std::string& _expression)//����
	{
		try
		{
			setExpression(_expression);//�������ַ���
			_parseString();//�����ַ����õ���׺���ʽ
			_convertToPostfix();//ת��Ϊ��׺���ʽ
			result = _calculatePostfix();//������
		}
		catch (Exception & e)
		{
			//		cout<<e.what();
			throw e;
		}

		return result;
	}

	Calculator::Operand_index Calculator::calculate()//����
	{
		try
		{
			//		setExpression(_expression);//�������ַ���
			if (expression == nullptr)
				throw Exception(Exception::ExpressionNotSet);//�׳��쳣
			_parseString();//�����ַ����õ���׺���ʽ
			_convertToPostfix();//ת��Ϊ��׺���ʽ
			this->result = _calculatePostfix();//������
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
		return result;//���ؼ�����
	}

	//��ȡ��ʽ���Ľ���ַ���
	string Calculator::getFormattedResult()const
	{

		string result;//����ַ���

		stringstream strStream;

		switch (formatMode)//������ģʽ
		{
		case Fix:
			strStream << fixed << setprecision(precision);
			//		strStream<<setiosflags(ios::fixed);//��ͬ����
			break;
		case Sci:
			strStream << scientific << setprecision(precision);
			break;
		case Norm:
			break;
		}

		strStream << this->result;
		result = strStream.str();//��ȡ��������

		if (displayDigitSeparator)//����Ƿ���ʾ�ָ���
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
				result.insert(i, 1, ',');//����һ���ַ�
			}
		}

		return result;
	}

	bool Calculator::setExpression(const string& tarExpression)//���ñ��ʽ
	{
		int length = tarExpression.size();

		try
		{
			if (expression != nullptr)//��ǰ���ʽ�ǿ�
				delete expression;//ɾ������
			expression = new char[length + 1];//�����ڴ�ռ�
		}
		catch (exception & e)
		{
			expression = nullptr;
			throw e;//�׳��쳣
		}

		if (tarExpression.copy(expression, length) != length)
		{
			//���Ʋ��ɹ�
			delete expression;//ɾ��������ڴ�
			expression = nullptr;
			return false;//�����˳�
		}
		expression[length] = '\0';
		return true;
	}

	string Calculator::getInfixExpression()//��ȡ��׺���ʽ
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
			iter++;//����
		}
		str = ss.str();
		return str;
	}

	string Calculator::getPostfixExpression()//��ȡ��׺���ʽ
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
			iter++;//����
		}
		str = ss.str();
		return str;
	}




	///�ڲ�����

	void Calculator::_jumpToNext(int& index)
	{
		char c = expression[index];
		while (c != '\0')
		{
			if ((c >= '0' && c <= '9') || c == '.')
				c = expression[++index];
			else
				break;//�˳�
		}
	}

	bool Calculator::_parseString()//�����ַ���
	{
		int index = 0;//��ǰ����λ��
	//    int flag=1;//,����ʵʱ���������
		bool lastIsOperator = 1;//��һ���ǲ�����
		Node crtNode;
		Constant_index constant;//
		crtNode._operator = None;
		if (!this->infixExpression.empty())//��׺���ʽ�ǿ�
			this->infixExpression.clear();//���
		int length = strlen(this->expression);

		while (index < length)//��ѭ��
		{
			if (expression[index] == ' ')//�����ո�
			{
				index++;
				continue;//����
			}
			crtNode._operator = _scanOperator(index);//ɨ�������
			if (crtNode._operator == None)//δ��⵽������
			{
				//������
				constant = _scanConstant(index);//ɨ�賣��
				if (constant == NotConstant)//���ǳ���
				{
					try
					{
						//������ȡ��ֵ
						crtNode._operand = std::stod(expression + index);
					}
					catch (std::exception & e)
					{
						//��ȡ��ֵʧ��
						throw Exception(Exception::UnknownSymbol);//�׳��쳣(δ֪����)
					}
				}
				else
				{
					//��鵽����
					if (constant == Ans)
						crtNode._operand = result;
					else
						crtNode._operand = constantsValue[constant];
				}

				//û���쳣

				this->infixExpression.push_back(crtNode);//�ڵ���ӵ�ĩβ
				lastIsOperator = 0;
				_jumpToNext(index);//����
			}
			else//������
			{
				//������
				if (crtNode._operator == Minu && lastIsOperator)//��ȡ������
					crtNode._operator = Neg;//ת��Ϊ����
				if (crtNode._operator == Plus && lastIsOperator)//��ȡ���Ӻ�
					crtNode._operator = Act;//ת��Ϊ����
				this->infixExpression.push_back(crtNode);//�ڵ���ӵ�ĩβ
				if (crtNode._operator != Fact && crtNode._operator != BraR)//��Ҫ����ǲ��Ǻ�׺�����
					lastIsOperator = 1;
			}
		}
		return true;//��������
	}

	bool Calculator::_convertToPostfix()//ת��Ϊ��׺���ʽ
	{
		if (!this->postfixExpression.empty())//�ǿ�
			postfixExpression.clear();//���

		bool lastIsOperand = false;//��һ���Ƿ��ǲ������ı��
		bool lastIsPrepositiveOperator = false;//��һ���Ƿ���ǰ�ò������ı��

		Expression::iterator iter, iter_end;//ָ��ڵ�ĵ�����
		iter = infixExpression.begin();//��ȡ��ʼ������
		iter_end = infixExpression.end();//��ȡĩβ������

		Node tempOperatorNode;//��ʱOP�ڵ�
		OperatorStack opStack;//������ջ

		while (iter != iter_end)//��ѭ��
		{
			if ((iter)->_operator == None)//����Ϊ������
			{
				postfixExpression.push_back(*iter);//ֱ������Ŀ���б�
				lastIsOperand = true;//���Ϊ��ֵ
				lastIsPrepositiveOperator = false;
			}
			else//����Ϊ������
			{
				if (iter->_operator == BraR)//������,������ֵ����
				{
					while (true)
					{
						if (opStack.empty())//ջΪ��
							throw Exception(Exception::BracketMismatching);//�׳��쳣(���Ų�ƥ��)
						tempOperatorNode = opStack.top();//��ȡջ��Ԫ��
						opStack.pop();//��ջ
						if (tempOperatorNode._operator == BraL)//��������������
							break;
						else
							postfixExpression.push_back(tempOperatorNode);//����Ŀ�����
					}//while
					lastIsOperand = true;//���Ϊ��ֵ
				}
				else if (iter->_operator == BraL)//������,������ֵ����
				{
					lastIsOperand = true;//���Ϊ��ֵ
					opStack.push(*iter);//����ջ
				}
				else//������
				{
					//�����﷨���

					if (!lastIsOperand && operatorsPosition[iter->_operator] == 1)
					{
						throw Exception(Exception::SyntaxError);//�׳��쳣(�﷨����)
					}

					//�﷨������

					if (!opStack.empty())//ջ��Ϊ��
					{
						//�������ջ�бȽ�Ҫ��ջ��OP���ȼ���(��һ����)��OP
						while ((!opStack.empty()) && priority[opStack.top()._operator] >= priority[iter->_operator] && opStack.top()._operator != iter->_operator)
						{
							tempOperatorNode = opStack.top();//��ȡջ��Ԫ��
							if (tempOperatorNode._operator != BraL)
								postfixExpression.push_back(tempOperatorNode);//����Ŀ�����
							else
								break;
							opStack.pop();//��ջ
						}//while
					}

					opStack.push(*iter);//����ջ
					if (operatorsPosition[iter->_operator] == 0)//ǰ�������
					{
						//lastIsPrepositiveOperator = true;//���Ϊǰ�ò�����
						lastIsOperand = false;//���Ϊ������
					}
					else//����
					{
						lastIsOperand = true;//���Ϊ��ֵ
					}
				}
			}
			iter++;//����
		}//while

		//�﷨���
		if (!lastIsOperand)
		{
			throw Exception(Exception::SyntaxError);//�׳��쳣(�﷨����)
		}

		while (!opStack.empty())//��ջ��ʣ�����������
		{
			tempOperatorNode = opStack.top();//��ȡջ��Ԫ��
			if (tempOperatorNode._operator == BraL)
				throw Exception(Exception::BracketMismatching);//�׳��쳣(���Ų�ƥ��)
			opStack.pop();//��ջ
			postfixExpression.push_back(tempOperatorNode);//����Ŀ�����
		}
		return true;
	}

	Calculator::Operator_index Calculator::_scanOperator(int& index)//ɨ�������,δ�ҵ�����None
	{
		//��������
		for (int i = 1; i < Operator_End; i++)
		{
			if (_operatorCopmare(this->expression + index, i))
				return index += operatorsIdentifierLength[i], i;
		}
		return None;//�ǲ�����(������)
	}

	Calculator::Constant_index Calculator::_scanConstant(int& index)//ɨ�賣��,����index
	{
		if (strncmp(this->expression + index, "PI", 2) == 0)
		{
			index += 2;//������λ
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
			return NotConstant;//���طǳ������
	}

	Calculator::Operand_index Calculator::_calculatePostfix()//�����׺���ʽ���
	{
		OperandStack operandStack;//������ջ
		Calculator::Operand_index operand1 = 0, operand2 = 0;

		Expression::iterator iter, iter_end;
		Node crtNode;
		iter = postfixExpression.begin();//��ȡ��ʼ������
		iter_end = postfixExpression.end();//��ȡĩβ������

		while (iter != iter_end)//��ѭ��
		{
			crtNode = *iter;
			if (crtNode._operator == None)//������������
			{
				//������
				operand1 = iter->_operand;
				operandStack.push(operand1);//��������ջ
			}
			else//������������
			{
				//������
				//�������������(��,˫Ŀ)��ջ�л�ȡ������
				if (operatorsType[crtNode._operator] == 1)//��Ŀ�����
				{
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//�׳��쳣(�޲�����)
					operand1 = operandStack.top();//��ȡջ��Ԫ��
					operandStack.pop();//��ջ
				}
				else//˫Ŀ�����
				{
					//��ȡ�ڶ���������
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//�׳��쳣(�޲�����)
					operand2 = operandStack.top();//��ȡջ��Ԫ��
					operandStack.pop();//��ջ

					//��ȡ��һ��������
					if (operandStack.empty())
						throw Exception(Exception::NoOperand);//�׳��쳣(�޲�����)
					operand1 = operandStack.top();//��ȡջ��Ԫ��
					operandStack.pop();//��ջ

				}
				try
				{
					operand1 = _getValue(operand1, operand2, crtNode._operator);//������
				}
				catch (Exception & e)
				{
					//�����쳣
					throw e;//�׳�
				}
				operandStack.push(operand1);//��������ջ
			}

			iter++;
		}
		if (operandStack.size() > 1)
			throw Exception(Exception::NoOperator);//�׳��쳣(�޲�����)
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
			if (operand2 == 0)//����Ϊ0
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return operand1 / operand2;
		}
		case Pow:
			return std::pow(operand1, operand2);
		case Fact://�׳�
		{
			Calculator::Operand_index temp = ceil(operand1);//����ȡ��
			if (temp > 1754)//�����㼫��ֵ
				throw Exception(Exception::ValueOverflow);//�׳��쳣(��ֵ���)
			if (temp != operand1 || operand1 < 0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			if (temp < 0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
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
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return (Calculator::Operand_index)(operand_1 % operand_2);
		}
		case Log:
		{
			if (operand1 == 0)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return std::log(operand2) / std::log(operand1);//���׹�ʽ(operand1�ǵ���)
		}
		case Sin:
		{
			Calculator::Operand_index operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			return std::sin(operand_1);
		}
		case Cos:
		{
			Calculator::Operand_index operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			return std::cos(operand_1);
		}
		case Tan:
		{
			Calculator::Operand_index operand_1 = operand1;
			if (angleUnit == Deg)
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			if (operand1 == constantsValue[PI] / 2)//���ֵ����PI/2
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
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
				operand_1 /= angleUnitsConvertConstant;//ת��Ϊ����
			if (operand1 == 0)//���ֵ����0
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)
			return 1.0 / std::tan(operand_1);
		}
		case Arra://������
		{
			Calculator::Operand_index operand_1 = ceil(operand1);
			Calculator::Operand_index operand_2 = ceil(operand2);

			if (operand1 < 0 || operand2 < 0 || operand_1 != operand1 || operand_2 != operand2)
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)

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
				throw Exception(Exception::MathError);//�׳��쳣(��ѧ����)

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
			if (str[i] == '\0')//����
			{
				break;
			}
			c = operatorsIdentifier[op][i];
			if (str[i] == c || str[i] == c - 32)//�����ִ�Сд
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



