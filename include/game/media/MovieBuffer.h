#pragma once

// The movie library's buffer (movie::Buffer, 0x40 bytes), whose members are all out of line in the
// library; the movie stream units hold arrays of them. Only what the hand-written sources call is
// declared.

#include "types.h"

namespace movie {
class Buffer {
public:
    Buffer();
    explicit Buffer(u64 size);
    Buffer(void* data, u64 size);
    Buffer(const Buffer& other);
    ~Buffer();
    void SetRange(s32 offset, s32 size);

private:
    u8 storage[0x40];
};
}  // namespace movie
