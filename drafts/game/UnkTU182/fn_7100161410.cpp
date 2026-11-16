// Unit 182: the graphics format for a vertex attribute description packed in a word (the low byte
// the component type, the next byte 1 for normalized, the high half the component layout), or 0
// when there is none. Names and values are placeholders.
#include "types.h"

static inline u32 PlainFormat(u32 type, u32 layout) {
    switch (type) {
    case 3:
        switch (layout) {
        case 12:
            return 0x4b;
        case 1:
        case 14:
            return 0x42;
        case 2:
            return 0x44;
        case 15:
            return 0x50;
        }
        break;
    case 2:
        if (layout == 16)
            return 0xa;
        break;
    case 1:
        switch (static_cast<u16>(layout)) {
        case 1:
            return 0x6d;
        case 2:
            return 0x25;
        case 3:
            return 1;
        case 4:
            return 1;
        case 5:
            return 0xd;
        case 6:
            return 0x3c;
        case 7:
            return 0x3b;
        case 9:
            return 1;
        case 10:
            return 1;
        case 11:
            return 0xd;
        case 12:
            return 0xd;
        case 13:
            return 0x12;
        case 14:
            return 0x6d;
        }
        break;
    }
    return 0;
}

static inline u32 NormalizedFormat(u32 type, u32 layout) {
    switch (type) {
    case 3:
        switch (layout) {
        case 1:
        case 14:
            return 0x46;
        case 2:
            return 0x48;
        }
        break;
    case 1:
        switch (layout) {
        case 1:
        case 14:
            return 0x37;
        case 2:
            return 0x38;
        }
        break;
    }
    return 0;
}

u32 fn_7100161410(u32 desc) {
    u32 layout = desc >> 16;
    u32 type = desc & 0xff;
    switch ((desc >> 8) & 0xff) {
    case 1:
        return NormalizedFormat(type, layout);
    case 0:
        return PlainFormat(type, layout);
    }
    return 0;
}
