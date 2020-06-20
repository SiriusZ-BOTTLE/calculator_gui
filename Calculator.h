#pragma once

#include <iostream>
#include <exception>
#include <vector>
#include <list>
#include <stack>
#include <sstream>
#include <algorithm>
#include <iomanip>


//�궨��
#define DLL_EXPORTS//����DLL
#define INSIDE_DLL//DLL�ڲ�

#ifdef __linux__
#ifdef DLL_EXPORTS
#undef DLL_EXPORTS
#endif
#endif

#ifdef DLL_EXPORTS
#ifdef INSIDE_DLL
#define TEXT_ANALYSIS_EXPORT __declspec(dllexport)
#else
#define TEXT_ANALYSIS_EXPORT __declspec(dllimport)
#endif
#else
#define TEXT_ANALYSIS_EXPORT
#endif


namespace NS_Calculator
{

	using namespace std;

	//�쳣��
	class Exception :public std::exception
	{
		///һЩ����
	public:
		//ö�� �쳣����{��ֵ��ȡ����,���Ų�ƥ��,����}
		enum ExceptionType
		{
			//Calculator������쳣
			UnknownSymbol,//δ֪����
			BracketMismatching,//���Ų�ƥ��
			NoOperand,//�޲�����
			NoOperator,//�޲�����
			SyntaxError,//�﷨����
			MathError,//��ѧ����
			ExpressionNotSet,//�ޱ��ʽ
			ValueOverflow,//���

			//����̨�쳣
			UnknownCommand,//δ֪����
			UnknownOption,//δ֪ѡ��
			ExpectMoreOption,//��������ѡ��
			ArgumentMismatching,//������ƥ��
			MutexOptionExists,//���ڻ���ѡ��
			KeyOptionNotFound,//ȱ�ٱ�Ҫѡ��

			OtherException,//�����쳣
			ExceptionType_End//�ڱ�
		};
		static const string exceptionDetail[ExceptionType_End];

		///�����Ա
	private:
		ExceptionType exceptionType;//�쳣����

	public:
		explicit Exception(const ExceptionType& exceptionType = OtherException);
		~Exception();
		virtual const char* what()const;

	};

	/**	�滮һ��:
	 *	1.�����ַ���,�õ���׺���ʽ
	 *	2.����׺���ʽת��Ϊ��׺���ʽ
	 *	3.�����׺���ʽ
	 *	��ַ��¼
	 *	https://www.cnblogs.com/hantalk/p/8734511.html
	 */

	 //������
	class TEXT_ANALYSIS_EXPORT Calculator
	{
		///һЩ����:
	public:
		typedef int Operator_index;//������
		typedef int Constant_index;//����
		typedef long double Operand_index;//������

		//ö�� ������ {�Ӻ�"+",����"-",�˺�"*",����"/",��"^",�׳�"!",ģ"%",����"log",����"sin",����"cos",����"tan",������"arcsin",������"arccos",������"arctan",����"cot",����"ARR",���"COM",������"(",������")",����'+',����"-"};
		enum Operators
		{
			None = 0,//��
			Plus,//�Ӻ�"+"
			Minu,//����"-"
			Mult,//�˺�"*"
			Divi,//����"/"
			Pow,//��"^"
			Fact,//�׳�"!"
			Mod,//ģ"%"
			Abs,//����ֵ"ABS"
			Log,//����"log"
			Sin,//����"sin"
			Cos,//����"cos"
			Tan,//����"tan"
			ASin,//������"arcsin"
			ACos,//������"arccos"
			ATan,//������"arctan"
			Cot,//����"cot"
			Arra,//������"ARR"
			Comb,//�����"COM"
			BraL,//������"("
			BraR,//������")"
			Act,//��"+"
			Neg,//��"-"
			Operator_End//�ڱ�
		};
		//ö�� ���� {Բ����"PI",��Ȼ����'e'}
		enum Constants
		{
			NotConstant = -1,
			PI,//Բ����"PI"
			NaLo,//��Ȼ����"e"
			Ans,//��һ�ν��
			Constant_End//�ڱ�
		};
		//ö�� ��ʽģʽ {λ��,��ѧ,����}
		enum FormatMode { Fix = 0, Sci, Norm };
		//ö�� �Ƕȵ�λ {�Ƕ�,����}
		enum AngleUnit { Deg = 0, Rad };

		static const int offset;//ƫ����
		static const char* operatorsIdentifier[Operator_End];//�������ʶ��
		static const int operatorsIdentifierLength[Operator_End];//�������ʶ������
		static const Operand_index constantsValue[Constant_End];//����ֵ
		static const int priority[Operator_End];//���ȼ�
		static const int operatorsType[Operator_End];//������Ŀ��
		static const int operatorsPosition[Operator_End];//������λ��
		static const Operand_index angleUnitsConvertConstant;//�ǵ�λת������


	public:


		class Node//�ڵ�
		{
		public:
			Operator_index _operator;//������
			Operand_index _operand;//������
		};
		typedef std::list<Node> Expression;//���ʽ
		typedef std::stack<Node> OperatorStack;//������ջ
	//    typedef std::list<Node>::iterator IteratorToNode;//������

		typedef std::stack<Operand_index> OperandStack;//������ջ


	private:
		///�����Ա
	//		std::string expression;//�ַ������ʽ
		char* expression;//�ַ������ʽ
		Operand_index result;//���
		Expression infixExpression;//��׺���ʽ
		Expression postfixExpression;//��׺���ʽ

		FormatMode formatMode;//��ǰ��ʽģʽ
		AngleUnit angleUnit;//�Ƕȵ�λ
		int precision;//����
		bool displayDigitSeparator;//��ʾλ�ָ���


	public:
		///�����Ա����

		//���캯��
		Calculator();
		Calculator(const Calculator& another) = delete;//��֧�ָ��ƹ���
		//��������
		~Calculator();

		//������������
		void setFormat(const FormatMode& format = Norm, const int& precision = 2);
		void setPrecision(const int& precision);
		void setAngleUnit(const AngleUnit& unit = Rad);
		void setDisplayDigitSeparator(const bool& displayState = false);

		Operand_index getResult()const;//��ȡ������(����double)
		string getFormattedResult()const;//��ȡ��ʽ�����(�����ַ���)
		Operand_index calculate(const std::string& expression);//����
		Operand_index calculate();//����
		bool setExpression(const string& tarExpression);//���ñ��ʽ
		string getInfixExpression();//��ȡ��׺���ʽ
		string getPostfixExpression();//��ȡ��׺���ʽ

		Calculator& operator=(const Calculator& another) = delete;//��֧�ָ�ֵ����

	//    static Operand Calculate(const std::string &expression);//��̬��������

	private:
		///�ڲ�����
		void _jumpToNext(int& index);
		bool _parseString();//�����ַ���(�����׳��쳣)
		bool _convertToPostfix();//ת��Ϊ��׺���ʽ(�����׳��쳣)
		Operand_index _calculatePostfix();//�����׺���ʽ���(�����׳��쳣)
		Operand_index _getValue(const Operand_index& operand1, const Operand_index& operand2, const Operator_index& op);//����ֵ
		Operator_index _scanOperator(int& index);//ɨ�������,����index
		Constant_index _scanConstant(int& index);//ɨ�賣��,����index
		bool _operatorCopmare(const char* str, const Operator_index& op)noexcept;//������ƥ��(��Сд������)

	};
}








