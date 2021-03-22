#include "tabu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// タブ。
struct _tabu {

    // タイトルを保存する。
    char taitoru[31];
    // 「UＲＬ」を保存する。
    char url[1025];

    // タブの開口部に関する情報を保存する。
    int hi;
    int gatsu;
    int jikan;
    int bun;

};

// タブを作成する。
Tabu* tabu_sakusei (char* taitoru, char* url, int hi, int gatsu, int jikan, int bun) {
    
    // メモリを割り当てる。
    Tabu* atarashii_tabu = NULL;
    atarashii_tabu = (Tabu*)malloc(sizeof(Tabu));
    if(atarashii_tabu == NULL)
        return NULL;

    // 新しいコンテンツをコピーする。
    strcpy(atarashii_tabu->taitoru, taitoru);
    strcpy(atarashii_tabu->url, url);

    // コピーする前に番号を検証する。
    if(hi < 1) hi = 1;
    else if(hi > 31) hi = 31;
    atarashii_tabu->hi = hi;

    if(gatsu < 1) gatsu = 1;
    else if(gatsu > 12) hi = 12;
    atarashii_tabu->gatsu = gatsu;

    if(jikan < 0) jikan = 0;
    else if(jikan > 23) jikan = 23;
    atarashii_tabu->jikan = jikan;

    if(bun < 0) bun = 0;
    else if(bun > 59) bun = 59;
    atarashii_tabu->bun = bun;

    return atarashii_tabu;

}

// タブを削除する。
void tabu_sakujo (Tabu** tabu) {

    free(*tabu);
    *tabu = NULL;

}

// タブを印刷する。 
int tabu_insatsu (Tabu* tabu){
    printf("%s %s %02d/%02d %02d:%02d\n", tabu->taitoru, tabu->url, 
            tabu->hi, tabu->gatsu, tabu->jikan, tabu->bun);
}

// タブのコンテンツを返す。
char* taitoru_eru(Tabu* tabu) { return tabu->taitoru; }
char* url_eru(Tabu* tabu) { return tabu->url; }
int hi_eru(Tabu* tabu) { return tabu->hi; }
int gatsu_eru(Tabu* tabu) { return tabu->gatsu; }
int jikan_eru(Tabu* tabu) { return tabu->jikan ;}
int bun_eru(Tabu* tabu) { return tabu->bun; }