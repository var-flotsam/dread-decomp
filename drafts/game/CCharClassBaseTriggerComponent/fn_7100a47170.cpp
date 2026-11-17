// CCharClassBaseTriggerComponent's unit (tools/tu_map.py unit 1562): an entry leaves the trigger. When
// its actor is still there, the kept player name (+0x9b0) is cleared if it is the player, and the
// trigger is told through its virtual 0x1a0; the entry is then erased from the list (+0xa8) by moving
// the last one into its place. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkTriggerInfo_la47170 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;

    UnkTriggerInfo_la47170(const UnkTriggerInfo_la47170& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8) {}
};

// A counted reference to a weak handle.
struct UnkHandleRef_la47170 {
    UnkWeakHandle* handle;

    UnkHandleRef_la47170(const UnkHandleRef_la47170& other) : handle(nullptr) {
        AssignRef(&handle, other.handle);
    }
    ~UnkHandleRef_la47170() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

struct UnkTriggerEntry_la47170 {
    UnkHandleRef_la47170 ref;
    UnkTriggerInfo_la47170 info;
};

// The entries' copy-assignment, out of line.
void fn_7100a495e4(UnkTriggerEntry_la47170* self, const UnkTriggerEntry_la47170* other);

struct UnkTriggerComponent_la47170;

struct UnkTriggerComponentVtbl_la47170 {
    void* pad_0x0[0x1a0 / 8];
    void (*vf_0x1a0)(UnkTriggerComponent_la47170* self, UnkWeakTarget* actor, UnkTriggerInfo_la47170* info);
};

// The entries: a count and room for 32 of them.
struct UnkTriggerEntryList_la47170 {
    u32 count;
    u8 storage[32 * sizeof(UnkTriggerEntry_la47170)];

    UnkTriggerEntry_la47170* items() { return reinterpret_cast<UnkTriggerEntry_la47170*>(storage); }
    UnkTriggerEntry_la47170* end() { return &items()[count]; }

    void Resize(u32 n) {
        if (n > count) {
            __builtin_memset(&items()[count], 0, (n - count) * sizeof(UnkTriggerEntry_la47170));
        } else {
            for (u32 i = n; i < count; ++i)
                items()[i].~UnkTriggerEntry_la47170();
        }
        count = n;
    }

    // Moves the last entry into `pos` and drops the last; `pos` then holds the next entry.
    UnkTriggerEntry_la47170* Erase(UnkTriggerEntry_la47170* pos) {
        UnkTriggerEntry_la47170* last = end() - 1;
        if (pos != last)
            fn_7100a495e4(pos, last);
        Resize(count - 1);
        return pos;
    }
};

struct UnkTriggerComponent_la47170 {
    UnkTriggerComponentVtbl_la47170* vtable;
    u8 pad_0x8[0xa0];
    UnkTriggerEntryList_la47170 entries;  // 0xa8
    u8 pad_0x3ac[0x9b0 - 0x3ac];
    CStrId player_name;  // 0x9b0

    void vf_0x1a0(UnkWeakTarget* actor, UnkTriggerInfo_la47170* info) { vtable->vf_0x1a0(this, actor, info); }

    // The player's name is no longer kept once it leaves.
    void OnExit(UnkWeakTarget* actor, UnkTriggerInfo_la47170* info) {
        if (fn_7100dec788(g_unk_7101d47310) == reinterpret_cast<UnkPlayer*>(actor))
            player_name = g_unk_7101d470e0;
        vf_0x1a0(actor, info);
    }
};
static_assert(__builtin_offsetof(UnkTriggerComponent_la47170, player_name) == 0x9b0, "layout");

void fn_7100a47170(UnkTriggerComponent_la47170* self, UnkTriggerEntry_la47170** it) {
    UnkTriggerEntry_la47170 entry = **it;
    if (entry.ref.handle) {
        if (UnkWeakTarget* actor = entry.ref.handle->target)
            self->OnExit(actor, &entry.info);
    }
    *it = self->entries.Erase(*it);
}
