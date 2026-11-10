// Unit 2177 (no class name): paths in the world path pool. fn_7100c66b50 replaces one path inner
// entries with another, fn_7100c77a9c tests a path (on a copy with a reset bounding box) against a
// target, and fn_7100c778f8 keeps the nearer of two targets that pass. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEntry108 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkEntryArray108;
void fn_7100119f3c(UnkEntryArray108* self, u32 index);
void fn_7100119e14(UnkEntryArray108* self, const UnkEntry108* entry);

struct UnkPool_0x48;
struct UnkStruct_0x48;
UnkStruct_0x48* fn_7100124344(UnkPool_0x48* pool, u32 handle);
UnkStruct_0x48* fn_710012431c(UnkPool_0x48* pool, u32 handle);

struct UnkPath_b0255 {
    UnkEntry108* data;  // 0x0
    u32 count;          // 0x8
    u8 unk_0xc[0x20];
    u8 field_0x2c;
};

struct UnkPathOwner_b0255 {
    u8 unk_0x0[0xa8];
    u8 pool[8];  // 0xa8
};

static inline u32 InnerEnd(const UnkPath_b0255* path) {
    u32 open = path->field_0x2c == 0;
    u32 n = path->count;
    return n - 1 - (open & (n != 0));
}

void fn_7100c66b50(UnkPathOwner_b0255* self, UnkPathOwner_b0255* other) {
    auto* path = reinterpret_cast<UnkPath_b0255*>(fn_7100124344(reinterpret_cast<UnkPool_0x48*>(self->pool), 0));
    fn_7100119f3c(reinterpret_cast<UnkEntryArray108*>(path), 0);
    fn_7100119f3c(reinterpret_cast<UnkEntryArray108*>(path), InnerEnd(path));
    auto* source = reinterpret_cast<UnkPath_b0255*>(fn_710012431c(reinterpret_cast<UnkPool_0x48*>(other->pool), 0));
    UnkEntry108 entry;
    entry.field_0x0 = 0;
    entry.field_0x4 = 0;
    entry.field_0x8 = 0;
    for (u64 i = 1; i < InnerEnd(source); ++i) {
        u32 a = source->data[i].field_0x0;
        u32 b = source->data[i].field_0x4;
        entry.field_0x0 = a;
        entry.field_0x4 = b;
        fn_7100119e14(reinterpret_cast<UnkEntryArray108*>(path), &entry);
    }
}

struct UnkPart0;
void fn_710011d8b4(UnkPart0* self, const UnkPart0* other);

struct UnkPool_0x48;
struct UnkStruct_0x48;
UnkStruct_0x48* fn_710012431c(UnkPool_0x48* pool, u32 handle);

struct UnkPathCopy_b0255 {
    u8 points[0x28];  // 0x0, a base::UnkVector<UnkVec3f>
    u32 field_0x28;
    u8 field_0x2c;
    bool field_0x2d;
    u8 unk_0x2e[2];
    f32 min_x, min_y;  // 0x30
    f32 max_x, max_y;  // 0x38
    u32 field_0x40;
};
struct UnkPathSlot_b0255 {
    u8 unk_0x0[0x28];
    u32 field_0x28;
    u8 field_0x2c;
    u8 unk_0x2d[0x13];
    u32 field_0x40;
};

bool fn_7100118ef0(UnkPathCopy_b0255* path, void* target);

struct UnkWorld_b0255 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    u8 unk_0x8[0xa0];
    u8 pool[8];  // 0xa8
};

struct UnkWorldOwner_b0255 {
    u8 unk_0x0[0x158];
    UnkWorld_b0255* world;  // 0x158
};
struct UnkWorldHolder_b0255 {
    u8 unk_0x0[0x10];
    UnkWorldOwner_b0255* owner;  // 0x10
};
struct UnkPathRef_b0255 {
    UnkWorldHolder_b0255* holder;  // 0x0
    u8 unk_0x8[8];
    u32 handle;  // 0x10
};
struct UnkPathUser_b0255 {
    u8 unk_0x0[0x10];
    void* target;  // 0x10
};

__attribute__((noinline)) bool fn_7100c77a9c(UnkPathUser_b0255* self, UnkPathRef_b0255* ref) {
    UnkWorld_b0255* world = ref->holder->owner->world;
    if ((world != nullptr) & (world->vf_0x18() == 0x10)) {
        UnkPool_0x48* pool = reinterpret_cast<UnkPool_0x48*>(world->pool);
        auto* slot = reinterpret_cast<UnkPathSlot_b0255*>(fn_710012431c(pool, ref->handle));
        UnkPathCopy_b0255 path;
        fn_710011d8b4(reinterpret_cast<UnkPart0*>(&path), reinterpret_cast<const UnkPart0*>(slot));
        path.field_0x28 = slot->field_0x28;
        path.field_0x2c = slot->field_0x2c;
        path.field_0x2d = true;
        path.min_x = 3.4028235e38f;
        path.min_y = 3.4028235e38f;
        path.max_x = -3.4028235e38f;
        path.max_y = -3.4028235e38f;
        path.field_0x40 = slot->field_0x40;
        bool inside = fn_7100118ef0(&path, self->target);
        auto* points = reinterpret_cast<base::UnkVector<UnkVec3f>*>(path.points);
        if (self->target) {
            bool missing = fn_710012431c(pool, ref->handle) == nullptr;
            if (!(inside | missing)) {
                points->Free();
                return true;
            }
        }
        points->Free();
        return false;
    }
    return false;
}

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}


extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};
extern UnkVisitResult_b0040 g_unk_71726a4778;  // the "skip" result
extern u64 g_unk_71726a4780;                   // its guard

static inline UnkVisitResult_b0040 Skip_b0040() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkTarget_b0255 {
    UnkAssignedSlot slot_a;  // 0x0
    UnkAssignedSlot slot_b;  // 0x8
    u32 handle;              // 0x10
    f32 x, y, z;             // 0x14
    u32 w0, w1, w2, w3, w4, w5, w6, w7, w8;  // 0x20

    UnkTarget_b0255& operator=(const UnkTarget_b0255& o) {
        if (this != &o) {
            slot_a = o.slot_a;
            slot_b = o.slot_b;
        handle = o.handle;
        x = o.x;
        y = o.y;
        z = o.z;
        w0 = o.w0;
        w1 = o.w1;
        w2 = o.w2;
        w3 = o.w3;
        w4 = o.w4;
        w5 = o.w5;
        w6 = o.w6;
        w7 = o.w7;
        w8 = o.w8;
        }
        return *this;
    }
};
static_assert(sizeof(UnkTarget_b0255) == 0x48, "layout");

struct UnkTracker_b0255 {
    u8 unk_0x0[8];
    UnkTarget_b0255 target;  // 0x8
    u8 unk_0x50[0x20];
    bool has_target;         // 0x70
    u8 unk_0x71[7];
    f32 x, y, z;             // 0x78
};

UnkVisitResult_b0040 fn_7100c778f8(UnkTracker_b0255* self, const UnkTarget_b0255* target) {
    if (!fn_7100c77a9c(reinterpret_cast<UnkPathUser_b0255*>(&self->x),
                       reinterpret_cast<UnkPathRef_b0255*>(const_cast<UnkTarget_b0255*>(target))))
        goto skip;
    if (self->has_target) {
        f32 held = (self->x - self->target.x) * (self->x - self->target.x) +
                   (self->y - self->target.y) * (self->y - self->target.y) +
                   (self->z - self->target.z) * (self->z - self->target.z);
        f32 other = (self->x - target->x) * (self->x - target->x) + (self->y - target->y) * (self->y - target->y) +
                    (self->z - target->z) * (self->z - target->z);
        f32 diff = held - other;
        if ((s32)(f32)((diff > 0.0f) - (diff < 0.0f)) < 1)
            goto skip;
    }
    self->has_target = true;
    self->target = *target;
    return UnkVisitResult_b0040(1);
skip:
    return Skip_b0040();
}
