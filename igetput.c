#include "stdio.h"
#include "stdlib.h"
#include "filesys.h"

struct inode *iget(unsigned int dinodeid) /*iget( )*/
{
  int existed = 0, inodeid;
  long addr;
  struct inode *temp;
  struct inode *newinode;
  inodeid = dinodeid % NHINO;

  if (hinode[inodeid].i_forw == NULL)
  {

    existed = 0;
  }
  else
  {
    temp = hinode[inodeid].i_forw;
    while (temp)
      if (temp->i_ino == inodeid) /* existed */
      {
        existed = 1;
        temp->i_count++;
        return temp;
      }
      else
        temp = temp->i_forw; /* not existed */
  }
  /* not existed */
  /*1. calculate the addr of the dinode in the file sys column */

  addr = DINODESTART + dinodeid * sizeof(struct dinode);

  /*2. malloc the new inode */
  newinode = (struct inode *)malloc(sizeof(struct inode));
  memset(newinode, 0, (sizeof(struct inode)));

  /*3. read the dinode to the inode */
  fseek(fd, addr, SEEK_SET);
  fread(&(newinode->di_number), sizeof(struct dinode), 1, fd);

  /*4. put it into hinode [inodeid] queue */

  newinode->i_forw = hinode[inodeid].i_forw;
  newinode->i_back = newinode;

  if (newinode->i_forw != NULL)
    newinode->i_forw->i_back = newinode;

  hinode[inodeid].i_forw = newinode;

  /*5. initialize the inode */
  newinode->i_count = 1;
  newinode->i_flag = 0; /* flag for not update */
  newinode->i_ino = dinodeid;
  return newinode;
}

int iput(struct inode *pinode) /*iput( )*/
{
  int i = 0, inodeid;
  long addr;
  unsigned int block_num;
  struct inode temp;

  inodeid = pinode->i_ino % NHINO;

  if (pinode->i_count > 1)
  {
    pinode->i_count--;
    return 1;
  }
  else
  {
    if (pinode->di_number != 0)
    { /* write back the inode */
      addr = DINODESTART + pinode->i_ino * sizeof(struct dinode);
      fseek(fd, addr, SEEK_SET);
      fwrite(&(pinode->di_number), sizeof(struct dinode), 1, fd);

      fseek(fd, addr, SEEK_SET);
      fread(&(temp.di_number), sizeof(struct dinode), 1, fd);
    }
    else
    {
      /* rm the inoide & the block of the file in the disk */
      block_num = pinode->di_size / BLOCKSIZ;
      for (i = 0; i < block_num; i++)
        //bfree(pinode->di_addr[i]);
        ifree(pinode->i_ino);
    }
    /*free the inode in the memory */
    if (pinode->i_forw == NULL)
      pinode->i_back->i_forw = NULL;
    else
    {
      pinode->i_forw->i_back = pinode->i_back;
      pinode->i_back->i_forw = pinode->i_forw;
    }

    if (pinode->i_back == pinode) //hash表中该列剩下最后一个inode
      hinode[inodeid].i_forw = NULL;
    free(pinode);
  }
  return 0;
}
