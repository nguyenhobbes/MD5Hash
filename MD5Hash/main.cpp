#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <cmath>

const long long int mod = pow(2, 32);


using namespace std;

//-----------------------FUNCTION DECLARATION----------------------------
string stringToBin(string s);
string decToBin(long long int num);
long long int binToDec(string s);
string binToHex(string s);
string hexToBin(string hexStr);
void cutStringInHalf(string& s);

void paddingTo512Block(string& s);
void printTheString(string s);

void createBinHexMap(unordered_map<string, char>* BinHexMap);
void createHexBinMap(unordered_map<char, string>* HexBinMap);

void hash512Block(string s);

string bwAND(string str1, string str2);
string bwOR(string str1, string str2);
string bwXOR(string str1, string str2);
string bwNOT(string str);


void leftRotateBin(string& s, int amount);

string functionF(string b, string c, string d);
string functionG(string b, string c, string d);
string functionH(string b, string c, string d);
string functionI(string b, string c, string d);

int main() {
	string text = "a";
	string bin = stringToBin(text);
	
	//This instance is to test for length = 447;
	//string temp = bin.substr(0, 447);

	//padding the binary to a block of N*512bit
	paddingTo512Block(bin);
	printTheString(bin);

	//convert the block to hex string
	string hex = binToHex(bin);
	printTheString(hex);

	long long int len = bin.length();
	string lenBinary = decToBin(len);

	/*string* m = new string[16];
	for (long long int i = 0; i < 16; i++)
	{
		m[i] = hex.substr(8*i, 8);
	}
	for (long long int i = 0; i < 16; i++)
	{
		cout << m[i] << endl;
	}*/

	hash512Block(hex);
	
	
	//cout << len;
	
	return 0;
}

void createBinHexMap(unordered_map<string, char>* BinHexMap) {
	(*BinHexMap)["0000"] = '0';
	(*BinHexMap)["0001"] = '1';
	(*BinHexMap)["0010"] = '2';
	(*BinHexMap)["0011"] = '3';
	(*BinHexMap)["0100"] = '4';
	(*BinHexMap)["0101"] = '5';
	(*BinHexMap)["0110"] = '6';
	(*BinHexMap)["0111"] = '7';
	(*BinHexMap)["1000"] = '8';
	(*BinHexMap)["1001"] = '9';
	(*BinHexMap)["1010"] = 'a';
	(*BinHexMap)["1011"] = 'b';
	(*BinHexMap)["1100"] = 'c';
	(*BinHexMap)["1101"] = 'd';
	(*BinHexMap)["1110"] = 'e';
	(*BinHexMap)["1111"] = 'f';
}


void createHexBinMap(unordered_map<char, string>* HexBinMap) {
	(*HexBinMap)['0'] = "0000";
	(*HexBinMap)['1'] = "0001";
	(*HexBinMap)['2'] = "0010";
	(*HexBinMap)['3'] = "0011";
	(*HexBinMap)['4'] = "0100";
	(*HexBinMap)['5'] = "0101";
	(*HexBinMap)['6'] = "0110";
	(*HexBinMap)['7'] = "0111";
	(*HexBinMap)['8'] = "1000";
	(*HexBinMap)['9'] = "1001";
	(*HexBinMap)['a'] = "1010";
	(*HexBinMap)['b'] = "1011";
	(*HexBinMap)['c'] = "1100";
	(*HexBinMap)['d'] = "1101";
	(*HexBinMap)['e'] = "1110";
	(*HexBinMap)['f'] = "1111";
}

string stringToBin(string s) {
	//This function is to convert a given string into Binary form
	string binaryString = "";
	for (char& c : s)
	{
		binaryString += bitset<8>(c).to_string();
	}
	return binaryString;
}

long long int binToDec(string s) {
	long long int decimalValue = 0;
	long long int power = 0;
	long long int temp = 0;
	// Iterate through the binary string from right to left
	for (long long int i = s.length() - 1; i >= 0; --i) {
		if (s[i] == '1') {
			temp = pow(2, power);
			decimalValue += temp;
		}
		power++;
	}
	return decimalValue;
}

string decToBin(long long int num) {
	bitset<64> binary(num);
	string temp = binary.to_string();
	// Converting the binary representation to a string
	return temp;
}

string binToHex(string s) {
	string hex = "";

	unordered_map<string, char> BinHexMap;
	createBinHexMap(&BinHexMap);

	//In this loop, we start to transform each 4-bit of binary into a hex character
	for (long long int i = 0; i < s.length() - 3; i+=4)
	{
		hex += BinHexMap[s.substr(i, 4)];
	}
	return hex;
}

string hexToBin(string s) {
	string bin = "";

	unordered_map<char, string> HexBinMap;
	createHexBinMap(&HexBinMap);

	//In this loop, we start to transform each 4-bit of binary into a hex character
	for (long long int i = 0; i < s.length(); i++)
	{
		bin += HexBinMap[s[i]];
	}
	return bin;
}

long long int hexToDec(string s) {
	string temp = hexToBin(s);
	return binToDec(temp);
}


string decToHex(long long int decimalValue) {
	string temp = binToHex(decToBin(decimalValue));
	cutStringInHalf(temp);
	return temp;
}

void cutStringInHalf(string& s) {
	 s = s.substr(s.length() / 2, s.length() / 2);
}

void paddingTo512Block(string& s) {
	long long int len = s.length();
	long long int block = len / 512 + 1;
	long long int zeros;
	
	//if the remain is exact 448
	if (len % 512 >= 448) {
		block += 1;
		zeros = 512 * block - len % 512 - 1 + 448;
	}
	else if (len % 512 == 447) {
		zeros = 0;
	}
	else
	{
		zeros = 512 * block - len - 64 - 1;
	}
	s += "1";
	for (long long int i = 0; i < zeros; i++)
	{
		s += "0";
	}
	s += decToBin(len);
	
}

void printTheString(string s) {
	for (long long int i = 0; i < s.length(); i+=8)
	{
		for (long long int j = 0; j < 8; j++)
		{
			cout << s[i + j];
		}
		cout << endl;
	}
	cout << endl;
}

//THE MAIN ALGORITHM
string bwAND(string hexStr1, string hexStr2) {
	string binaryStr1 = hexToBin(hexStr1);
	string binaryStr2 = hexToBin(hexStr2);

	string result;
	for (size_t i = 0; i < binaryStr1.size(); ++i) {
		if (binaryStr1[i] == '1' && binaryStr2[i] == '1') {
			result += '1';
		}
		else {
			result += '0';
		}
	}
	
	return binToHex(result);
}

// Perform bw OR operation between two hexadecimal strings
string bwOR(string hexStr1, string hexStr2) {
	string binaryStr1 = hexToBin(hexStr1);
	string binaryStr2 = hexToBin(hexStr2);

	string result;
	for (size_t i = 0; i < binaryStr1.size(); ++i) {
		if (binaryStr1[i] == '1' || binaryStr2[i] == '1') {
			result += '1';
		}
		else {
			result += '0';
		}
	}

	return binToHex(result);
}

// Perform bw XOR operation between two hexadecimal strings
string bwXOR(string hexStr1, string hexStr2) {
	string binaryStr1 = hexToBin(hexStr1);
	string binaryStr2 = hexToBin(hexStr2);

	string result;
	for (size_t i = 0; i < binaryStr1.size(); ++i) {
		if ((binaryStr1[i] == '1' && binaryStr2[i] == '0') || (binaryStr1[i] == '0' && binaryStr2[i] == '1')) {
			result += '1';
		}
		else {
			result += '0';
		}
	}

	return binToHex(result);
}


// Perform bw NOT operation on a hexadecimal string
string bwNOT(string hexStr) {
	string binaryStr = hexToBin(hexStr);
	
	string result;
	for (char c : binaryStr) {
		result += (c == '0' ? '1' : '0');
	}

	return binToHex(result);
}

void leftRotateBin(string& binaryStr, int shiftAmount) {
	int length = binaryStr.length();

	// Perform left rotation operation
	for (int i = 0; i < shiftAmount; i++) {
		char temp = binaryStr[0]; // Store the leftmost bit
		binaryStr.erase(0, 1);    // Remove the leftmost bit
		binaryStr += temp;        // Append the leftmost bit to the right
		//cout << binaryStr << endl;
	}
}


void hash512Block(string s) {
	string temp;

	//original;
	string oa = "01234567";
	string ob = "89abcdef";
	string oc = "fedcba98";
	string od = "76543210";

	//dynamic, will change later
	string a = "01234567";
	string b = "89abcdef";
	string c = "fedcba98";
	string d = "76543210";


	string F;
	long long int x;
	long long int y;
	long long int Fa;

	string ans;


	int r[4][16] = { { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 },
		{ 1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12 },
		{ 5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2 },
		{ 0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9 } };
	string m[16];
	for (int i = 0; i < 16; i++)
	{
		m[i] = s.substr(8 * i, 8);
	}
	string k[64];
	for (int i = 0; i < 64; i++)
	{
		k[i] = decToHex(floor(mod * abs(sin(i + 1))));
		//cout << "K[" << i + 1 << "] = " << k[i] << endl;
	}
	int shift[4][16];
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 16; j++)
		{
			if (i == 1)
			{
				if (j == 1 || j == 5 || j == 9 || j == 13)
				{
					shift[i - 1][j - 1] = 7;
				}
				else if (j == 2 || j == 6 || j == 10 || j == 14)
				{
					shift[i - 1][j - 1] = 12;
				}
				else if (j == 3 || j == 7 || j == 11 || j == 15)
				{
					shift[i - 1][j - 1] = 17;
				}
				else
				{
					shift[i - 1][j - 1] = 22;
				}
			}
			else if (i == 2)
			{
				if (j == 1 || j == 5 || j == 9 || j == 13)
				{
					shift[i - 1][j - 1] = 5;
				}
				else if (j == 2 || j == 6 || j == 10 || j == 14)
				{
					shift[i - 1][j - 1] = 9;
				}
				else if (j == 3 || j == 7 || j == 11 || j == 15)
				{
					shift[i - 1][j - 1] = 14;
				}
				else
				{
					shift[i - 1][j - 1] = 20;
				}
			}
			else if (i == 3)
			{
				if (j == 1 || j == 5 || j == 9 || j == 13)
				{
					shift[i - 1][j - 1] = 4;
				}
				else if (j == 2 || j == 6 || j == 10 || j == 14)
				{
					shift[i - 1][j - 1] = 11;
				}
				else if (j == 3 || j == 7 || j == 11 || j == 15)
				{
					shift[i - 1][j - 1] = 16;
				}
				else
				{
					shift[i - 1][j - 1] = 13;
				}

			}
			else
			{
				if (j == 1 || j == 5 || j == 9 || j == 13)
				{
					shift[i - 1][j - 1] = 6;
				}
				else if (j == 2 || j == 6 || j == 10 || j == 14)
				{
					shift[i - 1][j - 1] = 10;
				}
				else if (j == 3 || j == 7 || j == 11 || j == 15)
				{
					shift[i - 1][j - 1] = 15;
				}
				else
				{
					shift[i - 1][j - 1] = 21;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 16 * i; j < 16 * (i + 1); j++) {
			temp = m[r[i][j % 16]];
			cout << "---------------ROUND " << j + 1 << ": " << r[i][j % 16] << "\t" << m[r[i][j % 16]] << endl;
			cout << a << endl << b << endl << c << endl << d << endl << endl;
			//compute F and add A to F
			switch (i)
			{
			default:
				break;
			case 0: 
				F = functionF(b, c, d);
			case 1:
				F = functionG(b, c, d);
			case 2:
				F = functionH(b, c, d);
			case 3:
				F = functionI(b, c, d);
			}
			cout << F << endl;
			x = hexToDec(a);
			y = hexToDec(F);
			Fa = (x + y) % mod;
			ans = decToHex(Fa);
			cout << ans << endl;

			//Add message Mi
			x = hexToDec(temp);
			y = hexToDec(ans);
			Fa = (x + y) % mod;
			ans = decToHex(Fa);
			cout << ans << endl;


			//Add Constant Ki
			x = hexToDec(k[j]);
			y = hexToDec(ans);
			Fa = (x + y) % mod;
			ans = decToHex(Fa);
			cout << ans << endl;

			//Rotate left
			temp = hexToBin(ans);
			leftRotateBin(temp, shift[i][j]);
			ans = binToHex(temp);
			cout << ans << endl;

			//Add B
			x = hexToDec(b);
			y = hexToDec(ans);
			Fa = (x + y) % mod;
			ans = decToHex(Fa);
			cout << ans << endl << endl;

			//Finishing up
			a = d;
			d = c;
			c = b;
			b = ans;


			cout << endl << "4 vectors now are:\n" << a << endl << b << endl << c << endl << d << endl;
		}
	}
	string finA, finB, finC, finD;
	x = hexToDec(a);
	y = hexToDec(oa);
	Fa = (x + y) % mod;
	finA = decToHex(Fa);

	x = hexToDec(b);
	y = hexToDec(ob);
	Fa = (x + y) % mod;
	finB = decToHex(Fa);

	x = hexToDec(c);
	y = hexToDec(oc);
	Fa = (x + y) % mod;
	finC = decToHex(Fa);

	x = hexToDec(d);
	y = hexToDec(od);
	Fa = (x + y) % mod;
	finD = decToHex(Fa);

	cout << endl << "Final Result:\n";
	cout << finA << endl << finB << endl << finC << endl << finD << endl;

	b = "7d502063";
	c = "8b3d715d";
	d = "1de3a739";
	cout << endl << functionI(b, c, d);
}

string functionF(string b, string c, string d) {
	return bwOR(bwAND(b, c), bwAND(bwNOT(b), d));
}

string functionG(string b, string c, string d) {
	return bwOR(bwAND(b, d), bwAND(c , bwNOT(d)));
}

string functionH(string b, string c, string d) {
	return bwXOR(bwXOR(b, c), d);
}

string functionI(string b, string c, string d) {
	return bwXOR(c, bwOR(bwNOT(d), b));
}









