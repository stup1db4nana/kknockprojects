//linux용 k.knock 포켓몬 게임 과제

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define savedef "pokesave" //세이브 위치
#define configdef "pokeconf" //설정 위치

typedef enum {FIRE, WATER, GRASS} poketype;
const char *poketypename[] = {"fire", "water", "grass"};

typedef struct {
    char *name;
    char *nickname;
    int type;
    int atk;
    int hp;
    int curhp;
} pokedef;

typedef struct {
    FILE *savedir;
    FILE *configdir;
    pokedef *charastat;
    int pokecount;
    int currentpokemon;
} gamestatus;

void characreator(gamestatus *stat);
void saveloader(gamestatus *stat);
void adventure(gamestatus *stat);
void initload(gamestatus *stat);
int mainmenu(gamestatus *stat);

void combatinit(pokedef *playerchara, pokedef *enemychara);
void replaceplayerchara(pokedef *chara, pokedef *enemychara);
void attack(pokedef *chara1, pokedef *chara2);



int main() {
    gamestatus *stat = malloc(sizeof(gamestatus));
    
    initload(stat);

    while(1) {
        switch (mainmenu(stat)) {
            case 1:
                adventure(stat);
                break;
            case 2:
                printf("bruhsave\n");
                break;
            case 3:
                printf("bruh2\n");
                break;
            case 4:
                printf("bruh3\n");
                break;
            case 5:
                printf("bruh4\n");
                break;
            default:
                printf("bruh!!!!\n");
        }
    }
    

    free(stat);
    return 0;
}



void initload(gamestatus *stat) { //새로하기, 이어하기 선택
    int validvalue = 0;
    printf("====================================\n\tK.Knock Pokemon Game\n\n\tpress enter to start\n====================================\n");
    getc(stdin);
    printf("\033[H\033[J\n===============================\n1. 새로하기\t2. 이어하기\n>>");
    

    
    while (validvalue == 0) {
        int choice;
        scanf(" %d", &choice);


        switch (choice) {
            case 1:
                printf("\033[H\033[J\n새 게임을 시작합니다...\n");
                characreator(stat);
                validvalue = 1;
                break;
            case 2:
                printf("\033[H\033[J\n세이브를 불러옵니다...\n");
                saveloader(stat);
                validvalue = 1;
                break;

            default:
                printf("\033[H\033[J\n1과 2 중 선택하십시오.\n\n===============================\n1. 새로하기\t2. 이어하기\n>>");
        }
    }
    printf("\033[H\033[J\n");

    return;
}



void characreator(gamestatus *stat) { // 세이브 생성, 포켓몬 선택
    stat->savedir = fopen(savedef, "w");
    stat->configdir = fopen(configdef, "r");
    stat->charastat = malloc(sizeof(pokedef) * 2); //포켓몬 수는 2 나중에 변경?
    char starterpokes[3][20];
    char buffer[100];

    srand(time(NULL));

    pokedef *playerchara = &stat->charastat[0];
    stat->pokecount = 1;
    fprintf(stat->savedir, "pokecount= %d\n", stat->pokecount);
    int selection;

    // starterpoke에 첫 3마리 이름 저장
    fscanf(stat->configdir, "%*d %s %*d %*d %*d %s %*d %*d %*d %s", starterpokes[0], starterpokes[1], starterpokes[2]);
    printf("===============================\n어느 포켓몬을 선택하시겠습니까?\n");
    printf("1. %s 2. %s 3. %s\n>>", starterpokes[0], starterpokes[1], starterpokes[2]);
    scanf("%d", &selection);

    // 포인터 리셋, 선택한 줄까지 이동
    rewind(stat->configdir);
    for(int i=0; i<selection; i) {
        if(fgetc(stat->configdir) == '\n') {
            i++;
        }
    } 

    // configdir에서 선택한 포켓몬 정보 읽어서 cdef에 저장
    fscanf(stat->configdir, "%s %d %d %d", buffer, &playerchara->type, &playerchara->atk, &playerchara->hp);
    playerchara->name = strdup(buffer);
    playerchara->atk = rand() % 100 + playerchara->atk + 1;
    playerchara->hp = rand() % 150 + playerchara->hp + 1;
    playerchara->curhp = playerchara->hp;

    // savedir에 저장
    fprintf(stat->savedir, "charactername= %s\nnickname= none\ncharactertype= %d\nattack= %d\nmaxhp= %d\ncurrenthp= %d\n", playerchara->name, playerchara->type, playerchara->atk, playerchara->hp, playerchara->curhp);
    stat->currentpokemon = 0;

    fclose(stat->savedir);
    fclose(stat->configdir);
    return;
}



void saveloader(gamestatus *stat) {
    stat->savedir = fopen(savedef, "r");
    stat->charastat = malloc(sizeof(pokedef) * 2);
    char buffer[100];
    int pokecount;
    fscanf(stat->savedir, "%*s %d\n", &pokecount);

    //savedir 검사
    if(stat->savedir == NULL) {
        printf("저장된 파일이 없습니다.\n새 게임을 시작합니다...\n");
        characreator(stat);
        return;
    }
    
    for(int i=0; i<pokecount; i++) {
        pokedef *playerchara = &stat->charastat[i];
        fscanf(stat->savedir, "\n%d\n", &stat->pokecount);
        fscanf(stat->savedir, "%*s %s\n", buffer);
        playerchara->name = strdup(buffer);
        fscanf(stat->savedir, "%*s %s\n", buffer);
        playerchara->nickname = strdup(buffer);
        fscanf(stat->savedir, "%*s %d\n", &playerchara->type);
        fscanf(stat->savedir, "%*s %d\n", &playerchara->atk);
        fscanf(stat->savedir, "%*s %d\n", &playerchara->hp);
        fscanf(stat->savedir, "%*s %d\n", &playerchara->curhp);
    }
    //printf("%s %d/%ddebug\n", stat->charastat[0].name, stat->charastat[0].curhp, stat->charastat[0].hp);
    //printf("%s %d/%ddebug\n", stat->charastat[1].name, stat->charastat[1].curhp, stat->charastat[1].hp);
    //sleep(100);
    

    fclose(stat->savedir);
    return;
}



int mainmenu(gamestatus *stat) {
    printf("===============================\n모험을 진행하시겠습니까?\n1. 네\t2. 저장 3. 상점 4. 포켓몬 센터 5. 포켓몬 도감\n>>");
    int choice;
    scanf("%d", &choice);
    return choice;
}



void adventure(gamestatus *stat) {
    pokedef *playerchara = &stat->charastat[0];
    pokedef *enemychara = malloc(sizeof(pokedef));
    stat->configdir = fopen(configdef, "r");
    int buffer, validvalue=0;

    srand(time(NULL));

    // configdir에서 포켓몬 수 읽어서 buffer에 저장, 랜덤 번호 생성
    fscanf(stat->configdir, "%d", &buffer);
    buffer = rand() % buffer +1;

    //enemypoke까지 이동
    for (int i=0; i<buffer; i){
        if(fgetc(stat->configdir) == '\n') {
            i++;
        }
    }

    // configdir에서 buffer 정보 읽어서 edef에 저장, 적 등장
    fscanf(stat->configdir, "%s %d %d %d", enemychara->name, &enemychara->type, &enemychara->atk, &enemychara->hp);

    printf("\033[H\033[J\n===============================\n포켓몬을 탐색하는 중...\n");
    //sleep(rand() % 5 + 1); debug 다시 추가
    printf("\033[H\033[J\n===============================\n야생의 %s가 나타났다!\n===============================\n", enemychara->name);

    //적 상태 설정
    enemychara->atk = rand() % 100 + enemychara->atk + 1;
    enemychara->hp = rand() % 150 + enemychara->hp + 1;
    enemychara->curhp = enemychara->hp;


    while(validvalue == 0) {
        printf("\t\t\t%s\n\t\t\t%d/%d\n%s\n%d/%d\n===============================\n무엇을 해야할까?\n1. 공격 2. 가방열기 3. 도망가기\n>>", enemychara->name, enemychara->curhp, enemychara->hp, playerchara->name, playerchara->curhp, playerchara->hp);
        scanf("%d", &buffer);
        switch (buffer) {
            case 1:
                combatinit(playerchara, enemychara);
                break;
            case 2:
                //bag
                break;
            case 3:
                //run
                break;
            default:
                printf("\n1, 2, 3 중 선택하십시오.\n===============================\n");
                sleep(2);
        }
    }

    




    fclose(stat->configdir);
    free(enemychara);
    return;
}

void combatinit(pokedef *chara1, pokedef *chara2) {

    srand(time(NULL));
    if(rand()%2 == 0) {
        attack(chara1, chara2);
        if(chara2->curhp <= 0) {
            printf("%s는 쓰려졌다.\n===============================\n", chara2->name); // 적 사망
            chara2->curhp = 0;
            return;
        }
        attack(chara2, chara1);
        if(chara1->curhp <= 0) {
            printf("%s는 쓰려졌다.\n===============================\n", chara1->name); //아군 사망
            chara1->curhp = 0;
            replaceplayerchara(chara1, chara2);
            return;
        }
    }
    else {
        attack(chara2, chara1);
        if(chara1->curhp <= 0) {
            printf("%s는 쓰려졌다.\n===============================\n", chara1->name); //아군 사망
            chara1->curhp = 0;
            replaceplayerchara(chara1, chara2);
            return;
        }
        attack(chara1, chara2);
        if(chara2->curhp <= 0) {
            printf("%s는 쓰려졌다.\n===============================\n", chara2->name); //적 사망
            chara2->curhp = 0;
            return;
        }
    }

    return;
}

void attack(pokedef *chara1, pokedef *chara2) {
    printf("===============================\n%s의 공격!\n", chara1->name);
    switch(chara1->type) {
        case 1:
            if(chara2->type == 2) {
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5));
                printf("효과가 굉장했다!\n");
                chara2->curhp -= chara1->atk * 1.5;
                break;
            }
            else if(chara2->type == 3) {
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk / 2);
                printf("효과가 별로인 듯 하다...\n");
                chara2->curhp -= chara1->atk / 2;
                break;
            }

        case 2:
            if(chara2->type == 3) {
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5));
                printf("효과가 굉장했다!\n");
                chara2->curhp -= chara1->atk * 1.5;
                break;
            }
            else if(chara2->type == 1) {
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk / 2);
                printf("효과가 별로인 듯 하다...\n");
                chara2->curhp -= chara1->atk / 2;
                break;
            }

        case 3:
            if(chara2->type == 1) {
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5));
                printf("효과가 굉장했다!\n");
                chara2->curhp -= chara1->atk * 1.5;
                break;
            }
            else if(chara2->type == 2) {
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk / 2);
                printf("효과가 별로인 듯 하다...\n");
                chara2->curhp -= chara1->atk / 2;
                break;
            }
        printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk);
        chara2->curhp -= chara1->atk;
    }
}

void replaceplayerchara(pokedef *chara1, pokedef *chara2) {
    printf("\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n%d/%d\n===============================\n어느 포켓몬을 내보낼까?\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->curhp, chara1->hp);
    //교체 시스템
    return;
}