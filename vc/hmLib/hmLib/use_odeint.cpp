#include<array>
#include<iostream>
#include<fstream>
#include<iterator>
#include"../../../csv/csvbase.hpp"


void print(char c) {
	switch (c) {
	case '\t':
		std::cout << "\\t";
		break;
	case '\n':
		std::cout << "\\n";
		break;
	case ' ':
		std::cout << "\\s";
		break;
	default:
		std::cout << c;
	}
}

int main(void) {
	std::ifstream fin("test2.csv");
	hmLib::csv::csv_traits Traits;
	std::istreambuf_iterator<char> Beg(fin);
	std::istreambuf_iterator<char> End;

	unsigned int Row = 0;
	unsigned int Col = 0;
	while (Beg != End) {
		std::string Str;
		auto [Sep, Out, State] = hmLib::csv::cell_advance(Beg, End, std::back_inserter(Str),Traits);

		std::cout << "[" << Row << "," << Col << "]";
		switch (State.Mode) {
		case hmLib::csv::encode_mode::full:
			std::cout << "F";
			break;
		case hmLib::csv::encode_mode::simple:
			std::cout << "S";
			break;
		default:
			std::cout << "N";
		}
		std::cout << std::distance(Beg, Sep)<<":";
		for (auto Itr = Str.begin(); Itr != Str.end(); ++Itr) {
			print(*Itr);
		}
		std::cout<<std::endl;

		if (Sep == End) {
			std::cout << "[EOF]" << std::endl;
			break;
		}
		if (*Sep == Traits.sep()) {
			++Col;
		} else if (*Sep == Traits.end()) {
			Col = 0;
			++Row;
		} else {
			std::cout << "[Error End]" << std::endl;
			break;
		}

		Beg = std::next(Sep);
	}

	system("pause");

	return 0;
}
