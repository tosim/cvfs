#include "stdio.h"
#include "filesys.h"
unsigned int read(unsigned int user_id, int fd_1, char *buf, unsigned int size)
{
  unsigned long off;
  int block, block_off, i, j, k;
  struct inode *inode;
  char temp_buf[size];
  char *temp;

  temp = temp_buf;

  for (j = 0; j < USERNUM; j++)
    if (user[j].u_uid == user_id)
    {
      k = j;
      break;
    }

  inode = sys_ofile[user[k].u_ofile[fd_1]].f_inode;
  if (!(sys_ofile[user[k].u_ofile[fd_1]].f_flag & FREAD))
  {
    printf("\n the file is not opened for read\n");
    return 0;
  }

  off = sys_ofile[user[k].u_ofile[fd_1]].f_off;
  if ((off + size) > inode->di_size)
    size = inode->di_size - off;
  block_off = off % BLOCKSIZ;
  block = off / BLOCKSIZ;
  if (block_off + size < BLOCKSIZ)
  {
    fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
    fread(buf, 1, size, fd);
    sys_ofile[user[k].u_ofile[fd_1]].f_off += size; //记录文件读写指针现在的偏移
    return size;
  }

  fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
  fread(temp, 1, BLOCKSIZ - block_off, fd);
  temp += BLOCKSIZ - block_off;

  for (i = 0; i < (size - (BLOCKSIZ - block_off)) / BLOCKSIZ; i++)
  {
    fseek(fd, DATASTART + inode->di_addr[block + 1 + i] * BLOCKSIZ, SEEK_SET);
    fread(temp, 1, BLOCKSIZ, fd);
    temp += BLOCKSIZ;
  }
  block_off = (size - block_off) % BLOCKSIZ; //读最后一块
  block = inode->di_addr[size / BLOCKSIZ + 1];
  fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
  fread(temp, 1, block_off, fd);
  sys_ofile[user[k].u_ofile[fd_1]].f_off += size;

  memcpy(buf, temp_buf, size);
  return size;
}

unsigned int write(unsigned int user_id, int fd_2, char *buf, unsigned int size) /*write*/
{
  unsigned long off;
  int block, block_off;
  int i, j, k;
  struct inode *inode;
  char *temp_buf;

  for (j = 0; j < USERNUM; j++)
    if (user[j].u_uid == user_id)
    {
      k = j;
      break;
    }

  inode = sys_ofile[user[k].u_ofile[fd_2]].f_inode;
  if ((!(sys_ofile[user[k].u_ofile[fd_2]].f_flag & FWRITE)) && (!(sys_ofile[user[k].u_ofile[fd_2]].f_flag & (FAPPEND))))
  {
    printf("\n the file is not opened for write or append\n");
    return 0;
  }
  temp_buf = buf;
  off = sys_ofile[user[k].u_ofile[fd_2]].f_off;
  block_off = off % BLOCKSIZ; //块内写入的起始位置
  block = off / BLOCKSIZ;     //要写入的文件内的块序号

  if (block_off + size < BLOCKSIZ) //写入后不超过该块的长度
  {
    fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
    fwrite(buf, 1, size, fd);
    inode->di_size = sys_ofile[user[k].u_ofile[fd_2]].f_off += size; //记录文件大小
    return size;
  }

  //写入后超过该块的长度,写入下一块
  fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
  fwrite(temp_buf, 1, BLOCKSIZ - block_off, fd); //填满该块
  temp_buf += BLOCKSIZ - block_off;

  for (i = 0; i < (size - (BLOCKSIZ - block_off)) / BLOCKSIZ; i++)
  {
    inode->di_addr[block + 1 + i] = balloc();
    fseek(fd, DATASTART + inode->di_addr[block + 1 + i] * BLOCKSIZ, SEEK_SET);
    fwrite(temp_buf, 1, BLOCKSIZ, fd); //写入下一空闲块
    temp_buf += BLOCKSIZ;
  }

  block_off = (size - (temp_buf - buf)) % BLOCKSIZ;

  inode->di_addr[block + size / BLOCKSIZ] = balloc();
  block = inode->di_addr[block + size / BLOCKSIZ];

  fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
  fwrite(temp_buf, 1, block_off, fd);                              //写入最后一块
  inode->di_size = sys_ofile[user[k].u_ofile[fd_2]].f_off += size; //记录文件大小

  return size;
}
