#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkChargeComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
struct UnkPlayerMovementD_b0204;
void fn_71010d80e8(UnkPlayerMovementD_b0204* self, u32 kind);
void fn_71010d9f88(UnkPlayerMovementD_b0204* self);
void fn_71010d7d0c(UnkPlayerMovementD_b0204* self, u32 kind);
void fn_71010da220(UnkPlayerMovementD_b0204* self, f32 a, f32 b, f32 c);
extern void* g_unk_71726dbf68;
extern base::global::CStrId g_unk_71726dda50;
extern base::reflection::UnkTunable g_unk_7172a03d80;

struct UnkRequest_b0204 {
    u8 pad_0x0[0xb8];
    const void* field_0xb8;
};

bool fn_71010dc968(UnkPlayerMovementD_b0204* self, UnkRequest_b0204* req) {
    if (req->field_0xb8 != g_unk_71726dda50.data)
        fn_71010d80e8(self, 2);
    else
        fn_71010d9f88(self);
    if (req->field_0xb8 != g_unk_71726dbf68) {
        fn_71010d7d0c(self, 3);
    } else {
        const f32* charge = reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_71726e0aa8) + 0xd8);
        fn_71010da220(self, charge[0], charge[1], *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172a03d80) + 0x60));
    }
    return false;
}

struct UnkSmartObjectActor;
struct UnkSmartObjectUserB;
UnkSmartObjectUserB* fn_7100a81ed0(UnkSmartObjectActor* actor);

struct UnkCollisionObj_b0204 {
    base::reflection::UnkReflObject base;
    u8 pad_0x8[0x28];
    void* field_0x30;
};

struct UnkUserB_b0204 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkWeakTarget* field_0x30;
};

struct UnkRefHolder_b0204 {
    u8 pad_0x0[0x7e8];
    UnkWeakHandle* handle;
};

struct UnkHolderArg_b0204 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};

bool fn_71010d9774(UnkRefHolder_b0204* self, UnkHolderArg_b0204* arg, bool flag) {
    if (arg) {
        base::reflection::UnkReflObject* c = arg->field_0x10;
        if (c && fn_710009d50c(c->vf_0x0(), fn_710085fc44())) {
            UnkUserB_b0204* user = reinterpret_cast<UnkUserB_b0204*>(fn_7100a81ed0(reinterpret_cast<UnkSmartObjectActor*>(reinterpret_cast<UnkCollisionObj_b0204*>(c)->field_0x30)));
            if (user) {
                if (user->field_0x30)
                    AssignRef(&self->handle, fn_7100d78378(user->field_0x30));
                else
                    AssignRef(&self->handle, static_cast<UnkWeakHandle*>(nullptr));
                u32 r = reinterpret_cast<u32 (*)(UnkUserB_b0204*, UnkHolderArg_b0204*, bool)>(user->vtable[0x170 / 8])(user, arg, flag);
                return r & 1;
            }
        }
    }
    AssignRef(&self->handle, static_cast<UnkWeakHandle*>(nullptr));
    return false;
}
