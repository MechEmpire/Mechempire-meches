#include "SimpleMeshNavGraph.h"
#include <queue>

using namespace std;

SimpleMeshNavGraph::SimpleMeshNavGraph()
	:_path(),_current_position(_path.begin())
{
	_graph = NULL;
}

SimpleMeshNavGraph::~SimpleMeshNavGraph()
{
	deleteGraph();
}

void SimpleMeshNavGraph::clearPath()
{
	_path.clear();
	_current_position = _path.begin();
}

void SimpleMeshNavGraph::searchPath(double sx,double sy,double ex,double ey)
{
	//A star


	//init closed and open node list
	bool ** closed;
	closed = new bool*[_row];
	for (int i = 0; i < _row; i++)
	{
		closed[i] = new bool[_column];
		for (int j = 0; j < _column; j++)
		{
			closed[i][j] = false;
		}
	}

	queue<NodePair> open;


	//find the closest node according to the sx,sy
	NodePair start_node = findClosestNode(sx,sy);
	NodePair target_node = findClosestNode(ex,ey);

	open.push(start_node);
	
	
	NodePair cur(-1,-1);
	//main loop
	while(open.size() > 0)
	{
		cur = open.front();

		if(cur == target_node)
		{
			//success
			//TODO:build path
		}
		else
		{
			
		}
	}







	//delete closed
	for (int i = 0; i < _row; i++)
	{
		delete closed[i];
	}
}



bool SimpleMeshNavGraph::getNextPosition(Vector2D & position)
{
	//TODO
	return true;
}


void SimpleMeshNavGraph::deleteGraph()
{
	if(_graph != NULL)
	{
		for (int i = 0; i < _row; i++)
		{
			delete _graph[i];
		}
		delete _graph;
		_graph = NULL;
	}
}


SimpleMeshNavGraph::NodePair SimpleMeshNavGraph::findClosestNode(double x,double y)
{
	double ox = x - _x_offset;
	double oy = y - _y_offset;

	int xid = (int)(ox / _w_length + 0.5);
	int yid = (int)(oy / _h_length + 0.5);

	return SimpleMeshNavGraph::NodePair(xid,yid);
}

void SimpleMeshNavGraph::initGraph(double width,double height,double x_offset, double y_offset,int row,int column,VALID_FUNCTION vf,VALUE_FUNCTION cf)
{
	_width = width;
	_height = height;
	_row = row;
	_column = column;
	_x_offset = x_offset;
	_y_offset = y_offset;

	_w_length = _width / _column;
	_h_length = _height / _row;

	_d_length = sqrt(_w_length * _w_length + _h_length * _h_length);


	if(_graph != NULL)
	{
		deleteGraph();
	}


	_graph = new Node*[_row];
	for(int i = 0; i < _row; i++)
	{
		_graph[i] = new Node[_column];
		for (int j = 0; j < _column; j++)
		{
			Node * n = & _graph[i][j];
			
			n -> pos.x = _x_offset + _width * ((double)j)/((double)_column);
			n -> pos.y = _y_offset + _height * ((double)i)/((double)_row);

			n -> valid = (*vf)(n->pos.x , n->pos.y);
			n -> value = (*cf)(n->pos.x , n->pos.y);
		}
	}
}