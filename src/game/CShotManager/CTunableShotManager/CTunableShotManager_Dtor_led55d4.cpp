// CShotManager::CTunableShotManager's unit (tools/tu_map.py, unit 2576): the shot manager's destructor. It
// hands its lights back to the light manager, frees the render instances and models of every shot type
// and variant, resets the audio presets and then destroys its members. Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntVector.h"
#include "game/CLightManager.h"
#include "game/CShotManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::global::CRntVector;

extern void* g_unk_7101caa0d8[];
void fn_7100e0bc40(char* self, UnkListedObj* obj);

// A shot (0x290 bytes), destroyed through its vtable.
struct UnkShot_led55d4 {
    virtual ~UnkShot_led55d4();
    u8 pad_0x8[0x288];
};

struct UnkShotEffect_led55d4 {
    u8 pad_0x0[0xfc];
};

struct UnkShotWeakRef_led55d4 {
    void* target;
    s64 generation;
};

// A model of a shot type and variant.
struct UnkShotModel_led55d4 {
    u64 field_0x0;
};
void fn_7100ecf944(UnkShotModel_led55d4* self);  // Destructor.

// A render item the render manager holds; the part it knows starts at +0x8.
struct UnkShotItem_led55d4 {
    u64 field_0x0;
    u64 item;
};

// The preset names (string ids) and the resolved presets, by kind and preset.
struct UnkShotPresetNames_led55d4 {
    u8 data[0x3c0];
};
struct UnkShotPresets_led55d4 {
    UnkStrIdPathResource presets[15][8];
};
void fn_7100edaa68(UnkShotPresetNames_led55d4* self);  // Destructor.
void fn_7100ed5950(UnkShotPresets_led55d4* self);      // Destructor.

// A reference-counted object, deleted through vf_0x10 when the last reference goes.
struct UnkShotShared_led55d4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 refs;
};

struct UnkShotManager_led55d4 {
    void** vtable;
    CRntVector<UnkShot_led55d4> field_0x8;
    CRntVector<UnkShotEffect_led55d4> field_0x30;
    CRntVector<UnkShot_led55d4*> field_0x58;
    u32 field_0x80;
    u8 pad_0x84[0x4];
    UnkShotModel_led55d4 field_0x88[5][2];
    u32 field_0xd8[5][2];
    UnkShotItem_led55d4* field_0x100[5][2];
    UnkShotItem_led55d4* field_0x150[5][2];
    u64* field_0x1a0[5][2][2];
    void* field_0x240[5][5];
    UnkShotPresetNames_led55d4 field_0x308;
    UnkShotPresets_led55d4 field_0x6c8;
    f32 field_0x1208;
    u8 pad_0x120c[0x4];
    CRntVector<UnkShotWeakRef_led55d4> field_0x1210;
    CRntVector<UnkListedObj*> field_0x1238;  // the shot lights
    u32 field_0x1260;
    u8 pad_0x1264[0x4];
    UnkShotShared_led55d4* field_0x1268;
};
static_assert(__builtin_offsetof(UnkShotManager_led55d4, field_0x308) == 0x308, "placeholder layout");
static_assert(__builtin_offsetof(UnkShotManager_led55d4, field_0x1210) == 0x1210, "placeholder layout");
static_assert(__builtin_offsetof(UnkShotManager_led55d4, field_0x1268) == 0x1268, "placeholder layout");

static inline char* LightManager() {
    if (!g_unk_71726b1688) {
        void* p = fn_71000839a0(0x198, 0x3f);
        fn_7100359118(p);
        g_unk_71726b1688 = p;
    }
    return static_cast<char*>(g_unk_71726b1688);
}

static inline void RemoveItem(UnkShotItem_led55d4* item) {
    if (item)
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(&item->item), 0);
}

void fn_7100ed55d4(UnkShotManager_led55d4* self) {
    self->vtable = g_unk_7101caa0d8 + 2;

    u32 n = self->field_0x1238.capacity;
    for (u32 i = 0; i < n; ++i) {
        UnkPtrList* list = reinterpret_cast<UnkPtrList*>(&self->field_0x1238);
        fn_7100e0bc40(LightManager(), *fn_7100e0a5ec(list, i));
        *fn_7100e0a5ec(list, i) = nullptr;
    }
    self->field_0x1260 = 0;

    for (u32 i = 0; i < 5; ++i) {
        for (u32 j = 0; j < 2; ++j) {
            delete[] self->field_0x1a0[i][j][0];
            delete[] self->field_0x1a0[i][j][1];
            RemoveItem(self->field_0x100[i][j]);
            RemoveItem(self->field_0x150[i][j]);
        }
    }

    for (u32 k = 0; k < 15; ++k)
        for (u32 j = 0; j < 8; ++j)
            fn_7100027044(&self->field_0x6c8.presets[g_unk_710164bec0[k]][j]);

    if (UnkShotShared_led55d4* shared = self->field_0x1268) {
        if (--shared->refs == 0)
            shared->vf_0x10();
    }
    self->field_0x1238.Free();
    self->field_0x1210.Free();
    fn_7100ed5950(&self->field_0x6c8);
    fn_7100edaa68(&self->field_0x308);
    // The models, last first, as an array is destroyed.
    UnkShotModel_led55d4* models = &self->field_0x88[0][0];
    for (UnkShotModel_led55d4* p = models + 10; p != models;)
        fn_7100ecf944(--p);
    self->field_0x58.Free();
    self->field_0x30.Free();
    self->field_0x8.Free();
}
