#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>

#define NUM_BLOCKS 255
#define DIR_SIZE 256 //This goes into the first block of disk
char disk[NUM_BLOCKS]; //one block is for directory, rest for files

typedef struct file{
    char file_name;
    int file_size;
    int index_block;
}file_info;

file_info* dir[DIR_SIZE]; 

int modify(char name,int size);
int create(char name, int size, int printit);
int deletion(char name, int printit);
void printinfo(char name);

int create(char name, int size, int printit){
    //printf("hello");
    for (int i=0;i<DIR_SIZE;i++){
        if (dir[i]!=NULL && dir[i]->file_name==name){
            return modify(name,size);
        }
    }
    for (int i=0;i<NUM_BLOCKS;){
        int flag=1;
        int j;
        for (j=i;j<NUM_BLOCKS;j++){
            if (disk[j]!='*'){
                break;
            }  
            if (j-i+1==size){
                flag=0;
                break;
            }      
        }
        if (flag==0){
            for (int k=i;k<i+size;k++){
                disk[k]=name;
            }
            if (printit){
                printf("\n%d blocks of memory allocated succesfully to file %c...\n", size, name);
            }
            for (int l=0;l<DIR_SIZE;l++){
                if (dir[l]==NULL){
                    file_info* x=(file_info *)malloc(sizeof(file_info));
                    x->file_name=name;
                    x->file_size=size;
                    x->index_block=i;
                    dir[l]=x;
                    break;
                }
            }
            return 1;
        }
        i=j+1;
    }
    if (printit){
        printf("\nNot enough memory to allocate file %c of size %d.\n", name, size);
    }
    return -1;
}
int deletion(char name, int printit){
    //int count=0;
    int idx=-1;
    int size=0;
    for (int i=0;i<DIR_SIZE;i++){
        if (dir[i]!=NULL && dir[i]->file_name==name){
            idx=dir[i]->index_block;
            size=dir[i]->file_size;
            dir[i]=NULL;
            //count++;
            break;      
        }
    }
    if (idx==-1){
        if (printit){
            printf("\nNo deletion made\n");
        }
        return -1;
    }
    else if (idx>=0){
        for (int l=idx;l<idx+size;l++){
            disk[l]='*';
        }
        if (printit) printf("\n%c deleted successfully...\n", name); 
    }
    return 1;
}
int modify(char name, int size){
    int original_size=0;
    int idx_block=-1;
    for (int i=0;i<DIR_SIZE;i++){
        if (dir[i]!=NULL && dir[i]->file_name==name){
            original_size=dir[i]->file_size;
            idx_block=dir[i]->index_block;
            break;
        }
    }
    int y=deletion(name,0);
    if (create(name,size,0)==-1){
        for (int i=idx_block;i<idx_block+original_size;i++){
            disk[i]=name;
        }
        for (int l=0;l<DIR_SIZE;l++){
            if (dir[l]==NULL){
                file_info* x=(file_info *)malloc(sizeof(file_info));
                x->file_name=name;
                x->file_size=original_size;
                x->index_block=idx_block;
                dir[l]=x;
                break;
            }
        }
        printf("\n%c's modification is not possible\n", name);
        return -1;
    }
    printf("\n%c modified to size %d blocks\n", name, size);
    return 1;
}

void printinfo(char name){
    printf("\nInfo of file %c:\n", name);
    int idx;
    for(idx=0; idx<DIR_SIZE; idx++){
        if(dir[idx]!=NULL && dir[idx]->file_name==name){
            printf("\nName of the file - %c\nSize of the file - %d Blocks\nAddress of the index block - %d\n", name, dir[idx]->file_size,dir[idx]->index_block);
            printf("\nThe list of block addresses allocated to %c are:\n",name);
            for (int i=dir[idx]->index_block;i<dir[idx]->index_block+dir[idx]->file_size;i++){
                printf("%d\n",i);
            }
            return;
        }
    }
    if(idx==DIR_SIZE){
        printf("\n%c File does not exist in memory.\n",name);
    }
    return;
}

int main(){
    for (int i=0;i<NUM_BLOCKS;i++){
        disk[i]='*';
    }
    for (int i=0;i<DIR_SIZE;i++){
        dir[i]=NULL;
    }
    FILE *ptr;
    ptr=fopen("input.dat","r");
    if(ptr==NULL){
        printf("\nUnable to open file.\n");
        return 0;
    }
    char name, activity;
    int size;
    while(fscanf(ptr, "%c %c", &name, &activity)!=-1){
        //printf("%c is name, %c is activity",name,activity);
        // printf("lmao ");
        // printf("%c\n",name);
        //fscanf(ptr, "%c", &activity);
        //printf("%c\n",name);
        if (activity=='C'){
            fscanf(ptr, "%d\n", &size);
            int cr=create(name,size,1);
        }
        else if (activity=='D'){
            fscanf(ptr, "%d\n", &size);
            int del=deletion(name,1);
        }
        else if(activity=='P'){
            printinfo(name);
            fscanf(ptr, "\n");
        }
        else if (activity=='M'){
            fscanf(ptr, "%d\n", &size);
            //printf("%d\n",name);
            //printf("yes\n");
            //printf("%c",size);
            int mod=modify(name,size);
        }
        int i = 1;
        printf("\n");
        while(i <= NUM_BLOCKS){
            char name = disk[i-1];
            if(i%16 == 0)
            {
                printf("%c\n", name);
            }
            else{
                printf("%c", name);
            }
            i++;
        }
        printf("\n");
    }
    fclose(ptr);
    return 0;
}
