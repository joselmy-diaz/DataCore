#ifndef DATA_OBJ_H
#define DATA_OBJ_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


typedef uint8_t ObjType;

enum {
  TYPE_NULL = 0,
  TYPE_BOOL_F = 1,
  TYPE_BOOL_T = 2,
  TYPE_NUM       = 3,
  TYPE_NUMFL     = 5,
  TYPE_ENTRY      = 11,
  OBJ_STRING     = 6,
  OBJ_HASH_TABLE = 7,
  OBJ_AVL_TREE   = 8,
  OBJ_ARRAY      = 9,
  OBJ_FUNCTION   = 10,
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
} Nativo;

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