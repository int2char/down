#ifndef CSPALLOC_GRAPH_H
#define CSPALLOC_GRAPH_H
#include<bits/stdc++.h>
#include"edge.h"
#include"pathalg.h"
#include<set>
#include<queue>
#define MAXITER 10000
using namespace std;
enum SPWAY {NORMAL,ROUTE,ROTATE,ROTATE_DELETE,PUSH};
struct levelGraph {
    vector<int>startnodes;
    vector<int>startedges;
    int nodenum=-1;
    int edgenum=-1;
    vector<edge>edges;
    vector<vector<edge>>neartable;
    int levelsize;
};
struct fedge{
	int from,to,price,cap;
	fedge(int f,int t,int p,int c):from(f),to(t),price(p),cap(c){};
};
struct mmmp{
	bool operator ()(demand a,demand b)
	{
		return a.estimate<b.estimate;
		
	};
};
double randomExponential(double lambda)
{
    double pV = 0.0;
    while(true)
    {
        pV = (double)rand()/(double)RAND_MAX;
        if (pV != 1)
        {
            break;
        }
    }
    pV = (-1.0/lambda)*log(1-pV);
    return pV;
};
class Graph
{
    public:
        double remain;
        vector<edge>edges,extenedges;
        vector<vector<edge>>extable;
        unordered_map<int,levelGraph>levels ;
        int n,width,maxnode,maxedge;
        vector<int>etn2n,exe2e;
        vector<vector<int>>relate,neartable;
        vector<vector<int>>esignes;
        algbase&router1;
        algbase&router2;
        algbase*mrouter;
        int current;
        int method;
        priority_queue<event,vector<event>,cevent>timeque;
        vector<double>average;
        vector<double>averhops;
        vector<int>blocks;
        vector<int>pairsize;
        vector<int>tasksize;
        int busy;
        int reles;
        double time_count;
        vector<double>times;
        vector<vector<pair<int,int>>>delevent;
        vector<pair<int,pair<int,int>>>addevent;
		float LAMBDA;
		float ADDNUM;
		int METHOD;
		int PARAL;
		double RATIO;
		vector<vector<int>>HDist;
		vector<vector<int>>DDist;
        void run(float _ratio,float lambda,float MAXNUM,int _method,int _paral)
        {
        	
        	//cout<<"fic"<<endl;
			RATIO=_ratio;
        	LAMBDA=lambda;
        	ADDNUM=MAXNUM;
        	METHOD=_method;
        	PARAL=_paral;
        	busy=0;
        	int count=0;
        	current=0;
        	int release=0;
            vector<pair<int,pair<int,int>>>ee(MAXITER,pair<int,pair<int,int>>(-1,pair<int,int>(-1,-1)));
            vector<vector<pair<int,int>>>dd(MAXITER,vector<pair<int,int>>());
        	addevent=ee;
        	delevent=dd;
        	for(int i=0;i<ADDNUM;i++)
        	{
        		double k=rand()%100;
        		if(k/100<=RATIO)
        			{
        				addevent[i].first=1;
        				addevent[i].second.first=rand()%10+10;
        				addevent[i].second.second=rand()%10+10;
        			}
        	}
        	//cout<<"asd"<<endl;
        	for(int h=0;h<ADDNUM;h++)
        	{
        		//cout<<"deleting.................... "<<current<<endl;
        		if(delevent[h].size()>0)
        		{
					for(int i=0;i<delevent[h].size();i++)
					{
						int eid=delevent[h][i].second;
						int k=delevent[h][i].first;
						esignes[k][eid]*=-1;

					}
					//router2.updatE(esignes);
					router1.updatE(esignes);
        		}
        		if(addevent[current].first>0)
        		{
        			//cout<<"adding................. "<<current<<endl;
        			routalg(0,0,0);
        			count++;
        		}
        		current++;
        	}
        	//cout<<"kkkk"<<endl;
        	//cout<<"release: "<<release<<endl;
        	//cout<<"beasy: "<<busy<<endl;
        	for(int i=0;i<average.size();i++)
        		cout<<average[i]<<" ";
        	cout<<endl;
        	for(int i=0;i<averhops.size();i++)
                cout<<averhops[i]<<" ";
            cout<<endl;
            for(int i=0;i<tasksize.size();i++)
                cout<<tasksize[i]<<" ";
            cout<<endl;
        	for(int i=0;i<blocks.size();i++)
        		cout<<blocks[i]<<" ";
        	cout<<endl;
        	for(int i=0;i<blocks.size();i++)
			cout<<(double)blocks[i]/(double)tasksize[i]<<" ";
        	cout<<endl;
        	for(int i=0;i<times.size();i++)
        	    cout<<times[i]<<" ";
        	cout<<endl;
        }
        vector<vector<Sot>>Getspair(vector<vector<demand>>&ds)
		{
        	vector<vector<vector<pair<int,int>>>>sarray(PC,vector<vector<pair<int,int>>>(n,vector<pair<int,int>>()));
        	for(int k=0;k<PC;k++)
        	{
        		for(int i=0;i<ds[k].size();i++)
        		{
        			demand dd=ds[k][i];
        			if(dd.mark==1)continue;
        			sarray[k][dd.s].push_back(make_pair(dd.t,dd.id));
        		}
        	}
        	vector<vector<Sot>>stpair(PC,vector<Sot>());
        	for(int k=0;k<PC;k++)
        	{
        		for(int i=0;i<sarray[k].size();i++)
        		{
        			if(sarray[k][i].size()==0)continue;
        			Sot S(i);
        			for(int j=0;j<sarray[k][i].size();j++)
        				S.push(sarray[k][i][j].first,sarray[k][i][j].second);
        			stpair[k].push_back(S);
        		}	
        	}
        	return stpair;
		}
        vector<vector<demand>>Gendemand(vector<int>&tasknum)
		{
        	vector<vector<demand>>ds(PC,vector<demand>());
			int c1=0;
			for(int k=0;k<PC;k++)
			{
				set<pair<int,int>>se;
				c1=0;
				for(int i=0;i<tasknum[k];i++)
				{
					while(1)
					{
						int s=rand()%n;
						int t=s;
						while(t==s)t=rand()%n;
						if(se.find(make_pair(s,t))==se.end())
							{
								se.insert(make_pair(s,t));
								ds[k].push_back(demand(s,t,c1++,(int)randomExponential(LAMBDA)));
								break;
							}
						}
					}
			
			}
			return ds;
		}
        vector<vector<demand>>greedy(vector<vector<demand>>&ds,vector<demand>&addin,vector<demand>&block,double &timecount)
		{
            algbase&router=router2;
        	float starty=float(1000*clock())/ CLOCKS_PER_SEC;
        	vector<vector<Sot>>stpair=Getspair(ds);
        	time_t startu=clock();
        	cout<<"get pair: "<<startu-starty<<endl;
        	if(PARAL>0)
			{router2.updatS(stpair);
			router2.updatE(esignes);}
        	else
        	{
        		router1.updatS(stpair);
        		router1.updatE(esignes);
        	}
			time_t endu=clock();
			cout<<"updating time: "<<endu-startu<<endl;
			time_t startro=clock();
			vector<vector<Rout>> result;
			if(PARAL>0)
				result=router2.routalg(0,0,0);
			else
				result=router1.routalg(0,0,0);
			time_t endro=clock();
			cout<<"rout alg time: "<<endro-startro<<endl;
			vector<vector<demand>>remain(PC,vector<demand>());
			time_t starta=clock();
			for(int k=0;k<PC;k++)
					for(int i=0;i<result[k].size();i++)
					{
							int id=result[k][i].id;
							int vv=result[k][i].di;
							ds[k][id].routid.push(make_pair(i,vv));
					}
			//cout<<"here!"<<endl;
			//cout<<result[0].size()<<" "<<result[1].size()<<endl;
			vector<priority_queue<pair<int,int>,vector<pair<int,int>>,paircomp>>dsque(2,priority_queue<pair<int,int>,vector<pair<int,int>>,paircomp>());
			for(int k=0;k<PC;k++)
					for(int i=0;i<ds[k].size();i++)
							{
									if(!ds[k][i].routid.empty())
											{
											  int vv=ds[k][i].routid.top().second;
											  dsque[k].push(make_pair(i,vv));
											  
											}
									else
										{
											block.push_back(ds[k][i]);
										}
							}
			time_t mid=clock();
			cout<<"build queue: "<<mid-starta<<endl;
			int count=0;
			int sss=0;
			for(int k=0;k<PC;k++)
			{
				int newid=0;
				while(!dsque[k].empty())
				{
						count++;
						pair<int,int> pp=dsque[k].top();
						demand nde=ds[k][pp.first];
						dsque[k].pop();
						int flag=0;
						while(!nde.routid.empty())
						{
								int id=nde.routid.top().first;
								nde.routid.pop();
								Rout RR=result[k][id];
								int ly=RR.ly;
								int offf=RR.offf;
								int v=RR.di;	
								int s=RR.s;
								int node=RR.t;
								vector<int>rout;
								int ff=1;
								int wv=0;
								while(node!=s)
								{
									
									int eid;
									if(PARAL>0)
										eid=router2.p[node+offf];
									else
										eid=router1.p[node+offf];
									if(IFHOP>0)
										offf-=n;
									if(esignes[ly][eid]<0)
									{
										ff=-1;
										break;
									}
									wv+=esignes[ly][eid];
									rout.push_back(eid);
									node=edges[eid].s;
								}
								if(ff<0)continue;
								for(int i=0;i<rout.size();i++)
										{
												int eid=rout[i];
												esignes[ly][eid]*=-1;
										}
								flag=rout.size();
								nde.rout=rout;
								nde.mark=ly;
								nde.value=wv;
								addin.push_back(nde);
								break;
						}
						if(flag==0){
								int nid=newid++;
								demand dd(nde.s,nde.t,nid,nde.sert);
								remain[k].push_back(dd);
								
						}
				}
			}
		float enda=float(1000*clock())/ CLOCKS_PER_SEC;
		cout<<"time is: "<<enda-starty<<endl;
		timecount+=(enda-starty);
		return remain;
		}
        void routalg(int s,int t,int bw)
		{
        	vector<int>tasknum;
        	int num1=addevent[current].second.first;
        	int num2=addevent[current].second.second;
        	//cout<<"fuck"<<endl;
        	tasknum.push_back(num1*DSIZE);
        	tasknum.push_back(num2*3*DSIZE);
        	vector<vector<demand>>ds=Gendemand(tasknum);
        	int totalnum=tasknum[0]+tasknum[1];
        	tasksize.push_back(tasknum[0]+tasknum[1]);
			vector<demand>block;
			vector<demand>addin;
			double timecount=0;
			if(METHOD==2)
				sortadd(ds,addin,block,timecount);
			if(METHOD==1)
				serialadd(ds,addin,block,timecount);
			if(METHOD==0)
			{
				while(ds[0].size()>0||ds[1].size()>0)
							ds=greedy(ds,addin,block,timecount);
			}
			//cout<<"out method"<<endl;
			times.push_back(timecount);
			int count=0;
			int hops=0;
			for(int i=0;i<addin.size();i++)
				{
					count+=addin[i].value;
					hops+=addin[i].rout.size();
				}
			for(int i=0;i<addin.size();i++)
			{
				
				demand dd=addin[i];
				int serv=dd.sert;//randomExponential(LAMBDA);
				if(serv+current+1<ADDNUM)
					for(int h=0;h<dd.rout.size();h++)
						delevent[serv+current+1].push_back(make_pair(dd.mark,dd.rout[h]));
			}
			//cout<<"what f2"<<endl;
			//if(addin.size()==0)cout<<"what happend!"<<endl;
			average.push_back((double)count/(double)addin.size());
			averhops.push_back((double)hops/(double)addin.size());
			blocks.push_back(block.size());
			/*cout<<"add in rout cost is "<<count<<endl;
			cout<<"add in is "<<addin.size()<<endl;
			cout<<"remain size"<<ds[0].size()+ds[1].size()<<endl;
			cout<<"block size "<<block.size()<<endl;*/
			//cout<<"time is"<<end-start<<endl;
		}
        void serialadd(vector<vector<demand>>&ds,vector<demand>&addin,vector<demand>&block,double&timecount)
        {
        	//cout<<"aas"<<endl;
        	float start=float(1000*clock())/ CLOCKS_PER_SEC;
		router1.updatE(esignes);
        	vector<int>L(3,0);
        	L[0]=0;L[1]=LY1;L[2]=LY2+LY1;
        	for(int y=1;y<PC+1;y++)
        		{
        			for(int i=0;i<ds[y-1].size();i++)
        			{
        				int s=ds[y-1][i].s;
        				int t=ds[y-1][i].t;
        				int flag=0;
        				for(int k=L[y-1];k<L[y];k++)
        				{
        					vector<int> rout=router1.tunel(s,t,k).second;
        					int w=0;
        					if(rout.size()>0){
        						ds[y-1][i].rout=rout;
        						for(int l=0;l<rout.size();l++)
        							{
                						int eid=ds[y-1][i].rout[l];
        								w+=esignes[k][eid];
                						esignes[k][eid]*=-1;
        							}
        						ds[y-1][i].mark=k;
        						ds[y-1][i].value=w;
								router1.updatR(k,rout);
        						addin.push_back(ds[y-1][i]);
        						flag=1;
        						break;
        					}
        				}
        				if(flag==0)block.push_back(ds[y-1][i]);
        			}
        		}
        	float end=float(1000*clock())/ CLOCKS_PER_SEC;
        	//cout<<"alg time is "<<end-start<<endl;
        	timecount+=end-start;	
        }
        void sortadd(vector<vector<demand>>&ds,vector<demand>&addin,vector<demand>&block,double&timecount)
               {
                float start=float(1000*clock())/ CLOCKS_PER_SEC;
                router1.updatE(esignes);
               	vector<int>L(3,0);
               	L[0]=0;L[1]=LY1;L[2]=LY2+LY1;
               	for(int y=0;y<PC;y++)
               	{
               		for(int i=0;i<ds[y].size();i++)
               		{
               			int s=ds[y][i].s;
               			int t=ds[y][i].t;
						pair<int,vector<int>>dd=router1.tunel(s,t,L[y],1);
       					ds[y][i].estimate=dd.first;
       					ds[y][i].rout=dd.second;
               		}
               	}
               	sort(ds[0].begin(),ds[0].end(),mmmp());
               	sort(ds[1].begin(),ds[1].end(),mmmp());
               	for(int y=1;y<PC+1;y++)
               		{
               			for(int i=0;i<ds[y-1].size();i++)
               			{
               				int s=ds[y-1][i].s;
               				int t=ds[y-1][i].t;
               				int flag=0;
               				vector<int> rout=ds[y-1][i].rout;
							int w=0;
							if(rout.size()>0){
								ds[y-1][i].rout=rout;
								int bb=0;
								for(int l=0;l<rout.size();l++)
								{
									if(esignes[L[y-1]][rout[l]]<0)
										{bb=1;break;}
								}
								if(bb==0)
								{
									for(int l=0;l<rout.size();l++)
										{
											int eid=rout[l];
											w+=esignes[L[y-1]][eid];
											esignes[L[y-1]][eid]*=-1;
										}
									router1.updatR(L[y-1],rout);
									ds[y-1][i].mark=L[y-1];
									ds[y-1][i].value=w;
									addin.push_back(ds[y-1][i]);
									flag=1;
								}
							}
							if(flag==0)
								{
									for(int k=L[y-1]+1;k<L[y];k++)
									{
										vector<int> rout=router1.tunel(s,t,k).second;
										int w=0;
										if(rout.size()>0){
											ds[y-1][i].rout=rout;
											for(int l=0;l<rout.size();l++)
												{
													int eid=ds[y-1][i].rout[l];
													w+=esignes[k][eid];
													esignes[k][eid]*=-1;
												}
											ds[y-1][i].mark=k;
											ds[y-1][i].value=w;
											router1.updatR(k,rout);
											addin.push_back(ds[y-1][i]);
											flag=1;
											break;
										}
									}
								}
               				if(flag==0)block.push_back(ds[y-1][i]);
               			}
               		}
               	float end=float(1000*clock())/ CLOCKS_PER_SEC;
               	timecount+=end-start;		
               	//cout<<"time is "<<end-start<<endl;
               }
        virtual ~Graph(){ 
        };
    protected:
        void addedge(int _s,int _t,int _w,double _bw=500){
            neartable[_s].push_back(edges.size());
            edges.push_back(edge(_s,_t,1));
            neartable[_t].push_back(edges.size());
            edges.push_back(edge(_t,_s,1));
        };
        virtual void GenGraph()=0;
        Graph(int _n,int _degree,algbase&alg1,algbase&alg2):n(_n),width(WD),remain(500),etn2n(n*(width+1),-1),maxnode(0),router1(alg1),router2(alg2),neartable(_n,vector<int>()){

        };
        pair<vector<edge>,vector<vector<int>>> extend()
        {
        	vector<vector<int>>rs(edges.size(),vector<int>());
        	relate=rs;
        	vector<int>es(edges.size()*(width+1),-1);
        	exe2e=es;
        	for(int i=0;i<neartable.size();i++)
				for(int k=0;k<width;k++)
					for(int j=0;j<neartable[i].size();j++)
						{
							if(edges[neartable[i][j]].w+k<=width)
							{
								edge e=edges[neartable[i][j]];
								int s=i+n*k;
								etn2n[s]=i;
								int t=e.t+n*(k+1);
								etn2n[t]=e.t;
								exe2e[extenedges.size()]=neartable[i][j];
								relate[neartable[i][j]].push_back(extenedges.size());
								extenedges.push_back(edge(s,t,e.w));
								if(s>maxnode||t>maxnode)
									maxnode=max(s,t);
							}
						}
        	vector<vector<int>>erelate(extenedges.size(),vector<int>());
        	for(int i=0;i<extenedges.size();i++)
        		erelate[i]=relate[exe2e[i]];
            maxedge=extenedges.size()-1;
            vector<edge> pedges;
            for(int i=0;i<edges.size();i++)
            {
                for(int k=0;k<width;k++)
                	{
                		int s=edges[i].s+n*k;
                		int t=edges[i].t+n*(k+1);
                		pedges.push_back(edge(s,t,1));
                	}
            }
            vector<vector<int>>near(maxnode,vector<int>());
            for(int i=0;i<edges.size();i++)
            {
            	int s=edges[i].s;
            	int t=edges[i].t;
            	edges[i].w=(rand()%10+1);
            	near[s].push_back(edges[i].t);
            }
            vector<edge>redges;
            for(int i=0;i<near.size();i++)
            	for(int j=0;j<near[i].size();j++)
            		redges.push_back(edge(i,near[i][j],1));
            //cout<<"asasd"<<endl;
            vector<vector<int>>esigns(LY,vector<int>());
            for(int i=0;i<LY;i++)
            	for(int j=0;j<edges.size();j++)
                  esigns[i].push_back(edges[i].w);		
            esignes=esigns;
            int W=WD+1;
            vector<vector<int>>nesigns(LY,vector<int>());
            vector<edge>nedges;
            vector<pair<int,int>> stpair;
            int count=0;
            //cout<<"init ing"<<endl;
            edges=redges;
            if(IFHOP>0)
            {
            	router2.init(make_pair(redges,esigns),stpair,n*W);
            	router1.init(make_pair(redges,esigns),stpair,n*W);
            }
            else
            {
            	router2.init(make_pair(redges,esigns),stpair,n);
            	router1.init(make_pair(redges,esigns),stpair,n);
            }
           // cout<<"init end"<<endl;
            return make_pair(redges,esigns);
        };
};
class ERGraph:public Graph{
public:
    ERGraph(int _n,int _degree,algbase&alg1,algbase&alg2):Graph(_n,_degree,alg1,alg2){
    	//cout<<"before gen graph"<<endl;
    	GenGraph();
    	//cout<<"gen graph success"<<endl;
    	extend();
    };
private:
    virtual void GenGraph(){
        int count = 0;
        set<pair<int, int>>flag;
        double threhod = 6*n/((n-1));
        for (int i = 0; i <n; i++)
            for (int j =i+1; j<n;j++)
                if (i != j)
                {
                    double ran=(double)(rand()%n);
                    if (ran<threhod)
                    {
                        if (flag.find(make_pair(i, j))==flag.end())
                        {
                            addedge(i,j,rand()%3+1);
                            flag.insert(make_pair(i,j));
                            flag.insert(make_pair(j,i));
                        }
                    }
                }
    };
private:
    void checkflow(int src,int dst,enum SPWAY cache = NORMAL)
    {}
};
class BAGraph:public Graph{
public:
    BAGraph(int _n,int _degree,algbase&alg1,algbase&alg2):Graph(_n,_degree,alg1,alg2){GenGraph();extend();};
private:
    virtual void GenGraph(){
        int todu = 0;
        int count = 0;
        int k =3;
        vector<int>du(n,0);
        for (int i = 0; i < 5; i++)
        {
            addedge(i,i+1,rand()%3+1);
            du[i]++;
            du[i + 1]++;
            todu += 2;
        }
        for (int i = 5; i < n; i++)
        {
            int addin = 0;
            while (addin < k)
            {
                for (int j = 0; j < n; j++)
                {
                    if (i!=j&&rand() % todu < du[j])
                    {
                        addedge(i,j,rand()%3+1);
                        du[i]++;
                        du[j]++;
                        todu += 2;
                        addin++;
                    }
                    if (addin >= k)
                        break;
                }
            }
        }
    };
};
#endif
