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
  unsigned char BS_jmpBoot[3];
  unsigned char BS_OEMName[8];
  unsigned int BPB_BytesPerSec; //2 bytes
  unsigned char BPB_SecPerClus;
  unsigned int  BPB_RsvdSecCnt; //2 bytes
  unsigned char BPB_NumFATs;
  unsigned int  BPB_RootEntCnt; //2 bytes
  unsigned int  BPB_TotSec16; //2 bytes
  unsigned char BPB_Media;
  unsigned int  BPB_FATSz16; //2 bytes
  unsigned int  BPB_SecPerTrk; //2 bytes
  unsigned int  BPB_NumHeads; //2 bytes
  unsigned long BPB_HiddSec; //4 bytes
  unsigned long BPB_TotSec32; //4 bytes
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
  unsigned char DIR_Name[11];     //8 chars filename
  unsigned char DIR_Attr;         //file attributes RSHA, Longname, Drive Label, Directory
  unsigned char DIR_NTRes;        //set to zero
  unsigned char DIR_CrtTimeTenth; //creation time part in milliseconds
  unsigned int  DIR_CrtTime;      //creation time
  unsigned int  DIR_CrtDate;      //creation date
  unsigned int  DIR_LastAccDate;  //last access date
  unsigned int  DIR_FstClusHI;    //first cluster high word                 
  unsigned int  DIR_WrtTime;      //last write time
  unsigned int  DIR_WrtDate;      //last write date
  unsigned int  DIR_FstClusLO;    //first cluster low word                 
  unsigned long DIR_FileSize;     
  };



int fat_init ();
//############################################################################
//Auslesen der Adresse des First Root Directory von Volume Boot Record
unsigned int fat_root_dir_addr (unsigned char *Buffer);

//############################################################################
//  Ausgabe des angegebenen Directory Eintrag in Entry_Count
//  ist kein Eintrag vorhanden, ist der Eintrag im 
//  Rückgabe Cluster 0xFFFF. Es wird immer nur ein Eintrag ausgegeben
//  um Speicherplatz zu sparen um es auch für kleine Atmels zu benutzen
unsigned int fat_read_dir_ent (unsigned int dir_cluster, //Angabe Dir Cluster
          unsigned char Entry_Count,   //Angabe welcher Direintrag
          unsigned long *Size,       //Rückgabe der File Größe
          unsigned char *Dir_Attrib,   //Rückgabe des Dir Attributs
          unsigned char *Buffer);
//############################################################################
//  Auslesen der Cluster für ein File aus der FAT
//  in den Buffer(512Byte). Bei einer 128MB MMC/SD 
//  Karte ist die Cluster größe normalerweise 16KB groß
//  das bedeutet das File kann max. 4MByte groß sein.
//  Bei größeren Files muß der Buffer größer definiert
//  werden! (Ready)
//  Cluster = Start Clusterangabe aus dem Directory 
void fat_load ( unsigned int Cluster,     //Angabe Startcluster
        unsigned long *Block,
        unsigned char *TMP_Buffer);

//############################################################################
//Lesen eines 512Bytes Blocks von einem File
void fat_read_file (unsigned int Cluster,//Angabe des Startclusters vom File
         unsigned char *Buffer,   //Workingbuffer
         unsigned long BlockCount);

//############################################################################
//Lesen eines 512Bytes Blocks von einem File
void fat_write_file (unsigned int cluster,//Angabe des Startclusters vom File
          unsigned char *buffer,    //Workingbuffer
          unsigned long blockCount);   //Angabe welcher Bock vom File gespeichert 
                    //werden soll a 512 Bytes
