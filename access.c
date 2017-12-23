#include "stdio.h"
#include "filesys.h"
unsigned int access(unsigned int user_id, struct inode *inode, unsigned short mode)
{
  int j, k;
  for (j = 0; j < USERNUM; j++)
    if (user[j].u_uid == user_id)
    {
      k = j;
      break;
    }

  switch (mode)
  {
  case READ:
    if (inode->di_mode & ODIREAD)
      return 1;
    if ((inode->di_mode & GDIREAD) && (user[k].u_gid == inode->di_gid))
      return 1;
    if ((inode->di_mode & UDIREAD) && (user[k].u_uid == inode->di_uid))
      return 1;
    return 0;
  case WRITE:
    if (inode->di_mode & ODIWRITE)
      return 1;
    if ((inode->di_mode & GDIWRITE) && (user[k].u_gid == inode->di_gid))
      return 1;
    if ((inode->di_mode & UDIWRITE) && (user[k].u_uid == inode->di_uid))
      return 1;
    return 0;
  case EXICUTE:
    if (inode->di_mode & ODIEXICUTE)
      return 1;
    if ((inode->di_mode & GDIEXICUTE) && (user[k].u_gid == inode->di_gid))
      return 1;
    if ((inode->di_mode & UDIEXICUTE) && (user[k].u_uid == inode->di_uid))
      return 1;
    return 0;

  case FAPPEND:
    return 1;

  case DEFAULTMODE:
    return 1;
  default:
    return 0;
  }
}
