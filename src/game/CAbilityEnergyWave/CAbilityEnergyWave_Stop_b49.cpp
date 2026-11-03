// CAbilityEnergyWave (tools/tu_map.py): stops the wave. Unless the game manager's flag at +0x1af0 is
// set, releases the object at +0x68 (its slot 0x90), detaches the owner's (+0x30) two components
// (+0x260 with 0, +0x210 with that object) and refreshes the items (fn_71002fda50 with true, false);
// then calls slot 0x78 on every child (+0x40, count +0x48), clears +0xcc, sets +0xd8 to -1 and, when
// the world behind the manager's +0x29a0 (+0x10, +0x280) has the object with hash 0xb1489dc93c1d38dd,
// releases it (fn_710114fe8c, as in fn_7100300bd0.cpp). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkStruct_71002fda50;
struct UnkWorld;
struct UnkPart;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;

void fn_71002fda50(UnkStruct_71002fda50* self, bool all, bool force_off);
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);
void fn_710113f270(UnkEnergyWaveCompA_b49* comp, void* object);

struct UnkEnergyWaveObject_b49 {
    void** vtable;
    void vf_0x90() { reinterpret_cast<void (*)(UnkEnergyWaveObject_b49*)>(vtable[0x90 / 8])(this); }
};
void fn_710094f8f0(UnkEnergyWaveCompB_b49* comp, UnkEnergyWaveObject_b49* object);

struct UnkEnergyWaveChild_b49 {
    void** vtable;
    void vf_0x78() { reinterpret_cast<void (*)(UnkEnergyWaveChild_b49*)>(vtable[0x78 / 8])(this); }
};

struct UnkEnergyWaveOwner_b49 {
    u8 pad_0x0[0x210];
    UnkEnergyWaveCompB_b49* field_0x210;
    u8 pad_0x218[0x48];
    UnkEnergyWaveCompA_b49* field_0x260;
};

struct UnkEnergyWave_b49 {
    u8 pad_0x0[0x30];
    UnkEnergyWaveOwner_b49* owner;
    u8 pad_0x38[0x8];
    UnkEnergyWaveChild_b49** children;
    u32 count;
    u8 pad_0x4c[0x1c];
    UnkEnergyWaveObject_b49* object;
    u8 pad_0x70[0x5c];
    u32 field_0xcc;
    u8 pad_0xd0[0x8];
    f32 field_0xd8;
};

struct UnkWorldHolder_b49 {
    u8 pad_0x0[0x280];
    UnkWorld* world;
};
struct UnkWorldLink_b49 {
    u8 pad_0x0[0x10];
    UnkWorldHolder_b49* holder;
};

void fn_71002fdfac(UnkEnergyWave_b49* self) {
    if (!reinterpret_cast<u8*>(g_unk_7101d47310)[0x1af0]) {
        if (self->object) {
            self->object->vf_0x90();
            self->object = nullptr;
        }
        if (self->owner->field_0x260)
            fn_710113f270(self->owner->field_0x260, nullptr);
        if (self->owner->field_0x210)
            fn_710094f8f0(self->owner->field_0x210, self->object);
        fn_71002fda50(reinterpret_cast<UnkStruct_71002fda50*>(self), true, false);
    }
    for (UnkEnergyWaveChild_b49 **it = self->children, **end = it + self->count; it != end; ++it)
        (*it)->vf_0x78();
    self->field_0xcc = 0;
    self->field_0xd8 = -1.0f;
    if (UnkWorldLink_b49* link = *reinterpret_cast<UnkWorldLink_b49**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0)) {
        if (UnkWorldHolder_b49* holder = link->holder) {
            if (UnkWorld* world = holder->world) {
                if (fn_7101152b90(world, 0xb1489dc93c1d38ddull))
                    fn_710114fe8c(world, 0xb1489dc93c1d38ddull, 0, 1);
            }
        }
    }
}
