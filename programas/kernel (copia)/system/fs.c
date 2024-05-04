#include <os.h>


bool active = false;
/*create initial directory*/
struct fsystem fsd;

/*create open file descriptor table  */	
struct fdesc openfiletable[FILETABLEN];

/*Create core inode list*/
struct in_core_inode core_inode[FILETABLEN];

int dev0_numblocks;	
int dev0_blocksize;
char *dev0_blocks;
int dev0 = 0;
uint32 vfs_size=0;


int delete(){

//freemem(dev0_blocks,dev0_numblocks * dev0_blocksize);
//freemem(fsd.freemask,fsd.freemaskbytes);
//hexdump(dev0_blocks,dev0_numblocks * dev0_blocksize,true);
free(dev0_blocks);
free(fsd.freemask);
active = false;
printf("vfs deleted\n");
}


int mkbsdev(int dev, int blocksize, int numblocks) {
  //uint32 mask=disable();
  if(active){
  	 printf("error mount\n");
    return SYSERR;
  }
  if (dev != 0) {
    printf("Unsupported device: %d\n", dev);
    return SYSERR;
  }

  if (blocksize != 0) {
    dev0_blocksize = blocksize;
  }
  else {
    dev0_blocksize = MDEV_BLOCK_SIZE;
  }

  if (numblocks != 0) {
    dev0_numblocks =  numblocks;
  }
  else {
    dev0_numblocks =  MDEV_NUM_BLOCKS;
  }


   /*DSTATUS res= disk_initialize(0);
   if(res){
      return SYSERR;
   }*/

  if ( (dev0_blocks = malloc(dev0_numblocks * dev0_blocksize)) == NULL) {
  //if ( (dev0_blocks = getmem(dev0_numblocks * dev0_blocksize)) == (char *)SYSERR) {
    printf("mkbsdev mallocfailed\n");
    //restore(mask);
    return SYSERR;
  }


  printf("numblocks :%d  , blocksize :%d -> size: %d kb\n", dev0_numblocks,dev0_blocksize,(dev0_numblocks * dev0_blocksize)/1024);
   
  mkfs(0,DEFAULT_NUM_INODES); /* bsdev 0*/
  active = true;
  //restore(mask);
  return OK;
}






int mkfs(int dev, int num_inodes) {
	int i;
  int bm_blk = 0;
  int inodes_per_block;
  int inode_blocks;
       
	struct inode node,target;
	
    if (dev == 0) {
        fsd.nblocks = dev0_numblocks;
        fsd.blocksz = dev0_blocksize;
    }
    else {
        printf("Unsupported device\n");
        return SYSERR;
    }

    if(num_inodes < 1 || num_inodes > MDEV_NUM_BLOCKS){
        printf("Invalid number of inodes\n");
    }
    else {
        fsd.ninodes = num_inodes;
        fsd.inodes_used = 0;
    } 
  
	i = fsd.nblocks;
 	while ( (i % 8) != 0) {i++;}
 	fsd.freemaskbytes = i / 8; 

 	if ((fsd.freemask = malloc(fsd.freemaskbytes)) == NULL) 
	//if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (char *)SYSERR) 
  {
 		printf("mkfs malloc failed.\n");
  	return SYSERR;
	}

	/* zero the free mask */
 	for(i=0;i<fsd.freemaskbytes;i++) {
  	fsd.freemask[i] = '\0';
 	}
 	
  /*Initialize inode number in File system directory to UNSUED*/
  for(i=0 ;i<DIRECTORY_SIZE; i++)
	{
	  fsd.root_dir.entry[i].inode_num = UNUSED;
	}
	
	/*Initialize the opefiletable state to unused*/
	for(i=0; i<FILETABLEN; i++)
	{
	   openfiletable[i].state = UNUSED;
	}
	
	/*Initialize inode number in File system directory to -1*/
    for(i=0; i<FILETABLEN; i++)
	{
	    core_inode[i].id = UNUSED;
	}
		
 	/* write the fsystem block to block 0, mark block used */
 	setmaskbit(0);
 	bwrite(dev0, bm_blk, 0, &fsd, sizeof(struct fsystem));

 	/* Computation for number of blocks required to stote inode*/
	inodes_per_block = MDEV_BLOCK_SIZE / sizeof(struct inode);
	inode_blocks = num_inodes / inodes_per_block;
	
	
	/* Reserve blocks in memory for inode table*/
	for(i=2; i< inode_blocks + 2;i++ )                                                  //HARDCODED VLAUES HERE
	{
	  setmaskbit(i);
	}
	
	/* write the freemask in block 0, mark block used */
 	setmaskbit(1);
 	bwrite(dev0, bm_blk, 1, fsd.freemask, fsd.freemaskbytes);
    
     for (int i = 0; i < MAX_NUMBER_OF_TASKS; i++)
    {
        for (int j = 3; j < NDESC-1; j++)
        {
            task[i].fdesc[j] = UNUSED;  // UNSED value declared in the FS.h
        }
    }

    return OK;
}

int bread(int dev, int block, int offset, void *buf, int len) {
    char *bbase;

    if (dev != 0) {
        printf("Unsupported device\n");
        return SYSERR;
    }
    
    if (offset >= dev0_blocksize) {
        printf("Bad offset\n");
        return SYSERR;
    }

    bbase = &dev0_blocks[block * dev0_blocksize];
    memcpy(buf, (bbase+offset), len);

    /*unsigned int address;
    address = (dev0_blocksize * block) + offset + OFF;
    read_flash(buf,address,len);*/
    return OK;

}

int bwrite(int dev, int block, int offset, void * buf, int len) {
    char *bbase;

    if (dev != 0) {
        printf("Unsupported device\n");
        return SYSERR;
    }
    if (offset >= dev0_blocksize) {
        printf("Bad offset\n");
        return SYSERR;
    }

    bbase = &dev0_blocks[block * dev0_blocksize];
    memcpy((bbase+offset), buf, len);
    /*unsigned int address;
    address = (dev0_blocksize * block) + offset + OFF;

    write_flash(buf,address,len);*/
  
    return OK;

}

int block_offset_write(int dev, int block, int offset, void * buf, int len)
{
    char tmp_buf[MDEV_BLOCK_SIZE];
    
    // Read inode block from store
    if (bread( dev, block, 0, tmp_buf, MDEV_BLOCK_SIZE) == SYSERR){
        return SYSERR;
    }
   
    // Put inode in the correct block 
    memcpy((tmp_buf+offset), buf, len);
  
    // Write inode block to store
    if (bwrite( dev, block, 0, tmp_buf, MDEV_BLOCK_SIZE) == SYSERR){
        return SYSERR;
    }
    
    return OK;
}

int block_offset_read(int dev, int block, int offset, void * buf, int len)
{
    char tmp_buf[MDEV_BLOCK_SIZE];
    
    // Read inode block from store
    if (bread( dev, block, 0, tmp_buf, MDEV_BLOCK_SIZE) == SYSERR){
        return SYSERR;
    }
   
    // Put inode in the correct block 
    memcpy( buf, (tmp_buf+offset), len);
  
    return OK;
}

int get_inode_by_num(int dev, int inode_number)
{
    char buf[MDEV_BLOCK_SIZE];    //Temporary buffer to hold inode block
    int free_index = -1;
    struct inode in;
    int i;
  
    if (dev != 0) 
    {
        printf("Unsupported device\n");
        return SYSERR;
    }
  
    if(inode_number < 0 || inode_number > MDEV_NUM_BLOCKS) 
    {
        printf("Invalid inode nubmer \n");
        return SYSERR;
    }
  
    // if requested inode is already in memory, return the same
    for (i=0; i < FILETABLEN; i++)
    {
        if (core_inode[i].id == inode_number)
        {
            printf("\n\rFound incore inode %d at %d", inode_number, i);
            return i;
        }
        else if ((free_index == -1) && (core_inode[i].id == UNUSED))
        {
            free_index = i;
        }
    }

    // if in core inode table is full
    if ((i == FILETABLEN) && (free_index == -1))
    {
        printf("\n\rNot enough room to bring inode %d incore.", inode_number);
        return SYSERR;
    }
    
    // Read inode block from backing store
    if (bread( dev, INODE2BLOCKNO(inode_number), 0, buf, MDEV_BLOCK_SIZE) == SYSERR)
    {
        return SYSERR;
    }
  
    // extract exact inode from inode block
    memcpy(&in, (buf+INODE2OFFSET(inode_number)), sizeof(struct inode));
    core_inode[free_index].id = in.id;
    core_inode[free_index].type = in.type;
    core_inode[free_index].nlink = in.nlink;
    core_inode[free_index].device = in.device;
    core_inode[free_index].size = in.size;
    for (i=0; i < FILEBLOCKS; i++)
    {
        core_inode[free_index].blocks[i] = in.blocks[i];
    }
     
    return free_index;
}

int put_inode_by_num(int dev, int incore_inode_index){
    
    char buf[MDEV_BLOCK_SIZE];    //Temporary buffer to hold inode block
    int i;
    struct inode in;
    
    in.id = core_inode[incore_inode_index].id;
    in.type = core_inode[incore_inode_index].type;
    in.nlink = core_inode[incore_inode_index].nlink;
    in.device = core_inode[incore_inode_index].device;
    in.size = core_inode[incore_inode_index].size;
    
    for (i=0; i < FILEBLOCKS; i++)
    {
         in.blocks[i]  = core_inode->blocks[i];
    }
    if (dev != 0) {
        printf("Unsupported device\n");
        return SYSERR;
    }
  
    if((incore_inode_index < 0) || (incore_inode_index > FILETABLEN)) {
        printf("Invalid incore inode nubmer \n");
        return SYSERR;
    }
    
    //bwrite(dev, INODE2BLOCKNO(inode_number), INODE2OFFSET(inode_number), in, sizeof(struct inode));
    
    // Read inode block from store
    if (bread( dev, INODE2BLOCKNO(in.id), 0, buf, MDEV_BLOCK_SIZE) == SYSERR){
        return SYSERR;
    }
   
    // Put inode in the correct block 
    memcpy((buf+INODE2OFFSET(in.id)), &in, sizeof(struct inode));
  
    // Write inode block to store
    if (bwrite( dev, INODE2BLOCKNO(in.id), 0, buf, MDEV_BLOCK_SIZE) == SYSERR){
        return SYSERR;
    }
    
    return OK;
}


int get_inode_by_name(char *filename)
{
    int length;
    int i;
    
    //Check filename length
    length = strnlen(filename, FILENAMELEN+1);
    if (length > FILENAMELEN)
    {  
        printf("\n\rFile name cannot be longer than %d.", FILENAMELEN); 
        return SYSERR;
    }

    //Read directory into memory
    //if(bread(0, bm_blk, 0, &fsd, sizeof(struct fsystem)) == SYSERR)
    //{
    //  return -1;
    //}
         
    //Check root directory for same filename
    for (i=0; i < DIRECTORY_SIZE; i++)
    {
        if ((fsd.root_dir.entry[i].inode_num != UNUSED) &&
        (strncmp(filename,fsd.root_dir.entry[i].name, FILENAMELEN) == 0))
        {
            return get_inode_by_num(0, i);    
        }
    }

    return SYSERR;
}

/* specify the block number to be set in the mask */
int setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
}

/* specify the block number to be read in the mask */
int getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
}

/* specify the block number to be unset in the mask */
int clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
}

/* This is maybe a little overcomplicated since the first block is indicated in the
   high-order bit.  Shift the byte by j positions to make the match in bit7 (the 8th 
   bit) and then shift that value 7 times to the low-order bit to print.  Yes, it
   could be the other way...  */
void printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

int allocate_next_free_block ()
{
    int i = 0;
  	for (i=0; getmaskbit(i) == 1 && i<MDEV_NUM_BLOCKS; i++);
  	if(i==MDEV_NUM_BLOCKS)
  	{
  	  printf("\n\rNo more free blocks in filesystem.");
  	  return SYSERR;
  	}
  	setmaskbit(i);
  	return i;
}

int allocate_block(int incore_inode_index, int datablock_no)
{
    int no_blocks_1_indirection = MDEV_BLOCK_SIZE / sizeof(int);
    int no_blocks_2_indirection = (MDEV_BLOCK_SIZE / sizeof(int)) * (MDEV_BLOCK_SIZE / sizeof(int));
    int first_level_datablock_no;
    int second_level_datablock_no;

    int tmp_buf[no_blocks_1_indirection];
    int tmp_buf2[no_blocks_1_indirection];
    int i, allocated_block_no;
    
    // direct block pointer 
    if (datablock_no < 10)
		{
		    if(core_inode[incore_inode_index].blocks[datablock_no] == UNUSED)
			  {
			      return (core_inode[incore_inode_index].blocks[datablock_no] = allocate_next_free_block()) ;
			  }
			 
		}
		
		// first level indirection pointers
		else if (datablock_no < (10 + no_blocks_1_indirection)) 	
		{
			  if(core_inode[incore_inode_index].blocks[10] == UNUSED)
			  {
			      allocated_block_no = allocate_next_free_block();
			      if (allocated_block_no == SYSERR)
			      {
			        return SYSERR;
			      }
			     core_inode[incore_inode_index].blocks[10] = allocated_block_no;
			     
			     //bread(0, core_inode[incore_inode_index].blocks[10], 0, tmp_buf, MDEV_BLOCK_SIZE);
			     for (i=0; i < no_blocks_1_indirection; i++)
			     {
			        tmp_buf[i] = UNUSED;
			     }
			     bwrite(0, core_inode[incore_inode_index].blocks[10], 0, tmp_buf, MDEV_BLOCK_SIZE);
			  }
				
				bread(0, core_inode[incore_inode_index].blocks[10], 0, tmp_buf, MDEV_BLOCK_SIZE);
				allocated_block_no = allocate_next_free_block();
			      if (allocated_block_no == SYSERR)
			      {
			        return SYSERR;
			      }
    		tmp_buf[datablock_no-10] = allocated_block_no;
    		bwrite(0, core_inode[incore_inode_index].blocks[10], 0, tmp_buf, MDEV_BLOCK_SIZE);
    		
			  return (tmp_buf[datablock_no-10]);
		}
		
		//second level indirection allocation
		else if(datablock_no < (10 + no_blocks_1_indirection + no_blocks_2_indirection))
		{
		    if(core_inode[incore_inode_index].blocks[11] == UNUSED)
			  {
			      allocated_block_no = allocate_next_free_block();
			      if (allocated_block_no == SYSERR)
			      {
			        return SYSERR;
			      }
			     core_inode[incore_inode_index].blocks[11] = allocated_block_no;
			     //bread(0, core_inode[incore_inode_index].blocks[10], 0, tmp_buf, MDEV_BLOCK_SIZE);
			     for (i=0; i < no_blocks_1_indirection; i++)
			     {
			        tmp_buf[i] = UNUSED;
			     }
			     bwrite(0, core_inode[incore_inode_index].blocks[11], 0, tmp_buf, MDEV_BLOCK_SIZE);
			  }
			  
			  bread(0, core_inode[incore_inode_index].blocks[11], 0, tmp_buf, MDEV_BLOCK_SIZE);
			  
			  // calculate first level datablock no. to read
			  first_level_datablock_no = (datablock_no - (10 + no_blocks_1_indirection)) / no_blocks_1_indirection ;
			  //printf("\n\r First level data block no is : %d",first_level_datablock_no);
			  // check first level for UNUSED
			  if(tmp_buf[first_level_datablock_no] == UNUSED)
			  { 
			      allocated_block_no = allocate_next_free_block();
			      if (allocated_block_no == SYSERR)
			      {
			        return SYSERR;
			      }
			      tmp_buf[first_level_datablock_no] = allocated_block_no;
			      //bread(0, core_inode[incore_inode_index].blocks[10], 0, tmp_buf, MDEV_BLOCK_SIZE);
			      for (i=0; i < no_blocks_1_indirection; i++)
			      {
			         tmp_buf2[i] = UNUSED;
			      }
			      
			      // update previous block 
			      bwrite(0, core_inode[incore_inode_index].blocks[11], 0, tmp_buf, MDEV_BLOCK_SIZE);
			      
			      // Save UNUSED initialized block
			      bwrite(0, tmp_buf[first_level_datablock_no], 0, tmp_buf2, MDEV_BLOCK_SIZE);
			  }
			  
			  // read second level block
			  bread(0, tmp_buf[first_level_datablock_no], 0, tmp_buf2, MDEV_BLOCK_SIZE);
			   
			  // calculate second level block number
			  second_level_datablock_no = (datablock_no - (10 + no_blocks_1_indirection)) % no_blocks_1_indirection;
			  
			  // check second level for UNUSED
			  allocated_block_no = allocate_next_free_block();
			      if (allocated_block_no == SYSERR)
			      {
			        return SYSERR;
			      }
			  tmp_buf2[second_level_datablock_no] = allocated_block_no;
			  bwrite(0, tmp_buf[first_level_datablock_no], 0, tmp_buf2, MDEV_BLOCK_SIZE);
			  
			  return tmp_buf2[second_level_datablock_no]; 
		}
		return SYSERR;
}

int block_no_from_datablock_no(int incore_inode_index, int datablock_no)
{
    int no_blocks_1_indirection = (MDEV_BLOCK_SIZE / sizeof(int));
    int no_blocks_2_indirection = (MDEV_BLOCK_SIZE / sizeof(int)) * (MDEV_BLOCK_SIZE / sizeof(int));
    int first_level_datablock_no;
    int second_level_datablock_no;
    int tmp_buf[no_blocks_1_indirection];
    
    // direct block pointer 
    if (datablock_no < 10)
		{
		    if(core_inode[incore_inode_index].blocks[datablock_no] == UNUSED)
			  {
			      //printf("\n\r Data block is UNUSED");
			      return UNUSED;
			  }
			  else
			  {		
			      return core_inode[incore_inode_index].blocks[datablock_no];
			  }
		}
		
		// first level indirection pointers
		else if (datablock_no < (10 + no_blocks_1_indirection)) 	
		{
			  if(core_inode[incore_inode_index].blocks[10] == UNUSED)
			  {
			      //printf("\n\r Data block is UNUSED");
			      return UNUSED;
			  }else
			  {
			      bread(0, core_inode[incore_inode_index].blocks[10], 0, tmp_buf, MDEV_BLOCK_SIZE);
			      if(tmp_buf[datablock_no-10] == UNUSED)
			      {
			          return UNUSED;
			      }
			      else
			      {
			          return tmp_buf[datablock_no-10];
			      }
			  }		
		}
		// second level indirection pointers
		else if(datablock_no < (10 + no_blocks_1_indirection + no_blocks_2_indirection))
		{
		    if(core_inode[incore_inode_index].blocks[11] == UNUSED)
			  {
			      //printf("\n\r Data block is UNUSED 3");
			      return UNUSED;
			  }
			  
			  bread(0, core_inode[incore_inode_index].blocks[11], 0, tmp_buf, MDEV_BLOCK_SIZE);
			  
			  // calculate first level datablock no. to read
			  first_level_datablock_no = (datablock_no - (10 + no_blocks_1_indirection)) / no_blocks_1_indirection ;
			  
			  // check first level for UNUSED
			  if(tmp_buf[first_level_datablock_no] == UNUSED)
			  { 
			      //printf("\n\r Data block is UNUSED 2");
			      return UNUSED;
			  }
			  
			  // read second level block
			  bread(0, tmp_buf[first_level_datablock_no], 0, tmp_buf, MDEV_BLOCK_SIZE);
			   
			  // calculate second level block number
			  second_level_datablock_no = (datablock_no - (10 + no_blocks_1_indirection)) % no_blocks_1_indirection;
			  
			  // check first level for UNUSED
			  if(tmp_buf[second_level_datablock_no] == UNUSED)
			  { 
			      //printf("\n\r Data block is UNUSED 1");
			      return UNUSED;
			  }
			  else
			  {
			        return tmp_buf[second_level_datablock_no];
			  }
			  
		}
		return SYSERR;
}


 



int _fopen(char *filename, int flags){
    int i;                                        //counter
    int incore_inode_index;
    int filetablentry;
    int dev_desc;
    //uint32 mask=disable();

    if(!active){
  	    printf("error fopen\n");
        return SYSERR;
    }
    switch(flags)
    {
    
      case O_RDONLY: break;
      
      case O_WRONLY: break;
      
      case O_RDWR:   break;
      
      default: 
         printf("\n\r Invalid flag"); 
         //restore(mask);
        return -1;
    }
    
    i = get_inode_by_name(filename);

    if (i == SYSERR)
    {
        printf("\n\r%s: No such file or directory.", filename);
        //restore(mask);
        return -1;
    }
    else
    {
        incore_inode_index = i; 
    }
	  
	  /*In core inode changes*/
	  core_inode[incore_inode_index].state = UNLOCKED;
    core_inode[incore_inode_index].ref_count ++;
    
    //find free slot in open file table
    for (i=0; i < FILETABLEN; i++)
	  {
	    if (openfiletable[i].state == UNUSED)
      {
	        break;
	    }
	  }
	
	  if (i < FILETABLEN)
	  {
        filetablentry = i;
    }
    else
    {
        printf("\n\r Open-file table is full.");
        //restore(mask);
        return -1;
    }
  
    //Update open file table for new file
    openfiletable[filetablentry].state = USED; 
    openfiletable[filetablentry].mode = flags;
    openfiletable[filetablentry].fptr = 0;
    openfiletable[filetablentry].in_core_inode_num   = incore_inode_index;
  
    /* truncate logic here */

    //File descriptor 
    for (i=3; i < NDESC; i++)
    {
        if(active_task->fdesc[i] == UNUSED)
        {
            break;
        }
    }
  
    if(i < NDESC)
	  {
        dev_desc = i;
    }
    else
    {
        printf("\n\r Maximum open file descriptor limit reached");
        //restore(mask);
        return -1;
    }
  
    active_task->fdesc[dev_desc] = filetablentry;
    //restore(mask);
    return dev_desc;
}



int _fclose(int fd)
{
    int openftab_index = -1;
    int cinode_index   = -1;
    int i;
    //uint32 mask=disable();
    if(!active){
  	    printf("error fclose\n");
        //restore(mask);
        return SYSERR;
    }

    if ((fd < 3) || (fd > NDESC))
    {
        printf("\n\rInvalid file descriptor. %d\n",fd);
        //restore(mask);
        return EOF;
    }
   

    /* File descriptor to openfiletable index */
    
    if (active_task->fdesc[fd] == UNUSED)
    {
        printf("\n\rFile descriptor %d not already in use.", fd);
        //restore(mask);
        return EOF;
    }
    else
    {
        openftab_index = active_task->fdesc[fd];
        active_task->fdesc[fd] = UNUSED;
    }
    
    
    /* openfiletable index to core inode */
    if(openfiletable[openftab_index].state == UNUSED)
    {
        printf("\n\rOpen file table entry not found");
        //restore(mask);
        return EOF;
    }else
    {
        openfiletable[openftab_index].state = UNUSED;
        openfiletable[openftab_index].mode = -1;
        openfiletable[openftab_index].fptr = 0;
        cinode_index = openfiletable[openftab_index].in_core_inode_num;
        openfiletable[openftab_index].in_core_inode_num = UNUSED;
    }
        
    if(core_inode[cinode_index].id == UNUSED)
    {
        printf("\n\rCore Inode table entry not found");
        //restore(mask);
        return EOF;
    }
    else
    {
        core_inode[cinode_index].ref_count--;
        if(0 == core_inode[cinode_index].ref_count)
        {
            /* Write core inode back to disk */
            put_inode_by_num(0, cinode_index);
        
            /* Evacuate the Core inode for future use*/
            core_inode[cinode_index].id         = UNUSED ;
            core_inode[cinode_index].state      = UNLOCKED;
            core_inode[cinode_index].ref_count  = 0;
	          core_inode[cinode_index].type       = 0;
            core_inode[cinode_index].nlink      = 0;
            core_inode[cinode_index].device     = 0;
            core_inode[cinode_index].size       = 0;
        
            for(i=0 ;i<FILEBLOCKS; i++)
	          {
                core_inode[cinode_index].blocks[i] = UNUSED;
	          }
	      }
    }
    //restore(mask);
    return OK;
}



int _fcreat(char *filename, int mode)
{
  int i;                                        //counter
  int bm_blk = 0;                               //Directory block
  int length;
  int inode;
  int incore_inode;
  int filetablentry;
  int dev_desc;
  //uint32 mask=disable();
  if(!active){
  	  printf("error fcreat\n");
      //restore(mask);
      return SYSERR;
  }
   
  if(mode != O_CREAT)
  {
    printf("\n\r Invalid Mode");
    //restore(mask);
    return -1;
  }    
  
  //Check for max File descriptors for process 
  
  ///thrptr = &task[getpid()];
  for(i=3; i<NDESC; i++)
  {
    if(active_task->fdesc[i] == UNUSED)
    {
        break;
    }
  }
  
  if(i < NDESC){
     dev_desc = i;
  }else{
    printf("\n\r Maximum open file descriptor limit reached : %d\n",i);
    //restore(mask);
    return -1;
  }
  
  
	//Check filename length
	length = strnlen( filename, FILENAMELEN);
	if (length > FILENAMELEN) 
	{
    //restore(mask);
		return -1;
	}
	
	//Read directory into memory
  //if(bread(0, bm_blk, 0, &fsd, sizeof(struct fsystem)) == SYSERR)
	//{
	//  return -1;
	//}
	
	//Check root directory for same filename
	if(get_inode_by_name(filename) != SYSERR)
	{
	  printf("\n\r File alrady exist");
    //restore(mask);
	  return -1;
  }
	
	
  //Check number of used inode
  if(fsd.inodes_used >= DIRECTORY_SIZE)                                     //HARDCODING
  {
    printf("All inode are in use \n\r");
    //restore(mask);
    return -1;
  }	
  
  //Create file in directory
  fsd.inodes_used = fsd.inodes_used + 1;
  fsd.root_dir.numentries = fsd.root_dir.numentries + 1;
  
  //Find empty directory entry from predefined list.
  for(i=0 ;i<DIRECTORY_SIZE; i++)
	{
	  if(fsd.root_dir.entry[i].inode_num == UNUSED )
	  {
	    break;
	  }
	}
	
	inode = i;
	
	//find free slot in incore list 
  for(i=0 ;i<FILETABLEN; i++)
	{
	    if(core_inode[i].id == UNUSED){
	       break;
	    }
	}
	
	if(i < FILETABLEN)
	{
    incore_inode = i;
  }
  else
  {
    printf("\n\r In core list is full");
    //restore(mask);
    return -1;
  }
  
  core_inode[incore_inode].id         = inode ;
  core_inode[incore_inode].state      = UNLOCKED;
  core_inode[incore_inode].ref_count  = 1;
  core_inode[incore_inode].type       = TYPE_FILE;
  core_inode[incore_inode].nlink      = 1;
  core_inode[incore_inode].device     = 0;
  core_inode[incore_inode].size       = 0;
  for(i=0 ;i<FILEBLOCKS; i++)
	{
    core_inode[incore_inode].blocks[i] = UNUSED;
	}
	
	//store inode number
	fsd.root_dir.entry[inode].inode_num = inode;
	
	//copy file name 
	strncpy(fsd.root_dir.entry[inode].name,filename, length);
	
	//put inode in inode table 
  put_inode_by_num( 0, incore_inode);
  
  //Write directory into memory
  //if(bwrite(0, bm_blk, 0, &fsd, sizeof(struct fsystem)) == SYSERR)
	//{
	//  return -1;
	//}
    
  //find free slot in open file table
  for(i=0 ;i<FILETABLEN; i++)
	{
	    if(openfiletable[i].state == UNUSED){
	       break;
	    }
	}
	
	if(i < FILETABLEN)
	{
    filetablentry = i;
  }
  else
  {
    printf("\n\r Open file table full");
    //restore(mask);
    return -1;
  }
  
  //Update open file table for new file 
  openfiletable[filetablentry].state = USED; 
  openfiletable[filetablentry].mode = O_RDWR;
  openfiletable[filetablentry].fptr = 0;
  openfiletable[filetablentry].in_core_inode_num  = incore_inode;
  
  active_task->fdesc[dev_desc] = filetablentry;
  //restore(mask);
  return dev_desc;
}



int _fseek(int fd, int offset)
{
    int incore_inode_index;
    int filetablentry;
    int seek;
    int size;
    //uint32 mask=disable();
    if(!active){
  	    printf("error fseek\n");
        //restore(mask);
        return SYSERR;
    }
    if (active_task->fdesc[fd] == UNUSED)
    {
      printf("\n\rInvalid File Descriptor %d.", fd);
      //restore(mask);
      return 0;
    }
    
    filetablentry = active_task->fdesc[fd];
    if (openfiletable[filetablentry].state == UNUSED)
    {
      printf("\n\rOpen file table entry not found.");
      //restore(mask);
      return 0;
    }
    
    
    incore_inode_index = openfiletable[filetablentry].in_core_inode_num;
    if(core_inode[incore_inode_index].id == UNUSED)
    {
        printf("\n\rCore Inode table entry not found");
        //restore(mask);
        return 0;
    }
    
    size = core_inode[incore_inode_index].size;
    seek = openfiletable[filetablentry].fptr;
    
    if (((seek+offset) >= 0) && ((seek+offset) <= size))
    {
        openfiletable[filetablentry].fptr = (seek+offset); 
        //restore(mask);
        return seek+offset;
    }
    else
    {    
        openfiletable[filetablentry].fptr =  seek;
        //restore(mask);
        return SYSERR;
    }
    //restore(mask);
    return SYSERR;
}


int _fread(int fd, void *buf, int nbytes)
{
 
    int incore_inode_index;
    int filetablentry;
    int seek;
    int data_block_no;
    int data_block_offset;
    int no_bytes_read;
    int byes_to_read;
    int block_no;
    int start_poistion;
    int remaining;
    int i;
    int size;
    //uint32 mask=disable();
    if(!active){
  	    printf("error fread\n");
        //restore(mask);
        return SYSERR;
    }

    if (nbytes == 0)
    {
      printf("\n\rInvalid number of bytes to read.");
      //restore(mask);
      return 0;
    }
    
    if (buf == NULL)
    {
      printf("\n\rInvalid buffer pointer ");
      //restore(mask);
      return 0;
    }
    
    
 
    if (active_task->fdesc[fd] == UNUSED)
    {
      printf("\n\rInvalid File Descriptor %d.", fd);
      //restore(mask);
      return 0;
    }
    
    filetablentry = active_task->fdesc[fd];
    if (openfiletable[filetablentry].state == UNUSED)
    {
      printf("\n\rOpen file table entry not found.");
      //restore(mask);
      return 0;
    }
    
    
    seek = 0;
    seek = openfiletable[filetablentry].fptr;
    incore_inode_index = openfiletable[filetablentry].in_core_inode_num;
    if(core_inode[incore_inode_index].id == UNUSED)
    {
        printf("\n\rCore Inode table entry not found");
        //restore(mask);
        return 0;
    }
    
    size = core_inode[incore_inode_index].size;
    
    //logic to write partially filled block
    data_block_no = seek / MDEV_BLOCK_SIZE;
    data_block_offset = seek % MDEV_BLOCK_SIZE;
    no_bytes_read = 0;
    remaining  = ((seek + nbytes) > size) ? (size - seek) : nbytes;

    while(remaining > 0)
    {
        byes_to_read = ( remaining < (MDEV_BLOCK_SIZE - data_block_offset))? remaining : (MDEV_BLOCK_SIZE - data_block_offset);
        start_poistion  =  data_block_offset;
        
        block_no = block_no_from_datablock_no( incore_inode_index,  data_block_no);
        if((block_no == UNUSED) || (block_no == SYSERR))
        {
            printf("\n\r Invalid data block");
            //restore(mask);
            return 0;  
        }
              
        //printf("\n\rReading with parameters:%d, %d, %d", block_no, start_poistion, byes_to_read);  
        if (block_offset_read( 0, block_no, start_poistion, buf+no_bytes_read, byes_to_read) == SYSERR)
        {
            printf("\n\r Error while reading data block");
            //restore(mask);
            return 0;
        }
        
        no_bytes_read += byes_to_read;
        remaining -= byes_to_read;
        data_block_offset = 0;
        data_block_no++; 
    }
    
    openfiletable[filetablentry].fptr += no_bytes_read ;

    //restore(mask);
    return no_bytes_read;
}




int _fwrite(int fd, void *buf, int nbytes)
{
 
  int incore_inode_index;
  int filetablentry;
  int seek;
  int data_block_no;
  int data_block_offset;
  int no_bytes_written;
  int byes_to_write;
  int block_no;
  int start_poistion;
  int remaining;
  int i;
  //uint32 mask=disable();
  if(!active){
  	  printf("error fwrite\n");
      //restore(mask);
      return SYSERR;
  }
  if (nbytes == 0)
  {
      printf("\n\rInvalid number of bytes to write.");
      //restore(mask);
      return 0;
  }
  
  if (buf == NULL)
  {
      printf("\n\rInvalid buffer pointer ");
      //restore(mask);
      return 0;
  }
  
 
  if (active_task->fdesc[fd] == UNUSED)
  {
      printf("\n\rInvalid File Descriptor %d.", fd);
      //restore(mask);
      return 0;
  }
  
  filetablentry = active_task->fdesc[fd];
  if (openfiletable[filetablentry].state == UNUSED)
  {
      printf("\n\rOpen file table entry not found.");
      //restore(mask);
      return 0;
  }
  if (openfiletable[filetablentry].mode == O_RDONLY)
  {
      printf("\n\rFile not open for write/append.");
      //restore(mask);
      return 0;
  }
  
  seek = openfiletable[filetablentry].fptr;
  incore_inode_index = openfiletable[filetablentry].in_core_inode_num;
  if(core_inode[incore_inode_index].id == UNUSED)
  {
      printf("\n\rCore Inode table entry not found");
      //restore(mask);
      return 0;
  }
  
  //logic to write partially filled block
  data_block_no = seek / MDEV_BLOCK_SIZE;
  data_block_offset = seek % MDEV_BLOCK_SIZE;
  no_bytes_written = 0;
  remaining = nbytes;
  
  
  while( no_bytes_written < nbytes )
  {
      byes_to_write = ( remaining < (MDEV_BLOCK_SIZE - data_block_offset))? remaining : (MDEV_BLOCK_SIZE - data_block_offset);
      start_poistion  =  data_block_offset;
      
      block_no = block_no_from_datablock_no( incore_inode_index,  data_block_no);
      
      if(block_no == UNUSED)
      {
          block_no = allocate_block(incore_inode_index,  data_block_no);
          //printf("\n\r Allocated block no is : %d",block_no);
      }
      
      if(block_no == SYSERR)
      {
          printf("\n\r Invalid Data block number");
          break;   
      }
      
      //printf("\n\r Data Block no is : %d  and block no is : %d",data_block_no,block_no);
      if (block_offset_write( 0, block_no, start_poistion, buf+no_bytes_written, byes_to_write) == SYSERR)
      {
          printf("\n\r Error while writing data block");
          break;
      }
      
      no_bytes_written += byes_to_write;
      remaining -= byes_to_write;
      data_block_offset = 0;
      data_block_no++; 
  }
  
   // update file size and seek pointer.
    core_inode[incore_inode_index].size +=  no_bytes_written;
      openfiletable[filetablentry].fptr += no_bytes_written ;
  
  //restore(mask);
  return no_bytes_written;
}
