#compiler-settings
directiveStartToken= %
#end compiler-settings
%slurp
#ifndef __BH_VE_CPU_TAC
#define __BH_VE_CPU_TAC
//
//  NOTE: This file is autogenerated based on the tac-definition.
//        You should therefore not edit it manually.
//
#include "stdint.h"

// Bohrium custom types, used of representing
// two inputs in one constant... hopefully we can get
// rid of it... at some point...
typedef struct { uint64_t first, second; } pair_LL; 

#ifndef __BH_BASE
#define __BH_BASE
typedef struct
{
    void* data;     // Pointer to memory allocation supporting the buffer.
    int64_t type;   // Datatype for which the buffer is intended to store.
    int64_t nelem;  // Number of elements of the given datatype for which there is room for in the buffer.
} bh_base;
#endif

typedef enum KP_OPERATION {
    %for $op in $ops
    $addw($op['name']) = ${op['id']}$addsep($op, $ops)
    %end for
} KP_OPERATION;

typedef enum KP_OPERATOR {
    %for $oper in $opers
    $addw($oper['name'],15) = ${oper['id']}$addsep($oper, $opers)
    %end for
} KP_OPERATOR;

typedef enum KP_ETYPE {
    %for $type in $types
    $addw($type['name']) = ${type['id']}$addsep($type, $types)
    %end for
} KP_ETYPE;

typedef enum KP_LAYOUT {
    %for $layout in $layouts
    $addw($layout['name']) = ${layout['id']}$addsep($layout, $layouts)
    %end for
} KP_LAYOUT;   // Uses a single byte

typedef struct kp_tac {
    KP_OPERATION op;    // Operation
    KP_OPERATOR  oper;  // Operator
    uint32_t  out;      // Output operand
    uint32_t  in1;      // First input operand
    uint32_t  in2;      // Second input operand
    void* ext;
} kp_tac;

typedef struct kp_operand {
    KP_LAYOUT  layout;  // The layout of the data
    void*   const_data; // Pointer to constant
    KP_ETYPE   etype;   // Type of the elements stored
    int64_t start;      // Offset from memory allocation to start of array
    int64_t nelem;      // Number of elements available in the allocation

    int64_t ndim;       // Number of dimensions of the array
    int64_t* shape;     // Shape of the array
    int64_t* stride;    // Stride in each dimension of the array
    bh_base* base;      // Pointer to operand base or NULL when layout == SCALAR_CONST.
} kp_operand;           // Meta-data for a block argument

typedef struct kp_iterspace {
    KP_LAYOUT layout;   // The dominating layout
    int64_t ndim;       // The dominating rank/dimension of the iteration space
    int64_t* shape;     // Shape of the iteration space
    int64_t nelem;      // The number of elements in the iteration space
} kp_iterspace;

#define SCALAR_LAYOUT   ( SCALAR | SCALAR_CONST | SCALAR_TEMP )
#define ARRAY_LAYOUT    ( CONTRACTABLE | CONTIGUOUS | CONSECUTIVE | STRIDED | SPARSE )
#define COLLAPSIBLE     ( SCALAR | SCALAR_CONST | CONTRACTABLE | CONTIGUOUS | CONSECUTIVE )
#define DYNALLOC_LAYOUT ( SCALAR | CONTIGUOUS | CONSECUTIVE | STRIDED | SPARSE )

#define EWISE           ( MAP | ZIP | GENERATE )
#define REDUCTION       ( REDUCE_COMPLETE | REDUCE_PARTIAL )
#define ACCUMULATION    ( REDUCE_COMPLETE | REDUCE_PARTIAL | SCAN )
#define ARRAY_OPS       ( MAP | ZIP | GENERATE | REDUCE_COMPLETE | REDUCE_PARTIAL | SCAN | INDEX )
#define NBUILTIN_OPS    %echo $len($ops)-1
#define NBUILTIN_OPERS  %echo $len($opers)-1

#endif
