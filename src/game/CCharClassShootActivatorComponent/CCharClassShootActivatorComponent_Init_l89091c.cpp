// CCharClassShootActivatorComponent's unit (tools/tu_map.py unit 1346): after the base's step, caches the
// owner as the character class it should be, copies two of its words, resolves the component's sound path
// and, when it names an audio preset, copies two of the preset's values. Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_71008e0804(char* s);

// CCharClassShootActivatorComponent's class (what fn_710088daec returns).
__attribute__((visibility("hidden"))) extern base::reflection::CClass g_unk_717272d0c8;

struct UnkShootCharClass_l89091c : UnkReflObject {
    u8 pad_0x8[0x114];
    u32 field_0x11c;
    u32 field_0x120;
};

struct UnkAudioPreset_l89091c : UnkReflObject {
    u8 pad_0x8[0x580];
    u32 field_0x588;
    u8 pad_0x58c[0x4];
    u32 field_0x590;
};

struct UnkShootActivator_l89091c;

struct UnkShootActivatorVtable_l89091c {
    void* pad_0x0[0x40];
    UnkStrIdPathPair (*vf_0x200)(UnkShootActivator_l89091c* self);
};

struct UnkShootActivator_l89091c {
    UnkShootActivatorVtable_l89091c* vtable;  // 0x0
    u8 pad_0x8[0x8];
    UnkReflObject* owner;  // 0x10
    u8 pad_0x18[0x9];
    bool field_0x21;
    u8 pad_0x22[0x42e];
    UnkStrIdPathResource sound;  // 0x450
    u8 pad_0x468[0x48];
    u32 field_0x4b0;
    u32 field_0x4b4;
    u32 field_0x4b8;
    u32 field_0x4bc;
    UnkShootCharClass_l89091c* char_class;  // 0x4c0
    u32 field_0x4c8;
    u32 field_0x4cc;
};
static_assert(__builtin_offsetof(UnkShootActivator_l89091c, sound) == 0x450, "layout");
static_assert(__builtin_offsetof(UnkShootActivator_l89091c, field_0x4c8) == 0x4c8, "layout");

bool fn_710089091c(UnkShootActivator_l89091c* self) {
    bool result = fn_71008e0804(reinterpret_cast<char*>(self));

    UnkShootCharClass_l89091c* char_class = nullptr;
    if (UnkReflObject* owner = self->owner) {
        const base::reflection::CClass* type = owner->vf_0x0();
        // The class getter runs for its registration; the class itself is compared.
        fn_710088daec();
        if (fn_710009d50c(type, &g_unk_717272d0c8))
            char_class = static_cast<UnkShootCharClass_l89091c*>(owner);
    }
    self->char_class = char_class;
    if (self->field_0x21) {
        self->field_0x4b0 = char_class->field_0x11c;
        self->field_0x4b4 = char_class->field_0x120;
    }
    self->field_0x4bc = self->field_0x4b8;

    fn_71000267f8(&self->sound, self->vtable->vf_0x200(self));

    if (self->sound.ids.id.data != &g_unk_7101d47108->empty &&
        self->sound.ids.path.data != &g_unk_7101d47200->empty) {
        if (UnkReflObject* resource = reinterpret_cast<UnkReflObject*>(self->sound.resource)) {
            if (fn_710009d50c(resource->vf_0x0(), fn_710102df18())) {
                UnkAudioPreset_l89091c* preset = static_cast<UnkAudioPreset_l89091c*>(resource);
                self->field_0x4c8 = preset->field_0x590;
                self->field_0x4cc = preset->field_0x588;
            }
        }
    }
    return result;
}
