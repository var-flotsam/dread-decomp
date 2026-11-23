// GUI::CGlobalMapArea's unit (tools/tu_map.py unit 1679): the area's set-up. On first use it builds its ids
// from its name ("XX_<area>": the area id, "Default<area>", and the focus events), then shows itself when
// its blackboard section holds the position property, binds the four neighbouring areas and its "Back"
// and "Highlight" sprites, and sets its text and alpha. Names and layouts are placeholders.
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringData;
using base::reflection::CType;

extern CType g_unk_71727629e0 __attribute__((visibility("hidden")));  // GUI::CGlobalMapArea's class
extern UnkPropertyEx<> g_unk_71727ca210;  // the property read from the area's blackboard section

bool fn_7100395d3c(CBlackboard::CSection* self, const CStrId* name, base::math::CVector3D* out);
struct UnkContainer_71011da0c0;
void fn_71011da0c0(UnkContainer_71011da0c0* self, bool flag);

struct UnkGlobalMapArea_b21f20 {
    void** vtable;
    u8 pad_0x8[0x120 - 0x8];
    UnkGuiDisplayObject* children;  // 0x120: where the neighbouring areas are looked up
    u8 pad_0x128[0x8];
    CStrId name;  // 0x130
    u8 pad_0x138[0x230 - 0x138];
    UnkGuiDisplayObject* back;       // 0x230
    UnkGuiDisplayObject* highlight;  // 0x238
    UnkGuiDisplayObject* area_0x240;
    UnkGuiDisplayObject* area_0x248;
    UnkGuiDisplayObject* area_0x250;
    UnkGuiDisplayObject* area_0x258;
    CStrId id;          // 0x260
    CStrId default_id;  // 0x268
    CStrId id_0x270;
    CStrId on_focus_received;  // 0x278
    CStrId on_focus_lost;      // 0x280
    bool flag_0x288;
    CStrId section;     // 0x290: the blackboard section of the area
    CStrId id_0x298;
    CStrId id_0x2a0;
    CStrId id_0x2a8;
    CStrId id_0x2b0;
};
static_assert(__builtin_offsetof(UnkGlobalMapArea_b21f20, id) == 0x260, "layout");
static_assert(__builtin_offsetof(UnkGlobalMapArea_b21f20, id_0x2b0) == 0x2b0, "layout");

// A string id built from a string out of line (fn_7100000850).
struct UnkBuiltId_b21f20 {
    UnkStringData* data;

    explicit UnkBuiltId_b21f20(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<UnkStrIdBase*>(this), text, true);
    }
    ~UnkBuiltId_b21f20() { reinterpret_cast<UnkStrId*>(this)->~UnkStrId(); }
};

// Releases `id` and takes the built one's string; the built id is left with the empty string of the pool
// last loaded.
static inline void TakeId(CStrId& id, UnkBuiltId_b21f20&& other) {
    base::global::UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                fn_71000803a4(pool, id.data);
                pool = g_unk_7101d47108;
            }
        }
        id.data = &pool->empty;
    }
    id.data = other.data;
    other.data = &pool->empty;
}

static inline bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

// The id's text, counted as read.
static inline const CRntString& TextOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Reads the value of `key` from the blackboard section `section`; false when either is missing.
static inline bool ReadValue(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& key,
                             base::math::CVector3D* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_7100395d3c(s, &key.id, out);
    return false;
}

// The child of that name when it is a map area.
static inline UnkGuiDisplayObject* FindArea(UnkGuiDisplayObject* container, const CStrId& name) {
    UnkGuiDisplayObject* child = fn_71011d94d4(container, name);
    if (!child)
        return nullptr;
    const CType* type = child->GetClass();
    fn_7100b21170();
    return fn_710009d50c(type, &g_unk_71727629e0) ? child : nullptr;
}

// The child of that name when it is a sprite.
static inline UnkGuiDisplayObject* FindSprite(UnkGuiDisplayObject* container, const CStrId& name) {
    UnkGuiDisplayObject* child = fn_71011d94d4(container, name);
    return child && fn_710009d50c(child->GetClass(), fn_71011f6890()) ? child : nullptr;
}

static inline void BindArea(UnkGuiDisplayObject* children, const CStrId& name, UnkGuiDisplayObject*& area) {
    if (!IsEmpty(name) && !area)
        area = FindArea(children, name);
}

void fn_7100b21f20(UnkGlobalMapArea_b21f20* self) {
    UnkGuiDisplayObject* object = reinterpret_cast<UnkGuiDisplayObject*>(self);
    if (IsEmpty(self->id)) {
        // The name is "XX_<area>": the area id is what follows the prefix.
        UnkTunableLine name(TextOf(self->name));
        {
            u32 length = name.Length() - 3;
            const char* rest = name.Data() + 3;
            UnkTunableLine area(rest, length);
            TakeId(self->id, UnkBuiltId_b21f20(*area.Str()));
        }
        UnkTunableLine text("Default");
        CRntString_Append(text.Str(), &TextOf(self->id));
        TakeId(self->default_id, UnkBuiltId_b21f20(*text.Str()));
        static const CStrId s_on_focus_received("OnFocusReceived", true);
        self->on_focus_received = s_on_focus_received;
        static const CStrId s_on_focus_lost("OnFocusLost", true);
        self->on_focus_lost = s_on_focus_lost;
    }

    base::math::CVector3D position = {};
    fn_71011cdc30(object, ReadValue(&g_unk_7101d47310->field_0x968, self->section, Qid(g_unk_71727ca210), &position));

    UnkGuiDisplayObject* children = self->children;
    BindArea(children, self->id_0x298, self->area_0x240);
    BindArea(children, self->id_0x2a0, self->area_0x248);
    BindArea(children, self->id_0x2a8, self->area_0x250);
    BindArea(children, self->id_0x2b0, self->area_0x258);

    if (!self->back) {
        self->back = FindSprite(object, CStrId("Back", true));
        if (self->back)
            fn_71011cdc30(self->back, true);
    }
    if (!self->highlight)
        self->highlight = FindSprite(object, CStrId("Highlight", true));

    self->flag_0x288 = false;
    typedef void (*SetTextFn)(UnkGlobalMapArea_b21f20*, const char*, bool);
    const char* text = self->on_focus_lost.Text();
    reinterpret_cast<SetTextFn>(self->vtable[0x1e8 / 8])(self, text, false);
    fn_71011da0c0(reinterpret_cast<UnkContainer_71011da0c0*>(self), true);
    typedef void (*SetAlphaFn)(UnkGlobalMapArea_b21f20*, f32);
    reinterpret_cast<SetAlphaFn>(self->vtable[0x58 / 8])(self, 0.1f);
}
