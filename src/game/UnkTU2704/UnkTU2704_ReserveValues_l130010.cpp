// CSpawnPointComponent's unit (unit 2704): reserve of an array of reflected values. A larger block is
// allocated, each value copied across and destroyed (its payload released through the game manager
// or deleted), and the old block freed. Names and layouts are placeholders.
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

void fn_7101130010(UnkReflValueArray* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkReflValue* values =
        static_cast<UnkReflValue*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkReflValue), 8, self->flags));
    UnkReflValue* src = self->data;
    if (src) {
        if (self->count) {
            UnkReflValue* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                fn_71000a179c(dst, src);
                if (src->type) {
                    bool released = fn_7100deabe4(g_unk_7101d47310);
                    void* payload = nullptr;
                    if (src->type) {
                        payload = src;
                        if (src->field_0x14 == 1 || *reinterpret_cast<const u64*>(src->type + 0x18) > 8)
                            payload = src->field_0x0;
                    }
                    if (released) {
                        fn_7100232124(static_cast<UnkBoundObj*>(payload));
                    } else if (payload) {
                        fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
                        fn_7100083c00(payload);
                    }
                }
                fn_71000a1c60(src);
            }
            src = self->data;
        }
        self->allocator->vf_0x48(src);
    }
    self->data = values;
    self->capacity = capacity;
}
