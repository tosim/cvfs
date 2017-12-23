#include "stdio.h"
#include "string.h"
#include "filesys.h"

void install()
{
  int i, j;
  int return_value;
  /*0. open the file column*/

  if (iHave_formated == 1)
    fd = fopen("filesystem", "rb+");
  else
    // fd = fopen("filesystem", "w+r+b");
    fd = fopen("filesystem", "wb+");
  if (fd == NULL)
  {
    printf("\n filesys can not be loaded \n");
    exit(0);
  }
  printf("\nIn install(), filesys has been loaded,fd = %x\n", fd);
  /*1. read the filsys from the superblock*/
  fseek(fd, BLOCKSIZ, SEEK_SET);
  // fwrite(&filsys,1,sizeof(struct filsys),fd);  原来的代码
  fread(&filsys, sizeof(struct filsys), 1, fd);

  /*2. initialize the inode hash chain */

  for (i = 0; i < NHINO; i++)
    hinode[i].i_forw = NULL;
  /*3. initialize the sys_ofile */

  for (i = 0; i < SYSOPENFILE; i++)
  {
    sys_ofile[i].f_count = 0;
    sys_ofile[i].f_inode = NULL;
  }
  /*4. initialize the user */

  for (i = 0; i < USERNUM; i++)
  {
    user[i].u_uid = 0;
    user[i].u_gid = 0;
    for (j = 0; j < NOFILE; j++)
      user[i].u_ofile[j] = SYSOPENFILE + 1;
  }
  /*5. read the main directory to initialize the dir */
  cur_path_inode = iget(1);

  dir.size = cur_path_inode->di_size / (sizeof(struct direct));
  strcpy(dir.d_name,"/");
  
  for (i = 0; i < DIRNUM; i++)
  {
    strcpy(dir.direct[i].d_name, "");
    dir.direct[i].d_ino = 0;
  }

  for (i = 0; i < dir.size / (BLOCKSIZ / (DIRSIZ + 2)); i++)
  {

    fseek(fd, DATASTART + BLOCKSIZ * cur_path_inode->di_addr[i], SEEK_SET);
    fread(&dir.direct[(BLOCKSIZ / (DIRSIZ + 2)) * i], 1, BLOCKSIZ, fd);
  }

  fseek(fd, DATASTART + BLOCKSIZ * cur_path_inode->di_addr[i], SEEK_SET);
  if (i == 0)
    fread(&dir.direct[BLOCKSIZ / (DIRSIZ + 2) * i], 1, cur_path_inode->di_size, fd);
  else //i>1
    fread(&dir.direct[BLOCKSIZ / (DIRSIZ + 2) * i], 1, cur_path_inode->di_size % BLOCKSIZ, fd);
}
