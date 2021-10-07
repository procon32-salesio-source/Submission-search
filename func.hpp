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
#define Point 5000

//比較関数
int Comp(vector<pixel10> a , vector<pixel10> b, int edge_size){
    int pixel_rate = 0;
    int r_rate = 0;
    int g_rate = 0;
    int b_rate = 0;
    for(int i = 0; i < edge_size ; i++){
        r_rate = abs(a[i].r - b[i].r);
        g_rate = abs(a[i].g - b[i].g);
        b_rate = abs(a[i].b - b[i].b);
        pixel_rate += r_rate + g_rate + b_rate;
    }
    return pixel_rate;
}



//vector<vector<over>> xを表示する
void draw_searchdata(vector<vector<over>> data){
    
    rep(i,data.size()){
        rep(j,data.at(0).size()){
            cout << " {" << data[i][j].x << "," << data[i][j].y << "," << data[i][j].direct << "},";
        } cout << endl;
    }
    
}

void draw_original_comp(vector<vector<SplitBlock>> data){
    
    rep(i,data.size()){
        rep(j,data.at(0).size()){
            cout << " " << data[i][j].getcomp() << " ";
        } cout << endl;
    }
    
}
//temp -> resultへ入れ替える
vector<vector<over>> swap_searchdata(vector<vector<over>> temp , int gridy , int gridx){
    
    vector<vector<over>> searchdata(gridy,vector<over>(gridx)); // return data
    rep(i,gridy) rep(j,gridx) searchdata[i][j] = {0,0,0}; //初期化
    
    
    
    for(int i = 0 ; i < temp.size(); i++){
        for(int j = 0 ; j < temp.at(0).size(); j++){
            
            for(int p = 0 ; p < temp.size(); p++){
                for(int h = 0 ; h < temp.at(0).size(); h++){
                    
                    if(temp[i][j].y == p && temp[i][j].x == h){
                        searchdata[p][h].x = j;
                        searchdata[p][h].y = i;
                        searchdata[p][h].direct = temp[i][j].direct;
                    }
                    
                }
            }
        }
    }
    
    return searchdata;
}

//左上を正当法で探す
over search_corner(vector<vector<SplitBlock>> tagu , int size_of_pixel , int gridx , int gridy){
    
    int competive = 0; //Compの引数を保存
    int low_competive = MOD; //Compの最小引数比較用
    int searchDirect = 0; //探索用の向き保存Flag 1 2 3 4
    searchpoint select = {0,0}; //現在の探索場所
    searchpoint bestpos = {0,0}; //もっともよいブロックの座標を保存
    
    vector<pixel10> ser_pos; //比較先のedge
    vector<pixel10> search = tagu[0][0].getedge_left(); //現在探索してる辺を入れる
    
    int roop_flag = 0;//端が見つかったら1
    int k = 0;
    
    //左橋をまず探します 端まで（この時クラスの変数に代入はせずに探すだけ）
    while(roop_flag != 1){
        for(int i = 0; i < tagu.size() ; i++){
            for(int j = 0; j < tagu.at(0).size() ; j++){
                if(!(i == select.y && j == select.x)){
                    //[i][j] up
                    ser_pos = tagu[i][j].getedge_up_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 1;
                    }
                    
                    //[i][j] bottom
                    ser_pos = tagu[i][j].getedge_bottom_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 2;
                    }
                    
                    //[i][j] right
                    ser_pos = tagu[i][j].getedge_right_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 3;
                    }
                    
                    //[i][j] left
                    ser_pos = tagu[i][j].getedge_left_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 4;
                    }
                }
            }
        }
        
        cout << low_competive << " " << bestpos.x << bestpos.y << endl;
        
        if(low_competive < Point){
            if(searchDirect == 1){ search = tagu[bestpos.y][bestpos.x].getedge_bottom(); }
            else if(searchDirect == 2){ search = tagu[bestpos.y][bestpos.x].getedge_up(); }
            else if(searchDirect == 3){ search = tagu[bestpos.y][bestpos.x].getedge_left(); }
            else if(searchDirect == 4){ search = tagu[bestpos.y][bestpos.x].getedge_right(); }
            select = bestpos;
            
        }else{
            roop_flag = 1;
            cout << "roop stop   :  looking edge block "<<endl;
        }
        low_competive = MOD;
        k++;
        if(k >= gridx){
            roop_flag = 1;
            cout << "roop stop  :  max weight " <<endl;
        }
    }
    
    //select.x , select.yが 00 から左にいった時の座標
    if(searchDirect == 1){  search = tagu[select.y][select.x].getedge_left();  }
    else if(searchDirect == 2){ search = tagu[select.y][select.x].getedge_right(); }
    else if(searchDirect == 3){ search = tagu[select.y][select.x].getedge_up();  }
    else if(searchDirect == 4){ search = tagu[select.y][select.x].getedge_bottom(); }
    
    //そこから上へ探索
    roop_flag = 0;
    k = 0;
    while(roop_flag != 1){
        for(int i = 0; i < tagu.size() ; i++){
            for(int j = 0; j < tagu.at(0).size() ; j++){
                if(!( i == j)){
                    //[i][j] up
                    ser_pos = tagu[i][j].getedge_up_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 1;
                    }
                    
                    //[i][j] bottom
                    ser_pos = tagu[i][j].getedge_bottom_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 2;
                    }
                    
                    //[i][j] right
                    ser_pos = tagu[i][j].getedge_right_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 3;
                    }
                    
                    //[i][j] left
                    ser_pos = tagu[i][j].getedge_left_r();
                    competive = Comp(ser_pos, search,size_of_pixel);
                    if(low_competive > competive){
                        low_competive = competive;
                        bestpos.x = j;
                        bestpos.y = i;
                        searchDirect = 4;
                    }
                }
            }
        }
        
        cout << low_competive << " " << bestpos.x << bestpos.y << endl;
        
        if(low_competive < Point){
            if(searchDirect == 1){ search = tagu[bestpos.y][bestpos.x].getedge_bottom(); }
            else if(searchDirect == 2){ search = tagu[bestpos.y][bestpos.x].getedge_up(); }
            else if(searchDirect == 3){ search = tagu[bestpos.y][bestpos.x].getedge_left(); }
            else if(searchDirect == 4){ search = tagu[bestpos.y][bestpos.x].getedge_right(); }
            select = bestpos;
            
        }else{
            roop_flag = 1;
            cout << "roop stop   :  looking edge block "<<endl;
        }
        low_competive = MOD;
        k++;
        if(k >= gridy){
            roop_flag = 1;
            cout << "roop stop  :  max weight " <<endl;
        }
    }
    //overにいれて3つの値を返す
    over leftup_pos = {select.x,select.y,searchDirect};
    return leftup_pos;
}

over comp_2_comp(vector<vector<SplitBlock>> tagu , int size_of_pixel){
    
    
    //一番いいブロックの座標とdirection
    over good;
    return good;
}

//左上を再起処理で探します。
over search_corner_re(vector<vector<SplitBlock>> tagu , int size_of_pixel , int gridx , int gridy){
    
    int competive = 0; //Compの引数を保存
    int low_competive = MOD; //Compの最小引数比較用
    int searchDirect = 0; //探索用の向き保存Flag 1 2 3 4
    searchpoint select = {0,0}; //現在の探索場所
    searchpoint bestpos = {0,0}; //もっともよいブロックの座標を保存
    
    vector<pixel10> ser_pos; //比較先のedge
    vector<pixel10> search = tagu[0][0].getedge_left(); //現在探索してる辺を入れる
    
    int roop_flag = 0;//端が見つかったら1
    int k = 0;
    
    
    
    over leftup_pos = {select.x,select.y,searchDirect};
    return leftup_pos;
}

//内部を探索します
vector<vector<over>> search_other(vector<vector<SplitBlock>> tagu , int size_of_pixel , int gridx , int gridy ,over pos){
    
    int competive = 0; //Compの引数を保存
    int low_competive = MOD; //Compの最小引数比較用
    int searchDirect = 0; //探索用の向き保存Flag 1 2 3 4
    searchpoint select = {pos.x,pos.y}; //現在の探索場所
    searchpoint bestpos = {0,0}; //もっともよいブロックの座標を保存
    
    vector<pixel10> ser_pos; //比較先のedge
    vector<pixel10> search;
    
    //現在探索してる辺を回転数に応じて入れる
    
    tagu[pos.y][pos.x].setdirection(pos.direct);
    tagu[pos.y][pos.x].setcomp(1);
    if(pos.direct == 0){
        search = tagu[pos.y][pos.x].getedge_right();
    }else if(pos.direct == 1){
        search = tagu[pos.y][pos.x].getedge_up();
    }else if(pos.direct == 2){
        search = tagu[pos.y][pos.x].getedge_left();
    }else if(pos.direct == 3){
        search = tagu[pos.y][pos.x].getedge_bottom();
    }
    
    vector<vector<over>> temp(gridy,vector<over>(gridx));
    rep(i,gridy) rep(j,gridx) temp[i][j] = {-1,-1,0}; //初期化
    temp[0][0] = pos;//左上の座標と向きをtempに入れる
    
    cout << " 1 " << endl;
    draw_searchdata(temp);
    draw_original_comp(tagu);
    //縦に探索 それをtempにいれてsearchdataにいれなおす。
    
    for(int k = 1 ; k < tagu.at(0).size() ; k++){
        competive = 0;
        low_competive = MOD;
        
        for(int i = 0 ; i < tagu.size() ; i++){
            for(int j = 0 ; j < tagu.at(0).size() ; j++){
                
                if(tagu[i][j].getcomp() == 0){
                    if (!(i == select.y && j == select.x)){
                        
                        //[i][j] up
                        ser_pos = tagu[i][j].getedge_up_r();
                        competive = Comp(ser_pos, search ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {j,i};
                            searchDirect = 1;
                        }
                        
                        //[i][j] bottom
                        ser_pos = tagu[i][j].getedge_bottom_r();
                        competive = Comp(ser_pos, search ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {j,i};
                            searchDirect = 2;
                        }
                        
                        //[i][j] right
                        ser_pos = tagu[i][j].getedge_right_r();
                        competive = Comp(ser_pos, search ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {j,i};
                            searchDirect = 3;
                        }
                        
                        //[i][j] left
                        ser_pos = tagu[i][j].getedge_left_r();
                        competive = Comp(ser_pos, search ,size_of_pixel);
                        if(low_competive > competive){
                            low_competive = competive;
                            bestpos = {j,i};
                            searchDirect = 4;
                        }
                    }
                }
            }
        }
        
        
        tagu[bestpos.y][bestpos.x].setcomp(1); //もう使えませーん
        
        cout << k << " lll " << bestpos.x << " " << bestpos.y << " " << low_competive << endl;
        //taguに値をいれておく。  //向き
        if(searchDirect == 1){  tagu[bestpos.y][bestpos.x].setdirection(3); }
        else if(searchDirect == 2){ tagu[bestpos.y][bestpos.x].setdirection(1); }
        else if(searchDirect == 3){ tagu[bestpos.y][bestpos.x].setdirection(2); }
        else if(searchDirect == 4){ tagu[bestpos.y][bestpos.x].setdirection(0); }
        
        temp[0][k] = {bestpos.x,bestpos.y,tagu[bestpos.y][bestpos.x].getdirection()};//tempへ
        
        
        //searchを反対がわに入れてk++
        if(searchDirect == 1){  search = tagu[bestpos.y][bestpos.x].getedge_bottom();  }
        else if(searchDirect == 2){ search = tagu[bestpos.y][bestpos.x].getedge_up(); }
        else if(searchDirect == 3){ search = tagu[bestpos.y][bestpos.x].getedge_left();  }
        else if(searchDirect == 4){ search = tagu[bestpos.y][bestpos.x].getedge_right(); }
        
        //探索ブロックを一個右へ
        select = bestpos;
        
    }
    cout << " 2 " << endl;
    draw_searchdata(temp);
    //        draw_original_comp(tagu);
    
    //
    //temp [i][n] i = 0 ~ maxまで探索
    //
    
    select = {0,0}; //現在の探索場所
    bestpos = {0,0};
    
    for(int k = 0; k < tagu.at(0).size(); k++){
        
        if(tagu[temp[0][k].y][temp[0][k].x].getdirection() == 0){ search = tagu[temp[0][k].y][temp[0][k].x].getedge_bottom(); }
        else if(tagu[temp[0][k].y][temp[0][k].x].getdirection() == 1){  search = tagu[temp[0][k].y][temp[0][k].x].getedge_right(); }
        else if(tagu[temp[0][k].y][temp[0][k].x].getdirection() == 2){  search = tagu[temp[0][k].y][temp[0][k].x].getedge_up(); }
        else if(tagu[temp[0][k].y][temp[0][k].x].getdirection() == 3){  search = tagu[temp[0][k].y][temp[0][k].x].getedge_left(); }
        
        printf("Direction = %d\n", tagu[temp[0][k].y][temp[0][k].x].getdirection());
        
        for(int p = 0; p < tagu.size()-1;p++){
            //          for(int k = 0; k < 1;k++){
            
            competive = 0;
            low_competive = MOD;
            
//            if(tagu[p][k].getdirection() == 0){ search = tagu[p][k].getedge_bottom(); }
//            else if(tagu[p][k].getdirection() == 1){  search = tagu[p][k].getedge_right(); }
//            else if(tagu[p][k].getdirection() == 2){  search = tagu[p][k].getedge_up(); }
//            else if(tagu[p][k].getdirection() == 3){  search = tagu[p][k].getedge_left(); }
            
            for(int i = 0 ; i < tagu.size() ; i++){
                for(int j = 0; j < tagu.at(0).size(); j++){
//            for(int i = tagu.size()-1 ; i >= 0 ; i--){
//                for(int j = tagu.at(0).size()-1 ; j >= 0 ; j--){
                    
                    if(tagu[i][j].getcomp() == 0 ){
                        if( !(i == select.y && j == select.x )){
                            
                            ser_pos = tagu[i][j].getedge_up_r();
                            competive = Comp(ser_pos, search ,size_of_pixel);
                            
                            if(low_competive > competive){
                                low_competive = competive;
                                bestpos = {j,i};
                                searchDirect = 0;
                            }
                            
                            ser_pos = tagu[i][j].getedge_bottom_r();
                            competive = Comp(ser_pos, search ,size_of_pixel);
                            if(low_competive > competive){
                                low_competive = competive;
                                bestpos = {j,i};
                                searchDirect = 2;
                            }
                            
                            ser_pos = tagu[i][j].getedge_right_r();
                            competive = Comp(ser_pos, search ,size_of_pixel);
                            if(low_competive > competive){
                                low_competive = competive;
                                bestpos = {j,i};
                                searchDirect = 3;
                            }
                            
                            ser_pos = tagu[i][j].getedge_left_r();
                            competive = Comp(ser_pos, search ,size_of_pixel);
                            if(low_competive > competive){
                                low_competive = competive;
                                bestpos = {j,i};
                                searchDirect = 1;
                            }
                        }
                    }
                    //i j 探索おわり
                }
            }
//            if(!(p == 0))
            {
                tagu[bestpos.y][bestpos.x].setcomp(1); //もう使えませーん
                
                
                //taguに値をいれておく。  //向き
                if(searchDirect == 0){  tagu[bestpos.y][bestpos.x].setdirection(0); }
                else if(searchDirect == 1){ tagu[bestpos.y][bestpos.x].setdirection(1); }
                else if(searchDirect == 2){ tagu[bestpos.y][bestpos.x].setdirection(2); }
                else if(searchDirect == 3){ tagu[bestpos.y][bestpos.x].setdirection(3); }
                
                temp[p+1][k] = {bestpos.x,bestpos.y,tagu[bestpos.y][bestpos.x].getdirection()};//tempへ
                
                
                //searchを反対がわに入れて
                if(searchDirect == 0){  search = tagu[bestpos.y][bestpos.x].getedge_bottom(); }
                else if(searchDirect == 2){ search = tagu[bestpos.y][bestpos.x].getedge_up(); }
                else if(searchDirect == 3){ search = tagu[bestpos.y][bestpos.x].getedge_left();  }
                else if(searchDirect == 1){ search = tagu[bestpos.y][bestpos.x].getedge_right(); }
                
                select = bestpos;
            }
        }
    }
    return temp;
}

//本関数
vector<vector<over>> main_search(vector<vector<SplitBlock>> tagu , int size_of_pixel , int gridx , int gridy){
    //左上Search
    cout << "check comp " << Comp(tagu[0][0].getedge_up(), tagu[0][0].getedge_up(), size_of_pixel) << endl;
    cout << "check comp " << Comp(tagu[0][0].getedge_bottom(), tagu[0][0].getedge_bottom(), size_of_pixel) << endl;
    cout << "check comp " << Comp(tagu[0][0].getedge_left(), tagu[0][0].getedge_left(), size_of_pixel) << endl;
    cout << "check comp " << Comp(tagu[0][0].getedge_right(), tagu[0][0].getedge_right(), size_of_pixel)<< endl;
    
    cout << "check comp " << Comp(tagu[0][1].getedge_right(), tagu[0][2].getedge_left_r(), size_of_pixel)<< endl;
    
    
    
    cout << " 1blocksize " << tagu[0][0].getSize() << endl;
    over corner_pos = search_corner(tagu ,size_of_pixel ,gridx ,gridy);
    corner_pos = {8,0,0};
    //temp作成 & 初期化
    vector<vector<over>> temp(gridy,vector<over>(gridx));
    rep(i,gridy) rep(j,gridx) temp[i][j] = {-1,-1,0}; //初期化
    
    //残りを探索しtempに代入
    temp = search_other(tagu , size_of_pixel , gridx , gridy , corner_pos);
    
    cout << "TEMP :" <<endl;
    draw_searchdata(temp);
    
    //tempをsearchdataに戻り値の形に入れ替える。
    vector<vector<over>> searchdata = swap_searchdata(temp,gridy,gridx); // return data
    
    cout << "result : " << endl;
    draw_searchdata(searchdata);
    
    return searchdata;
}


//初期値を入れる。
vector<vector<SplitBlock>> input_tagu(vector<vector<pixel10>> data , int size_of_pixel , int gridx , int gridy){
    
    vector<vector<SplitBlock>> tagu(gridy,vector<SplitBlock>(gridx));
    
    vector<vector<pixel10>> num(size_of_pixel,vector<pixel10>(size_of_pixel));//代入用配列
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
    }
    return tagu;
}

//
//ここから下は画像データの読み取りに使うGUI担当の方が作成したプログラム。
//必要に応じて変えて行くが、マージするときにはメインのプロジェクトにすでにあるものなので追加しない。
//

//pixel16
struct pixel16{
    string r;
    string g;
    string b;
};

//header read
void read_txtheader(string *magic_number,int *gridx,int *gridy,
                    int *select_limit,int *select_rate,int *change_rate,
                    int *picture_width,int *picture_height,int *max_light,
                    int *grid_size){ //受け取ったテキストファイルのヘッダーを読み込みます
    
    ifstream file("fujisan.txt");
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

//text read
vector<vector<pixel10>> read_txtdata(int width,int height){ //データを読み込みます
    vector<vector<pixel16>> data16(height,vector<pixel16>(width));//16進数でRGB値を保存
    vector<vector<pixel10>> data10(height,vector<pixel10>(width));//10進数でRGB値を保存
    std::ifstream file("fujisan.txt");
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
