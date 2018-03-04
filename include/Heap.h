#include"Edge.h"
#include<vector>
#define NN 10000
using namespace std; 
class Heap
{
public:
	Heap(int n);
	~Heap();
	void push(int vertID, int w);
	void update(int vertID, int w);
	int pop();
	int empty();
private:
	vector<Edge> h;
	vector<int> post;
	int nodeNum;
	int COUNT;
	int NUM;
	void fix(int fixID);
};


