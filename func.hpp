//関数を入れたり定義するところ
//pixel10はclassの方に書いてしまったので、マージするときに専用のfunc.hppをもう一つ作った方が良さそう。
//各ファイルに何を定義しているのかは自分で確認すること。

//標準、拡張
#include <iostream>
#include <string.h>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <math.h>
#include <random>

//file c
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

//define
#define MOD 1000000007
#define rng(i, a, b) for(int i = int(a); i < int(b); i++)
#define rep(i, b) rng(i, 0, b)
#define pb push_back
#define eb emplace_back
#define bg begin()
#define ed end()

//namespace
using namespace std;
using ll = long long;

// =^~^=)struct cat
struct over{
    int x;
    int y;
    int direct;
};

struct searchpoint{
    int x;
    int y;
};

//閾値 正規化すること
#define Point 500

//int比較関数
int Comp(vector<pixel10> a , vector<pixel10> b, int edge_size){
    int pixel_rate = 0;
    int r_rate = 0;
    int g_rate = 0;
    int b_rate = 0;
    rep(i,edge_size){
        r_rate = abs(a[i].r - b[i].r);
        g_rate = abs(a[i].g - b[i].g);
        b_rate = abs(a[i].b - b[i].b);
        pixel_rate += r_rate + g_rate + b_rate;
    }
    return pixel_rate;
}

//本関数
vector<vector<over>> search_1(vector<vector<SplitBlock>> tagu , int size_of_pixel , int gridx , int gridy){
    //この関数では、4つの引数から、エッジ情報の比較、検討を行い、元画像を復元する。
    //復元した情報はvector<vector<over>> search_1で返す。
    //また、必要に応じて探索方法をかえたsearch_2 search_3 search_4なども実装する予定。
    
    rep(i,tagu.size()){//dataをmainから読み取れてるか確認
        tagu[i][0].displayClassData();
    }
    
    //探索開始　変数の定義 -------------
    
    vector<pixel10> upComp; //比較先のup
    vector<pixel10> bottomComp; //比較先のbottom
    vector<pixel10> rightComp; //比較先のright
    vector<pixel10> leftComp; //比較先のleft
    
    vector<pixel10> rightSearch;
    vector<pixel10> bottomSearch;
    
    int competive = 0; //Compの引数を保存
    int low_competive = MOD; //Compの最小引数比較用
    searchpoint select = {0,0}; //現在の探索場所
    searchpoint bestpos = {0,0}; //もっともよいブロックを保存
    int searchDirect = 0; //探索の時の向き保存用 1 2 3 4 ->見つかったらtagu[i][j].setdirection(x);
    
    vector<pixel10> search = tagu[0][0].getedge_left(); //現在探索してる辺を入れる
    
    //左上をまず探します 端まで（この時クラスの変数に代入はせずに探すだけ）
    rep(k,tagu.size()){
        
        rep(i,tagu.size()){
            rep(j,tagu.at(0).size()){
                //探す
                //edgedataを代入
                upComp = tagu[i][j].getedge_up_r();
                bottomComp = tagu[i][j].getedge_bottom_r();
                rightComp = tagu[i][j].getedge_right_r();
                leftComp = tagu[i][j].getedge_left_r();
                //[i][j] up
                competive = Comp(upComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 1;
                }
                //[i][j] bottom
                competive = Comp(bottomComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 2;
                }
                //[i][j] right
                competive = Comp(rightComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 3;
                }
                //[i][j] left
                competive = Comp(leftComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 4;
                }
            }
        }
        if(low_competive < Point){
            if(searchDirect == 1){ search = tagu[bestpos.y][bestpos.x].getedge_bottom(); }
            else if(searchDirect == 2){ search = tagu[bestpos.y][bestpos.x].getedge_up(); }
            else if(searchDirect == 3){ search = tagu[bestpos.y][bestpos.x].getedge_left(); }
            else if(searchDirect == 4){ search = tagu[bestpos.y][bestpos.x].getedge_right(); }
            select = bestpos;
        }else{
            break;
        }
    }
    
    //0 0 から左端にいったblockの座標は[select.y][select.x]
    //上にsearchを変更。座標はそのまま
    
    if(searchDirect == 1){  search = tagu[select.y][select.x].getedge_left();  }
    else if(searchDirect == 2){ search = tagu[select.y][select.x].getedge_right(); }
    else if(searchDirect == 3){ search = tagu[select.y][select.x].getedge_up();  }
    else if(searchDirect == 4){ search = tagu[select.y][select.x].getedge_bottom(); }
    
    //上側探索
    rep(k,tagu.at(0).size()){
        rep(i,tagu.size()){
            rep(j,tagu.at(0).size()){
                //探す
                //edgedataを代入
                upComp = tagu[i][j].getedge_up_r();
                bottomComp = tagu[i][j].getedge_bottom_r();
                rightComp = tagu[i][j].getedge_right_r();
                leftComp = tagu[i][j].getedge_left_r();
                //[i][j] up
                competive = Comp(upComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 1;
                }
                //[i][j] bottom
                competive = Comp(bottomComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 2;
                }
                //[i][j] right
                competive = Comp(rightComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 3;
                }
                //[i][j] left
                competive = Comp(leftComp, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 4;
                }
            }
        }
        if(low_competive < Point){
            if(searchDirect == 1){  search = tagu[bestpos.y][bestpos.x].getedge_bottom(); }
            else if(searchDirect == 2){ search = tagu[bestpos.y][bestpos.x].getedge_up(); }
            else if(searchDirect == 3){ search = tagu[bestpos.y][bestpos.x].getedge_left();  }
            else if(searchDirect == 4){ search = tagu[bestpos.y][bestpos.x].getedge_right(); }
            
            select = bestpos;
        }else{
            break;
        }
    }
    
    //select.x , select.yが左上
    cout << "leftTop : " << select.y << select.x << endl;
    //向きを計算 bottom 1 up 2 left 3 right 4 になってるので、 directはそれぞれ 2 0 1 3
    if(searchDirect == 1 ) tagu[select.y][select.x].setdirection(2);
    if(searchDirect == 2 ) tagu[select.y][select.x].setdirection(0);
    if(searchDirect == 3 ) tagu[select.y][select.x].setdirection(1);
    if(searchDirect == 4 ) tagu[select.y][select.x].setdirection(3);
    
    //左上に値を代入
    if(tagu[select.y][select.x].getdirection() == 0){
        rightSearch = tagu[select.y][select.x].getedge_right();
    }
    else if(tagu[select.y][select.x].getdirection() == 1){
        rightSearch = tagu[select.y][select.x].getedge_up();
    }
    else if(tagu[select.y][select.x].getdirection() == 2){
        rightSearch = tagu[select.y][select.x].getedge_left();
    }
    else if(tagu[select.y][select.x].getdirection() == 3){
        rightSearch = tagu[select.y][select.x].getedge_bottom();
    }
    
    //
    //　左上から横、縦へ全部探索していく。
    //
    
    vector<vector<over>> temp(gridy,vector<over>(gridx));
    rep(i,gridy) rep(j,gridx) temp[i][j] = {j,i,0}; //初期化
    
    temp[0][0] =  {select.y,select.x,tagu[select.y][select.x].getdirection()};
    //縦に探索 それをtempにいれてsearchdataにいれなおす。
    competive = 0; //Compの引数を保存
    low_competive = MOD; //Compの最小引数比較用
    row in_edge_info = {0,0};
    
    for(int k = 0; k < tagu.size();){
        rep(i,tagu.size()){
            rep(j,tagu.at(0).size()){
                if ( i != select.y && j != select.x ){
                    //edgedataを代入
                    upComp = tagu[i][j].getedge_up_r();
                    bottomComp = tagu[i][j].getedge_bottom_r();
                    rightComp = tagu[i][j].getedge_right_r();
                    leftComp = tagu[i][j].getedge_left_r();
                    //[i][j] up
                    competive = Comp(upComp, rightSearch ,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos = {i,j};
                        searchDirect = 1;
                    }
                    //[i][j] bottom
                    competive = Comp(bottomComp, rightSearch ,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos = {i,j};
                        searchDirect = 2;
                    }
                    //[i][j] right
                    competive = Comp(rightComp, rightSearch ,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos = {i,j};
                        searchDirect = 3;
                    }
                    //[i][j] left
                    competive = Comp(leftComp, rightSearch ,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos = {i,j};
                        searchDirect = 4;
                    }
                }
            }
        }
        if(low_competive < Point){
            if(searchDirect == 1){  rightSearch = tagu[select.y][select.x].getedge_bottom();  }
            else if(searchDirect == 2){ rightSearch = tagu[select.y][select.x].getedge_up(); }
            else if(searchDirect == 3){ rightSearch = tagu[select.y][select.x].getedge_left();  }
            else if(searchDirect == 4){ rightSearch = tagu[select.y][select.x].getedge_right(); }
        }else{
            break;
        }
        
        if(tagu[select.y][select.x].getcomp() == 0){
            //tempへ
            temp[0][k] = {select.y,select.x,tagu[select.y][select.x].getdirection()};
            
            //taguに値をいれておく。
            in_edge_info.x = bestpos.x;
            in_edge_info.y = bestpos.y;
            if(searchDirect == 1){  tagu[select.y][select.x].setbottom(in_edge_info);  }
            else if(searchDirect == 2){ tagu[select.y][select.x].setup(in_edge_info); }
            else if(searchDirect == 3){ tagu[select.y][select.x].setleft(in_edge_info);  }
            else if(searchDirect == 4){ tagu[select.y][select.x].setright(in_edge_info);}
            
            tagu[select.y][select.x].setcomp(1); //もう使えませーん
            select.x = select.x + 1; //探索ブロックを一個右へ
            k++;
        }
    }
    
    
    //
    //temp [i][n] i = 0 ~ maxまで探索
    //
    
    competive = 0; //Compの引数を保存
    low_competive = MOD; //Compの最小引数比較用
    
    for(int p = 0; p < tagu.size();p++){
        for(int k = 0; k < tagu.at(0).size();k++){
            
            if(tagu[p][k].getdirection() == 0){
                bottomSearch = tagu[p][k].getedge_bottom();
            }else if(tagu[p][k].getdirection() == 1){
                bottomSearch = tagu[p][k].getedge_right();
            }else if(tagu[p][k].getdirection() == 2){
                bottomSearch = tagu[p][k].getedge_up();
            }else if(tagu[p][k].getdirection() == 3){
                bottomSearch = tagu[p][k].getedge_left();
            }
            
            rep(i,tagu.size()){
                rep(j,tagu.at(0).size()){
                    if ( tagu[i][j].getcomp() == 0 ){
                        //edgedataを代入
                        upComp = tagu[i][j].getedge_up_r();
                        bottomComp = tagu[i][j].getedge_bottom_r();
                        rightComp = tagu[i][j].getedge_right_r();
                        leftComp = tagu[i][j].getedge_left_r();
                        
                        competive = Comp(upComp, rightSearch ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {i,j};
                            searchDirect = 0;
                        }
                        competive = Comp(bottomComp, rightSearch ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {i,j};
                            searchDirect = 2;
                        }
                        competive = Comp(rightComp, rightSearch ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {i,j};
                            searchDirect = 3;
                        }
                        competive = Comp(leftComp, rightSearch ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {i,j};
                            searchDirect = 1;
                        }
                    }
                    //i j 探索おわり
                }
            }
            //一番いいであろうブロックは bestpos のsearchdirectに格納されているのでそれをtempに移す。
            temp[p][k + 1] = {bestpos.x , bestpos.y , searchDirect};
        }
    }
    
    //temp -> searchdata
    //tempには探索後のデータが入ってる。00のとこに23がdirect 3など
    //しかし。実際の描画、受け渡しでは00の移動先を00のとこに、23の移動先を23のとこにいれている。
    //tempにはしっかりデータが入っていることを想定して作るので、low_competive < 500の値がおかしいとバグる。
    //ここは学校での活動でhelp出す予定。
    
    
    //探索結果を代入　& return
    vector<vector<over>> searchdata(gridy,vector<over>(gridx)); // return data
    rep(i,gridy) rep(j,gridx) searchdata[i][j] = {0,0,0}; //初期化
    
    //[i][j]のブロックをover pass のy.xのとこに格納。
    //格納内容は{i,j,pass.direct}
    //tempの中身
    cout << "temp :" << endl;
    rep(i,gridy) {
        rep(j,gridx){
            cout << " " << temp[i][j].x << temp[i][j].y << " " << temp[i][j].direct << " : ";
        }
        cout << endl;
    }
    
    
    for(int j = 0; j < temp[0].size(); j++){
        for(int i = 0; i < temp.size(); i++){
            for(int y = 0; y < temp[0].size(); y++){
                for(int x = 0; x < temp.size(); x++){
                    if(temp[x][y].x == i && temp[x][y].y == j){
                        searchdata[x][y].x = x;
                        searchdata[x][y].y = y;
                        searchdata[x][y].direct = temp[x][y].direct;
                    }
                }
            }
        }
    }
    
    //temp        searchdata
    //12 21 10    10 20 11
    //00 02 22 -> 02 21 00
    //01 11 20    22 01 12
    
    //余ったやつを適当なとこにランダムに入れる (よくわからん 変数を増やせばいい？)　※未実装
    cout << "comp :" << endl;
    
    rep(i,gridy){
        cout << endl;
        rep(j,gridx){
            cout << "{ " << searchdata[i][j].x << " , " << searchdata[i][j].y << " , " << searchdata[i][j].direct << " } ";
        }
    }
    cout << " size : " <<  searchdata.size() << searchdata.at(0).size() << endl;
    return searchdata;
}


//左上を探します
over search_corner(vector<vector<SplitBlock>> tagu , int size_of_pixel , int gridx , int gridy){
    vector<pixel10> ser_pos; //比較先のedge
    
    int competive = 0; //Compの引数を保存
    int low_competive = MOD; //Compの最小引数比較用
    int searchDirect = 0; //探索の時の向き保存用 1 2 3 4 ->見つかったらtagu[i][j].setdirection(x);
    searchpoint select = {0,0}; //現在の探索場所
    searchpoint bestpos = {0,0}; //もっともよいブロックを保存
    
    vector<pixel10> search = tagu[0][0].getedge_left(); //現在探索してる辺を入れる
    
    int roop_flag = 0;
    int k = 0;
    
    //左上をまず探します 端まで（この時クラスの変数に代入はせずに探すだけ）
    while(roop_flag != 1){
        rep(i,tagu.size()){
            rep(j,tagu.at(0).size()){
                
                //[i][j] up
                ser_pos = tagu[i][j].getedge_up_r();
                competive = Comp(ser_pos, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 1;
                }
                
                //[i][j] bottom
                ser_pos = tagu[i][j].getedge_bottom_r();
                competive = Comp(ser_pos, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 2;
                }
                
                //[i][j] right
                ser_pos = tagu[i][j].getedge_right_r();
                competive = Comp(ser_pos, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 3;
                }
                
                //[i][j] left
                ser_pos = tagu[i][j].getedge_left_r();
                competive = Comp(ser_pos, search,size_of_pixel);
                if(low_competive > competive){
                    low_competive = competive;
                    bestpos = {i,j};
                    searchDirect = 4;
                }
            }
        }
        
        if(low_competive < Point){
            if(searchDirect == 1){ search = tagu[bestpos.y][bestpos.x].getedge_bottom(); }
            else if(searchDirect == 2){ search = tagu[bestpos.y][bestpos.x].getedge_up(); }
            else if(searchDirect == 3){ search = tagu[bestpos.y][bestpos.x].getedge_left(); }
            else if(searchDirect == 4){ search = tagu[bestpos.y][bestpos.x].getedge_right(); }
            select = bestpos;
        }else{
            roop_flag = 1;
        }
        low_competive = MOD;
        k++;
        cout << k << endl;
        if(k >= gridy) roop_flag = 1;
    }
    
    
    over leftup_pos = {select.x,select.y,searchDirect};
    return leftup_pos;
}








//
//ここから下は画像データの読み取りに使うＧＵＩ担当の方が作成したプログラム。
//必要に応じて変えて行くが、マージするときにはメインのプロジェクトにすでにあるものなので追加しない。
//

void read_txtheader(string *magic_number,int *gridx,int *gridy,
                    int *select_limit,int *select_rate,int *change_rate,
                    int *picture_width,int *picture_height,int *max_light,
                    int *grid_size){ //受け取ったテキストファイルのヘッダーを読み込みます
    
    ifstream file("make.txt");
    string line_string;
    int line_number =0;
    
    getline(file, line_string);
    *magic_number = line_string;//マジックナンバーを読み取る
    
    cout << *magic_number << endl;
    
    file >> line_number;
    *gridx = line_number;
    file >> line_number;
    *gridy = line_number;
    
    cout << *gridx << " " <<*gridy << endl;
    
    file >> line_number;
    *select_limit = line_number;
    
    cout << *select_limit << endl;
    
    file >> line_number;
    *select_rate = line_number;
    file >> line_number;
    *change_rate = line_number;
    
    cout << *select_rate << " " <<*change_rate << endl;
    
    file >> line_number;
    *picture_width = line_number;
    file >> line_number;
    *picture_height = line_number;
    
    cout << *picture_width << " " << *picture_height << endl;
    
    file >> line_number;
    *max_light = line_number;
    
    cout << *max_light << endl;
    
    *grid_size = *picture_width / *gridx;
    cout << *grid_size << endl;
}

struct pixel16{
    string r;
    string g;
    string b;
};

vector<vector<pixel10>> read_txtdata(int width,int height){ //データを読み込みます
    vector<vector<pixel16>> data16(height,vector<pixel16>(width));//16進数でRGB値を保存
    vector<vector<pixel10>> data10(height,vector<pixel10>(width));//10進数でRGB値を保存
    std::ifstream file("make.txt");
    char one_size;
    string tmp[2];
    string trash;
    
    for(int i = 0;i < 6; i++){
        std::getline(file, trash);//ヘッダーは要らないので読み取るだけ
    }
    
    for(int i = 0;i < height; i++){
        for(int j = 0;j < width; j++){ //2重for文で1文字ずつ読み取り2文字に合体してRGBとして構造体に保存
            file.get(one_size);
            tmp[0] = one_size;
            file.get(one_size);
            tmp[1] = one_size;
            data16[i][j].r = tmp[0]+tmp[1];
            
            file.get(one_size);
            tmp[0] = one_size;
            file.get(one_size);
            tmp[1] = one_size;
            data16[i][j].g = tmp[0]+tmp[1];
            
            file.get(one_size);
            tmp[0] = one_size;
            file.get(one_size);
            tmp[1] = one_size;
            data16[i][j].b = tmp[0]+tmp[1];
            //cout << data16[i][j].r << data16[i][j].g << data16[i][j].b << " : ";//表示、処理が重いので無くても良い
        }
        //file.get(one_size);//textに改行がある際にコメントアウトを外すこと
        //file.get(one_size);
        //cout << endl;
    }
    for(int i = 0;i < height; i++){
        for(int j = 0;j < width; j++){//16進数で保存したデータを10進数に変換して保存
            data10[i][j].r = stoi(data16[i][j].r, NULL, 16);
            data10[i][j].g = stoi(data16[i][j].g, NULL, 16);
            data10[i][j].b = stoi(data16[i][j].b, NULL, 16);
            //cout << data10[i][j].r << data10[i][j].g << data10[i][j].b << " : ";//表示
        }
        //cout << endl;
    }
    return data10;//10進数のデータを返す
}
