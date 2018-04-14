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
			vector<double>u(EDge,1);
			vector<vector<pair<int,int>>>epath(EDge,vector<pair<int,int>>());
			map<pair<int,int>,vector<int>>pathe;
			vector<vector<double>>x(ser.size(),vector<double>());
			vector<double>y(ser.size(),0);
			double alpha=4;
			double q=0.9;
			cout<<"asd"<<endl;
			for(int i=0;i<ser.size();i++)
			{
				double as=0;
				int num=min(_PathSets[i].num,10);
				//cout<<_PathSets[i].num<<endl;
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
			for(int t=0;t<10000000;t++)
			{
				if(t>1000)
					{if(t<10)
						tt++;

					}
				vector<double> sigxt(ser.size(),0);
				map<pair<int,int>,double>sigu;
				vector<double>esigxt(EDge,0);
				vector<double>tu(EDge,1);
				vector<double>asigxt(ser.size(),0);
				vector<double>ty(ser.size(),0);
				vector<vector<double>>tx(ser.size(),vector<double>());
				double objective=0;
				for(int i=0;i<EDge;i++)
					for(int j=0;j<epath[i].size();j++)
						sigu[epath[i][j]]+=u[i];
				for(int i=0;i<ser.size();i++)
					{
						double max=0;
						int id=-1;
						double toto=0;
						for(int j=0;j<x[i].size();j++)
						{
							double data=pow(pow(y[i],-alpha)/sigu[make_pair(i,j)],1/(1-q));
							if(data>max){
								max=data;
								id=j;
							}
							toto+=data;
							tx[i].push_back(data);
						}
						for(int j=0;j<tx[i].size();j++)
						{
							if(tx[i][j]<=max)tx[i][j]=0;
						}
						tx[i][id]=max;
					}
				for(int i=0;i<ser.size();i++)
					for(int j=0;j<x[i].size();j++)
						for(int k=0;k<pathe[make_pair(i,j)].size();k++)
							esigxt[pathe[make_pair(i,j)][k]]+=x[i][j];
				for(int i=0;i<EDge;i++)
					tu[i]=u[i]+(1/tt)*((1-q)/2)*u[i]*((esigxt[i]-capacity[i])/capacity[i]);
				for(int i=0;i<ser.size();i++)
					{
						for(int j=0;j<x[i].size();j++)
							asigxt[i]+=pow(x[i][j],q);
						ty[i]=y[i]+(1/tt)*((1-q)/(2*(alpha+q-1)))*y[i]*(asigxt[i]-pow(y[i],q))/(pow(y[i],q));
					}
				x=tx;
				y=ty;
				u=tu;
				for(int i=0;i<ser.size();i++)
					objective+=-pow(y[i],-3)/3;
				count++;
				if(objective>bestv)
				{
					count=0;
					bestv=objective;
				}
				if(count>100000)break;
				cout<<"obj is: "<<objective<<endl;
			}
			int toto=0;
			for(int i=0;i<ser.size();i++)
				{
					for(int j=0;j<x[i].size();j++)
						{
							toto+=x[i][j];
							if(x[i][j]>=10)
							cout<<x[i][j]<<" ";
							else
								cout<<0<<" ";
							for(int k=0;k<pathe[make_pair(i,j)].size();k++)
								capacity[pathe[make_pair(i,j)][k]]-=x[i][j];
						}
					cout<<endl;
				}
			for(int i=0;i<EDge;i++)
				if(capacity[i]<-1)
					cout<<"overflow "<<endl;
			cout<<"toto is "<<toto<<endl;
			return rdata;
		};
};