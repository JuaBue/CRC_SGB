#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <iomanip>

using namespace std;

#define Polynomial      0xF4
#define InitValue       0xFF
#define FinalXOR        0x00
#define PolySize        0x80

void compute_table( uint8_t table[256], uint8_t polynomial ) 
{
    uint8_t remainder = 0;
    size_t dividend, bit;
    memset(table, 0, 256);

    for (dividend = 0; dividend < 256; ++dividend) 
    {
        remainder = dividend;

        for ( bit = 8; bit > 0; --bit )
        {
            if ( remainder & 1 )
            {
                remainder = ( remainder >> 1 ) ^ Polynomial;
            }
            else
            {
                remainder = (remainder >> 1);
            }
        }

        table[dividend] = remainder;
    }
}

void show_table( uint8_t table[256] )
{
    size_t i;

    for (i = 0; i < 256; i++) 
    {
        if ((( i % 16 ) == 0) && ( i != 0))
        {
            printf( "\n0x%02X, ", table[i] );
        }
        else
        {
            printf( "0x%02X, ", table[i] );
        }
    }

    printf("\n\n");
}


uint8_t calculate_crc ( uint8_t cur_crc, size_t length, uint8_t* buffer, uint8_t table[256] )
{
    uint16_t index = 0U;

    while( 0U != length )
    {
        cur_crc = table[cur_crc ^ buffer[index]];
        index++;
        length--;
    }

    return ( cur_crc ^ FinalXOR );
}

int main(void) {
    uint8_t table[256]      = { 0 };
    uint8_t valor_salida    = 0;
    uint8_t calculo[2]      = { 0x64, 0x64 };

    compute_table( table, Polynomial );

    show_table( table );

    valor_salida = calculate_crc ( InitValue, sizeof(calculo), calculo , table );

    printf( "Valor 0x%02X\n", valor_salida );

    return 0;
}

