#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

class QInt
{
	
public:
	unsigned int data[4] = { 0 };
	QInt();

	//Input
	void SetBit(bool bit, int bit_pos);
	void DecimalStringToQInt(string dec_str);
	void BinaryStringToQInt(string bin_str);
	void HexadecimalStringToQInt(string hex_str);

	//Output
	string QIntToBinaryString();
	string QIntToDecimalString();
	string QIntToHexadecimalString();

	//Operator
	QInt operator=(const QInt& x);
	QInt operator&(const QInt& x);
	QInt operator|(const QInt& x);
	QInt operator^(const QInt& x);
	QInt operator~();

	friend bool operator>(const QInt& x1, const QInt& x2);

	friend QInt operator+(const QInt& x1, const QInt& x2);
	friend QInt operator-(const QInt& x1, const QInt& x2);
	friend QInt operator*(const QInt& x1, const QInt& x2);
	friend QInt operator/(const QInt& x1, const QInt& x2);

	QInt operator<<(int offset);
	QInt operator>>(int offset);
	void ror();
	void rol();
};

void Div2(string & dec_str);
string TwoToTheN(int n);
string AddTwoPositiveDecimalString(string a, string b);
string BinaryStringToHexadecimalString(string bin_str);

void Scan(QInt &q, string str, int base);
void Print(ofstream &outfile, QInt q, int base);

//REF:
//https://www.studytonight.com/cpp/stl/stl-container-map