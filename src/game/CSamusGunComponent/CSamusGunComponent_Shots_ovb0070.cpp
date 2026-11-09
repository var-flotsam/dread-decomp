// CSamusGunComponent unit (unit 1315 of tools/tu_map.py): the array of 0x18-byte entries at +0x348
// (a counted reference, a word and a u32). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// An entry: moved by taking a new reference and dropping the old one.
struct UnkGunShot_ovb0070 {
    union {
        UnkRefObject* ref;
    };
    u64 field_0x8;
    u32 field_0x10;

    UnkGunShot_ovb0070(UnkGunShot_ovb0070&& other) {
        ref = nullptr;
        AssignRef(&ref, other.ref);
        u64 a = other.field_0x8;
        u32 b = other.field_0x10;
        field_0x10 = b;
        field_0x8 = a;
    }
    ~UnkGunShot_ovb0070() {
        if (UnkRefObject* p = ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
static_assert(sizeof(UnkGunShot_ovb0070) == 0x18, "UnkGunShot_ovb0070 layout");

struct UnkGunShots_ovb0070 {
    UnkGunShot_ovb0070* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows the array to at least n entries, moving them over.
void fn_710085c338(UnkGunShots_ovb0070* shots, u32 n) {
    if (shots->capacity >= n)
        return;
    auto* p = static_cast<UnkGunShot_ovb0070*>(shots->allocator->vf_0x40((u64)n * sizeof(UnkGunShot_ovb0070), 8, shots->flags));
    if (shots->data) {
        UnkGunShot_ovb0070* src = shots->data;
        UnkGunShot_ovb0070* dst = p;
        for (u32 i = 0; i < shots->count; ++i, ++dst, ++src) {
            new (dst) UnkGunShot_ovb0070(static_cast<UnkGunShot_ovb0070&&>(*src));
            src->~UnkGunShot_ovb0070();
        }
        shots->allocator->vf_0x48(shots->data);
    }
    shots->data = p;
    shots->capacity = n;
}

// The array as the component owns it: destroyed with its entries.
struct UnkGunShotArray_ovb0070 : UnkGunShots_ovb0070 {
    // Drops every entry's reference, keeping the array.
    void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].~UnkGunShot_ovb0070();
        count = 0;
    }
    ~UnkGunShotArray_ovb0070() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkGunShot_ovb0070();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A string id released in two steps (the count is reloaded after the decrement), left empty.
struct UnkGunId_ovb0070 {
    base::global::UnkStringData* data;

    void InitEmpty() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkGunId_ovb0070() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

struct UnkGunRequest_ovb0070;
struct UnkSamusGun_ovb0070;
struct UnkActor;
void fn_7100d7c94c(UnkActor* self, const u64* key);
// Timeline ids reached through the GOT.
extern u64 g_unk_710164ad48;
extern u64 g_unk_710164ad50;
extern u64 g_unk_710164ad58;
extern u64 g_unk_710164ad60;
extern u64 g_unk_710164ad68;
bool fn_710085befc(UnkSamusGun_ovb0070* self, const UnkGunRequest_ovb0070& request);
void fn_7100838c7c(UnkSamusGun_ovb0070* self);
void fn_7100838f20(UnkSamusGun_ovb0070* self);
extern void* g_unk_7101a793d0[];

struct UnkSamusGun_ovb0070 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x18];
    UnkSamusGun_ovb0070* handler_object;  // 0x50: fn_710085befc on this component
    bool (*handler)(UnkSamusGun_ovb0070*, const UnkGunRequest_ovb0070&);
    u8 pad_0x60[0x2c8];
    UnkGunId_ovb0070 field_0x328;
    bool field_0x330;
    UnkGunId_ovb0070 field_0x338;
    bool field_0x340;
    u32 field_0x344;
    UnkGunShotArray_ovb0070 shots;  // 0x348
    u64 field_0x370;
    s64 field_0x378;
    bool field_0x380;
    u32 field_0x384;
    bool field_0x388;
    bool field_0x389;
    bool field_0x38a;
};
static_assert(__builtin_offsetof(UnkSamusGun_ovb0070, field_0x38a) == 0x38a, "layout");

// The constructor: the parent constructor fn_7100838c7c, the class vtable, two empty ids, an empty
// array, cleared fields and the request handler (fn_710085befc on this component).
void fn_7100858294(UnkSamusGun_ovb0070* self) {
    fn_7100838c7c(self);
    self->vtable = &g_unk_7101a793d0[2];
    self->field_0x328.InitEmpty();
    self->field_0x330 = false;
    self->field_0x338.InitEmpty();
    self->field_0x340 = false;
    self->shots.data = nullptr;
    self->shots.count = 0;
    self->shots.capacity = 0;
    self->shots.step = 10;
    self->shots.field_0x14 = true;
    self->shots.flags = 0xf;
    self->shots.allocator = fn_7100083990();
    self->field_0x370 = 0;
    self->field_0x378 = -1;
    self->field_0x380 = true;
    self->field_0x384 = 0;
    self->field_0x388 = false;
    self->field_0x389 = false;
    self->field_0x38a = false;
    self->handler_object = self;
    self->handler = &fn_710085befc;
}

// The destructor: the class vtable, the array, the two ids, then the parent destructor
// fn_7100838f20.
void fn_710085843c(UnkSamusGun_ovb0070* self) {
    self->vtable = &g_unk_7101a793d0[2];
    self->shots.~UnkGunShotArray_ovb0070();
    self->field_0x338.~UnkGunId_ovb0070();
    self->field_0x328.~UnkGunId_ovb0070();
    fn_7100838f20(self);
}

struct UnkGunBeamInfo_ovb0070 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkGunBeam_ovb0070 {
    UnkGunBeamInfo_ovb0070* info;
};

void fn_7100839924(UnkSamusGun_ovb0070* self);
UnkGunBeam_ovb0070* fn_7100840828(UnkSamusGun_ovb0070* self);

// A pending beam change (+0x330) removes the owner's timeline that goes with the current beam.
static inline void ApplyBeamChange(UnkSamusGun_ovb0070* self) {
    if (self->field_0x330) {
        self->field_0x330 = false;
        if (self->owner) {
            switch (fn_7100840828(self)->info->id) {
            case 0x6803e9b3d5c26ba0:
            case 0xb23e2ea9edd95fb7:
                fn_7100d7c94c(self->owner, &g_unk_710164ad48);
                break;
            case 0xa87817d815624dd3:
                fn_7100d7c94c(self->owner, &g_unk_710164ad50);
                break;
            case 0xa7b8a8ac9c26afaa:
                fn_7100d7c94c(self->owner, &g_unk_710164ad58);
                break;
            }
        }
    }
}

// A pending reset (+0x340) drops the entries and removes two more timelines.
static inline void ApplyReset(UnkSamusGun_ovb0070* self) {
    if (self->field_0x340) {
        self->field_0x340 = false;
        self->field_0x344 = 0;
        self->shots.Clear();
        if (self->owner) {
            fn_7100d7c94c(self->owner, &g_unk_710164ad68);
            fn_7100d7c94c(self->owner, &g_unk_710164ad60);
        }
    }
}

// The update: after the parent's (fn_7100839924), the pending beam change and reset; the entries are
// dropped every time.
void fn_71008585a8(UnkSamusGun_ovb0070* self) {
    fn_7100839924(self);
    ApplyBeamChange(self);
    ApplyReset(self);
    self->shots.Clear();
}

void fn_7100839b00(char*, bool);

// Another step: the parent's (fn_7100839b00 with false), then the pending reset and beam change.
void fn_710085a8a4(UnkSamusGun_ovb0070* self) {
    fn_7100839b00(reinterpret_cast<char*>(self), false);
    ApplyReset(self);
    ApplyBeamChange(self);
}
