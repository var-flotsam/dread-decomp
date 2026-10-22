// Refreshes a component: runs the base refresh, registers it, applies the entry its id names in
// the manager's table (when it has one), then rebuilds its flag set: 26 ids set, four cleared.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"

using base::global::CStrId;
using base::global::UnkStringPool;

// The set's key; the old id is released in two steps (the original reloads the id and its count
// after the decrement).
struct UnkFlagKey_71011415f4 {
    CStrId id;

    void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id.data && id.data != &pool->empty) {
                --id.data->ref_count;
                if (id.data->ref_count == 0) {
                    fn_71000803a4(pool, id.data);
                    pool = g_unk_7101d47108;
                }
            }
            id.data = &pool->empty;
        }
    }
};

// Flags keyed by id (parallel arrays). The flags are bytes: the original reloads the count after
// storing one.
struct UnkFlagSet_71011415f4 {
    u8* values;
    UnkFlagKey_71011415f4* keys;
    u32 count;
    u32 capacity;
    u32 grow;

    void Reserve(u32 n);  // fn_71007531f8

    void Clear() {
        if (keys) {
            for (u64 i = 0; i < count; ++i)
                keys[i].Release();
        }
        count = 0;
    }

    void Add(const CStrId& key, bool value) {
        if (count == capacity)
            Reserve(count + grow);
        // The id is copied as raw bytes, then retained: the original reloads the count after the
        // copy on both paths.
        CStrId* k = &keys[count].id;
        __builtin_memcpy(k, &key, sizeof(*k));
        if (&g_unk_7101d47108->empty != k->data)
            ++k->data->ref_count;
        values[count] = value;
        ++count;
    }
};

struct UnkTable_71011415f4;
struct UnkEntry_71011415f4 {
    u8 unk_0x0[0x18];
    u8 field_0x18[0x8];
};
bool fn_7101047208(UnkTable_71011415f4* table, const CStrId* id);
UnkEntry_71011415f4* fn_71010472e0(UnkTable_71011415f4* table, const CStrId* id);

struct UnkMgr_71011415f4 {
    u8 unk_0x0[0x968];
};

struct UnkComponent_71011415f4 {
    u8 unk_0x0[0x88];
    CStrId id;
    UnkFlagSet_71011415f4 flags;

    void Refresh();                // fn_71010c4228
    void Apply(void* entry_data);  // fn_710114433c
};

extern UnkMgr_71011415f4* g_unk_7101d47310;
extern const CStrId g_unk_7101d470e0;
extern char g_unk_7172a092b0[] __attribute__((visibility("hidden")));
void fn_7100de9540(UnkMgr_71011415f4* mgr, void* kind, UnkComponent_71011415f4* component);

extern CStrId g_unk_7172a092b8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092c0 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092c8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092d0 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092d8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092e0 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092e8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092f0 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a092f8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09300 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09308 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09310 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09318 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09320 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09328 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09330 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09338 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09340 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09348 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09350 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09358 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09360 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09368 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09370 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09378 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a09380 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a093a8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a093b0 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a093b8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a093c0 __attribute__((visibility("hidden")));

static inline UnkTable_71011415f4* Table() {
    return reinterpret_cast<UnkTable_71011415f4*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x968);
}

void fn_71011415f4(UnkComponent_71011415f4* self) {
    self->Refresh();
    fn_7100de9540(g_unk_7101d47310, g_unk_7172a092b0, self);
    if (self->id.data != g_unk_7101d470e0.data && fn_7101047208(Table(), &self->id))
        self->Apply(fn_71010472e0(Table(), &self->id)->field_0x18);
    self->flags.Clear();
    self->flags.Add(g_unk_7172a092b8, true);
    self->flags.Add(g_unk_7172a092c0, true);
    self->flags.Add(g_unk_7172a092c8, true);
    self->flags.Add(g_unk_7172a092d0, true);
    self->flags.Add(g_unk_7172a092d8, true);
    self->flags.Add(g_unk_7172a092e0, true);
    self->flags.Add(g_unk_7172a092e8, true);
    self->flags.Add(g_unk_7172a092f0, true);
    self->flags.Add(g_unk_7172a092f8, true);
    self->flags.Add(g_unk_7172a09300, true);
    self->flags.Add(g_unk_7172a09308, true);
    self->flags.Add(g_unk_7172a09310, true);
    self->flags.Add(g_unk_7172a09318, true);
    self->flags.Add(g_unk_7172a09320, true);
    self->flags.Add(g_unk_7172a09328, true);
    self->flags.Add(g_unk_7172a09330, true);
    self->flags.Add(g_unk_7172a09338, true);
    self->flags.Add(g_unk_7172a09340, true);
    self->flags.Add(g_unk_7172a09348, true);
    self->flags.Add(g_unk_7172a09350, true);
    self->flags.Add(g_unk_7172a09358, true);
    self->flags.Add(g_unk_7172a09360, true);
    self->flags.Add(g_unk_7172a09368, true);
    self->flags.Add(g_unk_7172a09370, true);
    self->flags.Add(g_unk_7172a09378, true);
    self->flags.Add(g_unk_7172a09380, true);
    self->flags.Add(g_unk_7172a093a8, false);
    self->flags.Add(g_unk_7172a093b0, false);
    self->flags.Add(g_unk_7172a093b8, false);
    self->flags.Add(g_unk_7172a093c0, false);
}
