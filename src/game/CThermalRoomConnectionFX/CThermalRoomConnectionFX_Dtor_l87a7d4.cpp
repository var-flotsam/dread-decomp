// CThermalRoomConnectionFX's unit (tools/tu_map.py unit 1334): the destructor. Its vtable, the four
// counted references at +0x118 down to +0x100, the callback lists at +0xc0 and +0x80, then the
// activatable component's destructor (fn_71007b00cc). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/UnkTU341.h"
#include "types.h"

template <u32 IdOffset>
struct UnkActivatable_b44a2;
void fn_71007b00cc(UnkActivatable_b44a2<0x48>* self);
extern void* g_unk_7101a7b058[];

struct UnkThermalRoomCallbacks_l87a7d4;
template <>
struct base::UnkCallbackTraits<UnkThermalRoomCallbacks_l87a7d4> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkThermalRoomCallbackList_l87a7d4 = base::UnkCallbackListShort<UnkThermalRoomCallbacks_l87a7d4>;

struct UnkThermalRoomConnectionFXD_l87a7d4 {
    void** vtable;
    u8 pad_0x8[0x78];
    UnkThermalRoomCallbackList_l87a7d4 list_a;  // 0x80
    UnkThermalRoomCallbackList_l87a7d4 list_b;  // 0xc0
    base::UnkRefPtrMember refs[4];              // 0x100
};
static_assert(__builtin_offsetof(UnkThermalRoomConnectionFXD_l87a7d4, refs) == 0x100, "layout");

void fn_710087a7d4(UnkThermalRoomConnectionFXD_l87a7d4* self) {
    self->vtable = g_unk_7101a7b058 + 2;
    using base::UnkRefPtrMember;
    self->refs[3].~UnkRefPtrMember();
    self->refs[2].~UnkRefPtrMember();
    self->refs[1].~UnkRefPtrMember();
    self->refs[0].~UnkRefPtrMember();
    self->list_b.~UnkThermalRoomCallbackList_l87a7d4();
    self->list_a.~UnkThermalRoomCallbackList_l87a7d4();
    fn_71007b00cc(reinterpret_cast<UnkActivatable_b44a2<0x48>*>(self));
}
