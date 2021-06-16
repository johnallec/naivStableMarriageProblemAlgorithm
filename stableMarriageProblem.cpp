#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Solution{
    vector<int> M;
    vector<int> W;
    vector<pair<int,int>> sol;
    vector<vector<int>> menPreferences;
    vector<vector<int>> womenPreferences;
    Solution(const vector<int>& u, const vector<int>& d, const vector<vector<int>>& pU, const vector<vector<int>>& pD):M(u),W(d),menPreferences(pU),womenPreferences(pD){}
};

bool secondCanAdd(const pair<int,int> myPair, const Solution& sol){
    if(find(sol.sol.begin(),sol.sol.end(),myPair)!=sol.sol.end())
        return false;
    int countM=1;
    int countW=1;
    for(auto c : sol.sol)
        if(c.first==myPair.first)
            countM++;
        else if(c.second==myPair.second)
            countW++;
    if(countM>1 || countW>1)
        return false;
    return true;
}

bool notStable(const Solution& sol, const pair<int,int>& c0, const pair<int,int>& c1){
    bool c1WomanFirst = false;
    for(auto  woman : sol.menPreferences[c0.first])
        if(woman==c0.second)
            break;
        else if(woman==c1.second){
            c1WomanFirst=true;
            break;
        }
    bool c0ManFirst = false;
    for(auto man : sol.womenPreferences[c1.second])
        if(man==c1.first)
            break;
        else if(man==c0.first){
            c0ManFirst=true;
            break;
        }
    return c1WomanFirst && c0ManFirst;
}

bool secondIsComplete(const Solution& sol){
    for(int i=0; i<sol.sol.size(); ++i)
        for(int j=i+1; j<sol.sol.size()-1; ++j)
            if(notStable(sol,sol.sol[i],sol.sol[j]))
                return false;
    return true;
}

bool backTracking(Solution& sol, const int& indexMan){
    int indexWoman = 0;
    while(indexWoman<sol.W.size()){
        pair<int,int> myPair(indexMan,indexWoman);
        if(secondCanAdd(myPair,sol)){
            sol.sol.push_back(myPair);
            if(secondIsComplete(sol)) return true;
            else if(backTracking(sol,indexMan)) return true;
            sol.sol.pop_back();
            indexWoman++;
        }
        else indexWoman++;
    }
    return false;    
}

bool isComplete(const Solution& sol){
    if(sol.sol.size()!=sol.M.size())
        return false;
    for(auto c0 : sol.sol)
        for(auto c1 : sol.sol)
            if(c0!=c1 && notStable(sol,c0,c1))
                return false;
    return true;
}

bool solve(Solution& sol){
    int index = 0;
    while(index<sol.M.size()){
        if(backTracking(sol,index)){
            if(isComplete(sol)) return true;
            else if(solve(sol)) return true;
            sol.sol.pop_back();
            index++;
        }
        else index++;
    }
    return false;
}

bool solve(const vector<int>& U, const vector<int>& D, const vector<vector<int>>& menPreferences, const vector<vector<int>>& womenPreferences){
    if(U.size()!=D.size())
        return false;
    Solution sol(U,D,menPreferences,womenPreferences);
    bool b = solve(sol);
    for(auto myPair : sol.sol)
        cout << "m" << myPair.first << " <-> w" << myPair.second << endl;
    return b;
}

int main(){
    vector<int> M = {0,1,2};
    vector<int> W = {0,1,2};

    vector<int> m0 = {1,0,2};
    vector<int> m1 = {2,0,1};
    vector<int> m2 = {1,2,0};

    vector<int> w0 = {0,1,2};
    vector<int> w1 = {0,2,1};
    vector<int> w2 = {1,2,0};

    vector<vector<int>> menPreferences={m0,m1,m2};
    vector<vector<int>> womenPreferences={w0,w1,w2};

    cout << solve(M,W,menPreferences,womenPreferences) << endl;
    return 0;
}