#include "utils.hpp" 
//
//  NOTE: This file is autogenerated based on the tac-definition.
//        You should therefore not edit it manually.
//
using namespace std; 
namespace bohrium{ 
namespace core{

ETYPE bhtype_to_etype(bh_type bhtype)
{
    switch(bhtype) {
        case BH_BOOL: return BOOL;
        case BH_INT8: return INT8;
        case BH_INT16: return INT16;
        case BH_INT32: return INT32;
        case BH_INT64: return INT64;
        case BH_UINT8: return UINT8;
        case BH_UINT16: return UINT16;
        case BH_UINT32: return UINT32;
        case BH_UINT64: return UINT64;
        case BH_FLOAT32: return FLOAT32;
        case BH_FLOAT64: return FLOAT64;
        case BH_COMPLEX64: return COMPLEX64;
        case BH_COMPLEX128: return COMPLEX128;
        case BH_R123: return PAIRLL;
    }
    throw runtime_error("Unsupported bhtype, cannot map to etype.");
}

string operation_text(OPERATION op)
{
    switch(op) {
        case MAP:   return "MAP";
        case ZIP:   return "ZIP";
        case REDUCE:   return "REDUCE";
        case SCAN:   return "SCAN";
        case GENERATE:   return "GENERATE";
        case SYSTEM:   return "SYSTEM";
        case EXTENSION:   return "EXTENSION";
        case NOOP:   return "NOOP";
    }
    return "_ERR-OP_";
}

string operator_text(OPERATOR oper)
{
    switch(oper) {
        case ADD: return "ADD";
        case SUBTRACT: return "SUBTRACT";
        case ARCTAN2: return "ARCTAN2";
        case BITWISE_AND: return "BITWISE_AND";
        case BITWISE_OR: return "BITWISE_OR";
        case BITWISE_XOR: return "BITWISE_XOR";
        case DIVIDE: return "DIVIDE";
        case EQUAL: return "EQUAL";
        case GREATER: return "GREATER";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case LEFT_SHIFT: return "LEFT_SHIFT";
        case LESS: return "LESS";
        case LESS_EQUAL: return "LESS_EQUAL";
        case LOGICAL_AND: return "LOGICAL_AND";
        case LOGICAL_OR: return "LOGICAL_OR";
        case LOGICAL_XOR: return "LOGICAL_XOR";
        case MAXIMUM: return "MAXIMUM";
        case MINIMUM: return "MINIMUM";
        case MOD: return "MOD";
        case MULTIPLY: return "MULTIPLY";
        case NOT_EQUAL: return "NOT_EQUAL";
        case POWER: return "POWER";
        case RIGHT_SHIFT: return "RIGHT_SHIFT";
        case ABSOLUTE: return "ABSOLUTE";
        case ARCCOS: return "ARCCOS";
        case ARCCOSH: return "ARCCOSH";
        case ARCSIN: return "ARCSIN";
        case ARCSINH: return "ARCSINH";
        case ARCTAN: return "ARCTAN";
        case ARCTANH: return "ARCTANH";
        case CEIL: return "CEIL";
        case COS: return "COS";
        case COSH: return "COSH";
        case EXP: return "EXP";
        case EXP2: return "EXP2";
        case EXPM1: return "EXPM1";
        case FLOOR: return "FLOOR";
        case IDENTITY: return "IDENTITY";
        case IMAG: return "IMAG";
        case INVERT: return "INVERT";
        case ISINF: return "ISINF";
        case ISNAN: return "ISNAN";
        case LOG: return "LOG";
        case LOG10: return "LOG10";
        case LOG1P: return "LOG1P";
        case LOG2: return "LOG2";
        case LOGICAL_NOT: return "LOGICAL_NOT";
        case REAL: return "REAL";
        case RINT: return "RINT";
        case SIN: return "SIN";
        case SINH: return "SINH";
        case SQRT: return "SQRT";
        case TAN: return "TAN";
        case TANH: return "TANH";
        case TRUNC: return "TRUNC";
        case DISCARD: return "DISCARD";
        case FREE: return "FREE";
        case SYNC: return "SYNC";
        case NONE: return "NONE";
        case FLOOD: return "FLOOD";
        case RANDOM: return "RANDOM";
        case RANGE: return "RANGE";
        case EXTENSION_OPERATOR: return "EXTENSION_OPERATOR";
    }
    return "_ERR-OPER_";
}

string etype_text(ETYPE etype)
{
    switch(etype) {
        case BOOL: return "BOOL";
        case INT8: return "INT8";
        case INT16: return "INT16";
        case INT32: return "INT32";
        case INT64: return "INT64";
        case UINT8: return "UINT8";
        case UINT16: return "UINT16";
        case UINT32: return "UINT32";
        case UINT64: return "UINT64";
        case FLOAT32: return "FLOAT32";
        case FLOAT64: return "FLOAT64";
        case COMPLEX64: return "COMPLEX64";
        case COMPLEX128: return "COMPLEX128";
        case PAIRLL: return "PAIRLL";
    }
    return "_ERR-ETYPE_";
}

string etype_text_shand(ETYPE etype)
{
    switch(etype) {
        case BOOL: return string("z");
        case INT8: return string("b");
        case INT16: return string("s");
        case INT32: return string("i");
        case INT64: return string("l");
        case UINT8: return string("B");
        case UINT16: return string("S");
        case UINT32: return string("I");
        case UINT64: return string("L");
        case FLOAT32: return string("f");
        case FLOAT64: return string("d");
        case COMPLEX64: return string("c");
        case COMPLEX128: return string("C");
        case PAIRLL: return string("P");
    }
    return string("_ERR-ETYPE-SH_");
}

string etype_to_ctype_text(ETYPE etype)
{
    switch(etype) {
        case BOOL: return "unsigned char";
        case INT8: return "int8_t";
        case INT16: return "int16_t";
        case INT32: return "int32_t";
        case INT64: return "int64_t";
        case UINT8: return "uint8_t";
        case UINT16: return "uint16_t";
        case UINT32: return "uint32_t";
        case UINT64: return "uint64_t";
        case FLOAT32: return "float";
        case FLOAT64: return "double";
        case COMPLEX64: return "float complex";
        case COMPLEX128: return "double complex";
        case PAIRLL: return "pair_LL";
    }
    return "_ERR-EtoC_";
}

string layout_text(LAYOUT layout)
{
    switch(layout) {
        case SCALAR_CONST:
            return "SCALAR_CONST";
        case CONTRACTABLE:
            return "CONTRACTABLE";
        case SCALAR:
            return "SCALAR";
        case CONTIGUOUS:
            return "CONTIGUOUS";
        case CONSECUTIVE:
            return "CONSECUTIVE";
        case STRIDED:
            return "STRIDED";
        case SPARSE:
            return "SPARSE";
    }
    return "_ERR-LAYOUT_";
}

string layout_text_shand(LAYOUT layout)
{
    switch(layout) {
        case SCALAR_CONST:
            return "CNST";
        case CONTRACTABLE:
            return "TEMP";
        case SCALAR:
            return "SCLR";
        case CONTIGUOUS:
            return "CONT";
        case CONSECUTIVE:
            return "CONS";
        case STRIDED:
            return "STRD";
        case SPARSE:
            return "SPRS";
    }
    return "_ERR-LAYOUT-SH_";
}



}}

