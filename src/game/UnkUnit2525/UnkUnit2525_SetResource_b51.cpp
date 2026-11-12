// Unit 2525 (tools/tu_map.py; no reflected class): three setters of one object (the environment visuals
// of src/game/UnkEnvironmentVisuals.cpp, fn_7100d8e398 there), each for its own kind
// of resource. Each looks the id `key` up in its table (fn_7100f9c250 on +0x0, fn_7100f94a74 on +0x8,
// fn_7100f89818 on +0x10) unless it is already the current one and `force` is clear (the second and
// third also need a resource loaded); the result is kept pending while the kind's flag (+0x4a4..+0x4a6)
// is set, else applied (fn_7100d8e398, fn_7100d8db30, fn_7100d8d854). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkEnvironmentManager;  // as src/game/CSubAreaManager.cpp declares the three setters

struct UnkUnit2525_b51 {
    void* table_a;  // 0x0
    void* table_b;  // 0x8
    void* table_c;  // 0x10
    UnkStringData* current_a;  // 0x18
    u8 unk_0x20[0x478 - 0x20];
    UnkStringData* current_b;  // 0x478
    void* resource_b;  // 0x480
    UnkStringData* current_c;  // 0x488
    void* resource_c;  // 0x490
    u8 unk_0x498[0x4a4 - 0x498];
    bool deferred_a;  // 0x4a4
    bool deferred_c;  // 0x4a5
    bool deferred_b;  // 0x4a6
    u8 unk_0x4a7;
    void* pending_a;  // 0x4a8
    void* pending_b;  // 0x4b0
    void* pending_c;  // 0x4b8
};

void* fn_7100f9c250(void* table, const CStrId* key);
void* fn_7100f94a74(void* table, const CStrId* key);
void* fn_7100f89818(void* table, const CStrId* key);
struct UnkEnvironmentVisuals;  // src/game/UnkEnvironmentVisuals.cpp
struct UnkEnvironmentData;

void fn_7100d8dac0(UnkEnvironmentManager* manager, const CStrId* key, bool force, bool) {
    UnkUnit2525_b51* self = reinterpret_cast<UnkUnit2525_b51*>(manager);
    if (self->resource_b && self->current_b == key->data && !force)
        return;
    void* resource = fn_7100f94a74(self->table_b, key);
    if (self->deferred_b) {
        self->pending_b = resource;
        return;
    }
    fn_7100d8db30(self, resource, false);
}

void fn_7100d8df84(UnkEnvironmentManager* manager, const CStrId* key, bool force, bool) {
    UnkUnit2525_b51* self = reinterpret_cast<UnkUnit2525_b51*>(manager);
    if (self->resource_c && self->current_c == key->data && !force)
        return;
    void* resource = fn_7100f89818(self->table_c, key);
    if (self->deferred_c) {
        self->pending_c = resource;
        return;
    }
    fn_7100d8d854(self, resource);
}

void fn_7100d8e32c(UnkEnvironmentManager* manager, const CStrId* key, bool force, bool flag) {
    UnkUnit2525_b51* self = reinterpret_cast<UnkUnit2525_b51*>(manager);
    if (self->current_a == key->data && !force)
        return;
    void* resource = fn_7100f9c250(self->table_a, key);
    if (self->deferred_a) {
        self->pending_a = resource;
        return;
    }
    fn_7100d8e398(reinterpret_cast<UnkEnvironmentVisuals*>(self), static_cast<const UnkEnvironmentData*>(resource),
                  flag);
}
