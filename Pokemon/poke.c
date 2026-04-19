// k.knock 포켓몬 게임 과제
// gcc 16.0.1, Linux 6.19.10-300.fc44.x86_64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define savedef "pokesave"   // 세이브 이름
#define configdef "pokeconf" // 설정 이름
#define clear "clear"        // ANSI

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

int userinputint();

void initload(gamestatus *stat);
void characreator(gamestatus *stat);
void saveloader(gamestatus *stat);
void save(gamestatus *stat);

void setcharastatus(pokedef *chara);

int mainmenu(gamestatus *stat);
int adventure(gamestatus *stat);
int combatinit(pokedef *playerchara, pokedef *enemychara);
int contact(gamestatus *stat, pokedef *playerchara, pokedef *enemychara);
int combatcondition(pokedef *chara1, pokedef *chara2);
int attack(pokedef *chara1, pokedef *chara2);
// void attack(pokedef *chara1, pokedef *chara2);
int replaceplayerchara(gamestatus *stat);
int retreatvalidness(pokedef *playerchara, pokedef *enemychara);

int inventory(pokedef *playerchara, pokedef *enemychara, gamestatus *stat);
int capturechara(gamestatus *stat, pokedef *capturechara);

void market(gamestatus *stat);
void centre(gamestatus *stat);

void pokedex(gamestatus *stat);

int main()
{
    gamestatus *stat = malloc(sizeof(gamestatus));
    int repeatcondition = 1;
    initload(stat);

    while (repeatcondition)
    {
        switch (mainmenu(stat))
        {
        case 1:
            int state = 0;
            for (int i = 0; i < stat->pokecount; i++)
            {
                if (stat->charastat[i].curhp > 0)
                {
                    state = 1;
                }
            }
            if (state == 1)
                adventure(stat);
            else
            {
                system(clear);
                printf("모든 포켓몬이 죽었다\n");
            }

            break;
        case 2:
            save(stat);
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
            system(clear);
            printf("1, 2, 3, 4, 5 중 하나만 입력하십시오.\n");
                repeatcondition = 0;
            break;
        }
    }

    free(stat);
    return 0;
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

int userinputint()
{
    char buffer[10];
    int returnvalue, status = 0;

    fgets(buffer, sizeof(buffer), stdin);
    status = sscanf(buffer, " %d", &returnvalue);

    if (status == 0)
        returnvalue = -99;

    return returnvalue;
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
        int choice = userinputint();

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
            // printf("test %d test\n", choice); //debug
            break;
        }
    }
}

void characreator(gamestatus *stat)
{
    stat->configdir = fopen(configdef, "r");
    stat->charastat = malloc(sizeof(pokedef));
    char starterpokes[3][20];
    char buffer[2];
    int selection = 0;

    srand(time(NULL));

    pokedef *playerchara = &stat->charastat[0];
    stat->pokecount = 1;    // 포켓몬 수
    stat->money = 10000;    // 돈
    stat->healthpotion = 0; // 약
    stat->pokeball = 0;

    fscanf(stat->configdir, "%*d %s %*d %*d %*d %s %*d %*d %*d %s", starterpokes[0], starterpokes[1], starterpokes[2]); // 시작 포켓몬 configdir 최초 3개 중 선택
    printf("===============================\n어느 포켓몬을 선택하시겠습니까?\n");
    printf("1. %s 2. %s 3. %s\n>>", starterpokes[0], starterpokes[1], starterpokes[2]);

    while (!selection)
    {
        selection = userinputint();
        switch (selection)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            system(clear);
            printf("1, 2, 3 중 선택하십시오.\n===============================\n어느 포켓몬을 선택하시겠습니까?\n");
            printf("1. %s 2. %s 3. %s\n>>", starterpokes[0], starterpokes[1], starterpokes[2]);
            selection = 0;
        }
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
    stat->currentpokemon = 0;

    fclose(stat->configdir);
}

void setcharastatus(pokedef *chara)
{
    chara->atk = rand() % 100 + chara->atk + 1;
    chara->hp = rand() % 150 + chara->hp + 1;
    chara->curhp = chara->hp;
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
    searchfromfile(stat->savedir, "pokeball=", buffer);
    stat->pokeball = atoi(buffer);
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
}

int mainmenu(gamestatus *stat)
{
    printf("===============================\n모험을 진행하시겠습니까?\n1. 네\t2. 저장 3. 상점 4. 포켓몬 센터 5. 포켓몬 도감\n>>");
    return userinputint();
}

void save(gamestatus *stat)
{
    stat->savedir = fopen(savedef, "w");
    fprintf(stat->savedir, "pokecount= %d\nmoney= %d\nhealthpotion= %d\npokeball= %d\n", stat->pokecount, stat->money, stat->healthpotion, stat->pokeball);
    for (int i = 0; i < stat->pokecount; i++)
    {
        fprintf(stat->savedir, "charactername= %s\nnickname= %s\ncharactertype= %d\nattack= %d\nmaxhp= %d\ncurrenthp= %d\n", stat->charastat->name, stat->charastat->nickname, stat->charastat->type, stat->charastat->atk, stat->charastat->hp, stat->charastat->curhp);
    }
    fclose(stat->savedir);
    system(clear);
    printf("게임 저장 완료!\n");

    return;
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
    contact(stat, playerchara, enemychara);

    free(enemychara);
    return validvalue;
}

int contact(gamestatus *stat, pokedef *playerchara, pokedef *enemychara)
{
    int validvalue = 0;
    while (validvalue == 0)
    {

        printf("\t\t\t%s\n\t\t\t%d/%d\n%s\n%d/%d\n===============================\n무엇을 해야할까?\n1. 공격 2. 가방열기 3. 도망가기\n>>", enemychara->name, enemychara->curhp, enemychara->hp, playerchara->name, playerchara->curhp, playerchara->hp);

        int condition = userinputint();

        switch (condition)
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
            inventory(playerchara, enemychara, stat);
            break;
        case 3:
            if (retreatvalidness(playerchara, enemychara) == 1)
            {
                validvalue = 1;
            }

            else
            {
                printf("도망칠 수 없었다...\n===============================\n");
                break;
            }

        default:
            system(clear);
            printf("\n1, 2, 3 중 선택하십시오.\n===============================\n");
        }
    }
}

int retreatvalidness(pokedef *playerchara, pokedef *enemychara)
{
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

        return 1;
    }
    else
    {
        if (combatinit(playerchara, enemychara) == 1)
            return 1;

        return 0;
    }
}

int combatinit(pokedef *chara1, pokedef *chara2)
{
    system(clear);

    if (combatcondition(chara1, chara2) == 1)
    {
        printf("win\n");
        return 1;
    }
    else if (combatcondition(chara1, chara2) == 2)
    {
        printf("===============================\n\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n0/%d\n===============================\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->hp);
        return 2;
    }

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

    if (combatcondition(chara1, chara2) == 1)
    {
        printf("win\n");
        return 1;
    }
    else if (combatcondition(chara1, chara2) == 2)
    {
        printf("===============================\n\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n0/%d\n===============================\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->hp);
        return 2;
    }

    return 0;
}

int combatcondition(pokedef *chara1, pokedef *chara2)
{
    if (chara2->curhp <= 0)
    {
        chara2->curhp = 0;
        return 1;
    }
    else if (chara1->curhp <= 0)
    {
        chara1->curhp = 0;
        /* debug
        printf("===============================\n\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n0/%d\n===============================\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->hp);
        printf("===============================\n\t\t\t%s\n\t\t\t%d/%d\n%s (기절)\n%d/%d\n===============================\n어느 포켓몬을 내보낼까?\n", chara2->name, chara2->curhp, chara2->hp, chara1->name, chara1->curhp, chara1->hp);
        */
        return 2;
    }
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

int replaceplayerchara(gamestatus *stat)
{
    int state = 1;
    int remainingindex[100];
    int indexnum = 1;
    for (int i = 0; i < stat->pokecount; i++)
    {
        if (stat->charastat[i].curhp == 0)
        {
            continue;
        }
        printf("%d. %s %d/%d\n", indexnum, stat->charastat[i].name, stat->charastat[i].curhp, stat->charastat[i].hp);
        remainingindex[indexnum] = i;
        indexnum++;
        state = 0;
    }
    if (state == 1)
    {
        printf("눈앞이 깜깜해졌다...\n");
        if (stat->money > 999)
        {
            stat->money -= 1000;
            printf("1000원을 잃었다...\n");
        }
        else
        {
            printf("나는 거지다...\n");
            stat->money = 0;
        }

        return 1;
    }
    printf("어느 포켓몬을 내보낼까?\n>>");
    state = userinputint();
    stat->currentpokemon = remainingindex[state];
    system(clear);

    return 0;
}

int inventory(pokedef *playerchara, pokedef *enemychara, gamestatus *stat)
{
    printf("가방에 들어있는 것\n1. 포켓볼: %d개\n2. 상처약: %d개\n무엇을 사용할까?\n>>", stat->pokeball, stat->healthpotion);
    int choice = userinputint();
    switch (choice)
    {
    case 1:
        if (stat->pokeball > 0)
        {
            capturechara(stat, enemychara);
        }
        else
            printf("포켓볼이 부족하다!\n");
        return 1;
        break;
    case 2:
        if (stat->healthpotion > 0)
        {
            playerchara->curhp = playerchara->hp;
            stat->healthpotion--;
            printf("체력 회복 완료!\n");
            return 2;
        }
        else
            printf("상처약이 부족하다!\n");
        return 3;
        break;
    }
    return 0;
}

int capturechara(gamestatus *stat, pokedef *capturechara)
{
    printf("%d test\n", rand() % (capturechara->hp / capturechara->curhp));
    int capturechance = 99; // 0부터 99까지, rand 뺄샘 후 양음수 판별
    if (capturechance - rand() % 99 >= 0)
    {
        printf("success\n");
        return 1;
    }
    return 0;
}

void market(gamestatus *stat)
{
    int choice, condition = 1;
    char buffer[4];

    while (condition)
    {
        int count = 0;
        char neg;
        printf("===============================\n상점\t지갑 : %d원\n1. 포켓몬볼 1000원\n2. 상처약 500원\n===============================\n무엇을 구매할까? (나가기 0)\n>>", stat->money);
        choice = userinputint();

        switch (choice)
        {
        case 0:
            condition = 0;
            break;
        case 1:
            printf("포켓몬볼을 몇개 구매할까? (취소 0)\n>>");
            count = userinputint();
            system(clear);

            printf("test %d", count);
            if (count == 0)
                break;
            else if (stat->money >= 1000 * count && count > 0)
            {
                stat->money -= 1000 * count;
                stat->pokeball += count;
                printf("성공적으로 구매하였다! (잔액 %d원)\n", stat->money);

                break;
            }
            else
                printf("돈이 부족하다!\n");
            break;
        case 2:
            printf("상처약을 몇개 구매할까? (취소 0)\n>>");
            count = userinputint();
            system(clear);

            if (count == 0)
                break;
            else if (stat->money >= 500 * count && count != 0)
            {
                stat->money -= 500 * count;
                stat->healthpotion += count;
                printf("성공적으로 구매하였다! (잔액 %d원)\n", stat->money);

                break;
            }
            else
                printf("돈이 부족하다!\n");
            break;
        default:
            system(clear);
            printf("1, 2 중 골라라\n");

            break;
        }
    }
}

void centre(gamestatus *stat)
{
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
    system(clear);
}