#include "headers.h"

int main() {
    printf("Trying To Get In...\n");

    int semd;
    struct sembuf sb;
    semd = semget(KEY, 1, 0);
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(semd, &sb, 1);

    int shmd;
    int v, r;
    char* data;
    shmd = shmget(KEY, SEG_SIZE, 0644);
    data = shmat(shmd, 0, 0);

    printf("Last Addition: ");
    if (*data) printf("%s\n", data);
    else printf("\n");

    printf("Your Addition: ");
    fgets(data, SEG_SIZE, stdin);
    for (int i = 0; i < SEG_SIZE; i++) {
        if (data[i] == '\n') data[i] = '\0';
    }

    FILE* fp;
    fp = fopen("story.txt", "a");
    fputs(data, fp);
    fclose(fp);

    shmdt(data);
    semctl(semd, IPC_RMID, 0);
}