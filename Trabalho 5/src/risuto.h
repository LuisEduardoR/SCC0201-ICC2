#ifndef RISUTO_H
#define RISUTO_H

#include "tabu.h"

// リスト。
typedef struct _risuto Risuto;

// リストを作成する。
Risuto* risuto_sakusei ();

// リストを削除する。
void risuto_sakujo (Risuto** risuto);

// リストを挿入する。
int risuto_sounyou (Risuto* risuto, Tabu* tabu);

//　リストの先頭をポップする。
Tabu* risuto_poppufuronto (Risuto* risuto);

// リストを移動する。
int risuto_sooto (Risuto* risuto);

// リストを移動する。
int risuto_idou(Risuto* risuto, char* tagetto_taitoru, int tagetto_pojishon);

// リストを印刷する。 
int risuto_insatsu (Risuto* risuto);

#endif