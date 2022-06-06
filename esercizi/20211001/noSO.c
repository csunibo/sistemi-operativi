typedef unsigned char uint8_t;
#define DDRC (*(volatile uint8_t *) 0x27)
#define PORTC (*(volatile uint8_t *) 0x28)

int main(void)
{
        volatile int i;
        DDRC |= 1 << 5;
        for (;;) {
                for (i=0; i < 32767; i++)
                        ;
                PORTC ^= 1 << 5;
        }
}
