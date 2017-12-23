#include "stdio.h"
#include "string.h"
#include "filesys.h"
void _dir() /*dir*/
{
  unsigned int di_mode;
  int i, j, one;
  struct inode *temp_inode;

  //1.?????????
  // printf("\nCURRENT DIRECTORY:\n");
  printf("%14s  %23s  %47s\n","name","d_ino"," Ù–‘");
  for (i = 0; i < DIRNUM; i++)
  {
    if (!(strcmp(dir.direct[i].d_name, "")) && (dir.direct[i].d_ino == 0))
    {
      dir.size = i;
      break;
    }
  }

  //2.??????????
  for (i = 0; i < dir.size; i++)
  {

    if (dir.direct[i].d_ino != DIEMPTY)
    {
      printf("%14s  dir.direct[%d].d_ino = %-10d  Ù–‘:", dir.direct[i].d_name, i, dir.direct[i].d_ino);
      temp_inode = iget(dir.direct[i].d_ino);

      di_mode = temp_inode->di_mode;
      for (j = 0; j < 12; j++)
      {
        one = di_mode % 2;
        di_mode = di_mode / 2;
        if (one)
          printf("x");
        else
          printf("-");
      }

      if (temp_inode->di_mode & DIFILE)
      {
        printf("%15d  bytes\n", temp_inode->di_size);
        printf("block chain of the file:");
        for (j = 0; j < temp_inode->di_size / BLOCKSIZ + 1; j++)
          printf("%d--", temp_inode->di_addr[j]);

        printf("end\n");
      }
      else
        printf("<dir>\n");

      iput(temp_inode);
    }
  }
}

void mkdir(char *dirname) /*mkdir*/
{
  int dirid, dirpos;
  struct inode *inode;
  struct direct buf[BLOCKSIZ / (DIRSIZ + 2)];
  unsigned int block;

  memset(buf, 0, (BLOCKSIZ / (DIRSIZ + 2)) * sizeof(struct direct));
  dirid = namei(dirname);

  if (dirid != NULL)
  {
    inode = iget(dirid);
    if (inode->di_mode & DIDIR)
      printf("\n directory already existed!!\n");
    else
      printf("\n%s is a file name,&can't creat a dir the same name", dirname);
    iput(inode);
    return;
  }

  dirpos = iname(dirname);
  inode = ialloc();

  //  inode->i_ino=dirid;
  dir.direct[dirpos].d_ino = inode->i_ino;
  dir.size++;

  /* fill the new dir buf*/
  strcpy(buf[0].d_name, ".");
  buf[0].d_ino = inode->i_ino;
  strcpy(buf[1].d_name, "..");
  buf[1].d_ino = cur_path_inode->i_ino;

  block = balloc();

  fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
  fwrite(buf, 1, BLOCKSIZ, fd);
  inode->di_size = 2 * sizeof(struct direct);
  inode->di_number = 1;
  inode->di_mode = user[user_id].u_default_mode;
  inode->di_uid = user[user_id].u_uid | DIDIR;
  inode->di_gid = user[user_id].u_gid;
  inode->di_addr[0] = block;

  iput(inode);
  return;
}

void chdir(char *dirname) /*chdir*/
{
  unsigned int dirid;
  struct inode *inode;
  unsigned short block;
  int i, j, low = 0, high = 0;
  dirid = namei(dirname);

  if (dirid == NULL)
  {
    printf("\n %s does not existed\n", dirname);
    return;
  }

  inode = iget(dir.direct[dirid].d_ino);
  if (!access(user_id, inode, user[user_id].u_default_mode))
  {
    printf("\nThe directory %s ,Permission  deny", dirname);
    iput(inode);
    return;
  }

  /*pack the current directory
for(i=0;i<dir.size;i++)
{ for (;j<DIRNUM;j++)
   {
    printf("\nIn chdir(%s),j = %d",dirname,j);
    if (dir.direct[j].d_ino==0)   break;
    }
    printf("\nIn chdir(%s),j2 = %d",dirname,j);
  memcpy(&dir.direct[i],&dir.direct[j],DIRSIZ+2);
dir.direct[j].d_ino=0;
}
*/

  /* write back the current directory  */
  //for(i=0;i<cur_path_inode->di_size/BLOCKSIZ+1;i++)
  //   bfree(cur_path_inode->di_addr[i]);
  for (i = 0; i < dir.size; i += BLOCKSIZ / (sizeof(struct direct)))
  {
    if (i > 0)
    {
      block = balloc();
      cur_path_inode->di_addr[i] = block;
      fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
      fwrite(&dir.direct[i], 1, BLOCKSIZ, fd);
    }
    else
    {
      fseek(fd, DATASTART + cur_path_inode->di_addr[0] * BLOCKSIZ, SEEK_SET);
      fwrite(&dir.direct[0], 1, BLOCKSIZ, fd);
    }
  }
  cur_path_inode->di_size = dir.size * (sizeof(struct direct));
  iput(cur_path_inode);
  cur_path_inode = inode;

  /* read the change dir from disk */
  j = 0;
  for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
  {
    fseek(fd, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
    fread(&dir.direct[j], 1, BLOCKSIZ, fd);
    j += BLOCKSIZ / (DIRSIZ + 2);
  }
  return;
}
