#include <algorithm>
#include "ComputerClub.h"

ComputerClub::ComputerClub(int numberOfTableC, Time openC, Time closeC, int priceOfHourC) {
		numberOfTables = numberOfTableC;
		open = openC;
		close = closeC;
		priceOfHour = priceOfHourC;
		Tables.resize(numberOfTables);
		isTableNotBusy.resize(numberOfTables);
		for (int i = 0; i < numberOfTables; i++) {
			Tables[i] = std::make_pair(0, Time(0, 0));
		}
		for (int i = 0; i < numberOfTables; i++) {
			isTableNotBusy[i] = true;
		}
	}
	//event ID 1
	void ComputerClub::ClientCome(Time startActivity, std::string clientName) {
		if (!(open < startActivity)) { //Club is not open
			std::cout << startActivity << " 13 NotOpenYet" << std::endl;
		}
		for (int j = 0; j < clients.size(); j++) { //checking the presence of a client in the club
			if (clients[j] == clientName) {
				std::cout << startActivity << " 13 YouShallNotPass" << std::endl;
				break;
			}
		}
		clients.push_back(clientName);
	}
	//event ID 2
	void ComputerClub::ClientTakeTable(Time startActivity, std::string clientName, int numbOfTable) {
		// checking client in database
		auto itr = std::find(clients.begin(), clients.end(), clientName);
		if (itr == clients.end()) {
			std::cout << startActivity << " 13 ClientUnknown" << std::endl;
		}
		if (isTableNotBusy[numbOfTable - 1] == true) { //check is free
			isTableNotBusy[numbOfTable - 1] = false;
			//transfer accounting + expense calculation
			for (auto find : ClientAtTable) {
				if (find.second.first == clientName) {
					startActivity.subtractTime(find.second.second);
					Tables[numbOfTable - 1].first += startActivity.countPrice(priceOfHour);
					Tables[numbOfTable - 1].second.addTime(startActivity);
					ClientAtTable.erase(find.first);
					break;
				}
			}
			ClientAtTable[numbOfTable - 1] = std::make_pair(clientName, startActivity);
			// search for a client in the waiting database
			auto itr = std::find(clientsWaiting.begin(), clientsWaiting.end(), clientName);
			if (!(itr == clientsWaiting.end())) {
				clientsWaiting.erase(itr);
			}
		}
		else {
			std::cout << startActivity << " 13 PlaceIsBusy" << std::endl;
		}
	}
	// event ID 3
	void ComputerClub::ClientWait(Time startActivity, std::string clientName) {
		for (int j = 0; j < isTableNotBusy.size(); j++) { //checking available tables
			if (isTableNotBusy[j] == true) {
				std::cout << startActivity << " 13 ICanWaitNoLonger!" << std::endl;
				break;
			}
		}
		//checking the number of people waiting
		if (clientsWaiting.size() > numberOfTables)std::cout << startActivity << " 11" << clientName << std::endl;
		else clientsWaiting.push_back(clientName);
	}
	//event ID 4
	void ComputerClub::ClientGoAway(Time startActivity, std::string clientName) {
		//checking client in database
		auto itr = std::find(clients.begin(), clients.end(), clientName);
		//we have not got client in database
		if (itr == clients.end()) {
			std::cout << startActivity << " 13 ClientUnknown" << std::endl;
		}
		else {
			//if it is in the waiting list, then we delete it
			auto itr1 = std::find(clientsWaiting.begin(), clientsWaiting.end(), clientName);
			if (itr1 != clientsWaiting.end()) {
				clientsWaiting.erase(itr1);
			}
			//calculating earnings and usage time
			for (auto find : ClientAtTable) {
				if (find.second.first == clientName) {
					Time forCount = startActivity;
					startActivity.subtractTime(find.second.second);
					Tables[find.first].first += startActivity.countPrice(priceOfHour);
					Tables[find.first].second.addTime(startActivity);

					//we give table for the waiting client
					if (clientsWaiting.size() > 0) {
						ClientAtTable[find.first].first = clientsWaiting[0];
						ClientAtTable[find.first].second = forCount;
						clientsWaiting.erase(clientsWaiting.begin());
						std::cout << forCount << " 12" << ClientAtTable[find.first].first << " " << find.first + 1 << std::endl;
					}
					else {
						ClientAtTable.erase(find.first);
					}

					break;
				}
			}
			clients.erase(itr);
		}
	}
	void ComputerClub::PrintAtTheEnd() {
		//we calculate the earnings and the time of use at closing
		for (auto find : ClientAtTable) {
			Time forCount = close;
			forCount.subtractTime(find.second.second);
			Tables[find.first].first += forCount.countPrice(priceOfHour);
			Tables[find.first].second.addTime(forCount);
		}

		std::sort(clients.begin(), clients.end());

		for (const auto& str : clients) {
			std::cout << close << " 11" << str << std::endl;
		}
		std::cout << close << std::endl;
		// table data output
		for (int i = 0; i < Tables.size(); i++) {
			std::cout << i + 1 << " " << Tables[i].first << " " << Tables[i].second << std::endl;
		}
	}
