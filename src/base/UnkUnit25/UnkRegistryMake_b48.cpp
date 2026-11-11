// Unit 25 (tools/tu_map.py): makes an object for a key through the resource registry
// (g_unk_7101d4bbc0): the factory entry's callback (+0x40, with its context at +0x38) when the key is
// registered, else an empty object (fn_710008d2a0). Names and layouts are placeholders.
#include "base/file/CThreadSafeDevice.h"
#include "game/UnkTU25.h"
#include "game/UnkTU27.h"
#include "types.h"

struct UnkRegistry_7100d92158;
extern UnkRegistry_7100d92158* g_unk_7101d4bbc0;

// The id and path pair it returns (as src/base/curves/CCurveResource_Load_b45.cpp names it).
struct UnkPathPairTemp_b45a1;

struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    UnkPathPairTemp_b45a1() { fn_710008d2a0(this); }
    UnkPathPairTemp_b45a1(const UnkPathPairTemp_b45a1& o);
};

struct UnkRegistryKey;

struct UnkFactoryEntry {
    u8 pad_0x0[0x38];
    void* context;                                             // 0x38
    UnkPathPairTemp_b45a1 (*make)(void* context, const UnkRegistryKey* key);  // 0x40
};

UnkPathPairTemp_b45a1 fn_710001ab24(void* path) {
    const UnkRegistryKey* key = static_cast<const UnkRegistryKey*>(path);
    UnkFactoryEntry* e = fn_7100020320(g_unk_7101d4bbc0, key);
    if (e != nullptr)
        return e->make(e->context, key);
    return UnkPathPairTemp_b45a1();
}
