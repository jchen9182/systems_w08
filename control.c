#include "headers.h"

void create_story() {
    int semd;
    int v, r;
    semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd < 0) {
        printf("Error %d: %s\n", errno, strerror(errno));
        semd = semget(KEY, 1, 0);
        semctl(semd, 0, GETVAL, 0);
    }
    else {
        union semun sem;
        sem.val = 1;
        semctl(semd, 0, SETVAL, sem);
        printf("Semaphore Created...");
    }

    int shmd;
    shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    if (shmd < 0) printf("Error %d: %s\n", errno, strerror(errno));
    else printf("Shared Memory Created...");

    int file;
    file = open("story.txt", O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0644);
    if (file < 0) printf("Error %d: %s\n", errno, strerror(errno));
    else printf("File Created...");
}

void view_story() {
    int text[FILE_SIZE];

    int fileopen = open("story.txt", O_RDONLY);
    if (fileopen < 0) printf("Error %d: %s\n", errno, strerror(errno));

    int fileread = read(fileopen, &text, sizeof(text));
    if (fileread < 0) printf("Error %d: %s\n", errno, strerror(errno));

    for (int i = 0; i < FILE_SIZE; i++) {
        printf("%s", text[i]);
    }
}

void remove_story() {
    int semd, shmd;
    int v, r;
    struct sembuf sb;

    printf("Trying To Get In...");
    semd = semget(KEY, 1, 0);
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(semd, &sb, 1);

    shmd = shmget(KEY, SEG_SIZE, 0644);


    view_story();

    shmctl(shmd, IPC_RMID, 0);
    printf("Shared Memory Removed\n");

    semctl(semd, IPC_RMID, 0);
    printf("Semaphore Removed...");

    remove("story.txt");
    printf("File Removed...");
}

int main(int argc, char* argv[]) {
    char* arg = argv[1];

    if (!strcmp(arg, "-c")) create_story();
    else if (!strcmp(arg, "-v")) view_story();
    else if (!strcmp(arg, "-r")) remove_story();
    else printf("Argument is invalid.\n");
}