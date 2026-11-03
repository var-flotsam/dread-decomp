// GUI::CGlobalMapComposition (unit 1680 of tools/tu_map.py, the name from reflection): two identical
// handlers that, when the global sound holder (created on first use) has a group, the composition
// has its object at +0x5f0 and the group accepts the request (virtual slot 0x30 with 0x2000, 0xfd),
// pass that object to fn_7100b26470. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkSoundHolder;
extern UnkSoundHolder* g_unk_7101d47340;
void fn_710006cd9c(UnkSoundHolder*);

struct UnkMapSoundGroup {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool vf_0x30(u32 flags, u32 id);
};
struct UnkMapSoundHolder {
    UnkMapSoundGroup* group;
};

struct UnkGlobalMapComposition {
    u8 unk_0x0[0x5f0];
    void* field_0x5f0;
};
void fn_7100b26470(UnkGlobalMapComposition* self, void* object, bool flag);

__attribute__((always_inline)) static inline UnkMapSoundHolder* Holder() {
    UnkSoundHolder* holder = g_unk_7101d47340;
    if (!holder) {
        holder = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(holder);
        g_unk_7101d47340 = holder;
    }
    return reinterpret_cast<UnkMapSoundHolder*>(holder);
}

void fn_7100b2b868(UnkGlobalMapComposition* self) {
    UnkMapSoundGroup* group = Holder()->group;
    if (group && self->field_0x5f0 && group->vf_0x30(0x2000, 0xfd))
        fn_7100b26470(self, self->field_0x5f0, true);
}

void fn_7100b2b93c(UnkGlobalMapComposition* self) {
    UnkMapSoundGroup* group = Holder()->group;
    if (group && self->field_0x5f0 && group->vf_0x30(0x2000, 0xfd))
        fn_7100b26470(self, self->field_0x5f0, true);
}
