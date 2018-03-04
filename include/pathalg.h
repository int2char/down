#ifndef CSPALLOC_PATHALG_H
#include"limits.h"
#define CSPALLOC_PATHALG_H
#define INFCOST INT_MAX/2
#include<bits/stdc++.h>
#include <unistd.h>
#include"edge.h"
#include<sys/time.h>
#include<queue>
#include"Heap.h"
#include <algorithm>
#include<map>
#include"BFS.h"
#define ML 50
#define BS 5
#define WD 8
#ifndef LY 
	#define LY 80
#endif
#ifndef NODE
	#define NODE 100
#endif
#ifndef DSIZE
	#define DSIZE 20
#endif
#ifndef SERT
	#define SERT 20
#endif
#ifndef IFHOP
	#define IFHOP 0
#endif
#ifndef OBNUM
	#define OBNUM 10
#endif

#define PC 2
#define LY1 ((LY/4))
#define LY2 ((LY/4)*3)
#define YE NODE
#define inf INT_MAX/2
#define INF 10000
#define NUT ((WD+1))
using namespace std;
struct event{
	int fuhao;
	int time;
	vector<int>rout;
	int level;
	event(int fuh,int tt,vector<int>_rout=vector<int>(),int _level=-1):fuhao(fuh),time(tt),rout(_rout),level(_level){};
	
};
struct Rout{
	int s,t;
	int id;
	int di;
	int offf;
	int ly;
	//vector<int>routes;
	Rout(int _s,int _t,int _id,int _hops,int _offf,int _ly):s(_s),t(_t),id(_id),di(_hops),offf(_offf),ly(_ly){};
};
struct Sot{
	int s;
	set<int>ts;
	map<int,int>mmpid;
	vector<int>ters;
	int size;
	Sot(int _s=0){s=_s,size=0;};
	Sot(){};
	bool push(int t,int i){
		if(t==s)return false;
		ts.insert(t);
		mmpid[t]=i;
		size++;
		ters.push_back(t);
		return true;
	}
};
class comp{
	public:
	bool operator()(Rout& r1,Rout& r2)
	{
		if(r1.di>r2.di)
			return true;
		return false;
	};
};
class paircomp{
public:
	bool operator()(pair<int,int>&a,pair<int,int>&b)
	{
		return a.second>b.second;
	}
};
struct demand{
	priority_queue<Rout,vector<Rout>,comp> backroute;
	priority_queue<pair<int,int>,vector<pair<int,int>>,paircomp>routid;
	//vector<pair<int,int>>routid;
	int id;
	int value;
	int s,t;
	int mark;
	int estimate;
	vector<int>rout;
	int sert;
	demand(int _s,int _t,int _id,int _sert):s(_s),t(_t),id(_id),mark(-1),sert(_sert){};
};
class compd{
	public:
	bool operator()(demand &d1,demand& d2)
	{
		if(d1.backroute.empty())
			return true;
		if(d2.backroute.empty())
			return false;
		if(d1.backroute.top().di>d2.backroute.top().di)
			return true;
		return false;
	};
};
struct cevent{  
    bool operator()(event&a,event&b){  
        return a.time>b.time;    
    }  
}; 
class algbase {
    protected:
        vector<int> getrout(int &s, int &t, vector<edge> &edges, vector<int> &pre) {
            vector<int> rout;
            int pp = pre[t];
            while (pp >= 0) {
                rout.push_back(pp);
                pp = pre[edges[pp].s];
            }
            reverse(rout.begin(), rout.end());
            return rout;
        }
    public:
        int*p;
        algbase(){};
        virtual bool cutcake(int)=0;
        virtual void updatE(vector<vector<int>>&esigns){};
	 	virtual void updatS(vector<vector<Sot>>&stpair){};	 
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,int _nodenum)=0;
	 	virtual vector<vector<Rout>> routalg(int s,int t,int bw)=0;
	 	virtual pair<int,vector<int>> tunel(int s,int t,int k,int m=0){}
	 	virtual void updatR(int k,vector<int>&rout){};
	 	//virtual vector<vector<int>> allroutes(int k){};
	 	virtual pair<int,int> prepush(int s,int t,int bw)=0;
};
class PBellmanor:public algbase{
    public:
		vector<vector<int>>neibour;
		vector<int>ancestor;
		int edgesize,nodenum,pesize,pnodesize,maxbw;
		vector<vector<vector<int>>>mask;
		vector<edge>edges;
		vector<int>dist;
		vector<int>leveln;
		vector<int>exn2n;
		vector<vector<int>>rela;
		vector<int>order;
		vector<vector<vector<int>>>neie;
		vector<vector<vector<int>>>nein;
		vector<vector<vector<int>>>neiw;
		vector<int>esign;
		vector<pair<int,int>>stes;
		int W;
		vector<vector<Sot>>stpairs;
		vector<int>L;
		//
		vector<vector<vector<int>>>neieid;
		vector<vector<int>>esigns;
		
		PBellmanor():L(3,0){};
        virtual void updatE(vector<vector<int>>&_esigns){
        	esigns=_esigns;
        	memset(p,-1,LY*YE*nodenum*sizeof(int));
        };
        virtual void updatS(vector<vector<Sot>>&stpair){
        	stpairs=stpair;
        	L[0]=0;
        	L[1]=LY1;
        	L[2]=LY;
        }
	 	virtual void updatR(int k,vector<int>&rout){
        	//cout<<"in updat R"<<endl;
	 		for(int i=0;i<rout.size();i++)
	 			{
					int eid=rout[i];
					if(IFHOP<1)
						esigns[k][eid]*=-1;
					if(IFHOP==1)
					{
						eid=(eid/WD)*WD;
						for(int j=0;j<WD;j++)
							esigns[k][eid+j]*=-1;
					}
	 			}
	 	};
        void topsort()
        {
        }
        virtual bool cutcake(int index){
        }
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,int _nodenum){
        	stes=stpair;
        	edges=extenedges.first;
        	esigns=extenedges.second;
        	edgesize=edges.size();
        	nodenum=_nodenum;
        	pnodesize=nodenum/NUT;
			W=WD+1;
			for(int k=0;k<LY;k++)
			{
				vector<vector<int>>tmpn(pnodesize,vector<int>());
				vector<vector<int>>tmpe(pnodesize,vector<int>());
				vector<vector<int>>tmpeid(pnodesize,vector<int>());
				for(int i=0;i<edges.size();i++)
					{
						int s=edges[i].s;
						int t=edges[i].t;
						tmpn[t].push_back(s);
						tmpe[t].push_back(esigns[k][i]);
						tmpeid[t].push_back(i);
					}
				neie.push_back(tmpe);
				nein.push_back(tmpn);
				neieid.push_back(tmpeid);
			}
			p=new int[nodenum*LY*YE];

        }
        virtual vector<vector<Rout>> routalg(int s,int t,int bw){
        		vector<vector<Rout>>result(2,vector<Rout>());
        		int ncount=0;
        		for(int y=1;y<PC+1;y++)
					for(int k=L[y-1];k<L[y];k++)
					{
						int tnode=-1;
						vector<int>d(nodenum,INT_MAX/2);
						for(int l=0;l<stpairs[y-1].size();l++)
						{	
							int s=stpairs[y-1][l].s;
							set<int>ts=stpairs[y-1][l].ts;
							vector<int>ters=stpairs[y-1][l].ters;
							int size=stpairs[y-1][l].size;
							int off=ncount*nodenum;
							int neioff=pnodesize;
							d[s]=0;
							for(int kk=1;kk<=WD;kk++)
							{
								for(int i=0;i<pnodesize;i++)
								{
									for(int m=0;m<nein[k][i].size();m++)
									{
										if(neie[k][i][m]>0)
										{
											if(neie[k][i][m]+d[neioff-pnodesize+nein[k][i][m]]<d[neioff+i])
												{
												 d[neioff+i]=neie[k][i][m]+d[neioff-pnodesize+nein[k][i][m]];
												 p[off+neioff+i]=neieid[k][i][m];
												}
										}
									}
								}
								neioff+=pnodesize;
							}
							//dijkstra(s,s,d,p+off,neie[k],nein[k],neieid[k],esigns[k],nodenum,WD,ts,size);
							for(int i=0;i<ters.size();i++)
							{
								vector<int>rout;
								int hop=0;
								int tt=ters[i];
								int min=10000;
								int prn=-1;
								for(int i=1;i<W;i++)
								{
									if(d[tt+i*pnodesize]<min)
									{	
										min=d[tt+i*pnodesize];
										prn=tt+i*pnodesize;
									}
								}
								if(prn<0)continue;
								int offf=prn-tt+off;
								int di=d[prn];
								int id=stpairs[y-1][l].mmpid[ters[i]];
								int node=tt;
								/*cout<<"dist is "<<prn<<" "<<"t is "<<tt<<" "<<d[prn]<<endl;
								while(node!=s)
								{
									
									int eid;
									cout<<"what is it:"<<node+offf<<endl;
									eid=p[node+offf];
									cout<<eid<<endl;
									offf-=pnodesize;
									node=edges[eid].s;
								}
								cout<<endl;
								cout<<"return "<<endl;*/
								Rout S(s,tt,id,min,offf,k);
								result[y-1].push_back(S);
							}
							ncount++;
						}
					}
        		//end=clock();
        		//cout<<"cpu time is: "<<end-start<<endl;
        		//cout<<"good sofor"<<endl;
        		cout<<"return successful"<<endl;
        		return result;
	 	}
        /*vector<vector<int>> allroutes(int k)
        {
        	vector<vector<int>>result;
        	for(int i=0;i<nodenum;i++)
        	{
				vector<int>d(nodenum,INT_MAX);
				memset(p,-1,nodenum*sizeof(int));
				set<int>ts;
				int size=nodenum;
				dijkstra(i,i,d,p,neie[k],nein[k],neieid[k],esigns[k],nodenum,WD,ts,size);
				result.push_back(d);
        	}
        	return result;
        }*/
        pair<int,vector<int>> tunel(int s,int t,int k,int m=0)
		{
        	vector<int>d(nodenum,INT_MAX);
        	set<int>ts;
        	s=s*NUT;
        	ts.insert(t);
        	int size=1;
        	memset(p,-1,nodenum*sizeof(int));
			dijkstra(s,s,d,p,neie[k],nein[k],neieid[k],esigns[k],nodenum,WD,ts,size);
			//cout<<"ppp"<<endl;
			vector<int>rout;
			int hop=0;
			int tt=t;
			int min=INF;
			int prn=-1;
			for(int i=1;i<W;i++)
			{
				if(d[tt*W+i]<min)
				{	
					min=d[tt*W+i];
					prn=tt*W+i;
				}
			}
			//cout<<"what fu"<<endl;
			if(prn<0){
				return make_pair(INF,rout);
			}
			if(prn>=0)
			{
				while(prn!=s)
				{
					int eid=p[prn];
					prn=edges[eid].s;
					rout.push_back(eid);
					if(m==0)
					{
						if(IFHOP<1)
							esigns[k][eid]*=-1;
						if(IFHOP==1)
						{
							eid=(eid/WD)*WD;
							for(int j=0;j<WD;j++)
								esigns[k][eid+j]*=-1;
						}
					}
					hop++;
				}
			}
			//cout<<"oooo"<<endl;
			return make_pair(min,rout);
		}
        static bool compare(pair<int,int>&a,pair<int,int>&b)
        {
        	if(a.second<b.second)
        		return true;
        	return false;
        }
        virtual pair<int,int> prepush(int s,int t,int bw)
        {
        };
};
class Bellmanor:public algbase
{
	private:
		edge *dev_edges,*aedges;
		int*dev_m1,*m1,*dev_pre,*pre,*pred,*dev_pred,*dev_d,*d,*dev_mask,*mask,*dev_leveln,*leveln;
		int*dev_m2,*m2;
		int*dev_rela,*rela;
		int presize,dsize,masksize,levelnsize;
		int edgesize,nodenum,pesize,pnodesize;
		int neisize,duansize;
		int *choosel,*dev_choosel;
		int *rout,*dev_rout;
		int *routn,*dev_routn,*order,*dev_order;
		vector<int>hleveln,ancestor;
		int maxbw;
		int *dev_qian,*qian,*dev_qbeg,*qbeg,*dev_qsize,*qsize;
		epair *dev_nei,*nei;
		int *dev_duan,*duan;
		int *dev_beg,*beg;
		int *dev_value,*value;
		int *dev_height,*height;
		int *dev_has,*has;
		int*w,*dev_w;
		vector<vector<int>>neibour;
		vector<edge>edges;
		vector<int>ordernode;
		void allocate(int maxn,int maxedges);
		void copydata(int s,vector<edge>&edges,int nodenum);
		void dellocate();
		int W;
		int *st,*te,*dev_st,*dev_te;
		int *chan,*dev_chan;
		int*dev_p;
		int*esignes;
		vector<vector<int>>neibn;
		int *mark,*dev_mark;
		int*rudu,*dev_rudu;
		int*rudw,*dev_rudw;
		int*ruid,*dev_ruid;
		int*rid,*dev_rid;
		int mm;
		vector<pair<int,int>>stp;
		//
		vector<int>L;
		vector<int>S;
		vector<int>NF;
		int ncount;
		vector<int>Size;
		vector<int>nodeoff;
		vector<int>leveloff;
		vector<vector<Sot>>stps;
		vector<vector<int>>rus,ruw;
		vector<vector<int>>esigns;
	public:
		 Bellmanor();
	 	 void topsort();
	 	 virtual pair<int,int> prepush(int s,int t,int bw){};
	 	 virtual bool cutcake(int index){};
	 	 virtual vector<vector<Rout>> routalg(int s,int t,int bw);
	 	 virtual ~Bellmanor(){}
	 	 virtual void updatE(vector<vector<int>>&esigns);
	 	 virtual void updatS(vector<vector<Sot>>&stpair);	 	 
	 	 virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,int _nodenum);
};
class BFSor:public algbase
{
	private:
		edge *dev_edges,*aedges;
		int*dev_m,*m,*dev_pre,*pre,*pred,*dev_pred,*dev_d,*d,*dev_mask,*mask,*dev_leveln,*leveln;
		int*dev_rela,*rela;
		int presize,dsize,masksize,levelnsize;
		int edgesize,nodenum,pesize,pnodesize;
		int neisize,duansize;
		int *choosel,*dev_choosel;
		int *rout,*dev_rout;
		int *routn,*dev_routn,*order,*dev_order;
		vector<int>hleveln,ancestor;
		int maxbw;
		int *dev_qian,*qian,*dev_qbeg,*qbeg,*dev_qsize,*qsize;
		epair *dev_nei,*nei;
		int *dev_duan,*duan;
		int *dev_beg,*beg;
		int *dev_value,*value;
		int *dev_height,*height;
		vector<vector<int>>neibour;
		vector<edge>edges;
		vector<int>ordernode;
		void allocate(int maxn,int maxedges);
		void copydata(int s,vector<edge>&edges,int nodenum);
		void dellocate();
		int W;
		int *st,*te,*dev_st,*dev_te;
		int *chan,*dev_chan;
		int*esignes,*dev_esignes;
		vector<vector<int>>neibn;
		int *mark,*dev_mark;
		vector<pair<int,int>>stp;
		//add parameters
		vector<int>L;
		vector<int>S;
		vector<int>NF;
		int ncount;
		vector<int>Size;
		vector<int>nodeoff;
		vector<int>leveloff;
		vector<vector<Sot>>stps;
		int*dev_p;
		int *stid,*dev_stid;
		vector<vector<int>>nein,neie;
		vector<event>addevent;
		vector<vector<int>>delevent;
	public:
		 BFSor();
	 	 void topsort();
	 	 virtual pair<int,int> prepush(int s,int t,int bw){};
	 	 virtual bool cutcake(int index){};
	 	 virtual vector<vector<Rout>> routalg(int s,int t,int bw);
	 	 virtual void updatE(vector<vector<int>>&esigns);
	 	 virtual void updatS(vector<vector<Sot>>&stpair);	 	 
	 	 virtual ~BFSor(){dellocate();}
	 	 virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,int _nodenum);
};

class PBFSor:public algbase{
    public:
		vector<vector<int>>neibour;
		vector<int>ancestor;
		int edgesize,nodenum,pesize,pnodesize,maxbw;
		vector<vector<vector<int>>>mask;
		vector<edge>edges;
		vector<int>dist;
		vector<int>pre;
		vector<int>leveln;
		vector<int>exn2n;
		vector<vector<int>>rela;
		vector<int>order;
		vector<vector<vector<int>>>neie;
		vector<vector<vector<int>>>nein;
		vector<int>esign;
		vector<pair<int,int>>stes;
		vector<vector<Sot>>stpairs;
		vector<int>L;
		int W;
		//add
		vector<vector<vector<int>>>neieid;
		vector<vector<int>>esigns;
		vector<vector<int>>tesigns;
		int *stid,*dev_stid;
		PBFSor():L(3,0){};
        void topsort()
        {
        }
        virtual bool cutcake(int index){
        }
        virtual void updatE(vector<vector<int>>&_esigns){
        	esigns=_esigns;
        	memset(p,-1,LY*YE*nodenum*sizeof(int));
        };
        virtual void init(pair<vector<edge>,vector<vector<int>>>extenedges,vector<pair<int,int>>stpair,int _nodenum){
        	maxbw=500;
        	stes=stpair;
        	//cout<<"in pathalg init "<<stes.size()<<endl;
        	edges=extenedges.first;
        	esigns=extenedges.second;
        	edgesize=edges.size(),
        	nodenum=_nodenum;
        	vector<vector<int>>nd(nodenum*LY,vector<int>());
        	neibour=nd;
        	vector<int>ad(nodenum*LY,0);
        	ancestor=ad;	
    		vector<int>bl(BS,0);
    		leveln=bl;
    		vector<int>dd(nodenum*LY,inf);
    		dist=dd;
    		vector<int>pp(nodenum*LY,-1);
    		pre=pp;
			pesize=edges.size();
			W=WD+1;
			//cout<<"what f"<<endl;
			for(int k=0;k<LY;k++)
			{
				vector<vector<int>>tmpn(nodenum,vector<int>());
				vector<vector<int>>tmpe(nodenum,vector<int>());
				vector<vector<int>>tmpeid(nodenum,vector<int>());
				for(int i=0;i<edges.size();i++)
					{
						int s=edges[i].s;
						int t=edges[i].t;
						tmpn[s].push_back(t);
						tmpe[s].push_back(esigns[k][i]);
						tmpeid[s].push_back(i);
					}
				neie.push_back(tmpe);
				nein.push_back(tmpn);
				neieid.push_back(tmpeid);
			}
			p=new int[nodenum*LY*YE];
        }
        virtual void updatS(vector<vector<Sot>>&stpair){
               	stpairs=stpair;
               	L[0]=0;
               	L[1]=LY1;
               	L[2]=LY;
               }
        virtual void updatR(int k,vector<int>&rout){
        	//cout<<"in updat R"<<endl;
        	 		for(int i=0;i<rout.size();i++)
        	 			{
        					int eid=rout[i];
        					if(IFHOP<1)
        						esigns[k][eid]*=-1;
        					if(IFHOP==1)
        					{
        						eid=(eid/WD)*WD;
        						for(int j=0;j<WD;j++)
        							esigns[k][eid+j]*=-1;
        					}
        	 			}
        	 	};
        virtual vector<vector<Rout>> routalg(int s,int t,int bw){
        	//cout<<"in BFS rout alg"<<endl;
			time_t start,end;
			start=clock();
			vector<vector<Rout>>result(2,vector<Rout>());
			int ncount=0;
			for(int y=1;y<PC+1;y++)
				for(int k=L[y-1];k<L[y];k++)
				{
					for(int l=0;l<stpairs[y-1].size();l++)
					{
						int off=ncount*nodenum;
						vector<int>dist(nodenum,INT_MAX/2);
						//vector<int>pre(nodenum,-1)
						int s=stpairs[y-1][l].s;
						set<int> ts=stpairs[y-1][l].ts;
						vector<int>ters=stpairs[y-1][l].ters;
						int size=stpairs[y-1][l].size;
						BFS(s,s,dist,p+off,neie[k],nein[k],neieid[k],esigns[k],ts,size,WD);
						for(int i=0;i<ters.size();i++)
							{
								vector<int>rout;
								int hop=0;
								int prn=ters[i];
								int d=dist[ters[i]];
								if(dist[prn]>WD)
									continue;
								int id=stpairs[y-1][l].mmpid[ters[i]];
								Rout S(s,ters[i],id,d,off,k);
								result[y-1].push_back(S);
							}
					  ncount++;
					}
				}
			end=clock();
			return result;
	 	}
        pair<int,vector<int>> tunel(int s,int t,int k,int m=0){
	 		int tnode=-1;
			int tv=WD+1;
			vector<int>dist(nodenum,INT_MAX);
        	memset(p,-1,nodenum*sizeof(int));
			set<int> ts;
			ts.insert(t);
			int size=1;
			BFS(s,s,dist,p,neie[k],nein[k],neieid[k],esigns[k],ts,size,WD);
			vector<int>rout;
			int hop=0;
			int prn=t;
			int d=dist[t];
			if(p[prn]<0||d>WD){
				return make_pair(INF,rout);
			}
			if(prn>=0)
			{
				while(prn!=s)
				{
					int eid=p[prn];
					rout.push_back(eid);
					prn=edges[eid].s;
					if(m==0)
					{
						if(IFHOP<1)
							esigns[k][eid]*=-1;
						if(IFHOP==1)
						{
							eid=(eid/WD)*WD;
							for(int j=0;j<WD;j++)
								esigns[k][eid+j]*=-1;
						}
					}
					//cout<<eid<<" ";
				}
				//cout<<endl;
			}
			return make_pair(d,rout);
	 	}
        static bool compare(pair<int,int>&a,pair<int,int>&b)
        {
        	if(a.second<b.second)
        		return true;
        	return false;
        }
        virtual pair<int,int> prepush(int s,int t,int bw)
        {
        };
};

#endif //CSPALLOC_PATHALG_H
