#ifndef SPI_H
#define SPI_H

void initSPI();
void eightBitCommand(unsigned char data);
void twoByteCommand(unsigned char data1, unsigned char data2);
void sixteenCommand(unsigned char data1, unsigned char data2);

#endif
