//============================================================================
// Name        : OptimalPlanner.cpp
// Author      : Nikhil Nagori
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

/*
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
*/

typedef pair<int, int> Pair;
typedef pair<int, Pair> Pair_Pair;

struct cell{
	int f =-1, g=-1, h=-1;
	Pair parent = make_pair(-1,-1);
};

int ROW, COL;
int neighbour[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};


bool isObstacle(int **world, int row, int col){
	return (world[row][col] == 1);
}

bool isValid(int row, int col){
	return (row>=0) && (row<ROW) && (col>=0) && (col<COL);
}

bool isGoal(Pair curr, Pair dest){
	if (curr == dest)
	        return (true);
	    else
	        return (false);
}

void displayPath(vector<Pair> path){
	for(int i=path.size()-1; i>=0; i--){
		cout<<"("<<path[i].first<<","<<path[i].second<<")\t";
	}
}

//Heuristic used here is Manhattan Distance
int getHeuristic(int row, int col, Pair goal){
	return abs(row - goal.first) + abs(col - goal.second);
}

void search(int **world, Pair robot_pose, Pair goal_pose){

	int closed[ROW][COL] = {0};
	set<Pair_Pair> open;
	cell world_details[ROW][COL];

	bool goal_reached = false;

	int new_row, new_col;

	//Setting the details of robot pose
	new_row = robot_pose.first;
	new_col = robot_pose.second;
	world_details[new_row][new_col].parent = make_pair(new_row,new_col);
	world_details[new_row][new_col].f = 0;
	world_details[new_row][new_col].g = 0;
	world_details[new_row][new_col].h = 0;

	open.insert(make_pair(0, make_pair(new_row,new_col)));

	while(!open.empty() && !goal_reached){

		//Choosing the next best pose and setting it as current pose
		Pair curr_pose = (*open.begin()).second;
		int g_new, h_new, f_new;
		closed[curr_pose.first][curr_pose.second] = true;

		open.erase(open.begin());

		//Finding the neighbors of the current pose and adding to open list
		for(int i=0; i<4; i++){
			new_row = curr_pose.first + neighbour[i][0];
			new_col = curr_pose.second + neighbour[i][1];

			if(isValid(new_row, new_col)){
				if(isGoal(make_pair(new_row, new_col), goal_pose)){
					world_details[new_row][new_col].parent = curr_pose;
					goal_reached = true;
					break;
				}
				if(!isObstacle(world, new_row, new_col) && closed[new_row][new_col]==0){

					g_new = world_details[curr_pose.first][curr_pose.second].g + 1;
					h_new = getHeuristic(new_row, new_col, goal_pose);
					f_new = g_new + h_new;

					//Adding the new state to open list only if it is not present or
					//If it is already present then updating the cell details if the path it is reached from is smaller than previous path
					if(world_details[new_row][new_col].f==-1 || world_details[new_row][new_col].f > f_new){
						open.insert(make_pair(f_new, make_pair(new_row, new_col)));
						world_details[new_row][new_col].g = g_new;
						world_details[new_row][new_col].h = h_new;
						world_details[new_row][new_col].f = f_new;
						world_details[new_row][new_col].parent = curr_pose;
					}
				}
			}
		}
	}

	if(goal_reached){
		cout<<"SUCCESS";
		cout<<"\nPath:\t";
		vector<Pair> path;
		Pair parent = goal_pose;
		while(!(world_details[parent.first][parent.second].parent == parent)){
			path.push_back(parent);
			parent = world_details[parent.first][parent.second].parent;
		}
		path.push_back(robot_pose);
		displayPath(path);
	} else{
		cout<<"FAIL";
	}

}

int main() {

	int **world;
	int robot_row, robot_col, goal_row, goal_col;
	ifstream inputFile;

//Change the file path according to your system
	inputFile.open("C:/Users/dell/eclipse-workspace/OptimalPlanner/src/input1.txt");
	if(inputFile.is_open()){
		inputFile >> ROW;
		inputFile >> COL;
		world = new int *[ROW];
		for(int i=0; i<ROW; i++){
			world[i] = new int[COL];
			for(int j=0; j<COL; j++)
				inputFile >> world[i][j];
		}
		inputFile >> robot_row;
		inputFile >> robot_col;
		inputFile >> goal_row;
		inputFile >> goal_col;
		inputFile.close();
	} else {
		cout<<"File did not open correctly";
	}

	search(world, make_pair(robot_row,robot_col), make_pair(goal_row, goal_col));
	return 0;
}
