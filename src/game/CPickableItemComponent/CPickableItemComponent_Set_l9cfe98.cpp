// Unit 1493 (CPickableItemComponent): fills an item description: its ids, two values, a kind parsed
// from its name, two strings and three flags derived from them. Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrId;

struct UnkItemDesc_l9cfe98 {
    u8 pad_0x0[0x50];
    f32 field_0x50;
    u8 pad_0x54[0x44];
    UnkStrId field_0x98;
    UnkStrId field_0xa0;
    f32 field_0xa8;
    u8 pad_0xac[4];
    u32 kind;  // 0xb0
    UnkStrId field_0xb8;
    CRntString field_0xc0;
    CRntString field_0xf0;
    u8 pad_0x120[8];
    bool field_0x128;
    bool field_0x129;  // field_0xb8 is the empty id
    bool field_0x12a;  // field_0xf0 has text
};
static_assert(__builtin_offsetof(UnkItemDesc_l9cfe98, field_0x128) == 0x128, "layout");

static inline u32 ParseKind(const char* name) {
    if (strcmp(name, "None") == 0)
        return 0;
    if (strcmp(name, "Tank") == 0)
        return 1;
    if (strcmp(name, "Full") == 0)
        return 2;
    if (strcmp(name, "Custom") == 0)
        return 3;
    return 4;
}

void CPickableItemComponent_SetIdsStringsAndKind(UnkItemDesc_l9cfe98* self, f32 value_0xa8, f32 value_0x50, const char* id_0x98,
                   const char* kind, const char* id_0xa0, const char* id_0xb8, const char* string_0xc0,
                   const char* string_0xf0, bool flag) {
    {
        UnkStrId id(id_0x98, true);
        self->field_0x98 = static_cast<UnkStrId&&>(id);
    }
    self->field_0xa8 = value_0xa8;
    self->field_0x50 = value_0x50;
    self->kind = ParseKind(kind);
    {
        UnkStrId id(id_0xa0, true);
        self->field_0xa0 = static_cast<UnkStrId&&>(id);
    }
    {
        UnkStrId id(id_0xb8, true);
        self->field_0xb8 = static_cast<UnkStrId&&>(id);
    }
    CRntString_AssignCString(&self->field_0xc0, string_0xc0);
    CRntString_AssignCString(&self->field_0xf0, string_0xf0);
    self->field_0x128 = flag;
    self->field_0x129 = self->field_0xb8.data == &g_unk_7101d47108->empty;
    self->field_0x12a = self->field_0xf0.data && self->field_0xf0.data[0] != '\0';
}
