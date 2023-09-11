#include <bits/stdc++.h>
#include <fstream>

using namespace std;
pair<int,vector<string>>result;

pair<int,vector<string>>findmin(pair<int,vector<string>>&p1, pair<int,vector<string>>&p2, pair<int,vector<string>>&p3){
    if (p1.first<p2.first && p1.first<p3.first){
        return p1;
    }
    else if (p2.first<p1.first && p2.first<p3.first){
        return p2;
    }
    return p3;
}

pair<int,vector<string>> editDistance(vector<vector<pair<int,vector<string>>>>&dp, vector<string>&v1, vector<string>&v2, int counter1, int counter2){
    if (counter1==0 && counter2==0){
        return {0,{}};
    }
    if (counter2==0){
        dp[counter1][counter2]={counter1,{}};
        string lol="";
        if (counter1>1){
            lol+=(1+'0');
            lol+=',';
        }
        lol+=(counter1+'0');
        lol+="d0";
        dp[counter1][counter2].second.push_back(lol);
        for (int i=0;i<counter1;i++){
            string s="";
            s+="< ";
            s+=v1[i];
            dp[counter1][counter2].second.push_back(s);
        }
        
        return dp[counter1][counter2];
    }
    if (counter1==0){
        dp[counter1][counter2]={counter2,{}};
        string lol="";
        if (counter2>1){
            lol+=(1+'0');
            lol+=',';
        }
        
        lol+=(counter2+'0');
        lol+=("a");
        lol+=(counter2+'0');
        dp[counter1][counter2].second.push_back(lol);
        
        for (int i=0;i<counter2;i++){
            

            string s="";
            s+="> ";
            s+=v2[i];
            dp[counter1][counter2].second.push_back(s);
        }
        
        return dp[counter1][counter2];
    }
    if (dp[counter1][counter2].first!=-1){
        return dp[counter1][counter2];
    }
    if (v1[counter1-1]==v2[counter2-1]){
        return editDistance(dp,v1,v2,counter1-1,counter2-1);
    }
    int mini=INT_MAX;
    
    pair<int,vector<string>>add=editDistance(dp,v1,v2,counter1,counter2-1);
    pair<int,vector<string>>del=editDistance(dp,v1,v2,counter1-1,counter2);
    pair<int,vector<string>>cha=editDistance(dp,v1,v2,counter1-1,counter2-1);
    
    pair<int,vector<string>>final=findmin(add,del,cha);
    if (final==add){
        string s="";
        
        s+=(counter1+'0');
        s+="a";
        s+=(counter2+'0');
        final.second.push_back(s);
        s="";
        s+="> ";
        s+=v2[counter2-1];
        final.second.push_back(s);
        s="";
        final.first++;
        return dp[counter1][counter2]=final;
    }
    else if (final==del){
        string s="";
        s+=(counter1+'0');
        s+='d';
        s+=(counter2+'0');
        final.second.push_back(s);
        s="< ";
        s+=v1[counter1-1];
        final.second.push_back(s);
        s="";
        final.first++;
        return dp[counter1][counter2]=final;
    }
    string s="";
    s=(counter1+'0');
    s+='c';
    s+=(counter2+'0');
    final.second.push_back(s);
    s="";
    s="> ";
    s+=v1[counter1-1];
    final.second.push_back(s);
    s="";
    s+=v2[counter2-1];
    final.second.push_back(s);
    s="";
    final.first++;
    return dp[counter1][counter2]=final;
}

int main(){
    ifstream data1,data2;
    data1.open("txt1.txt");
    data2.open("txt2.txt");
    if (!data1 || !data2){
        cout<<"Invalid input";
    }
    string s;
    vector<string>v1,v2;
    while (getline(data1,s)){
        v1.push_back(s);
    }
    while (getline(data2,s)){
        v2.push_back(s);
    }
    vector<vector<pair<int,vector<string>>>>dp(v1.size()+1,vector<pair<int,vector<string>>>(v2.size()+1,{-1,{}}));
    vector<string>final=editDistance(dp,v1,v2,v1.size(),v2.size()).second;
    //reverse(final.begin(),final.end());
    for (string &x:final){
        //reverse(x.begin(),x.end());
        cout<<x<<endl;
    }
}