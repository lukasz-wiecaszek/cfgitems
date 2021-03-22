/* SPDX-License-Identifier: MIT */
/**
 * @file cfgitems_tests_without_cfgfile.c
 *
 * @author Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>
 */

/*===========================================================================*\
 * system header files
\*===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================================================================*\
 * project header files
\*===========================================================================*/
#include <cfgitems.h>
#include <gtest/gtest.h>

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

/*===========================================================================*\
 * local (internal linkage) object definitions
\*===========================================================================*/

/*===========================================================================*\
 * static inline (internal linkage) function definitions
\*===========================================================================*/

/*===========================================================================*\
 * global (external linkage) function definitions
\*===========================================================================*/
CFGITEMS_DEFINE_BOOL(CFGITEMS_GLOBAL_MODULE, multithreaded, false);
CFGITEMS_DEFINE_BOOL(submodule, multithreaded, true);

TEST(cfgitems, cfgitems_get_bool)
{
    bool value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_bool(NULL, "multithreaded", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_bool(NULL, "multithreaded", &value));
    EXPECT_EQ(false, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_bool("submodule", "multithreaded", &value));
    EXPECT_EQ(true, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_bool("x", "multithreaded", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_bool("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_bool("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_bool(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_bool)
{
    bool value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_bool(NULL, "multithreaded", true));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_bool(NULL, "multithreaded", &value));
    EXPECT_EQ(true, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_bool("submodule", "multithreaded", false));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_bool("submodule", "multithreaded", &value));
    EXPECT_EQ(false, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_bool("x", "multithreaded", false));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_bool("submodule", "x", false));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_bool("x", "x", false));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_bool(NULL, NULL, false));
}

CFGITEMS_DEFINE_STRING(CFGITEMS_GLOBAL_MODULE, configuration_file, "mystring1");
CFGITEMS_DEFINE_STRING(submodule, configuration_file, "mystring2");

TEST(cfgitems, cfgitems_get_string)
{
    const char* value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_string(NULL, "configuration_file", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_string(NULL, "configuration_file", &value));
    EXPECT_STREQ("mystring1", value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_string("submodule", "configuration_file", &value));
    EXPECT_STREQ("mystring2", value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_string("x", "configuration_file", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_string("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_string("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_string(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_string)
{
    const char* value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_string(NULL, "configuration_file", "mystring3"));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_string(NULL, "configuration_file", &value));
    EXPECT_STREQ("mystring3", value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_string("submodule", "configuration_file", "mystring4"));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_string("submodule", "configuration_file", &value));
    EXPECT_STREQ("mystring4", value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_string("x", "configuration_file", "mystring5"));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_string("submodule", "x", "mystring5"));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_string("x", "x", "mystring5"));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_string(NULL, NULL, "mystring5"));
}

CFGITEMS_DEFINE_DOUBLE(CFGITEMS_GLOBAL_MODULE, speed, 1.0);
CFGITEMS_DEFINE_DOUBLE(submodule, speed, 2.0);

TEST(cfgitems, cfgitems_get_double)
{
    double value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_double(NULL, "speed", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_double(NULL, "speed", &value));
    EXPECT_EQ(1.0, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_double("submodule", "speed", &value));
    EXPECT_EQ(2.0, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_double("x", "speed", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_double("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_double("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_double(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_double)
{
    double value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_double(NULL, "speed", 3.0));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_double(NULL, "speed", &value));
    EXPECT_EQ(3.0, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_double("submodule", "speed", 4.0));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_double("submodule", "speed", &value));
    EXPECT_EQ(4.0, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_double("x", "speed", 5.0));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_double("submodule", "x", 5.0));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_double("x", "x", 5.0));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_double(NULL, NULL, 5.0));
}

CFGITEMS_DEFINE_S8(CFGITEMS_GLOBAL_MODULE, s8, 1);
CFGITEMS_DEFINE_S8(submodule, s8, 2);

TEST(cfgitems, cfgitems_get_s8)
{
    int8_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s8(NULL, "s8", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s8(NULL, "s8", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s8("submodule", "s8", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s8("x", "s8", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s8("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s8("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s8(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_s8)
{
    int8_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s8(NULL, "s8", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s8(NULL, "s8", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s8("submodule", "s8", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s8("submodule", "s8", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s8("x", "s8", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s8("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s8("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s8(NULL, NULL, 5));
}

CFGITEMS_DEFINE_U8(CFGITEMS_GLOBAL_MODULE, u8, 1);
CFGITEMS_DEFINE_U8(submodule, u8, 2);

TEST(cfgitems, cfgitems_get_u8)
{
    uint8_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u8(NULL, "u8", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u8(NULL, "u8", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u8("submodule", "u8", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u8("x", "u8", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u8("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u8("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u8(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_u8)
{
    uint8_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u8(NULL, "u8", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u8(NULL, "u8", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u8("submodule", "u8", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u8("submodule", "u8", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u8("x", "u8", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u8("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u8("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u8(NULL, NULL, 5));
}

CFGITEMS_DEFINE_S16(CFGITEMS_GLOBAL_MODULE, s16, 1);
CFGITEMS_DEFINE_S16(submodule, s16, 2);

TEST(cfgitems, cfgitems_get_s16)
{
    int16_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s16(NULL, "s16", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s16(NULL, "s16", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s16("submodule", "s16", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s16("x", "s16", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s16("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s16("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s16(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_s16)
{
    int16_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s16(NULL, "s16", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s16(NULL, "s16", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s16("submodule", "s16", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s16("submodule", "s16", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s16("x", "s16", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s16("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s16("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s16(NULL, NULL, 5));
}

CFGITEMS_DEFINE_U16(CFGITEMS_GLOBAL_MODULE, u16, 1);
CFGITEMS_DEFINE_U16(submodule, u16, 2);

TEST(cfgitems, cfgitems_get_u16)
{
    uint16_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u16(NULL, "u16", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u16(NULL, "u16", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u16("submodule", "u16", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u16("x", "u16", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u16("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u16("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u16(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_u16)
{
    uint16_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u16(NULL, "u16", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u16(NULL, "u16", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u16("submodule", "u16", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u16("submodule", "u16", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u16("x", "u16", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u16("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u16("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u16(NULL, NULL, 5));
}

CFGITEMS_DEFINE_S32(CFGITEMS_GLOBAL_MODULE, s32, 1);
CFGITEMS_DEFINE_S32(submodule, s32, 2);

TEST(cfgitems, cfgitems_get_s32)
{
    int32_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s32(NULL, "s32", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s32(NULL, "s32", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s32("submodule", "s32", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s32("x", "s32", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s32("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s32("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s32(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_s32)
{
    int32_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s32(NULL, "s32", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s32(NULL, "s32", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s32("submodule", "s32", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s32("submodule", "s32", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s32("x", "s32", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s32("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s32("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s32(NULL, NULL, 5));
}

CFGITEMS_DEFINE_U32(CFGITEMS_GLOBAL_MODULE, u32, 1);
CFGITEMS_DEFINE_U32(submodule, u32, 2);

TEST(cfgitems, cfgitems_get_u32)
{
    uint32_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u32(NULL, "u32", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u32(NULL, "u32", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u32("submodule", "u32", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u32("x", "u32", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u32("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u32("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u32(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_u32)
{
    uint32_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u32(NULL, "u32", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u32(NULL, "u32", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u32("submodule", "u32", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u32("submodule", "u32", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u32("x", "u32", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u32("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u32("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u32(NULL, NULL, 5));
}

CFGITEMS_DEFINE_S64(CFGITEMS_GLOBAL_MODULE, s64, 1);
CFGITEMS_DEFINE_S64(submodule, s64, 2);

TEST(cfgitems, cfgitems_get_s64)
{
    int64_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s64(NULL, "s64", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s64(NULL, "s64", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s64("submodule", "s64", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s64("x", "s64", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s64("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s64("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_s64(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_s64)
{
    int64_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s64(NULL, "s64", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s64(NULL, "s64", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_s64("submodule", "s64", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_s64("submodule", "s64", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s64("x", "s64", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s64("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s64("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_s64(NULL, NULL, 5));
}

CFGITEMS_DEFINE_U64(CFGITEMS_GLOBAL_MODULE, u64, 1);
CFGITEMS_DEFINE_U64(submodule, u64, 2);

TEST(cfgitems, cfgitems_get_u64)
{
    uint64_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u64(NULL, "u64", NULL));

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u64(NULL, "u64", &value));
    EXPECT_EQ(1, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u64("submodule", "u64", &value));
    EXPECT_EQ(2, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u64("x", "u64", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u64("submodule", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u64("x", "x", &value));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_get_u64(NULL, NULL, &value));
}

TEST(cfgitems, cfgitems_set_u64)
{
    uint64_t value;

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u64(NULL, "u64", 3));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u64(NULL, "u64", &value));
    EXPECT_EQ(3, value);

    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_set_u64("submodule", "u64", 4));
    EXPECT_EQ(CFGITEMS_SUCCESS, cfgitems_get_u64("submodule", "u64", &value));
    EXPECT_EQ(4, value);

    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u64("x", "u64", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u64("submodule", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u64("x", "x", 5));
    EXPECT_EQ(CFGITEMS_FAILURE, cfgitems_set_u64(NULL, NULL, 5));
}

int main(int argc, char* argv[])
{
    int retval = EXIT_FAILURE;

    do {
        int status;

        ::testing::InitGoogleTest(&argc, argv);

        status = cfgitems_init(NULL);
        if (status != CFGITEMS_SUCCESS)
        {
            break;
        }

        status = cfgitems_init(NULL);
        if (status == CFGITEMS_SUCCESS)
        {
            break;
        }

        retval = RUN_ALL_TESTS();
    } while (0);

    return retval;
}

/*===========================================================================*\
 * local (internal linkage) function definitions
\*===========================================================================*/

