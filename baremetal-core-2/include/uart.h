/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */
#include <queue.h>
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


extern sem_t uart_tx;
extern sem_t uart_rx;
extern sem_t test;


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

#if 0
#define UART_IBLEN      100
#define UART_OBLEN      100
#define PL011_FIFO_LEN   8 
/* UART output flags */
#define UART_OFLAG_NOBLOCK    0x0001 /**< do non-blocking output        */

struct uart
{
    /* Pointers to associated structures */
    void *csr;                  /**< Control & status registers         */
    //device *dev;                /**< Dev structure                      */

    /* Statistical Counts */
    uint32 cout;                  /**< Characters output                  */
    uint32 cin;                   /**< Characters input                   */
    uint32 lserr;                 /**< Receiver error count               */
    uint32 ovrrn;                 /**< Characters overrun                 */
    uint32 iirq;                  /**< Input IRQ count                    */
    uint32 oirq;                  /**< Output IRQ count                   */

    /* UART input fields */
    uint8 iflags;               /**< Input flags                        */
    sem_t isema;            /**< Count of input bytes ready         */
    uint16 istart;              /**< Index of first byte                */
    uint16 icount;              /**< Bytes in buffer                    */
    uint8 in[UART_IBLEN];       /**< Input buffer                       */

    /* UART output fields */
    uint8 oflags;               /**< Output flags                       */
    sem_t osema;            /**< Count of buffer space free         */
    uint16 ostart;              /**< Index of first byte                */
    uint16 ocount;              /**< Bytes in buffer                    */
    uint8 out[UART_OBLEN];      /**< Output buffer                      */
    volatile bool oidle;        /**< UART transmitter idle              */
};
extern struct uart uarttab[];

#endif