// Node routine that calls fn_71002c6c6c, fn_710041a198, fn_710045ee14, fn_71004623f0, fn_71004632ac, fn_710046424c, fn_71008c2da8, builds the string ids "relax", "walk", "back", "walkinit", "backinit", "walkend", "backend", sets its vtable and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x399];
    u8 field_0x3a1;
    u8 pad_0x3a2[0x67];
    u8 field_0x409;
    u8 pad_0x40a[0x516];
    void** field_0x920;
    UnkStrId field_0x928;
    UnkStrId field_0x930;
    UnkStrId field_0x938;
    UnkStrId field_0x940;
    UnkStrId field_0x948;
    UnkStrId field_0x950;
    UnkStrId field_0x958;
};

void fn_710041a198(UnkNode*);
extern void* g_unk_7101a38b28[];
void fn_710045ee14(void*);
void fn_71004623f0(void*);
void fn_71004632ac(void*);
void fn_710046424c(void*);
extern void* g_unk_7101a38c40[];
void fn_71002c6c6c(void*);
void fn_71008c2da8(void*);

void fn_710046e130(UnkNode* self) {
    fn_710041a198(self);
    self->vtable = &g_unk_7101a38b28[2];
    fn_710045ee14(reinterpret_cast<char*>(self) + 0x528);
    fn_71004623f0(reinterpret_cast<char*>(self) + 0x640);
    fn_71004632ac(reinterpret_cast<char*>(self) + 0x730);
    fn_710046424c(reinterpret_cast<char*>(self) + 0x820);
    self->field_0x920 = &g_unk_7101a38c40[2];
    new (&self->field_0x928) UnkStrId("relax", true);
    new (&self->field_0x930) UnkStrId("walk", true);
    new (&self->field_0x938) UnkStrId("back", true);
    new (&self->field_0x940) UnkStrId("walkinit", true);
    new (&self->field_0x948) UnkStrId("backinit", true);
    new (&self->field_0x950) UnkStrId("walkend", true);
    new (&self->field_0x958) UnkStrId("backend", true);
    fn_71002c6c6c(reinterpret_cast<char*>(self) + 0x960);
    fn_71002c6c6c(reinterpret_cast<char*>(self) + 0x9b8);
    fn_71002c6c6c(reinterpret_cast<char*>(self) + 0xa10);
    fn_71008c2da8(reinterpret_cast<char*>(self) + 0xa68);
    fn_71008c2da8(reinterpret_cast<char*>(self) + 0xad8);
    fn_71008c2da8(reinterpret_cast<char*>(self) + 0xb48);
    self->field_0x409 = 0;
    self->field_0x3a1 = 1;
}
