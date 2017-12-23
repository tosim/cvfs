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
/*文件系统数据结构*/
struct inode
{
  struct inode *i_forw;
  struct inode *i_back;
  char i_flag;
  unsigned int i_ino;       /*磁盘i节点标志*/
  unsigned int i_count;     /*引用计数*/
  unsigned short di_number; /*关联文件数，当为0时，则删除该文件*/
  unsigned short di_mode;   /*存取权限*/
  unsigned short di_uid;    /*文件所有者*/
  unsigned short di_gid;
  unsigned short di_size;      /*文件大小*/
  unsigned int di_addr[NADDR]; /*物理块号*/
};
struct dinode
{
  unsigned short di_number; /*关联文件数*/
  unsigned short di_mode;   /*存取权限*/
  unsigned short di_uid;
  unsigned short di_gid;
  unsigned long di_size;       /*文件大小*/
  unsigned int di_addr[NADDR]; /*物理块号*/
};
struct direct
{
  char d_name[DIRSIZ];
  unsigned int d_ino;
};
struct filsys
{
  unsigned short s_isize;        /*i节点块数*/
  unsigned long s_fsize;         /*数据块数*/
  unsigned int s_nfree;          /*空闲块*/
  unsigned short s_pfree;        /*空闲块指针*/
  unsigned int s_free[NICFREE];  /*空闲块堆栈*/
  unsigned int s_ninode;         /*number of free inode in s_inode*/
  unsigned short s_pinode;       /*pointer of the sinode*/
  unsigned int s_inode[NICINOD]; /*空闲i节点数*/
  unsigned int s_rinode;         /*remember inode*/
  char s_fmod;                   /*超级块修改标志*/
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
  int size; /*当前目录大小*/
};
struct hinode
{
  struct inode *i_forw; /*hash表指针*/
};
struct file
{
  char f_flag;          /*文件操作标志*/
  unsigned int f_count; /*引用计数*/
  unsigned int f_inode; /*指向内存i节点*/
  unsigned long f_off;  /*read/write character pointer*/
};
struct user
{
  unsigned short u_default_mode;
  unsigned short u_uid;
  unsigned short u_gid;
  unsigned short u_ofile[NOFILE]; /*用户打开文件表*/
  /*system open file pointer number */
};

extern struct hinode hinode[NHINO];
extern struct dir dir; /*当前目录（在内存中全部读入）*/
extern struct file sys_ofile[SYSOPENFILE];
extern struct filsys filsys; /*内存中的超级块*/
extern struct pwd pwd[PWDNUM];/*保存系统中的用户密码*/
extern struct user user[USERNUM];/*保存登陆的用户*/
extern FILE *fd; /* the file system column of all the system*/
extern struct inode *cur_path_inode;
extern int user_id;/*当前用户index*/
extern int iHave_formated;/*是否格式化过磁盘*/
extern int iCur_free_block_index;

/*proptype of the sub roution used in the file system*/

//============================系统调用===================================//
//内存索引节点的分配
extern struct inode *iget(unsigned int dinodeid);
//内存索引节点的回收
extern int iput(struct inode *pinode);
//磁盘索引节点的分配
extern struct inode *ialloc();
//磁盘索引节点的回收
extern void ifree(unsigned dinodeid);
//成组链接法分配空闲盘块
extern unsigned int balloc();
//成组链接法回收盘块
extern void bfree(unsigned int block_num);
//格式化磁盘
extern void format();
//安装磁盘
extern void install();
//推出磁盘
extern void halt();
//目录搜索
extern unsigned int namei(char *name);
// 搜索当前目录下文件
extern unsigned short iname(char *name);
//============================系统调用===================================//

//============================文件接口===================================//
//打开文件
extern unsigned short open(int user_id, char *filename, unsigned short openmode);
//读取文件
extern unsigned int read(unsigned int user_id, int fd_1, char *buf, unsigned int size);
//写文件
extern unsigned int write(unsigned int user_id, int fd_2, char *buf, unsigned int size);
//关闭文件
extern void close(unsigned int user_id, unsigned short cfd);
//新建文件
extern int creat(unsigned int user_id, char *filename, unsigned short mode);
//当前目录下的文件信息
extern void _dir();
//创建文件夹
extern void mkdir(char *dirname);
//删除文件
extern void delete (char *filename);
//改变当前目录
extern void chdir(char *dirname);
//============================文件接口===================================//


//============================用户接口===================================//
//用户登陆
extern int login(unsigned short uid, char *passwd);
//用户注销
extern int logout(unsigned short uid);
//验证文件权限
extern unsigned int access(unsigned int user_id, struct inode *inode, unsigned short mode);
//用户帮助文档
extern void help();
//等待用户输入命令
extern int waitCommand(unsigned int user_id);
//============================用户接口===================================//
