// CSpawnPointComponent's unit (2704): removes one element of an array of reflected values: destroys it, then
// moves each later element down one place. Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkBoundObj;

struct UnkValueArray_l12f960 {
    UnkReflValue* data;  // 0x0
    u32 count;           // 0x8
};

// Releases the payload of a value, then destroys the value.
inline void DestroyValue_l12f960(UnkReflValue* v) {
    if (v->type) {
        bool released = fn_7100deabe4(g_unk_7101d47310);
        void* payload = nullptr;
        if (v->type)
            payload = (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8) ? v->field_0x0 : v;
        if (released) {
            fn_7100232124(static_cast<UnkBoundObj*>(payload));
        } else if (payload) {
            fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
            fn_7100083c00(payload);
        }
    }
    fn_71000a1c60(v);
}

void fn_710112f960(UnkValueArray_l12f960* self, const u32* index) {
    u64 i = *index;
    if (self->count <= i)
        return;
    DestroyValue_l12f960(&self->data[i]);
    for (; i < self->count - 1; ++i) {
        UnkReflValue* cur = &self->data[i];
        fn_71000a179c(cur, cur + 1);
        cur = &self->data[i];
        DestroyValue_l12f960(cur + 1);
    }
    self->count = self->count - 1;
}
