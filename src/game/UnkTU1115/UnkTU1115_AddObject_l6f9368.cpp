// Unit 1115 (the registry of objects, as src/game/UnkTU1115/UnkTU1115_Registry_ovb0255.cpp): creates an
// object with the given name and id, appends it to the registry's objects and files it by id in the
// index. Names and layouts are placeholders.
#include <new>

#include "UnkLeftoverReg.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkQueryTag_b50.h"
#include "game/UnkTU1115.h"
#include "types.h"

extern u32 g_unk_7101a6b7f0;

struct UnkQueryEntry_b50;
void fn_71006fb514(void* map, const u32* key, UnkQueryEntry_b50* const* entry);

struct UnkRegistry_l6f9368;

// An object of the registry.
struct UnkRegObj_l6f9368 {
    void* vtable;                                       // 0x0
    u32 id;                                             // 0x8
    u32 field_0xc;                                      // 0xc
    base::global::CStrId name;                          // 0x10
    base::global::CRntVector<u32> ids;                  // 0x18
    base::global::CRntVector<u64> names;                // 0x40
    UnkRegistry_l6f9368* registry;                      // 0x68

    UnkRegObj_l6f9368(UnkRegistry_l6f9368* owner, const base::global::CStrId& name, u32 id, u32 field_0xc)
        : vtable(reinterpret_cast<char*>(g_unk_7101a6b7f8) + 0x10), id(id), field_0xc(field_0xc),
          name(name), ids(0xf), names(0xf), registry(owner) {}
};
static_assert(__builtin_offsetof(UnkRegObj_l6f9368, registry) == 0x68, "layout");

struct UnkRegistry_l6f9368 {
    u8 pad_0x0[0x40];
    u8 index[0x28];                                          // 0x40
    base::global::CRntVector<UnkRegObj_l6f9368*> objects;  // 0x68
};

// The holder and tag as the unit's other file declares this function.
struct UnkQueryHolder_b50;

void fn_71006f9368(UnkQueryHolder_b50* holder, UnkQueryTag_b50 tag, void* data, u32 mode) {
    auto* self = reinterpret_cast<UnkRegistry_l6f9368*>(holder);
    const auto* name = static_cast<const base::global::CStrId*>(data);
    u32 id = mode;
    UnkRegObj_l6f9368* obj = new (fn_71000839a0(sizeof(UnkRegObj_l6f9368), 4)) 
        UnkRegObj_l6f9368(self, *name, id, g_unk_7101a6b7f0);
    self->objects.PushBack(obj);
    fn_71006fb514(self->index, &id, reinterpret_cast<UnkQueryEntry_b50* const*>(&obj));
}
