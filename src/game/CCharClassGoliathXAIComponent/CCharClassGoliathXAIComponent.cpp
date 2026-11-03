// Destructors (complete and deleting) in the CCharClassGoliathXAIComponent unit (vtable
// 0x7101a4b340): the members at +0x720 and +0x628 (fn_71005839ac, fn_71005835c0), then the base's
// vtable (0x7101a4c200), its member at +0x530 (fn_710058d990) and the base destructor fn_710041a230.
// Names and layouts are placeholders.
#include <new>

#include "types.h"

struct UnkGoliathXBase;
void fn_710041a230(UnkGoliathXBase* self);
void fn_710058d990(void* self);
void fn_71005835c0(void* self);
void fn_71005839ac(void* self);
extern void* g_unk_7101a4b340[];
extern void* g_unk_7101a4c200[];

struct UnkGoliathXAI {
    void** vtable;
    u8 pad_0x8[0x528];
    u8 field_0x530[0xf8];
    u8 field_0x628[0xf8];
    u8 field_0x720[0x8];

    __attribute__((always_inline)) ~UnkGoliathXAI() {
        vtable = g_unk_7101a4b340 + 2;
        fn_71005839ac(field_0x720);
        fn_71005835c0(field_0x628);
        vtable = g_unk_7101a4c200 + 2;
        fn_710058d990(field_0x530);
        fn_710041a230(reinterpret_cast<UnkGoliathXBase*>(this));
    }
};

void fn_7100587c9c(UnkGoliathXAI* self) { self->~UnkGoliathXAI(); }
void fn_7100587cf4(UnkGoliathXAI* self) {
    self->~UnkGoliathXAI();
    operator delete(self);
}
