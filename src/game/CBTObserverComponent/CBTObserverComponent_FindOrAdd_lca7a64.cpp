// CBTObserverComponent unit: the entry a numeric id names in the manager's arrays at +0x128, or a
// new one appended for it (fn_7100ca7b08). Names and layouts are placeholders.
#include "types.h"

struct UnkLinkList {
    u8 data[0x80];
};

struct UnkObserverArrays_7a64 {
    UnkLinkList* values;
    u32* ids;
    u32 count;

    UnkLinkList* End() { return &values[count]; }
    UnkLinkList* Find(u32 id) {
        for (u64 i = 0; i < count; ++i)
            if (ids[i] == id)
                return &values[i];
        return End();
    }
};

struct UnkBTObserverEntry;
struct UnkBTObserverEntries;
UnkBTObserverEntry* CBTObserverComponent_AppendEntryForKey(UnkBTObserverEntries* self, const u32* key);

struct UnkObserverManager_7a64 {
    u8 pad_0x0[0x128];
    UnkObserverArrays_7a64 entries;  // 0x128
};

UnkLinkList* fn_7100ca7a64(void* manager, u32 id) {
    UnkObserverManager_7a64* self = static_cast<UnkObserverManager_7a64*>(manager);
    UnkObserverArrays_7a64* entries = &self->entries;
    UnkLinkList* slot = entries->Find(id);
    if (slot == entries->End())
        slot = reinterpret_cast<UnkLinkList*>(
            CBTObserverComponent_AppendEntryForKey(reinterpret_cast<UnkBTObserverEntries*>(entries), &id));
    return slot;
}
