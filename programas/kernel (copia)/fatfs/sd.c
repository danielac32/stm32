#include <os.h>


#define nop asm volatile ("nop\n\t")
#define MMC_Disable() spi_cs(1)
#define MMC_Enable() spi_cs(0)


//############################################################################
//Routine zur Initialisierung der MMC/SD-Karte (SPI-MODE)
unsigned char mmc_init (){
  unsigned int Timeout = 0;
  spi_init(SPI_SCK_BIT,SPI_MISO_BIT,SPI_MOSI_BIT);
  gpio_init(SPI_CS_BIT, GPIO_FUNC_SIO);
  gpio_dir(SPI_CS_BIT, GPIO_OUT);
    
  for (unsigned char b = 0;b<0x0f;b++) //Sendet min 74+ Clocks an die MMC/SD-Karte
    {
    mmc_write_byte(0xff);
    }
  
  //Sendet Commando CMD0 an MMC/SD-Karte
  unsigned char CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
  while(mmc_write_command (CMD) !=1){
    if (Timeout++ > 200){
        MMC_Disable();
        return(1); //Abbruch bei Commando1 (Return Code1)
    }
  }
  //Sendet Commando CMD1 an MMC/SD-Karte
  Timeout = 0;
  CMD[0] = 0x41;//Commando 1
  CMD[5] = 0xFF;
  while(mmc_write_command (CMD) !=0){
        if (Timeout++ > 400){
      MMC_Disable();
      return(2); //Abbruch bei Commando2 (Return Code2)
        }
  }
  //set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
  MMC_Disable();
  return(0);
}

//############################################################################
//Sendet ein Commando an die MMC/SD-Karte
unsigned char mmc_write_command (unsigned char *cmd){
  unsigned char tmp = 0xff;
  unsigned int Timeout = 0;
  //set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv) 
  MMC_Disable();

  //sendet 8 Clock Impulse
  mmc_write_byte(0xFF);

  //set MMC_Chip_Select to low (MMC/SD-Karte Aktiv)
  MMC_Enable();

  //sendet 6 Byte Commando
  for (unsigned char a = 0;a<0x06;a++) //sendet 6 Byte Commando zur MMC/SD-Karte
    {
    mmc_write_byte(*cmd++);
    }

  //Wartet auf ein gültige Antwort von der MMC/SD-Karte
  while (tmp == 0xff) 
    {
    tmp = mmc_read_byte();
    if (Timeout++ > 500)
      {
      break; //Abbruch da die MMC/SD-Karte nicht Antwortet
      }
    }
        MMC_Disable();
//        MMC_Enable();
  return(tmp);
}

//############################################################################
//Routine zum Empfangen eines Bytes von der MMC-Karte 
unsigned char mmc_read_byte (void){
  unsigned char Byte = 0;
  //Byte=shiftIn(_MISO,_SCK,MSBFIRST);//
  Byte=transfer2(0xff);
  return (Byte);
}


//############################################################################
//Routine zum Senden eines Bytes zur MMC-Karte
void mmc_write_byte (unsigned char Byte){
      transfer2(Byte);
      //shiftOut(_MOSI,_SCK,MSBFIRST,Byte);
}

//############################################################################
//Routine zum schreiben eines Blocks(512Byte) auf die MMC/SD-Karte
unsigned char mmc_write_sector (unsigned long addr,unsigned char *Buffer){
  unsigned mask = disable();
  unsigned char tmp;
  //Commando 24 zum schreiben eines Blocks auf die MMC/SD - Karte
  unsigned char cmd[] = {0x58,0x00,0x00,0x00,0x00,0xFF}; 
  
  /*Die Adressierung der MMC/SD-Karte wird in Bytes angegeben,
    addr wird von Blocks zu Bytes umgerechnet danach werden 
    diese in das Commando eingefügt*/
  addr = addr << 9; //addr = addr * 512
  
  cmd[1] = ((addr & 0xFF000000) >>24 );
  cmd[2] = ((addr & 0x00FF0000) >>16 );
  cmd[3] = ((addr & 0x0000FF00) >>8 );

  //Sendet Commando cmd24 an MMC/SD-Karte (Write 1 Block/512 Bytes)
  tmp = mmc_write_command (cmd);
  if (tmp != 0)
    {
      restore(mask);
    return(tmp);
    }
      
  //Wartet einen Moment und sendet einen Clock an die MMC/SD-Karte
  for (unsigned char a=0;a<100;a++)
    {
    mmc_read_byte();
    }
  
  //Sendet Start Byte an MMC/SD-Karte
  mmc_write_byte(0xFE); 
  
  //Schreiben des Bolcks (512Bytes) auf MMC/SD-Karte
  for (unsigned int a=0;a<512;a++)
    {
    mmc_write_byte(*Buffer++);
    }
  
  //CRC-Byte schreiben
  mmc_write_byte(0xFF); //Schreibt Dummy CRC
  mmc_write_byte(0xFF); //CRC Code wird nicht benutzt
  
  //Fehler beim schreiben? (Data Response XXX00101 = OK)
  if((mmc_read_byte()&0x1F) != 0x05) {
    restore(mask);
    return(1);
  }

  //Wartet auf MMC/SD-Karte Bussy
  while (mmc_read_byte() != 0xff){};
  
  //set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
  MMC_Disable();
  
  restore(mask);
return(0);
}

//############################################################################
//Routine zum lesen des CID Registers von der MMC/SD-Karte (16Bytes)
void mmc_read_block(unsigned char *cmd,unsigned char *Buffer,unsigned int Bytes)
//############################################################################
{ 

  //Sendet Commando cmd an MMC/SD-Karte
  if (mmc_write_command (cmd) != 0){
      return;
  }
  //Wartet auf Start Byte von der MMC/SD-Karte (FEh/Start Byte)
  
  while (mmc_read_byte() != 0xfe){};

  //Lesen des Bolcks (normal 512Bytes) von MMC/SD-Karte
  for (unsigned int a=0;a<Bytes;a++)
    {
    *Buffer++ = mmc_read_byte();
    }
  //CRC-Byte auslesen
  mmc_read_byte();//CRC - Byte wird nicht ausgewertet
  mmc_read_byte();//CRC - Byte wird nicht ausgewertet
  
  //set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
  MMC_Disable();
  
  return;
}

//############################################################################
//Routine zum lesen eines Blocks(512Byte) von der MMC/SD-Karte
unsigned char mmc_read_sector (unsigned long addr,unsigned char *Buffer)
//############################################################################
{ 
  unsigned mask = disable();
  //Commando 16 zum lesen eines Blocks von der MMC/SD - Karte
  unsigned char cmd[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
  
  /*Die Adressierung der MMC/SD-Karte wird in Bytes angegeben,
    addr wird von Blocks zu Bytes umgerechnet danach werden 
    diese in das Commando eingefügt*/
  addr = addr << 9; //addr = addr * 512

  cmd[1] = ((addr & 0xFF000000) >>24 );
  cmd[2] = ((addr & 0x00FF0000) >>16 );
  cmd[3] = ((addr & 0x0000FF00) >>8 );

    mmc_read_block(cmd,Buffer,512);
  restore(mask);
  return(0);
}

//############################################################################
//Routine zum lesen des CID Registers von der MMC/SD-Karte (16Bytes)
unsigned char mmc_read_cid (unsigned char *Buffer)
//############################################################################
{
  //Commando zum lesen des CID Registers
  unsigned char cmd[] = {0x4A,0x00,0x00,0x00,0x00,0xFF}; 
  
  mmc_read_block(cmd,Buffer,16);
  return(0);
}

//############################################################################
//Routine zum lesen des CSD Registers von der MMC/SD-Karte (16Bytes)
unsigned char mmc_read_csd (unsigned char *Buffer)
//############################################################################
{ 
  //Commando zum lesen des CSD Registers
  unsigned char cmd[] = {0x49,0x00,0x00,0x00,0x00,0xFF};
  
  mmc_read_block(cmd,Buffer,16);

  return(0);
}