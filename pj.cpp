#include<iostream>
#include<string>
#include<vector>
using namespace std;

class clock {
public:
	int hour = 0;              //0<=hour<=23
	int min = 0;			   //0<=min<=59
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
		else cout << min<<" ";
	}
	int clk2num()      //In order to compare the which shifts comes first
	{
		return 60 * hour + min;
	}
};



class shift {                        
public:
	shift();
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
		cout << "Depart:";start.display(); 
		cout << "Arrive:";end.display();
		cout << "Price: " << price << endl;
	}
};

class city {
public:
	string name;
	vector<shift> train_departure;
	vector<shift> train_arrival;
	vector<shift> plane_departure;
	vector<shift> plane_arrival;
};

vector<city> City;

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

	int tag1 = 0,tag2 = 0;

	for (int i = 0; i < City.size(); ++i)
	{
		if (City[i].name == one_run.From) tag1 = 1;
		if (City[i].name == one_run.To) tag2 = 1;
	}

	if (tag1 == 0 || tag2 == 0) 
		cout << "City not recorded.Please check your input."<< endl;             
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

int time_gap(clock a, clock b)  //This branch asumes that all shifts finish in one day
{
	return b.clk2num() - a.clk2num();
}

void min2clock(int a)
{
	int hour = a % 60;
	int min = a - hour * 60;
	cout << hour << " : " << min << " ";
}

void print()
{
	cout << "Please input the name of city." << endl;
	string s;
	int tag = 0, n;
	for(n=0;n<City.size();++n)
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

int main()
{
	while (true)
	{
		cout << "What do you want to do?" << endl;
		cout << "1.Add a city;" << endl
			<< "2.Delete a city;" << endl
			<< "3.Add a shift;" << endl
			<< "4.delete a shift;" << endl
			<< "5.print the time table;" << endl
			<< "6:calculate the fastest scheme;" << endl
			<< "7.calculate the most economical scheme;" << endl
			<< "8.quit." << endl;
		int n;
		cin >> n;
		switch (n) {
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
			print();
			break;
		case 6:
			fast();
			break;
		case 7:
			economical();
			break;
		case 8: 
			return 0;
		default: 
			cout << "invalid operation.Try again." << endl;
		}
	}
	return 0;
}
