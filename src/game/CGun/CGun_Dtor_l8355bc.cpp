// CGun unit: a destructor. Takes its three render items out of the render manager, releases its
// string ids, then runs the game object's destructor. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/CGun.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkGun_l8355bc {
    void** vtable;
    u8 pad_0x8[0x8];
    CStrId field_0x10;
    u8 pad_0x18[0x8];
    CStrId field_0x20;
    CStrId field_0x28;
    u8 pad_0x30[0x20];
    CStrId field_0x50;
    CStrId field_0x58;
    u8 pad_0x60[0x10];
    CStrId field_0x70;
    u8 pad_0x78[0x50];
    UnkItemBase_71011a91c4* items[3];  // 0xc8
    u8 pad_0xe0[0x1868 - 0xe0];
    CFilePathStrId field_0x1868;
};
static_assert(__builtin_offsetof(UnkGun_l8355bc, items) == 0xc8, "UnkGun_l8355bc layout");
static_assert(__builtin_offsetof(UnkGun_l8355bc, field_0x1868) == 0x1868, "UnkGun_l8355bc layout");

void fn_71008355bc(UnkGun_l8355bc* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a77980 + 0x10);
    for (int i = 0; i < 3; ++i) {
        if (self->items[i]) {
            fn_71002199d8(g_unk_7101d47300, self->items[i], 0);
            self->items[i] = nullptr;
        }
    }
    self->field_0x1868.~CFilePathStrId();
    self->field_0x70.~CStrId();
    self->field_0x58.~CStrId();
    self->field_0x50.~CStrId();
    self->field_0x28.~CStrId();
    self->field_0x20.~CStrId();
    self->field_0x10.~CStrId();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
