// navmesh::CFixedDynamicSmartLinkRule's unit (tools/tu_map.py unit 354): reads one value of the rule's
// tree from a file (a direction by name, flags, a comparison, a cell kind, the side its normal points
// to) into the rule's root or as a new child of `parent`, then its children. Names and layouts are
// placeholders.
#include <string.h>

#include "UnkNavRuleValues_A1_b75.h"
#include "base/global/CRntFile.h"
#include "base/math/CVector.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::math::CVector2D;

// The rule as this function sees it: the root value of its tree at +0x20.
struct UnkSmartLinkRuleTree_l26ee04 {
    u8 pad_0x0[0x20];
    UnkNavRuleValue_A1_b75 root;  // 0x20
};

s32 fn_710026f548(const char* name);

// The twelve diagonal directions, from 27 degrees round to 333 (set by the unit's initialiser).
extern CVector2D g_unk_71726a4368[12] __attribute__((visibility("hidden")));

// A float's bits, as the value's words hold them, and back.
static inline u32 Bits(f32 f) {
    union {
        f32 f;
        u32 u;
    } v = {f};
    return v.u;
}
static inline f32 Float(u32 bits) {
    union {
        u32 u;
        f32 f;
    } v = {bits};
    return v.f;
}

// The direction and the normal as the value's words hold them.
struct UnkWords2_l26ee04 {
    u32 x;
    u32 y;
};

static inline UnkWords2_l26ee04 Words(f32 x, f32 y) { return {Bits(x), Bits(y)}; }
static inline UnkWords2_l26ee04 Words(const CVector2D& v) { return {Bits(v.x), Bits(v.y)}; }

// A diagonal direction from the table; a diagonal link only goes to a solid or a forbidden cell.
static inline UnkWords2_l26ee04 Diagonal(const CVector2D& direction, s32& kind) {
    UnkWords2_l26ee04 words = Words(direction);
    if (kind != 1 && kind != 5)
        kind = 1;
    return words;
}

// The element at `index`; with field_0x14 set an index past the end first grows the array.
static inline UnkNavRuleValue_A1_b75& At(UnkNavRuleValues_A1_b75* array, u32 index) {
    if (array->field_0x14 && array->count <= index)
        navmesh::Grow_A1_b75(array, index);
    return array->data[index];
}

static inline UnkNavRuleValue_A1_b75& PushBack(UnkNavRuleValues_A1_b75* array, const UnkNavRuleValue_A1_b75& value) {
    array->PushBackCopy(value);
    return At(array, array->count - 1);
}

void fn_710026ee04(UnkSmartLinkRuleTree_l26ee04* self, base::global::CRntFile* file, UnkNavRuleValue_A1_b75* parent) {
    const char* direction_name;
    u32 field_0x4;
    bool field_0x0;
    const char* comparison_name;
    const char* kind_name;
    u32 field_0x10;
    bool field_0x14;
    fn_710007cee4(file, &direction_name);
    fn_710007c714(file, &field_0x0, 1, 1);
    fn_710007c714(file, &field_0x4, 4, 1);
    fn_710007cee4(file, &comparison_name);
    fn_710007cee4(file, &kind_name);
    fn_710007c714(file, &field_0x10, 4, 1);
    fn_710007c714(file, &field_0x14, 1, 1);

    u32 comparison;
    if (strcmp(comparison_name, "None") == 0)
        comparison = 0;
    else if (strcmp(comparison_name, "Min") == 0)
        comparison = 1;
    else if (strcmp(comparison_name, "Equals") == 0)
        comparison = 2;
    else
        comparison = 3;

    s32 kind = fn_710026f548(kind_name);

    UnkWords2_l26ee04 direction;
    if (strcmp(direction_name, "UP") == 0) {
        direction = Words(0.0f, 1.0f);
    } else if (strcmp(direction_name, "DOWN") == 0) {
        direction = Words(0.0f, -1.0f);
    } else if (strcmp(direction_name, "LEFT") == 0) {
        direction = Words(-1.0f, 0.0f);
    } else if (strcmp(direction_name, "RIGHT") == 0) {
        direction = Words(1.0f, 0.0f);
    } else if (strcmp(direction_name, "RIGHT_UP_27") == 0) {
        direction = Diagonal(g_unk_71726a4368[0], kind);
    } else if (strcmp(direction_name, "RIGHT_UP_45") == 0) {
        direction = Diagonal(g_unk_71726a4368[1], kind);
    } else if (strcmp(direction_name, "RIGHT_UP_63") == 0) {
        direction = Diagonal(g_unk_71726a4368[2], kind);
    } else if (strcmp(direction_name, "LEFT_UP_117") == 0) {
        direction = Diagonal(g_unk_71726a4368[3], kind);
    } else if (strcmp(direction_name, "LEFT_UP_135") == 0) {
        direction = Diagonal(g_unk_71726a4368[4], kind);
    } else if (strcmp(direction_name, "LEFT_UP_153") == 0) {
        direction = Diagonal(g_unk_71726a4368[5], kind);
    } else if (strcmp(direction_name, "LEFT_DOWN_207") == 0) {
        direction = Diagonal(g_unk_71726a4368[6], kind);
    } else if (strcmp(direction_name, "LEFT_DOWN_225") == 0) {
        direction = Diagonal(g_unk_71726a4368[7], kind);
    } else if (strcmp(direction_name, "LEFT_DOWN_243") == 0) {
        direction = Diagonal(g_unk_71726a4368[8], kind);
    } else if (strcmp(direction_name, "RIGHT_DOWN_297") == 0) {
        direction = Diagonal(g_unk_71726a4368[9], kind);
    } else if (strcmp(direction_name, "RIGHT_DOWN_315") == 0) {
        direction = Diagonal(g_unk_71726a4368[10], kind);
    } else if (strcmp(direction_name, "RIGHT_DOWN_333") == 0) {
        direction = Diagonal(g_unk_71726a4368[11], kind);
    } else {
        direction = Words(0.0f, 0.0f);
    }

    UnkWords2_l26ee04 normal;
    if (kind == 2) {
        normal = Words(0.0f, 0.0f);
    } else {
        const char* side;
        fn_710007cee4(file, &side);
        if (strcmp(side, "LEFT") == 0)
            normal = Words(-Float(direction.y), Float(direction.x));
        else if (strcmp(side, "RIGHT") == 0)
            normal = Words(Float(direction.y), -Float(direction.x));
        else
            normal = Words(0.0f, 0.0f);
    }

    UnkNavRuleValue_A1_b75 value;
    value.field_0x0.value = direction.x;
    value.field_0x4.value = direction.y;
    value.field_0x8.value = normal.x;
    value.field_0xc.value = normal.y;
    value.tail.field_0x0 = field_0x0;
    value.tail.field_0x4 = field_0x4;
    value.tail.field_0x8 = kind;
    value.tail.field_0xc = comparison;
    value.tail.field_0x10 = field_0x10;
    value.tail.field_0x14 = field_0x14;

    UnkNavRuleValue_A1_b75* current;
    if (parent) {
        PushBack(&parent->children, value);
        current = &At(&parent->children, parent->children.count - 1);
    } else {
        self->root = value;
        current = &self->root;
    }

    s32 child_count = 0;
    fn_710007c714(file, &child_count, 4, 1);
    for (s32 i = 0; i < child_count; ++i)
        fn_710026ee04(self, file, current);
}
