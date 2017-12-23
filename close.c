#include "stdio.h"
#include "filesys.h"
void close(unsigned int user_id, unsigned short cfd) /*close*/
{
  struct inode *inode;
  int j, k;
  for (j = 0; j < USERNUM; j++)
    if (user[j].u_uid == user_id)
    {
      k = j;
      break;
    }

  inode = sys_ofile[user[k].u_ofile[cfd]].f_inode;

  iput(inode);
  sys_ofile[user[k].u_ofile[cfd]].f_count--;
  user[k].u_ofile[cfd] = SYSOPENFILE + 1;
}
