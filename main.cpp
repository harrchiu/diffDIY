// 2021-2-15
// HC
// Directory Non-recursive Filename Diff. (no content)

#include <iostream>     // basic
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>

#include <functional>   // trimming
#include <cctype>
#include <locale>
// directory reading
#include <experimental/filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;

/* drag files below to get its directory, then copy+paste when prompted:
 /Users/drag/file/here/for/path/name
 */

string fullTrim(string s){
    if (s.length() == 0)
        return "";
    string leftTrim = s.substr(s.find_first_not_of(" \t\n\r\f\v"), (int)s.length());
    string rightTrim = leftTrim.substr(0,s.find_last_not_of(" \t\n\r\f\v")+1);
    return rightTrim;
}

// return if directory exists (non-empty or empty)
bool pathExists(const fs::path& p, fs::file_status s = fs::file_status{}){
    if(fs::status_known(s) ? fs::exists(s) : fs::exists(p))
        return 1;
    return 0;
}

string padStr(string s, int length){
    string ret = s;
    ret += string(max(0,length - (int)s.length()), ' ');
    return ret;
}

void acceptFileNames(vector<string> &v){
    string s;
    while (getline(cin,s)){
        if (s == "d")
            break;
        if (s == "")
            continue;
        v.push_back(s);
    }
}

int main(){
    
    int round = 0;
    vector<string> v1, v2;
    string d1="dir 1", d2="dir 2", s, s1,s2;
    
    cout << "dir 1 name (<enter> for \"dir 1\"): ";
    getline(cin, s);
    if (s != ""){
        d1 = s;
    }
    cout << "dir 2 name (<enter> for \"dir 2\"): ";
    getline(cin, s);
    if (s != ""){
        d2 = s;
    }
    
    cout << "manually enter filenames? (y/<enter>): ";
    getline(cin, s);
    
    if (s == "y"){
        printf("\n---copy paste files for %s (\"d\" when done):\n",d1.c_str());
        acceptFileNames(v1);
        printf("\n---copy paste files for %s (\"d\" when done):\n",d2.c_str());
        acceptFileNames(v2);
    }
    else{
        cout << endl;
        while (true){
            cout << "enter path to " << d1 << ": ";
            getline(cin, s1);
            s1 = fullTrim(s1);
            if (pathExists(s1)){
                cout << endl << d1 << " @ \"" << s1 << '"';
                break;
            }
            cout << "path not found.\n";
        }
        cout << endl << "---" << endl;
        while (true){
            cout << "enter path to " << d2 << ": ";
            getline(cin, s2);
            s2 = fullTrim(s2);
            if (pathExists(s2)){
                cout << endl << d2 << " @ \"" << s2 << '"';
                break;
            }
            cout << "path not found.\n";
        }
        
    startComparingDirectoriesLine:   // upon ending, refresh/reupdate starts here
        for (const auto & entry: fs::directory_iterator( s1 )){
            string p = entry.path();
            v1.push_back(p.substr(p.find_last_of("/\\"), (int)p.length()));
        }
        for (const auto & entry: fs::directory_iterator( s2 )){
            string p = entry.path();
            v2.push_back(p.substr(p.find_last_of("/\\"), (int)p.length()));
        }
    }
    
    sort(v1.begin(),v1.end());
    sort(v2.begin(),v2.end());
    int n1 = (int)v1.size(), n2 = (int)v2.size();
    
    vector<string> onlyInV1, onlyInV2, inBoth;
    
    int onlyV1Longest = max(7, (int)d1.length());  // "only in".length() == 7
                                                   // "(only in dir x/all in dir)".length()
    onlyV1Longest = max(onlyV1Longest,
                        (int)log10(onlyInV1.size())+1 +(int)log10(n1)+1 +3);
    
    int onlyV2Longest = max(7, (int)d2.length());
    onlyV2Longest = max(onlyV2Longest,
                        (int)log10(onlyInV2.size())+1 +(int)log10(n2)+1 +3);
    int inBothLongest = 0;
    
    int id1=0,id2=0;
    while (id1 < n1 && id2 < n2){
        if (v1[id1] == v2[id2]){
            inBoth.push_back(v1[id1]);
            id1 ++;
            id2 ++;
            inBothLongest = max(inBothLongest,(int)v1[id1].length());
        }
        else if (v1[id1] < v2[id2]){
            onlyInV1.push_back(v1[id1]);
            id1 ++;
        }
        else{ // v2[id1] > v1[id2]
            onlyInV2.push_back(v2[id2]);
            id2 ++;
        }
    }
    // only one will run
    while (id1 < n1){
        onlyInV1.push_back(v1[id1]);
        id1++;
    }
    while (id2 < n2){
        onlyInV2.push_back(v2[id2]);
        id2++;
    }
    // find longest from file names
    for (string s: onlyInV1){
        onlyV1Longest = max(onlyV1Longest,(int)s.length());
    }
    for (string s: onlyInV2){
        onlyV2Longest = max(onlyV2Longest,(int)s.length());
    }
    
    cout << "\n\n\nComplete";
    if (round){
        cout << " (" << round << ")";
    }
    round++;
    cout << ".\n--------\n";
    
    //+1 for space between columns
    cout << padStr("only in",onlyV1Longest+1) << "| only in\n";
    cout << padStr(d1, onlyV1Longest+1) << "| " << d2 << '\n';
    //-1 for directory name??
    string l1 = "(" + to_string(onlyInV1.size()) + "/" + to_string(n1-1) + ")";
    string l2 = "(" + to_string(onlyInV2.size()) + "/" + to_string(n2-1) + ")";
    
    cout << padStr(l1,onlyV1Longest+1) << "| " << l2 << endl;
    
    id1=0;
    id2=0;
    while (id1 < onlyInV1.size() && id2 < onlyInV2.size()){
        cout << padStr(onlyInV1[id1], onlyV1Longest+3) << onlyInV2[id2] << '\n';
        id1 ++;
        id2 ++;
    }
    // only one of these will run
    for (int q=id1; q<onlyInV1.size(); q++){
        cout << onlyInV1[q] << '\n';
    }
    for (int q=id2; q<onlyInV2.size(); q++){
        cout << string(onlyV1Longest+3,' ') << onlyInV2[q] << '\n';
    }
    // queue for another test?
    cout << endl << endl << "refresh? ";
    string refresh;
    getline(cin,refresh);
    if (refresh == "y" || refresh == "yes" || refresh == "Y"){
        v1={};
        v2={};
        goto startComparingDirectoriesLine;
    }
    
    return 0;
}
