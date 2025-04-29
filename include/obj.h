#ifndef DATA_OBJ_H
#define DATA_OBJ_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


typedef uint8_t ObjType;

#define SetINDIX(T, I) ((I) << 4) | ((T) & 0x0F)
#define GetINDIX(x) (((x) >> 4) & 0x0F)

enum {
  TYPE_NULL       = 0,
  TYPE_BOOL_F     = 1,
  TYPE_BOOL_T     = 2,
  TYPE_NUM        = 3,
  TYPE_NUMFL      = 5,
  TYPE_ENTRY      = 6,
  TYPE_STRING     = 7,
  TYPE_HASH_TABLE = 8,
  TYPE_AVL_TREE   = 9,
  TYPE_ARRAY      = 10,
  TYPE_FUNCTION   = 11,
};

typedef struct {
  ObjType type;
} Obj;

typedef union {
  float NumF;
  int Num;
} As;

typedef struct {
  ObjType type;
  As as;
} Native;

typedef struct {
  ObjType type;
  int8_t reference;
} ObjR;


typedef struct {
  ObjR obj;
  size_t length;
  size_t capacity;
  char* chars;
} ObjString;

typedef Obj* (*Fun)(Obj*, Obj*);

typedef struct {
  ObjR obj;
  Fun fun;
} ObjFun;


#endif  