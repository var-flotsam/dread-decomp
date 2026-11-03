// CChangeStageNavMeshItemComponent's destructor body (vtable g_unk_7101a88328): the member at 0x48
// is destroyed, both handles still held are released through the registry at 0x68, then the base.
#include "game/UnkComponent.h"
#include "types.h"

extern void* g_unk_7101a88328[];

struct UnkNavMeshRegistry {
    void** vtable;
    void Release(void* handle) {
        reinterpret_cast<void (*)(UnkNavMeshRegistry*, void*)>(vtable[0x48 / 8])(this, handle);
    }
};

struct UnkNavMeshItemMember {
    void* handle;
};

void fn_71009ba68c(UnkNavMeshItemMember* self);

struct CChangeStageNavMeshItemComponent {
    void** vtable;
    u8 unk_0x8[0x40];
    UnkNavMeshItemMember member;  // 0x48
    void* handle;                 // 0x50
    u8 unk_0x58[0x10];
    UnkNavMeshRegistry* registry;  // 0x68
};

void fn_71009b99e8(CChangeStageNavMeshItemComponent* self) {
    self->vtable = g_unk_7101a88328 + 2;
    UnkNavMeshItemMember* member = &self->member;
    fn_71009ba68c(member);
    if (self->handle)
        self->registry->Release(self->handle);
    if (member->handle)
        self->registry->Release(member->handle);
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
