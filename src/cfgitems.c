/* SPDX-License-Identifier: MIT */
/**
 * @file cfgitems.c
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 */

/*===========================================================================*\
 * system header files
\*===========================================================================*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <errno.h>

/*===========================================================================*\
 * project header files
\*===========================================================================*/
#include <cfgitems.h>

/*===========================================================================*\
 * preprocessor #define constants and macros
\*===========================================================================*/

/*===========================================================================*\
 * local type definitions
\*===========================================================================*/

/*===========================================================================*\
 * global (external linkage) object definitions
\*===========================================================================*/

/*===========================================================================*\
 * local (internal linkage) function declarations
\*===========================================================================*/
static int cfgitems_compare(const struct cfgitems* l, const struct cfgitems* r);
static void cfgitems_add(size_t n_options, struct cfgitems* it);
static struct cfgitems* cfgitems_find(const char* module, const char* name);
static int cfgitems_parse_configuration_line(const char* module, char* line);
static int cfgitems_parse_configuration_file(const char* filename);

/*===========================================================================*\
 * local (internal linkage) object definitions
\*===========================================================================*/
static const struct cfgitems cfgitems_0
    __attribute__((__section__(CFGITEMS_SECTION_NAME)))
    __attribute__((__used__))
    __attribute__((aligned(CFGITEMS_ALIGN))) = {0};

static struct cfgitems** cfgitems = NULL;
static size_t n_cfgitems = 0;

/*===========================================================================*\
 * static inline (internal linkage) function definitions
\*===========================================================================*/
static inline int cfgitems_strlcpy(char *dest, const char *src, size_t n)
{
    if (n != 0) {
        n--;
        for (size_t i = 0; (i < n) && (*src != '\0'); ++i)
            *dest++ = *src++;
        *dest = '\0';
    }

    return *src == '\0' ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

static inline int cfgitems_strcasecmp(char const* str1, char const* str2)
{
    int d;

    do { d = tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
    } while (d == 0 && *str1++ && *str2++);

    return d;
}

/*===========================================================================*\
 * global (external linkage) function definitions
\*===========================================================================*/
int cfgitems_init(const char* filename)
{
    struct cfgitems* const cfgitems_start_addr = &CFGITEMS_SECTION_START;
    struct cfgitems* const cfgitems_end_addr = &CFGITEMS_SECTION_END;
    ptrdiff_t distance = cfgitems_end_addr - cfgitems_start_addr;
    size_t n = 0;

    if (cfgitems != NULL)
        return CFGITEMS_FAILURE;

    if (--distance < 0)
        return CFGITEMS_FAILURE;

    cfgitems = calloc(distance, sizeof(struct cfgitems*));
    if (cfgitems == NULL)
        return CFGITEMS_FAILURE;

    struct cfgitems* it;
    for (it = cfgitems_start_addr; it < cfgitems_end_addr; ++it)
        if (it->module != NULL)
            cfgitems_add(n++, it);

    n_cfgitems = n;

    return filename ? cfgitems_parse_configuration_file(filename) : CFGITEMS_SUCCESS;
}

int cfgitems_get_bool(const char* module, const char* name, bool* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._BOOL_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_bool(const char* module, const char* name, bool value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._BOOL_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_bool(const char* str, bool* value)
{
    if (!cfgitems_strcasecmp(str, "1") ||
        !cfgitems_strcasecmp(str, "true") ||
        !cfgitems_strcasecmp(str, "on")) {
        if (value)
            *value = true;
        return CFGITEMS_SUCCESS;
    }

    if (!cfgitems_strcasecmp(str, "0") ||
        !cfgitems_strcasecmp(str, "false") ||
        !cfgitems_strcasecmp(str, "off")) {
        if (value)
            *value = false;
        return CFGITEMS_SUCCESS;
    }

    return CFGITEMS_FAILURE;
}

int cfgitems_get_string(const char* module, const char* name, const char** value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._STRING_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_string(const char* module, const char* name, const char* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem) {
        if (strlen(value) >= sizeof(cfgitem->strvalue))
            return CFGITEMS_FAILURE;
        strcpy(cfgitem->strvalue, value);
        cfgitem->value._STRING_ = cfgitem->strvalue;
    }

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_get_double(const char* module, const char* name, double* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._DOUBLE_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_double(const char* module, const char* name, double value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._DOUBLE_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_double(const char* str, double* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        double d;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, d = strtod(str, &endptr);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && ((d == -HUGE_VAL) || (d == HUGE_VAL)))
            break; /* number in string exceeds 'double' range */

        if ((d == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if (value)
            *value = d;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_s8(const char* module, const char* name, int8_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._S8_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_s8(const char* module, const char* name, int8_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._S8_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_s8(const char* str, int8_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        long int li;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, li = strtol(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && ((li == LONG_MAX) || (li == LONG_MIN)))
            break; /* number in string exceeds 'long int' range */

        if ((li == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if ((li < INT8_MIN) || (li > INT8_MAX))
            break; /* number in string exceeds 'int8_t' range */

        if (value)
            *value = (int8_t)li;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_u8(const char* module, const char* name, uint8_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._U8_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_u8(const char* module, const char* name, uint8_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._U8_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_u8(const char* str, uint8_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        unsigned long int uli;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, uli = strtol(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && (uli == ULONG_MAX))
            break; /* number in string exceeds 'unsigned long int' range */

        if ((uli == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if (uli > UINT8_MAX)
            break; /* number in string exceeds 'uint8_t' range */

        if (value)
            *value = (uint8_t)uli;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_s16(const char* module, const char* name, int16_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._S16_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_s16(const char* module, const char* name, int16_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._S16_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_s16(const char* str, int16_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        long int li;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, li = strtol(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && ((li == LONG_MAX) || (li == LONG_MIN)))
            break; /* number in string exceeds 'long int' range */

        if ((li == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if ((li < INT16_MIN) || (li > INT16_MAX))
            break; /* number in string exceeds 'int16_t' range */

        if (value)
            *value = (int16_t)li;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_u16(const char* module, const char* name, uint16_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._U16_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_u16(const char* module, const char* name, uint16_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._U16_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_u16(const char* str, uint16_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        unsigned long int uli;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, uli = strtol(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && (uli == ULONG_MAX))
            break; /* number in string exceeds 'unsigned long int' range */

        if ((uli == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if (uli > UINT16_MAX)
            break; /* number in string exceeds 'uint16_t' range */

        if (value)
            *value = (uint16_t)uli;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_s32(const char* module, const char* name, int32_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._S32_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_s32(const char* module, const char* name, int32_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._S32_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_s32(const char* str, int32_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        long int li;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, li = strtol(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && ((li == LONG_MAX) || (li == LONG_MIN)))
            break; /* number in string exceeds 'long int' range */

        if ((li == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if ((li < INT32_MIN) || (li > INT32_MAX))
            break; /* number in string exceeds 'int32_t' range */

        if (value)
            *value = (int32_t)li;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_u32(const char* module, const char* name, uint32_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._U32_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_u32(const char* module, const char* name, uint32_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._U32_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_u32(const char* str, uint32_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        unsigned long int uli;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, uli = strtol(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && (uli == ULONG_MAX))
            break; /* number in string exceeds 'unsigned long int' range */

        if ((uli == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if (uli > UINT32_MAX)
            break; /* number in string exceeds 'uint32_t' range */

        if (value)
            *value = (uint32_t)uli;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_s64(const char* module, const char* name, int64_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._S64_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_s64(const char* module, const char* name, int64_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._S64_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_s64(const char* str, int64_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        long long int lli;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, lli = strtoll(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && ((lli == LLONG_MAX) || (lli == LLONG_MIN)))
            break; /* number in string exceeds 'long long int' range */

        if ((lli == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if ((lli < INT64_MIN) || (lli > INT64_MAX))
            break; /* number in string exceeds 'int64_t' range */

        if (value)
            *value = (int64_t)lli;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

int cfgitems_get_u64(const char* module, const char* name, uint64_t* value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        if (value)
            *value = cfgitem->value._U64_;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_set_u64(const char* module, const char* name, uint64_t value)
{
    struct cfgitems* cfgitem = cfgitems_find(module, name);

    if (cfgitem)
        cfgitem->value._U64_ = value;

    return cfgitem ? CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
}

int cfgitems_to_u64(const char* str, uint64_t* value)
{
    int retval = CFGITEMS_FAILURE;

    do {
        unsigned long long int ulli;
        char* endptr;

        if (*str == '\0')
            break; /* string is empty */

        errno = 0, ulli = strtoull(str, &endptr, 0);

        if (*endptr != '\0')
            break; /* string is invalid (has unparsed characters) */

        if ((errno == ERANGE) && (ulli == ULLONG_MAX))
            break; /* number in string exceeds 'unsigned long long int' range */

        if ((ulli == 0) && (errno != 0))
            break; /* no valid conversion was performed */

        if (ulli > UINT64_MAX)
            break; /* number in string exceeds 'uint64_t' range */

        if (value)
            *value = (uint64_t)ulli;

        retval = CFGITEMS_SUCCESS;
    } while (0);

    return retval;
}

/*===========================================================================*\
 * local (internal linkage) function definitions
\*===========================================================================*/
static int cfgitems_compare(const struct cfgitems* l, const struct cfgitems* r)
{
    int status;

    status = strcmp(l->module, r->module);
    if (status) {
        if (!strcmp(l->module, CFGITEMS_XSTR(CFGITEMS_GLOBAL_MODULE)))
            return -1;
        else
        if (!strcmp(r->module, CFGITEMS_XSTR(CFGITEMS_GLOBAL_MODULE)))
            return +1;
        else
            return status;
    }

    status = strcmp(l->name, r->name);

    return status;
}

static void cfgitems_add(size_t n_options, struct cfgitems* it)
{
    size_t i;

    for (i = 0; i < n_options; ++i)
        if (cfgitems_compare(it, cfgitems[i]) < 0)
            break;

    memmove(cfgitems + i + 1, cfgitems + i, (n_options - i) * sizeof(struct cfgitems*));
    cfgitems[i] = it;
}

static struct cfgitems* cfgitems_find(const char* module, const char* name)
{
    size_t l = 0;
    size_t r = n_cfgitems - 1;

    if (name == NULL)
        return NULL;

    if (cfgitems == NULL)
        return NULL;

    if (module == NULL)
        module = CFGITEMS_XSTR(CFGITEMS_GLOBAL_MODULE);

    const struct cfgitems x = {.module = module, .name = name};
    while (l <= r) {
        size_t m = (l + r) / 2;
        int status = cfgitems_compare(cfgitems[m], &x);

        if (status < 0)
            l = m + 1;
        else
        if (status > 0)
            r = m - 1;
        else
            return cfgitems[m];
    }

    return NULL;
}

static int cfgitems_parse_configuration_line(const char* module, char* line)
{
    int retval = CFGITEMS_FAILURE;

    static const char* delimiters = "\t =";

    do {
        char* name;
        char* value;
        struct cfgitems* cfgitem;
        int status;

        name = strtok(line, delimiters);
        if (name == NULL)
            break;

        cfgitem = cfgitems_find(module, name);
        if (cfgitem == NULL)
            break;

        value = strtok(NULL, delimiters);
        if (value == NULL)
            break;

        switch (cfgitem->type) {
            case CFGITEMS_TYPE_BOOL:
                status = cfgitems_to_bool(value, &cfgitem->value._BOOL_);
                break;

            case CFGITEMS_TYPE_STRING:
                status = strlen(value) < sizeof(cfgitem->strvalue) ?
                    CFGITEMS_SUCCESS : CFGITEMS_FAILURE;
                if (status == CFGITEMS_SUCCESS) {
                    strcpy(cfgitem->strvalue, value);
                    cfgitem->value._STRING_ = cfgitem->strvalue;
                }
                break;

            case CFGITEMS_TYPE_DOUBLE:
                status = cfgitems_to_double(value, &cfgitem->value._DOUBLE_);
                break;

            case CFGITEMS_TYPE_S8:
                status = cfgitems_to_s8(value, &cfgitem->value._S8_);
                break;

            case CFGITEMS_TYPE_U8:
              status = cfgitems_to_u8(value, &cfgitem->value._U8_);
                break;

            case CFGITEMS_TYPE_S16:
               status = cfgitems_to_s16(value, &cfgitem->value._S16_);
                break;

            case CFGITEMS_TYPE_U16:
               status = cfgitems_to_u16(value, &cfgitem->value._U16_);
                break;

            case CFGITEMS_TYPE_S32:
               status = cfgitems_to_s32(value, &cfgitem->value._S32_);
                break;

            case CFGITEMS_TYPE_U32:
               status = cfgitems_to_u32(value, &cfgitem->value._U32_);
                break;

            case CFGITEMS_TYPE_S64:
               status = cfgitems_to_s64(value, &cfgitem->value._S64_);
                break;

            case CFGITEMS_TYPE_U64:
              status = cfgitems_to_u64(value, &cfgitem->value._U64_);
                break;

            default:
                status = CFGITEMS_FAILURE;
                break;
        }

        retval = status;
    } while (0);

    return retval;
}

static int cfgitems_parse_configuration_file(const char* filename)
{
    FILE *fp;
    int status;
    char buf[1024];
    char module[1024];
    char* line;
    char* c;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "failed to open configuration file '%s': %m\n", filename);
        return CFGITEMS_FAILURE;
    }

    status = cfgitems_strlcpy(module, CFGITEMS_XSTR(CFGITEMS_GLOBAL_MODULE), sizeof(module));
    if (status == CFGITEMS_FAILURE)
        return CFGITEMS_FAILURE;

    while ((c = fgets(buf, sizeof(buf), fp)) != NULL) {

        /* skip leading whitespace characters */
        while (isspace(*c))
            c++;

        /* ignore comments and empty lines */
        if (*c == ';' || *c == '#' || *c == '\n' || *c == '\0')
            continue;

        line = c;

        /* remove trailing whitespace characters */
        c += strlen(line) - 1;
        while (c > line && (isspace(*c)))
            *c-- = '\0';

        if (*line == '[') { /* section definition line */
            line++;
            if (*line == '\0' || *c != ']') /* go to the next line if this one */
                continue;                   /* doesn't look like correct section definition line */
            *c = '\0';

            if (strlen(line) < sizeof(module))
                strcpy(module, line);

            continue;
        }

        cfgitems_parse_configuration_line(module, line);
    }

    fclose(fp);

    return CFGITEMS_SUCCESS;
}
