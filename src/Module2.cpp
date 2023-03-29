#include <iostream>
#include <fstream>
#include "Module1.cpp"
using namespace std;
OpSeq nowseq;
int now_step;
ResultType now_situ;
void check() {
  int fg = 1;
  for (int i = 0; i < game.map_run.num_lights; ++i)
    if (!game.map_run.lights[i].lighten) {
      fg = 0;
      break;
    }
  if (fg) {
    now_situ = LIGHT;
    return;
  }
  if (now_step >= game.limit) {
    now_situ = LIMIT;
  }
}
void call_proc(int pid) {
  for (int i = 0; i < nowseq.procs[pid].count; ++i) {
    ++now_step;
    if (nowseq.procs[pid].ops[i] == CALL) {
      if (nowseq.procs[pid].is_CALL[i] >= nowseq.count || nowseq.procs[nowseq.procs[pid].is_CALL[i]].count == 0) {
        cout << "WARNING:INVALID_PROCS_CALL!" << endl;
        auto_save();
        check();
        if (now_situ != DARK) return;
      } else {
        auto_save();
        check();
        if (now_situ != DARK) return;
        call_proc(nowseq.procs[pid].is_CALL[i]);
        if (now_situ != DARK) return;
      }
    } else {
      if (nowseq.procs[pid].ops[i] == TL) {
        (game.map_run.robot.dir += 1) %= 4;
      } else if (nowseq.procs[pid].ops[i] == TR) {
        (game.map_run.robot.dir += 3) %= 4;
      } else if (nowseq.procs[pid].ops[i] == JMP) {
        int nextx = game.map_run.robot.pos.x + dx[game.map_run.robot.dir];
        int nexty = game.map_run.robot.pos.y + dy[game.map_run.robot.dir];
        if (nextx < 0 || nextx >= game.map_run.col || nexty < 0 || nexty >= game.map_run.row || abs(game.map_run.cells[nextx][nexty].height - game.map_run.cells[game.map_run.robot.pos.x][game.map_run.robot.pos.y].height) != 1) {
          cout << "WARNING:INVALID_JUMP_OPERATION!" << endl;
        } else {
          game.map_run.cells[game.map_run.robot.pos.x][game.map_run.robot.pos.y].robot = 0;
          game.map_run.robot.pos = (Position){nextx, nexty};
          game.map_run.cells[game.map_run.robot.pos.x][game.map_run.robot.pos.y].robot = 1;
        }
      } else if (nowseq.procs[pid].ops[i] == MOV) {
        int nextx = game.map_run.robot.pos.x + dx[game.map_run.robot.dir];
        int nexty = game.map_run.robot.pos.y + dy[game.map_run.robot.dir];
        if (nextx < 0 || nextx >= game.map_run.col || nexty < 0 || nexty >= game.map_run.row || abs(game.map_run.cells[nextx][nexty].height - game.map_run.cells[game.map_run.robot.pos.x][game.map_run.robot.pos.y].height) != 0) {
          cout << "WARNING:INVALID_MOVE_OPERATION!" << endl;
        } else {
          game.map_run.cells[game.map_run.robot.pos.x][game.map_run.robot.pos.y].robot = 0;
          game.map_run.robot.pos = (Position){nextx, nexty};
          game.map_run.cells[game.map_run.robot.pos.x][game.map_run.robot.pos.y].robot = 1;
        }
      } else {
        int nowx = game.map_run.robot.pos.x;
        int nowy = game.map_run.robot.pos.y;
        if (game.map_run.cells[nowx][nowy].light_id == -1 || game.map_run.lights[game.map_run.cells[nowx][nowy].light_id].lighten) {
          cout << "WARNING:INVALID_LIGHT_OPERATION" << endl;
        } else {
          game.map_run.lights[game.map_run.cells[nowx][nowy].light_id].lighten = 1;
        }
      }
      auto_save();
      check();
      if (now_situ != DARK) return;
    }
  }
}
Result robot_run(const char * path) {
  ifstream fin(path);
  game.map_run = game.map_init;
  game.auto_save_id = 0;
  auto_save();
  now_step = 0;
  now_situ = DARK;
  fin >> nowseq.count;
  for (int i = 0; i < nowseq.count; ++i) {
    fin >> nowseq.procs[i].count;
    for (int j = 0; j < nowseq.procs[i].count; ++j) {
      string oper;
      fin >> oper;
      if (oper[0] == 'T') {
        if (oper[1] == 'L') { // TL
          nowseq.procs[i].ops[j] = TL;
        } else { // TR
          nowseq.procs[i].ops[j] = TR;
        }
      } else if (oper[0] == 'J') { // JMP
        nowseq.procs[i].ops[j] = JMP;
      } else if (oper[0] == 'L') { // LIT
        nowseq.procs[i].ops[j] = LIT;
      } else {
        if (oper[0] == 'P' || oper[1] == 'A') { // Pn or MAIN(P0)
          nowseq.procs[i].ops[j] = CALL;
          if (oper[0] == 'P') {
            int temp_num = 0;
            for (int k = 1; k < oper.size(); ++k)
              temp_num = temp_num * 10 + oper[k] - '0';
            nowseq.procs[i].is_CALL[j] = temp_num;
          } else {
            nowseq.procs[i].is_CALL[j] = 0;
          }
        } else { // MOV
          nowseq.procs[i].ops[j] = MOV;
        }
      }
    }  
  }
  call_proc(0);
  return (Result){now_step, now_situ};
}
