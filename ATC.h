#include<iomanip>
using namespace std;
class ATC {
public:
	ATC() {
		for (int i = 0; i < 10; i++)
			town[i] = 0;
		timestamp = 0;
		code = 0;
		cost = 0;
		from = 0;
		to = 0;
		duration = 0;
	}
	ATC& newCall(unsigned int _code, string str, unsigned int _duration, unsigned int _from, unsigned int _to, float _cost) {
		timestamp = time(0);
		code = _code;
		duration = _duration;
		for (int i = 0; i < 10; i++)
			town[i] = 0;
		for (int i = 0; i < 10 && i < str.length(); i++)
			town[i] = str[i];
		cost = _cost;
		from = _from;
		to = _to;
		return *this;
	}
	unsigned long long timestamp;
	int code;
	char town[10];
	unsigned int duration;
	float cost;
	unsigned int from;
	unsigned int to;
};
ostream& operator<< (ostream& out, ATC& obj) {
	struct tm dt;

	char timestr[30];
	const time_t rawtime = (const time_t)obj.timestamp;
	localtime_s(&dt, &rawtime);
	strftime(timestr, sizeof(timestr), "%d.%b.%y %H:%M:%S", &dt);
	out << "" << timestr << " \"" << setw(10) << obj.town << "\" " << setw(6) << obj.code << "        " << setw(7) << obj.from << "    " << setw(7) << obj.to;
	const time_t rawtime1 = (const time_t)obj.duration;
	gmtime_s(&dt, &rawtime1);
	strftime(timestr, sizeof(timestr), "%H:%M:%S", &dt);
	cout << std::fixed << std::setprecision(2);
	out << "     " << timestr << "  " << obj.cost * ceil((double)obj.duration / 60) << " $";
	return out;
}
