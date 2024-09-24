#define PAOUT (volatile unsigned char *)0xFFFFFFF1
#define PADIR (volatile unsigned char *)0xFFFFFFF2
#define PBIN (volatile unsigned char *)0xFFFFFFF3
#define PBDIR (volatile unsigned char *)0xFFFFFFF5
#define CNTM (volatile unsigned int *)0xFFFFFFD0
#define CTCON (volatile unsigned char *)0xFFFFFFD8
#define CTSTAT (volatile unsigned char *)0xFFFFFFD9
#define IVECT (volatile unsigned int *)(0x20)

interrupt void intserv();
unsigned char digit = 0;            /* Digit to be displayed */
unsigned char enable_increment = 0; /* Flag: 1 if E is pressed, 0 if D is pressed */

int main()
{
    *PADIR = 0xF0;                     /* Set Port A direction, leaving PA0-3 as inputs */
    *PBDIR = 0x00;                     /* Set Port B direction as inputs */
    *CTCON = 0x02;                     /* Stop Timer */
    *CTSTAT = 0x0;                     /* Clear “reached 0” flag */
    *CNTM = 100000000;                 /* Initialize Timer */
    *IVECT = (unsigned int *)&intserv; /* Set interrupt vector */
    asm("MoveControl PSR,#0x40");      /* CPU responds to IRQ */
    *CTCON = 0x11;                     /* Enable Timer interrupts and start counting */
    *PAOUT = 0x00;                     /* Display 0 initially */

    while (1)
    {
        /* Check if E (PB0) is pressed */
        if ((*PBIN & 0x01) != 0)
        {                         /* If E is pressed */
            enable_increment = 1; /* Enable digit incrementing */
        }

        /* Check if D (PB1) is pressed */
        if ((*PBIN & 0x02) != 0)
        {                         /* If D is pressed */
            enable_increment = 0; /* Disable digit incrementing */
        }
    }

    exit(0);
}

interrupt void intserv()
{
    *CTSTAT = 0x0; /* Clear “reached 0” flag */

    /* Increment the digit only if enabled by pressing E */
    if (enable_increment)
    {
        digit = (digit + 1) % 10; /* Increment digit (0-9) */
    }

    *PAOUT = digit; /* Update 7-segment display with the new digit */
}
