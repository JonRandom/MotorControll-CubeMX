

#define b0 1
#define b1 2
#define b2 4
#define b3 8
#define b4 16
#define b5 32
#define b6 64
#define b7 128
#define b8 256
#define b9 512
#define b10 1024
#define b11 2048
#define b12 4096
#define b13 8192
#define b14 16384
#define b15 32768
#define b16 (1<<16)
#define b17 (1<<17)
#define b18 (1<<18)
#define b19 (1<<19)
#define b20 (1<<20)
#define b21 (1<<21)
#define b22 (1<<22)
#define b23 (1<<23)
#define b24 (1<<24)
#define b25 (1<<25)
#define b26 (1<<26)
#define b27 (1<<27)
#define b28 (1<<28)
#define b29 (1<<29)
#define b30 (1<<30)
#define b31 (1<<31)

#define setmark GPIOB->ODR |= b6
#define togglemark GPIOB->ODR ^= b6
#define clearmark GPIOB->ODR &= ~b6

#define ledoff GPIOA->ODR |= b11
#define ledtoggle GPIOA->ODR ^= b11
#define ledon GPIOA->ODR &= ~b11





// add b12 to enable ch4 and b13 to invert ch4
#define phab b0+b2+b3+b11+b12+b13
#define phac b0+b2+b3+b7+b12+b13
#define phbc b3+b4+b6+b7+b12+b13
#define phba b4+b6+b7+b11+b12+b13
#define phca b7+b8+b10+b11+b12+b13
#define phcb b8+b10+b11+b3+b12+b13
#define alloff b3+b7+b11+b12+b13
