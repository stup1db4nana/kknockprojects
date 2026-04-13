// k.knock 포켓몬 게임 과제
// gcc 16.0.1, Linux 6.19.10-300.fc44.x86_64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define savedef "pokesave"   // 세이브 이름
#define configdef "pokeconf" // 설정 이름
#define clear "clear"        // ANSI 한정
/*
typedef enum
{
    FIRE,
    WATER,
    GRASS
} poketype;
 */
const char *poketypename[] = {"불", "물", "풀"};

typedef struct
{
    char *name;
    char *nickname;
    int type;
    int atk;
    int hp;
    int curhp;
} pokedef;

typedef struct
{
    FILE *savedir;
    FILE *configdir;
    pokedef *charastat;
    int pokecount;
    int currentpokemon;
    int money;
    int pokeball;
    int healthpotion;
} gamestatus;

int searchfromfile(FILE *dir, char *value, char *followingstring); // 파일 포인터, 찾는 열, 다음 열

void initload(gamestatus *stat);
void characreator(gamestatus *stat);
void saveloader(gamestatus *stat);

void setcharastatus(pokedef *chara);

int mainmenu(gamestatus *stat);
int adventure(gamestatus *stat);
int combatinit(pokedef *playerchara, pokedef *enemychara);
int attack(pokedef *chara1, pokedef *chara2);
// void attack(pokedef *chara1, pokedef *chara2);
int replaceplayerchara(gamestatus *stat);
int capturechara(gamestatus *stat, pokedef *capturechara);

void save(gamestatus *stat);

void market(gamestatus *stat);
void centre(gamestatus *stat);

void pokedex(gamestatus *stat);

/*
 initloader ->characreator 또는 saveloader 끝
 mainmenu에서 값 받아와 스위치문 -> adventure:
                                    combatinit:
                                        attack;
                                        replaceplayerchara;
                                도망




*/

void main()
{
    gamestatus *stat = malloc(sizeof(gamestatus));
    int repeatcondition = 1;
    initload(stat);

    while (repeatcondition)
    {
        switch (mainmenu(stat))
        {
        case 1:
            if (adventure(stat) == 0)
            {
                printf("wrong\n");
            }
            break;
        case 2:
            printf("bruhsave\n");
            break;
        case 3:
            market(stat);
            break;
        case 4:
            centre(stat);
            break;
        case 5:
            printf("docs\n");
            break;
        default:
            // printf("\033[H\033[J\n");
            system(clear);
            printf("1, 2, 3, 4, 5 중 하나만 입력하십시오.\n");
            break;
        }
    }

    free(stat);
    return;
}

int searchfromfile(FILE *dir, char *value, char *followingstring)
{
    char buffer[100];

    while (!feof(dir))
    {
        fscanf(dir, "%s", buffer);
        if (strcmp(buffer, value) == 0)
        {
            // int valueindex = ftell(dir);
            fscanf(dir, "%s", followingstring);
            return 0;
        }
    }

    followingstring[0] = '\0';
    return 1;
}

void initload(gamestatus *stat)
{
    char buffer[2];
    int condition = 0;

    printf("====================================\n\tK.Knock Pokemon Game\n\n\tpress enter to start\n====================================\n");
    fgetc(stdin);
    system(clear);
    printf("===============================\n1. 새로하기\t2. 이어하기\n>> ");

    while (condition == 0)
    {
        int choice;
        if (!fgets(buffer, 2, stdin))
        {
            choice = 0;
            continue;
        }

        choice = atoi(buffer);
        switch (choice)
        {
        case 1:
            system(clear);
            printf("새 게임을 시작합니다...\n");
            characreator(stat);
            condition = 1;
            break;
        case 2:
            system(clear);
            printf("세이브를 불러옵니다...\n");
            saveloader(stat);
            condition = 1;
            break;

        default:
            system(clear);
            printf("1과 2 중 선택하십시오.\n\n===============================\n1. 새로하기\t2. 이어하기\n>> ");
            // printf("test %d test\n", choice);
            break;
        }
    }
    system(clear);
    // printf("\033[H\033[J\n");

    return;
}

void characreator(gamestatus *stat)
{
    // stat->savedir = fopen(savedef, "w"); //세이브 기능으로 옮기기 uncomment
    stat->configdir = fopen(configdef, "r");
    stat->charastat = malloc(sizeof(pokedef));
    char starterpokes[3][20];
    char buffer[2];
    int selection = 0;

    srand(time(NULL));

    pokedef *playerchara = &stat->charastat[0];
    stat->pokecount = 1; // 포켓몬 수
    // fprintf(stat->savedir, "pokecount= %d\n", stat->pokecount);

    stat->money = 10000; // 돈
    // fprintf(stat->savedir, "money= %d\n", stat->money);

    stat->healthpotion = 0; // 약
    // fprintf(stat->savedir, "healthpotion= %d\n", stat->healthpotion);

    fscanf(stat->configdir, "%*d %s %*d %*d %*d %s %*d %*d %*d %s", starterpokes[0], starterpokes[1], starterpokes[2]); // 시작 포켓몬 configdir 최초 3개 중 선택
    printf("===============================\n어느 포켓몬을 선택하시겠습니까?\n");
    printf("1. %s 2. %s 3. %s\n>>", starterpokes[0], starterpokes[1], starterpokes[2]);

    // if (!fgets(buffer, 2, stdin))
    //     return;

    // scanf(" %d", &selection);
    while ((getchar()) != '\n')
        ;
    while (!selection)
    {
        if (!fgets(buffer, 2, stdin))
        {
            exit(EXIT_FAILURE);
        }
        selection = atoi(buffer);
        if (selection == 1 || selection == 2)
        {
            break;
        }

        // printf("\033[H\033[J\n");
        system(clear);
        printf("1, 2, 3 중 선택하십시오.\n===============================\n어느 포켓몬을 선택하시겠습니까?\n");
        printf("1. %s 2. %s 3. %s\n>>", starterpokes[0], starterpokes[1], starterpokes[2]);
    }

    rewind(stat->configdir); // 리와인드해서 선택한 포켓몬 자리까지 이동
    for (int i = 0; i < selection; i)
    {
        if (fgetc(stat->configdir) == '\n')
        {
            i++;
        }
    }

    fscanf(stat->configdir, "%s %d %d %d", buffer, &playerchara->type, &playerchara->atk, &playerchara->hp); // 선택한 포켓몬 정보 읽어서 메모리 playerchara에 저장
    playerchara->name = malloc(sizeof(char) * (strlen(buffer) + 1));
    playerchara->nickname = malloc(sizeof(char) * 4);
    playerchara->name = strdup(buffer);
    playerchara->nickname = strdup("none");

    setcharastatus(playerchara); // playerchara 상태 설정

    // fprintf(stat->savedir, "charactername= %s\nnickname= none\ncharactertype= %d\nattack= %d\nmaxhp= %d\ncurrenthp= %d\n", playerchara->name, playerchara->type, playerchara->atk, playerchara->hp, playerchara->curhp);
    // 세이브 기능으로 옮기기 uncomment
    stat->currentpokemon = 0;

    // fclose(stat->savedir);
    fclose(stat->configdir);
    return;
}

void setcharastatus(pokedef *chara)
{
    chara->atk = rand() % 100 + chara->atk + 1;
    chara->hp = rand() % 150 + chara->hp + 1;
    chara->curhp = chara->hp;

    return;
}

void saveloader(gamestatus *stat)
{
    stat->savedir = fopen(savedef, "r");
    char buffer[100];

    if (stat->savedir == NULL)
    {
        printf("저장된 파일이 없습니다.\n새 게임을 시작합니다...\n");
        characreator(stat);
        return;
    }

    searchfromfile(stat->savedir, "pokecount=", buffer);
    stat->pokecount = atoi(buffer);

    searchfromfile(stat->savedir, "money=", buffer);
    stat->money = atoi(buffer);

    searchfromfile(stat->savedir, "healthpotion=", buffer);
    stat->healthpotion = atoi(buffer);

    stat->charastat = malloc(sizeof(pokedef) * stat->pokecount);
    for (int i = 0; i < stat->pokecount; i++)
    {
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
    /* debug
    printf("%s %d/%ddebug1\n", stat->charastat[0].name, stat->charastat[0].curhp, stat->charastat[0].hp);
    printf("%s %d/%ddebug2\n", stat->charastat[1].name, stat->charastat[1].curhp, stat->charastat[1].hp);
    printf("%s %d/%ddebug3\n", stat->charastat[2].name, stat->charastat[2].curhp, stat->charastat[2].hp);
    printf("%s %d/%ddebug4\n", stat->charastat[3].name, stat->charastat[3].curhp, stat->charastat[3].hp);
    sleep(100);
    */

    fclose(stat->savedir);
    return;
}

int mainmenu(gamestatus *stat)
{
    printf("===============================\n모험을 진행하시겠습니까?\n1. 네\t2. 저장 3. 상점 4. 포켓몬 센터 5. 포켓몬 도감\n>>");

    while ((getchar()) != '\n')
        ;
    char buffer[2];
    if (!fgets(buffer, 2, stdin))
    {
        exit(EXIT_FAILURE);
    }
    printf("%d test\n", atoi(buffer));

    return atoi(buffer);
}

int adventure(gamestatus *stat)
{
    pokedef *playerchara = &stat->charastat[stat->currentpokemon];
    pokedef *enemychara = malloc(sizeof(pokedef));
    stat->configdir = fopen(configdef, "r");
    int buffer, validvalue = 0;
    char bufferstr[100];

    srand(time(NULL));

    fscanf(stat->configdir, "%d", &buffer);
    buffer = rand() % buffer + 1;

    for (int i = 0; i < buffer; i)
    {
        if (fgetc(stat->configdir) == '\n')
        {
            i++;
        }
    }

    fscanf(stat->configdir, "%s %d %d %d", bufferstr, &enemychara->type, &enemychara->atk, &enemychara->hp);
    fclose(stat->configdir);
    enemychara->name = malloc(sizeof(char) * (strlen(bufferstr) + 1));
    enemychara->name = strdup(bufferstr);
    setcharastatus(enemychara);

    system(clear);
    printf("===============================\n포켓몬을 탐색하는 중...\n");
    // sleep(rand() % 5 + 1); uncomment
    system(clear);
    printf("===============================\n야생의 %s가 나타났다!\n===============================\n", enemychara->name);

    while (validvalue == 0)
    {

        printf("\t\t\t%s\n\t\t\t%d/%d\n%s\n%d/%d\n===============================\n무엇을 해야할까?\n1. 공격 2. 가방열기 3. 도망가기\n>>", enemychara->name, enemychara->curhp, enemychara->hp, playerchara->name, playerchara->curhp, playerchara->hp);
        while ((getchar()) != '\n')
            ;
        if (!fgets(bufferstr, 2, stdin))
        {
            exit(EXIT_FAILURE);
        }

        // scanf(" %d", &buffer);
        // getchar();
        system(clear);
        // printf("\033[H\033[J\n");
        switch (atoi(bufferstr))
        {
        case 1:
            validvalue = combatinit(playerchara, enemychara);
            if (validvalue == 1)
            {
                int moneybuffer = (rand() % 2) * 100 + 300;
                printf("전투에서 이겼다! %d원을 얻었다.\n", moneybuffer);
                stat->money += moneybuffer;
                validvalue = 1;
            }
            else if (validvalue == 2)
            {
                validvalue = replaceplayerchara(stat);
                playerchara = &stat->charastat[stat->currentpokemon];
            }

            break;
        case 2:
        case 3:
            int retreatchance;
            if (enemychara->curhp == 100)
                retreatchance = 1;
            else if (enemychara->curhp >= 60)
                retreatchance = 4;
            else if (enemychara->curhp >= 40)
                retreatchance = 6;
            else
                retreatchance = 9;
            if (rand() % 10 == rand() % retreatchance)
            {
                printf("성공적으로 도망쳤다!\n");
                validvalue = 1;
            }
            else
            {
                printf("===============================\n도망칠 수 없었다...\n");
                validvalue = combatinit(playerchara, enemychara);
            }
            break;

            // default:
            //    printf("\n1, 2, 3 중 선택하십시오.\n===============================\n");
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

    free(enemychara);
    return validvalue;
}

int combatinit(pokedef *chara1, pokedef *chara2)
{
    srand(time(NULL));
    if (rand() % 2 == 0)
    {
        attack(chara1, chara2);
        attack(chara2, chara1);
        printf("===============================\n");
    }
    else
    {
        attack(chara2, chara1);
        attack(chara1, chara2);
        printf("===============================\n");
    }
    if (chara2->curhp <= 0)
    {
        chara2->curhp = 0;
        printf("win\n");
        return 1;
    }
    else if (chara1->curhp <= 0)
    {
        chara1->curhp = 0;
        printf("===============================\n\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n%d/%d\n===============================\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->curhp, chara1->hp);
        // printf("===============================\n\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n%d/%d\n===============================\n어느 포켓몬을 내보낼까?\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->curhp, chara1->hp);
        return 2;
    }

    return 0;
}

int attack(pokedef *chara1, pokedef *chara2)
{
    int attack1 = chara1->atk;
    srand(time(NULL));

    if ((chara1->type == 1 && chara2->type == 2) || (chara1->type == 2 && chara2->type == 3) || (chara1->type == 3 && chara2->type == 1))
    {
        printf("===============================\n%s의 공격!\n", chara1->name);
        if (rand() % 5 == 0)
        {
            attack1 *= 1.5;
            printf("급소에 맞았다!\n");
        }
        printf("%s는 %d의 데미지를 입었다.\n효과가 굉장했다!\n", chara2->name, (int)(attack1 * 1.5));
        chara2->curhp -= attack1 * 1.5;
    }
    else if ((chara1->type == 1 && chara2->type == 3) || (chara1->type == 2 && chara2->type == 1) || (chara1->type == 3 && chara2->type == 2))
    {
        printf("===============================\n%s의 공격!\n", chara1->name);
        if (rand() % 5 == 4)
        {
            attack1 *= 1.5;
            printf("급소에 맞았다!\n");
        }
        printf("%s는 %d의 데미지를 입었다.\n효과가 별로인 듯 하다.\n", chara2->name, attack1 / 2);
        chara2->curhp -= attack1 / 2;
    }
    else
    {

        printf("===============================\n%s의 공격!\n", chara1->name);
        if (rand() % 5 == 1)
        {
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
int replaceplayerchara(gamestatus *stat)
{
    int totaldefeat = 1;
    int remainingindex[100];
    int choice;
    int indexnum = 1;
    for (int i = 0; i < stat->pokecount; i++)
    {
        // if(i == stat->currentpokemon || stat->charastat[i].curhp == 0) {
        if (stat->charastat[i].curhp == 0)
        {
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
    if (totaldefeat == 1)
    {
        printf("눈앞이 깜깜해졌다...\n");
        return 1;
    }
    printf("어느 포켓몬을 내보낼까?\n");
    printf(">>");
    scanf(" %d", &choice);
    stat->currentpokemon = remainingindex[choice];
    // printf("\033[H\033[J\n");
    system(clear);

    return 0;
}

int capturechara(gamestatus *stat, pokedef *capturechara)
{
}

void market(gamestatus *stat)
{
    int choice, condition = 1;

    // printf("\033[H\033[J\n");
    system(clear);
    printf("===============================\n상점\t지갑 : %d원\n1. 포켓몬볼 1000원\n2. 상처약 500원\n===============================\n무엇을 구매할까? (나가기 0)\n>>", stat->money);
    scanf(" %d", &choice);
    getchar();

    while (condition)
    {
        switch (choice)
        {
        case 0:
            condition = 0;
            break;
        case 1:
            printf("포켓몬볼을 몇개 구매할까? (취소 -1)\n>>");
            scanf(" %d", &choice);
            getchar();
            if (choice == -1)
                break;
            if (stat->money >= 1000 * choice)
            {
                stat->money -= 1000 * choice;
                stat->pokeball += choice;
                printf("성공적으로 구매하였다! (잔액 %d원)\n", stat->money);
            }
            else
                printf("돈이 부족하다!\n");
            break;
        case 2:
            printf("상처약을 몇개 구매할까? (취소 -1)\n>>");
            scanf(" %d", &choice);
            getchar();
            if (choice == -1)
                break;
            if (stat->money >= 500 * choice)
            {
                stat->money -= 500 * choice;
                stat->healthpotion += choice;
                printf("성공적으로 구매하였다! (잔액 %d원)\n", stat->money);
                break;
            }
            else
                printf("돈이 부족하다!\n");
            break;
        default:
            printf("1, 2, 0 중 골라라\n");
            choice = 0;
            break;
        }
    }
}

void centre(gamestatus *stat)
{
    // printf("\033[H\033[J\n");
    system(clear);
    printf("===============================\n포켓몬 회복중...\n");
    // sleep(rand() % 5 + 1); uncomment
    printf("회복이 완료되었습니다!\n");
    for (int i = 0; i < stat->pokecount; i++)
    {
        stat->charastat[i].curhp = stat->charastat[i].hp;
        printf("%s %s %d/%d\n", stat->charastat[i].name, poketypename[stat->charastat[i].type], stat->charastat[i].curhp, stat->charastat[i].hp);
    }
    printf("===============================\n메인화면으로 돌아가려면 엔터를 누르십시오\n===============================\n");
    getc(stdin);
    // printf("\033[H\033[J\n");
    system(clear);
    return;
}