/* SPDX-License-Identifier: MIT */
/**
 * @file cfgitems.h
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 */

#ifndef _CFGITEMS_H_
#define _CFGITEMS_H_

#if defined(__cplusplus)
    #define LTS_EXTERN extern "C"
#else
    #define LTS_EXTERN extern
#endif

/*===========================================================================*\
 * system header files
\*===========================================================================*/
#include <stdbool.h>
#include <stdint.h>

/*===========================================================================*\
 * project header files
\*===========================================================================*/

/*===========================================================================*\
 * preprocessor #define constants and macros
\*===========================================================================*/
#if !defined(CFGITEMS_SECTION_PREFIX)
    #define CFGITEMS_SECTION_PREFIX lts_cfgitems
#endif

#define CFGITEMS_SUCCESS  (0)
#define CFGITEMS_FAILURE (-1)

#define CFGITEMS_ALIGN 32
#define CFGITEMS_STRINGIFY(x) #x
#define CFGITEMS_XSTR(x) CFGITEMS_STRINGIFY(x)
#define CFGITEMS_CONCATENATE(a, b) a ## b

#define CFGITEMS_CONCATENATE_SECTION_START(section)  CFGITEMS_CONCATENATE(__start_, section)
#define CFGITEMS_CONCATENATE_SECTION_END(section)    CFGITEMS_CONCATENATE(__stop_, section)

#define CFGITEMS_SECTION_NAME   CFGITEMS_XSTR(CFGITEMS_SECTION_PREFIX)
#define CFGITEMS_SECTION_START  CFGITEMS_CONCATENATE_SECTION_START(CFGITEMS_SECTION_PREFIX)
#define CFGITEMS_SECTION_END    CFGITEMS_CONCATENATE_SECTION_END(CFGITEMS_SECTION_PREFIX)

#define CFGITEMS_GLOBAL_MODULE _

#define CFGITEMTYPES                        \
    CFGITEMTYPE(void*,           UNDEFINED) \
    CFGITEMTYPE(bool,            BOOL)      \
    CFGITEMTYPE(const char*,     STRING)    \
    CFGITEMTYPE(double,          DOUBLE)    \
    CFGITEMTYPE(int8_t,          S8)        \
    CFGITEMTYPE(uint8_t,         U8)        \
    CFGITEMTYPE(int16_t,         S16)       \
    CFGITEMTYPE(uint16_t,        U16)       \
    CFGITEMTYPE(int32_t,         S32)       \
    CFGITEMTYPE(uint32_t,        U32)       \
    CFGITEMTYPE(int64_t,         S64)       \
    CFGITEMTYPE(uint64_t,        U64)

#define __CFGITEMS_DEFINE(_module_, _type_, _name_, _default_value_) \
    struct cfgitems cfgitems_ ## _module_ ## _ ## _name_             \
        __attribute__((__section__(CFGITEMS_SECTION_NAME)))          \
        __attribute__((__used__))                                    \
        __attribute__((aligned(CFGITEMS_ALIGN))) =                   \
        {                                                            \
            .module                      = #_module_,                \
            .type                        = CFGITEMS_TYPE_ ## _type_, \
            .name                        = #_name_,                  \
            .value = {. _ ## _type_ ## _ = _default_value_}          \
        }

#define CFGITEMS_DEFINE_BOOL(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, BOOL, _name_, _default_value_)

#define CFGITEMS_DEFINE_STRING(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, STRING, _name_, _default_value_)

#define CFGITEMS_DEFINE_DOUBLE(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, DOUBLE, _name_, _default_value_)

#define CFGITEMS_DEFINE_S8(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, S8, _name_, _default_value_)

#define CFGITEMS_DEFINE_U8(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, U8, _name_, _default_value_)

#define CFGITEMS_DEFINE_S16(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, S16, _name_, _default_value_)

#define CFGITEMS_DEFINE_U16(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, U16, _name_, _default_value_)

#define CFGITEMS_DEFINE_S32(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, S32, _name_, _default_value_)

#define CFGITEMS_DEFINE_U32(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, U32, _name_, _default_value_)

#define CFGITEMS_DEFINE_S64(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, S64, _name_, _default_value_)

#define CFGITEMS_DEFINE_U64(_module_, _name_, _default_value_) \
    __CFGITEMS_DEFINE(_module_, U64, _name_, _default_value_)

/*===========================================================================*\
 * global type definitions
\*===========================================================================*/
enum cfgitems_type
{
#define CFGITEMTYPE(TYPE, TYPENAME) CFGITEMS_TYPE_ ## TYPENAME,
    CFGITEMTYPES
#undef CFGITEMTYPE
};

union cfgitems_any
{
#define CFGITEMTYPE(TYPE, TYPENAME) TYPE _ ## TYPENAME ## _;
    CFGITEMTYPES
#undef CFGITEMTYPE
};

struct cfgitems
{
    const char* module;
    enum cfgitems_type type;
    const char* name;
    union cfgitems_any value;
    char strvalue[128];
} __attribute__((aligned(CFGITEMS_ALIGN)));

/*===========================================================================*\
 * static inline (internal linkage) function definitions
\*===========================================================================*/

/*===========================================================================*\
 * global (external linkage) object declarations
\*===========================================================================*/
LTS_EXTERN struct cfgitems CFGITEMS_SECTION_START;
LTS_EXTERN struct cfgitems CFGITEMS_SECTION_END;

/*===========================================================================*\
 * function forward declarations (external linkage)
\*===========================================================================*/
/**
 * Initializes configuration items library.
 *
 * @param[in] filename Path to the file containing configuration items
 *                     in .ini file format.
 *                     Can be NULL in which case it is just not used.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_init(const char* filename);

/**
 * Parses configuration file.
 *
 * @param[in] filename Path to the file containing configuration items
 *                     in .ini file format.
 *                     Can be NULL in which case it is just not used.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_parse(const char* filename);

/**
 * Gets value of 'bool' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_bool(const char* module, const char* name, bool* value);

/**
 * Sets value of 'bool' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_bool(const char* module, const char* name, bool value);

/**
 * Attempts to convert a string to a boolean value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_bool(const char* str, bool* value);

/**
 * Gets value of 'string (const char*)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_string(const char* module, const char* name, const char** value);

/**
 * Sets value of 'string (const char*)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_string(const char* module, const char* name, const char* value);

/**
 * Gets value of 'double' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_double(const char* module, const char* name, double* value);

/**
 * Sets value of 'double' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_double(const char* module, const char* name, double value);

/**
 * Attempts to convert a string to a double value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_double(const char* str, double* value);

/**
 * Gets value of 's8 (int8_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_s8(const char* module, const char* name, int8_t* value);

/**
 * Sets value of 's8 (int8_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_s8(const char* module, const char* name, int8_t value);

/**
 * Attempts to convert a string to a signed 8-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_s8(const char* str, int8_t* value);

/**
 * Gets value of 'u8 (uint8_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_u8(const char* module, const char* name, uint8_t* value);

/**
 * Sets value of 'u8 (uint8_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_u8(const char* module, const char* name, uint8_t value);

/**
 * Attempts to convert a string to an unsigned 8-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_u8(const char* str, uint8_t* value);

/**
 * Gets value of 's16 (int16_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_s16(const char* module, const char* name, int16_t* value);

/**
 * Sets value of 's16 (int16_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_s16(const char* module, const char* name, int16_t value);

/**
 * Attempts to convert a string to a signed 16-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_s16(const char* str, int16_t* value);

/**
 * Gets value of 'u16 (uint16_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_u16(const char* module, const char* name, uint16_t* value);

/**
 * Sets value of 'u16 (uint16_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_u16(const char* module, const char* name, uint16_t value);

/**
 * Attempts to convert a string to an unsigned 16-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_u16(const char* str, uint16_t* value);

/**
 * Gets value of 's32 (int32_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_s32(const char* module, const char* name, int32_t* value);

/**
 * Sets value of 's32 (int32_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_s32(const char* module, const char* name, int32_t value);

/**
 * Attempts to convert a string to a signed 32-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_s32(const char* str, int32_t* value);

/**
 * Gets value of 'u32 (uint32_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_u32(const char* module, const char* name, uint32_t* value);

/**
 * Sets value of 'u32 (uint32_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_u32(const char* module, const char* name, uint32_t value);

/**
 * Attempts to convert a string to an unsigned 32-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_u32(const char* str, uint32_t* value);

/**
 * Gets value of 's64 (int64_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_s64(const char* module, const char* name, int64_t* value);

/**
 * Sets value of 's64 (int64_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_s64(const char* module, const char* name, int64_t value);

/**
 * Attempts to convert a string to a signed 64-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_s64(const char* str, int64_t* value);

/**
 * Gets value of 'u64 (uint64_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Pointer to the variable which will be assigned
 *                   with the value of configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_get_u64(const char* module, const char* name, uint64_t* value);

/**
 * Sets value of 'u64 (uint64_t)' configuration item.
 *
 * @param[in] module Module name the item belongs to.
 * @param[in] name Name of the configuration item.
 * @param[out] value Value of the configuration item.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_set_u64(const char* module, const char* name, uint64_t value);

/**
 * Attempts to convert a string to an unsigned 64-bits-wide integer value.
 *
 * @param[in] str Pointer to the string to be converted.
 * @param[out] value Pointer to the object to be assigned with the converted value.
 *
 * @return CFGITEMS_SUCCESS on success, CFGITEMS_FAILURE value otherwise.
 */
LTS_EXTERN int cfgitems_to_u64(const char* str, uint64_t* value);

#endif /* _CFGITEMS_H_ */
