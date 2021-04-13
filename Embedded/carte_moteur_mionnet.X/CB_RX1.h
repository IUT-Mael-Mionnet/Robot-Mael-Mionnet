#ifndef CB_RX1_H
#define	CB_RX1_H

void CB_RX1_Add(unsigned char value);
unsigned char CB_RX1_Get();
unsigned char CB_RX1_IsDataAvailable();
int CB_RX1_GetRemainingSize();
int CB_RX1_GetDataSize();

#endif

