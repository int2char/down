/*
 * this is a header and implement of serial GA
 * no correlate .cpp file exit;
 * */
#include"Graph.h"
#include "service.h"
#include"taskPath.h"
#include"valuemark.h"
#include"curand_kernel.h"
#include"iostream"
#include <fstream>
#include"const.h"
#include<algorithm>
#include<vector>
#include<math.h>
#include"BFS.h"
#include"time.h"
#include"PathArrange.h"
#include<map>
using namespace std;
#define power .5
#define NNN 10000
double distan[NNN];
int pedge[NNN];
struct FitVM {
	double value;
	int mark;
	FitVM(double _value=0,double _mark=0){
		value = _value;
		mark = _mark;
	}
};
class NewGA
{
	private:
	taskPath* PathSets;
	Graph &G;
	int*st;
	int*te;
	double*demand;
	int**chormes;
	int**childs;
	int**monsters;
	double*capacity;
	double best;
	int totalweight;
	vector<FitVM> Fits;
	vector<pair<int,vector<int>>> Result;
	vector<pair<double,int>>answers;
	public:
		NewGA( Graph &_G):G(_G){
			st = (int*)malloc(Task*sizeof(int));
			te = (int*)malloc(Task*sizeof(int));
			demand = (double*)malloc(Task*sizeof(double));
			capacity = (double*)malloc(sizeof(double)*EDge);
			for (int i = 0; i < EDge; i++)
				capacity[i] = G.incL[i].capacity;
			chormes = (int**)malloc(sizeof(int*)*pop);
			childs = (int**)malloc(sizeof(int*)*Beta);
			monsters = (int**)malloc(sizeof(int*)*Gama);
			for (int i = 0; i < pop; i++)
				chormes[i] = (int*)malloc(sizeof(int)*Task);
			for (int i = 0; i < Beta; i++)
				childs[i] = (int*)malloc(sizeof(int)*Task);
			for (int i = 0; i<Gama; i++)
				monsters[i] = (int*)malloc(sizeof(int)*Task);
		};
		~NewGA(){
			free(st);
			free(te);
			free(demand);
			free(capacity);
			free(chormes);
			free(childs);
			free(monsters);
		}
	public:
		vector<pair<string,double> > GAsearch(vector<service>&ser, taskPath*_PathSets,ofstream&Out){
			cout<<"ops"<<endl;
			vector<double>capacity(EDge,100);
			vector<pair<string,double>> rdata;
			vector<double>u(EDge,0.1);
			vector<double>f(EDge,0);
			vector<vector<pair<int,int>>>epath(EDge,vector<pair<int,int>>());
			map<pair<int,int>,vector<int>>pathe;
			vector<vector<double>>x(ser.size(),vector<double>());
			vector<double>y(ser.size(),0);
			vector<vector<double>>overtime(ser.size(),std::vector<double>());
			map<pair<int,int>,double>wsigu;
			vector<double> tsum(ser.size(),-1e10);
			double alpha=4;
			double q=0.9;
			cout<<"asd"<<endl;
			_PathSets[111]=_PathSets[0];
			for(int i=0;i<ser.size();i++)
			{
				double as=0;
				int num=min(_PathSets[i].num,5);
				//cout<<_PathSets[i].num<<endl;
				if(num<2)cout<<"axc: "<<i<<endl;
				for(int j=0;j<num;j++)
					{
						if(_PathSets[i].Pathset[j][0]>-1)
							{
								x[i].push_back(1);
								as+=1;
							}
						int k=0;
						while(_PathSets[i].Pathset[j][k]>-1)
							{
								epath[_PathSets[i].Pathset[j][k]].push_back(make_pair(i,j));
								pathe[make_pair(i,j)].push_back(_PathSets[i].Pathset[j][k]);
								k++;
							}
							overtime[i].push_back(1);
					}
				y[i]=as;
			}

			for(int i=0;i<ser.size();i++)
				for(int j=0;j<x[i].size();j++)
					if(pathe[make_pair(i,j)].size()<=0)
						cout<<"fucker"<<endl;
			double bestv=-INT_MAX/2;
			int count=0;
			double tt=1;
			for(int t=0;t<10000;t++)
			{

				cout<<"before init "<<endl;
				vector<double> sigxt(ser.size(),0);
				map<pair<int,int>,double>sigu;
				map<pair<int,int>,double>sigf;
				vector<double>esigxt(EDge,0);
				vector<double>tu(EDge,1);
				vector<double>asigxt(ser.size(),0);
				vector<double>ty(ser.size(),0);
				vector<vector<double>>tx(ser.size(),vector<double>());
				vector<double>flag(ser.size(),0);
				cout<<"init sucess"<<endl;
				double objective=0;
				for(int i=0;i<EDge;i++)
					for(int j=0;j<epath[i].size();j++)
						sigu[epath[i][j]]+=u[i];
				for(int i=0;i<ser.size();i++)
					for(int j=0;j<x[i].size();j++)
						for(int k=0;k<pathe[make_pair(i,j)].size();k++)
							esigxt[pathe[make_pair(i,j)][k]]+=x[i][j];
				cout<<"before u"<<endl;
				for(int i=0;i<EDge;i++)
					{
							
							tu[i]=u[i]+(0.05)*u[i]*(esigxt[i]-capacity[i])/capacity[i];
							if(esigxt[i]-capacity[i]>0)
								for(int k=0;k<epath[i].size();k++)
									{
										flag[epath[i][k].first]=1;
									}
					}
				cout<<"after u"<<endl;
				for(int i=0;i<ser.size();i++)
				{
					for(int j=0;j<x[i].size();j++)
						asigxt[i]+=pow(x[i][j],q);
					if(flag[i]>0.5&&asigxt[i]-pow(y[i],q)<0)
						ty[i]=y[i];
					else
						ty[i]=y[i]+((1-q)/(2*(alpha+q-1)))*y[i]*(asigxt[i]-pow(y[i],q))/pow((y[i]),q);
				}
				cout<<"agter y"<<endl;
				for(int i=0;i<ser.size();i++)
					{
						double min=1e20;
						int id=-1;
						double toto=0;
						double sum=0;
						for(int j=0;j<x[i].size();j++)
						{
							double data=sigu[make_pair(i,j)];
							sum+=sigu[make_pair(i,j)];
							if(data<min){
								min=data;
								id=j;
							}
							toto+=data;
							tx[i].push_back(data);
						}
						for(int j=0;j<tx[i].size();j++)
							tx[i][j]=0;
						//cout<<sum<<" "<<tsum[i]<<endl;
						if(x[i][id]>0&&flag[i]<0.5)
							sum=max(sum,tsum[i]);
						tx[i][id]=pow(pow(ty[i],-alpha)/sum,1/(1-q))*ty[i];
						tsum[i]=sum;
						if(tsum[i]==0)cout<<"erro!!!"<<endl;
					}
				cout<<"after tx"<<endl;
				/*int overflow=0;
				vector<double> ca=capacity;
				double toto=0;
				for(int i=0;i<ser.size();i++)
				{
					for(int j=0;j<x[i].size();j++)
						{
							if(x[i][j]>0)
							toto+=x[i][j];
							for(int k=0;k<pathe[make_pair(i,j)].size();k++)
								ca[pathe[make_pair(i,j)][k]]-=x[i][j];
						}
				}
				int gg=0;
				for(int i=0;i<EDge;i++)
					if(ca[i]<-1)
						{
							gg=1;
							cout<<"&&&&&&&&&&&&&&&&& "<<i<<endl;
							for(int k=0;k<epath[i].size();k++)
								{
									int a=epath[i][k].first;
									int b=epath[i][k].second;
									if(x[a][b]>0)
										cout<<"************ "<<a<<" "<<y[a]<<" "<<ty[a]<<" "<<x[a][b]<<" "<<wsigu[epath[i][k]]<<" "<<sigu[epath[i][k]]<<" "<<tx[a][b]<<endl;
								}
							//cout<<i<<" "<<ca[i]<<endl;
							overflow++;
						}
				*/
				x=tx;
				y=ty;
				u=tu;
				wsigu=sigu;
				//for(int i=0;i<ser.size();i++)
					//objective+=-pow(asigxt[i],-3)/3;
				//count++;
				//if(gg>0)cout<<t<<endl;
				//if(objective>bestv)
				{
					//count=0;
					//bestv=objective;
				}
				//if(count>200)break;
				//cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;*/
				//cout<<"obj is: "<<overflow<<" "<<toto<<" "<<objective<<endl;
				cout<<"obj is: "<<objective<<endl;
			}
			int toto=0;
			cout<<"bestv is "<<bestv<<endl;
			cout<<"toto is "<<toto<<endl;
			return rdata;
		};
};