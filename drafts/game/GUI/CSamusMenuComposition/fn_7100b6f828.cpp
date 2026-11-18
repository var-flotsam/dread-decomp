// GUI::CSamusMenuComposition's unit (1714): the per-frame update. After the container's update, the colour
// set registered under the focused object's name is applied to the form sprite's material; and when the
// description timer runs out, the power description composition is linked and sent event 0x11.
// Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CTutoEnterLogicAction.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;
using base::reflection::UnkReflObject;

extern const CVector4D g_unk_71016477b4;  // (0, 0, 0, 0)
__attribute__((visibility("hidden"))) extern CStrId g_unk_7172765d08;

// The colours of a focused object's entry: a mode and the colour for one slot.
struct UnkSamusMenuColors_lb6f828 {
    u32 mode;
    u32 slot;
    CVector4D color;
};

struct UnkSamusMenuMaterial_lb6f828 {
    u8 pad_0x0[0x158];
    u32 mode;              // 0x158
    CVector4D colors[2];  // 0x15c
};

struct UnkSamusMenuMaterialRef_lb6f828 {
    u8 pad_0x0[0x78];
    UnkSamusMenuMaterial_lb6f828* material;  // 0x78
};

struct UnkSamusMenuWidget_lb6f828 {
    u8 pad_0x0[0x178];
    UnkSamusMenuMaterialRef_lb6f828* ref;  // 0x178

    UnkSamusMenuMaterial_lb6f828* Material() { return ref->material; }
};

struct UnkSamusMenuSprite_lb6f828 {
    u8 pad_0x0[0x190];
    UnkSamusMenuWidget_lb6f828* widget;  // 0x190
};

struct UnkSamusMenuListener_lb6f828 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void Send(UnkGuiEvent* event);  // vf_0xf0
};

struct UnkSamusMenuRef_lb6f828 {
    u8 pad_0x0[0x10];
    UnkReflObject* target;  // 0x10
};

struct UnkSamusMenuIngame_lb6f828 {
    u8 pad_0x0[0x488];
    UnkSamusMenuRef_lb6f828* description;  // 0x488
};

// A link to an object's listener set: registered with it while set (fn_7100a58e80), unregistered
// before it changes (fn_71004567dc).
struct UnkSamusMenuDescription_lb6f828 {
    u8 pad_0x0[0x230];
    u8 listeners[8];  // 0x230
};

struct UnkSamusMenuLink_lb6f828 {
    void* target;

    void Set(UnkSamusMenuDescription_lb6f828* description) {
        if (target) {
            fn_71004567dc(target, &target);
            target = nullptr;
        }
        target = description->listeners;
        fn_7100a58e80(target, &target);
    }
};

struct UnkSamusMenuComposition_lb6f828 {
    u8 pad_0x0[0x288];
    base::global::CRntDictionary<CStrId, UnkSamusMenuColors_lb6f828> colors;  // 0x288
    CStrId focused_name;                       // 0x2d0
    UnkSamusMenuSprite_lb6f828* form_sprite;  // 0x2d8
    UnkSamusMenuLink_lb6f828 description_link;  // 0x2e0
    u8 pad_0x2e8[0x18];
    UnkSamusMenuListener_lb6f828* listener;  // 0x300
    f32 description_timer;                    // 0x308
};
static_assert(__builtin_offsetof(UnkSamusMenuComposition_lb6f828, focused_name) == 0x2d0, "layout");
static_assert(__builtin_offsetof(UnkSamusMenuComposition_lb6f828, description_timer) == 0x308, "layout");

void fn_7100b6f828(UnkSamusMenuComposition_lb6f828* self, f32 dt) {
    fn_71011d8fe4(reinterpret_cast<UnkNewGameScreen*>(self), dt);

    if (self->colors.FindNode(self->focused_name)) {
        const UnkSamusMenuColors_lb6f828* colors = &self->colors.FindNode(self->focused_name)->value;
        if (UnkSamusMenuSprite_lb6f828* sprite = self->form_sprite) {
            UnkSamusMenuWidget_lb6f828* widget = sprite->widget;
            fn_710007b518(&widget->Material()->colors[0], &g_unk_71016477b4);
            fn_710007b518(&widget->Material()->colors[1], &g_unk_71016477b4);
            widget->Material()->mode = colors->mode;
            fn_710007b518(&widget->Material()->colors[colors->slot], &colors->color);
            UnkSamusMenuMaterial_lb6f828* material = widget->Material();
            base::math::UnkVector4D color(material->colors[1]);
            color.z = 2.8f;
            fn_710007b518(&material->colors[1], &color);
        }
    }

    if (self->description_timer > -1.0f) {
        f32 t = self->description_timer - dt;
        self->description_timer = t;
        if (t <= 0.0f) {
            self->description_timer = -1.0f;
            fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172765d08);
            auto* ingame = reinterpret_cast<UnkSamusMenuIngame_lb6f828*>(fn_7100deac84(g_unk_7101d47310));
            UnkReflObject* description = ingame->description ? ingame->description->target : nullptr;
            if (description && !fn_710009d50c(description->vf_0x0(),
                                              reinterpret_cast<const base::reflection::CType*>(fn_7100b6343c())))
                description = nullptr;
            self->description_link.Set(reinterpret_cast<UnkSamusMenuDescription_lb6f828*>(description));
            UnkGuiEvent event;
            s32 kind = 0x11;
            fn_710120235c(&event, reinterpret_cast<UnkGuiDisplayObject*>(self), &kind, nullptr);
            self->listener->Send(&event);
            fn_7101202488(&event);
        }
    }
}
