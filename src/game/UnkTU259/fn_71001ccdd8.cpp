// Unit 259 (the graphics system's unit): binds the shadow-map textures of up to four maps in pairs
// (a map of kind 0 and the kind-1 map of the same id share two consecutive slots), unbinds the
// slots left over, then binds the BRDF lookup table. Names and layouts are placeholders.
#include "base/UnkParallelFind.h"
#include "base/global/CStrId.h"
#include "engine/UnkGfxPairEntry.h"
#include "game/UnkTU182.h"
#include "types.h"

struct UnkShadowMap_l1ccdd8 {
    u8 pad_0x0[0x8c];
    u32 id;    // 0x8c
    u32 kind;  // 0x90
    u8 pad_0x94[0xa8 - 0x94];
    void* texture;  // 0xa8
};

struct UnkTextureMap_l1ccdd8 {
    UnkGfxPairEntry* values;
    base::global::CStrId* keys;
    u32 count;
};

struct UnkGfx_l1ccdd8 {
    u8 pad_0x0[0xd8];
    UnkShadowMap_l1ccdd8* maps[4];  // 0xd8
    u32 map_count;                  // 0xf8
    u8 pad_0xfc[0x3183628 - 0xfc];
    UnkTextureMap_l1ccdd8 textures;  // 0x3183628
};
static_assert(__builtin_offsetof(UnkGfx_l1ccdd8, textures) == 0x3183628, "");

// Binds map `index`'s texture to a sampler slot with the shadow sampling state.
static inline void BindShadowTexture(UnkGfx_l1ccdd8* self, u32 slot, u32 index) {
    fn_71001606f0(slot, true, 1);
    SetGraphicsSlotPointer(slot, self->maps[index]->texture, 1);
    fn_7100160810(slot, 0, 1);
    fn_7100160890(slot, 0, 1);
    fn_7100160910(slot, 0, 1);
    fn_7100160990(slot, 2, 1);
    fn_7100160ab0(slot, 2, 1);
    fn_7100160d50(slot, 2, 1);
}

static inline void UnbindTexture(u32 slot) {
    fn_71001606f0(slot, false, 1);
    SetGraphicsSlotPointer(slot, nullptr, 1);
}

void fn_71001ccdd8(UnkGfx_l1ccdd8* self) {
    bool bound[4] = {};
    u32 slot = 0xc;
    for (u32 i = 0; i < self->map_count; i++) {
        if (bound[i])
            continue;
        UnkShadowMap_l1ccdd8* map = self->maps[i];
        u32 id = map->id;
        if (map->kind == 0) {
            BindShadowTexture(self, slot, i);
            for (u32 j = i + 1; j < self->map_count; j++) {
                if (bound[j])
                    continue;
                UnkShadowMap_l1ccdd8* other = self->maps[j];
                if (other->id == id && other->kind == 1) {
                    BindShadowTexture(self, slot + 1, j);
                    bound[j] = true;
                }
            }
        } else if (map->kind == 1) {
            BindShadowTexture(self, slot + 1, i);
            for (u32 j = i + 1; j < self->map_count; j++) {
                if (bound[j])
                    continue;
                UnkShadowMap_l1ccdd8* other = self->maps[j];
                if (other->id == id && other->kind == 0) {
                    BindShadowTexture(self, slot, j);
                    bound[j] = true;
                }
            }
        }
        slot += 2;
        bound[i] = true;
    }
    for (u32 i = 0; i < 4; i++) {
        if (!bound[i])
            UnbindTexture(slot + i);
    }

    fn_71001606f0(7, true, 1);
    {
        base::global::CStrId name("BRDF_LUT", true);
        UnkGfxPairEntry* entry = UnkParallelFindSlot(&self->textures, name);
        void* lut = entry != &self->textures.values[self->textures.count] ? entry->second : nullptr;
        SetGraphicsSlotPointer(7, lut, 1);
    }
    fn_7100160810(7, 0, 1);
    fn_7100160890(7, 0, 1);
    fn_7100160910(7, 0, 1);
    fn_7100160990(7, 2, 1);
    fn_7100160ab0(7, 2, 1);
    fn_7100160d50(7, 1, 1);
}
