// base::global::CRntFile (include/base/global/CRntFile.h): the file held in memory. Only the
// position change is defined here so far.
#include "base/global/CRntFile.h"

using base::global::CRntFile;

// Sets the position, relative to the start (0), the current position (1) or the end (2); see the
// header. The upper bound is checked twice, once on the offset and once on pointers into the block.
bool fn_710007c00c(CRntFile* file, u64 offset, s32 whence) {
    u8* data = file->data;
    if (!data)
        return false;
    if (whence != 0) {
        u64* base = &file->position;
        if (whence != 1) {
            if (whence != 2)
                return false;
            base = &file->size;
            offset = -1;
        }
        offset = *base + offset;
    }
    u64 size = file->size;
    if (offset >= size) {
        file->position = size;
        return false;
    }
    if ((u64)data + offset > (u64)data + size)
        file->position = size;
    else
        file->position = offset;
    return true;
}
