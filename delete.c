#include "stdio.h"
#include "filesys.h"
void delete (char *filename)
{
    unsigned int dirid;
    struct inode *inode;
    int iReturn_value;
    int i;

    dirid = namei(filename);

    if (dirid != NULL)
        inode = iget(dir.direct[dirid].d_ino);

    inode->di_number--;

    iReturn_value = iput(inode);
    if (iReturn_value = 1)
    {
        //É¾³ýÄ¿Â¼Ïî
        strcpy(dir.direct[dirid].d_name, "");
        dir.direct[dirid].d_ino = 0;

        for (; dirid <= dir.size - 1; dirid++)
            if ((strcmp(dir.direct[dirid + 1].d_name, "")) && (dir.direct[dirid + 1].d_ino != 0))
            {
                memcpy(&dir.direct[dirid], &dir.direct[dirid + 1], sizeof(struct direct));
                memset(&dir.direct[dirid + 1], 0, sizeof(struct direct));
            }

        dir.size = dir.size - 1;
    }
    else
        printf("\nIn delete(%s),filename's  i_count>1, now i_count-1 ,but this file has not been deleted\n", filename);
}
