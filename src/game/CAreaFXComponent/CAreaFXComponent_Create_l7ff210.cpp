// CAreaFXComponent's unit: creates the owner's effect entry through the entity manager, checks that the
// resulting object is of kind 4 and stores a four-float value in it. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

__attribute__((visibility("hidden"))) extern char g_unk_7172721528[];

struct UnkFXObject;
struct UnkFXManager;

struct UnkFXEntry {
    u8 pad_0x0[0x158];
    UnkFXObject* object;  // 0x158
};

struct UnkFXObjectVt {
    void* pad_0x0[3];
    int (*GetKind)(UnkFXObject*);  // 0x18
    void* pad_0x20[12];
    void (*Apply)(UnkFXObject*);  // 0x80
};

struct UnkFXObject {
    const UnkFXObjectVt* vt;
    u8 pad_0x8[0xa0];
    f32 value[4];  // 0xa8
};

struct UnkFXManagerVt {
    void* pad_0x0[45];
    UnkFXEntry* (*Create)(UnkFXManager*, const void* a, const void* b, const base::global::CStrId* id,
                          const void* table, int c, int d, int e, u64 f, int g, void* h, int i, u64 j, int k,
                          bool l, bool m, bool n);  // 0x168
};

struct UnkFXManager {
    const UnkFXManagerVt* vt;
};

struct UnkFXOwner {
    u8 pad_0x0[0x2a0];
    UnkFXManager* manager;  // 0x2a0
};

struct UnkFXComponentSelf {
    u8 pad_0x0[0x30];
    UnkFXOwner* owner;  // 0x30
};

void fn_71007ff210(UnkFXComponentSelf* self, const f32* color) {
    UnkFXManager* manager = self->owner->manager;
    UnkGameManagerIdTable& table = GameManagerIdTable();
    UnkFXEntry* entry = manager->vt->Create(manager, g_unk_7172721528, g_unk_7172721528 + 8, &g_unk_7101d470e0,
                                            &table.field_0x628, 0x80, 0, 1, 0, -1, nullptr, -1, 0, -1,
                                            false, false, true);
    UnkFXObject* object = entry->object;
    if (object->vt->GetKind(object) != 4)
        object = nullptr;
    object->value[0] = color[0];
    object->value[1] = color[1];
    object->value[2] = color[2];
    object->value[3] = color[3];
    object->vt->Apply(object);
}
