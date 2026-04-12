#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[20];
    int score1;
    int score2;
} Student;

int main() {
    Student s[3];
    FILE *fp;
    int menu;

    while (1) {
        printf("\n--- 성적 관리 시스템 ---\n");
        printf("1. 성적 입력 및 저장 (score.txt)\n");
        printf("2. 성적 보고서 생성 및 출력 (report.txt)\n");
        printf("3. 종료\n");
        printf("선택: ");
        scanf("%d", &menu);

        if (menu == 3) break;

        switch (menu) {
            case 1:
                printf("학생 3명의 성적 입력 (이름 점수1 점수2):\n");
                for (int i = 0; i < 3; i++) {
                    printf("%d번: ", i + 1);
                    scanf("%s %d %d", s[i].name, &s[i].score1, &s[i].score2);
                }

                // score.txt 저장
                fp = fopen("score.txt", "w");
                if (fp == NULL) break;
                // 구조체에 score.txt 저장하기
                for (int i = 0; i < 3; i++) {
                    fprintf(fp, "%s %d %d\n", s[i].name, s[i].score1, s[i].score2);
                }
                // 파일 닫기
                fclose(fp);
                printf(">> score.txt에 저장이 완료되었습니다.\n");
                break;

            case 2:
                // score.txt 읽어서 report.txt 생성 및 화면 출력
                fp = fopen("score.txt", "r");
                FILE *fout = fopen("report.txt", "w");

                if (fp == NULL || fout == NULL) {
                    printf(">> 파일을 찾을 수 없습니다. (1번을 먼저 실행하세요)\n");
                    break;
                }

                char name[20];
                int s1, s2; // score 담는 변수
                printf("\n--- 성적 보고서 결과 ---\n");

                // fscanf로 파일 데이터를 한 줄씩 읽어 처리
                while (getc(fp) != EOF) {
                    if(getc(fp)==EOF)
                        break;
                    fscanf(fp, "%s %d %d", &name, &s1, &s2);
                    double avg = (s1 + s2) / 2.0;
                    const char* res = (avg >= 80) ? "PASS" : "FAIL";

                    printf("이름: %s | 평균: %.1f | 결과: %s\n", name, avg, res);
                    // report.txt에 결과 저장하기
                    fprintf(fout, "%s %0.1f %s\n", name, avg, res);
                }

                // 파일 닫기
                fclose(fp);
                fclose(fout);
                printf(">> report.txt 저장이 완료되었습니다.\n");
                break;

            default:
                printf("잘못된 선택입니다.\n");
        }
    }

    return 0;
}
