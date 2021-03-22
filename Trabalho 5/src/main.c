#include "tabu.h"
#include "risuto.h"
#include <stdio.h>

int main(void) {
    
    // リストを作成する。
    Risuto* risuto = NULL;
    risuto = risuto_sakusei();
    if(risuto == NULL) {
        printf("Memori no wariate eraa!\n");
        return 1;
    }

    // メイン・ループ。
    int sentaku = 0;
    while(sentaku != 5) {

        // ユーザー入力を受け取る。
        scanf(" %d", &sentaku);

        if(sentaku == 1) {

            // 新しいタブ情報を受け取る。
            char atarashii_taitoru[31];
            char atarashii_url[1025];
            int atarashii_hi, atarashii_gatsu, atarashii_jikan, atarashii_bun;

            scanf(" %[^\n]", atarashii_taitoru);
            scanf(" %[^\n]", atarashii_url);
            scanf(" %d %d %d %d", &atarashii_hi, &atarashii_gatsu, &atarashii_jikan, &atarashii_bun);

            //　新しいタブを作る。
            Tabu* atarashii_tabu = NULL;
            atarashii_tabu = tabu_sakusei(atarashii_taitoru, atarashii_url, 
                                            atarashii_hi, atarashii_gatsu,
                                            atarashii_jikan, atarashii_bun);
            if(atarashii_tabu == NULL) {
                printf("Memori no wariate eraa!\n");
                return 1;
            }

            // リストに新しいタブを挿入する。
            if(risuto_sounyou(risuto, atarashii_tabu) != 0) {
                printf("Memori no wariate eraa!\n");
                return 1;
            }

            continue;

        }

        if(sentaku == 2) {

            // リスト要素を移動する。
            char tagetto_taitoru[31];
            int tagetto_pojishon;
            scanf(" %[^\n] %d", tagetto_taitoru, &tagetto_pojishon);
            if(risuto_idou(risuto, tagetto_taitoru, tagetto_pojishon) != 0) {
                printf("Risuto idou eraa!\n");
                return 1;
            }

            continue;

        }

        if(sentaku == 3) {

            // リストをソートする。
            if(risuto_sooto(risuto) != 0) {
                printf("Risuto sooto eraa!\n");
                return 1;
            }

            continue;
        }

        if(sentaku == 4) {

            // リストを印刷する。
            if(risuto_insatsu(risuto) != 0) {
                printf("Risuto insatsu eraa!");
                return 1;
            }

            continue;

        }
    }

    // メモリをクリアする。
    risuto_sakujo(&risuto);

    return 0;
}
