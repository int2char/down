#ifndef BFS_ALG
#define BFS_ALG
#include<vector>
#include<queue>
#include<set>
using namespace std;
int BFS(int s,int t,vector<int>&dist,int*pre,vector<vector<int>>&neie,vector<vector<int>>&nein,vector<vector<int>>&neieid,vector<int>&esigns,set<int>sets,int size,int WD);
int dijkstra(int s,int t,vector<int>&dist,int*pre,vector<vector<int>>&neie,vector<vector<int>>&nein,vector<vector<int>>&neieid,vector<int>&esigns,int,int,set<int>&sets,int size);
#endif