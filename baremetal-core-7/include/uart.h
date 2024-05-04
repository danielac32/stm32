#include <stdint.h>
#include <stdbool.h>
/* Control register */
#define UART_EN		13 
#define UART_INTR_RX	5
#define UART_INTR_TCIE	6
#define UART_INTR_TX	7
#define UART_TX_EN 	3
#define UART_RX_EN 	2

#define PARITY		12

/* Status register */
#define UART_RXNE	0x20
#define UART_TC		0x40
#define UART_TC_TXE	0xC0

#define UART_TEST	6

#define UART_FIFO_SIZE	1
#define UARTBUF 256
#define ECHO 1

struct uart_csreg {
	volatile uint32 sr;
	volatile uint32 dr;
	volatile uint32 brr;
	volatile uint32 cr1;
	volatile uint32 cr2;
	volatile uint32 cr3;
	volatile uint32 gtr;
};



#define RECEIVE_BUFFER_SIZE 16
extern char receive_buffer[];
extern int receive_buffer_head;
extern int receive_buffer_tail;


#define MAX_SPECIAL_KEY_SEQ_LEN 6

enum SpecialKey {
    SPECIAL_KEYS_START = 0x1000,
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_RIGHT,
    KEY_ARROW_LEFT,
    KEY_BACKSPACE
};

/* character sequence for a sepcial key */
struct SpecialKeySequence {
    enum SpecialKey code;
    char sequence[MAX_SPECIAL_KEY_SEQ_LEN];
};

/* null-terminated character sequences for special keys */
static const struct SpecialKeySequence SPECIAL_KEY_SEQUENCES[] = {
    /* arrow keys */
    {KEY_ARROW_UP,    {0x1b, 0x5b, 0x41, 0x00}},
    {KEY_ARROW_DOWN,  {0x1b, 0x5b, 0x42, 0x00}},
    {KEY_ARROW_RIGHT, {0x1b, 0x5b, 0x43, 0x00}},
    {KEY_ARROW_LEFT,  {0x1b, 0x5b, 0x44, 0x00}},

    /* control keys */
    {KEY_BACKSPACE,   {0x7f, 0x00}},
    {KEY_BACKSPACE,   {0x08, 0x00}}
};

#define SPECIAL_KEY_COUNT 6


extern void kputch(int c);
extern int kgetch(void);
extern char *gets_base(int (*getch)(void)/*, void (*putch)(int)*/, char *buffer);
extern int circular_inc(int operand, int circle_size);
extern bool is_special_key_sequence_prefix(char *sequence);
extern int get_special_key_code(char *sequence);
extern char *get_special_key_sequence(int code);
extern int gets_base2(char *buffer);

 

extern uint32 uart_init(struct dentry *devptr);
extern void uarthandler();
extern  uint32 uart_getc(struct dentry *);
extern  uint32 uart_putc(struct dentry *, char);
extern  uint32 uart_read(struct dentry *, char *, int32);
extern  uint32 uart_write(struct dentry *, char *, int32);

