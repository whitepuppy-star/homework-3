#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("사용법:\n");
        printf(" link -h [기존파일] [새파일]   → 하드 링크 생성\n");
        printf(" link -s [기존파일] [새파일]   → 소프트 링크(심볼릭 링크) 생성\n");
        printf(" link -u [링크파일]            → 링크 해제\n");
        printf(" link -r [심볼릭링크파일]      → 심볼릭 링크 대상 경로 확인\n");
        return 0;
    }

    // 하드 링크 생성
    if (strcmp(argv[1], "-h") == 0) {
        if (argc != 4) {
            printf("인자 부족: link -h [기존파일] [새파일]\n");
            return 1;
        }
        if (link(argv[2], argv[3]) == -1) {
            perror("하드 링크 생성 실패");
            return 1;
        }
        printf("[하드 링크] 생성 완료: [%s] → [%s]\n", argv[2], argv[3]);
    }

    // 소프트(심볼릭) 링크 생성
    else if (strcmp(argv[1], "-s") == 0) {
        if (argc != 4) {
            printf("인자 부족: link -s [기존파일] [새파일]\n");
            return 1;
        }
        if (symlink(argv[2], argv[3]) == -1) {
            perror("심볼릭 링크 생성 실패");
            return 1;
        }
        printf("[심볼릭 링크] 생성 완료: [%s] → [%s]\n", argv[2], argv[3]);
    }

    // 링크 해제
    else if (strcmp(argv[1], "-u") == 0) {
        if (argc != 3) {
            printf("인자 부족: link -u [링크파일]\n");
            return 1;
        }
        if (unlink(argv[2]) == -1) {
            perror("링크 해제 실패");
            return 1;
        }
        printf("[링크 해제 완료] [%s]\n", argv[2]);
    }

    // 심볼릭 링크가 가리키는 실제 경로 읽기
    else if (strcmp(argv[1], "-r") == 0) {
        if (argc != 3) {
            printf("인자 부족: link -r [심볼릭링크파일]\n");
            return 1;
        }
        char buf[1024];
        ssize_t len = readlink(argv[2], buf, sizeof(buf) - 1);
        if (len == -1) {
            perror("링크 경로 읽기 실패");
            return 1;
        }
        buf[len] = '\0';
        printf("[심볼릭 링크 대상 확인]\n");
        printf("[%s] → [%s]\n", argv[2], buf);
    }

    // 잘못된 옵션 처리
    else {
        printf("잘못된 옵션입니다: %s\n", argv[1]);
        printf("사용법:\n");
        printf(" link -h [기존파일] [새파일]\n");
        printf(" link -s [기존파일] [새파일]\n");
        printf(" link -u [링크파일]\n");
        printf(" link -r [심볼릭링크파일]\n");
    }

    return 0;
}

