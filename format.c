#include "stdio.h"
#include "filesys.h"
void format()
{
  struct inode *inode;
  struct direct dir_buf[BLOCKSIZ / (DIRSIZ + 2)];
  struct pwd passwd[BLOCKSIZ / (PWDSIZ + 4)];
  /*    {
             {2116,03,"dddd"},
             {2117,03,"bbbb"},
             {2118,04,"abcd"},
             {2119,04,"cccc"},
             {2220,05,"eeee"},
            };
      */
  //struct filsys;
  unsigned int block_buf[BLOCKSIZ / sizeof(int)];
  char *buf;
  int i, j;
  /* creat the file system file */
  memset(dir_buf, 0, (BLOCKSIZ / (DIRSIZ + 2)) * sizeof(struct direct));
  fd = fopen("filesystem", "wb+");
  buf = (char *)malloc((DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char));
  if (buf == NULL)
    exit(0);

  printf("\nIn format(),file system file is created ,fd = %x\n", fd);

  fseek(fd, 0, SEEK_SET);
  fwrite(buf, 1, (DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char), fd);
  /*0.initialize the passwd*/

  passwd[0].p_uid = 2116;
  passwd[0].p_gid = 03;
  strcpy(passwd[0].password, "dddd");
  passwd[1].p_uid = 2117;
  passwd[1].p_gid = 03;
  strcpy(passwd[1].password, "bbbb");
  passwd[2].p_uid = 2118;
  passwd[2].p_gid = 04;
  strcpy(passwd[2].password, "abcd");
  passwd[3].p_uid = 2119;
  passwd[3].p_gid = 04;
  strcpy(passwd[3].password, "cccc");
  passwd[4].p_uid = 2220;
  passwd[4].p_gid = 05;
  strcpy(passwd[4].password, "eeee");

  /*给main()中的密码表pwd[PWDNUM]填入内容*/
  pwd[0].p_uid = 2116;
  pwd[0].p_gid = 03;
  strcpy(pwd[0].password, "dddd");
  pwd[1].p_uid = 2117;
  pwd[1].p_gid = 03;
  strcpy(pwd[1].password, "bbbb");
  pwd[2].p_uid = 2118;
  pwd[2].p_gid = 04;
  strcpy(pwd[2].password, "abcd");
  pwd[3].p_uid = 2119;
  pwd[3].p_gid = 04;
  strcpy(pwd[3].password, "cccc");
  pwd[4].p_uid = 2220;
  pwd[4].p_gid = 05;
  strcpy(pwd[4].password, "eeee");
  /*1.creat the main directory and its sub dir etc and the file password*/

  inode = iget(0); /*0 empty dinode id */
  inode->di_mode = DIEMPTY;
  iput(inode);

  inode = iget(1); /*1 main dir id*/
  inode->di_number = 1;
  inode->di_mode = DEFAULTMODE | DIDIR;
  inode->di_size = 3 * sizeof(struct direct);
  inode->di_addr[0] = 0; /* block 0# is used by the main directory*/
  strcpy(dir_buf[0].d_name, "..");
  dir_buf[0].d_ino = 1;
  strcpy(dir_buf[1].d_name, ".");
  dir_buf[1].d_ino = 1;
  strcpy(dir_buf[2].d_name, "etc");
  dir_buf[2].d_ino = 2;
  fseek(fd, DATASTART, SEEK_SET);
  fwrite(dir_buf, 1, 3 * sizeof(struct direct), fd);
  iput(inode);

  inode = iget(2); /*2 etc dir id*/
  inode->di_number = 1;
  inode->di_mode = DEFAULTMODE | DIDIR;
  inode->di_size = 3 * sizeof(struct direct);
  inode->di_addr[0] = 1; /*block 1# is used by the etc directory*/
  strcpy(dir_buf[0].d_name, "..");
  dir_buf[0].d_ino = 1;
  strcpy(dir_buf[1].d_name, ".");
  dir_buf[1].d_ino = 2;
  strcpy(dir_buf[2].d_name, "password");
  dir_buf[2].d_ino = 3;
  fseek(fd, DATASTART + BLOCKSIZ * 1, SEEK_SET);
  fwrite(dir_buf, 1, 3 * sizeof(struct direct), fd);
  iput(inode);

  inode = iget(3); /*3 password id*/
  inode->di_number = 1;
  inode->di_mode = DEFAULTMODE | DIFILE;
  inode->di_size = BLOCKSIZ;
  inode->di_addr[0] = 2; /* block 2# is used by the password file*/
  for (i = 5; i < PWDNUM; i++)
  {
    passwd[i].p_uid = 0;
    passwd[i].p_gid = 0;
    strcpy(passwd[i].password, "    ");
  }
  fseek(fd, DATASTART + 2 * BLOCKSIZ, SEEK_SET);
  fwrite(passwd, 1, BLOCKSIZ, fd);
  iput(inode);

  /*2.initialize the superblock*/
  filsys.s_isize = DINODEBLK;
  filsys.s_fsize = FILEBLK;
  filsys.s_ninode = DINODEBLK * BLOCKSIZ / DINODESIZ - 4;
  filsys.s_nfree = FILEBLK - 3;
  for (i = 0; i < NICINOD; i++)
  { /*begin with 4,0,1,2,3,is used by main,etc,password*/
    filsys.s_inode[i] = 4 + i;
  }
  filsys.s_pinode = 0;
  filsys.s_rinode = NICINOD + 4;
  block_buf[NICFREE - 1] = FILEBLK + 1; /*FILEBLK+1 is a flag of  end*/
  for (i = 0; i < NICFREE - 1; i++)
    block_buf[NICFREE - 2 - i] = FILEBLK - i;
  fseek(fd, DATASTART + BLOCKSIZ * (FILEBLK - NICFREE - 1), SEEK_SET);
  fwrite(block_buf, 1, BLOCKSIZ, fd);
  for (i = FILEBLK - NICFREE - 1; i > 2; i -= NICFREE)
  {
    for (j = 0; j < NICFREE; j++)
      block_buf[j] = i - j;
    block_buf[NICFREE] = NICFREE;
    fseek(fd, DATASTART + BLOCKSIZ * (i - 1), SEEK_SET);
    fwrite(block_buf, 1, BLOCKSIZ, fd);
  }

  i += NICFREE;
  iCur_free_block_index = i;

  for (i = i, j = 1; i > 3; i--, j++)
    filsys.s_free[NICFREE - j] = i;
  filsys.s_pfree = NICFREE - j + 1;

  filsys.s_pinode = 0;
  fseek(fd, BLOCKSIZ, SEEK_SET);
  fwrite(&filsys, 1, sizeof(struct filsys), fd);

  fclose(fd);
}
