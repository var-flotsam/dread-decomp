// GUI::CHUD (unit 2751): builds, on first use, the list of ids the Aeion gauge reacts to
// (ITEM_OPTIC_CAMOUFLAGE, ITEM_GHOST_AURA, ITEM_SONAR) and registers its release at exit.
// Names and layouts are placeholders.
#include <initializer_list>

#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// A string id with a value (base::global::CRntVector's element in fn_71011df420).
struct UnkHudIdValue_ovb0017 {
    CStrId id;
    u64 value;

    // The value is copied bitwise: the original reloads the array's count and the string pool
    // after storing it.
    UnkHudIdValue_ovb0017(const UnkHudIdValue_ovb0017& other) : id(other.id) {
        __builtin_memcpy(&value, &other.value, sizeof(value));
    }
};

using UnkHudIdValueVector_ovb0017 = base::global::CRntVector<UnkHudIdValue_ovb0017>;

void fn_71011df420(UnkHudIdValueVector_ovb0017* self, std::initializer_list<UnkHudIdValue_ovb0017> items,
                   u32 step, base::IAllocator* allocator, u32 flags);

struct UnkIdWordPair;
void fn_71011df574(base::global::CRntVector<UnkIdWordPair>* self);

// The ids the Aeion gauge reacts to, built by fn_71011df1b0 on first use.
struct UnkAeionIdEntry_ovb0017 {
    UnkStringData* id;
    u64 field_0x8;
};
struct UnkAeionIdList_ovb0017 {
    UnkAeionIdEntry_ovb0017* data;
    u32 count;
};
extern __attribute__((visibility("hidden"))) UnkAeionIdList_ovb0017 g_unk_7172a10040;
__attribute__((visibility("hidden"))) extern u64 g_unk_7172a10018;  // its guard

// The item's qualified id (built on first use), read as a list element: the id and its source.
static inline const UnkHudIdValue_ovb0017& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return reinterpret_cast<const UnkHudIdValue_ovb0017&>(qid);
}

UnkHudIdValueVector_ovb0017* fn_71011df1b0() {
    UnkHudIdValueVector_ovb0017* ids = reinterpret_cast<UnkHudIdValueVector_ovb0017*>(&g_unk_7172a10040);
    u64* guard = &g_unk_7172a10018;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(guard), __ATOMIC_ACQUIRE) & 1), 0) &&
        __cxa_guard_acquire(guard)) {
        UnkPropertyEx<>& camouflage = g_unk_717275aa58;
        UnkPropertyEx<>& ghost_aura = g_unk_717275aad8;
        UnkPropertyEx<>& sonar = g_unk_717275ab58;
        fn_71011df420(ids, {ItemId(camouflage), ItemId(ghost_aura), ItemId(sonar)}, 10, nullptr, 0xf);
        fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_71011df574), ids, g_unk_7101a19000);
        __cxa_guard_release(guard);
    }
    return ids;
}
