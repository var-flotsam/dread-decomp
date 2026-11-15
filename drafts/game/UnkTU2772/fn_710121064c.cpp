// UnkTU2772 (nn::atk): classifies a header type id into 0, 1 or 2 (0x220c is 0; 0x220d and 0 are 2). Names and layouts are placeholders.
#include "types.h"

struct UnkAtkHeader {
    u16 type;  // 0x0
};

s32 fn_710121064c(const UnkAtkHeader* header) {
    switch (header->type) {
    case 0x220c:
        return 0;
    case 0x220d:
        return 2;
    case 0:
        return 2;
    default:
        return 1;
    }
}
