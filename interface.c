#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "filesys.h"

void help() {
    printf("***************************************************************************\n");
    printf("* 1.mk + <string>                         �½�һ���ļ�,�ļ���Ϊ<string>   *\n");
    printf("* 2.rm + <string>                         ɾ���ļ���Ϊ<string>���ļ�      *\n");
    // printf("* 3.rename + <string> + <string>          �������ļ�,�ڶ���string�������� *\n");
    // printf("* 4.cp + <string>                         �����ļ�,�ļ���Ϊ<string>       *\n");
    // printf("* 5.paste                                 ճ���ļ�                        *\n");
    printf("* 3.read  + <string>                      ��ȡ�ļ�,�ļ���Ϊ<string>       *\n");
    printf("* 4.write  + <filename> + <content>       ��contentд���ļ�<filename>     *\n");
    // printf("* 7.vi + <string>                         �༭�ļ�,�ļ���Ϊ<string>       *\n");
    printf("* 5.mkdir + <string>                      �½�Ŀ¼,Ŀ¼��Ϊ<string>       *\n");
    printf("* 6.rmdir + <string>                      ɾ��Ŀ¼,Ŀ¼��Ϊ<string>       *\n");
    // printf("* 10.renamedir + <string> + <string>      ������Ŀ¼,�ڶ���string�������� *\n");
    // printf("* 11.cpdir + <string>                     ����Ŀ¼,Ŀ¼��Ϊ<string>       *\n");
    // printf("* 12.pastedir                             ճ��Ŀ¼                        *\n");
    printf("* 7.ls                                   ��ʾĿ¼����������              *\n");
    printf("* 8.cd +<string>                         ��Ŀ¼��һ��Ŀ¼����һ��Ŀ¼  *\n");
    // printf("* 15.cd..                                 ������һ��Ŀ¼                  *\n");
    printf("* 9.clear                                ����                            *\n");
    printf("* 10.exit                                 ע��                            *\n");
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
                mkdir(param);//����Ŀ¼
            }
            else if (!strcmp(choice,"mk")){
                getParam(param);
                creat(user_id, param, 01777);//�����ļ�
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
                printf( "�û�: %d ����ע��....\n",user_id);
                Sleep(3);
                logout(user_id);
                halt();
                return 0;
            }
            else if (!strcmp(choice,"help")) {
                help();
            }
            else if (!strcmp(choice,"mkdir?"))
                printf( "������Ŀ¼ ����ʽΪ mkdir [dirname]\n");
            else if (!strcmp(choice,"mk?"))
                printf( "�������ļ�����ʽΪ mk[filename]\n");
            else if (!strcmp(choice,"rmdir?"))
                printf( "ɾ����Ŀ¼����ʽΪ rmdir [dirname]\n");
            else if (!strcmp(choice,"rm?"))
                printf( "ɾ���ļ��������ʽΪ rm [filename]\n");
            else if (!strcmp(choice,"renamedir?"))
                printf( "Ŀ¼������ \n");
            else if (!strcmp(choice,"rename?"))
                printf( "�ļ������� \n");
            else if (!strcmp(choice,"cd?"))
                printf( "����Ŀ¼����ʽΪ cd [dirname]\n");
            else if (!strcmp(choice,"read?"))
                printf( "�鿴�ļ����ݣ���ʽΪ read [filename]\n");
            else if (!strcmp(choice,"ls?"))
                printf( "��ʾĿ¼�����б�\n");
            else if (!strcmp(choice,"cpdir?"))
                printf( "������Ŀ¼, ��ʽΪ cpDir [dirname]\n");
            else if (!strcmp(choice,"cy?"))
                printf( "�����ļ�,��ʽΪ cp [filename]\n");
            else if (!strcmp(choice,"pastedir?"))
                printf( "ճ���Ѹ��Ƶ�Ŀ¼\n");
            else if (!strcmp(choice,"paste?"))
                printf( "ճ���Ѹ��Ƶ��ļ�\n");
            else if (!strcmp(choice,"clear?"))
                printf( "����\n");
            else if (!strcmp(choice,"vi?"))
                printf( "�༭�ĵ��������ʽΪ vi[filename]\n");
            else if (!strcmp(choice,"cd..?"))
                printf( "���ص�ǰĿ¼����һ��Ŀ¼\n");
            else if (!strcmp(choice,"exit?"))
                printf( "ע���ļ�ϵͳ\n");
            else
                printf( "������Ч��\n");
        }
    }
}
