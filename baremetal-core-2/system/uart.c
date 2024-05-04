#include <xinu.h>




char receive_buffer[RECEIVE_BUFFER_SIZE];
int receive_buffer_head = 0;
int receive_buffer_tail = 0;

int circular_inc(int operand, int circle_size)
{
	operand++;
	if (operand == circle_size)
		operand = 0;

	return operand;
}

bool is_special_key_sequence_prefix(char *sequence)
{
	bool is_prefix = false;
	size_t i = 0;
	int sequence_length = strlen(sequence);

	for (i = 0; i < SPECIAL_KEY_COUNT; i++) {
		if (strncmp(sequence, SPECIAL_KEY_SEQUENCES[i].sequence,
			    sequence_length) == 0)
		{
			is_prefix = true;
			break;
		}
	}

	return is_prefix;
}

int get_special_key_code(char *sequence)
{
	int keycode = 0;
	size_t i = 0;

	for (i = 0; i < SPECIAL_KEY_COUNT; i++) {
		if (strcmp(sequence, SPECIAL_KEY_SEQUENCES[i].sequence) == 0) {
			keycode = SPECIAL_KEY_SEQUENCES[i].code;
			break;
		}
	}

	return keycode;
}

char *get_special_key_sequence(int code)
{
	char *sequence = 0;
	size_t i = 0;

	for (i = 0; i < SPECIAL_KEY_COUNT; i++) {
		if (SPECIAL_KEY_SEQUENCES[i].code == code) {
			sequence = (char *) SPECIAL_KEY_SEQUENCES[i].sequence;
			break;
		}
	}

	return sequence;
}


void uarthandler(){
struct	uart_csreg *regptr;
//SYS_ENTRY();//
//uint32 mask=disable();

regptr = (struct uart_csreg *)(0x40011400);
uint8 flags = regptr->sr & 0xFF;

if(flags & UART_RXNE){
	int receive_buffer_new_head = 0;
	int data = regptr->dr;

	receive_buffer_new_head = circular_inc(receive_buffer_head,RECEIVE_BUFFER_SIZE);
	if (receive_buffer_new_head != receive_buffer_tail) {
		receive_buffer[receive_buffer_head] = (char) data;
		receive_buffer_head = receive_buffer_new_head;
	}
}
/*if (flags & UART_RXNE) {

}else if (flags & UART_TC)  {

}*/
//SYS_EXIT();//
//restore(mask);
}


void kputch(int c)
{
	int sequence_length = 0;
	int i = 0;

	char single_char_sequence[2];
	char *sequence = get_special_key_sequence(c);

	if (c == '\n')
		kputch('\r');

	if (sequence == NULL) {
		sequence = single_char_sequence;
		sequence[0] = c;
		sequence[1] = 0x00;
	}

	sequence_length = strlen(sequence);
	for (i = 0; i < sequence_length; i++) {
		kputc(sequence[i]);
	}
	/*if (c == '\n')
		kputch('\r');
	kputc(c);*/
}


int kgetch(void)
{
	int keycode = 0;
	char sequence[MAX_SPECIAL_KEY_SEQ_LEN] = {0};
	int sequence_length = 0;

	while (receive_buffer_head != receive_buffer_tail &&
	       is_special_key_sequence_prefix(sequence) &&
	       get_special_key_code(sequence) == 0)
	{
		sequence[sequence_length] = receive_buffer[receive_buffer_tail];
		receive_buffer_tail = circular_inc(receive_buffer_tail, RECEIVE_BUFFER_SIZE);
		sequence_length++;
	}

	keycode = get_special_key_code(sequence);
	if (keycode == 0 && sequence_length != 0)
		keycode = sequence[sequence_length - 1];
	return keycode;
}




char *
gets_base(int (*getch)(void), /*void (*putch)(int),*/ char *buffer)
{
	int buffer_index = 0;
	int next_char = 0;
	sem_wait(&uart_rx);
	while (next_char != '\r' && next_char != '\n') {
		next_char = getch();
		if (next_char == 0)
			continue;

		if (next_char == '\r' || next_char == '\n')
			break;

		if (next_char == KEY_BACKSPACE) {
			if (buffer_index != 0) {
				//putch(KEY_ARROW_LEFT);
				//putch(' ');
				//putch(KEY_ARROW_LEFT);
				buffer_index--;
			}
		}
		else if (next_char >= 32 && next_char < 128) {
			//putch(next_char);

			buffer[buffer_index] = next_char;
			buffer_index++;
		}
	}

	//putch('\n');
	buffer[buffer_index] = 0;
    sem_signal(&uart_rx);
	return buffer;
}

int gets_base2(char *buffer)
{
	int buffer_index = 0;
	int next_char = 0;
	
	while (next_char != '\r' && next_char != '\n') {
		syscall_sema_wait(&uart_rx);
		next_char = syscall_getc();
		syscall_sema_signal(&uart_rx);
		if (next_char == 0)
			continue;

		if (next_char == '\r' || next_char == '\n')
			break;

		if (next_char == KEY_BACKSPACE) {
			if (buffer_index != 0) {
				//putch(KEY_ARROW_LEFT);
				//putch(' ');
				//putch(KEY_ARROW_LEFT);
				buffer_index--;
			}
		}
		else if (next_char >= 32 && next_char < 128) {
			//putch(next_char);

			buffer[buffer_index] = next_char;
			buffer_index++;
		}
	}

	//putch('\n');
	buffer[buffer_index] = 0;
    
	return buffer_index;
}

#if 0
void uarthandler(){
	struct	uart_csreg *regptr;//*csrptr;
	regptr = (struct uart_csreg *)(0x40011400);
    int u = 0, ris = 0, lsr = 0, count = 0;
    char c;
    struct uart *uartptr=&uarttab[0];

	uint8 flags = regptr->sr & 0xFF;
	//kprintf("%x", flags);	
	if (flags & UART_RXNE) {
		//ttyhandle_in(typtr, csrptr);
		//kprintf("rx\n");
		/*buff[count++]=csrptr->dr;
		if(count>=9){
			count=0;
			kprintf("%s\n",buff);
		}*/
	    uartptr->oirq++; //increment output IRQ count
        //lsr = regptr->lsr;  /* Read from LSR to clear interrupt */
        //regptr->icr |= PL011_ICR_TXIC; //clear transmitter interrupt
        count = 0;
        if (uartptr->ocount > 0) //if we have bytes in the buffer
        {
            /* Write characters to the lower half of the UART. */
            do
            {
                count++;
                uartptr->ocount--;
                regptr->dr = uartptr->out[uartptr->ostart]; //write a character to the FIFO
                uartptr->ostart = (uartptr->ostart + 1) % UART_OBLEN;
            }
            //while ((count < UART_FIFO_LEN) && (uartptr->ocount > 0));
            while ((count < PL011_FIFO_LEN) && (uartptr->ocount > 0));
        }

        if (count)
        {
            uartptr->cout += count;
            //signaln(uartptr->osema, count);
        }
        /* If no characters were written, set the output idle flag. */
        else
        {
            uartptr->oidle = TRUE;
        }
	}
	else if (flags & UART_TC)  {
		//ttyhandle_out(typtr, csrptr);
		//kprintf("rx\n");
		uartptr->iirq++; //increment input IRQ count
        count = 0;
        //while (regptr->lsr & UART_LSR_DR) //while there's data in the receive FIFO
        while ((regptr->sr & UART_RXNE) != 0) //while the receive FIFO is not empty
        {
            c = regptr->dr; //get a character from the FIFO
            if (uartptr->icount < UART_IBLEN)
            {
                uartptr->in
                    [(uartptr->istart +
                      uartptr->icount) % UART_IBLEN] = c;
                uartptr->icount++;
                count++;
            }
            else
            {
                uartptr->ovrrn++;
            }
        }
        uartptr->cin += count;
        sem_signaln(&uartptr->isema, count);
	}
}



uint32 uartWrite(void *buf, uint32 len)
{
    uint32 im;
    int count = 0;
    struct uart *uartptr=&uarttab[0];
    volatile struct uart_csreg *regptr;
    uint8 *buffer = buf;

    regptr = (struct uart_csreg *)0x40011400;


    im = disable();

    /* If in non-blocking mode, ensure there is enough space for the entire
     * write request */
    if ((uartptr->oflags & UART_OFLAG_NOBLOCK)
        && (sem_count(&uartptr->osema) < len))
    {
        restore(im);
        return SYSERR;
    }

    /* Put each character from the buffer into the output buffer for the
     * lower half */
    while (count < len)
    {
        /* If in non-blocking mode, ensure there is another byte of space
         * for output */
        if ((uartptr->oflags & UART_OFLAG_NOBLOCK)
            && (sem_count(&uartptr->osema) < 1))
        {
            break;
        }

        /* If the UART transmitter hardware is idle, write directly to the
         * hardware */
        if (uartptr->oidle)
        {
            uartptr->oidle = FALSE;
            //regptr->thr = *buffer++;
            regptr->dr = *buffer++;
            count++;
            uartptr->cout++;
        }
        /* Wait for space and place character in the output buffer for the
         * lower half; Preserve the circular buffer */
        else
        {
            sem_wait(&uartptr->osema);
            uartptr->out[(uartptr->ostart +
                          uartptr->ocount) % UART_OBLEN] = *buffer++;
            count++;
            uartptr->ocount++;
        }
    }

    restore(im);
    return count;
}
#endif