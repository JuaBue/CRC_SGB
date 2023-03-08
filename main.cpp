#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <iomanip>

using namespace std;

#define Polynomial      0x07
#define InitValue       0x00
#define FinalXOR        0x00
#define PolySize        0x80

void compute_table( uint8_t table[256], uint8_t polynomial ) 
{
    uint8_t crc = PolySize;
    size_t i, j;
    memset(table, 0, 256);

    for (i = 1; i < 256; i <<= 1) 
    {
        if ( crc & 0x80 )
        {
            crc = (crc << 1) ^ polynomial;
        }
        else
        {
            crc <<= 1;
        }

        for ( j = 0; j < i; j++ )
        {
            table[i + j] = crc ^ table[j];
        }
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
    uint8_t calculo[16]      = { 0x00, 0x2C, 0xB1, 0x84, 0x3E, 0xC8, 0x00, 0x00, 0x01, 0x2C, 0xB1, 0x84, 0x3E, 0xC8, 0x00, 0x00 };

    compute_table( table, Polynomial );

    show_table( table );

    valor_salida = calculate_crc ( InitValue, sizeof(calculo), calculo , table );

    printf( "Valor 0x%02X\n", valor_salida );

    return 0;
}

