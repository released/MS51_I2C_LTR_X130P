

#define MAIN_CTRL								(0x00)
#define PS_LED									(0x01)
#define PS_PULSES								(0x02)
#define PS_MEAS_RATE							(0x03)

#define PART_ID									(0x06)
#define MAIN_STATUS								(0x07)
#define PS_DATA_0 								(0x08)
#define PS_DATA_1								(0x09)

#define INT_CFG 									(0x19)
#define INT_PST									(0x1A)
#define PS_THRES_UP_0							(0x1B)
#define PS_THRES_UP_1							(0x1C)
#define PS_THRES_LOW_0							(0x1D)
#define PS_THRES_LOW_1							(0x1E)
#define PS_CAN_0									(0x1F)
#define PS_CAN_1									(0x20)

/* Function Prototypes */

void INIT_LTR_X130P(void);
void PRINT_LTR_X130P(void);


