#include "DataCore.h"

typedef struct {
    bool (*insert)(Obj*, Obj*);
    Obj* (*search)(Obj*, const char*);
    bool (*free)(Obj*);
} ObjOps;


ObjOps ObjOpsList[] = {
    {insertAVL, searchTree, freeTR},
    {insertTH, searchTH, freeTH},
};
