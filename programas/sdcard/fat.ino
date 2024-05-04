#include <stdint.h>
//Block Size in Bytes
#define BlockSize 512
//Master Boot Record
#define MASTER_BOOT_RECORD  0
//Volume Boot Record location in Master Boot Record
#define VBR_ADDR 0x1C6
//define ASCII
#define SPACE 0x20
#define DIR_ENTRY_IS_FREE   0xE5
#define FIRST_LONG_ENTRY  0x01
#define SECOND_LONG_ENTRY 0x42
//define DIR_Attr
#define ATTR_LONG_NAME    0x0F
#define ATTR_READ_ONLY    0x01
#define ATTR_HIDDEN     0x02
#define ATTR_SYSTEM     0x04
#define ATTR_VOLUME_ID    0x08
#define ATTR_DIRECTORY    0x10
#define ATTR_ARCHIVE    0x20


struct BootSec
{
   uint8_t BS_jmpBoot[3];
   uint8_t BS_OEMName[8];
   uint16_t BPB_BytesPerSec; //2 bytes
   uint8_t   BPB_SecPerClus;
   uint16_t   BPB_RsvdSecCnt; //2 bytes
   uint8_t   BPB_NumFATs;
   uint16_t   BPB_RootEntCnt; //2 bytes
   uint16_t   BPB_TotSec16; //2 bytes
   uint8_t   BPB_Media;
   uint16_t   BPB_FATSz16; //2 bytes
   uint16_t   BPB_SecPerTrk; //2 bytes
   uint16_t   BPB_NumHeads; //2 bytes
   uint32_t   BPB_HiddSec; //4 bytes
   uint32_t   BPB_TotSec32; //4 bytes
};

//FAT12 and FAT16 Structure Starting at Offset 36
#define BS_DRVNUM     36
#define BS_RESERVED1    37
#define BS_BOOTSIG      38
#define BS_VOLID      39
#define BS_VOLLAB     43
#define BS_FILSYSTYPE   54

//FAT32 Structure Starting at Offset 36
#define BPB_FATSZ32     36
#define BPB_EXTFLAGS    40
#define BPB_FSVER     42
#define BPB_ROOTCLUS    44
#define BPB_FSINFO      48
#define BPB_BKBOOTSEC   50
#define BPB_RESERVED    52

#define FAT32_BS_DRVNUM   64
#define FAT32_BS_RESERVED1  65
#define FAT32_BS_BOOTSIG  66
#define FAT32_BS_VOLID    67
#define FAT32_BS_VOLLAB   71
#define FAT32_BS_FILSYSTYPE 82
//End of Boot Sctor and BPB Structure

struct DirEntry {
   uint8_t   DIR_Name[11];     //8 chars filename
   uint8_t   DIR_Attr;         //file attributes RSHA, Longname, Drive Label, Directory
   uint8_t   DIR_NTRes;        //set to zero
   uint8_t   DIR_CrtTimeTenth; //creation time part in milliseconds
   uint16_t   DIR_CrtTime;      //creation time
   uint16_t   DIR_CrtDate;      //creation date
   uint16_t   DIR_LastAccDate;  //last access date
   uint16_t   DIR_FstClusHI;    //first cluster high word
   uint16_t   DIR_WrtTime;      //last write time
   uint16_t   DIR_WrtDate;      //last write date
   uint16_t   DIR_FstClusLO;    //first cluster low word
   uint32_t   DIR_FileSize;
   };

uint8_t cluster_size;
uint16_t fat_offset;
uint16_t cluster_offset;
uint16_t volume_boot_record_addr;

int fat_init (){
int result=0;
   struct BootSec *bootp; //Zeiger auf Bootsektor Struktur
    disk_read(0,Buffer,MASTER_BOOT_RECORD,1);//mmc_read_sector(MASTER_BOOT_RECORD,Buffer); //Read Master Boot Record
   if (Buffer[510] == 0x55 && Buffer[511] == 0xAA){
       volume_boot_record_addr = Buffer[VBR_ADDR] + (Buffer[VBR_ADDR+1] << 8);
       disk_read(0,Buffer,volume_boot_record_addr,1);//mmc_read_sector (volume_boot_record_addr,Buffer);
       bootp=(struct BootSec *)Buffer;
       cluster_size = bootp->BPB_SecPerClus;
       fat_offset = bootp->BPB_RsvdSecCnt;
       cluster_offset = ((bootp->BPB_BytesPerSec * 32)/BlockSize);
       cluster_offset += fat_root_dir_addr(Buffer);
       
       return 0;
  }else{
       return 1;
  }
}


uint16_t fat_root_dir_addr (uint8_t *Buffer) {
  struct BootSec *bootp; //Zeiger auf Bootsektor Struktur
  unsigned short FirstRootDirSecNum;
  disk_read(0,Buffer,volume_boot_record_addr,1);//mmc_read_sector (volume_boot_record_addr,Buffer);
  bootp=(struct BootSec *)Buffer;
  FirstRootDirSecNum = ( bootp->BPB_RsvdSecCnt + (bootp->BPB_NumFATs * bootp->BPB_FATSz16));
  FirstRootDirSecNum+= volume_boot_record_addr;
  
  return(FirstRootDirSecNum);
}



uint16_t fat_read_dir_ent (uint16_t dir_cluster, //Angabe Dir Cluster
          uint8_t Entry_Count,   //Angabe welcher Direintrag
          uint32_t *Size,       //Rückgabe der File Größe
          uint8_t *Dir_Attrib,   //Rückgabe des Dir Attributs
          uint8_t *Buffer)     //Working Buffer
{
  
  uint8_t *pointer;
  uint16_t TMP_Entry_Count = 0;
  uint32_t Block = 0;
  struct DirEntry *dir; //Zeiger auf einen Verzeichniseintrag
  pointer = Buffer;
  if (dir_cluster == 0){
      Block = fat_root_dir_addr(Buffer);
  }else{
      fat_load (dir_cluster,&Block,Buffer);      
      Block = ((Block-2) * cluster_size) + cluster_offset;
  }
 
  for (uint16_t blk = Block;;blk++){
       disk_read(0,Buffer,blk,1);//mmc_read_sector (blk,Buffer);  //Lesen eines Blocks des Root Directory
       for (uint16_t a=0;a<BlockSize; a = a + 32){
      dir=(struct DirEntry *)&Buffer[a]; //Zeiger auf aktuellen Verzeichniseintrag holen

      if (dir->DIR_Name[0] == 0){
          return (0xFFFF);
                  }
        if ((dir->DIR_Attr != ATTR_LONG_NAME) &&
        (dir->DIR_Name[0] != DIR_ENTRY_IS_FREE)) {
        if (TMP_Entry_Count == Entry_Count) {
          for(uint8_t b=0;b<12;b++){
          if (dir->DIR_Name[b] != SPACE){
            if (b == 8){
                *pointer++= '.';
            }
            *pointer++=dir->DIR_Name[b];
            }
          }           
          *pointer++='\0';
          *Dir_Attrib = dir->DIR_Attr;
          *Size=dir->DIR_FileSize;
          dir_cluster = dir->DIR_FstClusLO;
          return(dir_cluster);
        }
      TMP_Entry_Count++;
      }
    }
  }
  return (0xFFFF); //Kein Eintrag mehr gefunden Rücksprung mit 0xFFFF
}

void fat_load ( uint16_t Cluster,uint32_t *Block,uint8_t *TMP_Buffer){
  uint16_t FAT_Block_Store = 0; 

  uint16_t FAT_Byte_Addresse; 

  uint16_t FAT_Block_Addresse;
  
  for (uint16_t a = 0;;a++)
  { 
    if (a == *Block)
      {
      *Block = (0x0000FFFF & Cluster);
      return;
      }
    
    if (Cluster == 0xFFFF)
      {
      break; //Ist das Ende des Files erreicht Schleife beenden
      }
    FAT_Byte_Addresse = (Cluster*2) % BlockSize;
      
    FAT_Block_Addresse = ((Cluster*2) / BlockSize) + volume_boot_record_addr + fat_offset;  
    
    if (FAT_Block_Addresse != FAT_Block_Store)
      {
      FAT_Block_Store = FAT_Block_Addresse;
      //Lesen des FAT Blocks
      disk_read(0,Buffer,FAT_Block_Addresse,1);//mmc_read_sector (FAT_Block_Addresse,TMP_Buffer);  
      }

    Cluster = (TMP_Buffer[FAT_Byte_Addresse + 1] << 8) + 
          TMP_Buffer[FAT_Byte_Addresse];    
  }
//  return;
}


void fat_read_file (uint16_t Cluster,uint8_t *Buffer,uint32_t BlockCount){
  uint32_t Block = (BlockCount/cluster_size);
  fat_load (Cluster,&Block,Buffer);      
  Block = ((Block-2) * cluster_size) + cluster_offset;
  Block += (BlockCount % cluster_size);
   disk_read(0,Buffer,Block,1);//mmc_read_sector (Block,Buffer); 
//  return;
}



//uint8_t fat_search_file (uint8_t *File_Name,uint16_t *Cluster,uint32_t *Size,uint8_t *Dir_Attrib,uint8_t *Buffer)  //Working Buffer
//{
//  int result=0;
//  uint16_t Dir_Cluster_Store = *Cluster;
//  for (uint8_t a = 0;a < 200;a++){
//    *Cluster = fat_read_dir_ent(Dir_Cluster_Store,a,Size,Dir_Attrib,Buffer);
//    if (*Cluster == 0xffff){
//      result=0;//return(0); //File not Found
//    }
//    if(strcmp(( char *)File_Name,(char *)Buffer) == 0){
//      result=1;//return(1); //File Found
//    }
//  }
//  return result;//return(2); //Error
//}
