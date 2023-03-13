#ifndef __UART_H__
#define __UART_H__

#define UART0_Timer1  0
#define UART0_Timer3  1
#define UART1_Timer3  2
#define UART0 0
#define UART1 1

#define MAX_BUFFER 128

extern   bit PRINTFG,uart0_receive_flag,uart1_receive_flag;
extern   unsigned char uart0_receive_data,uart1_receive_data;

void UART_Open(unsigned long u32SysClock, unsigned char u8UARTPort,unsigned long u32Baudrate);
void UART_Send_Data(unsigned char UARTPort, unsigned char c);
unsigned char Receive_Data(unsigned char UARTPort);
void Enable_UART0_VCOM_printf_24M_115200(void);

void uartFresh(void);
void uartResetNByte(void);
uint8_t IsAvailable(void);
uint8_t IsFull(void);
uint8_t IsEmpty(void);
unsigned char uartReadByte(void);
unsigned char IsData(void);
void uartWriteByte(unsigned char item);

#endif