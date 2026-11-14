// CSpawnPointComponent's unit (2704): empties an array of reflected values, destroying each element
// (its payload is released through the game manager or deleted) and setting the count to zero.
// Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkBoundObj;

struct UnkValueElem_l12f85c {
    UnkReflValue value;
};
static_assert(sizeof(UnkValueElem_l12f85c) == 0x18, "element size");

struct UnkValueArray_l12f85c {
    UnkValueElem_l12f85c* data;  // 0x0
    u32 count;                   // 0x8
};

void fn_710112f85c(UnkValueArray_l12f85c* self) {
    for (u64 i = 0; i < self->count; i++) {
        UnkValueElem_l12f85c* elems = self->data;
        UnkReflValue* v = &elems[i].value;
        if (v->type) {
            bool released = fn_7100deabe4(g_unk_7101d47310);
            void* payload = nullptr;
            if (v->type)
                payload = (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
                              ? elems[i].value.field_0x0
                              : v;
            if (released) {
                fn_7100232124(static_cast<UnkBoundObj*>(payload));
            } else if (payload) {
                fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
                fn_7100083c00(payload);
            }
        }
        fn_71000a1c60(v);
    }
    self->count = 0;
}
