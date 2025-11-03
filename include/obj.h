#ifndef DATA_OBJ_H
#define DATA_OBJ_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>
#include <time.h>

typedef uint8_t _Atomic ObjType;

#define GetType(x) (atomic_load(&x->type) & 0x7F)
#define IsBlock(x) (atomic_load(&x->type) & 0x80)


enum {
  TYPE_NULL = 0,
  TYPE_BOOL_F = 1,
  TYPE_BOOL_T = 2,
  TYPE_NUM = 3,
  TYPE_NUMFL = 4,
  TYPE_ENTRY = 5,
  TYPE_TIME = 6,
  TYPE_STRING = 7,
  TYPE_ARRAY = 8,
  TYPE_AVL_TREE = 9,
  TYPE_HASH_TABLE = 10,
  TYPE_FUNCTION = 11,
  TYPE_BUFFER = 12,
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
  struct timespec time;
} ObjTime;

typedef struct {
  ObjR obj;
  size_t length;
  size_t capacity;
  char* chars;
} ObjString;

typedef struct Entry {
  ObjR obj;
  char* key;
  Obj* data;
  struct Entry* next;
} Entry;

typedef Obj* (*Fun)(Obj*, Obj*);

typedef struct {
  ObjR obj;
  Fun fun;
} ObjFun;

#endif