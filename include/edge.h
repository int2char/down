//
// Created by root on 17-5-9.
//
#ifndef CSPALLOC_EDGE_H
#define CSPALLOC_EDGE_H
#include<bits/stdc++.h>
using namespace std;
struct edge{
    int s,t,w;
    edge(int _s=0,int _t=0,int _w=0):s(_s),t(_t),w(_w){};
};
struct ginfo
{
	int pesize,eesize,pnodesize,enodesize;
	vector<int>&exn2n;
	ginfo(int pe,int ee,int pn,int en,vector<int>&ew):eesize(pe),pesize(ee),pnodesize(pn),enodesize(en),exn2n(ew){};
};
struct epair
{
	int f,t;
};
#endif //CSPALLOC_EDGE_H
