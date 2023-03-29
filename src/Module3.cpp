#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include "Module2.cpp"
using namespace std;
/*
#define MAX_PATH_LEN 100  

#define MAX_ROW 9  
#define MAX_COL 4  
#define MAX_OPS 10
#define MAX_LIT 10 
#define MAX_PROCS 10 

struct Position {
	int x, y; 
};
enum Direction {
	UP, DOWN, LEFT, RIGHT
};
struct Robot {
	Position pos; 
	Direction dir; 
};
struct Light {
	Position pos; 
	bool lighten; 
};
struct Cell {
	int height; 
	int light_id; 
	bool robot; 
};
enum OpType {
	TL, TR, MOV, JMP, LIT, CALL
};
struct Proc {
	OpType ops[MAX_OPS]; 
	int count; 
};
enum ResultType {
	LIGHT, 
	LIMIT,
	DARK 
};
struct Result {
	int steps; 
	ResultType result;
};

struct OpSeq {
	Proc procs[MAX_PROCS];
	int count; 
};
struct Map {
	Cell cells[MAX_ROW][MAX_COL];
	int row, col;
	Light lights[MAX_LIT];
	int num_lights; 
	int  num_procs; 
	Robot robot;
	int op_limit[MAX_PROCS];
};
struct Game {
	char map_name[MAX_PATH_LEN]; 
	Map map_init; 
	Map map_run; 
	char save_path[MAX_PATH_LEN];
	int auto_save_id; 
	int limit; 
};
static Game game; 
*/
bool loadMap(char * path);
bool autoSave(char * path);
void stepLimit(char * path);
bool statusDisp();
bool newOp(char * path);
bool runOp(char * path);
bool exitGame();
bool distributeTask(); 

int main() {
	game.limit = 100;
	while (distributeTask());
	return 0;
}
Map none_map;
bool loadMap(char * path) {
	string line; int num = 0; 
	int map[MAX_ROW + 3 + MAX_LIT][MAX_PROCS] = { 0 };
	ifstream newMap(path);
	if (!newMap.is_open())  
	{
		cout << "无法打开地图文件" << endl;
		game.map_init = none_map;
		memset(game.map_name, '\0', sizeof game.map_name);
		game.limit = 100;
		highest = 0;
		return 0;
	}
	for (int i = 0; i < MAX_ROW + 3 + MAX_LIT; i++)
	{
		getline(newMap, line); 
		stringstream ssin(line);
		int j = 0;
		while (ssin >> num) {
			map[i][j] = num; j++;
		}
	}
	newMap.close();
	Map mapNew;
	mapNew.row = map[0][0];
	mapNew.col = map[0][1];
	mapNew.num_lights = map[0][2];
	mapNew.num_procs = map[0][3];
	for (int i = 0; i < mapNew.row; i++) {  
		for (int j = 0; j < mapNew.col; j++) {
			mapNew.cells[j][i].height = map[i + 1][j];
			highest = max(highest, map[i + 1][j]);
			mapNew.cells[j][i].light_id = -1;
			mapNew.cells[j][i].robot = false;
		}
	}
	for (int i = 0; i < mapNew.num_lights; i++) {  
		mapNew.lights[i].pos.x = map[1 + mapNew.row + i][0];
		mapNew.lights[i].pos.y = map[1 + mapNew.row + i][1];
		mapNew.lights[i].lighten = false;
		mapNew.cells[mapNew.lights[i].pos.x][mapNew.lights[i].pos.y].light_id = i; 
	}
	for (int i = 0; i < mapNew.num_procs; i++) { 
		mapNew.op_limit[i] = map[1 + mapNew.row + mapNew.num_lights][i];
	}
	mapNew.robot.pos.x = map[1 + mapNew.row + mapNew.num_lights + 1][0];
	mapNew.robot.pos.y = map[1 + mapNew.row + mapNew.num_lights + 1][1];
	mapNew.cells[mapNew.robot.pos.x][mapNew.robot.pos.y].robot = true;
	mapNew.robot.dir = map[1 + mapNew.row + mapNew.num_lights + 1][2];
	if (mapNew.robot.dir == 1 || mapNew.robot.dir == 2) mapNew.robot.dir = 3 - mapNew.robot.dir;
	memset(game.map_name, '\0', sizeof(game.map_name));
	strcpy(game.map_name, path);
	cout << game.map_name << endl;
	game.map_init = mapNew;
	cout << "loadMap() 执行成功" << endl;
	return true;
}

bool autoSave(char * path) {
	memset(game.save_path, '\0', sizeof(game.save_path));
	if (!strcmp(path, "OFF")) { 
		cout << "关闭自动保存" << endl;
	}
	else {
		strcpy(game.save_path, path);
		cout << "开启自动保存" << endl;
	}
	return true;
}

void stepLimit(char * path) {
	game.limit = atoi(path);
	cout << "stepLimit() 执行" << endl;
}

bool statusDisp() {
	char pre[] = "\033["; char post[] = "\033[0m";
	char noNeedLight[] = ";100;1m"; 
	char toLight[] = ";104;1m"; 
	char lighting[] = ";103;1m"; 
	char robotOn[] = "91"; 
	char robotOff[] = "92";

	cout << "Map Name: " << game.map_name << endl;
	cout << "Autosave: " << game.save_path << endl;
	cout << "Step Limit: " << game.limit << endl;
	for (int i = 0; i < game.map_init.row; i++) { 
		char out[1000] = "\033[";
		for (int j = 0; j < game.map_init.col; j++) {
			if (game.map_init.cells[j][i].height == 0) {
				strcat(out, "0m \033[");
				continue;
			}
			if (game.map_init.cells[j][i].robot) strcat(out, robotOn);
			else strcat(out, robotOff);
			if (game.map_init.cells[j][i].light_id != -1) {
				if (game.map_init.lights[game.map_init.cells[j][i].light_id].lighten) strcat(out, lighting);
				else strcat(out, toLight);
			}
			else strcat(out, noNeedLight);
			char num[10]; _itoa(game.map_init.cells[j][i].height, num, 10);
			strcat(out, num);
			strcat(out, pre);
		}
		strcat(out, post);
		cout << out << endl;
	}
	switch (game.map_init.robot.dir) {
	case 0:
		cout << "Robot is facing up" << endl;
		break;
	case 1:
		cout << "Robot is facing left" << endl; break;
	case 2:
		cout << "Robot is facing down" << endl; break;
	case 3:
		cout << "Robot is facing right" << endl; break;
	}
	cout << "Proc Limit: [";
	for (int i = 0; i < game.map_init.num_procs; i++) {
		cout << game.map_init.op_limit[i];
		if (i != game.map_init.num_procs - 1) cout << ',';
	}
	cout << "]" << endl;
	return true;
}
bool newOp(char * path) {
	ofstream out; char order[MAX_OP_LEN];
	out.open(path, ios::out | ios::trunc);       
	int n = 0; cin >> n; if (n > game.map_init.num_procs) { cout << "过程数量过多" << endl; return 0; }
	out << n << endl;
	for (int i = 0; i < n; i++)
	{
		int N;
		cin >> N; if (N > game.map_init.op_limit[i]) cout << "指令数量超出上限" << endl;
		out << N << ' ';
		for (int j = 0; j < N; j++) { cin >> order; out << order << ' '; }
		out << endl;
	};
	out.close();
	cout << "newOp() 执行" << endl;
	return true;
}
bool runOp(char * path) {
	ifstream OP(path);
	if (!OP.is_open())  
	{
		cout << "无法打开指令文件" << endl;
		return 0;
	}
	
	char pre[] = "\033["; char post[] = "\033[0m";
	char noNeedLight[] = ";100;1m"; 
	char toLight[] = ";104;1m"; 
	char lighting[] = ";103;1m"; 
	char robotOn[] = "91"; 
	char robotOff[] = "92"; 
	Result result = robot_run(path);
	OP.close();
	cout << "Run " << path << ',' << "result: ";
	switch (result.result) {
	case 0: cout << "LIGHT" << endl; break;
	case 1: cout << "LIMIT" << endl; break;
	case 2: cout << "DARK" << endl; break;
	}
	cout << "step(s) used: " << result.steps << endl;;
	for (int i = 0; i < game.map_run.row; i++) { 
		char out[1000] = "\033[";
		for (int j = 0; j < game.map_run.col; j++) {
			if (game.map_run.cells[j][i].height == 0) {
				strcat(out, "0m \033[");
				continue;
			}
			if (game.map_run.cells[j][i].robot) strcat(out, robotOn);
			else strcat(out, robotOff);
			if (game.map_run.cells[j][i].light_id != -1) {
				if (game.map_run.lights[game.map_run.cells[j][i].light_id].lighten) strcat(out, lighting);
				else strcat(out, toLight);
			}
			else strcat(out, noNeedLight);
			char num[10]; _itoa(game.map_run.cells[j][i].height, num, 10);
			strcat(out, num);
			strcat(out, pre);
		}
		strcat(out, post);
		cout << out << endl;
	}
	switch (game.map_run.robot.dir) {
	case 0:
		cout << "Robot is facing up" << endl;
		break;
	case 1:
		cout << "Robot is facing left" << endl; break;
	case 2:
		cout << "Robot is facing down" << endl; break;
	case 3:
		cout << "Robot is facing right" << endl; break;
	}
	cout << "runOp() 执行" << endl;
	return true;
}

bool exitGame() {
	cout << "执行退出程序" << endl;
	return true;
}

bool distributeTask() {
	char OP[MAX_OP_LEN];
	char str[MAX_PATH_LEN]; 
	cin >> OP;
	if (!strcmp(OP, "STATUS")) statusDisp();
	else if (!strcmp(OP, "EXIT")) { exitGame(); return false; }
	else { 
		cin >> str;
		if (!strcmp(OP, "LOAD")) loadMap(str);
		else if (!strcmp(OP, "AUTOSAVE")) autoSave(str);
		else if (!strcmp(OP, "LIMIT")) stepLimit(str);
		else if (!strcmp(OP, "OP")) newOp(str);
		else if (!strcmp(OP, "RUN")) runOp(str);
	}
	return true;
}
