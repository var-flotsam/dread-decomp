// Unit 2604 (tools/tu_map.py, no reliable name): copy-assignment of a record whose two string ids and
// tail values are either held in place (kind 1) or referred to through pointers (kind 2, +0x28).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkWordPair_b0233 {
    u64 a, b;
};

struct UnkRecordTail_b0233 {
    CStrId second;
    UnkWordPair_b0233 words;  // 0x8
    u32 count;     // 0x18

    void CopyWords(const UnkRecordTail_b0233& other) {
        u64 a = other.words.a;
        u64 b = other.words.b;
        count = other.count;
        words.a = a;
        words.b = b;
    }
};

struct UnkRecordRefs_b0233 {
    CStrId* first;
    UnkRecordTail_b0233* tail;
};

struct UnkRecord_b0233 {
    CStrId first;              // 0x0
    UnkRecordTail_b0233 tail;  // 0x8
    u32 kind;                  // 0x28: 1 holds the values, 2 refers to them

    CStrId& First() { return kind == 2 ? *reinterpret_cast<UnkRecordRefs_b0233*>(this)->first : first; }
    UnkRecordTail_b0233& Tail() { return kind == 2 ? *reinterpret_cast<UnkRecordRefs_b0233*>(this)->tail : tail; }
};

UnkRecord_b0233* fn_7100f917f4(UnkRecord_b0233* self, UnkRecord_b0233* other) {
    if (other == self)
        return self;
    UnkRecord_b0233& src = *other;
    self->First() = src.First();
    UnkRecordTail_b0233& from = src.Tail();
    UnkRecordTail_b0233& to = self->Tail();
    to.second = from.second;
    to.CopyWords(from);
    return self;
}
