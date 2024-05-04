#include <os.h>


struct spi_hw {
	unsigned int cr0;
	unsigned int cr1;
	unsigned int dr;
	unsigned int sr;
	unsigned int cpsr;
	unsigned int imsc;
	unsigned int ris;
	unsigned int mis;
	unsigned int icr;
};

#define spi0 	((volatile struct spi_hw*)SPI0_BASE)


#define CR0_DSS		0x7			// 8 bit data

#define CR1_SOD		(1 << 3)
#define CR1_MS		(1 << 2)
#define CR1_SSE		(1 << 1)
#define CR1_LBM		(1 << 0)

#define SR_BSY		(1 << 4)
#define SR_RFF		(1 << 3)
#define SR_RNE		(1 << 2)
#define SR_TNF		(1 << 1)
#define SR_TFE		(1 << 0)

#define IMSC_TX		(1 << 3)
#define IMSC_RX		(1 << 2)
#define IMSC_RT		(1 << 1)
#define IMSC_ROR	(1 << 0)






void spi_init(uint32 sck,uint32 miso,uint32 mosi) {



gpio_init(sck, GPIO_FUNC_SIO);
gpio_dir(sck, GPIO_OUT);

gpio_init(mosi, GPIO_FUNC_SIO);
gpio_dir(mosi, GPIO_OUT);

gpio_init(miso, GPIO_FUNC_SIO);
gpio_dir(miso, GPIO_IN);

#if spi == 1
printf("hw spi\n");

spi0->cr0 = CR0_DSS;
spi0->cpsr = 60;
spi0->cr1 = CR1_SSE;
#endif
}



unsigned char transfer(unsigned char data) {
unsigned char rd = 0;


	/*unsigned int i;
	for (i = 0; i < len; ++i) {
		while (!(spi0->sr & SR_TNF));

		spi0->dr = data[i];

		while (spi0->sr & SR_BSY);

		if (spi0->sr & SR_RNE)
			data[i] = spi0->dr;
	}*/
    /*while (!(spi0->sr & SR_TNF));
    spi0->dr = (unsigned int)data;
    while (spi0->sr & SR_BSY);
    if (spi0->sr & SR_RNE)
        rv = spi0->dr;

    return rv;*/


    #if spi == 0
   for (int i = 0; i < 8; i++){
        rd =  rd <<1;
      if ((data << i) & 0x80){
          gpio_set(_SDI,1);//nok_sda=1;
      }else{
          gpio_set(_SDI,0);//nok_sda=0;
      }

      if(gpio_get(_SDO))  rd |= 1;
      gpio_set(_SCK,1);//nok_sclk=1;
      gpio_set(_SCK,0);// nok_sclk=0;
   }
   return rd;
   #else 
    spi0->dr = data;
    while (spi0->sr & SR_BSY);
    return (unsigned char)spi0->dr; 

   #endif

}

unsigned char transfer2(unsigned char data) {
   unsigned char rd = 0;
   gpio_set(SPI_SCK_BIT,0);// nok_sclk=0;
   gpio_set(SPI_MOSI_BIT,0);//nok_sda=0;

   for (int i = 0; i < 8; i++){
        rd =  rd <<1;
        if ((data << i) & 0x80){
             gpio_set(SPI_MOSI_BIT,1);//nok_sda=1;
        }else{
             gpio_set(SPI_MOSI_BIT,0);//nok_sda=0;
        }

        if(gpio_get(SPI_MISO_BIT))  rd |= 1;
        //asm("nop");asm("nop");
        gpio_set(SPI_SCK_BIT,1);//nok_sclk=1;
        //asm("nop");asm("nop");
        gpio_set(SPI_SCK_BIT,0);// nok_sclk=0;
   }
   
   return rd;

}

void spi_cs(uint32_t value)
{
    gpio_set(SPI_CS_BIT,value);// nok_sclk=0;
}

