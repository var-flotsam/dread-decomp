// GUI::CSprite unit: sets the sprite's image from an id of the form "sheet/image". When either part
// differs from the sprite's, the image is cleared, and unless the sprite is locked (+0x1d8) the sheet is
// loaded and the image looked up in the GUI root ("WhiteSquare" needs no sheet). Names and layouts are
// placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkGuiSprite.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringEntry;
using base::global::UnkStringPool;

struct UnkGuiSprite_l1f8bdc : UnkGuiDisplayObject {
    u8 pad_0x198[0x30];
    CStrId sheet;  // 0x1c8
    CStrId image;  // 0x1d0
    void* field_0x1d8;
};
static_assert(__builtin_offsetof(UnkGuiSprite_l1f8bdc, sheet) == 0x1c8, "UnkGuiSprite_l1f8bdc layout");

struct UnkGuiListOwner_b49;
void* fn_71011b9fac(UnkGuiListOwner_b49* self, u64 key);
void fn_71011b97bc(UnkGuiRoot* root, CRntString sheet);
void* fn_71011ba000(UnkGuiRoot* root, const CStrId* sheet, const CStrId* image);
void fn_71011f8f80(UnkGuiSprite_l1f8bdc* self, void* image, CStrId id);

// A counted prefix of a C string, on 0x100 bytes of inline storage.
struct UnkSpriteText_l1f8bdc : UnkBufferString<0x100, g_unk_7101a194b8> {
    UnkSpriteText_l1f8bdc(const char* str, u32 length) { fn_710007e7f8(Str(), str, length); }
};

// The id a text gives (fn_7100000850), released as CStrId is.
struct UnkSpriteStrId_l1f8bdc {
    UnkStringData* data;

    explicit UnkSpriteStrId_l1f8bdc(const UnkSpriteText_l1f8bdc& text) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), *text.Str(), true);
    }
    ~UnkSpriteStrId_l1f8bdc() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    const CStrId& Id() const { return *reinterpret_cast<const CStrId*>(this); }
};

// An id's text, counting the use.
static inline const CRntString& TextOf(const UnkSpriteStrId_l1f8bdc& id) {
    UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(id.data);
    ++entry->tail.field_0x44;
    return entry->buffer;
}

void fn_71011f8bdc(UnkGuiDisplayObject* self, CStrId id) {
    UnkGuiSprite_l1f8bdc* sprite = static_cast<UnkGuiSprite_l1f8bdc*>(self);
    if (id.data == &g_unk_7101d47108->empty)
        return;

    s32 slash = fn_710007facc(&base::reflection::refl_RntStringOf(id), '/', 0);
    const CRntString& path = base::reflection::refl_RntStringOf(id);
    u32 sheet_length = slash == -1 ? path.length : slash;
    UnkSpriteStrId_l1f8bdc sheet(UnkSpriteText_l1f8bdc(path.data, sheet_length));
    const CRntString& rest = base::reflection::refl_RntStringOf(id);
    u32 start = slash + 1;
    u32 image_length = rest.length - start;
    UnkSpriteStrId_l1f8bdc image(UnkSpriteText_l1f8bdc(rest.data + start, image_length));

    if (sprite->sheet.data != sheet.data || sprite->image.data != image.data)
        fn_71011f8f80(sprite, nullptr, g_unk_7101d470e0);

    if (sprite->field_0x1d8)
        return;
    fn_71011b97bc(g_unk_717271e8f8, CRntString(TextOf(sheet)));
    void* found;
    if (slash != -1 && CRntString("WhiteSquare").key != TextOf(image).key)
        found = fn_71011ba000(g_unk_717271e8f8, &sheet.Id(), &image.Id());
    else
        found = fn_71011b9fac(reinterpret_cast<UnkGuiListOwner_b49*>(g_unk_717271e8f8),
                              reinterpret_cast<u64>(&image.Id()));
    fn_71011f8f80(sprite, found, sheet.Id());
    sprite->vf_0x60();
}
