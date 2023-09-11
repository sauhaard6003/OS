#include <bits/stdc++.h>
#include <fstream>
using namespace std;

vector<int>sequence;

long long goleft(vector<pair<int,int>>left, int disksize, int &head){
    long long res=0;
    sort(left.rbegin(),left.rend());
    for (auto i:left){
        sequence.push_back(i.second+1);
        res+=(head-i.first);
        head=i.first;
    }
    return res;
}
long long goright(vector<pair<int,int>>right, int disksize, int &head){
    long long res=0;
    sort(right.begin(),right.end());
    for (auto i:right){
        sequence.push_back(i.second+1);
        res+=(i.first-head);
        head=i.first;
    }
    return res;
}

long long C_SCAN(vector<int>requests, int disksize, int head, string direction="right"){
    int cnt=0;
    vector<pair<int,int>>left,right;
    for (int i:requests){
        if (i<head){
            left.push_back({i,cnt++});
        }
        else{
            right.push_back({i,cnt++});
        }
    }
    int i(2);
    long long res=0;
    while (i--){
        if (i){
            if (direction=="right"){
                res+=goright(right,disksize,head);
                res+=(disksize-1-head);
                res+=(disksize-1);
                head=0;
            }
            if (direction=="left"){
                res+=goleft(left,disksize,head);
                res+=(head);
                res+=(disksize-1);
                head=disksize-1;
            }
        }
        else{
            if (direction=="right"){
                res+=goright(left,disksize,head);
            }
            if (direction=="left"){
                res+=goleft(right,disksize,head);
            }
        }
    }
    return res;
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
    x=avg_seek_time*C_SCAN(requests,num_track,head);

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