// CCharClassDropItemOverwriteProbabilitiesEvent's unit (tools/tu_map.py unit 2236): fn_7100cb7f90 builds the
// drop spawn settings from the event as the crazy-drop event does
// (src/game/CCharClassDropItemCrazyEvent/CCharClassDropItemCrazyEvent_Params_ovb0136.cpp), with the event's
// own kind (+0x84); fn_7100cb84ec is the event's copy assignment (plain blocks, three string ids taken with
// their counts adjusted, the record at +0x90 through fn_71000ce19c); fn_7100cb7b80 the destructor (the
// vtable, the id list at +0x90 and the ids at +0x88, +0x78 and +0x58 released, then the base destructor
// fn_7100067ed4, as src/game/CCharClassFXCreateTrailEvent/CCharClassFXCreateTrailEvent_Copy_ovb0121.cpp).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_71007c7ddc(void*);
u32 fn_7100cb51a0(const char* name);

// A string id assigned like CStrId; the old one is released in two steps (the original reloads
// `data` and the count after the decrement).
struct UnkDropItemName_ovb0136 {
    UnkStringData* data;
    UnkDropItemName_ovb0136& operator=(const UnkDropItemName_ovb0136& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkDropItemVec3_ovb0136 {
    f32 x, y, z;
    UnkDropItemVec3_ovb0136& operator=(const UnkDropItemVec3_ovb0136& other) {
        u64 xy;
        u32 w;
        __builtin_memcpy(&xy, &other, 8);
        __builtin_memcpy(&w, &other.z, 4);
        __builtin_memcpy(this, &xy, 8);
        __builtin_memcpy(&z, &w, 4);
        return *this;
    }
};

// The drop spawn settings (0x90 bytes; as CDropComponent passes them to its spawn), built by
// fn_71007c7ddc.
struct UnkDropItemParams_ovb0136 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10[7];
    u8 pad_0x2c[8];
    u32 field_0x34[4];
    u8 pad_0x44[0xc];
    u32 field_0x50[3];
    u8 pad_0x5c[4];
    UnkDropItemName_ovb0136 name;      // 0x60
    UnkDropItemVec3_ovb0136 field_0x68;
    UnkDropItemVec3_ovb0136 field_0x74;
    u32 item;                           // 0x80
    u32 kind;                           // 0x84
    u8 field_0x88;
    u8 field_0x89;
    u8 field_0x8a;

    UnkDropItemParams_ovb0136() { fn_71007c7ddc(this); }
};

// An item definition: a counted object with its name at +0x10.
struct UnkDropItemDef_ovb0136 {
    u8 pad_0x0[0x10];
    const char* name;
    u8 pad_0x18[0x2c];
    // A union member: the definition pointer is reloaded after the increment.
    union {
        u32 ref_count;  // 0x44
    };
};

static inline u32 ItemId(UnkDropItemDef_ovb0136* const& def) {
    if ((u64)def != (u64)&g_unk_7101d47108->empty) {
        ++def->ref_count;
        const char* name = def->name;
        return fn_7100cb51a0(name ? name : g_unk_710162e2ed);
    }
    return 0;
}

struct UnkDropItemOverwriteEvent_ovb0136 {
    u8 pad_0x0[0xc];
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 pad_0x18[4];
    u32 field_0x1c[7];
    u32 field_0x38[4];
    u32 field_0x48[3];
    u8 pad_0x54[4];
    UnkDropItemName_ovb0136 name;  // 0x58
    UnkDropItemVec3_ovb0136 field_0x60;
    UnkDropItemVec3_ovb0136 field_0x6c;
    UnkDropItemDef_ovb0136* def;  // 0x78
    u8 field_0x80;
    u8 field_0x81;
    u8 field_0x82;
    u8 pad_0x83;
    u32 kind;  // 0x84
};

struct UnkDropSpawnRequest_ovb0096;

UnkDropItemParams_ovb0136 fn_7100cb7f90(UnkDropSpawnRequest_ovb0096* request) {
    auto* event = reinterpret_cast<UnkDropItemOverwriteEvent_ovb0136*>(request);
    UnkDropItemParams_ovb0136 params;
    params.field_0x0 = event->field_0xc;
    params.field_0x4 = event->field_0x10;
    params.field_0x8 = event->field_0x14;
    params.field_0xc = event->field_0x14;
    params.field_0x10[0] = event->field_0x1c[0];
    params.field_0x10[1] = event->field_0x1c[1];
    params.field_0x10[2] = event->field_0x1c[2];
    params.field_0x10[3] = event->field_0x1c[3];
    params.field_0x10[4] = event->field_0x1c[4];
    params.field_0x10[5] = event->field_0x1c[5];
    params.field_0x10[6] = event->field_0x1c[6];
    params.field_0x34[0] = event->field_0x38[0];
    params.field_0x34[1] = event->field_0x38[1];
    params.field_0x34[2] = event->field_0x38[2];
    params.field_0x34[3] = event->field_0x38[3];
    params.field_0x50[0] = event->field_0x48[0];
    params.field_0x50[1] = event->field_0x48[1];
    params.field_0x50[2] = event->field_0x48[2];
    params.name = event->name;
    params.field_0x68 = event->field_0x60;
    params.field_0x74 = event->field_0x6c;
    params.item = ItemId(event->def);
    params.kind = event->kind;
    params.field_0x88 = event->field_0x80;
    params.field_0x89 = event->field_0x81;
    params.field_0x8a = event->field_0x82;
    return params;
}

struct UnkStruct_0x28;
void fn_71000ce19c(UnkStruct_0x28*, const UnkStruct_0x28*);

struct UnkCopiedId_ovb0136 {
    UnkStringData* data;

    __attribute__((always_inline)) UnkCopiedId_ovb0136& operator=(const UnkCopiedId_ovb0136& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkOverwriteBlock8_ovb0136 {
    u32 words[2];
};

struct UnkOverwriteBlock64_ovb0136 {
    u32 words[16];
};

struct UnkOverwriteBlock24_ovb0136 {
    u64 words[3];
};

struct UnkOverwriteEvent_ovb0136 {
    void* vtable;
    u32 field_0x8;
    UnkOverwriteBlock8_ovb0136 field_0xc;
    UnkOverwriteBlock64_ovb0136 field_0x14;
    u8 pad_0x54[4];
    UnkCopiedId_ovb0136 field_0x58;
    UnkOverwriteBlock24_ovb0136 field_0x60;
    UnkCopiedId_ovb0136 field_0x78;
    u64 field_0x80;
    UnkCopiedId_ovb0136 field_0x88;
    u8 field_0x90[0x28];
};

UnkOverwriteEvent_ovb0136* fn_7100cb84ec(UnkOverwriteEvent_ovb0136* self, const UnkOverwriteEvent_ovb0136* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0xc = other->field_0xc;
    self->field_0x14 = other->field_0x14;
    self->field_0x58 = other->field_0x58;
    self->field_0x60 = other->field_0x60;
    self->field_0x78 = other->field_0x78;
    self->field_0x80 = other->field_0x80;
    self->field_0x88 = other->field_0x88;
    fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(self->field_0x90),
                  reinterpret_cast<const UnkStruct_0x28*>(other->field_0x90));
    return self;
}

struct UnkReleasedId_ovb0136 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedId_ovb0136() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

void fn_7100067ed4(void* self);  // the base's destructor

extern char g_unk_7101aa5860[];

struct UnkOverwriteEventDtor_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkReleasedId_ovb0136 field_0x58;
    u8 pad_0x60[0x18];
    UnkReleasedId_ovb0136 field_0x78;
    u8 pad_0x80[0x8];
    UnkReleasedId_ovb0136 field_0x88;
    base::UnkVector<UnkReleasedId_ovb0136> field_0x90;

    __attribute__((always_inline)) ~UnkOverwriteEventDtor_ovb0136() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa5860 + 0x10);
        field_0x90.Free();
    }
};

void fn_7100cb7b80(UnkOverwriteEventDtor_ovb0136* self) {
    self->~UnkOverwriteEventDtor_ovb0136();
    fn_7100067ed4(self);
}
