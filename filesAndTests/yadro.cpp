#include <fstream>
#include <regex>
#include "ComputerClub.h"

void endWithError(std::string line) {
	std::cout << line;
	exit(-1);
}

void GetFileInformation(std::vector<std::string>& lines, std::regex Number, std::string argv) {
	std::string line;

	std::ifstream in(argv);

	std::regex NumberString("^[0-9]+$"); //1 and 3 entering strings
	std::regex OpenClose("^[0-9]{2}:[0-9]{2} [0-9]{2}:[0-9]{2}$"); //open and close time string
	std::regex StringWithID2("^[0-9]{2}:[0-9]{2} (\\d+) [a-z0-9-_]+ (\\d*)$"); //string with event ID 2
	std::regex OtherIDStrings("^[0-9]{2}:[0-9]{2} (\\d+) [a-z0-9-_]+$"); // strings with event ID 1,3,4

	int numberOfTables;
	int count = 0;
	Time prev = Time();
	if (in.is_open()) {
		while (std::getline(in, line)) {
			count++;

			switch(count){
			case 1: {
				if (std::regex_match(line, NumberString)) {
					lines.push_back(line);
					numberOfTables = std::stoi(lines[0]);
				}
				else {
					endWithError(line);
				}
				break;
			}
			case 2: {
				if (std::regex_match(line, OpenClose)) {
					// Check, hours < 24 and minutes < 60
					if (!(isValidTime(line.substr(0, 5)) && isValidTime(line.substr(6)))) {
						endWithError(line);
					}
					if (!(Time(line.substr(0, 5)) <= Time(line.substr(6)))) {
						endWithError(line);
					}
					lines.push_back(line);
				}
				else {
					endWithError(line);
				}
				break;
			}
			case 3: {
				if (std::regex_match(line, NumberString)) {
					lines.push_back(line);
				}
				else {
					endWithError(line);
				}
				break;
			}
			default: {
				std::smatch matches;
				if (std::regex_search(line, matches, Number)) {
					std::string number = matches[0];
					int numb = std::stoi(number);
					if (numb > 4) {
						endWithError(line);
					}
					else if (numb == 2) { //Check event ID = 2
						if (std::regex_match(line, StringWithID2)) {
							if (!(isValidTime(line.substr(0, 5)))) {
								endWithError(line);
							}
							int pos = 0;
							for (int i = line.size() - 1; i > 0; i--) {
								pos = i;
								if (line[i] == ' ')break;
							}
							if ((numberOfTables < std::stoi(line.substr(pos)))) {
								endWithError(line);
							}
							lines.push_back(line);
						}
						else {
							endWithError(line);
						}
					}
					else { //Check event ID = 1,3,4
						if (std::regex_match(line, OtherIDStrings)) {
							if (!(isValidTime(line.substr(0, 5)))) {
								endWithError(line);
							}
							lines.push_back(line);
						}
						else {
							endWithError(line);
						}
					}

					if (count > 4) { //checking the time sequence
						if (!(prev <= Time(line.substr(0, 5)))) {
							endWithError(line);
						}
					}
					prev = Time(line.substr(0, 5));
				}
				else {
					endWithError(line);
				}
				break;
			}
			}
		}
	}
	in.close();
}


int main(int argc, char** argv) {
	std::vector<std::string> lines;

	std::regex Number(" (\\d+) "); //to highlight the event number
	std::regex NumberOfTable(" (\\d*)"); //to highlight the number of the table
	std::regex time("^[0-9]{2}:[0-9]{2}"); // to highlight the time of the event
	std::regex client(" [a-z0-9-_]+"); // for the record name of client

	GetFileInformation(lines, Number, argv[1]); //checking the file for correctness + splitting into lines

	int numberOfTables = std::stoi(lines[0]); 
	Time open(lines[1].substr(0, 5)); 
	Time close(lines[1].substr(6)); 
	int priceOfHour = std::stoi(lines[2]); 

	std::cout << open << std::endl;

	ComputerClub Club = ComputerClub(numberOfTables, open, close, priceOfHour);

	for (int i = 3; i < lines.size(); i++) {
		std::cout << lines[i] << std::endl;
		std::smatch matches;
		//take event time
		std::regex_search(lines[i], matches, time);
		Time startActivity(matches[0]);
		//take event ID
		std::regex_search(lines[i], matches, Number);
		int numb = std::stoi(matches[0]);
		lines[i] = lines[i].substr(0, 5) + lines[i].substr(7);
		//take the name of client
		std::regex_search(lines[i], matches, client);
		std::string clientName = matches[0];

		switch (numb)
		{
		case 1: {
			Club.ClientCome(startActivity, clientName);
			break;
		}
		case 2: {
			lines[i] = lines[i].substr(0, 5) + lines[i].substr(5 + clientName.size());
			int numbOfTable;
			std::regex_search(lines[i], matches, NumberOfTable);
			numbOfTable = std::stoi(matches[0]);

			Club.ClientTakeTable(startActivity, clientName, numbOfTable);
			break;
		}
		case 3: {
			Club.ClientWait(startActivity, clientName);
			break;
		}
		case 4: {
			Club.ClientGoAway(startActivity, clientName);
			break;
		}
		}

	}
	Club.PrintAtTheEnd();

	return 0;
}
