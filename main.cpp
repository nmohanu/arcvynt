#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

const int COLWIDTH[4] = {12, 14, 14, 14};

const std::string GREENCOL = "\033[32m";
const std::string REDCOL = "\033[31m";
const std::string IMGCOL = "\033[33m";
const std::string RESCOL = "\033[0m";

const std::string price_script = "fetchprice.py";
const std::string price_script_sod = "fetchdaydiff.py";
const std::string portfname = "portf.txt";
const std::string imgname = "img.txt";

enum SCURR { EUR, USD };

#ifdef USE_EUR
const SCURR display_in = SCURR::EUR;
#else
const SCURR display_in = SCURR::USD;
#endif

const std::map<std::string, SCURR> stocurr = {{"EUR", SCURR::EUR}, {"USD", SCURR::USD}};

struct StockEntry {
	StockEntry(std::string n, int a, SCURR c) : name(n), amount(a), currency(c) {}
	std::string name;
	int amount;
	SCURR currency;
};

typedef std::vector<StockEntry> StockList;

void disable_input() {
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
	std::cout << "\033[?25l";
}

float usd_to_eur(float amount) {
	std::string line;
	std::string pycommand = "python " + price_script + " EUR=X > buffer.txt";
	system(pycommand.c_str());
	std::getline(std::ifstream("buffer.txt"), line);
	return std::round(amount * std::stof(line) * 100) / 100;
}

float eur_to_usd(float amount) {
	std::string line;
	std::string pycommand = "python " + price_script + " EURUSD=X > buffer.txt";
	system(pycommand.c_str());
	std::getline(std::ifstream("buffer.txt"), line);
	return std::round(amount * std::stof(line) * 100) / 100;
}

int main() {
	disable_input();
	std::string line;
	StockList entries{};
	std::vector<std::string> print_queue{}, img{};
	std::ifstream file(portfname);
	std::ifstream imgfile(imgname);

	if (!file || !imgfile) {
		std::cout << "A file could not be opened.\n";
		return 1;
	}

	// Load image.
	while (getline(imgfile, line)) {
		img.push_back(line);
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
		float total_worth = 0.f, total_sod = 0.f;
		for (const StockEntry& s : entries) {
			std::string pycommand = "python " + price_script + " " + s.name + " > buffer.txt";
			system(pycommand.c_str());
			std::getline(std::ifstream("buffer.txt"), line);
			float worth = std::stof(line);

			pycommand = "python " + price_script_sod + " " + s.name + " > buffer.txt";
			system(pycommand.c_str());
			std::getline(std::ifstream("buffer.txt"), line);
			float worth_sod = std::stof(line);

			if (display_in != s.currency) {
				worth = display_in == SCURR::EUR ? usd_to_eur(worth) : eur_to_usd(worth);
				worth_sod = display_in == SCURR::EUR ? usd_to_eur(worth_sod) : eur_to_usd(worth_sod);
			}
			worth *= s.amount;
			worth = std::round(worth * 100) / 100;

			worth_sod *= s.amount;
			worth_sod = std::round(worth_sod * 100) / 100;

			bool profit = worth_sod >= 0;

			std::ostringstream wstr;
			wstr << std::fixed << std::setprecision(2) << worth;

			std::ostringstream diffwstr;
			diffwstr << std::fixed << std::setprecision(2) << worth_sod;

			std::string printcol = profit ? GREENCOL : REDCOL;

			std::ostringstream printentry;
			printentry << std::left << std::setw(COLWIDTH[0]) << (std::to_string(s.amount) + "x ")
					   << std::setw(COLWIDTH[1]) << (s.name + ": ") << printcol << std::setw(COLWIDTH[2]) << wstr.str()
					   << std::setw(COLWIDTH[3]) << diffwstr.str() << RESCOL;
			print_queue.push_back(printentry.str());
			total_worth += worth;
			total_sod += worth_sod;
		}

		system("clear");
		std::cout << std::setw(COLWIDTH[0]) << "Amount" << std::setw(COLWIDTH[1]) << "Name" << std::setw(COLWIDTH[2])
				  << "Value" << std::setw(COLWIDTH[3]) << "+/-" << '\n';

		size_t img_it = 0;
		for (const auto& s : print_queue) {
			std::cout << s << IMGCOL << img[img_it++] << RESCOL << '\n';
		}
		while (img_it++ < img.size() - 1) {
			for (int i : COLWIDTH) std::cout << std::setw(i) << "";
			std::cout << IMGCOL << img[img_it] << RESCOL << '\n';
		}

		bool total_profit = total_sod >= 0;
		std::ostringstream diff;
		diff << std::fixed << std::setprecision(2) << std::abs(total_profit - total_sod);

		std::ostringstream total;
		total << std::fixed << std::setprecision(2) << total_worth;

		std::string printcol = total_profit ? GREENCOL : REDCOL;
		std::cout << std::left << std::setw(COLWIDTH[0]) << "Total:" << std::setw(COLWIDTH[1]) << "" << printcol
				  << std::setw(COLWIDTH[2]) << total.str() << std::setw(COLWIDTH[3]) << diff.str() << RESCOL << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}
