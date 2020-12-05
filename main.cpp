#include <iostream>
#include <ctime>

#include "Exception.h"
#include "ATC.h"
#include "File.h"

using namespace std;

int main() {

	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	unsigned int dur;
	ATC a;
	ATC b;

	cout << "\n###################################### BIN FILE TEST #######################################\n";
	BinFile f1("test.bin");
	f1.clear();
	f1.write(a.newCall(1103, "test1", 060, 1290901, 1160932, 22.63), 0);
	f1.write(a.newCall(1206, "test2", 185, 1347895, 1238568, 0.02), 1);
	f1.write(a.newCall(1010, "test3", 120, 3456390, 2947525, 0.63), 2);
	f1.write(a.newCall(1408, "test4", 220, 8800555, 9632489, 2.61), 3);
	f1.write(a.newCall(0107, "test5", 320, 7770901, 9793467, 0.23), 4);
	f1.show();
	cout << "REVERSE: \n";
	f1.showReverse();
	cout << "SORT BY COST: \n";
	f1.sort(sortByCost);
	f1.show();
	cout << "SELECT WHERE DATE 03/12/20 DURATION >180 S: \n";
	timeinfo.tm_year = 2020 - 1900;
	timeinfo.tm_mon = 12 - 1;
	timeinfo.tm_mday = 3;
	dur = 180;
	f1.select(mktime(&timeinfo), dur);

	cout << "\n###################################### TXT FILE TEST #######################################\n";
	TxtFile f2("test.txt");
	f2.clear();
	f2.write(a.newCall(1103, "test1", 060, 1290901, 1160932, 22.63), 0);
	f2.write(a.newCall(1206, "test2", 185, 1347895, 1238568, 0.02), 1);
	f2.write(a.newCall(1010, "test3", 120, 3456390, 2947525, 0.63), 2);
	f2.write(a.newCall(1408, "test4", 220, 8800555, 9632489, 2.61), 3);
	f2.write(a.newCall(0107, "test5", 320, 7770901, 9793467, 0.23), 4);
	f2.show();
	cout << "DELETE: st2 \n";
	f2.del("st2");
	f2.show();
	cout << "Search: est3 \n";
	f2.find("est3");

	cout << "\n#################################### BIN TXT FILE TEST #####################################\n";
	BinTxtFile f3("test.txt");
	f3.clear();
	f3.write(a.newCall(1103, "test1", 060, 1290901, 1160932, 22.63), 0);
	f3.write(a.newCall(1206, "test2", 185, 1347895, 1238568, 0.02), 1);
	f3.write(a.newCall(1010, "test3", 120, 3456390, 2947525, 0.63), 2);
	f3.write(a.newCall(1408, "test4", 220, 8800555, 9632489, 2.61), 3);
	f3.write(a.newCall(0107, "test5", 320, 7770901, 9793467, 0.23), 4);
	f3.show();
	cout << "DELETE: st5 \n";
	f3.del("st5");
	f3.show();
	cout << "SORT BY DURATION: \n";
	f3.sort(sortByDuration);
	f3.show();
	
	ATC tmp = f3.read(5);
	cout << tmp << "\n";
}
