#include <unistd.h>
#include <stdlib.h>

#define ERR_READ  -1
#define ERR_WRITE -2

int main(void)
{
        char buffer[128];
        int nread;

        nread = read(0, buffer, 128);
        if (nread == -1) {
                write(2, "A read error has occurred\n", 26);
                exit(ERR_READ);
        }
        if (write(1, buffer, nread) != nread) {
                write(2, "A write error has occurred\n", 27);
                exit(ERR_WRITE);
        }
        return 0;
}
