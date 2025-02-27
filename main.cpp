#include <unistd.h>

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

const std::string price_script = "fetchprice.py";
const std::string portfname = "portf.txt";

enum SCURR { EUR, USD };
const std::map<std::string, SCURR> stocurr = {{"EUR", SCURR::EUR}, {"USD", SCURR::USD}};

struct StockEntry {
	StockEntry(std::string n, int a, SCURR c) : name(n), amount(a), currency(c) {}
	std::string name;
	int amount;
	SCURR currency;
};

typedef std::vector<StockEntry> StockList;

float usd_to_eur(float amount) {
	std::string line;
	std::string pycommand = "python " + price_script + " EUR=X > buffer.txt";
	system(pycommand.c_str());
	std::getline(std::ifstream("buffer.txt"), line);
	return std::round(amount * std::stof(line) * 100) / 100;
}

int main() {
	std::string line;
	StockList entries{};
	std::vector<std::string> print_queue{};
	std::ifstream file(portfname);
	SCURR display_in = SCURR::EUR;

	if (!file) {
		std::cout << "Portfolio file could not be opened.\n";
		return 1;
	}

	// Import portfolio file.
	while (std::getline(file, line)) {
		std::stringstream linestream(line);
		std::string sname, samt, scurr;
		std::getline(linestream, sname, ',');
		std::getline(linestream, samt, ',');
		std::getline(linestream, scurr);
		entries.push_back(StockEntry(sname, std::stoi(samt), stocurr.find(scurr)->second));
	}

	while (true) {
		print_queue.clear();
		float total_worth = 0.f;
		for (const StockEntry& s : entries) {
			std::string pycommand = "python " + price_script + " " + s.name + " > buffer.txt";
			system(pycommand.c_str());
			std::getline(std::ifstream("buffer.txt"), line);
			float worth = std::stof(line);
			if (display_in != s.currency) {
				worth = usd_to_eur(worth);
			}
			worth *= s.amount;
			worth = std::round(worth * 100) / 100;
			std::ostringstream wstr;
			wstr << std::fixed << std::setprecision(2) << worth;
			print_queue.push_back(std::to_string(s.amount) + "x " + s.name + ": " + wstr.str());
			total_worth += worth;
		}

		system("clear");
		for (const std::string& s : print_queue) {
			std::cout << s << '\n';
		}
		std::cout << "Total worth: " << total_worth << '\n';
		sleep(5);
	}
}
