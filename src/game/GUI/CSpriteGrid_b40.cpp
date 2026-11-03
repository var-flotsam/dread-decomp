// GUI::CSpriteGrid unit (tools/tu_map.py, the name from reflection): two reflected setters of the
// object at 8. One sets the file-path id at +0x198 (a counted copy); when it changes and the flag at
// +0x184 is clear, it sets the flag and runs the virtual at 0xa0. The other resolves a frame from the
// argument (fn_71011f8750; null when it has no type, INT_MAX for none), stores it at +0x1dc and
// passes it to fn_71001c39c8 on the model at +0x190. Both return the empty value. Names are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflValue;

struct UnkSpriteGrid_b40a3 {
    void** vtable;
    u8 pad_0x8[0x17c];
    bool dirty;  // 0x184
    u8 pad_0x185[0xb];
    char* model;  // 0x190
    CFilePathStrId path;  // 0x198
    u8 pad_0x1a0[0x3c];
    s32 frame;  // 0x1dc
};

UnkReflValue fn_71011fde48(void*, char* obj, UnkReflValue* const* args) {
    auto* target = *reinterpret_cast<UnkSpriteGrid_b40a3**>(obj + 8);
    const CFilePathStrId* value = base::reflection::refl_ArgValue<CFilePathStrId>(args[0]);
    CFilePathStrId* path = &target->path;
    if (value->data != path->data) {
        UnkStringData* d = value->data;
        UnkStringPool* pool = g_unk_7101d47200;
        if (d != &pool->empty)
            ++d->ref_count;
        if (path->data != &pool->empty) {
            --path->data->ref_count;
            if (path->data->ref_count == 0)
                fn_71000803a4(pool, path->data);
        }
        path->data = d;
        if (!target->dirty) {
            target->dirty = true;
            reinterpret_cast<void (*)(UnkSpriteGrid_b40a3*)>(target->vtable[0xa0 / 8])(target);
        }
    }
    return base::reflection::refl_EmptyValue();
}

s32 fn_71011f8750(const void* arg);
void fn_71001c39c8(void* self, s32 frame);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71011fe48c(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkSpriteGrid_b40a3**>(obj + 8);
    s32 frame = fn_71011f8750(ArgPtr_b40a3(a));
    if (frame != 0x7fffffff) {
        char* model = target->model;
        target->frame = frame;
        if (model)
            fn_71001c39c8(*reinterpret_cast<void**>(model + 0x128), frame);
    }
    return base::reflection::refl_EmptyValue();
}
