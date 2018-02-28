#include"BFS.h"
#include"Heap.h"
#include"limits.h"
#include<queue>
#include<iostream>
int dijkstra(int s,int t,vector<int>&d,int*peg,vector<vector<int>>&neie,vector<vector<int>>&nein,vector<vector<int>>&neieid,vector<int>&esigns,int nodenum,int WD,set<int>&sets,int size){
	int tnode=-1;
	vector<int>flag(nodenum,0);
	for (int i = 0;i<nodenum;i++)
	if (i == s)
		d[i]=0;
	else
		d[i]=INT_MAX/2;
	for (int i=0; i<nodenum; i++)
	{
		flag[i]=0;
		peg[i]=-1;
	}
	int cur = s;
	Heap heap(nodenum);
	//priority_queue<pair<int, int>,vector<pair<int,int>>,std::less<std::pair<int, int>>>heap;
	for (int i = 0;i<nodenum;i++)
		heap.push(i,d[i]);
	//heap.push(make_pair(s,0));
	do{
		//int cur = heap.top().first;
		int cur=heap.pop();
		flag[cur] = 1;
		int accual=cur/(WD+1);
		if (sets.find(accual)!=sets.end())
			{	
				size--;
				set<int>::iterator iter=sets.begin();
				for(iter;iter!=sets.end();iter++)
					if(*iter==accual)
						sets.erase(iter);
				if(size==0)
					break;
			}
		int size = nein[cur].size();
		for (int i = 0;i<size; i++){
				int to=nein[cur][i];
				if (flag[to] ==0&&d[to]>(d[cur]+neie[cur][i])&&neie[cur][i]>0&&esigns[neieid[cur][i]]>0){
					d[to]=d[cur]+esigns[neieid[cur][i]];
					heap.update(to, d[to]);
					//heap.push(make_pair(to,d[to]));
					peg[to]=neieid[cur][i];
				}
		}
	} while (!heap.empty());
	return d[t];
}
