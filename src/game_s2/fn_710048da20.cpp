// Node routine that calls fn_710047ba88, fn_710047c6b4, fn_710047d830, fn_710047e698, fn_710047f4d4, fn_7100480114, fn_7100497584, sets its vtable and makes a virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
};

void fn_7100497584(UnkNode*);
extern void* g_unk_7101a3b748[];
void fn_710047c6b4(void*);
void fn_7100480114(void*);
void fn_710047d830(void*);
void fn_710047ba88(void*);
void fn_710047e698(void*);
void fn_710047f4d4(void*);

void fn_710048da20(UnkNode* self) {
    fn_7100497584(self);
    self->vtable = &g_unk_7101a3b748[2];
    fn_710047c6b4(reinterpret_cast<char*>(self) + 0x808);
    fn_7100480114(reinterpret_cast<char*>(self) + 0x8f8);
    fn_710047d830(reinterpret_cast<char*>(self) + 0x9e8);
    fn_710047ba88(reinterpret_cast<char*>(self) + 0xad8);
    fn_710047e698(reinterpret_cast<char*>(self) + 0xbc8);
    fn_710047f4d4(reinterpret_cast<char*>(self) + 0xcb8);
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
