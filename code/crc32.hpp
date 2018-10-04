
#ifndef __CRC32_HDR__
#define __CRC32_HDR__

class CRC32
{
private:
    typedef unsigned long ulong;

    enum { CRC_TABLE_SIZE = 0x100 };

    bool xor_output = true;
    bool reflect_output = false;
    unsigned long polynomial = 0x04C11DB7;
    bool big_endian = false;
    unsigned long starting = 0xFFFFFFFF;

    ulong crc_table[CRC_TABLE_SIZE];
    void (CRC32::*cycle) (ulong*, const char);

    void crc_fill_table()
    {
        ulong* table = crc_table;

        ulong lsb = (big_endian) ? 1 << 31 : 1; /* least significant bit */
        ulong poly = (big_endian) ? polynomial : crc_reflect (polynomial);
        int c, i;

        for (c = 0; c < CRC_TABLE_SIZE; c++, table++)
        {
            *table = (big_endian) ? c << 24 : c;
            for (i = 0; i < 8; i++)
            {
                if (*table & lsb)
                {
                    *table = (big_endian) ? *table << 1 : *table >> 1;
                    *table ^= poly;
                }
                else
                {
                    *table = (big_endian) ? *table << 1 : *table >> 1;
                }
                *table &= 0xFFFFFFFF;
            }
        }
    }

    void crc_be_cycle (ulong* remainder, const char c)
    {
        ulong byte = crc_table[ (((*remainder) >> 24) ^ c) & 0xff];
        *remainder = (((*remainder) << 8) ^ byte) & 0xFFFFFFFF;
    }

    void crc_le_cycle (ulong* remainder, const char c)
    {
        ulong byte = crc_table[ ((*remainder) ^ c) & 0xFF];
        *remainder = ((*remainder) >> 8) ^ byte;
    }

    ulong crc_reflect (ulong input)
    {
        ulong reflected = 0;
        int i;
        for (i = 0; i < 4 * 8; i++)
        {
            reflected <<= 1;
            reflected |= input & 1;
            input >>= 1;
        }
        return reflected;
    }
public:
    CRC32()
    {
        cycle = (big_endian) ? &CRC32::crc_be_cycle : &CRC32::crc_le_cycle;
        crc_fill_table();
    }

    unsigned long calc (const void* data, const int len)
    {
        ulong remainder = starting;

        for (int i = 0; i < len; i++)
        {
            (this->*cycle) (&remainder, ((const unsigned char*)data)[i]);
        }
        if (xor_output)
        {
            remainder ^= 0xFFFFFFFF;
        }
        if (reflect_output)
        {
            remainder = crc_reflect (remainder);
        }

        return remainder;
    }
};

#endif // __CRC32_HDR__