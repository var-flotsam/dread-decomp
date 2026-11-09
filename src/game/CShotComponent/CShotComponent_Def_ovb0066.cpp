// CShotComponent's unit (tools/tu_map.py unit 1508): the component's class definition (a
// CCharClassShotComponent, cached at +0x40 with a callback at +0x50) and the dispatch of a shot
// request to its entries. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

char* fn_71009f9808(char* p0, char* p1, char* p2, char* p3);

struct UnkShotEntry_ovb0066;

struct UnkShotEntryVtable_ovb0066 {
    void* vf_0x0;
    void* vf_0x8;
    void* vf_0x10;
    bool (*vf_0x18)(UnkShotEntry_ovb0066* self, void* a, void* b, void* c, bool d, bool e, f32 g, void* f);
};

struct UnkShotEntry_ovb0066 {
    const UnkShotEntryVtable_ovb0066* vtable;

    bool Fire(void* a, void* b, void* c, bool d, bool e, void* f, f32 g) {
        auto fire = vtable->vf_0x18;
        return fire(this, a, b, c, d, e, g, f);
    }
};

struct UnkShotDef_ovb0066 {
    u8 unk_0x0[0x58];
    UnkShotEntry_ovb0066** entries;  // 0x58
    u32 count;                       // 0x60
};

struct UnkShotComponent_ovb0066 : UnkBaseComponent {
    UnkShotDef_ovb0066* def;  // 0x40
    u64 field_0x48;
    char* (*callback)(char*, char*, char*, char*);  // 0x50

    UnkReflObject* Owner() const { return *reinterpret_cast<UnkReflObject* const*>(unk_0x8 + 0x8); }
};

bool fn_71009f922c(UnkShotComponent_ovb0066* self, void* arg) {
    if (!fn_71010c41bc(self, arg))
        return false;
    UnkReflObject* owner = self->Owner();
    UnkShotDef_ovb0066* def = nullptr;
    if (owner && fn_710009d50c(owner->vf_0x0(), fn_71009f87dc()))
        def = reinterpret_cast<UnkShotDef_ovb0066*>(owner);
    self->def = def;
    if (!def)
        return false;
    self->field_0x48 = 0;
    self->callback = fn_71009f9808;
    return true;
}

struct UnkShotKeys_ovb0066 {
    u64* keys;
    u32 count;
};

bool fn_71009f8d1c(UnkShotDef_ovb0066* def, const u64* key, UnkShotEntry_ovb0066** out);

bool fn_71009f9588(UnkShotComponent_ovb0066* self, void* a, void* b, void* c, bool d, bool e,
                   const UnkShotKeys_ovb0066* keys, f32 g, void* f) {
    if (!self->def)
        return false;
    if (keys->count) {
        for (const u64 *key = keys->keys, *end = key + keys->count; key != end; ++key) {
            UnkShotEntry_ovb0066* entry = nullptr;
            if (fn_71009f8d1c(self->def, key, &entry) && entry && entry->Fire(a, b, c, d, e, f, g))
                return true;
        }
        return false;
    }
    UnkShotDef_ovb0066* def = self->def;
    for (UnkShotEntry_ovb0066 **it = def->entries, **end = it + def->count; it != end; ++it) {
        if (*it && (*it)->Fire(a, b, c, d, e, f, g))
            return true;
    }
    return false;
}
