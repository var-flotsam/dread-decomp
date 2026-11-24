// Unit 2726 (gui/scripts/msemenu_ingame.lua): creates the in-game menu's two render items on layer 2 from the
// texture paths for the current language (kept in a unit static), then hides them and gives each the same scale,
// sort order, position and colour. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU223.h"
#include "game/UnkTU2733.h"
#include "game/UnkTU287.h"
#include "game/UnkTU441.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkMinimapVec3;
struct UnkStruct_710020a5a0;

// The two render items the in-game menu shows on layer 2.
__attribute__((visibility("hidden"))) extern UnkItemBase_71011a91c4* g_unk_7172a0b8f0[2];
// The language the items were made for.
__attribute__((visibility("hidden"))) extern s32 g_unk_7101cb51b0;
// The two items' texture paths ([item][language group]).
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_7172a0b8a0[2][4];

struct UnkMenuQuadRender_l186a80 {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

// A render item as this function sees it: its scene node at +0x18 and its render part.
struct UnkMenuQuad_l186a80 {
    u8 pad_0x0[0x18];
    u8 node[0x160];                      // 0x18
    UnkMenuQuadRender_l186a80* render;  // 0x178
};

static inline UnkMenuQuad_l186a80* Quad(UnkItemBase_71011a91c4* item) {
    return reinterpret_cast<UnkMenuQuad_l186a80*>(item);
}

static inline UnkLanguage* Language() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

static inline UnkItemBase_71011a91c4* Create(const CFilePathStrId* path) {
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkQuadNode_ovb0259* created = nullptr;
    fn_710035d9e0(manager->field_0x218, path, &created, &g_unk_7101d470e0);
    fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(created), 2);
    return reinterpret_cast<UnkItemBase_71011a91c4*>(created);
}

// Creates both items from a language's path for the first; the second's is the same entry of the next row.
static inline void CreateItems(const CFilePathStrId* path) {
    g_unk_7172a0b8f0[0] = Create(path);
    g_unk_7172a0b8f0[1] = Create(path + 4);
}

// Hides an item and places it.
static inline void Place(UnkItemBase_71011a91c4*& item, const CVector2D* scale) {
    fn_710020ac34(reinterpret_cast<UnkToggle*>(Quad(item)->node), false);
    SetNode2DScale(reinterpret_cast<UnkStruct_710020a5a0*>(Quad(item)->node), scale);
    fn_710020ad90(Quad(item)->node, -2004);
    CVector3D pos = {0.0f, -0.25f, 0.0f};
    fn_710020a4c4(Quad(item)->node, reinterpret_cast<const UnkMinimapVec3&>(pos));
    fn_710019c564(Quad(item)->render->field_0x78, 1.0f, 1.0f, 1.0f, 0.0f);
}

void fn_7101186a80() {
    g_unk_7101cb51b0 = fn_710119c9f4(Language());
    switch (g_unk_7101cb51b0) {
    case 2:
    case 8:
        CreateItems(&g_unk_7172a0b8a0[0][1]);
        break;
    case 9:
        CreateItems(&g_unk_7172a0b8a0[0][2]);
        break;
    case 14:
        CreateItems(&g_unk_7172a0b8a0[0][3]);
        break;
    default:
        CreateItems(&g_unk_7172a0b8a0[0][0]);
        break;
    }

    CVector2D scale = {0.64f, 0.16f};
    Place(g_unk_7172a0b8f0[0], &scale);
    Place(g_unk_7172a0b8f0[1], &scale);
}
