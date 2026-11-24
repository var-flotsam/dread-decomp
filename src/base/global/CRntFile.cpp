// base::global::CRntFile (include/base/global/CRntFile.h): the file held in memory. Only the
// position change is defined here so far.
#include "base/global/CRntFile.h"

using base::global::CRntFile;

// Sets the position, relative to the start (0), the current position (1) or the end (2); see the
// header. The upper bound is checked twice, once on the offset and once on pointers into the block.
bool CRntFile_Seek(CRntFile* file, u64 offset, s32 whence) {
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

#include <string.h>

// Allocates `size` bytes from the file's allocator, taking the default one on first use. Null when
// there is no allocator at all.
static inline void* AllocateBlock(CRntFile* file, s64 size) {
    base::IAllocator* allocator = file->allocator;
    if (!allocator) {
        allocator = GetDefaultAllocator();
        file->allocator = allocator;
        if (!allocator)
            return nullptr;
    }
    return allocator->vf_0x40(size, file->alloc_align, file->alloc_flags);
}

bool CRntFile_Allocate(CRntFile* file, s64 size) {
    u8* block = static_cast<u8*>(AllocateBlock(file, size));
    file->data = block;
    if (block) {
        file->size = size;
        file->position = 0;
    }
    return block != nullptr;
}

// NON_MATCHING(minor): one add has its operands swapped (the position and the block's end are
// summed in the other order); the same instructions otherwise. The barrier keeps a second product
// of count * size, as the original has, rather than one hoisted above the branch.
void CRntFile_Reserve(CRntFile* file, s64 size, s64 count) {
    u8* block = file->data;
    s64 bytes;
    if (!block) {
        bytes = count * size;
        block = static_cast<u8*>(AllocateBlock(file, bytes));
        file->data = block;
        if (block) {
            file->size = bytes;
            file->position = 0;
        }
    } else {
        asm("" : "+r"(count));
        bytes = count * size;
    }
    u64 pos = file->position;
    if ((u64)(block + bytes) + pos <= file->size + (u64)block)
        return;
    s64 grown = file->size + bytes;
    u8* bigger = static_cast<u8*>(AllocateBlock(file, grown));
    memcpy(bigger, file->data, file->size);
    if (file->allocator)
        file->allocator->vf_0x48(file->data);
    file->data = bigger;
    file->size = grown;
}

u64 CRntFile_Write(CRntFile* file, const void* data, u64 size) {
    CRntFile_Reserve(file, size, 1);
    memcpy(file->data + file->position, data, size);
    file->position += size;
    return size;
}

// left / size, where a zero size gives 0 as AArch64's udiv does (x86-64 would trap; SwapElements
// ignores the count for sizes other than 2, 4 and 8 anyway).
static inline u64 ElementCount(u64 left, u64 size) {
#if defined(__aarch64__)
    return left / size;
#else
    return size ? left / size : 0;
#endif
}

// Reverses the bytes of each of `count` elements of `size` bytes (2, 4 or 8; others are left).
static inline void SwapElements(u8* p, u64 size, u64 count) {
    if (size == 2) {
        for (; count != 0; --count, p += 2) {
            u8 t = p[0];
            p[0] = p[1];
            p[1] = t;
        }
    } else if (size == 8) {
        for (; count != 0; --count, p += 8) {
            for (int j = 0; j < 4; ++j) {
                u8 t = p[j];
                p[j] = p[7 - j];
                p[7 - j] = t;
            }
        }
    } else if (size == 4) {
        for (; count != 0; --count, p += 4) {
            for (int j = 0; j < 2; ++j) {
                u8 t = p[j];
                p[j] = p[3 - j];
                p[3 - j] = t;
            }
        }
    }
}

// NON_MATCHING(minor): the same reads and swaps, but Clang lays the blocks out differently (the
// early return and the exit are shared, and the byte-swap loops come out a little differently);
// the behaviour is the same.
u64 CRntFile_Read(CRntFile* file, void* dest, u64 size, u64 count) {
    u8* data = file->data;
    if (data) {
        u64 position = file->position;
        u64 end = file->size;
        u64 left = end - position;
        if (end > position) {
            u64 bytes = count * size;
            if (bytes + (u64)data + position > (u64)data + end) {
                memcpy(dest, data + position, left);
                if (file->swap)
                    SwapElements(static_cast<u8*>(dest), size, ElementCount(left, size));
                file->position += left;
                return left;
            }
            memcpy(dest, data + position, bytes);
            if (file->swap)
                SwapElements(static_cast<u8*>(dest), size, count);
            file->position += bytes;
            return bytes;
        }
    }
    return 0;
}

// Writes `count` elements of `size` bytes at the position (growing the file), moves the position
// on; returns the bytes written.
u64 CRntFile_WriteElements(CRntFile* file, const void* data, u64 size, u64 count) {
    CRntFile_Reserve(file, size, count);
    u64 bytes = count * size;
    __builtin_memcpy(file->data + file->position, data, bytes);
    file->position += bytes;
    return bytes;
}

// Writes `size` bytes of `value` at the position (growing the file), moves the position on; returns
// `size`.
u64 CRntFile_Fill(CRntFile* file, s32 value, u64 size) {
    CRntFile_Reserve(file, 1, size);
    __builtin_memset(file->data + file->position, value, size);
    file->position += size;
    return size;
}

// Reads a string in place: points `out` at the position and moves the position past the string's
// terminator, or on by 0x200 bytes when none comes first; returns the bytes passed.
// NON_MATCHING(minor): the original reloads the position and the block in every pass of the loop
// (and loads the block before storing the position); Clang keeps the position in a register and
// counts from it. The behaviour is the same.
s64 CRntFile_ReadCString(void* stream, const char** out) {
    CRntFile* file = static_cast<CRntFile*>(stream);
    *out = reinterpret_cast<const char*>(file->data + file->position);
    s32 count = 0;
    while (count != 0x200) {
        ++count;
        if (file->data[file->position++] == '\0')
            break;
    }
    return count;
}
