//-------------------------------------------------------------------------------------------------------------------------------------------------
// Group names: Diego Hernandez, Sergio Rojas Hurtado, Khoa Nguyen
// Assignment : Final Project
// Due date   : May 11, 2023
// Purpose    : This program formats a text file and prepares it to be tokenized so that it can be compiled through an LL
//				parsing table to check for any errors in the grammar, if the contents of the file are accepted by that grammar of the 
//				table then it will be converted to c++
//-------------------------------------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <unordered_map>

using namespace std;

// error codes: 3001: program | 3002: ; | 3003: integer | 3004: : | 3005: , | 3006: . | 3007: ( | 3008: ) | 3009: display | 3010: * | 3011 end. | 3012: identifier | 3013: "value=" || 3014: expression
					//		 a	   b	 c     g     f	   k     e     ,     ;     :     (     )     +     -      *    /	 =     0     1     2     3     4     5     6     7     8      9    p     q     r     s     $
int table[20][32] = /*P*/{ {100,  3000, 3000, 3001, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3001, 3000, 3001, 3001, 3000},
					/*I*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3013, 3012, 3000, 3000, 3012, 3000, 3000, 3000, 3000, 3000, 3012, 3000, 3012, 3012, 3000, 3000, 3000, 3000, 3000, 3000, 200,  200,  200,  200,  3000},
					/*X*/  {3000, 3002, 3000, 3000, 3004, 3002, 3000, 2000, 2000, 2000, 3010, 2000, 2000, 2000, 2000, 2000, 2000, 300,  300,  300,  300,  300,  300,  300,  300,  300,  300,  200,  200,  200,  200,  3000},
					/*B*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3012, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 400,  400,  400,  400,  3000},
					/*C*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3012, 3000, 3012, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 500,  500,  500,  500,  3000},
					/*G*/  {3000, 3000, 3000, 3000, 2100, 3000, 3000, 3000, 3003, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000},
					/*H*/  {3009, 3000, 3000, 3000, 3000, 600,  3011, 3000, 3000, 3000, 3009, 3000, 3000, 3000, 3000, 3000, 3012, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 600,  600,  600,  600,  3011},
					/*J*/  {3000, 3000, 3000, 3000, 3000, 4000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 2200, 2200, 2200, 2200, 3000},
					/*W*/  {3000, 3000, 3000, 3000, 3000, 700,  3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000},
					/*A*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 800,  800,  800,  800,  3000},
					/*E*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3014, 3000, 900,  3000, 900,  900,  3012, 3000, 3000, 900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  3000},
					/*Q*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 2000, 3000, 3000, 2000, 1000, 1100, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000},
					/*T*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 1200, 3000, 1200, 1200, 3000, 3000, 3000, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200, 3000},
					/*R*/  {3000, 3000, 3000, 3000, 3000, 3002, 3000, 3000, 2000, 3000, 3000, 2000, 2000, 2000, 1300, 1400, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000},
					/*F*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 1500, 3012, 1700, 1700, 3000, 3000, 3000, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1700, 1600, 1600, 1600, 1600, 3000},
					/*N*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 1800, 1800, 3000, 3000, 3000, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 3000, 3000, 3000, 3000, 3000},
					/*Y*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 2000, 3000, 3000, 2000, 2000, 2000, 2000, 2000, 3000, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 1900, 3002, 3000, 3000, 3000, 3000},
					/*S*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 2300, 2400, 3000, 3000, 3000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 3000, 3000, 3000, 3000, 3000},
					/*D*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 10,   20,   30,   40,   50,   60,   70,   80,   90,   99,   3012, 3000, 3000, 3000, 3000},
					/*L*/  {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 2500, 2600, 2700, 2800, 3000} };

void trace(vector<string> tokens, vector<string> copyTokens);
void displayStack(vector<string> stk);
bool processRowColumn(vector<string>& stk, int rowColumn, vector<string> copyTokens, int index);
int determineRowColumn(string pop, string read);
void split(const string& s, char delim, vector<string>& tokens);
void format_File(string unpreparedFileName, string fileName);
bool isPlusMissing(const string& line);
void pascalToC(vector<string> tokens, vector<string> copyTokens, string fileName);
std::stringstream encodeFileContent(std::ifstream& inFile, const std::string& inFileName,
	std::unordered_map<std::string, std::string>& encodeMap, std::unordered_map<std::string, std::string>& decodeMap);
std::stringstream cppConversion(std::stringstream& encodedSS, std::unordered_map<std::string, std::string> encodeMap, std::unordered_map<std::string, std::string> decodeMap);
bool isDisplayUsed(std::stringstream& encodedSS, std::unordered_map<std::string, std::string> encodeMap);
std::stringstream getContentsBetween(std::stringstream& encodedSS, std::string start, std::string end);

int main()
{
	string unpreparedFileName = "finalp1.txt";
	std::string fileName = "finalp2.txt";
	char choice;
	char choice2;
	cout << "Do you want to run the format_File function? (y/n): ";
	cin >> choice;

	if (choice == 'y' || choice == 'Y')
	{
		format_File(unpreparedFileName, fileName);
	}
	ifstream inFile(fileName);
	if (!inFile.is_open())
	{
		cerr << "Error opening file" << endl;
		return 0;
	}
	if (inFile)
	{
		string input;

		// Prepare tokens
		vector<string> tokens;
		vector<string> lines;

		while (getline(inFile, input))
		{
			if (input.empty()) continue; // skip empty lines
			lines.push_back(input);
			istringstream ss(input);
			string token;
			while (ss >> token)
			{
				tokens.push_back(token);

			}
		}


		tokens.push_back("$");

		for (int i = 0; i < tokens.size(); ++i)
		{
			cout << tokens[i] << endl;
		}

		vector<string> copyTokens;
		for (int i = 0; i < tokens.size(); ++i)
		{
			copyTokens.push_back(tokens[i]);
		}
		for (int i = 0; i < copyTokens.size(); ++i)
		{

			if (copyTokens[i] == "program")
				copyTokens[i] = "a";
			else if (copyTokens[i] == "var")
				copyTokens[i] = "b";
			else if (copyTokens[i] == "begin")
				copyTokens[i] = "c";
			else if (copyTokens[i] == "display")
				copyTokens[i] = "k";
			else if (copyTokens[i] == "\"value=\"")
				copyTokens[i] = "e";
			else if (copyTokens[i] == "integer")
				copyTokens[i] = "f";
			else if (copyTokens[i] == "end.")
				copyTokens[i] = "g";
		}

		cout << endl;

		for (int i = 0; i < copyTokens.size(); ++i)
		{
			cout << copyTokens[i] << endl;
		}

		// Tracing
		trace(copyTokens, copyTokens);
		cout << "\ndo you want to run the code conversion?(y/n) ";
		cin >> choice2;
		if (choice2 == 'y' || choice2 == 'Y')
		{
			pascalToC(tokens, copyTokens, fileName);
		}
		system("pause");
	}
	else
	{
		cout << "/nError opening file";
	}
	return 0;
} // end main

void trace(vector<string> tokens, vector<string> copyTokens)
{

	// DIEGO: removed table from trace

	vector<string> stk;
	string token = "";
	string pop = "";
	string read = "";
	int tokenIndex = 0;
	int stringIndex = 0;

	cout << endl << "Push $";
	stk.push_back("$");
	cout << endl << "Push P" << endl;
	stk.push_back("P");
	displayStack(stk);

	cout << endl << "Pop " << stk.back();
	pop = stk.back();
	stk.pop_back();
	displayStack(stk);
	read = copyTokens[tokenIndex][stringIndex];
	cout << endl << "Read " << tokens[0];

	while (true)
	{
		if (pop != read)
		{
			cout << endl << "Go to [" + pop + "," + " " + read + "]= ";
			int rowColumn = determineRowColumn(pop, read);
			bool isValid = processRowColumn(stk, rowColumn, copyTokens, tokenIndex);
			if (!isValid)
			{
				cout << endl << "REJECT";
				return;
			}

			cout << endl << "Pop " << stk.back();
			pop = stk.back();
			stk.pop_back();
			displayStack(stk);
		}
		else
		{
			cout << endl << "[MATCH WITH INPUT]" << endl;
			if (pop == "$" && read == "$")
			{
				cout << endl << "ACCEPTED" << endl;
				return;
			}
			if (stringIndex == copyTokens[tokenIndex].size() - 1)
			{
				cout << endl << "Pop " << stk.back();
				pop = stk.back();
				stk.pop_back();
				displayStack(stk);
				tokenIndex++;
				stringIndex = 0;
				cout << endl << "Read " << tokens[tokenIndex][stringIndex];
				read = copyTokens[tokenIndex][stringIndex];
			}
			else
			{
				cout << endl << "Pop " << stk.back();
				pop = stk.back();
				stk.pop_back();
				displayStack(stk);
				stringIndex++;

				cout << endl << "Read " << tokens[tokenIndex][stringIndex];
				read = copyTokens[tokenIndex][stringIndex];
			}
		}


	}

} // end trace

void displayStack(vector<string> stk)
{
	cout << endl << "Stack: ";
	for (int i = 0; i < stk.size(); ++i)
		cout << stk[i] << " ";
} // end displayStack

bool processRowColumn(vector<string>& stk, int rowColumn, vector<string> copyTokens, int index)
{
	if (rowColumn == 100)
	{

		cout << "program I ; var B begin H end.";
		cout << endl << "Push end. H begin B var ; I program";
		stk.push_back("g");
		stk.push_back("H");
		stk.push_back("c");
		stk.push_back("B");
		stk.push_back("b");
		stk.push_back(";");
		stk.push_back("I");
		stk.push_back("a");
		return true;
	}
	else if (rowColumn == 200)
	{
		if (copyTokens[index] == "p1")
		{
			if (copyTokens[index + 1] == "p2q" && copyTokens[index + 2] == ",")
			{
				cerr << "ERROR: , is missing.";
				exit(2);
			}
		}
		if (copyTokens[index] == "p2q")
		{
			if (copyTokens[index + 1] == "412")
			{
				cerr << "ERROR: expression is expected.";
				exit(2);
			}
		}
		if (copyTokens[index] == "p1" && copyTokens[index + 1] == "p2q")
		{
			cerr << "ERROR: expression is expected.";
			exit(2);
		}
		cout << "LX";
		cout << endl << "Push X L";
		stk.push_back("X");
		stk.push_back("L");
		return true;
	}
	else if (rowColumn == 300)
	{
		if (copyTokens[index] == "p1")
		{
			if (copyTokens[index + 1] == "p2q" || copyTokens[index + 1] == "33")
			{
				cerr << "ERROR: expression is missing.";
				exit(2);
			}
		}
		cout << "DX";
		cout << endl << "Push X D";
		stk.push_back("X");
		stk.push_back("D");
		return true;
	}
	else if (rowColumn == 400)
	{
		cout << "C:G;";
		cout << endl << "Push ; G : C";
		stk.push_back(";");
		stk.push_back("G");
		stk.push_back(":");
		stk.push_back("C");
		return true;
	}
	else if (rowColumn == 500)
	{
		if (copyTokens[index + 1] == ",")
		{
			cout << "I,C";
			cout << endl << "Push C , I";
			stk.push_back("C");
			stk.push_back(",");
			stk.push_back("I");
			return true;
		}
		else
		{
			cout << "I";
			cout << endl << "Push I";
			stk.push_back("I");
			return true;
		}
	}
	else if (rowColumn == 600)
	{
		if (copyTokens[index] == "d.")
		{
			cerr << "ERROR: end. is expected.";
			exit(2);
		}
		int i = index;
		while (i < copyTokens.size())
		{
			if (copyTokens[i] == ";")
			{
				break;
			}
			++i;
		}
		if (i == copyTokens.size())
		{
			cout << "ERROR: ; is missing.";
			exit(2);
			return false;
		}
		if (copyTokens[i + 1] != "g")
		{
			cout << "JH";
			cout << endl << "Push H J";
			stk.push_back("H");
			stk.push_back("J");
			return true;
		}
		else
		{
			cout << "J";
			cout << endl << "Push J";
			stk.push_back("J");
			return true;
		}

	}
	else if (rowColumn == 700)
	{
		if (copyTokens[index] != "k" && copyTokens[index + 1] == "(" && copyTokens[index + 2] == "pr")
		{
			cerr << "ERROR: display is expected.";
			exit(2);
		}
		if (copyTokens[index] != "k" && copyTokens[index + 1] == "(" && copyTokens[index + 2] == "e")
		{
			cerr << "ERROR: display is expected.";
			exit(2);
		}
		if (copyTokens[index + 2] == "e")
		{
			cout << "display (\"value=\", I);";
			cout << endl << "Push ; ) I , \"value=\" ( display";
			stk.push_back(";");
			stk.push_back(")");
			stk.push_back("I");
			stk.push_back(",");
			stk.push_back("e");
			stk.push_back("(");
			stk.push_back("k");
			return true;
		}
		else
		{
			cout << "display (I);";
			cout << endl << "Push ; ) I ( display";
			stk.push_back(";");
			stk.push_back(")");
			stk.push_back("I");
			stk.push_back("(");
			stk.push_back("k");
			return true;
		}


	}
	else if (rowColumn == 800)
	{
		cout << "I=E;";
		cout << endl << "Push ; E = I";
		stk.push_back(";");
		stk.push_back("E");
		stk.push_back("=");
		stk.push_back("I");
		return true;
	}
	else if (rowColumn == 900)
	{
		cout << "TQ";
		cout << endl << "Push Q T";
		stk.push_back("Q");
		stk.push_back("T");
		return true;
	}
	else if (rowColumn == 1000)
	{
		cout << "+TQ";
		cout << endl << "Push Q T +";
		stk.push_back("Q");
		stk.push_back("T");
		stk.push_back("+");
		return true;
	}
	else if (rowColumn == 1100)
	{
		cout << "-TQ";
		cout << endl << "Push Q T -";
		stk.push_back("Q");
		stk.push_back("T");
		stk.push_back("-");
		return true;
	}
	else if (rowColumn == 1200)
	{
		cout << "FR";
		cout << endl << "Push R F";
		stk.push_back("R");
		stk.push_back("F");
		return true;
	}
	else if (rowColumn == 1300)
	{
		if (copyTokens[index] == "*" && copyTokens[index + 2] == "+")
		{
			cerr << "ERROR: ( is missing.";
			exit(3);
		}
		if (copyTokens[index + 1] == "p2q")
		{
			cout << "Error: ( is missing.";
			exit(3);
		}
		cout << "*FR";
		cout << endl << "Push R F *";
		stk.push_back("R");
		stk.push_back("F");
		stk.push_back("*");
		return true;
	}
	else if (rowColumn == 1400)
	{
		cout << "/FR";
		cout << endl << "Push R F /";
		stk.push_back("R");
		stk.push_back("F");
		stk.push_back("/");
		return true;
	}
	else if (rowColumn == 1500)
	{
		if (copyTokens[index] == "(")
		{
			if (copyTokens[index + 1] == "+")
			{
				cerr << "ERROR: identifier is expected.";
				exit(2);
			}
		}
		cout << copyTokens[index];
		cout << "(E)";
		cout << endl << "Push ) E (";
		stk.push_back(")");
		stk.push_back("E");
		stk.push_back("(");
		return true;
	}
	else if (rowColumn == 1600)
	{
		cout << "I";
		cout << endl << "Push I";
		stk.push_back("I");
		return true;
	}
	else if (rowColumn == 1700)
	{
		cout << "N";
		cout << endl << "Push N";
		stk.push_back("N");
		return true;
	}
	else if (rowColumn == 1800)
	{
		cout << "SDY";
		cout << endl << "Push Y D S";
		stk.push_back("Y");
		stk.push_back("D");
		stk.push_back("S");
		return true;
	}
	else if (rowColumn == 1900)
	{
		cout << "DY";
		cout << endl << "Push Y D";
		stk.push_back("Y");
		stk.push_back("D");
		return true;
	}
	else if (rowColumn == 2100)
	{
		if (copyTokens[index] != "f")
		{
			cerr << "ERROR: integer is expected.";
			exit(2);
		}
		cout << "integer";
		cout << endl << "Push integer";
		stk.push_back("f");
		return true;
	}
	else if (rowColumn == 2200)
	{
		cout << "A";
		cout << endl << "Push A";
		stk.push_back("A");
		return true;
	}
	else if (rowColumn == 2300)
	{
		cout << "+";
		cout << endl << "Push +";
		stk.push_back("+");
		return true;
	}
	else if (rowColumn == 2400)
	{
		cout << "-";
		cout << endl << "Push -";
		stk.push_back("-");
		return true;
	}
	else if (rowColumn == 2500)
	{
		if (copyTokens[index] == "p1")
		{
			if (copyTokens[index + 1] == "p2q")
			{
				cerr << "ERROR: , is missing.";
				exit(2);
			}
		}
		cout << "p";
		cout << endl << "Push p";
		stk.push_back("p");
		return true;
	}
	else if (rowColumn == 2600)
	{
		if (copyTokens[index] == "p2q")
		{
			if (copyTokens[index + 1] == "2")
			{
				cerr << "ERROR: expression is expected.";
				exit(3);
			}
		}
		if (copyTokens[index] == "p2q" && copyTokens[index + 1] == "pr")
		{
			cerr << "ERROR: , is missing.";
			exit(2);
		}
		cout << "q";
		cout << endl << "Push q";
		stk.push_back("q");
		return true;
	}
	else if (rowColumn == 2700)
	{
		if (copyTokens[index] == "pr")
		{
			if (copyTokens[index + 1] == "p1")
			{
				cerr << "ERROR: expression is expected.";
				exit(2);
			}
		}
		cout << "r";
		cout << endl << "Push r";
		stk.push_back("r");
		return true;
	}
	else if (rowColumn == 2800)
	{
		cout << "s";
		cout << endl << "Push s";
		stk.push_back("s");
		return true;
	}
	else if (rowColumn == 10)
	{
		cout << "0";
		cout << endl << "Push 0";
		stk.push_back("0");
		return true;
	}
	else if (rowColumn == 20)
	{
		if (copyTokens[index] == "p1")
		{
			if (copyTokens[index + 2] == ";")
			{
				cerr << "ERROR: identifier is expected.";
				exit(3);
			}
		}
		cout << "1";
		cout << endl << "Push 1";
		stk.push_back("1");
		return true;
	}
	else if (rowColumn == 30)
	{
		if (copyTokens[index] == "2")
		{
			if (copyTokens[index + 1] == "pr")
			{
				cerr << "Error: expression is expected.";
				exit(3);
			}
		}
		cout << "2";
		cout << endl << "Push 2";
		stk.push_back("2");
		return true;
	}
	else if (rowColumn == 40)
	{
		cout << "3";
		cout << endl << "Push 3";
		stk.push_back("3");
		return true;
	}
	else if (rowColumn == 50)
	{
		cout << "4";
		cout << endl << "Push 4";
		stk.push_back("4");
		return true;
	}
	else if (rowColumn == 60)
	{
		cout << "5";
		cout << endl << "Push 5";
		stk.push_back("5");
		return true;
	}
	else if (rowColumn == 70)
	{
		cout << "6";
		cout << endl << "Push 6";
		stk.push_back("6");
		return true;
	}
	else if (rowColumn == 80)
	{
		cout << "7";
		cout << endl << "Push 7";
		stk.push_back("7");
		return true;
	}
	else if (rowColumn == 90)
	{
		cout << "8";
		cout << endl << "Push 8";
		stk.push_back("8");
		return true;
	}
	else if (rowColumn == 99)
	{
		cout << "9";
		cout << endl << "Push 9";
		stk.push_back("9");
		return true;
	}
	else if (rowColumn == 2000)
	{
		if (copyTokens[index] == ")" && copyTokens[index + 2] == "d.")
		{
			cerr << "ERROR: end. is expected.";
			exit(2);
		}
		if (copyTokens[index] == ":")
		{
			if (copyTokens[index + 1] != "f")
			{
				cerr << "ERROR: integer is expected.";
				exit(2);
			}
		}
		cout << "lambda";
		return true;
	}
	else if (rowColumn == 4000)
	{
		cout << "W";
		cout << endl << "Push W";
		stk.push_back("W");
		return true;
	}
	else if (rowColumn == 3001)
	{
		// print error if program is missing from file or mispelled
		cout << "Error: program is expected. \n";
		exit(2);

	}
	else if (rowColumn == 3002)
	{
		// print error message if ; is missing
		cout << "Error: ; is missing. \n";
		exit(3);
	}
	else if (rowColumn == 3003)
	{
		// print error message if integer is missing or misspelled
		cout << "Error: integer is expected. \n";
		exit(2);
	}
	else if (rowColumn == 3004)
	{
		// print error message if : is missing
		cout << "Error: : is missing \n";
		exit(3);
	}
	else if (rowColumn == 3009)
	{
		cout << "Error: display is expected. \n";
		exit(2);
	}
	else if (rowColumn == 3010)
	{
		cout << "Error: expression is expected.";
		exit(3);
	}
	else if (rowColumn == 3011)
	{
		cout << "Error: end. is expected.";
		exit(2);
	}
	else if (rowColumn == 3012)
	{
		cout << "ERROR: identifier expected.";
		exit(2);
	}
	else if (rowColumn == 3013)
	{
		cerr << "ERROR: \"value=\" is expected.";
		exit(2);
	}
	else if (rowColumn == 3014)
	{
		cerr << "ERROR: expression is expected.";
		exit(2);
	}
	else
	{
		return false;
	}
} // end processrowColumn

int determineRowColumn(string pop, string read)
{
	int row = -1;
	int column = -1;

	if (pop == "P")
		row = 0;
	else if (pop == "I")
		row = 1;
	else if (pop == "X")
		row = 2;
	else if (pop == "B")
		row = 3;
	else if (pop == "C")
		row = 4;
	else if (pop == "G")
		row = 5;
	else if (pop == "H")
		row = 6;
	else if (pop == "J")
		row = 7;
	else if (pop == "W")
		row = 8;
	else if (pop == "A")
		row = 9;
	else if (pop == "E")
		row = 10;
	else if (pop == "Q")
		row = 11;
	else if (pop == "T")
		row = 12;
	else if (pop == "R")
		row = 13;
	else if (pop == "F")
		row = 14;
	else if (pop == "N")
		row = 15;
	else if (pop == "Y")
		row = 16;
	else if (pop == "S")
		row = 17;
	else if (pop == "D")
		row = 18;
	else if (pop == "L")
		row = 19;
	else if (pop == "b")
	{
		cout << "Error: var is expected. \n";
		exit(2);
	}
	else if (pop == ":")
		cout << "Error: , is missing. ";
	else if (pop == "c")
	{
		cout << "Error: begin is expected. ";
		exit(2);
	}
	else if (pop == "(")
	{
		cout << "Error: ( is missing. ";
		exit(2);
	}
	else if (pop == ")")
	{
		cout << "Error: ) is missing. ";
		exit(3);
	}
	else if (pop == ";")
	{
		cout << "Error: ; is missing. ";
		exit(2);
	}
	else if (pop == ",")
	{
		cout << "Error: , is missing. ";
		exit(2);
	}

	if (read == "a")
		column = 0;
	else if (read == "b")
		column = 1;
	else if (read == "c")
		column = 2;
	else if (read == "g")
		column = 3;
	else if (read == "f")
		column = 4;
	else if (read == "k")
		column = 5;
	else if (read == "e")
		column = 6;
	else if (read == ",")
		column = 7;
	else if (read == ";")
		column = 8;
	else if (read == ":")
		column = 9;
	else if (read == "(")
		column = 10;
	else if (read == ")")
		column = 11;
	else if (read == "+")
		column = 12;
	else if (read == "-")
		column = 13;
	else if (read == "*")
		column = 14;
	else if (read == "/")
		column = 15;
	else if (read == "=")
		column = 16;
	else if (read == "0")
		column = 17;
	else if (read == "1")
		column = 18;
	else if (read == "2")
		column = 19;
	else if (read == "3")
		column = 20;
	else if (read == "4")
		column = 21;
	else if (read == "5")
		column = 22;
	else if (read == "6")
		column = 23;
	else if (read == "7")
		column = 24;
	else if (read == "8")
		column = 25;
	else if (read == "9")
		column = 26;
	else if (read == "p")
		column = 27;
	else if (read == "q")
		column = 28;
	else if (read == "r")
		column = 29;
	else if (read == "s")
		column = 30;
	else if (read == "$")
		column = 31;
	else if (read == "n" || read == ".")
	{
		cerr << "ERROR: end. is expected.";
		exit(2);
	}
	else if (read == "\"")
	{
		cerr << "ERROR: \"value=\" is expected.";
		exit(2);
	}
	else if (read == "d")
	{
		cerr << "ERROR: display is expected.";
		exit(2);
	}
	else if (read == "o")
	{
		cerr << "ERROR: program is expected.";
		exit(2);
	}
	else if (read == "m")
	{
		cerr << "ERROR: program is expected.";
		exit(2);
	}
	else if (read == "i")
	{
		cerr << "ERROR: display is expected.";
		exit(2);
	}
	else if (read == "l")
	{
		cerr << "ERROR: display is expected.";
		exit(2);
	}
	else if (read == "y")
	{
		cerr << "ERROR: display is expected.";
		exit(2);
	}

	int rowColumn = table[row][column];
	return rowColumn;


} // end processRowColumn

void split(const string& s, char delim, vector<string>& tokens)
{
	int tokenStart = 0; // Starting position of the next token

	// Find the first occurrence of the delimiter.
	int delimPosition = s.find(delim);

	if (delimPosition == string::npos)
	{
		tokens.push_back(s);
	}

	// While we haven't run out of delimiters...
	while (delimPosition != string::npos)
	{
		// Extract the token.
		string tok = s.substr(tokenStart, delimPosition - tokenStart);

		// Push the token onto the tokens vector.
		tokens.push_back(tok);
		// Move delimPosition to the next character position.
		delimPosition++;

		// Move tokenStart to delmiPosition.
		tokenStart = delimPosition;

		// Find the next occurrence of the delimiter.
		delimPosition = s.find(delim, delimPosition);

		// If no more delimiters, extract the last token.
		if (delimPosition == string::npos)
		{
			// Extract the token.
			string tok = s.substr(tokenStart, s.length() - tokenStart);

			// Push the token onto the vector.
			tokens.push_back(tok);
		}


	}

} // end split

void format_File(std::string unpreparedFileName, std::string fileName)
{
	ifstream inFile(unpreparedFileName);
	ofstream outFile(fileName);
	string line;
	if (!inFile.is_open())
	{
		cout << "Error opening file" << endl;
		exit(0);
	}
	while (getline(inFile, line))
	{
		bool isComment = false;
		std::string newLine = "";

		if (line.find("//") == 0)
		{
			isComment = true;
		}
		else
		{
			size_t commentPos = line.find("//");
			if (commentPos != string::npos)
			{
				if (commentPos == line.size() - 2)
				{
					isComment = true;
				}
				else
				{
					newLine = line.substr(0, commentPos);
				}
			}
			else
			{
				newLine = line;
			}

			int pos = 0;
			char operators[] = { '=', '+', ';' , ',', ')' };
			for (int i = 0; i < newLine.size(); i++)
			{
				if (std::find(std::begin(operators), std::end(operators), newLine[i]) != std::end(operators))
				{
					if (i == 0 || i == newLine.size() - 1)
					{
						// operator is at the beginning or end of the line, skip it
						continue;
					}
					if (newLine[i - 1] != ' ' || newLine[i + 1] != ' ')
					{
						if (newLine.substr(i - 5, 6) == "value=")
						{
							// "value=" should be treated as one token, so don't add spaces around it
							continue;
						}

						newLine = newLine.substr(0, i) + " " + newLine[i] + " " + newLine.substr(i + 1);
						i += 2;
					}
				}
			}

		}

		std::istringstream is(newLine);
		std::ostringstream os;
		std::string word;

		while (is >> word)
		{
			os << word << " ";
		}

		newLine = os.str();
		newLine = newLine.substr(0, newLine.size() - 1);
		if (!isComment && !newLine.empty())
		{
			outFile << newLine << "\n";
		}
	}
	inFile.close();
	outFile.close();
} // end format_File

bool isPlusMissing(const string& line) {
	size_t plusPos = line.find("+");
	size_t semicolonPos = line.find(";");
	return (plusPos == string::npos || plusPos > semicolonPos);
}

void pascalToC(vector<string> tokens, vector<string> copyTokens, string fileName)
{
	// Note: all strings in the form \"*\" where * represents any possible string, will be mapped to a string value of "a#"
	//		 all identifiers will be mapped to a string value of "g#"
	//		 all integer values will be mapped to a string value of "k#"
	std::unordered_map<std::string, std::string> encodeMap = {
	{"program", "x"}, {"var", "v"}, {"begin", "b"}, {"end.", "e"}, {"integer", "i"}, {"display", "d"},
	{",", ","}, {";", ";"}, {":", ":"}, {"(", "("}, {")", ")"}, {"+", "+"}, {"-", "-"}, {"*", "*"}, {"/","/"},
	{"=", "="}, {"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"},
	{"7", "7"}, {"8", "8"}, {"9", "9"}, {"p", "p"},{"q", "q"}, {"r", "r"}, {"s", "s"}, {"$", "$"} };

	// The converse to encodeMap
	std::unordered_map<std::string, std::string> decodeMap = {
	{"x", "program"}, {"v", "var"}, {"b", "begin"}, {"e", "end."}, {"i", "integer"}, {"d", "display"},
	{",", ","}, {";", ";"}, {":", ":"}, {"(", "("}, {")", ")"}, {"+", "+"}, {"-", "-"}, {"*", "*"}, {"/","/"},
	{"=", "="}, {"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"}, {"6", "6"},
	{"7", "7"}, {"8", "8"}, {"9", "9"}, {"p", "p"},{"q", "q"}, {"r", "r"}, {"s", "s"}, {"$", "$"} };

	// Open file
	std::string inputFileName = "finalp2.txt", outputFileName = "finalp2.cpp";
	std::cout << "\nOpening File \"" << inputFileName << "\"" << std::endl << std::endl;
	std::ifstream inFile(inputFileName, std::ios::in);
	std::cout << inFile.rdbuf();
	inFile.seekg(std::ios::beg);

	// Get encoded stream
	std::stringstream encodedSS = encodeFileContent(inFile, inputFileName, encodeMap, decodeMap);

	// Converting to C++
	std::cout << "\nConverting " << inputFileName << " into C++" << std::endl << std::endl;
	std::stringstream cppSS = cppConversion(encodedSS, encodeMap, decodeMap);
	std::cout << cppSS.rdbuf() << std::endl << std::endl;
	cppSS.seekg(std::ios::beg);
	std::ofstream outFile(outputFileName, std::ios::trunc);
	outFile << cppSS.rdbuf();
	if (outFile.fail())
	{
		std::cerr << "Error occured while creating " << outputFileName << std::endl << std::endl;
		outFile.close();
		inFile.close();
		exit(1);
	}
	std::cout << "Conversion into C++ was successful." << std::endl;
	std::cout << outputFileName << " was created" << std::endl;
	outFile.close();
	inFile.close();
}

std::stringstream encodeFileContent(std::ifstream& inFile, const std::string& inFileName,
	std::unordered_map<std::string, std::string>& encodeMap, std::unordered_map<std::string, std::string>& decodeMap)
{
	// opening file if not open already
	if (!inFile.is_open())
	{
		inFile.open(inFileName, std::ios::in);
		if (inFile.fail())
		{
			std::cerr << "Failed to open file \"" << inFileName << "\"\nClosing Program";
			exit(2);
		}
	}

	std::stringstream encodeSS;
	std::string stringBuffer, stringId, identifierId, numberId;
	char stringCodePrefix = 'a', identifierCodePrefix = 'g', numberCodePrefix = 'k';
	size_t stringNum = 0, identifierNum = 0, numberNum = 0;
	bool isMultiWordString = false;
	std::string token, line;

	while (std::getline(inFile, line))
	{
		std::stringstream ssLine(line);
		while (ssLine >> token)
		{
			try
			{
				encodeSS << encodeMap.at(token) << (ssLine.eof() ? " \n" : " ");
			}
			catch (const std::exception&)
			{
				// Check if a string was found
				size_t quotationIndex = token.find("\"");
				if (quotationIndex != std::string::npos)
				{
					isMultiWordString = true;
					// Check if it's a single word string
					size_t secondQuotation = token.find("\"", quotationIndex + 1);
					if (secondQuotation != std::string::npos)
					{
						// Check for duplicate entries
						if (encodeMap.find(token) == encodeMap.end())
						{
							stringId = stringCodePrefix + std::to_string(stringNum);
							encodeMap[token] = stringId;
							decodeMap[stringId] = token;
							encodeSS << stringId << (ssLine.eof() ? " \n" : " ");
							stringNum++;
						}
						isMultiWordString = false;
					}
					// String has multiple words
					while (isMultiWordString && !ssLine.eof())
					{
						stringBuffer += token + " ";
						ssLine >> token;
						if (token.find("\"") != std::string::npos)
						{
							stringBuffer += token;
							// Check for duplicate entries
							if (encodeMap.find(stringBuffer) == encodeMap.end())
							{
								stringId = stringCodePrefix + std::to_string(stringNum);
								encodeMap[stringBuffer] = stringId;
								decodeMap[stringId] = stringBuffer;
								encodeSS << stringId << (ssLine.eof() ? " \n" : " ");
								stringNum++;
							}
							stringBuffer.erase();
							isMultiWordString = false;
						}
					}
					// Missing right quotation mark for string
					if (isMultiWordString)
					{
						std::cerr << "REJECTED\nMissing a quotation mark: " << stringBuffer << std::endl;
						exit(2);
					}
				}
				// Identifier or number was found
				else
				{
					// Number was found
					if (token[0] == '+' || token[0] == '-' || std::isdigit(token[0]))
					{
						if (encodeMap.find(token) == encodeMap.end())
						{
							// todo Not sure if i want numbers to be coded or not
							numberId = numberCodePrefix + std::to_string(numberNum);
							encodeMap[token] = numberId;
							decodeMap[numberId] = token;
							encodeSS << numberId << (ssLine.eof() ? " \n" : " ");
							numberNum++;
						}
					}
					// Identifier was found
					else
					{
						if (encodeMap.find(token) == encodeMap.end())
						{
							identifierId = identifierCodePrefix + std::to_string(identifierNum);
							encodeMap[token] = identifierId;
							decodeMap[identifierId] = token;
							encodeSS << identifierId << (ssLine.eof() ? "\n" : " ");
							identifierNum++;
						}
					}
				}
			}
		}
	}
	return encodeSS;
}

std::stringstream cppConversion(std::stringstream& encodedSS, std::unordered_map<std::string, std::string> encodeMap, std::unordered_map<std::string, std::string> decodeMap)
{
	std::stringstream cppSS, declistSS, statlistSS, buffer;
	std::string token;
	encodedSS.seekg(std::ios::beg);
	cppSS << (isDisplayUsed(encodedSS, encodeMap) ? "#include<iostream>\n" : "");
	cppSS << "using namespace std;\n";
	cppSS << "int main()\n";
	cppSS << "{\n";

	// dec-list portion
	declistSS = getContentsBetween(encodedSS, encodeMap["var"], encodeMap["begin"]);
	while (std::getline(declistSS, token, ';'))
	{
		// data type
		if (token.find(encodeMap["integer"]) != std::string::npos)
		{
			cppSS << "\tint ";
			std::stringstream line(token);
			while (line >> token)
			{
				if (token == ":") {
					cppSS << ";\n";
					break;
				}
				cppSS << decodeMap[token] << " ";
			}
		}
	}

	// stat-list portion
	statlistSS = getContentsBetween(encodedSS, encodeMap["begin"], encodeMap["end."]);
	while (std::getline(statlistSS, token, ';'))
	{
		// remove a space at the end of the the line. 
		token.erase(token.length() - 1, 1);

		// line has display in it
		if (token.find(encodeMap["display"]) != std::string::npos)
		{
			bool startDisplay = false, endDisplay = false;
			cppSS << "\tcout << ";
			std::stringstream line(token);
			while (line >> token)
			{
				if (token == "(") startDisplay = true;
				else if (token == ",")
				{
					cppSS << "<< ";
					continue;
				}
				else if (token == ")") endDisplay = true;
				else if (startDisplay) cppSS << decodeMap[token] << " ";
				if (endDisplay)
				{
					cppSS << " << endl ;\n";
					break;
				}
			}
		}
		// operations
		else if (token.length() > 0)
		{
			std::stringstream line(token);
			cppSS << "\t";
			while (line >> token)
				cppSS << decodeMap[token] << " ";
			cppSS << ";\n";
		}

	}
	cppSS << "\treturn 0 ;\n";
	cppSS << "}";
	return cppSS;
}

bool isDisplayUsed(std::stringstream& encodedSS, std::unordered_map<std::string, std::string> encodeMap)
{
	encodedSS.clear();
	encodedSS.seekg(0, std::ios::beg);
	std::string token;
	while (std::getline(encodedSS, token))
	{
		std::stringstream line(token);
		while (line >> token)
		{
			if (token.compare(encodeMap["display"]) == 0)
			{
				encodedSS.clear();
				encodedSS.seekg(std::ios::beg);
				return true;
			}
		}
	}
	encodedSS.clear();
	encodedSS.seekg(std::ios::beg);
	return false;
}

std::stringstream getContentsBetween(std::stringstream& encodedSS, std::string start, std::string end)
{
	encodedSS.clear();
	encodedSS.seekg(0, std::ios::beg);
	bool contentsExtracted = false, insideContentBlock = false;
	std::stringstream content;
	std::string token;
	while (!contentsExtracted && !encodedSS.eof())
	{
		std::getline(encodedSS, token);
		std::stringstream line(token);
		while (line >> token)
		{
			if (token == end)
			{
				contentsExtracted = true;
				break;
			}
			else if (token == start) insideContentBlock = true;
			else if (insideContentBlock) content << token << (line.eof() ? "\n" : " ");
		}
	}
	encodedSS.clear();
	encodedSS.seekg(0, std::ios::beg);
	return content;
}

