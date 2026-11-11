// CChangeStageNavMeshItemComponent's deleting destructor: the destructor body of fn_71009b99e8
// (CChangeStageNavMeshItemComponent_b34.cpp), then the memory is freed (fn_7100083c00).
#include "base/UnkVector.h"
#include "game/CChangeStageNavMeshItemComponent.h"
#include "game/UnkComponent.h"
#include "types.h"

extern void* g_unk_7101a88328[];

struct UnkNavMeshRegistry2 {
    void** vtable;
    void Release(void* handle) {
        reinterpret_cast<void (*)(UnkNavMeshRegistry2*, void*)>(vtable[0x48 / 8])(this, handle);
    }
};

struct UnkNavMeshItemMember {
    void* handle;
};

struct UnkChangeStageNavMeshItem {
    void** vtable;
    u8 unk_0x8[0x40];
    UnkNavMeshItemMember member;  // 0x48
    void* handle;                 // 0x50
    u8 unk_0x58[0x10];
    UnkNavMeshRegistry2* registry;  // 0x68
};

void fn_71009b9a54(UnkChangeStageNavMeshItem* self) {
    self->vtable = g_unk_7101a88328 + 2;
    UnkNavMeshItemMember* member = &self->member;
    fn_71009ba68c(member);
    if (self->handle)
        self->registry->Release(self->handle);
    if (member->handle)
        self->registry->Release(member->handle);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
    fn_7100083c00(self);
}
