#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include"pathalg.h"
static const int WORK_SIZE =258;
void Bellmanor::copydata(int s,vector<edge>&edges,int nodenum){
};
void Bellmanor::dellocate(){
};
void Bellmanor::allocate(int maxn,int maxedge){
}
void Bellmanor::topsort()
{
};
void Bellmanor::updatE(vector<vector<int>>&tesigns)
{
	esigns=tesigns;
	for(int k=0;k<LY;k++)
		{
			int off=k*nodenum*mm;
			for(int i=0;i<nodenum;i++)
			{
				for(int j=0;j<mm;j++)
					if(j<rus[i].size())
						rudw[off+i*mm+j]=esigns[k][ruw[i][j]];
					else
						rudw[off+i*mm+j]=-1;
			}
		}
	cudaMemcpy(dev_rudw,rudw,mm*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
}
void Bellmanor::updatS(vector<vector<Sot>>&stpair)
{
	L[0]=0;
	L[1]=LY1;
	L[2]=LY2;
	S[0]=stpair[0].size();
	S[1]=stpair[1].size();
	stps=stpair;
	int count=0;
	ncount=L[1]*S[0]+L[2]*S[1];
	memset(d,1,ncount*nodenum*sizeof(int));
	memset(p,-1,ncount*nodenum*sizeof(int));
	for(int k=0;k<L[1];k++)
		{
		for(int j=0;j<stpair[0].size();j++)
			{
			 d[count*nodenum+stpair[0][j].s*NUT]=0;
			 count++;
			}
		}
	for(int k=0;k<L[2];k++)
		{
		for(int j=0;j<stpair[1].size();j++)
			{
			 d[count*nodenum+stpair[1][j].s*NUT]=0;
			 count++;
			}
		}
	Size[0]=nodenum*L[1]*S[0];
	Size[1]=nodenum*L[2]*S[1];
	cudaMemcpy(dev_d,d,ncount*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_p,p,ncount*nodenum*sizeof(int),cudaMemcpyHostToDevice);
}
void Bellmanor::init(pair<vector<edge>,vector<vector<int>>>ext,vector<pair<int,int>>stpair,int _nodenum)
{
	//cout<<"init bellmanor"<<endl;
	nodenum=_nodenum;
	edges=ext.first;
	esigns=ext.second;
	stp=stpair;
	W=WD+1;
	//st=new int[edges.size()*LY];
	//te=new int[edges.size()*LY];
	d=new int[nodenum*LY*YE];
	has=new int[nodenum*LY*YE];
	p=new int[nodenum*LY*YE];
	w=new int[edges.size()*LY];
	m1=new int;
	m2=new int;
	*m1=0,*m2=0;
	esignes=new int[edges.size()*LY];
	vector<vector<int>>nein(nodenum*LY,vector<int>());
	neibn=nein;
	vector<vector<int>>neie(nodenum,vector<int>());
	vector<vector<int>>rs(nodenum,vector<int>());
	vector<vector<int>>rw(nodenum,vector<int>());
	rus=rs;
	ruw=rw;
	for(int i=0;i<edges.size();i++)
		{
			int s=edges[i].s;
			int t=edges[i].t;
			rus[t].push_back(s);
			ruw[t].push_back(i);
			neibn[s].push_back(t);
			neie[s].push_back(i);
		}
	mm=0;
	for(int i=0;i<rus.size();i++)
		if(rus[i].size()>mm)mm=rus[i].size();
	rudu=new int[nodenum*mm*LY];
	rudw=new int[nodenum*mm*LY];
	rid=new int[nodenum*mm*LY];
	for(int k=0;k<LY;k++)
		{
		int off=k*nodenum*mm;
		for(int i=0;i<nodenum;i++)
		{
			for(int j=0;j<mm;j++)
				if(j<rus[i].size())
					rudu[off+i*mm+j]=rus[i][j];
				else
					rudu[off+i*mm+j]=INT_MAX;
			for(int j=0;j<mm;j++)
				if(j<rus[i].size())
					{	
						rudw[off+i*mm+j]=esigns[k][ruw[i][j]];
						rid[off+i*mm+j]=ruw[i][j];
					}
				else
					{
						rudw[off+i*mm+j]=-1;
						rid[off+i*mm+j]=-1;
					}
		}
		}
	int count=0;
	/*for(int k=0;k<LY;k++)
		for(int i=0;i<nodenum;i++)
			for(int j=0;j<neibn[i].size();j++)
			{
				st[count]=i;
				if(esigns[k][neie[i][j]]<0)
					te[count]=i;
				else
					te[count]=neibn[i][j];
				count++;
			}*/
	int cc=0;
	for(int k=0;k<LY;k++)
		for(int i=0;i<edges.size();i++)
			w[cc++]=esigns[k][i];
	//cudaMalloc((void**)&dev_st,LY*edges.size()*sizeof(int));
	//cudaMalloc((void**)&dev_te,LY*edges.size()*sizeof(int));
	cudaMalloc((void**)&dev_d,YE*LY*nodenum*sizeof(int));
	cudaMalloc((void**)&dev_p,YE*LY*nodenum*sizeof(int));
	cudaMalloc((void**)&dev_w,LY*edges.size()*sizeof(int));
	//cudaMalloc((void**)&dev_m1,sizeof(int));
	//cudaMalloc((void**)&dev_m2,sizeof(int));
	cudaMalloc((void**)&dev_rudu,mm*LY*nodenum*sizeof(int));
	cudaMalloc((void**)&dev_rudw,mm*LY*nodenum*sizeof(int));
	cudaMalloc((void**)&dev_rid,mm*LY*nodenum*sizeof(int));
	//cudaMalloc((void**)&dev_ruid,mm*LY*nodenum*sizeof(int));
	//cudaMemcpy(dev_te,te,LY*edges.size()*sizeof(int),cudaMemcpyHostToDevice);
	//cudaMemcpy(dev_st,st,LY*edges.size()*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_w,w,LY*edges.size()*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_rudu,rudu,mm*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_rudw,rudw,mm*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_rid,rid,mm*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	//cudaMemcpy(dev_ruid,ruid,mm*LY*nodenum*sizeof(int),cudaMemcpyHostToDevice);
	//cudaMemcpy(dev_m1,m1,sizeof(int),cudaMemcpyHostToDevice);
	//cudaMemcpy(dev_m2,m2,sizeof(int),cudaMemcpyHostToDevice);
};
Bellmanor::Bellmanor():L(PC+1,0),S(PC,0),NF(PC,0),Size(2,0)
{
};
__global__ void bellmandu(int *rudu,int*rudw,int *d,int*p,int N,int size,int sizeoff,int leveloff,int ye,int ly,int mm)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if(i>=size)return;
	int lyy=i/(ye*N);
	int yee=(i%(ye*N))/N;
	int off=lyy*N*ye+yee*N+sizeoff;
	int roff=(i%N+(lyy+leveloff)*N)*mm;
	i+=sizeoff;
	int dm=d[i];
	for(int k=0;k<mm;k++)
		if(rudu[roff+k]<INT_MAX)
		{
			int node=rudu[roff+k]+off;
			if(rudw[roff+k]<0)continue;
			if(dm>d[node]+rudw[roff+k])
				dm=d[node]+rudw[roff+k];
		}
	if(d[i]>dm)
		d[i]=dm;
}
__global__ void bellmancolor(int *rudu,int*rudw,int*rid,int *d,int*p,int N,int size,int sizeoff,int leveloff,int ye,int ly,int mm)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if(i>=size)return;
	int lyy=i/(ye*N);
	int yee=(i%(ye*N))/N;
	int off=lyy*N*ye+yee*N+sizeoff;
	int roff=(i%N+(lyy+leveloff)*N)*mm;
	i+=sizeoff;
	int dm=d[i];
	int mark=-1;
	for(int k=0;k<mm;k++)
		if(rudu[roff+k]<INT_MAX)
		{
			int node=rudu[roff+k]+off;
			if(rudw[roff+k]<0)continue;
			if(dm==d[node]+rudw[roff+k])
				{mark=rid[roff+k];break;}
		}
	p[i]=mark;
}
vector<vector<Rout>> Bellmanor::routalg(int s,int t,int bw)
{
	//cout<<"inbellman"<<endl;
	int kk=1;
	time_t start,end;
	start=clock();
	cudaStream_t stream0;
	cudaStreamCreate(&stream0);
	cudaStream_t stream1;
	cudaStreamCreate(&stream1);
	for(int i=0;i<WD+1;i++)
	{
		bellmandu<<<Size[0]/512+1,512,0>>>(dev_rudu,dev_rudw,dev_d,dev_p,nodenum,Size[0],0,0,S[0],L[1],mm);
		bellmandu<<<Size[1]/512+1,512,0>>>(dev_rudu,dev_rudw,dev_d,dev_p,nodenum,Size[1],Size[0],L[1],S[1],L[2],mm);
	}
	bellmancolor<<<Size[0]/512+1,512,0>>>(dev_rudu,dev_rudw,dev_rid,dev_d,dev_p,nodenum,Size[0],0,0,S[0],L[1],mm);
	bellmancolor<<<Size[1]/512+1,512,0>>>(dev_rudu,dev_rudw,dev_rid,dev_d,dev_p,nodenum,Size[1],Size[0],L[1],S[1],L[2],mm);
	cudaStreamSynchronize(stream1);
	cudaStreamSynchronize(stream0);
	cudaMemcpy(d,dev_d,ncount*nodenum*sizeof(int),cudaMemcpyDeviceToHost);
	cudaMemcpy(p,dev_p,ncount*nodenum*sizeof(int),cudaMemcpyDeviceToHost);
	end=clock();
	vector<vector<Rout>>result(2,vector<Rout>());
	vector<int>LL(3,0);
	LL=L;
	LL[2]+=LL[1];
	int count=0;
	for(int y=1;y<PC+1;y++)
		for(int k=LL[y-1];k<LL[y];k++)
		{	
			for(int l=0;l<stps[y-1].size();l++)
			{	
				int offf=count*nodenum;
				int s=stps[y-1][l].s*NUT;
				vector<int>ters=stps[y-1][l].ters;
				for(int i=0;i<ters.size();i++)
				{
					int id=stps[y-1][l].mmpid[ters[i]];
					int hop=0;
					int tt=ters[i];
					int min=INF;
					int prn=-1;
					for(int i=1;i<W;i++)
						{
						if(d[offf+tt*W+i]<min)
							{	
								min=d[offf+tt*W+i];
								prn=offf+tt*W+i;
							}
						}
					int node=prn-offf;
					if(prn<0)continue;
					Rout S(s,node,id,min,offf,k);
					result[y-1].push_back(S);
				}
				count++;
			}
		}
	//cout<<"GPU time is : "<<end-start<<endl;
	return result;
};

/*
__global__ void bellmanhigh(int *st,int *te,int *d,int *has,int *w,int E,int N,int size,int *m,int round,int Leveloff,int numoff,int ye,int ly)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if(i>size)return;	
	int eid=(i%(E*ly));
	int eeid=eid+Leveloff;
	int s=st[eeid],t=te[eeid],weight=w[eeid];
	if(weight<0)return;
	int off=(i/(E*ly))*N+(eid/E)*N*ye+numoff;
	//if(has[s+off]<round-1)return;
	if(d[s+off]+weight<d[t+off])  
		{
			d[t+off]=weight+d[s+off];
			//has[t+off]=round;
			*m=1;
		}
}*/
/*__global__ void color(int *st,int *te,int *d,int *pre,int *has,int *w,int E,int N,int size,int round,int Leveloff,int numoff,int ye,int ly)
{
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if(i>size)return;	
	int eid=(i%(E*ly));
	int eeid=eid+Leveloff;
	int s=st[eeid],t=te[eeid],weight=w[eeid];
	if(weight<0)return;
	int off=(i/(E*ly))*N+(eid/E)*N*ye+numoff;
	//if(has[s+off]<round-1)return;
	if(d[s+off]+weight==d[t+off])
		pre[t+off]=s+off;
}*/
/*m1=1;
	*m2=1;
	int round=1;
	cout<<"fuck wx!"<<endl;
	int flag1=0,flag2=0;
	int cc=0;
	while(*m2==1||*m1==1)
	{
		*m2=0,*m1=0;
		cudaMemcpyAsync(dev_m2,m2,sizeof(int),cudaMemcpyHostToDevice,stream1);
		bellmanhigh<<<size[1]/1024+1,1024,0,stream1>>>(dev_st,dev_te,dev_d,dev_has,dev_w,edges.size(),nodenum,size[1],dev_m2,round,leveloff[1],nodeoff[1],S[1],L[1]);
		cudaMemcpyAsync(dev_m1,m1,sizeof(int),cudaMemcpyHostToDevice,stream0);
		bellmanhigh<<<size[0]/1024+1,1024,0,stream0>>>(dev_st,dev_te,dev_d,dev_has,dev_w,edges.size(),nodenum,size[0],dev_m2,round,leveloff[0],nodeoff[0],S[0],L[0]);
		color<<<size[1]/1024+1,1024,0,stream1>>>(dev_st,dev_te,dev_d,dev_p,dev_has,dev_w,edges.size(),nodenum,size[1],round,leveloff[1],nodeoff[1],S[1],L[1]);
		cudaMemcpyAsync(m2,dev_m2,sizeof(int),cudaMemcpyDeviceToHost,stream1);
		color<<<size[0]/1024+1,1024,0,stream0>>>(dev_st,dev_te,dev_d,dev_p,dev_has,dev_w,edges.size(),nodenum,size[0],round,leveloff[0],nodeoff[0],S[0],L[0]);
		cudaMemcpyAsync(m1,dev_m1,sizeof(int),cudaMemcpyDeviceToHost,stream0);
		cudaStreamSynchronize(stream1);
		cudaStreamSynchronize(stream0);
	}*/