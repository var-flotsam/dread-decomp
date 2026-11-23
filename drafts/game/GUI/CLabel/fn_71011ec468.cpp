// Unit 2752 (GUI::CLabel by reflection): sets the label's text and builds the text it shows, each
// localization key between '#' and '#' (or '+') replaced by the language's text for it.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2733.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A text of 32-bit characters (game/UnkGuiLabel.h's UnkGuiText): the characters at +0x8, null when
// empty, and their count at +0x10.
struct UnkLayoutText {
    u8 unk_0x0[0x8];
    const char32_t* chars;
    u32 count;
    u8 unk_0x14[0xc];

    explicit UnkLayoutText(const char32_t* text) { fn_7100066a50(this, text); }
    ~UnkLayoutText() { fn_7100066e44(this); }
};
static_assert(sizeof(UnkLayoutText) == 0x20, "UnkLayoutText placeholder layout");

struct UnkDialogText : UnkGuiText {
    ~UnkDialogText() { fn_7100066e44(this); }
};

struct UnkWideTextCmp;
bool fn_7100067130(const UnkWideTextCmp* self, const UnkWideTextCmp* other);
bool fn_71000671a0(const UnkWideTextCmp* self, const UnkWideTextCmp* other);
void fn_7100066d4c(UnkLayoutText* self, const UnkLayoutText* other);
void fn_7100067430(UnkLayoutText* self, u32 c);
// Appends a text.
void fn_7100067204(UnkLayoutText* self, const UnkGuiText* other);
CRntString fn_710006c080(const u32* text);
extern const u32 g_unk_7101648434[] __attribute__((visibility("hidden")));  // U""

// The label's two texts and whether its keys are replaced.
struct UnkLabelTexts {
    u8 unk_0x0[0x218];
    UnkLayoutText shown;  // 0x218
    UnkLayoutText text;   // 0x238
    u8 unk_0x258[0xa4];
    bool replace_keys;  // 0x2fc
};

// A string id built from a string's text, out of line (fn_7100000850).
struct UnkTextKeyId_l1ec468 {
    UnkStringData* data;

    explicit UnkTextKeyId_l1ec468(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextKeyId_l1ec468() {
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

static inline const char32_t* EmptyText() {
    return reinterpret_cast<const char32_t*>(g_unk_7101648434);
}

static inline const char32_t* Chars(const UnkLayoutText& text) {
    return text.chars ? text.chars : EmptyText();
}

// The number of characters; the count is read whether or not there are any.
static inline s32 Length(const UnkLayoutText& text) {
    s32 count = text.count;
    return text.chars ? count : 0;
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

static inline CRntString ToString(const UnkLayoutText& text) {
    return fn_710006c080(reinterpret_cast<const u32*>(Chars(text)));
}

static inline void Append(UnkLayoutText& text, const UnkGuiText& other) {
    fn_7100067204(&text, &other);
}

void fn_71011ec468(UnkGuiDisplayObject* self, const UnkGuiText& newText, bool force) {
    UnkLabelTexts* label = reinterpret_cast<UnkLabelTexts*>(self);
    const UnkLayoutText& text = reinterpret_cast<const UnkLayoutText&>(newText);
    if (fn_7100067130(reinterpret_cast<const UnkWideTextCmp*>(&label->text),
                      reinterpret_cast<const UnkWideTextCmp*>(&text)) &&
        !force)
        return;
    fn_7100066d4c(&label->text, &text);

    UnkLayoutText shown(EmptyText());
    s32 count = Length(text);
    UnkLayoutText key(EmptyText());
    bool inKey = false;
    for (s32 i = 0; i < count; i++) {
        char32_t c = Chars(text)[i];
        if (inKey) {
            if (c == '#' || c == '+') {
                Append(shown, fn_710119d040(Language(), UnkTextKeyId_l1ec468(ToString(key)).Id()));
                inKey = false;
            } else {
                fn_7100067430(&key, c);
            }
        } else if (c == '#' && label->replace_keys) {
            fn_7100066af0(&key, EmptyText());
            inKey = true;
        } else {
            fn_7100067430(&shown, c);
        }
    }
    if (inKey) {
        UnkDialogText translated =
            fn_710119d040(Language(), UnkTextKeyId_l1ec468(ToString(key)).Id());
        Append(shown, translated);
    }

    if (fn_71000671a0(reinterpret_cast<const UnkWideTextCmp*>(&label->shown),
                      reinterpret_cast<const UnkWideTextCmp*>(&shown))) {
        fn_7100066d4c(&label->shown, &shown);
        self->vf_0x88();
    }
    fn_71011ec768(reinterpret_cast<UnkLabelLayout*>(self));
}
