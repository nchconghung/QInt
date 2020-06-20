#include "QInt.h"

//Set all bits to 0
QInt::QInt()
{
	for (int i = 0; i < 4; i++)
		data[i] = 0;
}

//Set the true position of a QInt's bit
void QInt::SetBit(bool bit, int bit_pos)
{
	if (bit == 1)
		data[3 - bit_pos / 32] = data[3 - bit_pos / 32] | 1 << (bit_pos % 32);
	//Use the bitwise OR operator to set the bit at bit_pos position to 1
}

//Convert from a decimal string to QInt
void QInt::DecimalStringToQInt(string dec_str)
{
	bool bit, neg = false;
	int bit_pos = 0;

	if (dec_str == "0")
		return;

	if (dec_str[0] == '-')
	{
		dec_str.erase(dec_str.begin());
		neg = true;
	}

	while (dec_str != "")
	{
		bit = (dec_str[dec_str.length() - 1] - '0') % 2;
		SetBit(bit, bit_pos);
		Div2(dec_str);
		bit_pos++;
	}

	if (neg == true)
	{
		//Use bitwise NOT operator to convert from positive to negative counterpart of QInt
		//One's complement format
		*this = ~*this;

		//Add 1 to QInt so it becomes two's complement format
		QInt temp;
		temp.data[3] = 1;
		*this = *this + temp;
	}
}

//Convert from a binary string to QInt
void QInt::BinaryStringToQInt(string bin_str)
{
	bool bit;
	int bit_pos = 0;

	while (bin_str != "")
	{
		int len = bin_str.length() - 1;
		bit = bin_str[len] - '0';
		SetBit(bit, bit_pos);
		bit_pos++;
		bin_str.erase(bin_str.begin() + len);
	}
}

//Convert from a hexadecimal string to QInt
void QInt::HexadecimalStringToQInt(string hex_str)
{
	string bin_str = "";
	map<char, string> hex_to_bin{
	{ '0', "0000" },{ '1', "0001" },{ '2', "0010" },{ '3', "0011" },
	{ '4', "0100" },{ '5', "0101" },{ '6', "0110" },{ '7', "0111" },
	{ '8', "1000" },{ '9', "1001" },{ 'A', "1010" },{ 'B', "1011" },
	{ 'C', "1100" },{ 'D', "1101" },{ 'E', "1110" },{ 'F', "1111" } };

	for (int i = 0; i < hex_str.length(); i++)
		bin_str = bin_str + hex_to_bin[hex_str[i]];

	BinaryStringToQInt(bin_str);
}

//Convert from QInt to binary string
string QInt::QIntToBinaryString()
{
	string bin_str = "";
	char bit;

	if (data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0)
		return "0";

	for (int i = 3; i >= 0; i--)
		for (int j = 0; j < 32; j++)
		{
			bit = (data[i] & 1) + '0';
			bin_str = bit + bin_str;
			data[i] = data[i] >> 1;
		}

	while (bin_str[0] == '0')
		bin_str.erase(bin_str.begin());

	return bin_str;
}

//Convert from QInt to decimal string
string QInt::QIntToDecimalString()
{
	string dec_str = "";
	string bin_str;
	bool neg = false;

	if (data[0] >= 2147483648)
	{
		neg = true;
		QInt temp;
		temp.data[3] = 1;
		*this = *this - temp;

		*this = ~*this;
	}

	bin_str = this->QIntToBinaryString();
	int len = bin_str.length() - 1;

	for (int i = len; i >= 0; i--)
		if (bin_str[i] == '1')
			dec_str = AddTwoPositiveDecimalString(TwoToTheN(len - i), dec_str);

	if (neg == true)
		dec_str = "-" + dec_str;

	return dec_str;
}

//Convert from QInt to hexadecimal string
string QInt::QIntToHexadecimalString()
{
	string bin_str = this->QIntToBinaryString();
	string hex_str = BinaryStringToHexadecimalString(bin_str);

	return hex_str;
}

//= operator
QInt QInt::operator=(const QInt & x)
{
	for (int i = 0; i < 4; i++)
		this->data[i] = x.data[i];

	return *this;
}

//AND operator
QInt QInt::operator&(const QInt & x)
{
	QInt result;

	for (int i = 0; i < 4; i++)
		result.data[i] = data[i] & x.data[i];

	return result;
}

//OR operator
QInt QInt::operator|(const QInt & x)
{
	QInt result;

	for (int i = 0; i < 4; i++)
		result.data[i] = data[i] | x.data[i];

	return result;
}


//XOR operator
QInt QInt::operator^(const QInt & x)
{
	QInt result;

	for (int i = 0; i < 4; i++)
		result.data[i] = data[i] ^ x.data[i];

	return result;
}

//NOT operator
QInt QInt::operator~()
{
	QInt result;

	for (int i = 0; i < 4; i++)
		result.data[i] = ~data[i];

	return result;
}

bool operator>(const QInt & x1, const QInt & x2)
{
	bool flat;
	QInt temp1 = x1, temp2 = x2;
	string str1 = temp1.QIntToDecimalString(), str2 = temp2.QIntToDecimalString();
	if (str1.length() > str2.length())
	{
		flat = true;  //if x1 > x2, decimal
	}
	else if (str1.length() < str2.length())
	{
		flat = false;  //if x2 > x1, decimal
	}
	else
	{  // if the length of 2 number decimal string is equal
		for (int i = 0; i < str1.length(); i++)
		{
			if (str1[i] != str2[i])
			{
				if (str1[i] > str2[i])
				{
					flat = true;   // x1 > x2
				}
				else
				{
					flat = false;  // x2 < x1
				}
				return flat;
			}
			else
			{
				flat = false;   // x1 = x2;
			}
		}
	}
	return flat;
}

//+ operator
QInt operator+(const QInt& x1, const QInt& x2)
{
	QInt result = x1;
	int bitpos = 0;
	int flat = 0; 

	for (int i = 3; i >= 0; i--)
	{
		do
		{
			int temp1 = result.data[i],
				temp2 = x2.data[i],
				temp3, tempflat = flat;
			char bit_x1, bit_x2;

			bit_x1 = temp1 >> bitpos & 1;
			bit_x2 = temp2 >> bitpos & 1;   // get bit at bitpos position
			temp3 = bit_x2 << bitpos;       // set the true position of plus bit 
			tempflat = tempflat << bitpos;  // set the true position of flat bit 
			result.data[i] = result.data[i] ^ temp3 ^ tempflat;

			if ((bit_x1 == 1 && bit_x2 == 1) || (bit_x1 == 1 && flat == 1) || (bit_x2 == 1 && flat == 1))
				flat = 1;    // plus flat variable
			else
				flat = 0;

			bitpos++;
			temp1 = result.data[i];
			temp1 = temp1 >> bitpos & 1;
		} while (bitpos < 32);

		bitpos = 0;
	}

	return result;
}

//- operator
QInt operator-(const QInt& x1, const QInt& x2)
{
	QInt temp1 = x2;

	//Use bitwise NOT operator to convert from positive to negative counterpart of QInt
	//One's complement format
	for (int i = 0; i < 4; i++) 
		temp1.data[i] = ~temp1.data[i];

	//Add 1 to QInt so it becomes two's complement format
	QInt temp2; temp2.data[3] = 1;
	temp1 = temp1 + temp2;

	return temp1 + x1;
}

//* operator
QInt operator*(const QInt& x1, const QInt& x2)
{
	QInt result, temp = x1, sohang;
	int bit, bitpos;
	for (int i = 3; i >= 0; i--)
	{
		bitpos = 0;
		do {
			bit = x2.data[i];
			bit = bit >> bitpos & 1;
			sohang = temp;
			if (bit == 1)
			{
				QInt temp1 = temp;
				//cout << temp1.QIntToBinaryString() << endl;
				result = result + sohang;
			}
			temp = temp << 1;
			bitpos++;
		} while (bitpos < 32);
	}
	return result;
}


QInt operator/(const QInt& x1, const QInt& x2)
{
	// Số bị chia là tổng "thương" lần số chia
	QInt temp = x2;
	QInt result, temp3;
	result.data[3] = 1; temp3 = result;
	// Nhân đôi temp và kết quả nhiều lần bằng phép dịch bit sang trái
	while (x1 > temp)
	{
		temp = temp + temp;
		result = result + result;

	}
	QInt t1 = temp, t2 = result;
	// Tìm thương bằng cách trừ dần số temp đến khi bằng số bị chia
	// hay temp = sobichia =x2 * result;
	while (temp > x1)
	{
		temp = temp - x2;
		result = result - temp3;
	}
	return result;
}

//Bitwise left shift
QInt QInt::operator<<(int offset)
{
	string res = this->QIntToBinaryString();

	for (int i = 0; i < offset; i++)
		res = res + "0";

	while (res.length() > 128)
		res.erase(res.begin());

	Scan(*this, res, 2);

	return *this;
}

//Bitwise right shift
QInt QInt::operator>>(int offset)
{
	string res = this->QIntToBinaryString();
	string leftmostBit;

	if (res.length() == 128) //Negative QInt
		leftmostBit = "1";
	else
		leftmostBit = "0";

	for (int i = 0; i < offset; i++)
	{
		res = leftmostBit + res;
		res.pop_back();
	}

	Scan(*this, res, 2);

	return *this;
}

//Bitwise rotate right
void QInt::ror()
{
	bool bit_right = data[3] & 1;

	*this = *this >> 1;

	if (bit_right == 0)
		data[0] = data[0] & 2147483647; //01111111 11111111 11111111 11111111
	else
		data[0] = data[0] | 2147483648; //10000000 00000000 00000000 00000000
}

//Bitwise rotate left
void QInt::rol()
{
	bool bit_left;

	if (data[0] < 2147483648)
		bit_left = 0;
	else
		bit_left = 1;

	*this = *this << 1;

	if (bit_left == 0)
		data[3] = data[3] & 4294967294; //11111111 11111111 11111111 11111110
	else
		data[3] = data[3] | 1;
}

//Divide a decimal string by 2
void Div2(string & dec_str)
{
	for (int i = 0; i < dec_str.length(); i++)
	{
		if ((dec_str[i] - '0') % 2 == 1)
			dec_str[i + 1] = 10 + dec_str[i + 1];

		dec_str[i] = (dec_str[i] - '0') / 2 + '0';

		if (dec_str[0] == '0')
		{
			dec_str.erase(dec_str.begin());
			i--;
		}
	}
}

//Compute two to the N
string TwoToTheN(int n)
{
	if (n == 0)
		return "1";

	string res = "2";

	for (int i = 1; i < n; i++)
	{
		int len = res.length() - 1;
		int carry = 0;

		for (int j = len; j >= 0; j--)
		{
			res[j] = (res[j] - '0') * 2 + carry;
			carry = res[j] / 10;
			res[j] = res[j] % 10 + '0';
		}

		if (carry != 0)
			res = (char)(carry + '0') + res;
	}

	return res;
}

//Add two positive decimal string
string AddTwoPositiveDecimalString(string a, string b)
{
	string res = "";

	while (a.length() < b.length())
		a = "0" + a;
	while (b.length() < a.length())
		b = "0" + b;

	int len = a.length() - 1;
	int carry = 0;

	for (int i = len; i >= 0; i--)
	{
		int temp = (a[i] - '0') + (b[i] - '0') + carry;
		res = (char)(temp % 10 + '0') + res;
		carry = temp / 10;
	}

	if (carry == 1)
		res = "1" + res;

	return res;
}

//Convert from binary string to hexadecimal one
string BinaryStringToHexadecimalString(string bin_str)
{
	if (bin_str.length() % 4 != 0)
	{
		int ii = 4 - (bin_str.length() - (bin_str.length() / 4) * 4);
		for (int i = 0; i < ii; i++)
			bin_str.insert(0, "0");
	}
	string hex_str = "";

	map<string, string> bin_to_hex{
	{ "0000", "0" },{ "0001", "1" },{ "0010", "2" },{ "0011", "3" },
	{ "0100", "4" },{ "0101", "5" },{ "0110", "6" },{ "0111", "7" },
	{ "1000", "8" },{ "1001", "9" },{ "1010", "A" },{ "1011", "B" },
	{ "1100", "C" },{ "1101", "D" },{ "1110", "E" },{ "1111", "F" } };

	int ii = 0;
	for (int i = 0; i < bin_str.length() / 4; i++)
	{
		string str1 = bin_str.substr(ii, 4);
		hex_str += bin_to_hex[str1];
		ii += 4;
	}

	return hex_str;
}

void Scan(QInt &q, string str, int base)
{
	switch (base)
	{
	case 2:
		q.BinaryStringToQInt(str);
		break;
	case 10:
		q.DecimalStringToQInt(str);
		break;
	case 16:
		q.HexadecimalStringToQInt(str);
		break;
	}
}

void Print(ofstream & outfile, QInt q, int base)
{
	switch (base)
	{
	case 2:
		outfile << q.QIntToBinaryString();
		break;
	case 10:
		outfile << q.QIntToDecimalString();
		break;
	case 16:
		outfile << q.QIntToHexadecimalString();
		break;
	}
}
