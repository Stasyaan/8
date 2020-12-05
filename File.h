
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iomanip>
#include <functional>
#include <windows.h>
using namespace std;
string padLeft(string str, int count) {// дополнение строки до указанного размера
	string space = "";
	for (int i = 0; i < count - str.length(); i++)
		space += " ";
	space += str;
	return space;
}
string skipSpace(string str) {
	int i;
	for (i = 0; i < str.length() && str[i] == ' '; i++);
	return str.substr(i, str.length() - i);
}
typedef std::function<signed char(ATC, ATC)> fn;

using namespace std;
class File {
public:
	File(string _FName, int size) {
		FName = _FName;
		ATCSize = size;
		nextPos = 0;
	}
	bool operator<<(ATC& obj) {
		return this->write(obj, size());
	}
	void operator>>(ATC& obj) {
		obj = this->read(nextPos % size());
		nextPos++;
	}
	virtual  int size() = 0;
	virtual ATC read(int  num) = 0;
	virtual bool write(ATC data, int num) = 0;
	void clear() {
		ofstream out(FName, ios::binary | ios::trunc);
		out.close();
	}
	void sort(fn cmp) {
		int n = size();
		for (int i = n - 2; i >= 0; i--) {
			ATC x = read(i);
			int j;
			for (j = i + 1; j < n && (cmp(read(j), x) > 0); j++) {
				write(read(j), j - 1);
			}
			write(x, j - 1);
		}
	}
	void del(string sub) {
		cout << "Delete:\n";
		cout << "   Date     Time        Town       Code         From       To        Duration   Cost\n";
		cout << "------------------------------------------------------------------------------------------\n";
		int cut = 0;
		for (int i = 0; i < size(); i++) {
			ATC tmp = read(i);
			if (strstr(tmp.town, sub.c_str()) != nullptr) {
				cout << tmp << "\n";
				cut++;
				for (int j = i; j < size() - cut; j++) {
					write(read(j + 1), j);
				}
			}
		}
		cout << "\n";
		if (cut > 0) {
			HANDLE hFile;
			try {
				hFile = CreateFile(FName.c_str(), GENERIC_WRITE, 0, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				LARGE_INTEGER li;
				li.QuadPart = -cut * ATCSize;
				SetFilePointerEx(hFile, li, NULL, 2);
				SetEndOfFile(hFile);
				CloseHandle(hFile);
			}
			catch (const std::exception& ex) {
				if (hFile != NULL)CloseHandle(hFile);
				std::cerr << "\nCaught an exception: " << ex.what() << "\n";
			}
		}
	}
	void select(unsigned long long time, unsigned long duration) {
		int n = size();
		cout << "   Date     Time        Town       Code         From       To        Duration   Cost\n";
		cout << "------------------------------------------------------------------------------------------\n";

		for (int i = 0; i < n; i++) {
			ATC tmp = read(i);
			if (tmp.timestamp > time && tmp.duration >= duration)cout << tmp << "\n";
		}
	}
	void find(string sub) {
		cout << "Find:\n";
		cout << "   Date     Time        Town       Code         From       To        Duration   Cost\n";
		cout << "------------------------------------------------------------------------------------------\n";
		for (int i = 0; i < size(); i++) {
			ATC tmp = read(i);
			if (strstr(tmp.town, sub.c_str()) != nullptr) {
				cout << tmp << "\n";
			}
			cout << "\n";
		}
	}
	void show() {
		int n = size();
		ATC tmp;
		cout << "Cont: " << n << "\n";
		cout << "   Date     Time        Town       Code         From       To        Duration   Cost\n";
		cout << "------------------------------------------------------------------------------------------\n";
		for (int i = 0; i < n; i++) {
			tmp = read(i);
			cout << tmp << "\n";
		}
		cout << "\n";
	}
	void showReverse() {
		int n = size();
		ATC tmp;
		cout << "Cont: " << n << "\n";
		cout << "   Date     Time        Town       Code         From       To        Duration   Cost\n";
		cout << "------------------------------------------------------------------------------------------\n";
		for (int i = n - 1; i >= 0; i--) {
			tmp = read(i);
			cout << tmp << "\n";
		}
		cout << "\n";
	}
protected:
	int nextPos;
	int ATCSize;
	string FName;
};

class TxtFile :public File {
public:
	TxtFile(string _FName) :File(_FName, 70) {}
	int  size() {
		int n = 0;
		ifstream in;
		try {
			in.open(FName);
			while (in) {
				n++;
				in.ignore(256, '\n');
			}
			in.close();
		}
		catch (const std::exception& ex) {
			if (in.is_open())in.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
		}
		return n - 2;
	}
	ATC read(int num) {
		ifstream in;
		try {
			if (num < 0 || num >= size())throw FileException("out_of_range_exception");
			in.open(FName);
			in.seekg((int)num * 70, ios::beg);
			ATC tmp;
			in >> tmp.timestamp >> tmp.code >> tmp.town >> tmp.duration >> tmp.cost >> tmp.from >> tmp.to;
			in.close();
			return tmp;
		}
		catch (const std::exception& ex) {
			if (in.is_open())in.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
			ATC tmp;
			return tmp;
		}

	}
	bool write(ATC data, int num) {
		ofstream out;
		try {
			if (num < 0)throw FileException("out_of_range_exception");
			out.open(FName, ios::in | ios::ate);
			out.seekp(num * 70, ios::beg);
			out << setw(12) << data.timestamp << setw(5) << data.code << setw(11) << data.town << setw(10) << data.duration << setw(10) << data.cost << setw(10) << data.from << setw(10) << data.to << "\n";
			out.close();
			return true;
		}
		catch (const std::exception& ex) {
			if (out.is_open())out.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
			return false;
		}
	}
};

class BinTxtFile :public File {
public:
	BinTxtFile(string _FName) :File(_FName, 70) {}
	int  size() {
		int n = 0;
		ifstream in;
		try {
			in.open(FName);
			while (in) {
				n++;
				in.ignore(256, '\n');
			}
			in.close();
		}
		catch (const std::exception& ex) {
			if (in.is_open())in.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
		}
		return n - 2;
	}
	ATC read(int  num) {
		ifstream in;
		try {
			if (num < 0 || num >= size())throw FileException("out_of_range_exception");
			in.open(FName);
			ATC tmp;
			string str;
			in.seekg(num * 70, ios::beg);
			char buf[71];
			in.read(buf, 70);
			in.close();
			str = buf;
			tmp.timestamp = stoull(skipSpace(str.substr(0, 12)));
			tmp.code = stoi(skipSpace(str.substr(12, 5)));
			string s = skipSpace(str.substr(17, 11));
			for (int i = 0; i < 10 && i < s.length(); i++)
				tmp.town[i] = s[i];
			tmp.duration = stoul(skipSpace(str.substr(28, 10)));
			tmp.cost = stof(skipSpace(str.substr(38, 10)));
			tmp.from = stof(skipSpace(str.substr(48, 10)));
			tmp.to = stof(skipSpace(str.substr(58, 10)));
			return tmp;
		}
		catch (const std::exception& ex) {
			if (in.is_open())in.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
			ATC tmp;
			return tmp;
		}
	}
	bool write(ATC data, int  num) {
		ofstream out;
		try {
			if (num < 0)throw FileException("out_of_range_exception");
			out.open(FName, ios::in | ios::ate);
			out.seekp(num * 70, ios::beg);
			string str;

			str += padLeft(to_string(data.timestamp), 12);
			str += padLeft(to_string(data.code), 5);
			str += padLeft(data.town, 11);
			str += padLeft(to_string(data.duration), 10);
			str += padLeft(to_string(data.cost), 10);
			str += padLeft(to_string(data.from), 10);
			str += padLeft(to_string(data.to), 10) + "\n";
			for (int i = 0; i < str.length(); i++)
				out.write(&str[i], 1);
			out.close();
			return true;
		}
		catch (const std::exception& ex) {
			if (out.is_open())out.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
			return false;
		}
	}
};

class BinFile :public File {
public:
	BinFile(string _FName) :File(_FName, sizeof(ATC)) {}
	int  size() {
		int n = 0;
		ofstream out;
		try {
			out.open(FName, ios::binary | ios::app);
			out.seekp(0, ios::end);
			n = out.tellp() / sizeof(ATC);
			out.close();
			return n;
		}
		catch (const std::exception& ex) {
			if (out.is_open())out.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
		}
		return n;
	}
	ATC read(int  num) {
		ifstream in;
		try {
			if (num < 0 || num >= size())throw FileException("out_of_range_exception");
			in.open(FName, ios::binary);
			ATC tmp;
			in.seekg(num * sizeof(ATC), ios::beg);
			in.read((char*)& tmp, sizeof(ATC));
			in.close();
			return tmp;
		}
		catch (const std::exception& ex) {
			if (in.is_open())in.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
			ATC tmp;
			return tmp;
		}
	}
	bool write(ATC data, int num) {
		ofstream out;
		try {
			if (num < 0)throw FileException("out_of_range_exception");
			out.open(FName, ios::binary | ios::in);
			out.seekp(num * sizeof(ATC), ios::beg);
			out.write((char*)& data, sizeof(ATC));
			out.close();
			return true;
		}
		catch (const std::exception& ex) {
			if (out.is_open())out.close();
			std::cerr << "\nCaught an exception: " << ex.what() << "\n";
			return false;
		}
	}
};

signed char sortByCost(ATC obj1, ATC obj2) {
	if (obj1.cost * ceil((double)obj1.duration / 60) < obj2.cost * ceil((double)obj2.duration / 60))return 1;
	if (obj1.cost * ceil((double)obj1.duration / 60) > obj2.cost * ceil((double)obj2.duration / 60))return -1;
	return 0;

}
signed char sortByTown(ATC obj1, ATC obj2) {
	return strcmp(obj1.town, obj2.town);
}
signed char sortByTime(ATC obj1, ATC obj2) {
	if (obj1.timestamp < obj2.timestamp)return 1;
	if (obj1.timestamp > obj2.timestamp)return -1;
	return 0;

}
signed char sortByTo(ATC obj1, ATC obj2) {
	if (obj1.to < obj2.to)return 1;
	if (obj1.to > obj2.to)return -1;
	return 0;
}
signed char sortByFrom(ATC obj1, ATC obj2) {
	if (obj1.from < obj2.from)return 1;
	if (obj1.from > obj2.from)return -1;
	return 0;
}
signed char sortByDuration(ATC obj1, ATC obj2) {
	if (obj1.duration < obj2.duration)return 1;
	if (obj1.duration > obj2.duration)return -1;
	return 0;
}
signed char sortByCode(ATC obj1, ATC obj2) {
	if (obj1.code < obj2.code)return 1;
	if (obj1.code > obj2.code)return -1;
	return 0;
}
