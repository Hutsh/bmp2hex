#include <iostream>

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <fstream>

void strRev(char *s){
    for(char *end = s + strlen(s) - 1; end > s ; --end, ++s){
        *s ^= *end ^= *s ^= *end;
    }
}

int main(int argc, char * argv[])
{

    FILE * fp, *out;
    int i, j, k;
    char filepath[100];
    char filename[50];
    if(argc==1){
        printf("===================Bmp2Hex===================\n");
        printf("=====Transform 24bit bmp file to Hex file====\n");
        printf("==============author:HuTianshuo==============\n");
        printf("=============================================\n");
        printf("Please input bmp filename(***.bmp): ");
        scanf("%s", filepath);
        for(i=0;filepath[i] != '.';i++){
            filename[i]=filepath[i];
        }
        filename[i++]='\0';
        if((fp = fopen(filepath, "rb")) == NULL){
        printf("Can't open BMP file!\n");
        system("PAUSE");
        return 1;
        }
    }
    else{//拽进来
        printf("======================Bmp2Hex======================\n");
        printf("========Transform 24bit bmp file to Hex file=======\n");
        printf("=================author:HuTianshuo=================\n");
        printf("===================================================\n");
        sprintf(filepath,"%s",argv[1]);
        for(i =0; filepath[i] != '\0'; i++){}

        if(filepath[i-1] == 'p' && filepath[i-2] == 'm' && filepath[i-3] == 'b' &&filepath[i-4] == '.'){//确认格式
            for(j=0,k=i-5; filepath[k] != '\\';j++,k--){
                filename[j]=filepath[k];
            }
            filename[j]='\0';
            strRev(filename);//获取文件名
        }
        else{
            printf("\n\n!!!!!!!WORONG FORMAT!!!!!!!\nPRESS ANY KEY TO EXIT.");
            getch();
            exit(1);
        }

        if((fp = fopen(filepath, "rb")) == NULL){//打开文件
        printf("Can't open BMP file!\n");
        system("PAUSE");
        return 1;
        }
    }


    fseek(fp, 0xa, 0);//0xa为信息偏移量的偏移量
    int InfoOffset[1];
    fread(InfoOffset, sizeof(char), 4, fp);//读取0xa位置的偏移量

    int twidth[1];int theight[1];
    fseek(fp, 0x12, 0);//0x12长款，各4字节
    fread(twidth, sizeof(char), 4, fp);
    fread(theight, sizeof(char), 4, fp);
    int width = twidth[0], height = theight[0];//获取长宽

    printf("Filename: %s.bmp\nImage size: %d*%d\npress any key to start",filename, width, height);
    getch();

    char outname[50];
    strcpy(outname, filename);
    strcat(outname, "_hex.h");
    out = fopen(outname, "w+");

#if 1


    printf("\ntransforming...\r\n");

    fprintf(out, "/*Image size: %d*%d*/\n", width, height);
    fprintf(out, "U32 PIC%s[%d][%d] = {\n",filename,height, width);

    int fill0 = width%4;//bmp每行信息最后按4的倍数补零
    int lineoffset = InfoOffset[0] + (height - 1) * ((3 * width)+fill0);//起始0x36+(高-1)*(宽度*3字节+补零),到最后一行首字节

    int alltask=height,finishtask=0,persentagebar=0,len=50;//进度条
    char *bar = (char *)malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len + 1; ++i)bar[i] = '#';
    float persentage=0.0;


    for(int h = 0; h < height; h++) {//每一行
        fseek(fp, lineoffset, 0);

        for(int w = 0; w < width; w++) {//每个块0x00BGR
            fprintf(out, "0x00");
            for(int si = 0; si < 3; si++) fprintf(out, "%02x", fgetc(fp));
            if((h != height - 1) || (w != width - 1)) fprintf(out, ",");//不是最后一行最后一列，打逗号
        }
        lineoffset -= ((3 * width)+fill0);//偏移量到上一行
        fprintf(out, "\n");

        finishtask++;persentage = (float)finishtask/(float)alltask*100;
        persentagebar=(int)(persentage/3);
        printf("progress:[%s]%.2f%\r", bar+len-persentagebar, persentage);//打印进度条
        fflush(stdout);
    }

    fprintf(out, "};");
    fclose(out);
    fclose(fp);

    printf("\n\n!!!!!!!!!!!!!!!!!!SUCCESS!!!!!!!!!!!!!!!!!!!\n");
    system("PAUSE");
#endif

    return 0;
}

