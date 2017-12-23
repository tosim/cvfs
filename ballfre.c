#include "stdio.h"
#include "filesys.h"
static unsigned int block_buf[BLOCKSIZ];

unsigned int balloc()
{
  unsigned int free_block, free_block_num;
  int i;

  if (filsys.s_nfree == 0)
  {
    printf("\n Disk Full!!!\n");
    return DISKFULL;
  }

  free_block = filsys.s_free[filsys.s_pfree];
  if (filsys.s_pfree == NICFREE - 1)
  {
    fseek(fd, DATASTART + BLOCKSIZ * (iCur_free_block_index + NICFREE - 1), SEEK_SET);
    fread(block_buf, 1, BLOCKSIZ, fd);
    free_block_num = block_buf[NICFREE]; /*the total block num in the group */
    for (i = 0; i < free_block_num; i++)
      filsys.s_free[NICFREE - 1 - i] = block_buf[i];
    filsys.s_pfree = NICFREE - free_block_num;
  }
  else
    filsys.s_pfree += 1;
  filsys.s_nfree -= 1;
  filsys.s_fmod = SUPDATE;
  return free_block;
}

void bfree(unsigned int block_num) 
{
  int i;

  if (block_num >= 0 && block_num <= 3)
    return;
  filsys.s_pfree -= 1;
  if (filsys.s_pfree == 0 && iCur_free_block_index != 11) /*s_free full */
  {
    block_buf[NICFREE] = NICFREE;
    for (i = 0; i < NICFREE; i++)
      block_buf[i] = filsys.s_free[NICFREE - 1 - i];
    filsys.s_pfree = NICFREE - 1;
  }

  if (iCur_free_block_index != 11)
  {
    iCur_free_block_index -= NICFREE;
    fseek(fd, DATASTART + BLOCKSIZ * (iCur_free_block_index - 1), SEEK_SET);
    fread(block_buf, 1, BLOCKSIZ, fd);
    filsys.s_nfree++;
    filsys.s_fmod = SUPDATE;
  }
}
