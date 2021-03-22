#include "tabu.h"
#include "risuto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// リストノード。
typedef struct _noodo Noodo;
struct _noodo {

    Noodo* mae;
    Noodo* tsugi;
    Tabu* kontentsu;

};

// リスト。
struct _risuto {

    int saizu;
    Noodo* kaishi;

};

// リストを作成する。
Risuto* risuto_sakusei () {

     // メモリを割り当てる。
    Risuto* atarashii_risuto = NULL;
    atarashii_risuto = (Risuto*)malloc(sizeof(Risuto));
    if(atarashii_risuto == NULL)
        return NULL;

    // リストを初期化する。
    atarashii_risuto->saizu = 0;
    atarashii_risuto->kaishi = NULL;

    return atarashii_risuto;

}

// リストを削除する。
void risuto_sakujo (Risuto** risuto) {

    // ノードとそのコンテンツをクリアする。
    Noodo* tenpi_noodo = (*risuto)->kaishi;
    Noodo* tsugi_noodo;
    while(tenpi_noodo != NULL) {

        tsugi_noodo = tenpi_noodo->tsugi;

        free(tenpi_noodo->kontentsu);
        free(tenpi_noodo);

        tenpi_noodo = tsugi_noodo;
    }

    free(*risuto);
    *risuto = NULL;

}

// リストを挿入する。
int risuto_sounyou (Risuto* risuto, Tabu* tabu) {

    // リストが「null」の場合「return」。
    if(risuto == NULL)
        return 2;

    // メモリを割り当てる。
    Noodo* atarashii_noodo = NULL;
    atarashii_noodo = (Noodo*)malloc(sizeof(Noodo));
    if(atarashii_noodo == NULL)
        return 1;

    // 新しいヌードを初期化する。
    atarashii_noodo->kontentsu = tabu;
    atarashii_noodo->tsugi = NULL;

    // 新しいノードをリストの最後に挿入する。
    if(risuto-> saizu > 0) {
        Noodo* tenpi_noodo = risuto->kaishi;
        while(tenpi_noodo->tsugi != NULL)
            tenpi_noodo = tenpi_noodo->tsugi;
        atarashii_noodo->mae = tenpi_noodo;
        tenpi_noodo->tsugi = atarashii_noodo;
    } else {
        atarashii_noodo->mae = NULL;
        risuto->kaishi = atarashii_noodo;
    }

    risuto->saizu++;

    return 0;

}

//　リストの先頭をポップする。
Tabu* risuto_poppufuronto (Risuto* risuto) {

    // リストが「null」の場合「return」。
    if(risuto == NULL)
        return NULL;

    // リストが空の場合「return」。
    if(risuto->saizu < 1)
        return NULL;

    Noodo* tenpi_noodo = risuto->kaishi;

    // 最初の要素を取得する。
    Tabu* tabu_return = tenpi_noodo->kontentsu;

    // リストを更新する。
    if(tenpi_noodo->tsugi != NULL) {
        tenpi_noodo->tsugi->mae = NULL;
        risuto->kaishi = tenpi_noodo->tsugi;
    } else risuto->kaishi = NULL;
    free(tenpi_noodo);
    risuto->saizu--;

    // 要素を「return」。
    return tabu_return;

}

// パラメータに基づくブケット・ソートする。
int baketto_sooto(Risuto* risuto, char parameeta) {

    Risuto** baketto = NULL;
    unsigned baketto_saizo = 0;

    // バケットサイズを取得する。
    if(parameeta == 'h') baketto_saizo = 32;
    else if (parameeta == 'g') baketto_saizo = 13;
    else if (parameeta == 'j') baketto_saizo = 24;
    else if (parameeta == 'b') baketto_saizo = 60;

    // 必要なバケットを作成する。
    baketto = (Risuto**)malloc(baketto_saizo * sizeof(Risuto*));
    if(baketto == NULL)
        return 1;
    // バケットを初期化する。
    for(int i = 0; i < baketto_saizo; i++) {
        baketto[i] = risuto_sakusei();
        if(baketto[i] == NULL)
            return 1;
    }
    
    // リストをバケツに空ける。
    Tabu* tenpi_tabu = NULL;
    while(risuto->saizu > 0) {

        tenpi_tabu = risuto_poppufuronto(risuto);   
        if(tenpi_tabu == NULL)
            return 0;

        if(parameeta == 'h') {
            if(risuto_sounyou(baketto[hi_eru(tenpi_tabu)], tenpi_tabu) != 0)
                return 1;
        } else if (parameeta == 'g') {
            if(risuto_sounyou(baketto[gatsu_eru(tenpi_tabu)], tenpi_tabu) != 0)
                return 1;
        } else if (parameeta == 'j'){
            if(risuto_sounyou(baketto[jikan_eru(tenpi_tabu)], tenpi_tabu) != 0)
                return 1;
        } else if (parameeta == 'b') {
            if(risuto_sounyou(baketto[bun_eru(tenpi_tabu)], tenpi_tabu) != 0)
                return 1;
        }
    }

    // バケツをリストに空ける。
    for(int i = 0; i < baketto_saizo; i++) {
        while(baketto[i]->saizu > 0) {

            tenpi_tabu = risuto_poppufuronto(baketto[i]);

            if(tenpi_tabu == NULL)
                return 1;

            if(risuto_sounyou(risuto, tenpi_tabu) != 0)
                return 1;
        }
    }

    // バケットメモリをクリアする。
    for(int i = 0; i < baketto_saizo; i++)
        risuto_sakujo(baketto+i);
    free(baketto);

    return 0;

}

// リストを移動する。
int risuto_sooto (Risuto* risuto) {

    // リストが「null」の場合「return」。
    if(risuto == NULL)
        return 2;

    // リストが十分に大きくない場合「return」。
    if(risuto->saizu < 2)
        return 0;

    //　リストに「radix」ソートする。
    if(baketto_sooto(risuto, 'b') != 0) return 1;
    if(baketto_sooto(risuto, 'j') != 0) return 1;
    if(baketto_sooto(risuto, 'h') != 0) return 1;
    if(baketto_sooto(risuto, 'g') != 0) return 1;
    
   
    

    return 0;

}

// リストを移動する。
int risuto_idou(Risuto* risuto, char* tagetto_taitoru, int tagetto_pojishon) {

    // リストが「null」の場合「return」。
    if(risuto == NULL)
        return 2;

    if(risuto->saizu <= 1)
        return 0;

    // タゲット・ポジションクランプする
    if(tagetto_pojishon < 1)
        tagetto_pojishon = 1;

    if(tagetto_pojishon > risuto->saizu)
        tagetto_pojishon = risuto->saizu;

    // ターゲット・ノードとターゲット・ポジションを見つける。
    int pojishon_kaunta = 1;
    Noodo* tenpi_noodo = risuto->kaishi;
    Noodo* tagetto_noodo = NULL;
    while(tagetto_noodo == NULL && tenpi_noodo != NULL) {
    
        if(tagetto_noodo == NULL && strcmp(taitoru_eru(tenpi_noodo->kontentsu), tagetto_taitoru) == 0)
            tagetto_noodo = tenpi_noodo;
        else {
            pojishon_kaunta++;
            tenpi_noodo = tenpi_noodo->tsugi;
        }

    }

    // ターゲット・タイトルを持つノードが存在しない場合 「return」。
    if(tagetto_noodo == NULL)
        return 0;

    // ノードと一緒に戻る。
    while(pojishon_kaunta > tagetto_pojishon) {

        if(tagetto_noodo->tsugi != NULL)
            tagetto_noodo->tsugi->mae = tagetto_noodo->mae;

        tagetto_noodo->mae->tsugi = tagetto_noodo->tsugi;
        tagetto_noodo->tsugi = tagetto_noodo->mae;

        if(tagetto_noodo->mae->mae != NULL)
            tagetto_noodo->mae->mae->tsugi = tagetto_noodo;
        
        tagetto_noodo->mae = tagetto_noodo->mae->mae;

        tagetto_noodo->tsugi->mae = tagetto_noodo;            

        pojishon_kaunta--;
        if(pojishon_kaunta == 1)
            risuto->kaishi = tagetto_noodo;

    }

    // ノードと一緒に前に行く。
    while(pojishon_kaunta < tagetto_pojishon) {

        if(pojishon_kaunta == 1)
            risuto->kaishi = tagetto_noodo ->tsugi;

        if(tagetto_noodo->mae != NULL)
            tagetto_noodo->mae->tsugi = tagetto_noodo->tsugi;

        tagetto_noodo->tsugi->mae = tagetto_noodo->mae;
        tagetto_noodo->mae = tagetto_noodo->tsugi;

        if(tagetto_noodo->tsugi->tsugi != NULL)
            tagetto_noodo->tsugi->tsugi->mae = tagetto_noodo;
        
        tagetto_noodo->tsugi = tagetto_noodo->tsugi->tsugi;

        tagetto_noodo->mae->tsugi = tagetto_noodo; 

        pojishon_kaunta++;

    }

    return 0;
    
}

// リストを印刷する。 
int risuto_insatsu (Risuto* risuto) {
    
    // リストが「null」の場合「return」。
    if(risuto == NULL)
        return 2;

    // 各ノードのコンテンツを印刷する
    Noodo* tenpi_noodo = risuto->kaishi;
    while(tenpi_noodo != NULL) {
        tabu_insatsu(tenpi_noodo->kontentsu);
        tenpi_noodo = tenpi_noodo->tsugi;
    }

    putchar('\n');

    return 0;

}