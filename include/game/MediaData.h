#pragma once

#include "types.h"

namespace movie {

class MediaData {
public:
    MediaData();
    ~MediaData();
    void Clear();
    bool FindString(const char* key, const char** out) const;
    bool FindInt32(const char* key, s32* out) const;
    bool FindInt64(const char* key, s64* out) const;

private:
    u8 storage[0x8];
};

}  // namespace movie
