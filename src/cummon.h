#ifndef CUMMON_H_INCLUDED

#define Q_UNUSED(x) (void)(x)
#define NELEMS(x) ((sizeof (x))/(sizeof ((x)[0])))
#define BIT_LO_24(x) ((x) &  0xFFF)
#define BIT_LO_16(x) ((x) &   0xFF)
#define BIT_LO_8(x)  ((x) &    0xF)
#define BIT_HI_8(x)  ((x) & 0xF000)
#define BIT_HI_16(x) ((x) & 0xFF00)
#define BIT_HI_24(x) ((x) & 0xFFF0)

#endif // CUMMON_H_INCLUDED