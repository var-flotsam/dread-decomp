// Unit 1444 of tools/tu_map.py (no name hint): the destructor pair of its class. It frees the array
// of 0x18-byte entries at +0x208, destroys the member at +0x90 (vtable g_unk_7101a6bff8, with a
// 0x100-byte object at its +0x40) and runs the base destructor fn_7100969058. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU1130.h"
#include "game/UnkTU1440.h"
#include "game/UnkTU1444.h"

struct UnkUnit1444Base;

struct UnkEntry_1444 {
    virtual ~UnkEntry_1444();
    u8 pad_0x8[0x10];
};

struct UnkMember_1444 {
    void* vtable;
    u8 pad_0x8[0x38];
    UnkObject_0x100 object;  // +0x40
};

struct UnkUnit1444 {
    void* vtable;
    u8 pad_0x8[0x88];
    UnkMember_1444 member;  // 0x90
    u8 pad_0x190[0x38];
    base::UnkVector<UnkEntry_1444> entries;  // 0x208
};
static_assert(__builtin_offsetof(UnkUnit1444, entries) == 0x208, "UnkUnit1444 layout");

static inline void Dtor1444(UnkUnit1444* self) {
    self->vtable = g_unk_7101a853b8 + 2;
    self->entries.Free();
    self->member.vtable = g_unk_7101a6bff8 + 2;
    fn_71002b6094(&self->member.object);
    fn_7100969058(reinterpret_cast<UnkUnit1444Base*>(self));
}

void fn_7100971574(UnkUnit1444* self) {
    Dtor1444(self);
}

void fn_710097163c(UnkUnit1444* self) {
    Dtor1444(self);
    fn_7100083c00(self);
}
