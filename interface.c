#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "filesys.h"

void help() {
    printf("***************************************************************************\n");
    printf("* 1.mk + <string>                         新建一个文件,文件名为<string>   *\n");
    printf("* 2.rm + <string>                         删除文件名为<string>的文件      *\n");
    // printf("* 3.rename + <string> + <string>          重命名文件,第二个string是新名字 *\n");
    // printf("* 4.cp + <string>                         复制文件,文件名为<string>       *\n");
    // printf("* 5.paste                                 粘贴文件                        *\n");
    printf("* 3.read  + <string>                      读取文件,文件名为<string>       *\n");
    printf("* 4.write  + <filename> + <content>       将content写入文件<filename>     *\n");
    // printf("* 7.vi + <string>                         编辑文件,文件名为<string>       *\n");
    printf("* 5.mkdir + <string>                      新建目录,目录名为<string>       *\n");
    printf("* 6.rmdir + <string>                      删除目录,目录名为<string>       *\n");
    // printf("* 10.renamedir + <string> + <string>      重命名目录,第二个string是新名字 *\n");
    // printf("* 11.cpdir + <string>                     复制目录,目录名为<string>       *\n");
    // printf("* 12.pastedir                             粘贴目录                        *\n");
    printf("* 7.ls                                   显示目录下所有内容              *\n");
    printf("* 8.cd +<string>                         打开目录上一个目录或下一个目录  *\n");
    // printf("* 15.cd..                                 返回上一级目录                  *\n");
    printf("* 9.clear                                清屏                            *\n");
    printf("* 10.exit                                 注销                            *\n");
    printf("***************************************************************************\n");
}

void getParam(char* param){
    scanf("%s",param);
    getchar();
}
int waitCommand(unsigned int user_id) {
    int filed;
    char choice[10];
    char param[255];
    char readBuf[1024];
    char filename[255];
    while (1) {
        system("CLS");
        help();
        while (1) {
            scanf("%s",choice);
            getchar();
            if (!strcmp(choice,"mkdir")){
                getParam(param);
                mkdir(param);//创建目录
            }
            else if (!strcmp(choice,"mk")){
                getParam(param);
                creat(user_id, param, 01777);//创建文件
            }
            else if (!strcmp(choice,"rmdir")){
                getParam(param);
                delete(param);
            }
            // else if(!strcmp(choice,"pwd")){
            //     printf("%s\n",dir.d_name);
            // }
            else if (!strcmp(choice,"rm")){
                getParam(param);
                delete(param);
            }
            // else if (!strcmp(choice,"renamedir"))
            //     printf("get req\n");// fs.renameDir();
            // else if (!strcmp(choice,"rename"))
            //     printf("get req\n");// fs.renameFile();
            else if (!strcmp(choice,"cd")){
                getParam(param);
                chdir(param);
            }
            else if(!strcmp(choice,"write")){
                getParam(param);
                strcpy(filename,param);
                getParam(param);
                filed = open(user_id,filename, FAPPEND);
                write(user_id,filed, param, strlen(param));
                // printf("write %s %s size=%d\n",filename,param,sizeof(param));
            }
            else if (!strcmp(choice,"read")){
                getParam(param);
                filed = open(user_id, param, FREAD);
                read(user_id, filed,readBuf,BLOCKSIZ);
                printf("%s",readBuf);
            }
            else if (!strcmp(choice,"ls")){
                _dir();
                //printf("get req\n");// fs.showCurrentDir();
            }
            // else if (!strcmp(choice,"cpdir"))
            //     printf("get req\n");// fs.copyDir();
            // else if (!strcmp(choice,"cp"))
            //     printf("get req\n");// fs.copyFile();
            // else if (!strcmp(choice,"pastedir"))
            //     printf("get req\n");// fs.pasteDir();
            // else if (!strcmp(choice,"paste")) {
            //     printf("get req\n");// fs.pasteFile();
            // }
            // else if (!strcmp(choice,"vi"))
            //     printf("get req\n");// fs.edit();
            // else if (!strcmp(choice,"cd.."))
            //     printf("get req\n");// fs.goback();
            else if (!strcmp(choice,"clear")) {
                system("CLS");
                help();
            }
            else if (!strcmp(choice,"exit") ){
                system("CLS");
                printf( "用户: %d 正在注销....\n",user_id);
                Sleep(3);
                logout(user_id);
                halt();
                return 0;
            }
            else if (!strcmp(choice,"help")) {
                help();
            }
            else if (!strcmp(choice,"mkdir?"))
                printf( "创建新目录 ，格式为 mkdir [dirname]\n");
            else if (!strcmp(choice,"mk?"))
                printf( "创建新文件，格式为 mk[filename]\n");
            else if (!strcmp(choice,"rmdir?"))
                printf( "删除子目录，格式为 rmdir [dirname]\n");
            else if (!strcmp(choice,"rm?"))
                printf( "删除文件，输入格式为 rm [filename]\n");
            else if (!strcmp(choice,"renamedir?"))
                printf( "目录重命名 \n");
            else if (!strcmp(choice,"rename?"))
                printf( "文件重命名 \n");
            else if (!strcmp(choice,"cd?"))
                printf( "打开子目录，格式为 cd [dirname]\n");
            else if (!strcmp(choice,"read?"))
                printf( "查看文件内容，格式为 read [filename]\n");
            else if (!strcmp(choice,"ls?"))
                printf( "显示目录内容列表\n");
            else if (!strcmp(choice,"cpdir?"))
                printf( "复制子目录, 格式为 cpDir [dirname]\n");
            else if (!strcmp(choice,"cy?"))
                printf( "复制文件,格式为 cp [filename]\n");
            else if (!strcmp(choice,"pastedir?"))
                printf( "粘贴已复制的目录\n");
            else if (!strcmp(choice,"paste?"))
                printf( "粘贴已复制的文件\n");
            else if (!strcmp(choice,"clear?"))
                printf( "清屏\n");
            else if (!strcmp(choice,"vi?"))
                printf( "编辑文档，输入格式为 vi[filename]\n");
            else if (!strcmp(choice,"cd..?"))
                printf( "返回当前目录的上一级目录\n");
            else if (!strcmp(choice,"exit?"))
                printf( "注销文件系统\n");
            else
                printf( "命令无效！\n");
        }
    }
}
