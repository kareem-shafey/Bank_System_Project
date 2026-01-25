#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <conio.h> // مكتبة getch


using namespace std;

const string ClientFileName = "Clients.txt";

enum enMainMenueOptions {
	ShowOnScreen = 1, AddClient = 2, DeleteClient = 3,
	UpdateClient = 4, FindClient = 5, Exit = 6
};

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool DeleteForMark = false;
};

sClient ReadNewClient(string AccountNumber)
{
	sClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "Enter Pin Code? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}
enMainMenueOptions ReadMainMenueOptions()
{
	short Number = 0;
	do
	{
		cout << "Choose What do you want to do [1 to 6] ? ";
		cin >> Number;
	} while (Number < 1 || Number > 6);
	return (enMainMenueOptions)Number;
}

vector<string> SpiltString(string S1, string Delim = "#//#")
{
	vector<string> vString;
	short pos = 0;
	string sWord;

	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

sClient ConvertLineToRecord(string Line, string Sperator = "#//#")
{
	sClient Client;

	vector<string> vClientData;

	vClientData = SpiltString(Line, Sperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}


vector<sClient> LoadClientDataFromFile(string FileName)
{
	fstream MyFile;
	vector<sClient> vClient;
	sClient Client;

	MyFile.open(FileName, ios::in);

	string Line;
	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}

		MyFile.close();
	}

	return vClient;
}

void PrintClientRecord(vector<sClient> vClient)
{
	for (sClient& Client : vClient)
	{
		cout << "| " << left << setw(20) << Client.AccountNumber;
		cout << "| " << left << setw(15) << Client.PinCode;
		cout << "| " << left << setw(30) << Client.Name;
		cout << "| " << left << setw(15) << Client.Phone;
		cout << "| " << left << setw(15) << Client.AccountBalance;
		cout << endl;
	}
	cout << "-----------------------------------------------------------------------------------------------------------\n";
}

void PrintAllClientData(vector<sClient> vClient)
{
	cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).\n";

	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(15) << "Pin Code";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(15) << "Balance";

	cout << "\n-----------------------------------------------------------------------------------------------------------\n";

	PrintClientRecord(vClient);

}
void ShowMainMenue()
{
	system("cls");
	cout << "=============================================\n";
	cout << "\t\tMain Menue Screen";
	cout << "\n=============================================\n";


	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";

	cout << "==========================================\n";
}

string ReadAccountNumber()
{
	string AccountNumber;
	cout << "\nPlease Enter Account Number? ";
	cin >> AccountNumber;
	return AccountNumber;
}

bool FindClientByAccountNumber(vector<sClient> vClientData, string AccountNumber, sClient& Client)
{

	for (sClient& C : vClientData)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

string ConvertRecordToLine(sClient Client, string Sperator = "#//#")
{
	string DataLine = "";

	DataLine = Client.AccountNumber + Sperator;
	DataLine += Client.PinCode + Sperator;
	DataLine += Client.Name + Sperator;
	DataLine += Client.Phone + Sperator;
	DataLine += to_string(Client.AccountBalance);

	return DataLine;
}


void AddDataLineToFile(string FileName, string Line)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << Line << endl;

	}
	MyFile.close();
}


sClient ChangeClientData(string AccountNumber)
{
	sClient	Client;

	Client.AccountNumber = AccountNumber;

	cout << "Enter PinCode: ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name: ";
	getline(cin, Client.Name);
	cout << "Enter Phone: ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;


}

void AddNewClient(string AccountNumber)
{
	sClient Client;
	Client = ChangeClientData(AccountNumber);
	string DataLine = ConvertRecordToLine(Client);
	AddDataLineToFile(ClientFileName, DataLine);
}



void AddClients(vector<sClient> vClientData)
{

	cout << "\n===================================\n";
	cout << "\tAdd New Client Screen";
	cout << "\n===================================\n";


	cout << "\nAdding New Client:\n";

	string AccountNumber;
	sClient Client;

	char AddMore = 'n';

	do
	{
		AccountNumber = ReadAccountNumber();

		if (FindClientByAccountNumber(vClientData, AccountNumber, Client))
		{

			while (true)
			{

				if (FindClientByAccountNumber(vClientData, AccountNumber, Client))
				{
					cout << "\nClient With [" << AccountNumber << "] already exists, Enter another Account Number? ";
					cin >> AccountNumber;


				}
				else
				{
					AddNewClient(AccountNumber);

					cout << "\nClient Added Successfully, do you want to more Client [Y/N]? ";
					cin >> AddMore;
					break;
				}


			}

		}
		else
		{
			AddNewClient(AccountNumber);
			cout << "\nClient Added Successfully, do you want to more Client [Y/N]? ";
			cin >> AddMore;
		}


	} while (toupper(AddMore) == 'Y');


}

void PrintClientCards(sClient Client)
{
	cout << "\n The Following are the client details:\n";

	cout << "=======================================\n";

	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;

	cout << "=======================================\n";


}

bool MarkClientForDleteByAccountNumber(vector<sClient>& vClient, string AccountNumber)
{
	for (sClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.DeleteForMark = true;
			return true;
		}
	}
	return false;
}

vector<sClient> SaveCleintsDataToFile(vector<sClient> vClient, string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);
	string Line;

	if (MyFile.is_open())
	{
		for (sClient C : vClient)
		{
			if (C.DeleteForMark == false)
			{
				Line = ConvertRecordToLine(C);

				MyFile << Line << endl;
			}
		}
		MyFile.close();
	}
	return vClient;
}

bool DeleteClientData(vector<sClient>& vClient)
{
	cout << "\n===================================\n";
	cout << "\tDelete Client Screen";
	cout << "\n===================================\n";

	sClient Client;
	string AccountNumber = ReadAccountNumber();
	char Answer = 'n';

	if (FindClientByAccountNumber(vClient, AccountNumber, Client))
	{
		PrintClientCards(Client);

		cout << "\nAre you sure you want delete this client [Y/N]? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkClientForDleteByAccountNumber(vClient, AccountNumber);
			SaveCleintsDataToFile(vClient, ClientFileName);

			// Refresh File
			vClient = LoadClientDataFromFile(ClientFileName);

			cout << "\nClient Deleted Sccessfully\n";
			return true;
		}

	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found!\n";
		return false;
	}
}


bool UpdateClientData(vector<sClient>& vClient)
{
	cout << "\n========================================\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n========================================\n";

	sClient Client;
	string AccountNumber = ReadAccountNumber();
	char Answer = 'n';

	if (FindClientByAccountNumber(vClient, AccountNumber, Client))
	{
		PrintClientCards(Client);

		cout << "\nAre you sure you want Update this client [Y/N]? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientData(AccountNumber);
					break;
				}
			}

			SaveCleintsDataToFile(vClient, ClientFileName);

			// Refresh File;

			vClient = LoadClientDataFromFile(ClientFileName);

			cout << "\nClient Updated Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found!\n";
		return false;
	}

}

bool FindClientData(vector<sClient> vClient)
{
	cout << "\n==================================\n";
	cout << "\tFind Client Screen";
	cout << "\n==================================\n";

	string AccountNumber = ReadAccountNumber();
	sClient Client;

	if (FindClientByAccountNumber(vClient, AccountNumber, Client))
	{
		PrintClientCards(Client);
		return true;
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found!\n";
		return false;
	}
}

void ShowOnScreenEndProgram()
{
	cout << "\n===============================\n";
	cout << "\tProgram Ends :-)";
	cout << "\n===============================\n";

}

void ChooseFromTheMainMenu(vector<sClient> vClient, enMainMenueOptions Choose)
{

	switch (Choose)
	{
	case enMainMenueOptions::ShowOnScreen:
		system("cls");
		PrintAllClientData(vClient);
		break;

	case enMainMenueOptions::AddClient:
		system("cls");
		AddClients(vClient);
		break;

	case enMainMenueOptions::DeleteClient:
		system("cls");
		DeleteClientData(vClient);
		break;

	case enMainMenueOptions::UpdateClient:
		system("cls");
		UpdateClientData(vClient);
		break;

	case enMainMenueOptions::FindClient:
		system("cls");
		FindClientData(vClient);
		break;

	case enMainMenueOptions::Exit:
		system("cls");
		ShowOnScreenEndProgram();
		break;
	}
}

int main()
{
	while (true)
	{

		ShowMainMenue();

		enMainMenueOptions ChooseNumber = ReadMainMenueOptions();

		vector<sClient> vClient = LoadClientDataFromFile(ClientFileName);


		ChooseFromTheMainMenu(vClient, ChooseNumber);

		cout << "\n\nPress any key to go back to main menue...";

		_getch(); // انتظار الضغط على أي مفتاح 

		system("CLS");
	}

	system("pause > 0");
}
