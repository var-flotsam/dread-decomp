// tools/tu_map.py unit 1 (0x7100001480), a unit with nothing but its static initialiser: the global
// empty file-path id. The file-path pool's counterpart of unit 0's empty string id
// (fn_7100001390, src/engine/HostArguments.cpp).
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

// The guard word after the id (an inline variable's: the low byte is tested, the word set to 1).
extern u64 g_unk_7101d470f8;

void fn_7100001480() {
    if (!(*reinterpret_cast<u8*>(&g_unk_7101d470f8) & 1)) {
        g_unk_7101d470e8.data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        g_unk_7101d470e8.data = &g_unk_7101d47200->empty;
        fn_7100000250(fn_7100000c7c, &g_unk_7101d470e8, g_unk_7101a19000);
        g_unk_7101d470f8 = 1;
    }
}
