// CXParasiteDropComponent unit (tools/tu_map.py unit 1231). Its destructors: the vtable, the list of
// owned objects at +0x48 (game_s2/UnkOwnedList.h) destroyed, then the component destructor
// fn_71010c4194; fn_71007cec94 also frees the object. Names and layouts are placeholders.
#include "game/UnkComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

extern void* g_unk_7101a718a8[];

struct UnkXParasiteDropComponent_b49a3 : UnkBaseComponent {
    u8 pad_0x40[0x8];
    UnkOwnedList list;  // 0x48
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkXParasiteDropComponent_b49a3* self) {
    self->vtable = g_unk_7101a718a8 + 2;
    self->list.~UnkOwnedList();
    fn_71010c4194(self);
}

void fn_71007cebf0(UnkXParasiteDropComponent_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_71007cec94(UnkXParasiteDropComponent_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
