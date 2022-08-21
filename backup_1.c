#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int CopyGzipFile (char *StartFile, char *FinishFile) {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        printf("Не удалось создать процесс\n");
        return 1;
    }
    if (pid == 0) {
        execl("/bin/cp","-p", StartFile, FinishFile, NULL);
    }
	wait(NULL);
    
    pid = fork();
    if (pid < 0) {
          printf("Не удалось создать процесс\n");
          return 1;
    }
    if(pid == 0) {	
        execl("/usr/bin/gzip","-f", "-n", FinishFile, NULL);
    }
	wait(NULL);
    	
    return 0;
}

int CopyDirectory(char *StartDirectory, char* FinishDirectory) {
    DIR *directory;
    struct dirent *dir;
    char *StartAdress;
    char *FinishAdress;
    
    if ((directory = opendir(StartDirectory)) == NULL) {
        printf("Не удалось открыть дирректорию\n");
        return 2;
    }
    
    while ((dir = readdir(directory)) != NULL) {
        StartAdress = calloc(1, 30);
        strcpy(StartAdress, StartDirectory);
        strncat(StartAdress, dir->d_name, strlen(dir->d_name));
        FinishAdress = calloc(1, 30);
        strcpy(FinishAdress, FinishDirectory);
        strncat(FinishAdress, dir->d_name, strlen(dir->d_name));
        
        if (dir->d_type == DT_REG) {
            CopyGzipFile(StartAdress, FinishAdress);
	}
        
        if ((dir->d_type == DT_DIR) && (strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0)) {
            strncat(StartAdress, "/", 1);
            strncat(FinishAdress, "/", 1);
            
            if (opendir(FinishAdress) == NULL) {
                pid_t pid = fork();
                if (pid < 0) {
                    printf("Не удалось создать процесс\n");
                    return 1;
                }
                if (pid == 0) {
                    execl("/bin/mkdir", "-p", FinishAdress, 0);
		    waitpid(pid, NULL, 0);
                }

                CopyDirectory(StartAdress, FinishAdress);
            }
        }
        free(StartAdress);
        free(FinishAdress);
    }
    closedir(directory);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Неверно введены данные.\n");
        return 3;
    }
    
    if (opendir(argv[1]) == NULL) {
        printf("Дирректория, которую необходимо скопировать, не существует.\n");
        return 4;
    }
    
    if (opendir(argv[2]) == NULL) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("/bin/mkdir", "-r", argv[2], 0);
	    
	}
	wait(NULL);
    }
    CopyDirectory(argv[1], argv[2]);
    return 0;
}
