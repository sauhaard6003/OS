#include <bits/stdc++.h>
#include <fstream>
using namespace std;

vector<int>sequence;

long long FCFS(vector<int>&requests, int head){
    long long tracks_changed=0;
    for (int i=0;i<requests.size();i++){
        tracks_changed+=abs(requests[i]-head);
        sequence.push_back(i+1);
        head=requests[i];
    }
    return tracks_changed;
}

int main(){
    sequence.clear();
    ifstream input_data;
    input_data.open("input.txt");
    if (!input_data){
        cout<<"Either file does not exist, or it could not be opened, so please try again\n";
    }
    vector<int> requests;
	int num_track;
	int num_sectors;
	int bytes_per_sector;
	double RPM;
	long long avg_seek_time;
	int head;
    input_data>>num_track;
    input_data>>num_sectors;
    input_data>>bytes_per_sector;
    input_data>>RPM;
    input_data>>avg_seek_time;
    input_data>>head;

    int request;
    while (input_data>>request){
        requests.push_back(request); //give space seperated inputs
    }

    // string s;
    // cin.ignore();
    // getline(input_data,s);
    // int j(0);
    // for (int i=0;i<s.length();i++){
    //     if (s[i]==' '){
    //         requests.push_back(stoi(s.substr(j,i-j)));
    //         j=i+1;
    //     }
    // }
    // requests.push_back(stoi(s.substr(j,s.length()-j)));
    
    cout<<"\nThe sequence of requests made is: ";
	for(int x : requests) {
        cout<<x<<" ";
    }
    cout<<"\n";
    long long x;
    x=avg_seek_time*FCFS(requests,head);

    cout<<"\nThe order in which track is changed is: ";
    cout<<sequence[0];
    for (int i=1;i<sequence.size();i++){
        cout<<" -> "<<sequence[i];
    }
    cout<<'\n';
	cout<<"\nTotal seek time is: "<<x<<"\n";
	cout<<"\nAverage rotational delay is: "<<(1/RPM)*(0.5)*60*1000<<" ms\n";
    cout<<"\n";
    return 0;
}