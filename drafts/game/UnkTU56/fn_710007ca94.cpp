// Unit 56 (base::global::CRntFile): reads bytes from the file into a buffer up to the terminating
// zero or 0x200 bytes, returning the count. Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

namespace {

// One byte of fn_710007c714 (src/base/global/CRntFile.cpp), inline.
inline void ReadByte(CRntFile* file, u8* dest) {
    u8* data = file->data;
    if (data) {
        u64 position = file->position;
        u64 end = file->size;
        u64 left = end - position;
        if (end > position) {
            if (1 + (u64)data + position > (u64)data + end) {
                memcpy(dest, data + position, left);
                file->position += left;
                return;
            }
            *dest = data[position];
            file->position = position + 1;
        }
    }
}

}  // namespace

u64 fn_710007ca94(CRntFile* file, u8* out) {
    u64 count = 0;
    u8 c = ' ';
    do {
        ReadByte(file, &c);
        out[count++] = c;
    } while (count < 0x200 && c);
    return count;
}
