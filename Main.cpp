#include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include <unistd.h>
#include "draw.hpp"
#include "splitClass.hpp" //class
#include "func.hpp"

using namespace std;

int Main()
{
    //一番大事な核となる変数
    string magic_number; //マジックナンバー
    int gridx;//分割数
    int gridy;
    int select_limit;//選択可能回数
    int select_rate;//選択コスト変換レート
    int change_rate;//交換コスト変換レート
    int picture_width;//画像の縦横の大きさ
    int picture_height;
    int max_light; //最大輝度値
    int grid_size; //分割画像の一辺のサイズ
    int num_of_block;//分割された画像の数gridx*gridy
    int cursor_x=0;//クリックした際のx座標
    int cursor_y=0;//クリックした際のy座標
    int grid_img_x=0;//分割した画像の左上のx座標
    int grid_img_y=0;//分割した画像の左上のy座標
    
    //2個選択する"交換モード"で使用する変数
    int change_select_1=100000;
    int change_select_2=100000;
    int change_select_1_grid_img_x=0;
    int change_select_1_grid_img_y=0;
    
    //モードを管理する変数
    bool select_judge=0;//選択しているかの判定
    bool change_judge=0;//交換モードかの判定
    bool sorting_judge=1;//並べ替えモードかの判定
    bool answer_judge=0;//確認モードかの判定
    bool answer_judge_1=0;//回答確認モードかの判定
    bool top_left_judge=0;//左上特定モードかの判定
    
    //現画像復元に使う変数
    int x=0;
    int y=0;
    int ra=0;
    int select_x;
    int select_y;
    over top_left={0,0,0};//原画像の左上の画像の情報
    vector<vector<over>> searchdata;//推測関数からもらうデータ

    //答え確認モードで使う変数
    int count_i=0;
    int count_j=0;
    
    //選択している画像がどれなのか表示してくれる変数
    int select_grid=-1;//選択した分割画像の番号
    
    //Siv3Dで描画する際に使うクラス
    DynamicTexture texture_tmp;//分割画像を移動させる際に使うテンプ
    Image image_tmp;//画像内の座標と数値を入れ替える際に使うテンプ
    vector<DynamicTexture> block_grid;//分割後の画像を管理するクラス
    vector<DynamicTexture> block_grid_tmp;//交換する際に使用するテンプ
    vector<DynamicTexture> block_grid_default;//
    vector<Image> block;//分割画像を保存するvector配列
    vector<Image> block_tmp;//交換する際に使用するテンプ
    vector<Image> block_default;//最初のデータを保存するデフォルト
    
    //その他
    const Font font(15, Typeface::Bold);//GUIに表示するフォントの定義
    int space = 0;//分割画像ごとの幅
    
    
    read_txtheader(&magic_number,&gridx,&gridy,&select_limit,
                   &select_rate,&change_rate,&picture_width,
                   &picture_height,&max_light,&grid_size);//ヘッダーを読み取る関数
    
    vector<vector<pixel10>> data(picture_width,vector<pixel10>(picture_height));//main関数内でデータを保存する
    data = read_txtdata(picture_width, picture_height);//読み取ったデータを保存
    
    int windowsize_x=picture_width+gridx*40;//ウィンドウのサイズを計算
    int windowsize_y=picture_height+40;
    if(windowsize_x<1300){//画像サイズが小さいとボタンとかぶっていたため、ウィンドウサイズ最低値を設定
        windowsize_x=1300;
    }
    if(windowsize_y<1000){
        windowsize_y=1000;
    }//1300*1000はmac13インチでいい感じになる
    Window::SetStyle(WindowStyle::Sizable);

    Window::Resize(windowsize_x, windowsize_y);//ウィンドウ生成
    Scene::SetBackground(Color(40, 40, 40));//背景色設定

    Image image(picture_width, picture_height, Palette::White);//分割前の画像の作成
    
    DynamicTexture texture(image);//分割前の画像を管理するクラス
    
    //答え確認モードの処理by小島
    vector<vector<Ido>> mov(gridx, vector<Ido>(gridy));
    rep(y, gridy) rep(x, gridx){
        mov[x][y].gox = x;
        mov[x][y].goy = y;
        mov[x][y].rot = 0;
    }
        
    ifstream an("solution.txt");
    int ansc;
    string anss;
    char ansh;
    an >> anss >> ansc;
    vector<answer> ans(ansc);
    rep(i, ansc) {
        an >> ans[i].xy >> ans[i].r >> ans[i].s;
        ans[i].x = xh(ans[i].xy[0]);
        ans[i].y = xh(ans[i].xy[1]);
    }
    //
    
    num_of_block=gridx*gridy;//分割画像数の計算
    
    for(int i=0;i<num_of_block;i++){
        block.emplace_back(grid_size, grid_size, Palette::White);//vector配列を一個ずつ拡張
        block_tmp.emplace_back(grid_size, grid_size, Palette::White);
        block_default.emplace_back(grid_size, grid_size, Palette::White);
    }
    
    //tagu関数
    vector<vector<SplitBlock>> tagu = input_tagu( data , grid_size , gridx , gridy);
    //
    
    for(int i_y=0;i_y<gridy;i_y++){
        for(int i_x=0;i_x<gridx;i_x++){
            for(int y=0;y<picture_height/gridy;y++){
                for(int x=0;x<picture_width/gridx;x++){
                    block[i_y*gridx+i_x][y][x]=Color(data[y + grid_size*i_y][x+grid_size*i_x].r,
                                                     data[y + grid_size*i_y][x+grid_size*i_x].g,
                                                     data[y + grid_size*i_y][x+grid_size*i_x].b);
                    block_default[i_y*gridx+i_x][y][x]=Color(data[y + grid_size*i_y][x+grid_size*i_x].r,
                                                     data[y + grid_size*i_y][x+grid_size*i_x].g,
                                                     data[y + grid_size*i_y][x+grid_size*i_x].b);
                    //画像データを分割して保存する
                }
            }
            block_grid.emplace_back(block[i_y*gridx+i_x]);//vector配列を一個ずつ拡張
            block_grid_tmp.emplace_back(block[i_y*gridx+i_x]);
            block_grid_default.emplace_back(block[i_y*gridx+i_x]);
        }
    }
    while (System::Update())//Siv3Dでの動作を入れる
    {
        if(SimpleGUI::Button(U"分割", Vec2(picture_width+gridx*10, 20))){
            space=3;//分割ボタンを押したときの処理
        }
        if(SimpleGUI::Button(U"結合", Vec2(picture_width+gridx*10, 60))){
            space=0;//結合ボタンを押したときの処理
        }
        
        if(SimpleGUI::Button(U"選択解除", Vec2(picture_width+gridx*10, 100))){
            select_judge=0;//選択解除ボタンを押したときの処理
            select_grid=-1;
            change_judge=0;
            sorting_judge=0;
        }
        
        if(SimpleGUI::Button(U"交換モード", Vec2(picture_width+gridx*10, 140))){
            select_judge=0;//選択解除ボタンを押したときの処理
            select_grid=-1;
            change_judge=1;
            sorting_judge=0;
        }
        
        if(SimpleGUI::Button(U"並べ替えモード", Vec2(picture_width+gridx*10, 180))){
            sorting_judge=1;
            change_judge=0;
            select_judge=0;
            select_grid=-1;
        }
        
        if(SimpleGUI::Button(U"左上予想", Vec2(picture_width+gridx*10, 220))){
            top_left_judge=1;
            select_judge=0;
            sorting_judge=1;
            searchdata = search_00(tagu,grid_size,gridx,gridy);
            for(int k=0;k<num_of_block;k++){
                for(int i = 0; i < gridy; i++){
                    for(int j = 0; j < gridx; j++){
                        if(searchdata[i][j].x==k%gridx&&searchdata[i][j].y==k/gridx){
                            x=j;
                            y=i;
                            ra=searchdata[i][j].direct;
                        }
                    }
                }
                block_grid_tmp[k]=block_grid[x+y*gridx];
                block_tmp[k]=block[x+y*gridx];
                for(int h=0;h<ra;h++){
                    block_grid_tmp[k].fill(block_tmp[k].rotate90());
                }
                x=0;
                y=0;
                ra=0;
            }
            for(int k=0;k<num_of_block;k++){
                block_grid[k]=block_grid_tmp[k];
                block[k]=block_tmp[k];
            }
        }
       
        if(SimpleGUI::Button(U"左上を特定", Vec2(picture_width+gridx*10, 260))){
            if(top_left_judge&&select_grid!=-1){
                select_x=select_grid%gridx;
                select_y=select_grid/gridx;
                cout << select_x << select_y<<endl;
                
                for(int i = 0; i < gridy; i++){
                    for(int j = 0; j < gridx; j++){
                        if(searchdata[i][j].x==select_x&&searchdata[i][j].y==select_y){
                            x=j;
                            y=i;
                            ra=searchdata[i][j].direct;
                        }
                    }
                }
                top_left.x=x;
                top_left.y=y;
                top_left.direct=ra;
                x=0;
                y=0;
                ra=0;
                cout << "top_left" << top_left.x << top_left.y << top_left.direct << endl;
                top_left_judge=0;
            }
        }
        if(SimpleGUI::Button(U"原画像予想", Vec2(picture_width+gridx*10, 300))){
            searchdata = main_search(tagu,grid_size,gridx,gridy,top_left);
            draw_searchdata(searchdata);
            /*for(int i = 0; i < gridy; i++){
                for(int j = 0; j < gridx; j++){
                    cout << "{" << searchdata[i][j].x << "," << searchdata[i][j].y << "," << searchdata[i][j].direct << "}";
                }
            }*/
            
            for(int k=0;k<num_of_block;k++){
                for(int i = 0; i < gridy; i++){
                    for(int j = 0; j < gridx; j++){
                        if(searchdata[i][j].x==k%gridx&&searchdata[i][j].y==k/gridx){
                            x=j;
                            y=i;
                            ra=searchdata[i][j].direct;
                        }
                    }
                }
                block_grid_tmp[k]=block_grid[x+y*gridx];
                block_tmp[k]=block[x+y*gridx];
                for(int h=0;h<ra;h++){
                    block_grid_tmp[k].fill(block_tmp[k].rotate90());
                }
                x=0;
                y=0;
                ra=0;
            }
            for(int k=0;k<num_of_block;k++){
                block_grid[k]=block_grid_tmp[k];
                block[k]=block_tmp[k];
            }
            select_grid=-1;
        }
        
        if(SimpleGUI::Button(U"初期化", Vec2(picture_width+gridx*10, 340))){
            for(int i=0;i<num_of_block;i++){
                block[i]=block_default[i];
                block_grid[i]=block_grid_default[i];
                block_grid[i].fill(block[i].rotate90());
                block_grid[i].fill(block[i].rotate90());
                block_grid[i].fill(block[i].rotate90());
                block_grid[i].fill(block[i].rotate90());
            }
            tagu=input_tagu( data , grid_size , gridx , gridy);
        }
        
        font(U"選択している座標:{}, {}"_fmt(select_grid%gridx, select_grid/gridx, Window::ClientSize())).draw(picture_width+gridx*10, 375, Color(255, 255, 255));
        
        if(SimpleGUI::Button(U"確認モード", Vec2(picture_width+gridx*10, 450))){
            sorting_judge=0;
            change_judge=0;
            answer_judge=1;
            select_judge=0;
            select_grid=-1;
        }
        
        for(int i_y=0;i_y<gridy;i_y++){
            for(int i_x=0;i_x<gridx;i_x++){
                block_grid[i_y*gridx+i_x].draw(i_x*grid_size+i_x*space, i_y*grid_size+i_y*space);//分割画像描画
            }
        }
        if(sorting_judge){
            if(MouseL.down()){//分割画像をクリックして選択した際の表示
                for(int i_y=0;i_y<gridy;i_y++){
                    for(int i_x=0;i_x<gridx;i_x++){
                        cursor_x=Cursor::Pos().x;//カーソルの座標を代入
                        cursor_y=Cursor::Pos().y;
                        if(cursor_x >= i_x*grid_size+i_x*space && cursor_x <= (i_x+1)*grid_size+i_x*space &&
                           cursor_y >= i_y*grid_size+i_y*space && cursor_y <= (i_y+1)*grid_size+i_y*space){//判定を計算
                            grid_img_x=i_x*grid_size+i_x*space;//分割画像の左上のx,y座標を代入
                            grid_img_y=i_y*grid_size+i_y*space;
                            select_grid=i_y*gridx+i_x;//どの分割画像を選んだのか保存する
                        }
                    }
                }
                select_judge=1;
            }
        }
        if(select_judge){
            Rect(grid_img_x+10, grid_img_y+10, grid_size-20, grid_size-20)
            .drawFrame(4, 0, Palette::Black);
            if(KeyW.down()){//上キーを押したとき
                if(select_grid-gridx<0){//一番上の画像を選択した際の処理
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+gridx*(gridy-1)];
                    block_grid[select_grid+gridx*(gridy-1)]=texture_tmp;//①swap関数のようにTextureクラスの画像と画像を交換
                                
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+gridx*(gridy-1)];
                    block[select_grid+gridx*(gridy-1)]=image_tmp;//②こちらも同じくImageクラス上で交換
                                
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][gridy-1]);
                    select_grid+=gridx*(gridy-1);//③選択している画像の番号を変換
                }else{//一番上の画像以外なら
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-gridx];
                    block_grid[select_grid-gridx]=texture_tmp;//①
                                
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-gridx];//②
                    block[select_grid-gridx]=image_tmp;
                                
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][select_grid/gridx-1]);
                    select_grid-=gridx;//③
                }
            }
            if(KeyS.down()){//下キーを押したとき
                if(select_grid+gridx>num_of_block-1){
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-gridx*(gridy-1)];
                    block_grid[select_grid-gridx*(gridy-1)]=texture_tmp;
                                
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-gridx*(gridy-1)];
                    block[select_grid-gridx*(gridy-1)]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][0]);
                    select_grid-=gridx*(gridy-1);
                }else{
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+gridx];
                    block_grid[select_grid+gridx]=texture_tmp;
                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+gridx];
                    block[select_grid+gridx]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][select_grid/gridx+1]);
                    select_grid+=gridx;
                }
            }
            if(KeyD.down()){//右キーを押したとき
                if((select_grid+1)%gridx==0){
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-(gridx-1)];
                    block_grid[select_grid-(gridx-1)]=texture_tmp;
                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-(gridx-1)];
                    block[select_grid-(gridx-1)]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[0][select_grid/gridx]);
                    select_grid-=(gridx-1);
                }else{
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+1];
                    block_grid[select_grid+1]=texture_tmp;
                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+1];
                    block[select_grid+1]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx+1][select_grid/gridx]);
                    select_grid++;
                }
            }
            if(KeyA.down()){//左キーを押したとき
                if(select_grid%gridx==0){
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+(gridx-1)];
                    block_grid[select_grid+(gridx-1)]=texture_tmp;
                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+(gridx-1)];
                    block[select_grid+(gridx-1)]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[gridx-1][select_grid/gridx]);
                    select_grid+=(gridx-1);
                }else{
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-1];
                    block_grid[select_grid-1]=texture_tmp;
                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-1];
                    block[select_grid-1]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx-1][select_grid/gridx]);
                    select_grid--;
                }
            }
            if(KeyX.down()){//Xキーを押したとき回転90度
                block_grid[select_grid].fill(block[select_grid].rotate90());
                mov[select_grid%gridx][select_grid/gridx].rot += 1;
                mov[select_grid%gridx][select_grid/gridx].rot %= 4;
                //Imageを90度回転させTextureクラスに保存
            }
            if(KeyZ.down()){
                block_grid[select_grid].fill(block[select_grid].rotate270());
                mov[select_grid%gridx][select_grid/gridx].rot += 3;
                mov[select_grid%gridx][select_grid/gridx].rot %= 4;
                //Imageを270度回転させTextureクラスに保存
            }
            grid_img_x=(select_grid%gridx)*(grid_size+space);//選択している画像の左上の座標を更新
            grid_img_y=(select_grid/gridx)*(grid_size+space);
        }
        
        if(change_judge){
            if(change_select_1!=100000){
                Rect(change_select_1_grid_img_x+10, change_select_1_grid_img_y+10, grid_size-20, grid_size-20)
                .drawFrame(4, 0, Palette::Black);
            }
            if(MouseL.down()){
                if(change_select_1==100000){
                    for(int i_y=0;i_y<gridy;i_y++){
                        for(int i_x=0;i_x<gridx;i_x++){
                            cursor_x=Cursor::Pos().x;//カーソルの座標を代入
                            cursor_y=Cursor::Pos().y;
                            if(cursor_x >= i_x*grid_size+i_x*space && cursor_x <= (i_x+1)*grid_size+i_x*space &&
                               cursor_y >= i_y*grid_size+i_y*space && cursor_y <= (i_y+1)*grid_size+i_y*space){//判定を計算
                                change_select_1_grid_img_x=i_x*grid_size+i_x*space;//分割画像の左上のx,y座標を代入
                                change_select_1_grid_img_y=i_y*grid_size+i_y*space;
                                change_select_1=i_y*gridx+i_x;//どの分割画像を選んだのか保存する
                            }
                        }
                    }
                }else if(change_select_2==100000){
                    for(int i_y=0;i_y<gridy;i_y++){
                        for(int i_x=0;i_x<gridx;i_x++){
                            cursor_x=Cursor::Pos().x;//カーソルの座標を代入
                            cursor_y=Cursor::Pos().y;
                            if(cursor_x >= i_x*grid_size+i_x*space && cursor_x <= (i_x+1)*grid_size+i_x*space &&
                               cursor_y >= i_y*grid_size+i_y*space && cursor_y <= (i_y+1)*grid_size+i_y*space){//判定を計算
                                change_select_2=i_y*gridx+i_x;//どの分割画像を選んだのか保存する
                            }
                        }
                    }
                }
                if(change_select_1!=100000&&change_select_2!=100000){
                    texture_tmp=block_grid[change_select_2];
                    block_grid[change_select_2]=block_grid[change_select_1];
                    block_grid[change_select_1]=texture_tmp;
                    
                    image_tmp=block[change_select_2];
                    block[change_select_2]=block[change_select_1];
                    block[change_select_1]=image_tmp;
                    
                    swap(mov[change_select_1%gridx][change_select_1/gridx],
                         mov[change_select_2%gridx][change_select_2/gridx]);
                    change_select_1=100000;
                    change_select_2=100000;
                }
            }
        }
        if(answer_judge){//答え確認モード
            if(count_j==0){
                select_grid = ans[count_i].x + ans[count_i].y * gridx;
            }
            ansh = ans[count_i].s[count_j];
            
            count_j++;
            if(count_j > ans[count_i].r){
                count_j=0;
                count_i++;
            }
            if(count_i > ansc){
                count_i=0;
                answer_judge=0;
                answer_judge_1=1;
            }
                    
            if(ansh == 'U'){//上に行くとき
                if(select_grid-gridx<0){//一番上の画像を選択した際の処理
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+gridx*(gridy-1)];
                    block_grid[select_grid+gridx*(gridy-1)]=texture_tmp;
                            
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+gridx*(gridy-1)];
                    block[select_grid+gridx*(gridy-1)]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][gridy-1]);
                    select_grid+=gridx*(gridy-1);
                }else{//一番上の画像以外なら
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-gridx];
                    block_grid[select_grid-gridx]=texture_tmp;
                                
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-gridx];
                    block[select_grid-gridx]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][select_grid/gridx-1]);
                    select_grid-=gridx;
                            
                }
            }
            if(ansh == 'D'){//下に行くとき
                if(select_grid+gridx>num_of_block-1){
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-gridx*(gridy-1)];
                    block_grid[select_grid-gridx*(gridy-1)]=texture_tmp;
                                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-gridx*(gridy-1)];
                    block[select_grid-gridx*(gridy-1)]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][0]);
                    select_grid-=gridx*(gridy-1);
                }else{
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+gridx];
                    block_grid[select_grid+gridx]=texture_tmp;
                        
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+gridx];
                    block[select_grid+gridx]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx][select_grid/gridx+1]);
                    select_grid+=gridx;
                }
            }
            if(ansh == 'R'){//右に行くとき
                if((select_grid+1)%gridx==0){
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-(gridx-1)];
                    block_grid[select_grid-(gridx-1)]=texture_tmp;
                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-(gridx-1)];
                    block[select_grid-(gridx-1)]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[0][select_grid/gridx]);
                    select_grid-=(gridx-1);
                }else{
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+1];
                    block_grid[select_grid+1]=texture_tmp;
                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+1];
                    block[select_grid+1]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx+1][select_grid/gridx]);
                    select_grid++;
                }
            }
            if(ansh == 'L'){//左に行くとき
                if(select_grid%gridx==0){
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid+(gridx-1)];
                    block_grid[select_grid+(gridx-1)]=texture_tmp;
                            
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid+(gridx-1)];
                    block[select_grid+(gridx-1)]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[gridx-1][select_grid/gridx]);
                    select_grid+=(gridx-1);
                }else{
                    texture_tmp=block_grid[select_grid];
                    block_grid[select_grid]=block_grid[select_grid-1];
                    block_grid[select_grid-1]=texture_tmp;
                                    
                    image_tmp=block[select_grid];
                    block[select_grid]=block[select_grid-1];
                    block[select_grid-1]=image_tmp;
                    
                    swap(mov[select_grid%gridx][select_grid/gridx], mov[select_grid%gridx-1][select_grid/gridx]);
                    select_grid--;
                }
            }
        }
        if(answer_judge_1){
            rep(i, anss.size()){
                rep(j, xh(anss[i])){
                    block_grid[i].fill(block[i].rotate90());
                    mov[j][i].rot += 1;
                }
            }
            answer_judge_1=0;
        }
    }
    write_movedata(mov,select_limit,select_rate,change_rate);
    write_searchdata(searchdata,select_limit,select_rate,change_rate);
    return 1;
}
