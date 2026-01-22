#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const string ClientFileName = "Clients.txt";

void ShowMainMenue();
void ShowTransactionsMenue();

enum enTransactionsMenueOptions { eDepoit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions {
	eListClient = 1, eAddNewClient = 2, eDeleteClient = 3,
	eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eExit = 7
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


short ReadMainMenueOptions()
{
	short Choice = 0;
	do
	{
		cout << "Choose What do you want to do [1 to 7] ? ";
		cin >> Choice;
	} while (Choice < 1 || Choice > 7);
	return Choice;
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

bool  ClientsExistsByAccountNumber(string FileName, string AccountNumber)
{
	fstream MyFile;
	string Line;
	sClient Client;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}

sClient ReadNewClient()
{
	sClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientsExistsByAccountNumber(ClientFileName, Client.AccountNumber))
	{
		cout << "\nClient With [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter Pin Code? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

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

void PrintClientRecordLine(sClient Client)
{
	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(15) << Client.PinCode;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(15) << Client.Phone;
	cout << "| " << left << setw(15) << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
	vector<sClient> vClient = LoadClientDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).\n";

	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(15) << "Pin Code";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(15) << "Balance";

	cout << "\n-----------------------------------------------------------------------------------------------------------\n";

	if (vClient.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In The System!";
	}

	for (sClient& Client : vClient)
	{
		PrintClientRecordLine(Client);
		cout << endl;
	}
	cout << "-----------------------------------------------------------------------------------------------------------\n";

}

string ReadClientsAccountNumber()
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


sClient ChangeClientRecord(string AccountNumber)
{
	sClient	Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode: ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name: ";
	getline(cin, Client.Name);
	cout << "Enter Phone: ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;


}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	string DataLine = ConvertRecordToLine(Client);
	AddDataLineToFile(ClientFileName, DataLine);
}

void AddNewClients()
{
	char AddMore = 'Y';

	do
	{
		cout << "Adding New Client:\n\n";
		AddNewClient();

		cout << "\nClient Added Successfully, do you want to more Client [Y/N]? ";
		cin >> AddMore;

	} while (AddMore == 'Y' || AddMore == 'y');

}

void ShowAddNewClientsScreen()
{

	cout << "\n===================================\n";
	cout << "\tAdd New Client Screen";
	cout << "\n===================================\n";


	AddNewClients();

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

bool MarkClientForDeleteByAccountNumber(vector<sClient>& vClient, string AccountNumber)
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

vector<sClient> SaveClientsDataToFile(vector<sClient> vClient, string FileName)
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

bool DeleteClientByAccountNumber(vector<sClient>& vClients, string AccountNumber)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(vClients, AccountNumber, Client))
	{
		PrintClientCards(Client);

		cout << "\nAre you sure you want delete this client [Y/N]? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkClientForDeleteByAccountNumber(vClients, AccountNumber);
			SaveClientsDataToFile(vClients, ClientFileName);

			// Refresh File
			vClients = LoadClientDataFromFile(ClientFileName);

			cout << "\nClient Deleted Sccessfully.\n";
			return true;
		}

	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found!\n";
		return false;
	}
}

void ShowDeleteClientScreen()
{
	cout << "\n===================================\n";
	cout << "\tDelete Client Screen";
	cout << "\n===================================\n";

	vector<sClient> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadClientsAccountNumber();

	DeleteClientByAccountNumber(vClients, AccountNumber);
}

bool UpdateClientByAccountNumber(vector<sClient>& vClient, string AccountNumber)
{

	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(vClient, AccountNumber, Client))
	{
		PrintClientCards(Client);

		cout << "\n\nAre you sure you want Update this client [Y/N]? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(vClient, ClientFileName);


			cout << "\n\nClient Updated Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found!\n";
		return false;
	}

}


void ShowUpdateClientScreen()
{
	cout << "\n========================================\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n========================================\n";

	vector<sClient> vClient = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadClientsAccountNumber();

	UpdateClientByAccountNumber(vClient, AccountNumber);
}


void ShowFindClientScreen()
{
	cout << "\n==================================\n";
	cout << "\tFind Client Screen";
	cout << "\n==================================\n";

	vector<sClient> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadClientsAccountNumber();
	sClient Client;

	if (FindClientByAccountNumber(vClients, AccountNumber, Client))
		PrintClientCards(Client);
	else
		cout << "\nClient With Account Number [" << AccountNumber << "] Is Not Found!";
}

void ShowEndScreen()
{
	cout << "\n===============================\n";
	cout << "\tProgram Ends :-)";
	cout << "\n===============================\n";

}
void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to main menue...";
	system("pause>0");
	ShowMainMenue();
}


short ReadTransactionsMenueOptions()
{
	short Choice = 0;
	do
	{
		cout << "Choose What do you want to do [1 to 4] ? ";
		cin >> Choice;
	} while (Choice < 1 || Choice > 4);
	return Choice;

}


bool DepositBalanceToClientByAccountNumber(vector<sClient>& vClients, double Amount, string AccountNumber)
{
	char Answer = 'n';



	cout << "\n\nAre you sure you want perform this transactions [Y/N]? ";;
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		for (sClient& C : vClients)
		{
			C.AccountBalance += Amount;
			SaveClientsDataToFile(vClients, ClientFileName);
			cout << "\n\nDone Successfully New Balance = " << C.AccountBalance << endl;
			return true;
		}

	}

	return false;
}

void ShowDepositScreen()
{
	cout << "\n===============================\n";
	cout << "\tDeposit Screen";
	cout << "\n===============================\n";
	string AccountNumber = ReadClientsAccountNumber();
	vector<sClient> vClients = LoadClientDataFromFile(ClientFileName);
	sClient Client;

	while (!FindClientByAccountNumber(vClients, AccountNumber, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist\n";
		AccountNumber = ReadClientsAccountNumber();
	}

	PrintClientCards(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(vClients, Amount, AccountNumber);

}

void GoBackToTransactionsMenue()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowTransactionsMenue();
}

void PrintClientRecordBalanceLine(sClient Client)
{
	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(15) << Client.AccountBalance;

}



void ShowWithdrawScreen()
{
	cout << "\n===============================\n";
	cout << "\tWithdraw Screen";
	cout << "\n===============================\n";

	vector<sClient> vClients = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadClientsAccountNumber();
	sClient Client;

	while (!FindClientByAccountNumber(vClients, AccountNumber, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist\n";
		AccountNumber = ReadClientsAccountNumber();
	}

	PrintClientCards(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;

	//Validate that the amount does not exceeds the balance
	while ((Amount > Client.AccountBalance))
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to = " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Amount;
	}


	DepositBalanceToClientByAccountNumber(vClients, Amount * -1, AccountNumber);
}
void ShowTotalBalances()
{
	vector<sClient> vClients = LoadClientDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";

	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(15) << "Balance";

	cout << "\n-----------------------------------------------------------------------------------------------------------\n";

	double TotalBalances = 0;


	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In The System!";
	}

	for (sClient Client : vClients)
	{
		PrintClientRecordBalanceLine(Client);
		TotalBalances += Client.AccountBalance;
		cout << endl;
	}

	cout << "-----------------------------------------------------------------------------------------------------------\n";

	cout << "\t\t\t\t\t\tTotal Balance = " << TotalBalances << endl;

}

void ShowTotalBalancesScreen()
{
	ShowTotalBalances();
}

void PerformTransactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
	switch (TransactionMenueOption)
	{
	case enTransactionsMenueOptions::eDepoit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case enTransactionsMenueOptions::eWithdraw:
	{
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case enTransactionsMenueOptions::eShowTotalBalance:
	{
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case enTransactionsMenueOptions::eShowMainMenue:
	{
		ShowMainMenue();
		break;
	}
	}
}

void ShowTransactionsMenue()
{
	system("cls");
	cout << "==========================================\n";
	cout << "\tTransactions Menue Screen";
	cout << "\n==========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";

	cout << "==========================================\n";
	PerformTransactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOptions());
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{

	switch (MainMenueOption)
	{
	case enMainMenueOptions::eListClient:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();

		break;

	case enMainMenueOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();

		break;

	case enMainMenueOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();

		break;

	case enMainMenueOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();

		break;

	case enMainMenueOptions::eShowTransactionsMenue:
		system("cls");
		ShowTransactionsMenue();
		break;

	case enMainMenueOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
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
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";

	cout << "==========================================\n";

	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOptions());
}

int main()
{
	ShowMainMenue();
	system("pause > 0");
}
