// GUI::CGlobalMapComposition's unit (tools/tu_map.py unit 1680): shows the transport between the selected
// area and the hovered scenario's map: plays their animations and sounds, watches both animations, labels
// the root with "<area>_<current>-<map>" and, for the sky base, centres the root. Names and layouts are
// placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiPos.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU2755.h"
#include "game/UnkTU57.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "game/UnkTU2564.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkStateKey;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0248;
extern UnkEnumType g_unk_71729f0240;

// The id the lookup returns: starts as the pool's empty entry (making the pool if there is none yet)
// and is released like a CStrId.
struct UnkMapNameId {
    UnkStringData* data;

    UnkMapNameId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkMapNameId() { reinterpret_cast<CStrId*>(this)->Reset(); }
    // Releases the string held and takes the one `id` holds, leaving `id` empty.
    UnkMapNameId& operator=(CStrId&& id) {
        reinterpret_cast<CStrId*>(this)->ReleaseAndTake(id);
        return *this;
    }
};

UnkMapNameId CGlobalMapComposition_GetMapNameIdForId(const CStrId& scenario) {
    UnkMapNameId result;
    const char* name;
    if (scenario.data == g_unk_71727c7fe8.data)
        name = "MAP_CAV";
    else if (scenario.data == g_unk_71727c7ff0.data)
        name = "MAP_MAG";
    else if (scenario.data == g_unk_71727c7ff8.data)
        name = "MAP_LAB";
    else if (scenario.data == g_unk_71727c8000.data)
        name = "MAP_AQUA";
    else if (scenario.data == g_unk_71727c8008.data)
        name = "MAP_FOR";
    else if (scenario.data == g_unk_71727c8010.data)
        name = "MAP_QUAR";
    else if (scenario.data == g_unk_71727c8018.data)
        name = "MAP_SANC";
    else if (scenario.data == g_unk_71727c8020.data)
        name = "MAP_SHIP";
    else if (scenario.data == g_unk_71727c8028.data)
        name = "MAP_SKYB";
    else
        return result;
    result = CStrId(name, true);
    return result;
}

// An anchor point as the point fn_71011cf0a8 takes.
static inline const UnkVec2Result& AsPoint(const UnkGuiPos& pos) {
    return reinterpret_cast<const UnkVec2Result&>(pos);
}

// What a widget's animation getter returns: the listeners of the animation at +0x98.
struct UnkMapAnimation {
    u8 pad_0x0[0x98];
    base::UnkDeferredSet<void> listeners;  // 0x98
};

// A display object as this file uses it: its animation calls and its name.
struct UnkMapWidget_lb28144 {
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
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void PlayAnimation(const char* name, bool loop);  // vf_0x1e8
    virtual void vf_0x1f0();
    virtual UnkMapAnimation* Animation();  // vf_0x1f8

    u8 pad_0x8[0x70];
    u8 anchor[0x8];  // 0x78: the anchor state fn_71011f6240 reads
    u8 pad_0x80[0xb0];
    CStrId name;  // 0x130
};
static_assert(__builtin_offsetof(UnkMapWidget_lb28144, name) == 0x130, "UnkMapWidget_lb28144 layout");

static inline UnkMapWidget_lb28144* Widget(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkMapWidget_lb28144*>(object);
}

static inline UnkGuiItemState* Anchor(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkGuiItemState*>(Widget(object)->anchor);
}

// A registration in a deferred set: the set, removed from it by the entry's address.
struct UnkMapListener {
    base::UnkDeferredSet<void>* set;
    u8 pad_0x8[0x10];

    // Moves the registration to the animation's listeners.
    void Listen(UnkMapAnimation* animation) {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        set = &animation->listeners;
        fn_71004f3cf0(set, this);
    }
};

// A string with 0x100 bytes of inline storage, built from part of an id's text.
struct UnkMapLabel {
    u8 str[0x30];
    void** storage_vtable;  // 0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    // The id's text past its first `skip` characters (a prefix such as "#" or "MAP_").
    UnkMapLabel(const CStrId& id, u32 skip) {
        ++id.data->reads;
        const CRntString& text = id.data->String();
        u32 length = text.length - skip;
        const char* start = text.data + skip;
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = g_unk_7101a194b8 + 2;
        field_0x138 = 0;
        field_0x140 = 0;
        CRntString_AssignPrefix(Str(), start, length);
    }
    ~UnkMapLabel() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};
static_assert(sizeof(UnkMapLabel) == 0x148, "UnkMapLabel layout");

struct UnkGlobalMapView_b28144 {
    u8 pad_0x0[0x594];
    s32 transport_state;  // 0x594
    f32 transport_timer;  // 0x598
    u8 pad_0x59c[0x5e0 - 0x59c];
    UnkGuiDisplayObject* root;     // 0x5e0
    UnkMapWidget_lb28144* current;         // 0x5e8
    u8 pad_0x5f0[0x8];
    UnkGuiDisplayObject* frame;    // 0x5f8
    u8 pad_0x600[0x6f8 - 0x600];
    CStrId selected_area;  // 0x6f8
    CStrId hovered;        // 0x700
    u8 pad_0x708[0x8];
    UnkSoundHandle open_sound;   // 0x710
    UnkSoundHandle close_sound;  // 0x720
    u8 pad_0x730[0x10];
    UnkMapListener listener_a;  // 0x740
    UnkMapListener listener_b;  // 0x758
};
static_assert(__builtin_offsetof(UnkGlobalMapView_b28144, root) == 0x5e0, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapView_b28144, selected_area) == 0x6f8, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapView_b28144, listener_b) == 0x758, "layout");

// `object` if it is a display-object container, else null.
static inline UnkGuiDisplayObject* TransportContainer(UnkGuiDisplayObject* object) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->GetClass(), fn_71011d7b84()) ? object : nullptr;
}

void fn_7100b28144(UnkGlobalMapView_b28144* self) {
    UnkGuiDisplayObject* area = TransportContainer(fn_71011d98ec(self->root, self->selected_area.data->Key()));
    UnkGuiDisplayObject* map =
        TransportContainer(fn_71011d98ec(self->root, CGlobalMapComposition_GetMapNameIdForId(self->hovered).data->Key()));
    if (!area || !map)
        return;

    fn_71011cdc30(area, true);
    fn_71011cdc30(map, true);
    Widget(map)->PlayAnimation("Transport_Destination", false);
    self->open_sound = fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0248), self);
    UnkMapWidget_lb28144* destination = Widget(fn_71011d98ec(map, 0x14b4b26c34e9a94bull));
    self->listener_a.Listen(destination->Animation());

    UnkMapLabel label(self->selected_area, 2);
    CRntString_AppendCString(label.Str(), "_");
    {
        UnkMapLabel part(self->current->name, 4);
        CRntString_Append(label.Str(), part.Str());
    }
    CRntString_AppendCString(label.Str(), "-");
    {
        UnkMapLabel part(Widget(map)->name, 4);
        CRntString_Append(label.Str(), part.Str());
    }
    Widget(self->root)->PlayAnimation(label.Str()->data, false);
    self->close_sound = fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0240), self);
    UnkMapWidget_lb28144* departure = Widget(fn_71011d98ec(area, 0x395e044e2bfbf935ull));
    self->listener_b.Listen(departure->Animation());

    UnkMapWidget_lb28144* sky_base = nullptr;
    if (self->current->name == CStrId("MAP_SKYB", true))
        sky_base = self->current;
    else if (Widget(map)->name == CStrId("MAP_SKYB", true))
        sky_base = Widget(map);
    if (sky_base) {
        UnkVec2Result size = fn_71011cf0a8(self->root, AsPoint(fn_71011f6240(Anchor(self->frame), 1, 1)));
        f32 dx = 0.5f - size.x;
        f32 dy = 0.5f - size.y;
        UnkGuiDisplayObject* root = self->root;
        base::math::CVector2D position;
        UnkGuiPos origin = fn_71011f6240(Anchor(root), 0, 0);
        position.x = dx + origin.x;
        position.y = dy + origin.y;
        fn_71011cde64(root, &position, 0, 0);
        fn_71011cd5a0(self->root);
    }
    self->transport_state = 1;
    self->transport_timer = 2.0f;
}
