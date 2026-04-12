//k.knock 포켓몬 게임 과제
//FreedesktopSDK 25.08, gcc 16.0.1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define savedef "pokesave"
#define configdef "pokeconf"

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

int searchfromfile(FILE *dir, char *value, char *followingstring); //파일 포인터, 찾는 열, 다음 열

void initload(gamestatus *stat);
    void characreator(gamestatus *stat);
    void saveloader(gamestatus *stat);

void setcharastatus(pokedef *chara);

int mainmenu(gamestatus *stat);
    void adventure(gamestatus *stat);
        int combatinit(pokedef *playerchara, pokedef *enemychara);
        int attack(pokedef *chara1, pokedef *chara2);
        //void attack(pokedef *chara1, pokedef *chara2);
        int replaceplayerchara(gamestatus *stat);

/*
 initloader ->characreator 또는 saveloader 끝
 mainmenu에서 값 받아와 스위치문 -> adventure:
                                    combatinit:
                                        attack;
                                        replaceplayerchara;




*/

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

int searchfromfile(FILE *dir, char *value, char *followingstring) {
    char buffer[100];

    while(!feof(dir)) {
        fscanf(dir, "%s", buffer);
        if(strcmp(buffer, value) == 0){
            //int valueindex = ftell(dir);
            fscanf(dir, "%s", followingstring);
            return 0;
        }       
    }

    followingstring[0] = '\0';
    return 1;
}

void initload(gamestatus *stat) {
    printf("====================================\n\tK.Knock Pokemon Game\n\n\tpress enter to start\n====================================\n");
    getc(stdin);
    printf("\033[H\033[J\n===============================\n1. 새로하기\t2. 이어하기\n>>");
    
    int condition = 0;
    while (condition == 0) {
        int choice;
        scanf(" %d", &choice);
        switch (choice) {
            case 1:
                printf("\033[H\033[J\n새 게임을 시작합니다...\n");
                characreator(stat);
                condition = 1;
                break;
            case 2:
                printf("\033[H\033[J\n세이브를 불러옵니다...\n");
                saveloader(stat);
                condition = 1;
                break;

            default:
                printf("\033[H\033[J\n1과 2 중 선택하십시오.\n\n===============================\n1. 새로하기\t2. 이어하기\n>>");
        }
    }
    printf("\033[H\033[J\n");

    return;
}

void characreator(gamestatus *stat) {
    stat->savedir = fopen(savedef, "w");
    stat->configdir = fopen(configdef, "r");
    stat->charastat = malloc(sizeof(pokedef));
    char starterpokes[3][20];
    char buffer[100];

    srand(time(NULL));

    pokedef *playerchara = &stat->charastat[0];
    stat->pokecount = 1;
    fprintf(stat->savedir, "pokecount= %d\n", stat->pokecount);
    
    fscanf(stat->configdir, "%*d %s %*d %*d %*d %s %*d %*d %*d %s", starterpokes[0], starterpokes[1], starterpokes[2]);
    printf("===============================\n어느 포켓몬을 선택하시겠습니까?\n");
    printf("1. %s 2. %s 3. %s\n>>", starterpokes[0], starterpokes[1], starterpokes[2]);

    int selection;
    scanf("%d", &selection);

    rewind(stat->configdir);
    for(int i=0; i<selection; i) {
        if(fgetc(stat->configdir) == '\n') {
            i++;
        }
    } 

    fscanf(stat->configdir, "%s %d %d %d", buffer, &playerchara->type, &playerchara->atk, &playerchara->hp);
    playerchara->name = malloc(sizeof(char) * (strlen(buffer) + 1));
    playerchara->nickname = malloc(sizeof(char) *4);
    playerchara->name = strdup(buffer);
    playerchara->nickname = strdup("none");

    setcharastatus(playerchara);

    fprintf(stat->savedir, "charactername= %s\nnickname= none\ncharactertype= %d\nattack= %d\nmaxhp= %d\ncurrenthp= %d\n", playerchara->name, playerchara->type, playerchara->atk, playerchara->hp, playerchara->curhp);
    stat->currentpokemon = 0;

    fclose(stat->savedir);
    fclose(stat->configdir);
    return;
}

void setcharastatus(pokedef *chara) {
    chara->atk = rand() % 100 + chara->atk + 1;
    chara->hp = rand() % 150 + chara->hp + 1;
    chara->curhp = chara->hp;
    return;
}

void saveloader(gamestatus *stat) {
    stat->savedir = fopen(savedef, "r");
    char buffer[100];
    
    searchfromfile(stat->savedir, "pokecount=", buffer);
    stat->pokecount = atoi(buffer);

    if(stat->savedir == NULL) {
        printf("저장된 파일이 없습니다.\n새 게임을 시작합니다...\n");
        characreator(stat);
        return;
    }

    stat->charastat = malloc(sizeof(pokedef)*stat->pokecount);
    for(int i=0; i<stat->pokecount; i++) {
        pokedef *playerchara = &stat->charastat[i];
        searchfromfile(stat->savedir, "charactername=", buffer);
        playerchara->name = strdup(buffer);
        searchfromfile(stat->savedir, "nickname=", buffer);
        playerchara->nickname = strdup(buffer);
        searchfromfile(stat->savedir, "charactertype=", buffer);
        playerchara->type = atoi(buffer);
        searchfromfile(stat->savedir, "attack=", buffer);
        playerchara->atk = atoi(buffer);
        searchfromfile(stat->savedir, "maxhp=", buffer);
        playerchara->hp = atoi(buffer);
        searchfromfile(stat->savedir, "currenthp=", buffer);
        playerchara->curhp = atoi(buffer);
    }
    /*
    printf("%s %d/%ddebug1\n", stat->charastat[0].name, stat->charastat[0].curhp, stat->charastat[0].hp);
    printf("%s %d/%ddebug2\n", stat->charastat[1].name, stat->charastat[1].curhp, stat->charastat[1].hp);
    printf("%s %d/%ddebug3\n", stat->charastat[2].name, stat->charastat[2].curhp, stat->charastat[2].hp);
    printf("%s %d/%ddebug4\n", stat->charastat[3].name, stat->charastat[3].curhp, stat->charastat[3].hp);
    sleep(100);
    */

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
    pokedef *playerchara = &stat->charastat[stat->currentpokemon];
    pokedef *enemychara = malloc(sizeof(pokedef));
    stat->configdir = fopen(configdef, "r");
    int buffer, validvalue=0;
    char bufferstr[100];

    srand(time(NULL));

    fscanf(stat->configdir, "%d", &buffer);
    buffer = rand() % buffer +1;

    for (int i=0; i<buffer; i){
        if(fgetc(stat->configdir) == '\n') {
            i++;
        }
    }

    fscanf(stat->configdir, "%s %d %d %d", bufferstr, &enemychara->type, &enemychara->atk, &enemychara->hp);
    enemychara->name = malloc(sizeof(char) * (strlen(bufferstr) + 1));
    enemychara->name = strdup(bufferstr);
    setcharastatus(enemychara);

    printf("\033[H\033[J\n===============================\n포켓몬을 탐색하는 중...\n");
    //sleep(rand() % 5 + 1); uncomment
    printf("\033[H\033[J\n===============================\n야생의 %s가 나타났다!\n===============================\n", enemychara->name);


    while(validvalue == 0) {
        printf("\t\t\t%s\n\t\t\t%d/%d\n%s\n%d/%d\n===============================\n무엇을 해야할까?\n1. 공격 2. 가방열기 3. 도망가기\n>>", enemychara->name, enemychara->curhp, enemychara->hp, playerchara->name, playerchara->curhp, playerchara->hp);
        scanf("%d", &buffer);
        printf("\033[H\033[J\n");
        switch(buffer) {
            case 1:
                validvalue = combatinit(playerchara, enemychara);
                if(validvalue == 1) {
                    printf("exp stuff\n");
                    return;
                }
                else if(validvalue == 2) {
                    printf("replacerrr\n");
                    validvalue = replaceplayerchara(stat);
                    playerchara = &stat->charastat[stat->currentpokemon];
                }
                    
                break;
            case 2:
            case 3:

            default:
                printf("\n1, 2, 3 중 선택하십시오.\n===============================\n");
        }
        
    }


    /*   
    while(validvalue == 0) {
        printf("\t\t\t%s\n\t\t\t%d/%d\n%s\n%d/%d\n===============================\n무엇을 해야할까?\n1. 공격 2. 가방열기 3. 도망가기\n>>", enemychara->name, enemychara->curhp, enemychara->hp, playerchara->name, playerchara->curhp, playerchara->hp);
        scanf("%d", &buffer);
        printf("\033[H\033[J\n");
        switch (buffer) {
            case 1:
                if(combatinit(playerchara, enemychara) == 0) {
                    printf("test test test\n");
                }
                else if(combatinit(playerchara, enemychara) == 1) {
                    printf("야생의 %s를 쓰러뜨렸다!\n===============================\n", enemychara->name);
                    printf("어쩌고저쩌고\n");
                    validvalue = 1;
                    break;
                }
                else if(combatinit(playerchara, enemychara) == 2) {
                    printf("%s가 쓰러졌다...\n===============================\n", playerchara->name);
                    replaceplayerchara(playerchara, enemychara);
                    validvalue = 1;
                    break;
                }
                break;
            case 2:
                //bag
                break;
            case 3:
                //run
                validvalue = 1;
                break;
            default:
                printf("\n1, 2, 3 중 선택하십시오.\n===============================\n");
        }
    }*/

    fclose(stat->configdir);
    free(enemychara);
    return;
}

int combatinit(pokedef *chara1, pokedef *chara2) {
    srand(time(NULL));
    if(rand()%2 == 0) {
        attack(chara1, chara2);
        attack(chara2, chara1);
        printf("===============================\n");
    }
    else {
        attack(chara2, chara1);
        attack(chara1, chara2);
        printf("===============================\n");
    }
    if(chara2->curhp <= 0) {
        chara2->curhp = 0;
        printf("win\n");
        return 1;
    }
    else if(chara1->curhp <= 0) {
        chara1->curhp = 0;
        printf("===============================\n\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n%d/%d\n===============================\n어느 포켓몬을 내보낼까?\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->curhp, chara1->hp);
        return 2;
    }
        
    return 0;
}

int attack(pokedef *chara1, pokedef *chara2) {
    int attack1 = chara1->atk;
    srand(time(NULL));

    if ((chara1->type == 1 && chara2->type == 2) || (chara1->type == 2 && chara2->type == 3) || (chara1->type == 3 && chara2->type == 1)) {
        printf("===============================\n%s의 공격!\n", chara1->name);
        if(rand()%5 == 0) {
            attack1 *= 1.5;
            printf("급소에 맞았다!\n");
        }
        printf("%s는 %d의 데미지를 입었다.\n효과가 굉장했다!\n", chara2->name, (int)(attack1 * 1.5));
        chara2->curhp -= attack1 * 1.5;
    }
    else if ((chara1->type == 1 && chara2->type == 3) || (chara1->type == 2 && chara2->type == 1) || (chara1->type == 3 && chara2->type == 2)) {
        printf("===============================\n%s의 공격!\n", chara1->name);
        if(rand()%5 == 4) {
            attack1 *= 1.5;
            printf("급소에 맞았다!\n");
        }
        printf("%s는 %d의 데미지를 입었다.\n효과가 별로인 듯 하다.\n", chara2->name, attack1 / 2);
        chara2->curhp -= attack1 / 2;
    }
    else {
        
        printf("===============================\n%s의 공격!\n", chara1->name);
        if(rand()%5 == 1) {
            attack1 *= 1.5;
            printf("급소에 맞았다!\n");
        }
        printf("%s는 %d의 데미지를 입었다.\n", chara2->name, attack1);
        chara2->curhp -= attack1;
    }

    return 0;
}

/*
int combatinit(pokedef *chara1, pokedef *chara2) {

    srand(time(NULL));
    if(rand()%2 == 0) {
        attack(chara1, chara2);
        if(chara2->curhp <= 0) {
            //printf("%s는 쓰려졌다.\n===============================\n", chara2->name);
            chara2->curhp = 0;
            return 1;
        }
        attack(chara2, chara1);
        if(chara1->curhp <= 0) {
            //printf("%s는 쓰려졌다.\n===============================\n", chara1->name); //아군
            chara1->curhp = 0;
            //replaceplayerchara(chara1, chara2);
            return 2;
        }
    }
    else {
        attack(chara2, chara1);
        if(chara1->curhp <= 0) {
            //printf("%s는 쓰려졌다.\n===============================\n", chara1->name); //아군
            chara1->curhp = 0;
            //replaceplayerchara(chara1, chara2);
            return 2;
        }
        attack(chara1, chara2);
        if(chara2->curhp <= 0) {
            //printf("%s는 쓰려졌다.\n===============================\n", chara2->name);
            chara2->curhp = 0;
            return 1;
        }
    }

    return 0;
}
    */
/*
void attack(pokedef *chara1, pokedef *chara2) {

    printf("===============================\n%s의 공격!\n", chara1->name);
    switch(chara1->type) {
        case 1:
            if(chara2->type == 2) {
                srand(time(NULL));
                if(rand()%5 == 0) {
                    printf("급소에 맞았다!\n");
                    printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 2 * 1.5));
                    chara2->curhp -= chara1->atk * 2 * 1.5;
                    break;
                }
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5));
                printf("효과가 굉장했다!\n");
                chara2->curhp -= chara1->atk * 1.5;
                break;
            }
            else if(chara2->type == 3) {
                srand(time(NULL));
                if(rand()%5 == 0) {
                    printf("급소에 맞았다!\n");
                    printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk / 2 * 1.5));
                    chara2->curhp -= chara1->atk / 2 * 1.5;
                    break;
                }
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk / 2);
                printf("효과가 별로인 듯 하다...\n");
                chara2->curhp -= chara1->atk / 2;
                break;
            }

        case 2:
            if(chara2->type == 3) {
                srand(time(NULL));
                if(rand()%5 == 0) {
                    printf("급소에 맞았다!\n");
                    printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5 * 1.5));
                    chara2->curhp -= chara1->atk * 1.5 * 1.5;
                    break;
                }
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5));
                printf("효과가 굉장했다!\n");
                chara2->curhp -= chara1->atk * 1.5;
                break;
            }
            else if(chara2->type == 1) {
                srand(time(NULL));
                if(rand()%5 == 0) {
                    printf("급소에 맞았다!\n");
                    printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk / 2 * 1.5));
                    chara2->curhp -= chara1->atk / 2 * 1.5;
                    break;
                }
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk / 2);
                printf("효과가 별로인 듯 하다...\n");
                chara2->curhp -= chara1->atk / 2;
                break;
            }

        case 3:
            if(chara2->type == 1) {
                srand(time(NULL));
                if(rand()%5 == 0) {
                    printf("급소에 맞았다!\n");
                    printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5 * 1.5));
                    chara2->curhp -= chara1->atk * 1.5 * 1.5;
                    break;
                }
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5));
                printf("효과가 굉장했다!\n");
                chara2->curhp -= chara1->atk * 1.5;
                break;
            }
            else if(chara2->type == 2) {
                srand(time(NULL));
                if(rand()%5 == 0) {
                    printf("급소에 맞았다!\n");
                    printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk / 2 * 1.5));
                    chara2->curhp -= chara1->atk / 2 * 1.5;
                    break;
                }
                printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk / 2);
                printf("효과가 별로인 듯 하다...\n");
                chara2->curhp -= chara1->atk / 2;
                break;
            }
        srand(time(NULL));
        if(rand()%5 == 0) {
            printf("급소에 맞았다!\n");
            printf("%s는 %d의 데미지를 입었다.\n", chara2->name, (int)(chara1->atk * 1.5));
            chara2->curhp -= chara1->atk * 1.5;
            return;
        }
        printf("%s는 %d의 데미지를 입었다.\n", chara2->name, chara1->atk);
        chara2->curhp -= chara1->atk;
    }

    
    return;
}
*/
int replaceplayerchara(gamestatus *stat) {
    int totaldefeat = 1;
    int remainingindex[100];
    int choice;
    int indexnum = 1;
    for (int i=0; i<stat->pokecount; i++) {
        if(i == stat->currentpokemon || stat->charastat[i].curhp == 0) {
            continue;
        }
        printf("%d. %s %d/%d\n", indexnum, stat->charastat[i].name, stat->charastat[i].curhp, stat->charastat[i].hp);
        remainingindex[indexnum] = i;
        indexnum++;
        totaldefeat = 0;
        /*
        searchfromfile(stat->savedir, "charactername=", buffer);

        if(i == stat->currentpokemon || stat->charastat[i].curhp == 0) {
            continue;
        }
        printf("%d. %s\n", i, buffer);
        totaldefeat = 0; */
    }
    if(totaldefeat == 1) {
        return 1;
    }
    printf(">>");
    scanf("%d", &choice);
    stat->currentpokemon = remainingindex[choice];
    printf("\033[H\033[J\n");

    fclose(stat->savedir);
    return 0;
}
