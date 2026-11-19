// CBlackboard's unit: loads the blackboard from its users' files (each user names one; the first that
// fails to parse stops the walk), or, when none of them opens, from the file its own two strings name;
// whether it parsed. Names and layouts are placeholders.
#include "base/UnkFileReader.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/UnkTunableLine.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/CBlackboard.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::reflection::UnkReflObject;

struct UnkBlackboardUser_b0026;
struct UnkBlackboardStrings_b0026;
void fn_7101047b2c(UnkBlackboardUser_b0026* self, CRntString* out);
void fn_71010476a0(UnkBlackboardStrings_b0026* self, CRntString* out);
bool fn_71010477f0(UnkReflObject* self, const CRntFile* file);

// The users listed from +0x70 (the nodes of the map at +0x30, whose count is at +0x3c).
struct UnkBlackboardUserNode_l047ca8 {
    base::global::CStrId key;
    UnkBlackboardUser_b0026* user;
    UnkBlackboardUserNode_l047ca8* next;
};

struct UnkBlackboardUsers_l047ca8 {
    u8 pad_0x0[0x3c];
    u32 count;
    u8 pad_0x40[0x30];
    UnkBlackboardUserNode_l047ca8* first;
};
static_assert(__builtin_offsetof(UnkBlackboardUsers_l047ca8, first) == 0x70, "layout");

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
bool fn_7101047ca8(CBlackboard* self) {
    self->vf_0x18();
    UnkBlackboardUsers_l047ca8* users = reinterpret_cast<UnkBlackboardUsers_l047ca8*>(self);
    if (users->count) {
        bool any = false;
        bool ok = true;
        for (UnkBlackboardUserNode_l047ca8* node = users->first; node; node = node->next) {
            UnkTunableLine path;
            fn_7101047b2c(node->user, path.Str());
            CRntFile file;
            fn_710007be8c(&file, 0x16, nullptr);
            bool opened;
            {
                UnkStrPair_0x10 s;
                fn_710008d4c0(&s, reinterpret_cast<const UnkEntry_0x30*>(path.Str()));
                {
                    CRntString mode("rb");
                    opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &s, &mode);
                }
                fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
            }
            if (opened) {
                ok = fn_71010477f0(reinterpret_cast<UnkReflObject*>(self), &file);
                fn_710007c144(&file);
                any = true;
            }
            fn_710007c0e4(&file);
            if (!ok)
                break;
        }
        if (any)
            return ok;
    }
    // No user's file opened: the blackboard's own.
    UnkTunableLine path;
    fn_71010476a0(reinterpret_cast<UnkBlackboardStrings_b0026*>(self), path.Str());
    CRntFile file;
    fn_710007be8c(&file, 0x16, nullptr);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d4c0(&s, reinterpret_cast<const UnkEntry_0x30*>(path.Str()));
        {
            CRntString mode("rb");
            opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    bool ok = false;
    if (opened) {
        ok = fn_71010477f0(reinterpret_cast<UnkReflObject*>(self), &file);
        fn_710007c144(&file);
    }
    fn_710007c0e4(&file);
    return ok;
}
