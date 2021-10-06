

#include <stdio.h>
#include <stdlib.h>

#include "MS51_16K.h"


void HAL_Delay(uint16_t timing);
unsigned char ReadByte (unsigned char slave_addr , unsigned char register_addr);
int WriteByte(unsigned char slave_addr , unsigned char register_addr , unsigned char value );

