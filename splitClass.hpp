#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD
//--------------------------

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <math.h>

#include "draw.hpp"

using namespace std;
struct row{ //座標
  int x;
  int y;
};

class SplitBlock{
private:
      //実際の画像データ
     vector<vector<pixel10>> data;

     int number_x; //分割画像の番号 (00 - FF)
     int number_y;
     //分割画像の隣接画像の保存関数　隣接画像がない場合 Null
     row up;
     row bottom;
     row left;
     row right;

     int size;//size
     int direction; //向き 0,1,2,3 初期値0

     int complete; //complete -> 1 : not complete -> 0 ( default constract )

public:
    //classの情報を表示
    void displayClassData();
    //classのpixeldataを表示
    void displayPiexelData_fromSplitBlockClass();

    //比較元エッジ抽出　時計回り
    vector<pixel10> getedge_up();
    vector<pixel10> getedge_bottom();
    vector<pixel10> getedge_right();
    vector<pixel10> getedge_left();

    //比較先エッジ抽出　反時計回り
    vector<pixel10> getedge_up_r();
    vector<pixel10> getedge_bottom_r();
    vector<pixel10> getedge_right_r();
    vector<pixel10> getedge_left_r();

    //GET
    vector<vector<pixel10>> getdata(); //vector<vector<Pixel>>型で一括代入
    int getX();
    int getY();
    row getup();
    row getbottom();
    row getleft();
    row getright();
    int getSize();
    int getdirection();
    int getcomp();

    //SET
    void setPixelData(vector<vector<pixel10>> num);
    void setX(int num);
    void setY(int num);
    void setup(row num);
    void setbottom(row num);
    void setleft(row num);
    void setright(row num);
    void setSize(int num);
    void setdirection(int num);
    void setcomp(int num);

    //constract
    SplitBlock();
};

//--------------------------------
//
//関数の実装
//
//--------------------------------

void SplitBlock::displayClassData(){
    cout << "-----格納されている情報------" << endl;
    cout << "SplitNumber : " << this->number_y << " , " << this->number_x << endl;
    cout << "Direction 角度 : " << this->direction << endl;
    cout << "data : INSTANCEにPixelDataがない場合は出力されません。" << endl;
    cout << data.size() << " : " << data.at(0).size() << endl;
    //displayPiexelData_fromSplitBlockClass();
}
void SplitBlock::displayPiexelData_fromSplitBlockClass(){
    cout << "---- PixelData ----" << endl;
    for(int i = 0 ;i < this->data.size(); i++){
        for(int j = 0;j < this->data.at(0).size();j++){
            cout << " [🟥:"<< this->data[i][j].r << "/";
            cout << "🟩:"<< this->data[i][j].g << "/";
            cout << "🟦:"<< this->data[i][j].b << "] ";
        }
        cout << endl;
    }
}

vector<pixel10> flame;
//比較元エッジ抽出　反時計回り
//比較先エッジ抽出　反時計回り
vector<pixel10> SplitBlock::getedge_up(){
    flame.resize(this->size);
    for(int i = 0; i < this->size ; i++){
        flame[i] = data[0][i];
    }
    return flame;
}
vector<pixel10> SplitBlock::getedge_up_r(){
    flame.resize(this->size);
    int insert = 0;
    for(int i = this->size; i > 0 ;){
        i--;
        flame[insert] = data[0][i];
        insert++;
    }
    return flame;
}
////
vector<pixel10> SplitBlock::getedge_bottom(){
    flame.resize(this->size);
    int insert = 0;
    for(int i = this->size; i > 0  ;){
        i--;
        flame[insert] = data[this->size - 1][i];
        insert++;
    }
    return flame;
}
vector<pixel10> SplitBlock::getedge_bottom_r(){
    flame.resize(this->size);
    for(int i = 0; i < this->size ; i++){
        flame[i] = data[this->size - 1][i];
    }
    return flame;
}
//// ok
vector<pixel10> SplitBlock::getedge_right(){
    flame.resize(this->size);
    for(int i = 0; i < flame.size() ; i++){
        flame[i] = data[i][this->size - 1];
    }
    return flame;
}

vector<pixel10> SplitBlock::getedge_right_r(){
    flame.resize(this->size);
    int insert = 0;
    for(int i = this->size; i > 0 ;){
        i--;
        flame[insert] = data[i][this->size - 1];
        insert++;
    }
    return flame;
}
////
vector<pixel10> SplitBlock::getedge_left(){
    flame.resize(this->size);
    int insert = 0;
    for(int i = this->size; i > 0  ;){
        i--;
        flame[insert] = data[i][0];
        insert++;
    }
    return flame;
}
vector<pixel10> SplitBlock::getedge_left_r(){
    flame.resize(this->size);
    for(int i = 0 ; i < this->size;i++){
        flame[i] = data[i][0];
    }
    return flame;
}

//--------------------------------
//
//get系
//
//--------------------------------

vector<vector<pixel10>> SplitBlock::getdata(){ return this->data; }

int SplitBlock::getX(){ return this->number_x; }

int SplitBlock::getY(){ return this->number_y; }

row SplitBlock::getup(){ return this->up; }

row SplitBlock::getbottom(){ return this->bottom; }

row SplitBlock::getleft(){ return this->left; }

row SplitBlock::getright(){ return this->right; }

int SplitBlock::getSize(){ return this->size; }

int SplitBlock::getdirection(){ return this->direction; }

int SplitBlock::getcomp(){ return this->complete; }

//--------------------------------
//
//set系
//
//--------------------------------

void SplitBlock::setPixelData(vector<vector<pixel10>> num){ this->data = num; }

void SplitBlock::setX(int num){ this->number_x = num; }

void SplitBlock::setY(int num){ this->number_y = num; }

void SplitBlock::setup(row num){ this->up = num; }

void SplitBlock::setbottom(row num){ this->bottom = num; }

void SplitBlock::setleft(row num){ this->left = num; }

void SplitBlock::setright(row num){ this->right = num; }

void SplitBlock::setSize(int num){ this->size = num;}

void SplitBlock::setdirection(int num){ this->direction = num; }

void SplitBlock::setcomp(int num){ this->complete = num; }

//--------------------------------
//
//constract
//
//--------------------------------

SplitBlock::SplitBlock(){
    this->number_x = 0;
    this->number_y = 0;
    this->up = {0,0};
    this->bottom = {0,0};
    this->left = {0,0};
    this->right = {0,0};
    this->size = 0;
    this->direction = 0;
    this->complete = 0;
}

#endif
