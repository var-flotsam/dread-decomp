// Unit 376: builds a navigation link record from its parts: two ends (an id, a kind, a 2D and a 3D
// position and a value each), a name, flags and floats, a list of 2-byte entries and three key
// arrays. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

void fn_710027e258(void* self, const void* other);  // the key array's copy constructor

// Two floats, copied one by one.
struct UnkPoint2_l2b5aa8 {
    f32 x, y;
    UnkPoint2_l2b5aa8(const UnkPoint2_l2b5aa8& o) : x(o.x), y(o.y) {}
};

// A key array (a CRntVector of 4-byte keys), copied out of line.
struct alignas(8) UnkLinkKeys_l2b5aa8 {
    u8 storage[0x28];
};

struct UnkLinkRecord_l2b5aa8 {
    bool valid;            // 0x0
    u8 from_kind;          // 0x1
    u16 from_id;           // 0x2
    UnkPoint2_l2b5aa8 from_point;  // 0x4
    UnkVec3f from_pos;     // 0xc
    u32 from_value;        // 0x18
    u8 to_kind;            // 0x1c
    u16 to_id;             // 0x1e
    UnkPoint2_l2b5aa8 to_point;  // 0x20
    UnkVec3f to_pos;       // 0x28
    u32 to_value;          // 0x34
    CStrId name;           // 0x38
    bool field_0x40;
    bool field_0x41;
    f32 field_0x44;
    f32 field_0x48;
    bool field_0x4c;
    bool field_0x4d;
    f32 field_0x50;
    CRntVector<u16> entries;     // 0x58
    UnkLinkKeys_l2b5aa8 keys_a;  // 0x80
    UnkLinkKeys_l2b5aa8 keys_b;  // 0xa8
    UnkLinkKeys_l2b5aa8 keys_c;  // 0xd0
    u64 field_0xf8;
};
static_assert(__builtin_offsetof(UnkLinkRecord_l2b5aa8, entries) == 0x58, "");
static_assert(__builtin_offsetof(UnkLinkRecord_l2b5aa8, field_0xf8) == 0xf8, "");

void fn_71002b5aa8(UnkLinkRecord_l2b5aa8* self, const u16* from_id, const u8* from_kind,
                   const UnkPoint2_l2b5aa8* from_point, const UnkVec3f* from_pos, u32 from_value, const u16* to_id,
                   const u8* to_kind, const UnkPoint2_l2b5aa8* to_point, const UnkVec3f* to_pos, u32 to_value,
                   const CStrId* name, bool flag_a, f32 a, f32 b, bool flag_b, bool flag_c, f32 c,
                   const CRntVector<u16>* entries, const UnkLinkKeys_l2b5aa8* keys_a,
                   const UnkLinkKeys_l2b5aa8* keys_b, const UnkLinkKeys_l2b5aa8* keys_c) {
    self->valid = true;
    self->from_kind = *from_kind;
    self->from_id = *from_id;
    new (&self->from_point) UnkPoint2_l2b5aa8(*from_point);
    new (&self->from_pos) UnkVec3f(*from_pos);
    self->from_value = from_value;
    self->to_kind = *to_kind;
    self->to_id = *to_id;
    new (&self->to_point) UnkPoint2_l2b5aa8(*to_point);
    new (&self->to_pos) UnkVec3f(*to_pos);
    self->to_value = to_value;
    new (&self->name) CStrId(*name);
    self->field_0x40 = flag_a;
    self->field_0x41 = false;
    self->field_0x44 = a;
    self->field_0x48 = b;
    self->field_0x4c = flag_b;
    self->field_0x4d = flag_c;
    self->field_0x50 = c;
    new (&self->entries) CRntVector<u16>(*entries);
    fn_710027e258(&self->keys_a, keys_a);
    fn_710027e258(&self->keys_b, keys_b);
    fn_710027e258(&self->keys_c, keys_c);
    self->field_0xf8 = 0;
}
