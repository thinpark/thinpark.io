#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        FILE *write_fp;
        char buffer[BUFSIZ+1];

        sprintf(buffer, "Once upon a time, there was...\n");
        write_fp = popen("od -c", "w");
        if (write_fp != NULL) {
                fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
                pclose(write_fp);
                exit(EXIT_SUCCESS);
        } else {
                fprintf(stderr, "popen() error.\n");
                exit(EXIT_FAILURE);
        }
}
