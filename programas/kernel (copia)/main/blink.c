#include <os.h>


 #define SIZE 1000



void blink() {
    int rval;
    int fd, i, j;

    //char buf1[SIZE], buf2[SIZE];
    char *buf1, *buf2;
    //testbitmask();

    uint32_t t = systick_get();
    
    buf1 = (char *)malloc(SIZE*sizeof(char));
    buf2 = (char *)malloc(SIZE*sizeof(char));


    mkbsdev(0,512,100);



    printf("write a.txt\n");
    fd = _fcreat("texto1.txt", O_CREAT);
    //memset(buf1,0xbe,SIZE);
    //_fwrite(fd,buf1,SIZE);
    for (int i = 0; i < SIZE; ++i)
    { 
        char c=0xbe;
       _fwrite(fd,&c,1);
    }
    _fclose(fd);

    printf("write b.txt\n");
    fd = _fcreat("app2.txt", O_CREAT);
    //memset(buf1,0xca,SIZE);
   // _fwrite(fd,buf1,SIZE);

    for (int i = 0; i < SIZE; ++i)
    { 
        char c=0xca;
       _fwrite(fd,&c,1);
    }
    _fclose(fd);


   printf("write c.txt\n");
    fd = _fcreat("elf.txt", O_CREAT);
   // memset(buf1,0xcc,SIZE);
   // _fwrite(fd,buf1,SIZE);
    for (int i = 0; i < SIZE; ++i)
    { 
        char c=0xcc;
       _fwrite(fd,&c,1);
    }
    _fclose(fd);
    
    printf("write texto.txt\n");
    fd = _fcreat("texto.txt", O_CREAT);
    memset(buf1,'a',SIZE);

    rval = _fwrite(fd,buf1,SIZE);


     printf("read..\n");
    //_fseek(fd,-rval); 
    _fread(fd, buf2, SIZE);

     hexdump(buf2,SIZE,true);

    _fclose(fd);
   
 
   
    printf("root fs\n");
    for (int i=0; i < DIRECTORY_SIZE; i++)
    {
      char *name = fsd.root_dir.entry[i].name;

      int size = core_inode[get_inode_by_num(0, i)].size;


        if ((fsd.root_dir.entry[i].inode_num != UNUSED))
        {
            printf("---->%s  %d\n", name,size);//return get_inode_by_num(0, i);    
        }
      //printf("---->%s  %d\n", name,size);
      
    }
        
   
    free(buf1);
    free(buf2);
   

    delete();
  /*  char buff[512];
   for (int i = 0; i < 20; ++i){
        printf("\nsector: %d\n",i);
        disk_read(0,buff,i,1);
        hexdump(buff,512,true);
   }
*/
uint32_t t2 = systick_get();
printf("time curr  fs: %d\n",t2 - t );




 
   while (1) {

    /*gpio_set(25, 1);
    sleep(50);
    gpio_set(25, 0);
    sleep(100);*/
    gpio_xor(25);
    sleep(1000);
  }
}
