#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const string ClientFileName = "Clients.txt";
const string UserFileName = "Users.txt";

void ShowMainMenue();
void ShowTransactionsMenue();
void ShowMangeUsersMenue();
void ShowAccessDeniedMessage();
void Login();

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions {
	eListClients = 1, eAddNewClient = 2, eDeleteClient = 3,
	eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eMangeUsers = 7, eExit = 8
};

enum enMangeUserMenueOptions
{
	eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};

enum enMainMenuePermissions
{
	eAll = -1, pListClients = 1, pAddNewClients = 2, pDeleteClients = 4, pUpdateClients = 8, pFindClients = 16, pTransactions = 32, pMangeUsers = 64
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

struct stUser
{
	string UserName;
	string Password;
	int Permissions;
	bool DeleteForMark = false;

};

stUser CurrentUser;

short ReadMainMenueOption()
{
	short Choice = 0;
	do
	{
		cout << "Choose What do you want to do [1 to 8] ? ";
		cin >> Choice;
	} while (Choice < 1 || Choice > 8);
	return Choice;
}

void ShowAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";

}

bool CheckAccessPermissions(enMainMenuePermissions Permissions)
{
	if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
		return true;
	if ((CurrentUser.Permissions & Permissions) == Permissions)
		return true;
	else
		return false;
}

void PrintUserCard(stUser User)
{
	cout << "\n============================================\n";
	cout << "The Following are the user details:";
	cout << "\n============================================\n";

	cout << "Username   : " << User.UserName << endl;
	cout << "Password   : " << User.Password << endl;
	cout << "Permissions:  " << User.Permissions << endl;
	cout << "============================================\n";

}

int ReadPermissionsToSet()
{
	int Permissions = 0;
	char answer = 'n';

	cout << "\nDo you want to give Full access? ";
	cin >> answer;

	if (answer == 'Y' || answer == 'y')
	{
		return -1;
	}

	cout << "\nDo you want to give access to:\n";

	cout << "\nShow Client List [Y/N]? ";
	cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		Permissions += enMainMenuePermissions::pListClients;
	}

	cout << "\nAdd Client [Y/N]? ";
	cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		Permissions += enMainMenuePermissions::pAddNewClients;
	}

	cout << "\nDelete Client [Y/N]? ";
	cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		Permissions += enMainMenuePermissions::pDeleteClients;
	}

	cout << "\nUpdate Client [Y/N]? ";
	cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		Permissions += enMainMenuePermissions::pUpdateClients;
	}

	cout << "\nFind Client [Y/N]? ";
	cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		Permissions += enMainMenuePermissions::pFindClients;

	}

	cout << "\nTransactions [Y/N]? ";
	cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		Permissions += enMainMenuePermissions::pTransactions;

	}

	cout << "\nMange User [Y/N]? ";
	cin >> answer;
	if (answer == 'Y' || answer == 'y')
	{
		Permissions += enMainMenuePermissions::pMangeUsers;


	}

	return Permissions;

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

sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sClient Client;

	vector<string> vClientData;

	vClientData = SpiltString(Line, Seperator);

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

vector<sClient> LoadClientsDataFromFile(string FileName)
{
	fstream MyFile;
	vector<sClient> vClients;
	sClient Client;
	string Line;

	MyFile.open(FileName, ios::in);


	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
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

	if (!CheckAccessPermissions(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector<sClient> vClients = LoadClientsDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";

	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(15) << "Pin Code";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(15) << "Balance";

	cout << "\n-----------------------------------------------------------------------------------------------------------\n";

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In The System!\n";
	}

	for (sClient& Client : vClients)
	{
		PrintClientRecordLine(Client);
		cout << endl;
	}
	cout << "-----------------------------------------------------------------------------------------------------------\n";


}

string ReadClientAccountNumber()
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

stUser ChangeUserRecord(string Username)
{
	stUser User;


	User.UserName = Username;

	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionsToSet();

	return User;

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

	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewClientsScreen()
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pAddNewClients))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n===================================\n";
	cout << "\tAdd New Client Screen";
	cout << "\n===================================\n";

	AddNewClients();

}

void PrintClientCard(sClient Client)
{
	cout << "\nThe Following are the client details:\n";

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
		PrintClientCard(Client);

		cout << "\nAre you sure you want delete this client [Y/N]? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkClientForDeleteByAccountNumber(vClients, AccountNumber);
			SaveClientsDataToFile(vClients, ClientFileName);

			// Refresh File
			vClients = LoadClientsDataFromFile(ClientFileName);

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

	if (!CheckAccessPermissions(enMainMenuePermissions::pDeleteClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n===================================\n";
	cout << "\tDelete Client Screen";
	cout << "\n===================================\n";

	vector<sClient> vClients = LoadClientsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(vClients, AccountNumber);

}

bool UpdateClientByAccountNumber(vector<sClient>& vClient, string AccountNumber)
{

	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(vClient, AccountNumber, Client))
	{
		PrintClientCard(Client);

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


bool FindUserByUsername(vector<stUser> vUsers, string Username, stUser& User)
{
	for (stUser U : vUsers)
	{
		if (U.UserName == Username)
		{
			User = U;
			return true;
		}
	}
	return false;
}

string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
	string DataLine = "";

	DataLine += User.UserName + Seperator;
	DataLine += User.Password + Seperator;
	DataLine += to_string(User.Permissions);

	return DataLine;
}

vector<stUser> SaveUserDataToFile(string FileName, vector<stUser> vUserData)
{
	fstream MyFile;
	string Line;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (stUser User : vUserData)
		{
			if (User.DeleteForMark == false)
			{
				Line = ConvertUserRecordToLine(User);

				MyFile << Line << endl;
			}
		}
		MyFile.close();
	}
	return vUserData;
}

bool UpdateUserByUsername(vector<stUser>& vUsers, string Username)
{
	stUser User;
	char Answer = 'n';

	if (FindUserByUsername(vUsers, Username, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre you sure you want Update this user [Y/N]? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (stUser& C : vUsers)
			{
				if (C.UserName == Username)
				{
					C = ChangeUserRecord(Username);
					break;
				}
			}
			SaveUserDataToFile(UserFileName, vUsers);

			cout << "\n\nClient Updated Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient With Account Number [" << Username << "] Is Not Found!\n";
		return false;
	}
}

void ShowUpdateClientScreen()
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n========================================\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n========================================\n";

	vector<sClient> vClient = LoadClientsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(vClient, AccountNumber);

}

void ShowFindClientScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pFindClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n==================================\n";
	cout << "\tFind Client Screen";
	cout << "\n==================================\n";

	vector<sClient> vClients = LoadClientsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();
	sClient Client;

	if (FindClientByAccountNumber(vClients, AccountNumber, Client))
		PrintClientCard(Client);
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
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(vClients, ClientFileName);
				cout << "\n\nDone Successfully New Balance = " << C.AccountBalance << endl;
				return true;
			}
		}
	}

	return false;

}

void ShowDepositScreen()
{
	cout << "\n===============================\n";
	cout << "\tDeposit Screen";
	cout << "\n===============================\n";
	string AccountNumber = ReadClientAccountNumber();
	vector<sClient> vClients = LoadClientsDataFromFile(ClientFileName);
	sClient Client;

	while (!FindClientByAccountNumber(vClients, AccountNumber, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

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

	vector<sClient> vClients = LoadClientsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();
	sClient Client;

	while (!FindClientByAccountNumber(vClients, AccountNumber, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

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
	vector<sClient> vClients = LoadClientsDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tBalance List (" << vClients.size() << ") Client(s).\n";

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
	else
	{
		for (sClient Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotalBalances += Client.AccountBalance;
			cout << endl;
		}
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
	case enTransactionsMenueOptions::eDeposit:
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
	}
	}
}

void ShowTransactionsMenue()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pTransactions))
	{
		ShowAccessDeniedMessage();
		return;
	}
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
	case enMainMenueOptions::eListClients:
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

	case enMainMenueOptions::eMangeUsers:
		system("cls");
		ShowMangeUsersMenue();
		break;
	case enMainMenueOptions::eExit:
		system("cls");
		Login();
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
	cout << "\t[7] Mange Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "==========================================\n";

	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

stUser ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
	vector<string> vUserData = SpiltString(Line, Seperator);

	stUser User;
	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	User.Permissions = stoi(vUserData[2]);

	return User;
}

bool UserExistByUserName(string FileName, string UserName)
{
	fstream MyFile;
	string Line;
	stUser User;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);

			if (User.UserName == UserName)
			{

				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}

vector<stUser> LoadUserDataFromFile(string FileName)
{
	vector<stUser> vUsers;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUsers.push_back(User);
		}

		MyFile.close();
	}
	return vUsers;
}

bool FindUserByUserNameAndPassword(vector<stUser> vUser, stUser User)
{
	for (stUser U : vUser)
	{
		if (U.UserName == User.UserName)
		{
			if (U.Password == User.Password)
				return true;
		}

	}
	return false;
}

short ReadMangeUsersMenueOption()
{
	short Choice = 0;
	do
	{
		cout << "Choose What do you want to do [1 to 6] ? ";
		cin >> Choice;
	} while (Choice < 1 || Choice > 6);
	return Choice;

}

void PrintUserRecordLine(stUser User)
{
	cout << "| " << left << setw(20) << User.UserName;
	cout << "| " << left << setw(15) << User.Password;
	cout << "| " << left << setw(10) << User.Permissions;
}

void ShowAllUserScreen()
{
	vector<stUser> vUserData = LoadUserDataFromFile(UserFileName);

	cout << "\t\t\tUsers List (" << vUserData.size() << ") User(s).\n";
	cout << "------------------------------------------------------------------------------\n";

	cout << "| " << left << setw(20) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(10) << "Permissions";
	cout << "\n------------------------------------------------------------------------------\n";

	if (vUserData.size() == 0)
	{
		cout << "\t\t\t\tNo Users Available In the System!";
	}
	else
	{
		for (stUser& User : vUserData)
		{
			PrintUserRecordLine(User);
			cout << endl;
		}
	}

	cout << "------------------------------------------------------------------------------\n";

}

void ShowListUsersScreen()
{
	ShowAllUserScreen();
}

void GoBackToManageUserMenue()
{
	cout << "\n\nPress any key to go back to Mange Users\n";
	system("pause> 0");
	ShowMangeUsersMenue();
}

stUser ReadNewUser()
{
	stUser User;

	cout << "Enter Username? ";
	getline(cin >> ws, User.UserName);

	while (UserExistByUserName(UserFileName, User.UserName))
	{
		cout << "User With [" << User.UserName << "] already exists, Enter another Username? ";
		getline(cin >> ws, User.UserName);

	}

	cout << "Enter Password? ";
	getline(cin >> ws, User.Password);


	User.Permissions = ReadPermissionsToSet();

	return User;

}

void AddNewUser()
{
	stUser User;
	User = ReadNewUser();
	string DataLine = ConvertUserRecordToLine(User);
	AddDataLineToFile(UserFileName, DataLine);
}

void AddNewUsers()
{
	char AddMore = 'Y';

	do
	{
		cout << "Adding New User:\n\n";

		AddNewUser();

		cout << "\nUser Added Successfully, do you want to add more Uers [Y/N]? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewUserScreen()
{
	cout << "\n===================================\n";
	cout << "\tAdd New User Screen";
	cout << "\n===================================\n";

	AddNewUsers();
}

bool MarkUserForDeleteByUsername(vector<stUser>& vUser, string Username)
{
	for (stUser& User : vUser)
	{
		if (User.UserName == Username)
		{
			User.DeleteForMark = true;
			return true;
		}
	}
	return false;
}

bool DeleteUserByUsername(vector<stUser>& vUserData, string Username)
{
	stUser User;
	char Answer = 'n';

	if (Username == "Admin")
	{
		system("cls");
		cout << "\n=================================\n";
		cout << "\tYou Connot Delete This User.\n";
		cout << "=================================\n";
		return false;
	}

	if (FindUserByUsername(vUserData, Username, User))
	{
		PrintUserCard(User);

		cout << "\nAre you sure want delete this user [Y/N]? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkUserForDeleteByUsername(vUserData, Username);

			SaveUserDataToFile(UserFileName, vUserData);

			// Referesh File
			vUserData = LoadUserDataFromFile(UserFileName);

			cout << "\n\nUser Deleted Successfully.\n";
			return true;
		}
	}
	else
	{
		cout << "\nUser with Username (" << Username << ") is Not Found!";
		return false;
	}
}

string ReadUsername()
{
	string Username;
	cout << "Please Enter Username? ";
	cin >> Username;
	return Username;
}

void ShowDeleteUserScreen()
{
	cout << "\n===================================\n";
	cout << "\tDelete User Screen";
	cout << "\n===================================\n";

	vector<stUser> vUserData = LoadUserDataFromFile(UserFileName);
	string Username = ReadUsername();

	DeleteUserByUsername(vUserData, Username);
}

void ShowFindUserScreen()
{
	cout << "\n===================================\n";
	cout << "\Find User Screen";
	cout << "\n===================================\n";

	vector<stUser> vUserData = LoadUserDataFromFile(UserFileName);
	string Username = ReadUsername();
	stUser User;

	if (FindUserByUsername(vUserData, Username, User))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "\nUser with Username [" << Username << "] is not found!";
	}
}

void ShowUpdateUserScreen()
{
	cout << "\n===================================\n";
	cout << "\tUpdate User Screen";
	cout << "\n===================================\n";

	vector<stUser> vUserData = LoadUserDataFromFile(UserFileName);
	string Username = ReadUsername();
	UpdateUserByUsername(vUserData, Username);
}

void ShowLoginScreen()
{
	cout << "\n============================\n";
	cout << "\tLogin Screen";
	cout << "\n============================\n";

}


void PerformMangeUsersMenueOption(enMangeUserMenueOptions MangeUsersOptions)
{
	switch (MangeUsersOptions)
	{
	case enMangeUserMenueOptions::eListUsers:
	{
		system("cls");
		ShowListUsersScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enMangeUserMenueOptions::eAddNewUser:
	{
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enMangeUserMenueOptions::eDeleteUser:
	{
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enMangeUserMenueOptions::eUpdateUser:
	{
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enMangeUserMenueOptions::eFindUser:
	{
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUserMenue();
		break;
	}
	case enMangeUserMenueOptions::eMainMenue:
	{
		ShowMainMenue();
		break;
	}
	}

}

void ShowMangeUsersMenue()
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pMangeUsers))
	{
		ShowAccessDeniedMessage();
		return;
	}
	system("cls");

	cout << "=============================================\n";
	cout << "\tMange User Menue Screen";
	cout << "\n=============================================\n";
	cout << "\t[1] List User.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "==========================================\n";
	PerformMangeUsersMenueOption((enMangeUserMenueOptions)ReadMangeUsersMenueOption());
}

void LoadUserInfo(vector<stUser> vUserData, stUser User)
{
	for (stUser U : vUserData)
	{
		if (User.UserName == U.UserName)
		{
			if (User.Password == U.Password)
			{
				CurrentUser = U;

			}
		}
	}
}

void Login()
{
	ShowLoginScreen();

	stUser User;
	cout << "Please Enter Username? ";
	getline(cin >> ws, User.UserName);

	cout << "Please Enter Password? ";
	getline(cin >> ws, User.Password);

	vector<stUser> vUserData = LoadUserDataFromFile(UserFileName);

	while (!FindUserByUserNameAndPassword(vUserData, User))
	{
		system("cls");

		ShowLoginScreen();

		cout << "Invalid UserName/Password!\n";

		cout << "Please Enter Username? ";
		getline(cin >> ws, User.UserName);

		cout << "Please Enter Password? ";
		getline(cin >> ws, User.Password);
	}


	LoadUserInfo(vUserData, User);



	ShowMainMenue();

}

int main()
{
	Login();


	system("pause > 0");
}
