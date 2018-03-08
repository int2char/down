#include"BFS.h"
#include"Heap.h"
#include"limits.h"
#include<queue>
#include<iostream>
struct cnmp
{
	bool operator()(pair<int,int>&a,pair<int,int>&b)
	{
		return a.second>b.second;
	};
};
int dijkstra(int s,vector<int>&d,int*peg,vector<vector<int>>&neie,vector<vector<int>>&nein,vector<vector<int>>&neieid,vector<int>&esigns,int nodenum,int pnodesize,int WD,set<int>&sets,int size)
{
	//cout<<"in djjs"<<endl;
	priority_queue<pair<int, int>,vector<pair<int,int>>,cnmp>heap;
	vector<int>flag(pnodesize,0);
	heap.push(make_pair(s,0));
	do{
		int cur = heap.top().first;
		//cout<<"cur is "<<cur<<endl;
		heap.pop();
		int accual=cur%pnodesize;
		if(flag[accual]==0)
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
		flag[accual]=1;
		if(cur>=WD*pnodesize)continue;
		int size = nein[accual].size();
		for (int i=0;i<size;i++){
				int to=nein[accual][i]+(cur/pnodesize+1)*pnodesize;
				if (d[to]>(d[cur]+neie[accual][i])&&neie[accual][i]>0&&esigns[neieid[accual][i]]>0){
					d[to]=d[cur]+esigns[neieid[accual][i]];
					heap.push(make_pair(to,d[to]));
					peg[to]=neieid[accual][i];
					//cout<<"peg "<<to<<" is "<<peg[to]<<endl;
				}
		}
	} while (!heap.empty());
	//cout<<"return success"<<endl;
	return 0;
}
