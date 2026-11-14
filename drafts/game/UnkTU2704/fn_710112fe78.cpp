// CSpawnPointComponent's unit (unit 2704): inserts a reflected value into an array of values at an index.
// The array grows first; values from the index up are moved one place along (the payload of each moved-from
// value is released through the game manager or deleted). Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkBoundObj;

struct UnkReflValueArray {
    UnkReflValue* data;
    u32 count;
    u32 capacity;
    u32 field_0x10;
    u32 field_0x14;
    u32 flags;
    u32 pad_0x1c;
    base::UnkAllocator* allocator;
};
static_assert(sizeof(UnkReflValueArray) == 0x28, "placeholder layout");

void fn_7101130010(UnkReflValueArray* self, u32 capacity);

void fn_710112fe78(UnkReflValueArray* self, const UnkReflValue* value, u32 index) {
    u32 count = self->count;
    u32 capacity = self->capacity;
    if (count == index) {
        if (capacity <= count && self->field_0x10 != 0)
            capacity = (index / self->field_0x10 + 1) * self->field_0x10;
        if ((s32)capacity <= (s32)index) {
            for (;;) {
            }
        }
        fn_7101130010(self, capacity);
        index = self->count;
    } else {
        if (capacity <= count && self->field_0x10 != 0)
            capacity = (count / self->field_0x10 + 1) * self->field_0x10;
        fn_7101130010(self, capacity);
        u32 i = self->count - 1;
        do {
            if (i < index)
                break;
            fn_71000a179c(&self->data[i + 1], &self->data[i]);
            UnkReflValue* v = &self->data[i];
            if (v->type) {
                bool released = fn_7100deabe4(g_unk_7101d47310);
                void* payload = nullptr;
                if (v->type) {
                    payload = v;
                    if (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
                        payload = v->field_0x0;
                }
                if (released) {
                    fn_7100232124(static_cast<UnkBoundObj*>(payload));
                } else if (payload) {
                    fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
                    fn_7100083c00(payload);
                }
            }
            fn_71000a1c60(v);
        } while (--i != 0xffffffffu);
    }
    fn_71000a179c(&self->data[index], value);
    self->count = self->count + 1;
}
