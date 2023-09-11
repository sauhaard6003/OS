#include <bits/stdc++.h>
#include <fstream>
using namespace std;


//Since SCAN can go in either left or right direction, and it hasn't been mentioned in the q which direction it will go in, I make sure that the direction taken is the one which leads to least total seek time...

long long goleft(vector<pair<int,int>>left, int disksize, vector<int>&sequence, int &head){
    long long res=0;
    sort(left.rbegin(),left.rend());
    for (auto i:left){
        sequence.push_back(i.second+1);
        res+=(head-i.first);
        head=i.first;
    }
    return res;
}
long long goright(vector<pair<int,int>>right, int disksize, vector<int>&sequence, int &head){
    long long res=0;
    sort(right.begin(),right.end());
    for (auto i:right){
        sequence.push_back(i.second+1);
        res+=(i.first-head);
        head=i.first;
    }
    return res;
}

pair<long long,vector<int>> SCAN(vector<int>requests, int disksize, int head){
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
    long long res1(0),res2(0);
    vector<int>sequence1,sequence2;
    string direction="right";
    int h=head;
    while (i--){
        if (direction=="right"){
            res1+=goright(right,disksize,sequence1,head);
            direction="left";
            if (i){
                res1+=(disksize-1-head);
            }          
            head=disksize-1;
        }
        else{
            res1+=goleft(left,disksize,sequence1,head);
            direction="right";
            if (i){
                res1+=head;
            }  
            head=0;
        }
    }
    int j(2);
    direction="left";
    head=h;
    while (j--){
        if (direction=="right"){
            res2+=goright(right,disksize,sequence2,head);
            direction="left";
            if (j){
                res2+=(disksize-1-head);
            }          
            head=disksize-1;
        }
        else{
            res2+=goleft(left,disksize,sequence2,head);
            direction="right";
            if (j){
                res2+=head;
            }  
            head=0;
        }
    }
    if (res1<res2){
        return {res1,sequence1};
    }
    return {res2,sequence2};
}

int main(){
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
    pair<long long,vector<int>>v=SCAN(requests,num_track,head);
    long long x;
    x=avg_seek_time*(v.first);

    cout<<"\nThe order in which track is changed is: ";
    cout<<v.second[0];
    for (int i=1;i<v.second.size();i++){
        cout<<" -> "<<v.second[i];
    }
    cout<<'\n';
	cout<<"\nTotal seek time is: "<<x<<"\n";
	cout<<"\nAverage rotational delay is: "<<(1/RPM)*(0.5)*60*1000<<" ms\n";
    cout<<"\n";
    return 0;
}