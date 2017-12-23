#include "stdio.h"
#include "filesys.h"

struct hinode hinode[NHINO];
struct dir dir;
struct file sys_ofile[SYSOPENFILE];
struct filsys filsys;
struct pwd pwd[PWDNUM];
struct user user[USERNUM];
FILE *fd;
struct inode *cur_path_inode;
int user_id;
int iHave_formated;
int iCur_free_block_index;

int main()
{
      unsigned short ab_fd1, ab_fd2, ab_fd3, ab_fd4, ab_fd5;
      char *buf;
      char file_content_buf[BLOCKSIZ];
      char c;

      /*printf("begin:\n");
      printf("\nDo you want to format the disk?\n");
      if (getchar() == 'y')
            printf("\nFormat will erase context on the disk \n Are you sure?\n");
      getchar();
      if ((c = getchar()) == 'y')
      {
            printf("format starts\n");
            format();
            iHave_formated = 1;
            printf("format ended\n");
      }
      else
            printf("received char :%c from stdin\n", c);

      printf("step 1 ended\n");//
      install();*/
      format();
      iHave_formated = 1;
      install();

      printf("user 2118 login\n");
      login(2118, "abcd");
      user_id = 0;
      waitCommand(2118);
      // logout(2118);
      // halt();
      // _dir();
      // mkdir("a2118");//创建目录
      // _dir();//ls
      // chdir("a2118");//改变目录
      // _dir();
      

      // printf("step 2 ended\n");
      // _dir();
      // printf("step 3 ended\n");
      
      // user_id = 0;
      
      // printf("step 4 ended\n");

      // mkdir("a2118");//创建目录
      // _dir();//ls
      // printf("step 5-1 ended\n");

      // chdir("a2118");//改变目录
      // _dir();
      // printf("step 5-2 ended\n");

      // ab_fd1 = creat(2118, "ab_file0.c", 01777);//创建文件
      // _dir();//ls
      // printf("step 6 ended\n");

      // buf = (char *)malloc(BLOCKSIZ * 6 + 5);
      // printf("step 7 ended\n");
      // write(2118, ab_fd1, buf, BLOCKSIZ * 6 + 5);//写文件
      // printf("step 8 ended\n");
      // close(2118, ab_fd1);//关闭文件
      // printf("step 9 ended\n");
      // free(buf);

      // mkdir("subdir");
      // printf("step 10-1 ended\n");

      // chdir("subdir");
      // printf("step 10-2 ended\n");

      // ab_fd2 = creat(2118, "file1.c", 01777);
      // printf("step 11 ended\n");
      // buf = (char *)malloc(BLOCKSIZ * 4 + 20);
      // printf("step 12 ended\n");
      // write(2118, ab_fd2, buf, BLOCKSIZ * 4 + 20);
      // printf("step 13 ended\n");
      // close(2118, ab_fd2);
      // free(buf);
      // printf("step 14 ended\n");
      // chdir("..");
      // printf("step 15 ended\n");

      // ab_fd3 = creat(2118, "ab_file2.c", 01777);
      // buf = (char *)malloc(BLOCKSIZ * 10 + 255);
      // write(2118, ab_fd3, buf, BLOCKSIZ * 3 + 255);
      // close(2118, ab_fd3);
      // free(buf);

      // delete ("ab_file0.c");
      // printf("step 16 ended\n");
      // ab_fd4 = creat(2118, "ab_file3.c", 01777);
      // buf = (char *)malloc(BLOCKSIZ * 8 + 300);
      // printf("step 17 ended\n");
      // write(2118, ab_fd4, buf, BLOCKSIZ * 8 + 300);
      // printf("step 18 ended\n");
      // close(2118, ab_fd4);
      // free(buf);
      // printf("step 19 ended\n");

      // ab_fd3 = open(2118, "ab_file2.c", FAPPEND);
      // printf("step 20 ended\n");
      // buf = (char *)malloc(BLOCKSIZ * 3 + 100);
      // write(2118, ab_fd3, buf, BLOCKSIZ * 3 + 100);
      // printf("step 21 ended,ab_fd3 = %d\n", ab_fd3);
      // close(2118, ab_fd3);

      // ab_fd5 = creat(2118, "文件5", 01777);
      // write(2118, ab_fd5, "\nFly me to the moon\nAnd let me play among the stars\nLet me see what Spring is like\nOn Jupiter and  Mars\n带我飞向月球,让我在群星中嬉戏,让我看看木星和火星上的春天\n", BLOCKSIZ);
      // close(2118, ab_fd5);
      // ab_fd5 = open(2118, "文件5", FREAD);
      // read(2118, ab_fd5, file_content_buf, BLOCKSIZ);
      // printf("读出文件5的内容:  %s\n", file_content_buf);
      // close(2118, ab_fd5);

      // printf("step 22 ended\n");
      // free(buf);
      // _dir();
      // chdir("..");
      // printf("\nstep 23 ended\n");
      return 0;
}
