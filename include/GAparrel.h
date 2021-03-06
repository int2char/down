#ifndef GAPARREL
#define GAPARREL
#include"Graph.h"
#include "service.h"
#include"taskPath.h"
#include"valuemark.h"
#include"algorithm"
#include"BFS.h"
#include"PathArrange.h"
#include"time.h"
struct FitVMP {
	float value;
	int mark;
	FitVMP(float _value = 0, float _mark = 0){
		value = _value;
		mark = _mark;
	}
};
struct FitCmp
{
	bool operator()(FitVMP a, FitVMP b)
	{
		return a.value>b.value;
	};
	
};
	
class NewGAParrel
{
private:
	time_t affier=0;
	time_t really_start=0;
	float best;
	taskPath* PathSets;
	int *pathset;
	int *dev_pathset;
	taskPath*dev_PathSets;
	int pathd;
	int taskd;
	Graph &G;
	ofstream &Out;
	int*st;
	int*te;
	float*demand;
	float*dev_demand;
	int*chormes;
	int*dev_chormes;
	int*childs;
	int*dev_childs;
	int *muinfo;
	int*dev_muinfo;
	float*capacity;
	float*popmcap;
	float*dev_popmcap;
	float*dev_capacity;
	vector<FitVMP> Fits;
	FitVMP CuFits[pop];
	FitVMP*dev_CuFits;
	int*randarray;
	int *dev_randarray;
	int *parents;
	int *dev_parents;
	unsigned int *seed;
	unsigned int *dev_seed;
	int*pathnum;
	int*dev_pathnum;
	int*fit_key;
	int*dev_fit_key;
	float*fit_value;
	float*dev_fit_value;
	int*hops;
	int*dev_hops;
	float distca[10000];
	int caped[10000];
	float*rawvalue;
	float *dev_rawvalue;
	int *rawmark;
	int *dev_rawmark;
	int totalweight;
	vector<service>&serv;
	vector<pair<int,vector<int>>> Result;
public:
	NewGAParrel(vector<service>&ser, taskPath*_PathSets, Graph &_G, ofstream&_Out) :serv(ser),PathSets(_PathSets), G(_G), Out(_Out){
		st = (int*)malloc(Task*sizeof(int));
		te = (int*)malloc(Task*sizeof(int));
		demand = (float*)malloc(Task*sizeof(float));
		capacity = (float*)malloc(sizeof(float)*G.m);
		popmcap=(float*)malloc(sizeof(float)*G.m*pop);
		for (int i = 0; i < G.m; i++)
			capacity[i] = G.incL[i].capacity;
		for(int i=0;i<pop;i++)
			for(int j=0;j<G.m;j++)
				popmcap[i*G.m+j]=capacity[j];
		chormes = (int*)malloc(sizeof(int)*pop*Task);
		childs = (int*)malloc(sizeof(int)*(Beta+Gama)*Task);
		randarray = (int*)malloc(sizeof(int)*500000);
		for (int i = 0; i <500000; i++)
			randarray[i] = 1000000 + rand() %999999999;
		parents = (int*)malloc(sizeof(int)* 2 * Beta);
		seed = (unsigned int*)malloc(sizeof(unsigned int));
		*seed = 119137;
		muinfo = (int*)malloc(sizeof(int)*Gama * 3);
		fit_value=(float*)malloc(sizeof(float)*(pop+ALPHA));
		fit_key=(int*)malloc(sizeof(int)*(pop+ALPHA));
		rawvalue=(float*)malloc(sizeof(float)*Task*pop);
		rawmark=(int*)malloc(sizeof(int)*Task*pop);
	    time_t stt =1000*clock()/ CLOCKS_PER_SEC;
		for (int i = 0; i < Task; i++)
		{
			st[i] = ser[i].s;
			te[i] = ser[i].t;
			demand[i] = ser[i].d;
		}
		int max = 0;
		hops=(int*)malloc(sizeof(int)*ROD*Task);
		pathnum = (int*)malloc(sizeof(int)*Task);
		for (int i = 0; i < Task; i++)
		{
			pathnum[i] = 0;
			for (int j = 0; j<ROD; j++)
			{
				hops[i*ROD+j]=PathSets[i].Pathset[j].size()-1;
				if (PathSets[i].Pathset[j].size()>0)
					pathnum[i]++;
				if (PathSets[i].Pathset[j].size()>max)
					max = PathSets[i].Pathset[j].size();
			}
		}
		pathset = (int*)malloc(sizeof(int)*Task*ROD* max);
		pathd = max;
		taskd = max*ROD;
		for (int i = 0; i < Task; i++)
			for (int j=0;j<ROD;j++)
			{
				for (int k = 0; k < max;k++)
					if (k < PathSets[i].Pathset[j].size())
					{
						pathset[i*taskd + j*pathd + k] = PathSets[i].Pathset[j][k];
					}
					else
						pathset[i*taskd + j*pathd + k] = -1;
			}

		time_t send =1000*clock()/ CLOCKS_PER_SEC;
		affier+=send-stt;

	};
	~NewGAParrel(){
		/*cudafree();
		free(st);
		free(te);
		free(demand);
		free(capacity);
		free(popmcap);
		free(chormes);
		free(childs);
		free(hops);
		free(pathnum);
		free(pathset);
		free(randarray);
		free(parents);
		free(muinfo);
		free(fit_key);
		free(fit_value);
		free(rawvalue);
		free(rawmark);*/
	}
	vector<pair<string,float> > GAsearch();
private:

	bool static randcmp(pair<float, pair<int, int> >a, pair<float, pair<int, int> >b)
	{
		if (a.first>b.first)
			return true;
		return false;
	};
	void GoldnessMake(){
		for (int i = 0; i < pop; i++)
		{
			float flows = 0;
			vector<pair<float, pair<int, int> > > OoO;
			for (int j = 0; j < Task; j++)
			{
				int round = rand() % PathSets[j].num;
				int k = 0;
				while (PathSets[j].Pathset[round][k]>0)k++;
				float value = demand[j] / pow(k, 0.5);
				OoO.push_back(make_pair(value, make_pair(j, round)));
			}
			sort(OoO.begin(), OoO.end(), randcmp);
			vector<int> tmpc(G.m, 0);
			for (int j = 0; j < Task; j++)
			{
				int OjO = OoO[j].second.first;
				int gene = OoO[j].second.second;
				int k = 0;
				int flag = 0;
				while (true)
				{
					int edge = PathSets[OjO].Pathset[gene][k];
					if (edge < 0)
						break;
					if (tmpc[edge] + demand[OjO]>capacity[edge])
					{
						flag = 1;
						break;
					}
					k++;
				}
				if (flag == 0)
				{
					int k = 0;
					while (true)
					{
						int edge = PathSets[OjO].Pathset[gene][k];
						if (edge < 0)
							break;
						tmpc[edge] += demand[OjO];
						k++;
					}
					chormes[i*Task + OjO] = gene;
					flows += demand[OjO];

				}
				else
				{
					chormes[i*Task + OjO] = -1;
				}
			}
			Fits.push_back(FitVMP(flows, i));
		}
		//for(int i=0;i<Fits.size();i++)
			//cout<<Fits[i].value<<" "<<Fits[i].mark<<endl;
		sort(Fits.begin(), Fits.end(), FitCmp());
		/*for (int i = 0; i < pop; i++)
			CuFits[i] = Fits[i];
		cout << Fits[0].value << endl;*/
		Check(chormes+Fits[0].mark*Task);
		cout<<"out check "<<endl;
	};
	void parrelmake();
	void cudapre();
	void cudamalloc();
	void process();
	void cudafree();
	void cudpppre();
	void Check(int*co){
		vector<int> tmpc(G.m, 0);
		float dd = 0;
		for (int j = 0; j < Task; j++)
		{
			int k = 0;
			if (co[j] >= 0){
				dd += demand[j];
				while (true)
				{
					int edge = PathSets[j].Pathset[co[j]][k];
					if (edge < 0)
						break;
					if (tmpc[edge] + demand[j]>capacity[edge])
					{
						cout << "erro!!!" << endl;
						break;
					}
					tmpc[edge] += demand[j];
					k++;
				}
			}
		}
		cout << "check over!!tof: " << endl;
	}
	pair<float,int> more(){
				float totalv = fit_value[0];
				cout<<"before more"<<fit_value[0]<<endl;
				int bestchoice = fit_key[0];
				vector<int>remain;
				float tf=0;
				totalweight=0;
				vector<pair<float,int>>answers;
				for (int i = 0; i < Task; i++)
				{
					int route = chormes[bestchoice*Task+i];
					//cout<<route<<" ";
					if (route < 0)
						remain.push_back(i);
					else{
						vector<int> path;
						int k = 0;
						while (true){
							int svalue = PathSets[i].Pathset[route][k];
							if (svalue < 0)
								break;
							path.push_back(svalue);
							capacity[svalue] -= demand[i];
							k++;
						}
						reverse(path.begin(),path.end());
						Result.push_back(make_pair(i,path));
						totalweight+=k;
						tf+=demand[i];
						answers.push_back(make_pair(demand[i],k));
					}
				}
				int stillremain=0;
				for (int i = 0; i < remain.size(); i++)
				{
					BFS(&G, st[remain[i]], te[remain[i]], distca, caped, demand[remain[i]],capacity,100000);
					int f = caped[te[remain[i]]];
					if (distca[te[remain[i]]]<INFHOPS)
					{
						vector<int>path;
						totalv += demand[remain[i]]*(distca[te[remain[i]]]-INFHOPS);
						int j = 0;
						while (f >= 0)
						{
							j++;
							capacity[f]-=demand[remain[i]];
							path.push_back(f);
							f = caped[G.incL[f].tail];
						}
						Result.push_back(make_pair(remain[i],path));
						totalweight+=distca[te[remain[i]]];
						tf+=demand[remain[i]];
						answers.push_back(make_pair(demand[remain[i]],j));

					}
					else
						stillremain++;
				}
				//writejsondanswer(answers,string("GA_Parallel"));
				cout<<Result.size()<<" "<<answers.size()<<endl;
				best=totalv;
				cout<<"after totalv"<<totalv;
				return make_pair(tf,Task-stillremain);
	}
};
#endif