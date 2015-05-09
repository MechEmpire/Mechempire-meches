#pragma once 

#include <list>
#include "NavGraph.h"

using namespace std;

struct Node
{
	Vector2D pos;

	bool valid;
	double value;

	

	Node(double x = 0,double y = 0,bool v = true)
		:pos(x,y)
	{
		valid = v;
		value = 0;
	}

	
};


/**
* 矩形的棋盘式简易导航图
* 每个点八个出口(不包括边界点)
*
*/
class SimpleMeshNavGraph
	: public NavGraph
{
public:
	typedef bool (*VALID_FUNCTION)(double x,double y);
	typedef double (*VALUE_FUNCTION)(double x,double y);

	typedef pair<int,int> NodePair;

	SimpleMeshNavGraph();
	~SimpleMeshNavGraph();

	
	virtual void searchPath(double sx,double sy,double ex,double ey);
	
	virtual bool getNextPosition(Vector2D & position);

	void clearPath();
	void deleteGraph();
	void initGraph(double width,double height,double x_offset, double y_offset,int row,int column,VALID_FUNCTION vf,VALUE_FUNCTION cf);

	NodePair findClosestNode(double x,double y);
protected:
	//graph
	Node ** _graph;

	int _row;
	int _column;

	double _width;
	double _height;

	double _w_length;	//边长
	double _h_length;
	double _d_length;	//diagonal

	double _x_offset;
	double _y_offset;

	//path
	list<Node*> _path;
	list<Node*>::iterator _current_position;
};