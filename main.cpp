//このプログラムではmain.cppでデータを扱い、func.hppにて探索関数を実装している。
//GUI担当者にmainは略奪されるので、このmainはあくまで仮。処理部分も可能な限り早く関数に落とし込もうと思う。
//使用する際は　ここから↓
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <math.h>

#include "splitClass.hpp" //class
#include "func.hpp" //事実上のmain
//ここまでの#includeを行った上で,func.hppとクラス本体の定義がされているsplitclass.hppを自分のプロジェクトに入れて欲しい。


using namespace std; //namespace


int main(){ //main
    string magic_number; //マジックナンバー
    int gridx;//分割数
    int gridy;
    int select_limit;//選択可能回数
    int select_rate;//選択コスト変換レート
    int change_rate;//交換コスト変換レート
    int picture_width;//画像の縦横の大きさ
    int picture_height;
    int max_light; //最大輝度値
    int grid_size; //ブロックの一辺のサイズ
    int num_of_block;//分割された画像の数gridx*gridy
    
    cout << "header" << endl;
    read_txtheader(&magic_number,&gridx,&gridy,&select_limit,
                   &select_rate,&change_rate,&picture_width,
                   &picture_height,&max_light,&grid_size);//ヘッダーを読み取る関数
    
    vector<vector<pixel10>> data(picture_width,vector<pixel10>(picture_height));//main関数内でデータを保存する
    
    data = read_txtdata(picture_width, picture_height);//読み取ったデータを保存
    
    num_of_block=gridx*gridy;//分割画像数の計算
    
    FILE* fp;
    char filename[256];
    sprintf(filename, "image_data.csv");
    fp = fopen(filename, "w");
    for(int i = 0 ; i < data.size(); i++){
        for(int j = 0 ; j < data.at(0).size(); j++){
            fprintf(fp, "%d, %d, %d\n", data[i][j].r, data[i][j].g, data[i][j].b);
        }
    }
    fclose(fp);
    ///
    
    int size_of_pixel = picture_width / gridx;//一つの分割画像の横、縦のサイズ。正方形なので一緒です
    vector<vector<SplitBlock>> tagu = input_tagu( data , size_of_pixel , gridx , gridy);
    
    //vector<vector<SplitBlock>> tagu(gridy,vector<SplitBlock>(gridx)); //gridy * gridxの二次元配列でクラスのインスタンスを生成。　実際に探索を行うときに使うやつ。中身はまだ入っていない。
    
    //クラスはコンストラクタの通りに初期化されている。
    //初期値代入用の変数
    /*vector<vector<pixel10>> num(size_of_pixel,vector<pixel10>(size_of_pixel));//代入用配列
    cout << " size_of_pixel : " << size_of_pixel <<endl;
    
    //Classのインスタンスblockに値を代入する
    for(int i = 0 ; i < tagu.size(); i++){
    for(int j = 0 ; j < tagu.at(0).size(); j++){
      tagu[i][j].setY(i); //添字sety
      tagu[i][j].setX(j); //添字setx
      tagu[i][j].setSize(size_of_pixel);//setsize
      //data
      for(int y = 0; y < size_of_pixel ; y++){
          for(int x = 0; x < size_of_pixel ; x++){
            num[y][x] = data[i * size_of_pixel + y][j * size_of_pixel + x];
              }
            }
      tagu[i][j].setPixelData(num);
    }
  }*/
     
    
    //こっから探索（クラスの関数を使って比較を行い、 vector<vector<over>> searchdataに値を入れて、txt形式で出力。
    vector<vector<over>> searchdata = main_search(tagu,size_of_pixel,gridx,gridy);
    getchar();
    
    //free & main function return
    vector<vector<over>>().swap(searchdata);
    vector<vector<SplitBlock>>().swap(tagu);
    return 0;
    
}
