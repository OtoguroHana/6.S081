#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[]) {
  int parent_to_child[2];
  int child_to_parent[2];
  char buf;
  int pid;

  pipe(parent_to_child);
  pipe(child_to_parent);

  if (fork() == 0)  // child process
  {
    close(parent_to_child[1]);
    close(child_to_parent[0]);
    if (read(parent_to_child[0], &buf, 1) == 1) {
      pid = getpid();
      printf("%d: received ping\n", pid);
      if (write(child_to_parent[1], &buf, 1) == 1) {
        close(child_to_parent[1]);
        close(parent_to_child[0]);
        exit(0);
      } else {
        fprintf(2, "child write error\n");
        exit(1);
      }
    } else {
      fprintf(2, "child read error\n");
      exit(1);
    }
  }

  else {
    close(parent_to_child[0]);
    close(child_to_parent[1]);
    if (write(parent_to_child[1], &buf, 1) == 1) {
      if (read(child_to_parent[0], &buf, 1) == 1) {
        pid = getpid();
        printf("%d: received pong\n", pid);
        wait(0);
        exit(0);
      } else {
        fprintf(2, "parent read error\n");
        exit(1);
      }
    } else {
      fprintf(2, "parent write error\n");
      exit(1);
    }
  }
}

