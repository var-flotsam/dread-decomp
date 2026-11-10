// Unit 46 of tools/tu_map.py: registers the controller buttons the owner reports for an id (0xfd meaning the current
// one at +8) in that id's binding entry of the flat map, adding the entry when missing. Names and layouts
// are placeholders.
#include <new>
#include "base/UnkVector.h"
#include "types.h"

struct UnkSoundGroup;
extern const u32 g_unk_7101647694[22];

struct UnkBindingMap_b0232 {
    base::UnkVector<u32>* values;  // 0x0
    u32* keys;  // 0x8
    u32 count;  // 0x10
    u32 capacity;  // 0x14
    u32 step;  // 0x18

    base::UnkVector<u32>* Find(u32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

void fn_710006d88c(UnkBindingMap_b0232* map, u32 capacity);

struct UnkInputOwner_b0232 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30(u32 button, u32 id);
    u8 pad_0x8[0x10];
    UnkBindingMap_b0232 map;  // 0x18
};

void fn_710006d2a0(UnkSoundGroup* self_, u32 id) {
    UnkInputOwner_b0232* self = reinterpret_cast<UnkInputOwner_b0232*>(self_);
    if (id == 0xfd)
        id = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 8);
    base::UnkVector<u32>* entry = self->map.Find(id);
    if (entry == self->map.values + self->map.count) {
        if (self->map.count == self->map.capacity)
            fn_710006d88c(&self->map, self->map.step + self->map.count);
        __builtin_memcpy(&self->map.keys[self->map.count], &id, 4);
        new (&self->map.values[self->map.count]) base::UnkVector<u32>(0xf);
        u32 n = self->map.count + 1;
        base::UnkVector<u32>* end = &self->map.values[n];
        entry = n ? end - 1 : &self->map.values[0];
        self->map.count = n;
    }
    for (u32 i = 0; i < 22; ++i) {
        u32 button = g_unk_7101647694[i];
        if (self->vf_0x30(button, id))
            entry->PushBack(button);
    }
}
