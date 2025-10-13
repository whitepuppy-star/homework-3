#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> // 매크로 함수 
#include <dirent.h>  // 파일 정보 구조
#include <unistd.h>
#include <string.h>

// 파일 권한 출력 함수
void print_permissions(mode_t mode) {
    printf("%c", S_ISDIR(mode) ? 'd' : '-');  // 파일 타입 검사 (디렉토리)
    printf("%c", (mode & S_IRUSR) ? 'r' : '-'); // 사용자 읽기
    printf("%c", (mode & S_IWUSR) ? 'w' : '-'); // 사용자 쓰기
    printf("%c", (mode & S_IXUSR) ? 'x' : '-'); // 사용자 실행
    printf("%c", (mode & S_IRGRP) ? 'r' : '-'); // 그룹 쓰기
    printf("%c", (mode & S_IWGRP) ? 'w' : '-'); // 그룹 쓰기
    printf("%c", (mode & S_IXGRP) ? 'x' : '-'); // 그룹 실행
    printf("%c", (mode & S_IROTH) ? 'r' : '-'); // 기타 읽기
    printf("%c", (mode & S_IWOTH) ? 'w' : '-'); // 기타 쓰기
    printf("%c ", (mode & S_IXOTH) ? 'x' : '-'); // 기타 실행
}

int main(int argc, char *argv[]) {
    DIR *dp;
    struct dirent *entry; // 디렉토리 항목 구조체 포인터
    struct stat statbuf;
    int opt_a = 0, opt_d = 0;

    // 옵션 처리
    if (argc > 1) {
        if (strcmp(argv[1], "-a") == 0) opt_a = 1;
        else if (strcmp(argv[1], "-d") == 0) opt_d = 1;
    }

    dp = opendir(".");   // 디렉토리 열기
    if (dp == NULL) {  // 디렉토리 열기 실패
        perror("디렉토리 열기 실패");
        return 1;
    }
    // task 1 과제 구현하기 시작체
    while ((entry = readdir(dp)) != NULL) {  // 파일 목록을 하나씩 읽기
        if (!opt_a && entry->d_name[0] == '.') continue; // 숨김파일 제외(-a옵션							 // 파일 이름의 첫글자 .)

        if (stat(entry->d_name, &statbuf) == -1) continue; // stat() 함수를이용해 파일의 
	//innode 정보를 가져와 statbuf에 저장 (파일 종류, incode 번호, 크기, 소유자 id, 수정 시간 등의 정보) , 만약 읽기 실패하면 -1 반환 후 , 스킵하기.

        if (opt_d) { // opt_d가 1때. 즉, 디렉토리인 항목만 출력
            if (S_ISDIR(statbuf.st_mode))  // 파일 종류 + 접근 권한을 16진수 비트값으로 넣어 이게 디렉토리인지, 일반파일인지, 권한이 뭔지에 대한 정보를 담음.
                printf("Directory: %s\n", entry->d_name);
        } else if (opt_a) { // opt_a가 1일 때. 즉, 상세 정보 출력
            print_permissions(statbuf.st_mode); // 파일 권한 출력 함수
            printf("inode:%ld  %s\n", statbuf.st_ino, entry->d_name); // 파일의 inode 번호와 파일 이름을 출력.
        } else { // 기본 ls
            printf("%s  ", entry->d_name); // 즉, 아무 옵션이 없어 파일 이름만 나열.
        }
    }
    printf("\n");
    closedir(dp);
    return 0;
}

