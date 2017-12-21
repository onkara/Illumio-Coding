#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

typedef struct IP_Format {
	string Host;
	string Port;
}IP_Format;

vector <IP_Format> src_v;
vector <IP_Format> target_v;

void insertEntry(string line){

	size_t pos = line.find(",");
	if (pos != string::npos){
		string ip_src = line.substr(0,pos);
		string dest_src = line.substr( pos + 1, line.size());

		IP_Format src; // = new IP_Format();
		IP_Format target; // = new IP_Format();

		pos = ip_src.find(":");
		if (pos != string::npos){
			src.Host = ip_src.substr(0, pos);
			src.Port = ip_src.substr(pos+1 ,ip_src.size());
		}
		else
		{
			return;
		}

		pos = dest_src.find(":");
		if (pos != string::npos){
			target.Host = dest_src.substr(0, pos);
			target.Port = dest_src.substr(pos+1 ,dest_src.size());
		}
		else
		{
			return;
		}
		src_v.push_back(src);
		target_v.push_back(target);
	}

	//cout << line + " broke as \n";
	//cout << ip_src + "\n";
	//cout << dest_src + "\n";
}

void prinNATTable(){
	cout<< "\n Printing Table \n";
	cout <<"src\n";
	for( auto it = src_v.begin(); it != src_v.end(); ++it ){
		cout<< (*it).Host << " : " << (*it).Port;
		cout<<"\n";
	}
	cout << "target\n";
	for( auto it = target_v.begin(); it != target_v.end(); ++it ){
		cout<< (*it).Host << " : " << (*it).Port;
		cout<<"\n";
	}
	cout<< "\n End of Table";
}

int main()
{
	string line;

	ifstream NAT_file ("NAT.txt");
	if (NAT_file.is_open())
	{
		while (getline (NAT_file, line))
		{
			insertEntry(line);
		}
	}
	ifstream FLOW_file ("FLOW.txt");
	ofstream OUT_file("OUTPUT.txt");

	if (FLOW_file.is_open())
	{
		while (getline(FLOW_file, line))
		{
			string flow_ip, flow_port;
			size_t pos = line.find(":");
			if (pos != string::npos){
				flow_ip = line.substr(0, pos);
				flow_port = line.substr(pos+1 ,line.size());
			}
			int i;
			for (i = 0; i < src_v.size(); i++)
			{
				if (src_v[i].Host == flow_ip || (src_v[i].Host == "*" && src_v[i].Port != "*")) {
					if (src_v[i].Port == flow_port || (src_v[i].Port == "*" && src_v[i].Host != "*")) {
						OUT_file << flow_ip << ":" <<  flow_port << " -> ";
						OUT_file << target_v[i].Host << ":" <<  target_v[i].Port << "\n";
						break;
					}
				}
			}
			if (i == src_v.size())
				OUT_file << "No nat match for " << line << "\n";
		}
	}

	//prinNATTable();

}
