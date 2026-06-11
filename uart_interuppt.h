// UART0 functions
void InitUART0(void);      // Initialize UART0
void UART0_Tx(char ch);    // Send one character
char UART0_Rx(void);       // Receive one character
void UART0_Str(char *s);   // Send a string

// UART0 receive buffer and flag
extern char buff[];
extern unsigned char r_flag;

// UART1 functions
void UART1_Tx(char ch);    // Send one character
char UART1_Rx(void);       // Receive one character
void UART1_Str(char *s);   // Send a string

// UART1 receive buffer and flag
extern char buff1[];
extern unsigned char r_flag1;

// External Interrupt 1
void eint1_isr(void) __irq; // Interrupt service routine
void enable_ent1(void);     // Enable EINT1

// Application functions
void adminmode(void);       // Administrator mode
void rtc_edit(void);        // Edit RTC time

// GSM status flag
extern int gsm_flag;
