// UnkTU180: looks a uniform block up by name: copies the name into a function-local scratch string,
// compares it with each block's name and writes the block's index plus 0xffff to `outIndex`. Names
// and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"


using base::global::CRntString;

struct UnkUnit180Table {
    u8 pad_0x00[0x4c];
    u32 count;
    u8 pad_0x50[0x60];
    u8* blocks;  // 0xd8 bytes each, the name first
};
static_assert(__builtin_offsetof(UnkUnit180Table, blocks) == 0xb0);

// A CRntString built by its out-of-line default constructor.
struct UnkScratchString {
    alignas(8) u8 storage[sizeof(CRntString)];
    CRntString* Get() { return reinterpret_cast<CRntString*>(storage); }
    UnkScratchString() { fn_710007e6c4(storage); }
    ~UnkScratchString() { CRntString_Destroy(Get()); }
};

bool fn_710015dc04(UnkUnit180Table* table, const void* key, u32* outIndex) {
    static UnkScratchString s_name;
    CRntString_AssignCString(s_name.Get(), static_cast<const char*>(key));
    for (u64 i = 0; i < table->count; ++i) {
        if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(table->blocks + i * 0xd8),
                          reinterpret_cast<const UnkCompared_0x30*>(s_name.Get()))) {
            *outIndex = i + 0xffff;
            return true;
        }
    }
    return false;
}
