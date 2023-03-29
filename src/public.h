// For All Modules

// Not sure about the maximum of the variables below. You can modify them and update the modified file.
const int MAX_OPS = 100;
const int MAX_PROCS = 100;
const int MAX_ROW = 20;
const int MAX_COL = 20;
const int MAX_LIT = 400;
const int MAX_PATH_LEN = 200;
const int MAX_OP_LEN = 10;

int highest = 0;

struct Position {
  int x, y;
};
// Direction : 0-UP, 1-LEFT, 2-DOWN, 3-RIGHT
const int dx[4] = {0, -1, 0, 1};
const int dy[4] = {-1, 0, 1, 0};
struct Robot {
  Position pos;
  int dir;
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
enum OyType {
  TL, TR, MOV, JMP, LIT, CALL
};
struct Proc {
  OyType ops[MAX_OPS];
  int is_CALL[MAX_OPS];
  int count;
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
  int num_procs;
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

Game game;

// Module 1
void save(const char * path);
void auto_save();

// Module 2
enum ResultType {
  LIGHT,
  LIMIT,
  DARK
};
struct Result {
  int steps;
  ResultType result;
};
Result robot_run(const char * path);

// You can add any comments in this file and update it in the WeChat group.