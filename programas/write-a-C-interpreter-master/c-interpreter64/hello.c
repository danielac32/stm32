#include<stdio.h>

int y;
int f;


int mul(int a,int b){
    return a*b;
}
int main(){
y=666;

if(y>0 && y<1000 || y==0){
    printf("ok %d\n",mul(100,100));
}
    
    return 0;
}
