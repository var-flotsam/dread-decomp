// Binds an actor and caches values from its component (0x240 -> 0x1610): two computed ids,
// several parameters, three values from its transform, the table entry for its key in a
// lazily created manager, and two derived pointers.
#include "types.h"

void* fn_71000839a0(u64 size, u32 flags);

struct UnkEntry_7100286798 {
    u8 unk_0x0[0x28];
};

struct UnkTable_7100286798 {
    u8 unk_0x0[0x338];
    UnkEntry_7100286798* values;
    void** keys;
    u32 count;

    UnkEntry_7100286798* Find(void* const& key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }
    UnkEntry_7100286798* Get(void* const& key) {
        UnkEntry_7100286798* it = Find(key);
        return it == &values[count] ? nullptr : it;
    }
};

struct UnkManager_7100286798 {
    UnkTable_7100286798* table;
    u8 unk_0x8[0x62ec0];
};
void fn_7100ebf278(UnkManager_7100286798* self);
extern UnkManager_7100286798* g_unk_71726a4510;

static inline UnkManager_7100286798* GetManager() {
    if (!g_unk_71726a4510) {
        UnkManager_7100286798* m = static_cast<UnkManager_7100286798*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

struct UnkSource_7100286798 {
    u8 unk_0x0[0xec];
    u32 field_0xec;
    u32 field_0xf0;
    u8 unk_0xf4[0xd4];
    void* key;  // 0x1c8
    u64 field_0x1d0;
    u8 unk_0x1d8[0x20];
    u32 field_0x1f8;
    u8 unk_0x1fc[0xc];
    u32 field_0x208;
    u8 unk_0x20c[0xc];
    u32 field_0x218;
    u8 unk_0x21c[0xc];
    u32 field_0x228;
    u8 unk_0x22c[0xc];
    u32 field_0x238;
};
u32 fn_7100413fd0(UnkSource_7100286798* self);
u32 fn_7100413fd8(UnkSource_7100286798* self);

struct UnkComponent_7100286798 {
    u8 unk_0x0[0x1610];
    UnkSource_7100286798* source;
};
void* fn_71010fb398(UnkComponent_7100286798* self);
void* fn_71010fb288(UnkComponent_7100286798* self);

struct UnkTransform_7100286798 {
    u32 field_0x0;
    u32 field_0x4;
    u8 unk_0x8[0xc];
    u32 field_0x14;
    u8 unk_0x18[0xc];
    u32 field_0x24;
};
struct UnkTransformSrc_7100286798;
UnkTransform_7100286798* fn_710002614c(UnkTransformSrc_7100286798* self);

struct UnkActor_7100286798 {
    u8 unk_0x0[0x38];
    u8 field_0x38;  // the transform source
    u8 unk_0x39[0x207];
    UnkComponent_7100286798* component;  // 0x240
};

struct UnkStruct_7100286798 {
    u8 unk_0x0[0x50];
    UnkActor_7100286798* actor;
    u8 unk_0x58[0x8];
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u8 unk_0x70[0xe4];
    u32 field_0x154;
    u32 field_0x158;
    u32 field_0x15c;
    u32 field_0x160;
    u32 field_0x164;
    u32 field_0x168;
    u32 field_0x16c;
    u32 field_0x170;
    u8 unk_0x174[4];
    UnkEntry_7100286798* field_0x178;
    u64 field_0x180;
    u8 unk_0x188[8];
    void* field_0x190;
    void* field_0x198;
};

void fn_7100286798(UnkStruct_7100286798* self, UnkActor_7100286798* actor) {
    self->actor = actor;
    if (!actor || !actor->component)
        return;
    UnkSource_7100286798* src = actor->component->source;
    if (!src)
        return;
    self->field_0x60 = fn_7100413fd0(src);
    self->field_0x64 = fn_7100413fd8(src);
    self->field_0x68 = src->field_0x218;
    self->field_0x6c = src->field_0x228;
    self->field_0x154 = src->field_0x208;
    self->field_0x158 = src->field_0x1f8;
    self->field_0x15c = src->field_0x238;
    self->field_0x160 = src->field_0xec;
    self->field_0x164 = src->field_0xf0;
    UnkTransform_7100286798* t =
        fn_710002614c(reinterpret_cast<UnkTransformSrc_7100286798*>(&self->actor->field_0x38));
    u32 a = t->field_0x4;
    u32 b = t->field_0x14;
    u32 c = t->field_0x24;
    self->field_0x168 = a;
    self->field_0x16c = b;
    self->field_0x170 = c;
    UnkTable_7100286798* table = GetManager()->table;
    self->field_0x178 = table ? table->Get(src->key) : nullptr;
    self->field_0x180 = src->field_0x1d0;
    self->field_0x190 = fn_71010fb398(self->actor->component);
    self->field_0x198 = fn_71010fb288(self->actor->component);
}
