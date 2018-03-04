#include"BFS.h"
#include<iostream>
struct ccmp{  
    bool operator()(pair<int,int>& a,pair<int,int>& b){  
        return a.second>b.second;    
    }  
}; 
int BFS(int s,int t,vector<int>&dist,int*pre,vector<vector<int>>&neie,vector<vector<int>>&nein,vector<vector<int>>&neieid,vector<int>&esigns,set<int>sets,int size,int WD)
{
	int vflag=1;
	int tnode=-1;
	int tv=-1;
	//priority_queue<pair<int,int>,vector<pair<int,int>>,ccmp>que;
	queue<pair<int,int>>que;
	que.push(make_pair(s,0));
	dist[s]=0;
	while(!que.empty()&&vflag)
	{
		int node=que.front().first;
		int v=que.front().second;
		que.pop();
		if(v>WD)break;
		for(int i=0;i<nein[node].size();i++)
		{
			if(neie[node][i]>0&&esigns[neieid[node][i]]>0)
			{	
				int to=nein[node][i];
				if(dist[to]>dist[node]+1)
				{
					pre[to]=neieid[node][i];
					dist[to]=dist[node]+1;
					que.push(make_pair(to,dist[to]));
				}
				else
					continue;
				if(sets.find(to)!=sets.end()){size--;if(size==0)break;}
			}
		}
	}
	return dist[tnode];
}
