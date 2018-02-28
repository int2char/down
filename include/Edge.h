#ifndef __EDGE__
#define __EDGE__

struct Edge
{
public:
	int head, tail;
    float weight;
	float capacity;
	float backweight;
	Edge(){};
	Edge(int _head,float _weight,float _capacity=0,float _backweight=0):head(_head),tail(0),weight(_weight),capacity(_capacity),backweight(_backweight){
	};
private:

};

#endif
