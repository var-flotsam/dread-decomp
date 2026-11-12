// CAINavigationComponent unit (tools/tu_map.py unit 1113): fn_71006f4450 registers the component's two
// listeners (+0x68, +0x88) again with what the owner's enemy movement (+0x1f0, when it is a CEnemyMovement) and
// its object at +0x258 now point at (as CAIAttackComponent's fn_7101133dc8); fn_71006f5104 finds the index of
// the path point nearest the owner. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAINavigationComponent.h"
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkLinkOwner_ain;
struct UnkListener_ain;
void fn_7101137c7c(void* target, void* link);
void fn_71006f5630(void* source, UnkWeakHandle** slot);

struct UnkNavOwner_b0129 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x60];
    void* field_0x258;
};

struct UnkNavLinks_b0129 {
    u8 pad_0x0[0x30];
    UnkNavOwner_b0129* owner;  // 0x30
    u8 pad_0x38[0x30];
    u8 link0[0x18];          // 0x68
    UnkWeakHandle* handle0;  // 0x80
    u8 link1[0x18];          // 0x88
    UnkWeakHandle* handle1;  // 0xa0
};

static inline UnkWeakHandle** Slot(UnkNavLinks_b0129* self, u32 offset) {
    return reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<u8*>(self) + offset);
}

// Registers the two listeners again with what the owner's enemy movement (+0x1f0, when it is one) and
// its object at +0x258 now point at.
bool fn_71006f4450(UnkNavLinks_b0129* self) {
    void* link0 = self->link0;
    UnkReflObject* movement = self->owner->movement;
    if (movement)
        movement = fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) ? movement : nullptr;
    if (UnkWeakHandle* handle = self->handle0) {
        if (handle->target) {
            fn_71010cb2fc(handle->target, link0);
            if (!self->handle0)
                goto fetch0;
        }
        ResetRef(&self->handle0);
    }
fetch0:
    if (movement) {
        fn_71006f552c(movement, Slot(self, 0x80));
        fn_71010cb2f4(self->handle0 ? self->handle0->target : nullptr, link0);
    }
    UnkNavOwner_b0129* owner = self->owner;
    void* link1 = self->link1;
    UnkWeakHandle* handle = self->handle1;
    void* source = owner->field_0x258;
    if (handle) {
        if (handle->target) {
            fn_7101137c84(reinterpret_cast<UnkLinkOwner_ain*>(handle->target),
                          reinterpret_cast<UnkListener_ain*>(link1));
            if (!self->handle1)
                goto fetch1;
        }
        ResetRef(&self->handle1);
    }
fetch1:
    if (source) {
        fn_71006f5630(source, Slot(self, 0xa0));
        fn_7101137c7c(self->handle1 ? self->handle1->target : nullptr, link1);
    }
    return true;
}

struct UnkNavPoint_b0129 {
    f32 x, y, z;
};

// A node of a path tree: children by index (slot 0x30), a count (slot 0x28) and a point (slot 0x18).
struct UnkNavNode_b0129 {
    void** vtable;

    const UnkNavPoint_b0129* Point() {
        return reinterpret_cast<const UnkNavPoint_b0129* (*)(UnkNavNode_b0129*)>(vtable[0x18 / 8])(this);
    }
    u32 Count() { return reinterpret_cast<u32 (*)(UnkNavNode_b0129*)>(vtable[0x28 / 8])(this); }
    UnkNavNode_b0129* Child(u32 i) {
        return reinterpret_cast<UnkNavNode_b0129* (*)(UnkNavNode_b0129*, u32)>(vtable[0x30 / 8])(this, i);
    }
};

struct UnkNavPath_b0129 {
    u8 pad_0x0[0x30];
    UnkNavNode_b0129 root;
};

struct UnkNavSelf_b0129 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

static inline f32 Dist2(f32 x, f32 y, f32 z, const UnkNavPoint_b0129& p) {
    f32 dx = x - p.x;
    f32 dy = y - p.y;
    f32 dz = z - p.z;
    return dx * dx + dy * dy + dz * dz;
}

// The index of the path's point nearest the owner.
u32 fn_71006f5104(UnkNavSelf_b0129* self, UnkNavPath_b0129* path) {
    f32 x = self->owner->field_0x44;
    f32 y = self->owner->field_0x54;
    f32 z = self->owner->field_0x64;
    UnkNavNode_b0129* root = &path->root;
    UnkNavPoint_b0129 first = *root->Child(0)->Child(0)->Point();
    f32 best_d = Dist2(x, y, z, first);
    u32 best = 0;
    for (u32 i = 1; i < root->Child(0)->Count(); ++i) {
        const UnkNavPoint_b0129* p = root->Child(0)->Child(i)->Point();
        f32 dx = x - p->x;
        f32 dy = y - p->y;
        f32 d = dx * dx + dy * dy;
        f32 dz = z - p->z;
        d += dz * dz;
        bool closer = d < best_d;
        best_d = closer ? d : best_d;
        best = closer ? i : best;
    }
    return best;
}
