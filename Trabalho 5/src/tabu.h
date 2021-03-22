#ifndef TABU_H
#define TABU_H

// タブ。
typedef struct _tabu Tabu;

// タブを作成する。
Tabu* tabu_sakusei (char* taitoru, char* url, int hi, int gatsu, int jikan, int bun);
// タブを削除する。
void tabu_sakujo (Tabu** tabu);
// タブを印刷する。 
int tabu_insatsu (Tabu* tabu);

// タブのコンテンツを返す。
char* taitoru_eru(Tabu* tabu);
char* url_eru(Tabu* tabu);
int hi_eru(Tabu* tabu);
int gatsu_eru(Tabu* tabu);
int jikan_eru(Tabu* tabu);
int bun_eru(Tabu* tabu);

#endif