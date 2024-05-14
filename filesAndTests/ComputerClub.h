#pragma once
#include <vector>
#include <map>
#include "myTime.h"

class ComputerClub {
public:
	ComputerClub(int numberOfTableC, Time openC, Time closeC, int priceOfHourC);
	void ClientCome(Time startActivity, std::string clientName);
	void ClientTakeTable(Time startActivity, std::string clientName, int numbOfTable);
	void ClientWait(Time startActivity, std::string clientName);
	void ClientGoAway(Time startActivity, std::string clientName);
	void PrintAtTheEnd();

private:
	int numberOfTables; 
	Time open; 
	Time close; 
	int priceOfHour; 

	std::vector<std::pair<int, Time>> Tables; //calculation of earnings and time spent
	std::vector<std::string> clients; //clients who came
	std::map<int, std::pair<std::string, Time>> ClientAtTable; //which client is at this table and when did he sit down
	std::vector<std::string> clientsWaiting; //customers in the queue
	std::vector<bool> isTableNotBusy; //free tables
};