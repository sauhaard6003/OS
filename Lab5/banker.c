#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
int main(int argc, int* argv[])
{ 

    int p;
    printf("Enter number of processes\n");
    scanf("%d",&p);
    int n;
    printf("Enter number of resources\n");
    scanf("%d",&n);  

    printf("Enter system allocation\n");
    
    int system_resource[n];
    for (int i=0;i<n;i++){
        int x;
        scanf("%d",&x);
        system_resource[i]=x;
    }

    printf("Enter max allocation followed by current allocation\n");
    //printf("hello\n");
    int max_alloc[p][n];
    int current_alloc[p][n];
    for (int i=0;i<p;i++){
        for (int j=0;j<n;j++){
            int x;
            
            scanf("%d",&x);
            max_alloc[i][j]=x;
        }
        for (int j=0;j<n;j++){
            int x;
            
            scanf("%d",&x);
            current_alloc[i][j]=x;
        }
    }
    
    
    for(int i=0;i<p;i++){
        for(int j=0;j<n;j++){
            system_resource[j]-=current_alloc[i][j];
        }
    }

    int process_order[n];
    int y = 0;
    bool covered[p];
    for(int i=0;i<p;i++){
        covered[i]=0;
    }
    int or=0;
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < p; j++) {
            if (!covered[j]) {
                int f = 0;
                for (int k = 0; k < n; k++) {
                    if (system_resource[k]<max_alloc[j][k]-current_alloc[j][k]){
                        f = 1;
                        break;
                    }
                }
                if (f==0) {
                    process_order[or] = j+1;
                    or++;
                    for (int k = 0; k < n; k++)
                        system_resource[k] += current_alloc[j][k];
                    covered[j] = true;
                }
            }
        }
    }
    int f=1;
    
    for(int i=0;i<p;i++)
    {
      if(!covered[i])
      {
        f=0;
        break;
      }
    }
    
    if (!f){
        printf("System is in unsafe state\n");
    }
     
    else
    {
    printf("System is in safe state and can execute processes in following order ->\n");
        for (int i = 0; i<p; i++){
            printf(" P%d\n", process_order[i]);
        }
    }
    return 0;
}