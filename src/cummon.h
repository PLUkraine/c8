#ifndef CUMMON_H_INCLUDED

#define Q_UNUSED(x) (void)(x)
#define NELEMS(x) ((sizeof (x))/(sizeof ((x)[0])))
#define BIT_LO_12(x) ((x) &  0xFFF)
#define BIT_LO_8(x)  ((x) &   0xFF)
#define BIT_LO_4(x)  ((x) &    0xF)
#define BIT_HI_4(x)  (((x) & 0xF000) >> 12)
#define BIT_HI_8(x)  (((x) & 0xFF00) >> 8)
#define BIT_HI_12(x) (((x) & 0xFFF0) >> 4)
#define NIMB_3(x)    (((x) & 0x0F00) >> 8)
#define NIMB_2(x)    (((x) & 0x00F0) >> 4)

#endif // CUMMON_H_INCLUDED