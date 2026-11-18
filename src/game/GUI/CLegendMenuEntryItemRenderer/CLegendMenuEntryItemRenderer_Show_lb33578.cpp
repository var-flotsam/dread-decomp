// GUI::CLegendMenuEntryItemRenderer's unit (tools/tu_map.py, unit 1684): shows an entry of the legend. An entry
// out of range (or no data) gets the id table's style at +0xe10 on the label's text; one in range gets the style
// at +0xf00, the entry's text on the label (+0x230) and the image its name at +0x30 names on the sprite (+0x238).
// Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiSprite.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

static inline const CStrId& TableId(u32 offset) {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + offset);
}

struct UnkClone_fn_7100b32d78;
using UnkLegendEntrySlot_lb33578 = UnkCloneElem<UnkClone_fn_7100b32d78, 8>;
extern template UnkLegendEntrySlot_lb33578& base::global::CRntVector<UnkLegendEntrySlot_lb33578>::GrowAt(u32);

struct UnkLegendEntry_lb33578 {
    CRntString text;   // 0x0
    CRntString image;  // 0x30
};

struct UnkLegendEntryData_lb33578 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual u32 Count();  // vf_0x38
    u8 pad_0x8[0x8];
    base::global::CRntVector<UnkLegendEntrySlot_lb33578> entries;  // 0x10

    UnkLegendEntry_lb33578* At(u32 index) {
        return *reinterpret_cast<UnkLegendEntry_lb33578**>(entries.GrowAt(index).data);
    }
};

// The label's text object: slot 0x1b8 sets its style.
struct UnkLegendText_lb33578 {
    void SetStyle(const CStrId& style, bool a, bool b) {
        typedef void (*Fn)(UnkLegendText_lb33578*, const CStrId*, bool, bool);
        (*reinterpret_cast<Fn**>(this))[0x1b8 / 8](this, &style, a, b);
    }
};

struct UnkLegendLabel_lb33578 {
    u8 pad_0x0[0x120];
    UnkLegendText_lb33578* text;  // 0x120
};

struct UnkLegendEntryItemRenderer_lb33578 {
    u8 pad_0x0[0x230];
    UnkLegendLabel_lb33578* label;  // 0x230
    UnkGuiDisplayObject* sprite;    // 0x238
};

// A string id built from a string's text, out of line (fn_7100000850), released as ~CStrId does.
struct UnkLegendImageId_lb33578 {
    UnkStringData* data;

    explicit UnkLegendImageId_lb33578(const CRntString& text) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkLegendImageId_lb33578() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    const CStrId& Id() const { return *reinterpret_cast<const CStrId*>(this); }
};

void fn_7100b33578(UnkLegendEntryItemRenderer_lb33578* self, UnkLegendEntryData_lb33578* data, u32 index) {
    if (!data || data->Count() <= index) {
        self->label->text->SetStyle(TableId(0xe10), true, true);
        return;
    }
    self->label->text->SetStyle(TableId(0xf00), true, true);
    CRntString text(data->At(index)->text);
    UnkLegendImageId_lb33578 image(CRntString(data->At(index)->image));
    fn_71011ec424(reinterpret_cast<UnkGuiDisplayObject*>(self->label), text);
    fn_71011f8bdc(self->sprite, image.Id());
}
