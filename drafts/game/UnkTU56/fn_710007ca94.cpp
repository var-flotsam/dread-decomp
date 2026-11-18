// Unit 56 (base::global::CRntFile): reads bytes from the file into a buffer up to the terminating
// zero or 0x200 bytes, returning the count. Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

namespace {

// fn_710007c714 (src/base/global/CRntFile.cpp) for single bytes, which the original inlines here:
// reads one byte at the position unless the position is at the end.
inline void ReadByte(CRntFile* file, char* dest) {
    u8* data = file->data;
    if (data) {
        u64 position = file->position;
        u64 end = file->size;
        u64 left = end - position;
        if (end > position) {
            if (position + 1 + (u64)data > end + (u64)data) {
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

u64 fn_710007ca94(CRntFile* file, char* out) {
    u64 count = 0;
    char c = ' ';
    do {
        ReadByte(file, &c);
        out[count] = c;
        ++count;
    } while (count < 0x200 && c != 0);
    return count;
}
