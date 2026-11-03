// CLockOnMissileGun unit (tools/tu_map.py, the name from reflection): the constructor and destructor. The constructor: the base
// (fn_710084e848), this class's vtable (g_unk_7101a784a8), its default state, and the node
// registration fn_7100023854 last. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkNode;
void fn_7100023854(UnkNode*);

extern char g_unk_7101a784a8[];

// A locked-on target: a counted reference, kept on a 4-byte boundary.
struct __attribute__((packed, aligned(4))) UnkLockOnTarget {
    UnkRefObject* ref;
    u8 unk_0x8[0x10];
};

struct UnkLockOnMissileGun {
    char* vtable;
    u8 base_0x8[0x186c];
    u32 field_0x1874;
    u8 pad_0x1878[0xa8];
    u32 count;  // 0x1920
    UnkLockOnTarget targets[5];  // 0x1924

    bool field_0x199c;
    u32 field_0x19a0;
    bool field_0x19a4;
    bool field_0x19a5;
    f32 field_0x19a8;
    f32 field_0x19ac;
    f32 field_0x19b0;
    f32 field_0x19b4;
    u64 field_0x19b8;
};

void fn_710084e848(UnkLockOnMissileGun* self);  // the base constructor
// The base destructor, with the signature src/game/GUI/CSubtitleComposition/fn_7100b85000.cpp defines it with.
template <void** Vtable, u32 At>
struct UnkLinkOwner;
extern void* g_unk_7101a786e0[];
void fn_710084e8fc(UnkLinkOwner<g_unk_7101a786e0, 0x18a0>* self);

void fn_710084d9d0(UnkLockOnMissileGun* self) {
    fn_710084e848(self);
    self->vtable = g_unk_7101a784a8 + 0x10;
    self->count = 0;
    self->field_0x199c = false;
    self->field_0x19a0 = 1;
    self->field_0x19a4 = true;
    self->field_0x19a5 = false;
    self->field_0x19a8 = 0.0f;
    self->field_0x19ac = 0.05f;
    self->field_0x19b0 = 0.0f;
    self->field_0x19b4 = 0.001f;
    self->field_0x19b8 = 0;
    self->field_0x1874 = 10;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}

// The destructor: drops the target references, then the base destructor (fn_710084e8fc); the
// deleting form frees the object after.
static inline __attribute__((always_inline)) void Destroy(UnkLockOnMissileGun* self) {
    self->vtable = g_unk_7101a784a8 + 0x10;
    for (u64 i = 0; i < self->count; i++) {
        if (UnkRefObject* p = self->targets[i].ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    self->count = 0;
    fn_710084e8fc(reinterpret_cast<UnkLinkOwner<g_unk_7101a786e0, 0x18a0>*>(self));
}

void fn_710084cb58(UnkLockOnMissileGun* self) {
    Destroy(self);
}

void fn_710084cbec(UnkLockOnMissileGun* self) {
    Destroy(self);
    operator delete(self);
}

// The tunables (CLockOnMissileGun::CTunableLockOnMissile, src/reflection/CLockOnMissileGun.cpp): two
// flags at +0x28 and +0x29 pick which retry timer runs.
extern base::reflection::UnkTunable g_unk_7172726e58;

void fn_7100835bbc(UnkLockOnMissileGun* self, f32 dt);
void fn_710084cd84(UnkLockOnMissileGun* self);

// The update: after the base update (fn_7100835bbc), while locking (+0x199c) counts down the timer the
// tunables pick (+0x19a8, else +0x19b0), rearming it from its interval and stepping the lock
// (fn_710084cd84) when it runs out; with neither timer it steps until the lock ends.
void fn_710084ccd0(UnkLockOnMissileGun* self, f32 dt) {
    fn_7100835bbc(self, dt);
    if (!self->field_0x199c)
        return;
    const u8* tunable = reinterpret_cast<const u8*>(&g_unk_7172726e58);
    if (tunable[0x28]) {
        f32 t = self->field_0x19a8 - dt;
        self->field_0x19a8 = t;
        if (!(t <= 0.0f))
            return;
        self->field_0x19a8 = self->field_0x19ac;
        fn_710084cd84(self);
        return;
    } else if (tunable[0x29]) {
        f32 t = self->field_0x19b0 - dt;
        self->field_0x19b0 = t;
        if (!(t <= 0.0f))
            return;
        self->field_0x19b0 = self->field_0x19b4;
    } else {
        do
            fn_710084cd84(self);
        while (self->field_0x199c);
        return;
    }
    fn_710084cd84(self);
}
