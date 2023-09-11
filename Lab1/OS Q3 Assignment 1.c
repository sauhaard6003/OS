#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
struct termios copy;
void die(const char *s) {
  perror(s);
  exit(1);
}
void disableRaw() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &copy)==-1){
    die("tcsetattr");
  }
}
void makeHexaDecimal(int decimalNumber[],int size){
  for (int q=0;q<size;q++){
    int i=1,j,temp;
    long int quotient,remainder;
    char hexadecimalNumber[100];
    quotient = decimalNumber[q];
    while(quotient!=0) {
      temp = quotient % 16;
      if(temp < 10){
        temp =temp + 48;
      } 
      else{
        temp = temp + 55;
      }
      hexadecimalNumber[i++]=temp;
     quotient = quotient/16;
    }
    for (j = i -1 ;j> 0;j--){
      printf("%c",hexadecimalNumber[j]);
    }
    printf(" ");
  }
}
void enableRaw() {
  if (tcgetattr(STDIN_FILENO, &copy) == -1){
    die("tcgetattr"); 
  }
  tcgetattr(STDIN_FILENO, &copy);
  atexit(disableRaw);
  struct termios raw = copy;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){
    die("tcsetattr");
  }
}

int main() {
  enableRaw();
  int st[1000000];
  char ct[1000000];
  int count=0;
  while (1)
  { char input;
    scanf("%c",&input); 
    count++;
    if (input=='?'){
      break;
    }
    else if (input=='\n'){
      count=0;
      printf("\n");
      memset(st,0,sizeof st);
      memset(ct,'\0',sizeof ct);
      continue;
    }
    else if (count==9){
      count=1;
      printf("\n");
      memset(st,0,sizeof st);
      memset(ct,'\0',sizeof ct);
    }
    if (!iscntrl(input)) {
      printf("\33[2K\r");
      
      st[count-1]=input;
      ct[count-1]=input;
      makeHexaDecimal(st,count);
      printf("\r");
      printf("\033[25C"); 
      //printf("\t\t");
      for (int i=0;i<count;i++){
        printf("%c",ct[i]);
      }
      
    }
  }
  
  st[count]='\0';
  ct[count]='\0';
  disableRaw();
  return 0;
}
