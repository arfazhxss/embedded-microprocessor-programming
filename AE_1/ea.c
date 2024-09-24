#define PAOUT (volatile unsigned char *)0xFFFFFFF1
#define PADIR (volatile unsigned char *)0xFFFFFFF2
#define PBIN (volatile unsigned char *)0xFFFFFFF3
#define PBDIR (volatile unsigned char *)0xFFFFFFF5
#define CNTM (volatile unsigned int *)0xFFFFFFD0
#define CTCON (volatile unsigned char *)0xFFFFFFD8
#define CTSTAT (volatile unsigned char *)0xFFFFFFD9
#define IVECT (volatile unsigned int *)(0x20)

interrupt void intserv();
unsigned char digit = 0; /* Digit to be displayed */
unsigned char led = 0x1; /* LED state: 0/1 = on/off */
int main()
{
    *PADIR = 0xF1;                     /* Set Port A direction */
    *PBDIR = 0x00;                     /* Set Port B direction */
    *CTCON = 0x02;                     /* Stop Timer */
    *CTSTAT = 0x0;                     /* Clear “reached 0” flag */
    *CNTM = 100000000;                 /* Initialize Timer */
    *IVECT = (unsigned int *)&intserv; /* Set interrupt vector */
    asm(“MoveControl PSR, #0x40”);     /* CPU responds to IRQ */
    *CTCON = 0x11;                     /* Enable Timer interrupts
                                        * and start counting */
    *PAOUT = 0x01;                     /* Display 0, turn LED off */
    while (1)
    {
        while ((*PBIN & 0x1) != 0)
            ; /* Wait until SW is pressed */
        while ((*PBIN & 0x1) == 0)
            ; /* Wait until SW is released */
        if (led == 0x1)
            led = 0x0; /* If off, turn LED on */
        else
            led = 0x1;                 /* Else, turn LED off */
        *PAOUT = ((digit << 4) | led); /* Update Port A */
        /* We can also put “*CTCON &= 0xEF;” before and “*CTCON |= 0x10;”
         * after the last statement, to make sure that intserv() is not
         * interfering with main() accessing shared digit/led/PAOUT */
    }
    exit(0);
}
interrupt void intserv()
{
    *CTSTAT = 0x0;                 /* Clear “reached 0” flag */
    digit = (digit + 1) % 10;      /* Increment digit */
    *PAOUT = ((digit << 4) | led); /* Update Port A */
}