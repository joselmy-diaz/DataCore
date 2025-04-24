#ifndef DATA_OBJ_H
#define DATA_OBJ_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


typedef uint8_t ObjType;

enum {
  TYPE_NULL = 0,
  TYPE_BOOL = 1,
  TYPE_NUM       = 3,
  TYPE_NUMFL     = 5,
  OBJ_STRING     = 6,
  OBJ_HASH_TABLE = 7,
  OBJ_AVL_TREE   = 8,
  OBJ_ARRAY      = 9,
  OBJ_FUNCTION   = 10
};

typedef union {
  float NumF;
  int Num;
  bool Bool;
} As;

typedef struct {
  ObjType type;
  As as;
} Obj;


typedef struct {
  Obj obj;
  size_t length;
  char* chars;
} ObjString;

typedef Obj* (*Fun)(Obj*, Obj*);

typedef struct {
  Obj obj;
  Fun fun;
} ObjFun;


#endif  