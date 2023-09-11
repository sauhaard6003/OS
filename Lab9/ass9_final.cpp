#include<bits/stdc++.h>
#include<dirent.h>

using namespace std ;

vector<string>final_ans;
void DFS(string Home){
    DIR *dir;
    struct dirent *ent;
    //start reading directories
    if ((dir = opendir (Home.c_str())) != nullptr) {
        while ((ent = readdir (dir)) != nullptr) {
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
                string s = Home + '/' + ent->d_name ;
                final_ans.push_back(s); 
                DFS(s) ;
            }
        }
        closedir (dir);
    } else {
        return; 
    }
}

int main(){

    string Home = "." ;
    DFS(Home);
    //cout<<"print output"<<endl;
    unordered_map<size_t, string> hmp; 

    for(int i = 0 ; i<final_ans.size() ; i++){
        ifstream f(final_ans[i]) ;
        string line1, st ;
        vector<string> fi1 ;
        while(getline(f, line1)){
            fi1.push_back(line1);
            fi1.push_back("\n");
        }
        for(auto x: fi1)
        {
            st += x ; 
        }

        size_t hsh = hash<string>{}(st); 
        if(hmp.count(hsh) != 0){
            remove(final_ans[i].c_str()); 
        }
        else {
            hmp[hsh] = final_ans[i] ; 
        }
    }
    return 0;
}