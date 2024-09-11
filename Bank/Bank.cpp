#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";

enum enMenu { ShowClinet = 1, AddedClinet = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, Exit };

void StartScreenMenu();

void ResetScreenManu()
{
	system("pause>0");
	system("cls");
	StartScreenMenu();
}

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};


vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable
	// use find() function to get the position of the delimiters
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}
	return vString;
}


sClient ReadNewClient()
{
	sClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);
	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}
sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	vector <string> vClients;
	sClient Client;
	vClients = SplitString(Line, Seperator);
	Client.AccountNumber = vClients[0];
	Client.PinCode = vClients[1];
	Client.Name = vClients[2];
	Client.Phone = vClients[3];
	Client.AccountBalance = stod(vClients[4]);

	return Client;

}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}

void PrintClientCard(sClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.Name;
	cout << "\nPhone : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
}
void PrintClientRecord(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintAllClientsData(vector <sClient> vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s).";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	for (sClient Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	sClient Client;
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

	vector <sClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string line;
		sClient Client;

		while (getline(MyFile, line))
		{
			Client = ConvertLinetoRecord(line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return vClients;

}


bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		string line;
		for (sClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}

	return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(ClientsFileName, vClients);

			vClients = LoadCleintsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answer = 'y';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
		}

		SaveCleintsDataToFile(ClientsFileName, vClients);
		cout << "\n\nClient Updated Successfully.";
		return true;

	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}


void ScreenFindClientByAccountNumber(string AccountNumber, vector <sClient> vClients)
{
	sClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
	}
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}


void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}
void AddClients()
{
	char AddMore = 'Y';
	do
	{
		system("cls");
		cout << "Adding New Client:\n\n";
		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void ScreenShowClient()
{
	system("cls");
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	PrintAllClientsData(vClients);
	ResetScreenManu();
}

void ScreenAddNewClient()
{
	system("cls");
	AddClients();
	ResetScreenManu();
	StartScreenMenu();

}

void ScreenDeleteClient()
{
	system("cls");
	string AccountNumber = ReadClientAccountNumber();
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DeleteClientByAccountNumber(AccountNumber, vClients);
	ResetScreenManu();
}

void ScreenUpdateClient()
{
	system("cls");
	string AccountNumber = ReadClientAccountNumber();
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	UpdateClientByAccountNumber(AccountNumber, vClients);
	ResetScreenManu();
}

void ScreenFindClient()
{
	system("cls");
	string AccountNumber = ReadClientAccountNumber();
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	ScreenFindClientByAccountNumber(AccountNumber, vClients);
	ResetScreenManu();
}

void ExitProgram()
{
	system("cls");
	exit(0);
}

void MyChoice(enMenu ScrrenMenuCho1ice)
{

	switch (ScrrenMenuCho1ice)
	{
	case enMenu::ShowClinet:
		ScreenShowClient();
		break;
	case enMenu::AddedClinet:
		ScreenAddNewClient();
		break;
	case enMenu::DeleteClient:
		ScreenDeleteClient();
		break;
	case enMenu::UpdateClient:
		ScreenUpdateClient();
		break;
	case enMenu::FindClient:
		ScreenFindClient();
		break;
	case enMenu::Exit:
		ExitProgram();
		break;
	default:
		break;
	}

}

enMenu ScrrenMenuCho1ice()
{
	short number = 0;
	cout << "===============================================\n";
	cout << "                  Main Menu Screen             \n";
	cout << "===============================================\n";
	cout << left << setw(37) << "          [1] Show Client List." << endl;
	cout << left << setw(37) << "          [2] Add New Client." << endl;
	cout << left << setw(37) << "          [3] Delete Client." << endl;
	cout << left << setw(37) << "          [4] Update Client Info." << endl;
	cout << left << setw(37) << "          [5] Find Client." << endl;
	cout << left << setw(37) << "          [6] Exit." << endl;
	cout << "===============================================\n";
	cout << "Choose what do you want to do? [1 to 6]: ";

	cin >> number;

	return (enMenu)number;
}

void StartScreenMenu()
{
	MyChoice(ScrrenMenuCho1ice());
}

int main()
{

	StartScreenMenu();

	system("pause>0");
	return 0;
}