//============================================================================
// Name        : RandomPlanner.cpp
// Author      : Nikhil Nagori
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

/*
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
*/

int neighbour[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

typedef pair<int, int> Pair;

int max_step_number;
int ROW, COL;

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
	for(int i=0; i<path.size(); i++){
		cout<<"\n("<<path[i].first<<","<<path[i].second<<")";
	}
}

void search(int **world, Pair robot_pose, Pair goal_pose){
	vector<Pair> path;
	vector<Pair> prev_visited;
	int step_number = max_step_number;
	int allowed_prev_visited = sqrt(step_number);
	Pair curr_pose = robot_pose;

	path.push_back(robot_pose);

	while(!isGoal(curr_pose,goal_pose) && step_number > 0){
		int new_row, new_col;
		vector<Pair> move;
		for(int i=0; i<4; i++)
			move.push_back(make_pair(neighbour[i][0],neighbour[i][1]));
		int number_move_available = move.size();
		bool flag_already_visited = false;
		bool flag_search_new_state = true;
		vector<Pair> neighbour_visited;

		while(flag_search_new_state && number_move_available > 0){
			int next_step = rand() % number_move_available--;

			new_row = curr_pose.first + move[next_step].first;
			new_col = curr_pose.second + move[next_step].second;

			move.erase(move.begin()+next_step);

			if(isValid(new_row, new_col)){
				if(!isObstacle(world, new_row, new_col)){
					for(int i=0; i<prev_visited.size(); i++){
						if(new_row == prev_visited[i].first && new_col == prev_visited[i].second){
							neighbour_visited.push_back(make_pair(new_row,new_col));
							flag_already_visited = true;
							break;
						}
					}

					if(!flag_already_visited){
						curr_pose = make_pair(new_row,new_col);
						prev_visited.push_back(curr_pose);
						flag_search_new_state = false;
					} else {
						flag_already_visited = false;
					}
				}
			}
		}

		if(flag_search_new_state){
			if(neighbour_visited.size() != 0){
				int rnd = rand() % neighbour_visited.size();
				curr_pose = neighbour_visited[rnd];
				prev_visited.push_back(curr_pose);
			} else {
				cout<< "\nFAIL No place to go";
				break;
			}
		}

		path.push_back(curr_pose);

		if(prev_visited.size() > allowed_prev_visited){
			prev_visited.erase(prev_visited.begin(), prev_visited.begin() + (prev_visited.size() - allowed_prev_visited));
		}
		step_number--;
	}

	if(isGoal(curr_pose, goal_pose)){
		cout<<"SUCCESS";
		cout<<"\nPath:";
		displayPath(path);
	} else {
		cout<<"FAIL Out of max_number_steps";
		displayPath(path);

	}

}

int main() {

	int **world;
	int robot_row, robot_col, goal_row, goal_col;
	ifstream inputFile;

//Change the file path according to your system
	inputFile.open("C:/Users/dell/eclipse-workspace/RandomPlanner/src/input1.txt");
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
		inputFile >> max_step_number;
		inputFile.close();
	} else {
		cout<<"File did not open correctly";
	}

	search(world, make_pair(robot_row,robot_col), make_pair(goal_row, goal_col));

	return 0;
}
