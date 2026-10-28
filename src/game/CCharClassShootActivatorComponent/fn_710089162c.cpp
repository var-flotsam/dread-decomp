// Return copies of a pair of static ids: a string id and a file path id.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkIdPathPair {
    CStrId id;
    CFilePathStrId path;
};

extern CStrId g_unk_717272cf80 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_717272cf88 __attribute__((visibility("hidden")));
extern CStrId g_unk_717272d388 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_717272d390 __attribute__((visibility("hidden")));

UnkIdPathPair fn_710089162c() {
    return {g_unk_717272cf80, g_unk_717272cf88};
}

UnkIdPathPair fn_71008974e8() {
    return {g_unk_717272d388, g_unk_717272d390};
}
