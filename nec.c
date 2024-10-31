#include "nec.h"

nec_command_type nec_receiving_command;
nec_receiving_data_type nec_receiving_data;

void nec_receiving_data_reset()
{
    nec_receiving_data.nec_low_tick_cnt = 0;
    nec_receiving_data.nec_high_tick_cnt = 0;
    nec_receiving_data.nec_receiving_bit_cnt = 0;
    nec_receiving_data.nec_receiving_start = 0;
    nec_receiving_data.nec_receiving_repeat = 0;

    nec_receiving_command.nec_command.command = 0;

#if (NEC_USE_ADDRESS != 0)
    nec_receiving_command.nec_command.address = 0;
#endif

#if (NEC_USE_INVERSE_ADDRESS != 0)
    nec_receiving_command.nec_command.address_r = 0;
#endif

#if (NEC_USE_INVERSE_COMMAND != 0)
    nec_receiving_command.nec_command.command_r = 0;
#endif

#if (NEC_USE_REPEAT != 0)
    nec_receiving_command.repeat = 0;
#endif
}

void nec_scan_100us(uint8_t level)
{
    if (level == NEC_LOGICAL_LOW)
    {
        nec_receiving_data.nec_low_tick_cnt++;

        if (80 <= nec_receiving_data.nec_high_tick_cnt && nec_receiving_data.nec_high_tick_cnt <= 100)
        {
            if (nec_receiving_data.nec_receiving_start == 0)
            {
                nec_receiving_data.nec_receiving_start = 1;
            }
            else if (nec_receiving_data.nec_receiving_start == 2 && nec_receiving_data.nec_receiving_repeat == 0)
            {
                nec_receiving_data.nec_receiving_repeat = 1;
            }
        }

        nec_receiving_data.nec_high_tick_cnt = 0;
    }
    else
    {
        nec_receiving_data.nec_high_tick_cnt++;

        if (3 <= nec_receiving_data.nec_low_tick_cnt && nec_receiving_data.nec_low_tick_cnt <= 8)
        {
            if (nec_receiving_data.nec_receiving_start == 2)
            {
                nec_receiving_data.nec_receiving_bit_cnt++;
            }
        }
        else if (15 <= nec_receiving_data.nec_low_tick_cnt && nec_receiving_data.nec_low_tick_cnt <= 19)
        {
            if (nec_receiving_data.nec_receiving_start == 2)
            {
                int8_t bit_pos = nec_receiving_data.nec_receiving_bit_cnt & (8 - 1);
                switch (nec_receiving_data.nec_receiving_bit_cnt >> 3)
                {
#if (NEC_USE_ADDRESS != 0)
                case 0:
                    nec_receiving_command.nec_command.address |= (1 << bit_pos);
                    break;
#endif
#if (NEC_USE_INVERSE_ADDRESS != 0)
                case 1:
                    nec_receiving_command.nec_command.address_r |= (1 << bit_pos);
                    break;
#endif
                case 2:
                    nec_receiving_command.nec_command.command |= (1 << bit_pos);
                    break;
#if (NEC_USE_INVERSE_COMMAND != 0)
                case 3:
                    nec_receiving_command.nec_command.command_r |= (1 << bit_pos);
                    break;
#endif
                default:
                    break;
                }

#if (NEC_USE_GET_DATA_FLAG != 0)
                if (nec_receiving_data.nec_receiving_bit_cnt == 31)
                {
                    nec_receiving_data.nec_get_data_flag = 1;
                }
#endif
                if (nec_receiving_data.nec_receiving_bit_cnt != 31)
                {
                    nec_receiving_data.nec_receiving_bit_cnt++;
                }
            }
        }
        else if (20 <= nec_receiving_data.nec_low_tick_cnt && nec_receiving_data.nec_low_tick_cnt <= 25)
        {
            if (nec_receiving_data.nec_receiving_repeat == 1)
            {
#if (NEC_USE_REPEAT != 0)
                nec_receiving_command.repeat++;
#endif
                nec_receiving_data.nec_receiving_repeat = 0;
#if (NEC_USE_GET_DATA_REPEAT_FLAG != 0)
                nec_receiving_data.nec_get_data_repeat_flag = 1;
#endif
            }
        }
        else if (42 <= nec_receiving_data.nec_low_tick_cnt && nec_receiving_data.nec_low_tick_cnt <= 48)
        {
            if (nec_receiving_data.nec_receiving_start == 1)
            {
                nec_receiving_data.nec_receiving_start = 2;
            }
        }

        nec_receiving_data.nec_low_tick_cnt = 0;
    }

    if (nec_receiving_data.nec_low_tick_cnt > 1200)
    {
#if (NEC_USE_GET_DATA_FINISH_FLAG != 0)
        if (nec_receiving_data.nec_receiving_bit_cnt == 31)
        {
            nec_receiving_data.nec_get_data_finish_flag = 1;
        }
#endif
        nec_receiving_data_reset();
    }
}
