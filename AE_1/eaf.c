#define PAOUT (volatile unsigned char *)0xFFFFFFF1
#define PADIR (volatile unsigned char *)0xFFFFFFF2
#define PBIN (volatile unsigned char *)0xFFFFFFF3
#define PBDIR (volatile unsigned char *)0xFFFFFFF5
#define CNTM (volatile unsigned int *)0xFFFFFFD0
#define CTCON (volatile unsigned char *)0xFFFFFFD8
#define CTSTAT (volatile unsigned char *)0xFFFFFFD9
#define IVECT (volatile unsigned int *)(0x20)

interrupt void intserv();
