#include "stdio.h"
#include "filesys.h"
unsigned short open(int user_id, char *filename, unsigned short openmode)
{
    unsigned int dirid;
    struct inode *inode;
    int i, j, k;
    dirid = namei(filename);
    if (dirid == NULL) /*no such file */
    {
        printf("\n file does not existed!!!\n");
        return NULL;
    }

    inode = iget(dir.direct[dirid].d_ino);
    if (!access(user_id, inode, openmode)) /*access denied*/
    {
        printf("\n file open has not access!!!");
        iput(inode);
        return NULL;
    }

    /* alloc the sys_ofile item*/
    for (i = 1; i < SYSOPENFILE; i++)
        if (sys_ofile[i].f_count == 0)
            break;
    if (i == SYSOPENFILE)
    {
        printf("\n system open file too much\n");
        iput(inode);
        return NULL;
    }

    sys_ofile[i].f_inode = inode;
    sys_ofile[i].f_flag = openmode;
    sys_ofile[i].f_count = 1;
    if (openmode & FAPPEND)
        sys_ofile[i].f_off = inode->di_size;
    else
        sys_ofile[i].f_off = 0;

    /*alloc the user open file item*/
    for (j = 0; j < USERNUM; j++)
        if (user[j].u_uid == user_id)
        {
            k = j;
            break;
        }

    for (j = 0; j < NOFILE; j++)
        if (user[k].u_ofile[j] == SYSOPENFILE + 1)
            break;

    if (j == NOFILE)
    {
        printf("\n user open file too much!!!\n");
        sys_ofile[i].f_count = 0;
        iput(inode);
        return NULL;
    }
    user[k].u_ofile[j] = i; //记录用户打开的文件在sys_ofile[ ]中的位置

    /*if APPEND,free the block of the file before*/
    if (openmode & FAPPEND)
    {
        for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
            bfree(inode->di_addr[i]);
        inode->di_size = 0;
    }
    return j;
}
