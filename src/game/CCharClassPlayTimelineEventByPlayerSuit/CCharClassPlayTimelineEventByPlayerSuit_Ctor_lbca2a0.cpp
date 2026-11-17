// CCharClassPlayTimelineEventByPlayerSuit's unit (tools/tu_map.py): its value-initialising constructor (the
// object cleared first, as `new (p) T()` does), on the event base (fn_7100067ea4): the vtable and four string
// ids made the pool's empty entry (the pool created first when there is none). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101aa1708[];

struct UnkPlayTimelineEventByPlayerSuit_lbca2a0 {
    void* vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    CStrId id_0x18;
    CStrId id_0x20;
    CStrId id_0x28;
    u64 field_0x30;
};

inline void MakeEmpty_lbca2a0(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100bca2a0(UnkPlayTimelineEventByPlayerSuit_lbca2a0* self) {
    __builtin_memset(static_cast<void*>(self), 0, sizeof(*self));
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa1708 + 0x10;
    MakeEmpty_lbca2a0(&self->id_0x10);
    MakeEmpty_lbca2a0(&self->id_0x18);
    MakeEmpty_lbca2a0(&self->id_0x20);
    MakeEmpty_lbca2a0(&self->id_0x28);
}
