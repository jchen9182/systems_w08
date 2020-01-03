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
        printf("Semaphore Created...\n");
    }

    int shmd;
    shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    if (shmd < 0) printf("Error %d: %s\n", errno, strerror(errno));
    else printf("Shared Memory Created...\n");

    int file;
    file = open("story.txt", O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0644);
    if (file < 0) printf("Error %d: %s\n", errno, strerror(errno));
    else printf("File Created...\n");
}

void view_story() {
    printf("The Story So Far:\n");

    FILE* fp;
    fp = fopen("story.txt", "r");

    char line[100];
    while (fgets(line, 100, fp)) {
        printf("%s\n", line);
    }
    
    fclose(fp);
}

void remove_story() {
    printf("Trying To Get In...\n");

    int semd;
    struct sembuf sb;
    semd = semget(KEY, 1, 0);
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(semd, &sb, 1);

    int shmd;
    int v, r;
    shmd = shmget(KEY, SEG_SIZE, 0644);

    view_story();

    shmctl(shmd, IPC_RMID, 0);
    printf("Shared Memory Removed...\n");

    semctl(semd, IPC_RMID, 0);
    printf("Semaphore Removed...\n");

    remove("story.txt");
    printf("File Removed...\n");
}

int main(int argc, char* argv[]) {
    char* arg = argv[1];

    if (!strcmp(arg, "-c")) create_story();
    else if (!strcmp(arg, "-v")) view_story();
    else if (!strcmp(arg, "-r")) remove_story();
    else printf("Argument is invalid.\n");
}