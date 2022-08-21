#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
        	printf("Неверно введены данные.\n");
        	return 3;
    	}
    
    	if (opendir(argv[1]) == NULL) {
        	printf("Директория, которую необходимо скопировать, не существует.\n");
        	return 4;
    	}
	
	pid_t pid = fork();
	if (pid < 0) {
		printf("Не удалось создать процесс\n");
                return 1;
     	}
	if(pid == 0) {
		execl("/bin/cp", "-R", argv[1], argv[2], NULL);
	}
	wait(NULL);
	
	pid = fork();
    	if (pid < 0) {
          	printf("Не удалось создать процесс\n");
          	return 1;
    	}
    	if(pid == 0) {	
        	execl("/usr/bin/gzip", "-f", "-r", argv[2], NULL);
    	}
    	wait(NULL);	
	return 0;
}

