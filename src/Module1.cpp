#include <cstdint>
#include <fstream>
#include "public.h"
#include <cstring>
using namespace std;

#pragma pack(1)
struct BmpHeader {
    char bitmapSignatureBytes[2] = {'B', 'M'};
    uint32_t sizeOfBitmapFile;
    uint32_t reservedBytes = 0;
    uint32_t pixelDataOffset = 54;
} bmpHeader, bot_bmpHeader;
struct BmpInfoHeader {
    uint32_t sizeOfThisHeader = 40;
    int32_t width; // in pixels
    int32_t height; // in pixels
    uint16_t numberOfColorPlanes = 1; // must be 1
    uint16_t colorDepth = 24;
    uint32_t compressionMethod = 0;
    uint32_t rawBitmapDataSize = 0; // generally ignored
    int32_t horizontalResolution = 3780; // in pixel per meter
    int32_t verticalResolution = 3780; // in pixel per meter
    uint32_t colorTableEntries = 0;
    uint32_t importantColors = 0;
} bmpInfoHeader, bot_bmpInfoHeader;
struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
Pixel a[5000][5000];

//机器人
void bot() {
    int x = 0, y = 0; 
    x = 96 + (game.map_run.robot.pos.x - game.map_run.robot.pos.y + game.map_run.row - 1) * 96 + 30;
    y = 96 + 24 * highest - 24 * game.map_run.cells[game.map_run.robot.pos.x][game.map_run.robot.pos.y].height
      + (game.map_run.robot.pos.x + game.map_run.robot.pos.y) * 48 + 8;
    ifstream fin;
    if (game.map_run.robot.dir == 0) fin.open("Resources/UP.bmp", ios::binary);
    else if (game.map_run.robot.dir == 2) fin.open("Resources/DOWN.bmp", ios::binary);
    else if (game.map_run.robot.dir == 1) fin.open("Resources/LEFT.bmp", ios::binary);
    else fin.open("Resources/RIGHT.bmp", ios::binary);
    fin.read((char *) &bot_bmpHeader, 14);
    fin.read((char *) &bot_bmpInfoHeader, 40);
    for (int j = bot_bmpInfoHeader.height - 1; j >= 0; j--) {
        for (int i = 0; i < bot_bmpInfoHeader.width; i++) {
            Pixel temp;
            fin.read((char *)&temp, 3);
            if (!( ((temp.red - 163) < 10) && ((temp.red - 163) > -10) 
                && ((temp.green - 73) < 10) && ((temp.green - 73) > -10)
                && ((temp.blue - 164) < 10) && ((temp.blue - 164) > -10) )) 
                a[x + i][y + j + 1 - bot_bmpInfoHeader.height] = temp;
        }
    }
    fin.close();
}

//填充单格颜色
void grid1(int x ,int y ,int r, int g, int b) {
    a[x][y].red = r;
    a[x][y].green = g;
    a[x][y].blue = b;
}

//上表面
void sur(int x, int y, int r, int g, int b) {
    for (int s = 0; s <= 48; s++) {
        for (int t = -2*s+5; t <= 2*s-4; t++) {
            grid1(x + t, y + s - 48, r, g, b);
            grid1(x + t, y - s + 48, r, g, b);
        }
    }
}

//上表面轮廓
void sur_otl(int x, int y, int k, int i, int j) {
    for (int s = 1; s <= 48; s++) {
        if (game.map_run.cells[i][j].light_id == -1) {
            if (s == 1) {
                grid1(x-2*s+1, y+s-48, 40, 40, 40); grid1(x-2*s+2, y+s-48, 77, 81, 105);
                grid1(x-2*s+1, y-s+48, 40, 40, 40); grid1(x-2*s+2, y-s+48, 77, 81, 105);
                grid1(x+2*s, y+s-48, 40, 40, 40); grid1(x+2*s-1, y+s-48, 77, 81, 105);
                grid1(x+2*s, y-s+48, 40, 40, 40); grid1(x+2*s-1, y-s+48, 77, 81, 105);
            } else {
                grid1(x-2*s+1, y+s-48, 40, 40, 40); grid1(x-2*s+2, y+s-48, 77, 81, 105); grid1(x-2*s+3, y+s-48, 114, 122, 130); grid1(x-2*s+4, y+s-48, 151, 163, 175);
                grid1(x-2*s+1, y-s+48, 40, 40, 40); grid1(x-2*s+2, y-s+48, 77, 81, 105); grid1(x-2*s+3, y-s+48, 114, 122, 130); grid1(x-2*s+4, y-s+48, 151, 163, 175);
                grid1(x+2*s, y+s-48, 40, 40, 40); grid1(x+2*s-1, y+s-48, 77, 81, 105); grid1(x+2*s-2, y+s-48, 114, 122, 130); grid1(x+2*s-3, y+s-48, 151, 163, 175);
                grid1(x+2*s, y-s+48, 40, 40, 40); grid1(x+2*s-1, y-s+48, 77, 81, 105); grid1(x+2*s-2, y-s+48, 114, 122, 130); grid1(x+2*s-3, y-s+48, 151, 163, 175);
            }
        } else if (game.map_run.lights[game.map_run.cells[i][j].light_id].lighten) {
            if (s == 1) {
                grid1(x-2*s+1, y+s-48, 40, 40, 40); grid1(x-2*s+2, y+s-48, 94, 90, 32);
                grid1(x-2*s+1, y-s+48, 40, 40, 40); grid1(x-2*s+2, y-s+48, 94, 90, 32);
                grid1(x+2*s, y+s-48, 40, 40, 40); grid1(x+2*s-1, y+s-48, 94, 90, 32);
                grid1(x+2*s, y-s+48, 40, 40, 40); grid1(x+2*s-1, y-s+48, 94, 90, 32);
            } else {
                grid1(x-2*s+1, y+s-48, 40, 40, 40); grid1(x-2*s+2, y+s-48, 94, 90, 32); grid1(x-2*s+3, y+s-48, 148, 139, 25); grid1(x-2*s+4, y+s-48, 202, 189, 17);
                grid1(x-2*s+1, y-s+48, 40, 40, 40); grid1(x-2*s+2, y-s+48, 94, 90, 32); grid1(x-2*s+3, y-s+48, 148, 139, 25); grid1(x-2*s+4, y-s+48, 202, 189, 17);
                grid1(x+2*s, y+s-48, 40, 40, 40); grid1(x+2*s-1, y+s-48, 94, 90, 32); grid1(x+2*s-2, y+s-48, 148, 139, 25); grid1(x+2*s-3, y+s-48, 202, 189, 17);
                grid1(x+2*s, y-s+48, 40, 40, 40); grid1(x+2*s-1, y-s+48, 94, 90, 32); grid1(x+2*s-2, y-s+48, 148, 139, 25); grid1(x+2*s-3, y-s+48, 202, 189, 17);
            }

        } else {
            if (s == 1) {
                grid1(x-2*s+1, y+s-48, 40, 40, 40); grid1(x-2*s+2, y+s-48, 32, 32, 90);
                grid1(x-2*s+1, y-s+48, 40, 40, 40); grid1(x-2*s+2, y-s+48, 32, 32, 90);
                grid1(x+2*s, y+s-48, 40, 40, 40); grid1(x+2*s-1, y+s-48, 32, 32, 90);
                grid1(x+2*s, y-s+48, 40, 40, 40); grid1(x+2*s-1, y-s+48, 32, 32, 90);
            } else {
                grid1(x-2*s+1, y+s-48, 40, 40, 40); grid1(x-2*s+2, y+s-48, 32, 32, 90); grid1(x-2*s+3, y+s-48, 25, 25, 140); grid1(x-2*s+4, y+s-48, 18, 18, 190);
                grid1(x-2*s+1, y-s+48, 40, 40, 40); grid1(x-2*s+2, y-s+48, 32, 32, 90); grid1(x-2*s+3, y-s+48, 25, 25, 140); grid1(x-2*s+4, y-s+48, 18, 18, 190);
                grid1(x+2*s, y+s-48, 40, 40, 40); grid1(x+2*s-1, y+s-48, 32, 32, 90); grid1(x+2*s-2, y+s-48, 25, 25, 140); grid1(x+2*s-3, y+s-48, 18, 18, 190);
                grid1(x+2*s, y-s+48, 40, 40, 40); grid1(x+2*s-1, y-s+48, 32, 32, 90); grid1(x+2*s-2, y-s+48, 25, 25, 140); grid1(x+2*s-3, y-s+48, 18, 18, 190);
            }
        }
    }
}

//竖线
void vtc(int x, int y, int r, int g,int b) {
    for (int s = 0; s < 24; s++) {
        grid1(x, y + 48 + s, r, g, b); grid1(x + 1, y + 48 + s, r, g, b);
        grid1(x - 96, y + s, r, g, b); grid1(x - 95, y + s, r, g, b);
        grid1(x + 96, y + s, r, g, b); grid1(x + 97, y + s, r, g, b);
    }
}

//侧面
void side1(int x, int y) {
    for (int t = 0; t < 96; t++) {
        for (int s = 2; s < 25; s++) {
            grid1(x - 94 + t, y + (t+1)/2 + s - 1, 176, 186, 196);
            grid1(x + 96 - t, y + t/2 + s - 1, 160, 160, 160);
        }
    }
}

//格子
void grid() {
    //为了处理一些极端情况的遮挡关系，最终选择先把坐标比机器人小的所有格子画完
    for (int k = 1; k <= highest; k++) { //每层
        for (int j = 0; j <= game.map_run.robot.pos.y; j++) {
            for (int i = 0; i <= game.map_run.robot.pos.x; i++) {
                if ((j != game.map_run.robot.pos.y)||(i != game.map_run.robot.pos.x)) {
                    if (game.map_run.cells[i][j].height >= k) { //每格
                        int x = 0, y = 0; 
                        x = 148 + (i - j + game.map_run.row - 1) * 96;
                        y = 96 + 24 * highest - 24 * k + (i + j) * 48;
                        //上表面
                        sur(x, y, 188, 204, 220); 
                        //侧面
                        if (k > 1) side1(x, y);
                        //灯
                        if (game.map_run.cells[i][j].light_id != -1) {
                            if (!game.map_run.lights[game.map_run.cells[i][j].light_id].lighten) sur(x, y, 9, 10, 240);
                            else sur(x, y, 255, 238, 10);
                        }
                        //上表面轮廓
                        sur_otl(x, y, k, i ,j);
                        //竖线
                        vtc(x, y, 60, 60, 60);
                    }
                }
            }
        }
    }
    //这是画其他格子，代码一样
    for (int k = 1; k <= highest + 1; k++) { //每层
        for (int j = 0; j < game.map_run.row; j++) {
            for (int i = 0; i < game.map_run.col; i++) {
                if ((j > game.map_run.robot.pos.y)||(i > game.map_run.robot.pos.x)||game.map_run.cells[i][j].robot) {
                    if (game.map_run.cells[i][j].height >= k) { //每格
                        int x = 0, y = 0; 
                        x = 148 + (i - j + game.map_run.row - 1) * 96;
                        y = 96 + 24 * highest - 24 * k + (i + j) * 48;
                        //上表面
                        sur(x, y, 188, 204, 220); 
                        //侧面
                        if (k > 1) side1(x, y);
                        //灯
                        if (game.map_run.cells[i][j].light_id != -1) {
                            if (!game.map_run.lights[game.map_run.cells[i][j].light_id].lighten) sur(x, y, 9, 10, 240);
                            else sur(x, y, 255, 238, 10);
                        }
                        //上表面轮廓
                        sur_otl(x, y, k, i ,j);
                        //竖线
                        vtc(x, y, 60, 60, 60);
                    } else if ((game.map_run.cells[i][j].robot)&&(k == game.map_run.cells[i][j].height + 1)) bot(); //机器人
                }
            }
        }
    }
}



//主函数
void save(const char * path) {
    bmpInfoHeader.width = 96 * (game.map_init.row + game.map_init.col - 1) + 200;
    bmpInfoHeader.height = 48 * (game.map_init.row + game.map_init.col - 1) + 200 + highest * 24;
    bmpHeader.sizeOfBitmapFile = 0x36 + bmpInfoHeader.width * bmpInfoHeader.height * 3;
//背景
    for (int j = 0; j < bmpInfoHeader.height; j++) {
        for (int i = 0; i < bmpInfoHeader.width; i++) {
            a[i][j].red = 20;
            a[i][j].green = 128;
            a[i][j].blue = 19;
        }
    }

//格子
    grid();

//bmp头
    ofstream fout(path, ios::binary);
    fout.write((char *) &bmpHeader, 14);
    fout.write((char *) &bmpInfoHeader, 40);
//bmp内容
    for (int j = bmpInfoHeader.height - 1; j >= 0; j--) {
        for (int i = 0; i < bmpInfoHeader.width; i++) {
            fout.write((char *) &a[i][j], 3);
        }
    }
    fout.close();
}

void auto_save() {
    if (game.save_path[0] == '\0') return;
    char path[MAX_PATH_LEN];
    for (int i = 0; i < MAX_PATH_LEN; i++) path[i] = '\0';
    int i = 0, j = 0;
    while (i < strlen(game.save_path)) {
        if (game.save_path[i] == '%') {
            int k = 0;
            i++;
            while (game.save_path[i] != 'd') {
                k = k * 10 + game.save_path[i] - '0';
                i++;
            }
            i++;
            int id = game.auto_save_id;
            int max = 1;
            for (int t = 0; t < k; t++) max *= 10;
            if (id >= max) {
                int id1 = id, k1 = 0;
                while (id1 != 0) {
                    k1++;
                    id1 /= 10;
                }
                j += k1 - 1;
                for (int t = 0; t < k1; t++) {
                    path[j] = id % 10 + '0';
                    id /= 10;
                    j--;
                }
                j += k1 + 1;
            } else {
                j += k - 1;
                for (int t = 0; t < k; t++) {
                    path[j] = id % 10 + '0';
                    id /= 10;
                    j--;
                }
                j += k + 1;
            }
        } else {
            path[j] = game.save_path[i];
            i++;
            j++;
        }
    }
    save(path);
    game.auto_save_id++;
}
