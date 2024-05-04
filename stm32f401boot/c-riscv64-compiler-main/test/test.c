 void putc();
void puti();

void printf();

char buff[100];



void asus(int y){
        //putc('o');
        //putc('k');
        //putc('\n');
        puti(y);
        putc('\n');
}




int main(){
    int t=0;
     int i=0;
     int u = 555;

     char *str="daniel";



    printf("%s %d\n",str,u; );
    for ( i = 0; i < 6; ++i)
   {
      putc(*str++);
    }
    putc('\n');
    while(1){
        
        asus(t);
        t = t+1;
        if(t>=255)t=0;

    }
	return 0;
}