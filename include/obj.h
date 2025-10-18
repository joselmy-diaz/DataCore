#ifndef DATA_OBJ_H
#define DATA_OBJ_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


typedef uint8_t ObjType;

enum {
  TYPE_NULL       = 0,
  TYPE_BOOL_F     = 1,
  TYPE_BOOL_T     = 2,
  TYPE_NUM        = 3,
  TYPE_NUMFL      = 4,
  TYPE_ENTRY      = 5,
  TYPE_STRING     = 6,
  TYPE_ARRAY      = 7,
  TYPE_AVL_TREE   = 8,
  TYPE_HASH_TABLE = 9,
  TYPE_FUNCTION   = 10,
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


typedef struct Entry {
    ObjR obj;
    char *key;
    Obj* data;
    struct Entry *next;
} Entry;

typedef Obj* (*Fun)(Obj*, Obj*);

typedef struct {
  ObjR obj;
  Fun fun;
} ObjFun;


#endif 