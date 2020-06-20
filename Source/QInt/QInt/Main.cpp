#include "QInt.h"

//BASE		t1				t2					t3
//base		base			num_str
//base		~,ror,rol		num_str
//base		num_str			<<,>>				offset
//base		num_str			+,-,*,/,&,|,^		num_str

int main(int argc, char *argv[])
{
	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);

	int base;
	string t1, t2, t3;

	infile.seekg(ios::beg);
	while (infile >> base >> t1 >> t2)
	{
		QInt q, q1, q2;
		bool Printed = false;

		if (t1 == "2" || t1 == "10" || t1 == "16" || t1 == "~" || t1 == "ror" || t1 == "rol")
		{
			Scan(q, t2, base);

			if (t1 == "2" || t1 == "10" || t1 == "16")
			{
				Print(outfile, q, atoi(t1.c_str()));
				Printed = true;
			}
			else
			{
				if (t1 == "~")
					q = ~q;
				else if (t1 == "ror")
					q.ror();
				else
					q.rol();
			}
		}
		else
		{
			infile >> t3;

			if (t2 == ">>")
			{
				Scan(q1, t1, base);
				q = q1 >> atoi(t3.c_str());
			}
			else if (t2 == "<<")
			{
				Scan(q1, t1, base);
				q = q1 << atoi(t3.c_str());
			}
			else
			{
				Scan(q1, t1, base);
				Scan(q2, t3, base);

				switch (t2[0])
				{
				case '+':
					q = q1 + q2;
					break;
				case '-':
					q = q1 - q2;
					break;
				case '*':
					q = q1 * q2;
					break;
				case '/':
					q = q1 / q2;
					break;
				case '&':
					q = q1 & q2;
					break;
				case '|':
					q = q1 | q2;
					break;
				case'^':
					q = q1 ^ q2;
					break;
				default:
					break;
				}
			}
		}

		if (Printed == false)
			Print(outfile, q, base);

		if (infile.eof())
			break;
		else
			outfile << endl;
	}

	infile.close();
	outfile.close();

	system("pause");
	return 0;
}