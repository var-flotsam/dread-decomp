// Unit 71 of tools/tu_map.py (0x71000943bc-0x7100094fc0, no reflected class): the copy constructor
// of a block of bytes that either owns its copy (+0x10 set) or borrows the memory it points at. An
// owned, non-empty block is copied into a new allocation (fn_7100083ba0, flags 0xf); anything else
// is copied as it is. Names are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

// The original reloads the source after every store (its accesses may alias the copy).
struct __attribute__((may_alias)) UnkBlob_71000947e4 {
    u32 size;
    void* data;
    bool owned;

    UnkBlob_71000947e4(const UnkBlob_71000947e4& other) {
        size = 0;
        data = nullptr;
        owned = false;
        if (u32 n = other.size) {
            if (other.owned) {
                data = nullptr;
                size = 0;
                owned = true;
                data = fn_7100083ba0(n, 0xf);
                size = n;
                memcpy(data, other.data, other.size);
                return;
            }
        }
        data = other.data;
        size = other.size;
        owned = other.owned;
    }
};

void fn_71000947e4(UnkBlob_71000947e4* self, const UnkBlob_71000947e4* other) { new (self) UnkBlob_71000947e4(*other); }
