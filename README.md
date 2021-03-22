# cfgitems

TODO: Some description would be greatly appreciated.

## How to build this library

First of all this library has only been tested with decent versions of gcc (9.3.0) and clang (10.0.0).
And because it uses compiler specific extensions it might not work with other compilers.
The second thing is, cfgitems uses cmake as a build system. So its integration to a project
which also uses cmake is terribly easy. And as it is with cmake this can be accomplished twofold.
You can either install cfgitems in your system

```
  $ git submodule update --init --recursive
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ sudo make install
  $ sudo ldconfig
```

and then in the project which is going to use cfgitems you add/find it by cmake command

```
  $ find_package(cfgitems REQUIRED)
```

Or, you can include cfgitems as subdirectory or submodule to your project's source tree
and then use cmake's add_subdirectory() command. For example:

```
  $ add_subdirectory(cfgitems)
```

Regardless which method you use to include cfgitems to your project, don't forget to link agains it.

```
   add_executable(your_project_name main.c other.c project.c files.c)
   target_link_libraries(your_project_name cfgitems)
```

## How to use this library

Defining a configuration item is easy. Just use the appropriate macro. Below are some examples:

```
CFGITEMS_DEFINE_BOOL(CFGITEMS_GLOBAL_MODULE, multithreaded, false);
CFGITEMS_DEFINE_BOOL(submodule, multithreaded, true);

CFGITEMS_DEFINE_STRING(CFGITEMS_GLOBAL_MODULE, configuration_file, "mystring1");
CFGITEMS_DEFINE_STRING(submodule, configuration_file, "mystring2");

CFGITEMS_DEFINE_DOUBLE(CFGITEMS_GLOBAL_MODULE, speed, 1.0);
CFGITEMS_DEFINE_DOUBLE(submodule, speed, 2.0);

CFGITEMS_DEFINE_S8(CFGITEMS_GLOBAL_MODULE, my_config_item_s8, 1);
CFGITEMS_DEFINE_S8(submodule, my_config_item_s8, 2);

CFGITEMS_DEFINE_U8(CFGITEMS_GLOBAL_MODULE, my_config_item_u8, 1);
CFGITEMS_DEFINE_U8(submodule, my_config_item_u8, 2);

CFGITEMS_DEFINE_S16(CFGITEMS_GLOBAL_MODULE, my_config_item_s16, 1);
CFGITEMS_DEFINE_S16(submodule, my_config_item_s16, 2);

CFGITEMS_DEFINE_U16(CFGITEMS_GLOBAL_MODULE, my_config_item_u16, 1);
CFGITEMS_DEFINE_U16(submodule, my_config_item_u16, 2);

CFGITEMS_DEFINE_S32(CFGITEMS_GLOBAL_MODULE, my_config_item_s32, 1);
CFGITEMS_DEFINE_S32(submodule, my_config_item_s32, 2);

CFGITEMS_DEFINE_U32(CFGITEMS_GLOBAL_MODULE, my_config_item_u32, 1);
CFGITEMS_DEFINE_U32(submodule, my_config_item_u32, 2);

CFGITEMS_DEFINE_S64(CFGITEMS_GLOBAL_MODULE, my_config_item_s64, 1);
CFGITEMS_DEFINE_S64(submodule, my_config_item_s64, 2);

CFGITEMS_DEFINE_U64(CFGITEMS_GLOBAL_MODULE, my_config_item_u64, 1);
CFGITEMS_DEFINE_U64(submodule, my_config_item_u64, 2);
```
