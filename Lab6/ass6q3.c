#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>

#define NUM_BLOCKS 255
#define DIR_SIZE 256 //This goes into the first block of disk

typedef struct file{
    char file_name;
    int file_size;
    int index_block;
}file_info;

typedef struct disk_block{
    char file_name;
}block;

typedef struct idx_block{
    int address_of_file[NUM_BLOCKS];
}index_block;

index_block* list_of_files[NUM_BLOCKS];

file_info* dir[DIR_SIZE]; 
block* disk[NUM_BLOCKS];

//'.' denotes the index block

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
    int temp_size=size+1;
    for (int i=0;i<NUM_BLOCKS;i++){
        if (disk[i]->file_name=='*'){
            temp_size--;
        }
        if (temp_size==0){
            break;
        }
    }
    if (temp_size>0){
        if (printit){
            printf("\nNot enough memory to allocate file %c of size %d.\n", name, size);
        }
        return -1;
    }   
    else{
        int flag=1;
        temp_size=size+1;
        int file_address=-1;
        int count=0;
        if (printit){
            printf("\n%d blocks of memory allocated succesfully to file %c...\n", size, name);
        }
        for (int i=0;i<NUM_BLOCKS;i++){
            if (disk[i]->file_name=='*' && flag && temp_size){
                for (int k=0;k<DIR_SIZE;k++){
                    if (dir[k]==NULL){
                        file_info* x=(file_info *)malloc(sizeof(file_info));
                        x->file_name=name;
                        x->file_size=size;
                        x->index_block=i;
                        dir[k]=x;
                        break;
                    }
                }
                flag=0;
                disk[i]->file_name='.';
                file_address=i;
                list_of_files[file_address]=(index_block*)malloc(sizeof(index_block));
                for (int l=0;l<NUM_BLOCKS;l++){
                    list_of_files[file_address]->address_of_file[l]=-1;
                    //printf("%d",list_of_files[file_address]->address_of_file[l]);
                }
                temp_size--;
            }
            else if (count<NUM_BLOCKS && disk[i]->file_name=='*' && temp_size){
                disk[i]->file_name=name;
                list_of_files[file_address]->address_of_file[count++]=i;
                temp_size--;
            }
            if (!temp_size){
                break;
            }
        }
    }    
    return 1;
}
int deletion(char name, int printit){
    //int count=0;
    int idx=-1;
    for (int i=0;i<DIR_SIZE;i++){
        if (dir[i]!=NULL && dir[i]->file_name==name){            
            idx=dir[i]->index_block;
            dir[i]=NULL;
            break;    
        }
    }
    if (idx==-1){
        if (printit){
            printf("\nNo deletion made\n");
        }
        return -1;
    }
    int i=0;
    while (i<NUM_BLOCKS && list_of_files[idx]->address_of_file[i]!=-1){
        disk[list_of_files[idx]->address_of_file[i]]->file_name='*';
        list_of_files[idx]->address_of_file[i]=-1;
        i++;
    }
    list_of_files[idx]=NULL;
    disk[idx]->file_name='*';
    if (printit){
        printf("\n%c deleted successfully...\n", name);
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
        }
    }
    int count=0;
    for (int i=0;i<NUM_BLOCKS;i++){
        if (disk[i]->file_name=='*' || disk[i]->file_name==name){
            count++;
        }
    }
    //printf("%d",count);
    if (count<size){
        printf("\n%c's modification is not possible\n", name);
        return -1;
    }
    deletion(name,0);
    int lol=create(name,size,0);
    printf("\n%c modified to size %d blocks\n", name, size);
    return 1;
}

void printinfo(char name){
    int idx;
    for(idx=0; idx<DIR_SIZE; idx++){
        if(dir[idx]!=NULL && dir[idx]->file_name==name){
            printf("\nName of the file - %c\nSize of the file - %d Blocks\nAddress of the index block - %d\n", name, dir[idx]->file_size,dir[idx]->index_block);
            printf("\nThe list of block addresses allocated to %c are:\n",name);
            int i=0;
            while (i<NUM_BLOCKS && list_of_files[dir[idx]->index_block]->address_of_file[i]!=-1){
                printf("%d\n",list_of_files[dir[idx]->index_block]->address_of_file[i]);
                i++;
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
        block* x = (block*) malloc(sizeof(block));
        x->file_name='*';
        disk[i]=x;
    }
    for (int i=0;i<DIR_SIZE;i++){
        dir[i]=NULL;
    }
    for (int i=0;i<NUM_BLOCKS;i++){
        list_of_files[i]=NULL;
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
            char name = disk[i-1]->file_name;
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