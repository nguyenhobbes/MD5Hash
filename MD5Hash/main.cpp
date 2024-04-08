#include <iostream>
#include <string>
#include <bitset>
#include <unordered_map>
#include <sstream>

using namespace std;

string stringToBinary(string s);
string intToBinary(int num);
string binToHex(string s);
string hexToBin(string hexStr);
void paddingTo512Block(string& s);
void printTheString(string s);

void createHexMap(unordered_map<string, char>* hexMap);

void hash512Block(string s);

string bwAND(string str1, string str2);
string bwOR(string str1, string str2);
string bwXOR(string str1, string str2);
string bwNOT(string str);

string functionF(string b, string c, string d);

int main() {
	string text = "They are deterministic";
	string bin = stringToBinary(text);
	
	//This instance is to test for length = 447;
	//string temp = bin.substr(0, 447);

	//padding the binary to a block of N*512bit
	paddingTo512Block(bin);
	printTheString(bin);

	//convert the block to hex string
	string hex = binToHex(bin);
	//printTheString(hex);

	int len = bin.length();
	string lenBinary = intToBinary(len);

	/*string* m = new string[16];
	for (int i = 0; i < 16; i++)
	{
		m[i] = hex.substr(8*i, 8);
	}
	for (int i = 0; i < 16; i++)
	{
		cout << m[i] << endl;
	}*/

	hash512Block(hex);
	
	
	//cout << len;
	
	return 0;
}

void createHexMap(unordered_map<string, char>* hexMap) {
	(*hexMap)["0000"] = '0';
	(*hexMap)["0001"] = '1';
	(*hexMap)["0010"] = '2';
	(*hexMap)["0011"] = '3';
	(*hexMap)["0100"] = '4';
	(*hexMap)["0101"] = '5';
	(*hexMap)["0110"] = '6';
	(*hexMap)["0111"] = '7';
	(*hexMap)["1000"] = '8';
	(*hexMap)["1001"] = '9';
	(*hexMap)["1010"] = 'a';
	(*hexMap)["1011"] = 'b';
	(*hexMap)["1100"] = 'c';
	(*hexMap)["1101"] = 'd';
	(*hexMap)["1110"] = 'e';
	(*hexMap)["1111"] = 'f';
}

void createBinMap(unordered_map<char, string>* binMap) {
	(*binMap)['0'] = "0000";
	(*binMap)['1'] = "0001";
	(*binMap)['2'] = "0010";
	(*binMap)['3'] = "0011";
	(*binMap)['4'] = "0100";
	(*binMap)['5'] = "0101";
	(*binMap)['6'] = "0110";
	(*binMap)['7'] = "0111";
	(*binMap)['8'] = "1000";
	(*binMap)['9'] = "1001";
	(*binMap)['a'] = "1010";
	(*binMap)['b'] = "1011";
	(*binMap)['c'] = "1100";
	(*binMap)['d'] = "1101";
	(*binMap)['e'] = "1110";
	(*binMap)['f'] = "1111";

}


string stringToBinary(string s) {
	//This function is to convert a given string into Binary form
	string binaryString = "";
	for (char& c : s)
	{
		binaryString += bitset<8>(c).to_string();
	}
	return binaryString;
}

string intToBinary(int num) {
	bitset<64> binary(num);
	// Converting the binary representation to a string
	return binary.to_string();
}

string binToHex(string s) {
	string hex = "";

	unordered_map<string, char> hexMap;
	createHexMap(&hexMap);

	//In this loop, we start to transform each 4-bit of binary into a hex character
	for (int i = 0; i < s.length() - 3; i+=4)
	{
		hex += hexMap[s.substr(i, 4)];
	}
	return hex;
}

string hexToBin(string s) {
	string bin = "";

	unordered_map<char, string> binMap;
	createBinMap(&binMap);

	//In this loop, we start to transform each 4-bit of binary into a hex character
	for (int i = 0; i < s.length(); i++)
	{
		bin += binMap[s[i]];
	}
	return bin;
}

void paddingTo512Block(string& s) {
	int len = s.length();
	int block = len / 512 + 1;
	int zeros;
	
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
	for (int i = 0; i < zeros; i++)
	{
		s += "0";
	}
	s += intToBinary(len);
	
}

void printTheString(string s) {
	for (int i = 0; i < s.length(); i+=8)
	{
		for (int j = 0; j < 8; j++)
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


void hash512Block(string s) {
	string A = "01234567";
	string B = "89abcdef";
	string C = "fedcba98";
	string D = "76543210";


	string f = functionF(B, C, D);
	

}

string functionF(string b, string c, string d) {
	return bwOR(bwAND(b, c), bwAND(bwNOT(b), d));
}







