// CCooldownXBossLavaCarpetAttack (tools/tu_map.py): the attack's destructors (fn_71004c55e0, and
// fn_71004c5678 which also frees it: the vtable, the record array at +0x1d8 (its count cleared), the
// member at +0xa0 (fn_7100d62bd8), the listener at +0x70 (game_5821e4/CAttackDtor.h), then CAttack's
// destructor), its copy constructor (fn_71004c5820: its two bases' parts, each storing its vtable, the
// plain fields, the listener at +0x70 made to refer to the other's weak handle at +0x88, the member at
// +0xa0 (fn_7100d62b68), the records at +0x1d8 and the 11 bytes at +0x77c), the assignment of its
// record array (fn_71004c5c2c; the records, 0x30 bytes from 4 bytes in, hold a flag, two triples and
// 20 plain bytes), and adding the outline from one point to another (each flattened to its x and y, at
// z 0) to the outline set at +0xa0 (fn_71004c51e4; the outline's layout is CBreakableTileGroup's, in
// src/game/CBreakableTileGroup.cpp). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_5821e4/CAttackDtor.h"
#include "types.h"

extern void* g_unk_7101a3e5b8[];  // the attack's vtable
void fn_7100d62bd8(void* p);

struct UnkLavaCarpetAttack_ovb0059 {
    void** vtable;
    u8 pad_0x8[0x68];
    UnkAttackListenerRaw listener;
    u8 pad_0x90[0x10];
    u8 field_0xa0[0x138];
    u32 count_0x1d8;

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a3e5b8 + 2;
        count_0x1d8 = 0;
        fn_7100d62bd8(field_0xa0);
        listener.Destroy();
        fn_71003606c0(this);
    }
};
static_assert(__builtin_offsetof(UnkLavaCarpetAttack_ovb0059, count_0x1d8) == 0x1d8, "layout");

void fn_71004c55e0(UnkLavaCarpetAttack_ovb0059* self) {
    self->Destroy();
}

void fn_71004c5678(UnkLavaCarpetAttack_ovb0059* self) {
    self->Destroy();
    operator delete(self);
}

extern void* g_unk_7101a24060[];  // the first base's vtable
extern void* g_unk_7101a3e3a8[];  // the second base's vtable
extern void* g_unk_7101a3e5b8[];  // the attack's vtable

void fn_7100d62b68(char*, const char*);

struct UnkLavaVec3_ovb0059 {
    u32 x, y, z;
    UnkLavaVec3_ovb0059(const UnkLavaVec3_ovb0059& o) : x(o.x), y(o.y), z(o.z) {}
    UnkLavaVec3_ovb0059& operator=(const UnkLavaVec3_ovb0059& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkLavaTail_ovb0059 {
    u32 words[5];
};

struct UnkLavaRecord_ovb0059 {
    bool flag;
    UnkLavaVec3_ovb0059 a;
    UnkLavaVec3_ovb0059 b;
    UnkLavaTail_ovb0059 tail;
};
static_assert(sizeof(UnkLavaRecord_ovb0059) == 0x30, "record size");

struct UnkLavaRecords_ovb0059 {
    u32 count;
    UnkLavaRecord_ovb0059 data[30];

    // The records as raw slots, 4 bytes in.
    UnkLavaRecord_ovb0059* Slots() {
        return reinterpret_cast<UnkLavaRecord_ovb0059*>(reinterpret_cast<u8*>(this) + 4);
    }
};

struct UnkLavaListener_ovb0059 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkWeakHandle* handle;  // 0x18
};

struct UnkLavaAttack_ovb0059 {
    void** vtable;
    u8 field_0x8[32];
    u64 field_0x28;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u8 field_0x58[9];
    u8 pad_0x61[7];
    u64 field_0x68;
    UnkLavaListener_ovb0059 listener;  // 0x70
    u8 field_0x90[12];
    u8 pad_0x9c[4];
    u8 field_0xa0[0x120];
    u8 field_0x1c0[24];
    UnkLavaRecords_ovb0059 records;  // 0x1d8
    u8 pad_0x77c_[0x77c - 0x1d8 - sizeof(UnkLavaRecords_ovb0059)];
    u8 field_0x77c[11];
};
static_assert(__builtin_offsetof(UnkLavaAttack_ovb0059, records) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkLavaAttack_ovb0059, field_0x77c) == 0x77c, "layout");

// Declared with untyped parameters in src/game/CCooldownXBossLavaCarpetAttack/fn_71004c5a14.cpp.
void fn_71004c5c2c(void* self_, const void* other_) {
    UnkLavaRecords_ovb0059* self = static_cast<UnkLavaRecords_ovb0059*>(self_);
    const UnkLavaRecords_ovb0059* other = static_cast<const UnkLavaRecords_ovb0059*>(other_);
    if (self == other)
        return;
    u32 n = self->count;
    if (n > other->count) {
        for (u64 i = 0; i < other->count; ++i)
            self->data[i] = other->data[i];
    } else {
        for (u64 i = 0; i < n; ++i)
            self->data[i] = other->data[i];
    }
    for (u64 i = n; i < other->count; ++i) {
        new (self->Slots() + self->count) UnkLavaRecord_ovb0059(other->data[i]);
        ++self->count;
    }
    self->count = other->count;
}

void fn_71004c5820(UnkLavaAttack_ovb0059* self, const UnkLavaAttack_ovb0059* other) {
    self->vtable = &g_unk_7101a24060[2];
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    self->vtable = &g_unk_7101a3e3a8[2];
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x34 = other->field_0x34;
    self->field_0x38 = other->field_0x38;
    self->field_0x3c = other->field_0x3c;
    self->field_0x40 = other->field_0x40;
    self->field_0x44 = other->field_0x44;
    self->field_0x48 = other->field_0x48;
    self->field_0x4c = other->field_0x4c;
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    __builtin_memcpy(self->field_0x58, other->field_0x58, 9);
    self->vtable = &g_unk_7101a3e5b8[2];
    self->field_0x68 = other->field_0x68;
    self->listener.vtable = &g_unk_7101a22d48[2];
    self->listener.field_0x8 = other->listener.field_0x8;
    self->listener.field_0x10 = other->listener.field_0x10;
    self->listener.handle = nullptr;
    AssignRef(&self->listener.handle, other->listener.handle);
    __builtin_memcpy(self->field_0x90, other->field_0x90, 12);
    fn_7100d62b68(reinterpret_cast<char*>(self->field_0xa0), reinterpret_cast<const char*>(other->field_0xa0));
    __builtin_memcpy(self->field_0x1c0, other->field_0x1c0, 24);
    u64 i = 0;
    for (; i < other->records.count; ++i)
        new (&self->records.data[i]) UnkLavaRecord_ovb0059(other->records.data[i]);
    self->records.count = other->records.count;
    __builtin_memcpy(self->field_0x77c, other->field_0x77c, 11);
}

struct UnkVec3;
struct UnkTileOutlines;
struct UnkTileOutline;
void* fn_7100118b94(void* sink, UnkVec3* point);
void fn_7100d62d40(UnkTileOutlines* self, UnkTileOutline* outline);

struct UnkLavaPoint_ovb0059 {
    f32 x, y, z;
};

struct UnkLavaOutline_ovb0059 {
    base::UnkVector<UnkLavaPoint_ovb0059> points;
    f32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    base::math::CVector2D min;
    base::math::CVector2D max;
    u32 field_0x40;

    UnkLavaOutline_ovb0059()
        : points(7), field_0x28(0.0f), field_0x2c(false), field_0x2d(true), min{3.4028235e38f, 3.4028235e38f},
          max{-3.4028235e38f, -3.4028235e38f}, field_0x40(0) {}
    ~UnkLavaOutline_ovb0059() { points.Free(); }
};
static_assert(sizeof(UnkLavaOutline_ovb0059) == 0x48, "outline size");

struct UnkLavaAttackOutlines_ovb0059 {
    u8 pad_0x0[0xa0];
    u8 outlines[0x120];  // 0xa0
};

void fn_71004c51e4(UnkLavaAttackOutlines_ovb0059* self, const base::math::CVector3D* from,
                   const base::math::CVector3D* to) {
    UnkLavaOutline_ovb0059 outline;
    base::math::CVector2D flat_from;
    fn_710007b430(&flat_from, from);
    UnkLavaPoint_ovb0059 point_from = {flat_from.x, flat_from.y, 0.0f};
    fn_7100118b94(&outline, reinterpret_cast<UnkVec3*>(&point_from));
    base::math::CVector2D flat_to;
    fn_710007b430(&flat_to, to);
    UnkLavaPoint_ovb0059 point_to = {flat_to.x, flat_to.y, 0.0f};
    fn_7100118b94(&outline, reinterpret_cast<UnkVec3*>(&point_to));
    fn_7100d62d40(reinterpret_cast<UnkTileOutlines*>(self->outlines), reinterpret_cast<UnkTileOutline*>(&outline));
}
