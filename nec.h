#ifndef TINY_NEC_RECEIVER_H_
#define TINY_NEC_RECEIVER_H_

// CONFIG BEGIN

/*
You can define data types below, or just include <stdint.h> if possible.
*/
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int
#define int8_t signed char

/*
Set logic level.
*/
#define NEC_LOGICAL_HIGH 0
#define NEC_LOGICAL_LOW 1

#define NEC_USE_REPEAT 1                   // Set to 1 to enable repeat counting.
#define NEC_USE_ADDRESS 1                  // Set to 1 to enable the address.
#define NEC_USE_INVERSE_ADDRESS 1          // Set to 1 to enable the inverted address.
#define NEC_USE_INVERSE_COMMAND 1          // Set to 1 to enable the inverted command.
#define NEC_USE_GET_DATA_CALLBACK 1        // Set to 1 to enable the nec_get_data_callback.
#define NEC_USE_GET_DATA_REPEAT_CALLBACK 1 // Set to 1 to enable the nec_get_data_repeat_callback.
#define NEC_USE_GET_DATA_FINISH_CALLBACK 1 // Set to 1 to enable the nec_get_data_finish_callback.

#define NEC_REPEAT_COUNTER_TYPE uint8_t // Set NEC_REPEAT_COUNTER_TYPE.

// CONFIG END

typedef struct
{
    struct
    {
        uint8_t command;

#if (NEC_USE_ADDRESS != 0)
        uint8_t address;
#endif

#if (NEC_USE_INVERSE_ADDRESS != 0)
        uint8_t address_r;
#endif

#if (NEC_USE_INVERSE_COMMAND != 0)
        uint8_t command_r;
#endif
    } nec_command;

#if (NEC_USE_REPEAT != 0)
    NEC_REPEAT_COUNTER_TYPE repeat;
#endif

} nec_command_type;

typedef struct
{
    uint16_t nec_low_tick_cnt : 11;
    uint16_t nec_high_tick_cnt : 11;
    uint8_t nec_receiving_bit_cnt : 5;
    uint8_t nec_receiving_start : 2;
    uint8_t nec_receiving_repeat : 2;
} nec_receiving_data_type;

void nec_scan_100us(uint8_t level);

#if (NEC_USE_GET_DATA_CALLBACK != 0)
extern void nec_get_data_callback(nec_command_type *command);
#endif

#if (NEC_USE_GET_DATA_REPEAT_CALLBACK != 0)
extern void nec_get_data_repeat_callback(nec_command_type *command);
#endif

#if (NEC_USE_GET_DATA_FINISH_CALLBACK != 0)
extern void nec_get_data_finish_callback(nec_command_type *command);
#endif

#endif /* TINY_NEC_RECEIVER_H_ */