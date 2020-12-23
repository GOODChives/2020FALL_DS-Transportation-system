#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<map>
#include<algorithm>
using namespace std;


class clock {
public:
	int hour ;              //0<=hour<=23
	int min ;			   //0<=min<=59
	clock()
	{
		hour = 0;
		min = 0;
	}
	clock(int a, int b)
	{
		if (a >= 0 && a <= 23)
			if (b >= 0 && b <= 59)
			{
				hour = a;
				min = b;
				return;
			}
		cout << "ERROR: wrong time!" << endl;
	}

	void display()
	{
		if (hour == 0) cout << " 00";
		else cout << hour;
		cout << " : ";
		if (min == 0) cout << "00 ";
		else cout << min << " ";
	}
	int clk2num()      //In order to compare the which shifts comes first
	{
		return 60 * hour + min;
	}
	bool operator >(clock& temp)
	{
		return this->clk2num() > temp.clk2num();
	}
};



class shift {
public:
	bool operator ==(shift& temp)
	{
		if (type == temp.type && From == temp.From
			&& To == temp.To && start.clk2num() == temp.start.clk2num()
			&& end.clk2num() == temp.end.clk2num() && price == temp.price) return true;
		else return false;
	}
	int type;             // 0---train     1---plane
	string From;
	string To;
	clock start;
	clock end;
	int price;
	void print()
	{
		if (type == 0) cout << "Train" << " ";
		else cout << "Plane" << " ";
		cout << From << "->" << To << " ";
		cout << "Depart:"; start.display();
		cout << "Arrive:"; end.display();
		cout << "Price: " << price << endl;
	}
};

queue<shift> que;
int Min;
void clear_que(queue<shift>& q) 
{
	queue<shift> empty;
	swap(empty, q);
}

class city {
public:
	string name;
	vector<shift> train_departure;
	vector<shift> train_arrival;
	vector<shift> plane_departure;
	vector<shift> plane_arrival;
};

vector<city> City;
map<string, int> city_num;

int TAG[50];         //assume that there would be only less than 50 cities.

void refresh_city()              //inorder to traverse the cities.
{
	for (int i = 0; i < City.size(); ++i)
		city_num[City[i].name] = i;
	for (int i = 0; i < 50; ++i)
		TAG[i] = 0;
}

void add_city()
{
	string s;
	cout << "Please input city_name" << endl;
	cin >> s;
	for (int i = 0; i < City.size(); ++i)
		if (City[i].name == s)
		{
			cout << s << " already exists." << endl;
			return;
		}
	city c1;
	c1.name = s;
	City.push_back(c1);
}

void add_city(string s)
{
	for (int i = 0; i < City.size(); ++i)
		if (City[i].name == s)
		{
			cout << s << " already exists." << endl;
			return;
		}
	city c1;
	c1.name = s;
	City.push_back(c1);
}

void delete_city()
{
	string s;
	cout << "Please input city_name" << endl;
	cin >> s;
	int tag = 0;
	int k;
	for (int i = 0; i < City.size(); ++i)
		if (City[i].name == s)
		{
			tag = 1;
			k = i;
		}
	if (tag == 0)
	{
		cout << "Nonexistent city.try again." << endl;
		return;
	}

	for (int i = 0; i < City[k].plane_departure.size(); ++i)       //delete all the departure flights
	{
		string n1 = City[k].plane_departure[i].To;
		int j;
		for (j = 0; j < City.size(); ++j) if (City[j].name == n1) break;
		vector<shift>::iterator iter = City[j].plane_arrival.begin();
		for (; iter != City[j].plane_arrival.end(); iter++)
		{
			if (iter->From == s) City[j].plane_arrival.erase(iter);
			break;
		}
	}

	for (int i = 0; i < City[k].plane_arrival.size(); ++i)        //delete all the arrival flights
	{
		string n1 = City[k].plane_arrival[i].From;
		int j;
		for (j = 0; j < City.size(); ++j) if (City[j].name == n1) break;
		vector<shift>::iterator iter = City[j].plane_departure.begin();
		for (; iter != City[j].plane_departure.end(); iter++)
		{
			if (iter->To == s) City[j].plane_departure.erase(iter);
			break;
		}
	}

	for (int i = 0; i < City[k].train_departure.size(); ++i)      // delete all the departure shifts
	{
		string n1 = City[k].train_departure[i].To;
		int j;
		for (j = 0; j < City.size(); ++j) if (City[j].name == n1) break;
		vector<shift>::iterator iter = City[j].train_arrival.begin();
		for (; iter != City[j].train_arrival.end(); iter++)
		{
			if (iter->From == s) City[j].train_arrival.erase(iter);
			break;
		}
	}

	for (int i = 0; i < City[k].train_arrival.size(); ++i)        //delete all the arrival shifts
	{
		string n1 = City[k].train_arrival[i].From;
		int j;
		for (j = 0; j < City.size(); ++j) if (City[j].name == n1) break;
		vector<shift>::iterator iter = City[j].train_departure.begin();
		for (; iter != City[j].train_departure.end(); iter++)
		{
			if (iter->To == s) City[j].train_departure.erase(iter);
			break;
		}
	}

	vector<city>::iterator iter = City.begin();
	for (int i = 0; i < k; ++i) iter++;
	City.erase(iter);
}

void add_shift()
{
	cout << "Please input the vehicle_type+start_city+destination+start_time+arrival_time+price." << endl;
	cout << "e.g. 0 Beijing Shanghai 6 30 8 30 600 . in which 0 means train." << endl;
	shift one_run;
	cin >> one_run.type
		>> one_run.From >> one_run.To
		>> one_run.start.hour >> one_run.start.min
		>> one_run.end.hour >> one_run.end.min >> one_run.price;
	int tag1, tag2;
	tag1 = 0; tag2 = 0;
	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == one_run.From) tag1 = 1;
		if (City[i].name == one_run.To) tag2 = 1;
	}
	if (tag1 == 0) add_city(one_run.From);                  //Add a city if not exist
	if (tag2 == 0) add_city(one_run.To);
	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == one_run.From)
			if (one_run.type == 0)	City[i].train_departure.push_back(one_run);
			else City[i].plane_departure.push_back(one_run);
		if (City[i].name == one_run.To)
			if (one_run.type == 0)City[i].train_arrival.push_back(one_run);
			else City[i].plane_arrival.push_back(one_run);
	}
}

void delete_shift()
{
	cout << "Please input the vehicle_type+start_city+destination+start_time+arrival_time+price." << endl;
	cout << "e.g. 0 Beijing Shanghai 6 30 8 30 600 . in which 0 means train." << endl;
	shift one_run;
	cin >> one_run.type
		>> one_run.From >> one_run.To
		>> one_run.start.hour >> one_run.start.min
		>> one_run.end.hour >> one_run.end.min >> one_run.price;

	int tag1 = 0, tag2 = 0;

	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == one_run.From) tag1 = 1;
		if (City[i].name == one_run.To) tag2 = 1;
	}

	if (tag1 == 0 || tag2 == 0)
		cout << "City not recorded.Please check your input." << endl;
	//if city not found

	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == one_run.From)
		{
			int tag = 0;
			if (one_run.type == 0)
			{
				vector<shift>::iterator iter = City[i].train_departure.begin();
				for (; iter != City[i].train_departure.end(); iter++)
				{
					if (*iter == one_run) City[i].train_departure.erase(iter);
					tag = 1;
					break;
				}
			}
			else
			{
				vector<shift>::iterator iter = City[i].plane_departure.begin();
				for (; iter != City[i].plane_departure.end(); iter++)
				{
					if (*iter == one_run) City[i].plane_departure.erase(iter);
					tag = 1;
					break;
				}
			}
			if (tag == 0) {
				cout << "Nonexistent Shift.Please check your input." << endl;
				break;
			}
		}
		if (City[i].name == one_run.To)
		{
			int tag = 0;
			if (one_run.type == 0)
			{
				vector<shift>::iterator iter = City[i].train_arrival.begin();
				for (; iter != City[i].train_arrival.end(); iter++)
				{
					if (*iter == one_run) City[i].train_arrival.erase(iter);
					tag = 1;
					break;
				}
			}
			else
			{
				vector<shift>::iterator iter = City[i].plane_arrival.begin();
				for (; iter != City[i].plane_arrival.end(); iter++)
				{
					if (*iter == one_run) City[i].plane_arrival.erase(iter);
					tag = 1;
					break;
				}
			}
			if (tag == 0) {
				cout << "Nonexistent Shift.Please check your input." << endl;
				break;
			}
		}
	}
}

int time_gap(clock a, clock b)  //This branch assumes that all shifts finish in one day
{
	return b.clk2num() - a.clk2num();
}

clock min2clock(int a)
{
	clock clk;
	clk.hour = a % 60;
	clk.min = a - clk.hour * 60;
	return clk;
}

void print()
{
	cout << "Please input the name of city." << endl;
	string s;
	cin >> s;
	int tag = 0, n;
	for (n = 0; n < City.size(); ++n)
		if (City[n].name == s)
		{
			tag = 1;
			break;
		}
	if (tag == 0)
	{
		cout << "Nonexistent city.Please check your input." << endl;
		return;
	}
	for (int i = 0; i < City[n].train_departure.size(); ++i)
		City[n].train_departure[i].print();
	for (int i = 0; i < City[n].train_arrival.size(); ++i)
		City[n].train_arrival[i].print();
	for (int i = 0; i < City[n].plane_departure.size(); ++i)
		City[n].plane_departure[i].print();
	for (int i = 0; i < City[n].plane_arrival.size(); ++i)
		City[n].plane_arrival[i].print();
}

bool eco_train(string from, string to, int cost, queue<shift> Q, clock clk)
{
	bool succ = 0;
	TAG[city_num[from]] = 1;
	if (from == to)
	{
		if (cost < Min)
		{
			Min = cost;
			que = Q;
		}
		return true;
	}
	int k;
	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == from) k = i;
	}
	for (int i = 0; i < City[k].train_departure.size(); ++i)
		if (City[k].train_departure[i].start > clk&& TAG[city_num[City[k].train_departure[i].To]] == 0)
		{
			queue<shift> temp = Q;
			temp.push(City[k].train_departure[i]);
			if (eco_train(City[k].train_departure[i].To, to,
				cost + City[k].train_departure[i].price,
				temp, City[k].train_departure[i].end))
				succ = true;
		}
	TAG[city_num[from]] = 0;
	if (succ) return true;
	else return false;
}

bool eco_plane(string from, string to, int cost, queue<shift> Q, clock clk)
{
	bool succ = 0;
	TAG[city_num[from]] = 1;
	if (from == to)
	{
		if (cost < Min)
		{
			Min = cost;
			que = Q;
		}
		return true;
	}
	int k;
	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == from) k = i;
	}
	for (int i = 0; i < City[k].plane_departure.size(); ++i)
		if (City[k].plane_departure[i].start > clk&& TAG[city_num[City[k].plane_departure[i].To]] == 0)
		{
			queue<shift> temp = Q;
			temp.push(City[k].plane_departure[i]);
			if (eco_plane(City[k].plane_departure[i].To, to,
				cost + City[k].plane_departure[i].price,
				temp, City[k].plane_departure[i].end))
				succ = true;
		}
	TAG[city_num[from]] = 0;
	if (succ) return true;
	else return false;
}

bool fast_train(string from, string to, int cost, queue<shift> Q, clock clk)
{
	bool succ = 0;
	TAG[city_num[from]] = 1;
	if (from == to)
	{
		if (cost < Min)
		{
			Min = cost;
			que = Q;
		}
		return true;
	}
	int k;
	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == from) k = i;
	}
	for (int i = 0; i < City[k].train_departure.size(); ++i)
		if (City[k].train_departure[i].start > clk&& TAG[city_num[City[k].train_departure[i].To]] == 0)
		{
			queue<shift> temp = Q;
			temp.push(City[k].train_departure[i]);
			if (fast_train(City[k].train_departure[i].To, to,
				cost + time_gap(clk, City[k].train_departure[i].start) + time_gap(City[k].train_departure[i].start, City[k].train_departure[i].end),
				temp, City[k].train_departure[i].end))
				succ = true;
		}
	TAG[city_num[from]] = 0;
	if (succ) return true;
	else return false;
}

bool fast_plane(string from, string to, int cost, queue<shift> Q, clock clk)
{
	bool succ = 0;
	TAG[city_num[from]] = 1;
	if (from == to)
	{
		if (cost < Min)
		{
			Min = cost;
			que = Q;
		}
		return true;
	}
	int k;
	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == from) k = i;
	}
	for (int i = 0; i < City[k].plane_departure.size(); ++i)
		if (City[k].plane_departure[i].start > clk&& TAG[city_num[City[k].plane_departure[i].To]] == 0)
		{
			queue<shift> temp = Q;
			temp.push(City[k].plane_departure[i]);
			if (fast_plane(City[k].plane_departure[i].To, to,
				cost + time_gap(clk, City[k].plane_departure[i].start) + time_gap(City[k].plane_departure[i].start, City[k].plane_departure[i].end),
				temp, City[k].plane_departure[i].end))
				succ = true;
		}
	TAG[city_num[from]] = 0;
	if (succ) return true;
	else return false;
}

void display_scheme(queue<shift> que)
{
	shift temp;
	while (!que.empty())
	{
		temp = que.front();
		temp.print();
		que.pop();
	}
}

void economical()
{
	cout << "Please input your originating city + destination + type of vehicle." << endl
		<< "( 0 means train and 1 means plane.)" << endl;
	string from, to;
	int _type;
	cin >> from >> to >> _type;
	int k;
	k = 0;
	for (int i = 0; i < City.size(); ++i)
		if (City[i].name == from) k = 1;
	if (k == 0)
	{
		cout << "Nonexistent City: " << from << endl;
		return;
	}
	k = 0;
	for (int i = 0; i < City.size(); ++i)
		if (City[i].name == to) k = 1;
	if (k == 0)
	{
		cout << "Nonexistent City: " << to << endl;
		return;
	}
	Min = INT_MAX;
	clear_que(que);
	queue<shift> Q;
	if (_type == 0)
	{

		if (eco_train(from, to, 0, Q, min2clock(0)))
		{
			cout << "The lowest cost: ￥" << Min << endl;
			display_scheme(que);
		}
		else cout << "There is no route from " << from << " to " << to << "." << endl;

	}
	else
	{
		if (eco_plane(from, to, 0, Q, min2clock(0)))
		{
			cout << "The lowest cost: ￥" << Min << endl;
			display_scheme(que);
		}
		else cout << "There is no route from " << from << " to " << to << "." << endl;
	}
	clear_que(que);
	Min = INT_MAX;
}


void fast()
{
	cout << "Please input your originating city + destination + type of vehicle." << endl
		<< "( 0 means train and 1 means plane.)" << endl;
	string from, to;
	int _type;
	cin >> from >> to >> _type;
	int k;
	k = 0;
	for (int i = 0; i < City.size(); ++i)
		if (City[i].name == from) k = 1;
	if (k == 0)
	{
		cout << "Nonexistent City: " << from << endl;
		return;
	}
	k = 0;
	for (int i = 0; i < City.size(); ++i)
		if (City[i].name == to) k = 1;
	if (k == 0)
	{
		cout << "Nonexistent City: " << to << endl;
		return;
	}
	Min = INT_MAX;
	clear_que(que);
	queue<shift> Q;
	if (_type == 0)
	{
		if (fast_train(from, to, 0, Q,min2clock(0)))
		{
			cout << "The lowest time cost: ";
			min2clock(Min).display();
			cout << endl;
			display_scheme(que);
		}
		else cout << "There is no route from " << from << " to " << to << "." << endl;
	}
	else
	{
		if (fast_plane(from, to, 0, Q,min2clock(0)))
		{
			cout << "The lowest time cost: ";
			min2clock(Min).display();
			cout << endl;
			display_scheme(que);
		}
		else cout << "There is no route from " << from << " to " << to << "." << endl;
	}
	Min = INT_MAX;
	clear_que(que);
}

void print_cities()
{
	for (int i = 0; i < City.size(); ++i)
	{
		cout << City[i].name << "   ";
	}
	cout << endl;
}

int main()
{
	char n;
	while (1)
	{
		cout << "What do you want to do?" << endl;
		cout << "1.Add a city;" << endl
			<< "2.Delete a city;" << endl
			<< "3.Add a shift;" << endl
			<< "4.delete a shift;" << endl
			<< "5.print all the cities;" << endl
			<< "6.print the time table;" << endl
			<< "7:calculate the fastest scheme;" << endl
			<< "8.calculate the most economical scheme;" << endl
			<< "9.quit." << endl;
		cin >> n;
		if (n-'1'<0 || n-'9'>0)
		{
			cout << "invalid input.Try again." << endl << endl << endl;
			continue;
		}
		int b = n - '0';
		switch (b) {
		case 1:
			add_city();
			break;
		case 2:
			delete_city();
			break;
		case 3:
			add_shift();
			break;
		case 4:
			delete_shift();
			break;
		case 5:
			print_cities();
			break;
		case 6:
			print();
			break;
		case 7:
			fast();
			break;
		case 8:
			economical();
			break;
		case 9:
			return 0;
		default:
			cout << "invalid operation.Try again." << endl;
			break;
		}
		cout << endl << endl;
	}
	return 0;
}
