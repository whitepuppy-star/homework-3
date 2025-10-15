#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

// cd 명령어 함수
void cmd_cd(char *input) {
    char *arg = strtok(NULL, " \n"); // "cd ../test" → arg = "../test"
    char resolved[PATH_MAX];

    // 인자 없는 경우 홈 디렉토리로 이동하도록 처리 (선택사항)
    if (arg == NULL) {
        const char *home = getenv("HOME");
        if (home == NULL) home = "/";
        if (chdir(home) == -1)
            perror("cd error");
        return;
    }

    // realpath로 . , ..를 제거함으로써 절대 경로로 바꾼다. (보기 쉽게) 
    if (realpath(arg, resolved) == NULL) {
        perror("cd: 경로 오류");
        return;
    }

    // 실제 디렉토로 변경. 만약 변경할 디렉토리가 -1이면 실패
    if (chdir(resolved) == -1) {
        perror("cd: 디렉토리 이동 실패");
        return;
    }

    // 이동 후 현재 경로 출력 (프롬프트 가시화)
    printf("현재 디렉토리: %s\n", resolved);
}

int main() {
    char input[256];

    while (1) {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd)); // 현재 경로 표시

        // 프롬프트 표시
        printf("[%s]$ ", cwd);

        // 명령어 입력 받기
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        // 공백, 엔터만 입력했을 때 무시
        if (strcmp(input, "\n") == 0)
            continue;

        // strtok 이용해서  명령어 파싱
        char *cmd = strtok(input, " \n");
        if (cmd == NULL) continue;

        // 명령어 처리
        if (strcmp(cmd, "cd") == 0) {
            cmd_cd(input);
        } else if (strcmp(cmd, "exit") == 0) {
            break;
        } else { // 위의 명령어가 아닌 다른 명령어 입력 시
            printf("명령어 '%s'는 지원하지 않습니다.\n", cmd);
        }
    }

    return 0;
}

