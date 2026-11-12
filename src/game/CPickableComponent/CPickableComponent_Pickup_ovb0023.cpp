// CPickableComponent's unit (tools/tu_map.py). fn_71009cd46c: sets the alpha of the owner's model
// tint (slot from the id table's +0x16d8 entry, read then written back, fn_71001c2bac and
// fn_71001c28f8). fn_71009cd2c4: reads the two string ids at +0x40 and +0x48 after the base step
// (fn_710024fc08) while the stream has data. fn_71009cd8e0, the update: unless the player is busy
// with input, picks up (vf_0x180) when fn_71009cd9e0 allows it, else counts the timer at +0x50 down,
// fading out under 1 and removing the owner at 0; then fades the linked object (+0x60, checked
// against the id at +0x68) to 0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSlots.h"
#include "game/UnkTU101.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU56.h"
#include "types.h"

struct UnkPlayer;

using base::color::CColor4F;

struct UnkPickableColorKey_ovb0023 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkPickableModel_ovb0023 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkPickableRender_ovb0023 {
    u8 pad_0x0[0x50];
    UnkPickableModel_ovb0023* model;
};

struct UnkPickableOwner_ovb0023 {
    u8 pad_0x0[0x228];
    UnkPickableRender_ovb0023* render;
};

struct UnkPickableComponent_ovb0023 {
    u8 pad_0x0[0x30];
    UnkPickableOwner_ovb0023* owner;
};

void fn_71009cd46c(UnkPickableComponent_ovb0023* self, f32 alpha) {
    UnkPickableRender_ovb0023* render = self->owner->render;
    if (!render)
        return;
    UnkPickableModel_ovb0023* model = render->model;
    if (!model)
        return;
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    u64 key = reinterpret_cast<UnkPickableColorKey_ovb0023*>(GameManagerIdTable().field_0x16d8.data)->key;
    fn_71001c2bac(model->slots, key, 0, &color, 1);
    color.a = alpha <= 0.0f ? 0.0f : alpha;
    key = reinterpret_cast<UnkPickableColorKey_ovb0023*>(GameManagerIdTable().field_0x16d8.data)->key;
    fn_71001c28f8(model->slots, key, 0, &color, 1);
}

using base::global::CStrId;

struct UnkPickableStream_ovb0023 {
    u8 pad_0x0[0x8];
    u64 pos;  // 0x8
    u8 pad_0x10[0x10];
    u64 size;  // 0x20
};

struct UnkPickableIds_ovb0023 {
    u8 pad_0x0[0x40];
    base::global::UnkStrId field_0x40;
    base::global::UnkStrId field_0x48;
};

void fn_71009cd2c4(UnkPickableIds_ovb0023* self, UnkPickableStream_ovb0023* stream) {
    fn_710024fc08();
    if (stream->pos < stream->size) {
        const char* name;
        fn_710007cee4(stream, &name);
        base::global::UnkStrId id(name, true);
        self->field_0x40 = static_cast<base::global::UnkStrId&&>(id);
    }
    if (stream->pos < stream->size) {
        const char* name;
        fn_710007cee4(stream, &name);
        base::global::UnkStrId id(name, true);
        self->field_0x48 = static_cast<base::global::UnkStrId&&>(id);
    }
}

struct UnkParent;
struct UnkInputComponent;
struct UnkInterp_7100105d34;

struct UnkPickablePlayer_ovb0023 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
};

struct UnkPickableLink_ovb0023 {
    UnkInterp_7100105d34* object;
    u64 id;
};

struct UnkPickableComponent2_ovb0023 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
    u8 pad_0x38[0x18];
    f32 timer;  // 0x50
    u8 pad_0x54[0xc];
    UnkPickableLink_ovb0023* link;  // 0x60
    u64 link_id;                   // 0x68

    UnkInterp_7100105d34* Linked() {
        if (link && link->id == link_id && link_id != (u64)-1)
            return link->object;
        return nullptr;
    }
};

struct UnkPickableComponent2V_ovb0023 {
    u8 pad_0x0[0x180];
    void (*vf_0x180)(UnkPickableComponent2_ovb0023* self, UnkPickablePlayer_ovb0023* player);
};

bool fn_71009cd9e0(UnkPickableComponent2_ovb0023* self, UnkPickablePlayer_ovb0023* player);

void fn_71009cd8e0(UnkPickableComponent2_ovb0023* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkPickablePlayer_ovb0023* player =
        reinterpret_cast<UnkPickablePlayer_ovb0023*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    if (player->input && fn_710087238c(player->input))
        return;
    if (fn_71009cd9e0(self, player)) {
        (*reinterpret_cast<UnkPickableComponent2V_ovb0023**>(self))->vf_0x180(self, player);
    } else {
        if (!(self->timer > 0.0f))
            return;
        self->timer -= dt;
        if (self->timer <= 1.0f)
            fn_71009cd46c(reinterpret_cast<UnkPickableComponent_ovb0023*>(self), self->timer);
        if (!(self->timer <= 0.0f))
            return;
        fn_7100232c68(self->owner);
    }
    if (UnkInterp_7100105d34* object = self->Linked())
        fn_7100105d34(object, 0.0f, false, 0.25f);
}
