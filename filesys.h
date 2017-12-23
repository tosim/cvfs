#define BLOCKSIZ 512
#define SYSOPENFILE 40
#define DIRNUM 128
#define DIRSIZ 14
#define PWDSIZ 12
#define PWDNUM 32
#define NOFILE 20
#define NADDR 10
#define NHINO 128 /*must be power of 2 */
#define USERNUM 10
#define DINODESIZ 32
/*filsys*/
#define DINODEBLK 32
#define FILEBLK 512
#define NICFREE 50
#define NICINOD 50
#define DINODESTART 2 * BLOCKSIZ
#define DATASTART (2 + DINODEBLK) * BLOCKSIZ
/*di_node*/
#define DIEMPTY 00000
#define DIFILE 01000
#define DIDIR 02000
#define UDIREAD 00001 /*user*/
#define UDIWRITE 00002
#define UDIEXICUTE 00004
#define GDIREAD 00010 /* group*/
#define GDIWRITE 00020
#define GDIEXICUTE 00040
#define ODIREAD 00100 /*other*/
#define ODIWRITE 00200
#define ODIEXICUTE 00400
#define READ 1
#define WRITE 2
#define EXICUTE 3
#define DEFAULTMODE 00777
/*i_flag*/
#define IUPDATE 00002
/*s_fmod*/
#define SUPDATE 00001
/*f_flag*/
#define FREAD 00001
#define FWRITE 00002
#define FAPPEND 00004
/*error*/
#define DISKFULL 655535
/*fseek origin */
#define SEEK_SET 0
/*�ļ�ϵͳ���ݽṹ*/
struct inode
{
  struct inode *i_forw;
  struct inode *i_back;
  char i_flag;
  unsigned int i_ino;       /*����i�ڵ��־*/
  unsigned int i_count;     /*���ü���*/
  unsigned short di_number; /*�����ļ�������Ϊ0ʱ����ɾ�����ļ�*/
  unsigned short di_mode;   /*��ȡȨ��*/
  unsigned short di_uid;    /*�ļ�������*/
  unsigned short di_gid;
  unsigned short di_size;      /*�ļ���С*/
  unsigned int di_addr[NADDR]; /*������*/
};
struct dinode
{
  unsigned short di_number; /*�����ļ���*/
  unsigned short di_mode;   /*��ȡȨ��*/
  unsigned short di_uid;
  unsigned short di_gid;
  unsigned long di_size;       /*�ļ���С*/
  unsigned int di_addr[NADDR]; /*������*/
};
struct direct
{
  char d_name[DIRSIZ];
  unsigned int d_ino;
};
struct filsys
{
  unsigned short s_isize;        /*i�ڵ����*/
  unsigned long s_fsize;         /*���ݿ���*/
  unsigned int s_nfree;          /*���п�*/
  unsigned short s_pfree;        /*���п�ָ��*/
  unsigned int s_free[NICFREE];  /*���п��ջ*/
  unsigned int s_ninode;         /*number of free inode in s_inode*/
  unsigned short s_pinode;       /*pointer of the sinode*/
  unsigned int s_inode[NICINOD]; /*����i�ڵ���*/
  unsigned int s_rinode;         /*remember inode*/
  char s_fmod;                   /*�������޸ı�־*/
};
struct pwd
{
  unsigned short p_uid;
  unsigned short p_gid;
  char password[PWDSIZ];
};
struct dir
{
  // char d_name[DIRSIZ];
  struct direct direct[DIRNUM];
  int size; /*��ǰĿ¼��С*/
};
struct hinode
{
  struct inode *i_forw; /*hash��ָ��*/
};
struct file
{
  char f_flag;          /*�ļ�������־*/
  unsigned int f_count; /*���ü���*/
  unsigned int f_inode; /*ָ���ڴ�i�ڵ�*/
  unsigned long f_off;  /*read/write character pointer*/
};
struct user
{
  unsigned short u_default_mode;
  unsigned short u_uid;
  unsigned short u_gid;
  unsigned short u_ofile[NOFILE]; /*�û����ļ���*/
  /*system open file pointer number */
};

extern struct hinode hinode[NHINO];
extern struct dir dir; /*��ǰĿ¼�����ڴ���ȫ�����룩*/
extern struct file sys_ofile[SYSOPENFILE];
extern struct filsys filsys; /*�ڴ��еĳ�����*/
extern struct pwd pwd[PWDNUM];/*����ϵͳ�е��û�����*/
extern struct user user[USERNUM];/*�����½���û�*/
extern FILE *fd; /* the file system column of all the system*/
extern struct inode *cur_path_inode;
extern int user_id;/*��ǰ�û�index*/
extern int iHave_formated;/*�Ƿ��ʽ��������*/
extern int iCur_free_block_index;

/*proptype of the sub roution used in the file system*/

//============================ϵͳ����===================================//
//�ڴ������ڵ�ķ���
extern struct inode *iget(unsigned int dinodeid);
//�ڴ������ڵ�Ļ���
extern int iput(struct inode *pinode);
//���������ڵ�ķ���
extern struct inode *ialloc();
//���������ڵ�Ļ���
extern void ifree(unsigned dinodeid);
//�������ӷ���������̿�
extern unsigned int balloc();
//�������ӷ������̿�
extern void bfree(unsigned int block_num);
//��ʽ������
extern void format();
//��װ����
extern void install();
//�Ƴ�����
extern void halt();
//Ŀ¼����
extern unsigned int namei(char *name);
// ������ǰĿ¼���ļ�
extern unsigned short iname(char *name);
//============================ϵͳ����===================================//

//============================�ļ��ӿ�===================================//
//���ļ�
extern unsigned short open(int user_id, char *filename, unsigned short openmode);
//��ȡ�ļ�
extern unsigned int read(unsigned int user_id, int fd_1, char *buf, unsigned int size);
//д�ļ�
extern unsigned int write(unsigned int user_id, int fd_2, char *buf, unsigned int size);
//�ر��ļ�
extern void close(unsigned int user_id, unsigned short cfd);
//�½��ļ�
extern int creat(unsigned int user_id, char *filename, unsigned short mode);
//��ǰĿ¼�µ��ļ���Ϣ
extern void _dir();
//�����ļ���
extern void mkdir(char *dirname);
//ɾ���ļ�
extern void delete (char *filename);
//�ı䵱ǰĿ¼
extern void chdir(char *dirname);
//============================�ļ��ӿ�===================================//


//============================�û��ӿ�===================================//
//�û���½
extern int login(unsigned short uid, char *passwd);
//�û�ע��
extern int logout(unsigned short uid);
//��֤�ļ�Ȩ��
extern unsigned int access(unsigned int user_id, struct inode *inode, unsigned short mode);
//�û������ĵ�
extern void help();
//�ȴ��û���������
extern int waitCommand(unsigned int user_id);
//============================�û��ӿ�===================================//
