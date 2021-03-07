#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
 
#define R 0
#define W 1
 
int main(int argc, char *argv[])
{
    int numbers[100],cnt=0;
    int fd[2];
    for(int i=2;i<=35;++i)
    {
        numbers[cnt++]=i;
    }
    while(cnt>0)
    {
        pipe(fd);
        if(fork()==0)
            /*子进程存放从父进程传过来的数，直到写端口被关闭。然后再向
             子进程写入numbers数组中不能被thisprime整除的数*/
        {
            close(fd[W]);
            int this_prime,prime;
            cnt=-1;
            while(read(fd[R],&prime,sizeof(prime))!=0)
            {
                if(cnt==-1)
                {
                    this_prime=prime;
                    printf("prime %d\n",this_prime);
                    cnt++;
                }
                else
                {
                    if(prime%this_prime!=0)
                    {
                        numbers[cnt]=prime;
                        cnt++;
                    }
                }
            }
            close(fd[R]);
        }
        else
        {
            close(fd[R]);
            for(int i=0;i<cnt;++i)
            {
                write(fd[W],&numbers[i],sizeof(int));
            }
            close(fd[W]);
            wait(0);// 这边为什么要wait
            break;
        }
    }
    exit(0);
}
