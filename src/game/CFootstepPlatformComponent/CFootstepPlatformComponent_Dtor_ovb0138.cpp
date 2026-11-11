// CFootstepPlatformComponent's unit (tools/tu_map.py unit 1255): the destructor. Its two vtables,
// the names at +0xe8 and +0xe0, the callback lists at +0x88 and +0x48, then the base component's
// destructor (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CFootstepPlatformComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkFootstepCallbacks_ovb0138;
template <>
struct base::UnkCallbackTraits<UnkFootstepCallbacks_ovb0138> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkFootstepCallbackList_ovb0138 = base::UnkCallbackListShort<UnkFootstepCallbacks_ovb0138>;

struct UnkFootstepPlatformD_ovb0138 {
    char* vtable;
    u8 pad_0x8[0x38];
    char* vtable_0x40;
    UnkFootstepCallbackList_ovb0138 list_a;  // 0x48
    UnkFootstepCallbackList_ovb0138 list_b;  // 0x88
    u8 pad_0xc8[0x18];
    base::UnkStrIdMember name_a;  // 0xe0
    base::UnkStrIdMember name_b;  // 0xe8
};
static_assert(__builtin_offsetof(UnkFootstepPlatformD_ovb0138, name_b) == 0xe8, "layout");

void fn_71007e78a8(UnkFootstepPlatformD_ovb0138* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a73a70) + 0x10;
    self->vtable_0x40 = reinterpret_cast<char*>(g_unk_7101a73a70) + 0x1a0;
    using base::UnkStrIdMember;
    self->name_b.~UnkStrIdMember();
    self->name_a.~UnkStrIdMember();
    self->list_b.~UnkFootstepCallbackList_ovb0138();
    self->list_a.~UnkFootstepCallbackList_ovb0138();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
