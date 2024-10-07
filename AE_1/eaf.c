#define PCONT (volatile unsigned char *)0xFFFFFFF7
#define CNTM (volatile unsigned int *)0xFFFFFFD0
#define CTCON (volatile unsigned char *)0xFFFFFFD8
#define CTSTAT (volatile unsigned char *)0xFFFFFFD9
#define IVECT (volatile unsigned int *)(0x20)
interrupt void intserv();
int main()
{
    char digit = 0;                    /* Digit to be displayed */
    *PBDIR = 0b11110000;               /* Set Port B direction */
    *IVECT = (unsigned int *)&intserv; /* Set interrupt vector */
    asm(“MoveControl PSR, #0x40”);     /* CPU responds to IRQ */
    *PCONT = 0b01000000;               /* Enable PBIN interrupts */
    *CTCON = 0b0010;                   /* Stop Timer */
    *CSTAT = 0b0000;                   /* Clear “reached 0” flag */
    *CNTM = 100000000;                 /* Initialize Timer */
    *PBOUT = 0b0000;                   /* Display 0 */
    while (1)
    {
        while ((*CTSTAT & 0x1) == 0)
            ;                     /* Wait until 0 is reached */
        *CSTAT = 0x0;             /* Clear “reached 0” flag */
        digit = (digit + 1) % 10; /* Increment digit */
        *PBOUT = digit << 4;      /* Update display */
    }
    exit(0);
}
interrupt void intserv()
{
    unsigned char sample;    /* Port B input sample */
    sample = *PBIN & 0b0011; /* Sample PBIN, isolate bits [1:0] */
    if (sample == 0b0001)
        *CTCON = 0b0001; /* Start Timer */
    else if (sample == 0b0010)
        *CTCON = 0b0010; /* Stop Timer */
}