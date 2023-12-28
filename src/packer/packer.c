/*
 * file: packer.c
 * author: Tuhkis (2034)
 * license: CC0; go figure
 *
 * This is the program I use to pack my game assets into
 * one file for simplicity. So far it doesn't compress anything.
 * This is written in ansi C. It should compile on your toaster
 * with a 20-year-old compilers. The code is a buggy mess of
 * loops upon loops and hope that the limits aren't exceeded lol.
 *
 * Do what you like with the parts *I wrote*,
 * the parts from rxi are not owned me and I cannot give
 * permission to use it. I repeat. You are allowed to use
 * this file (packer.c) as you like; Microtar was made by rxi,
 * I do not own it; it is licensed under MIT. Respect that.
 * 
 * Microtar: https://github.com/rxi/microtar
 *
 */

/* Program is almost C++ compliant... */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "dirent.h"
#include "microtar.c"
#include "stdio.h"
#include "sys/stat.h"
#include "util.h"

#define MAX_FILES (512)
#define OUTPUT ((char*)"game.tar")

#ifndef __cplusplus
mut Ptr nullptr = (Ptr)0;
#endif /* __cplusplus */

/** Checks if path is directory */
U8 is_file(const char *path);
/** Prints help text. */
U0 print_help(U0);
/** Packs the specified directory into game.tar. */
S32 pack(const char* path);
/** Main function. */
S32 main(const S32 argc, const char** argv);

U8 is_file(const char *path) {
  mut struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode) == true;
}

U0 print_help(U0) {
  printf((char*)
    "-====- Packer -====-\n"
    "Just run in the folder you want to pack.\n"
    "This will pack everything that isn't in `packer.ignore` or.\n"
    "in a directory that starts with \".\"\n\n"
    "Run like this: `packer .`\n"
    "-==================-\n"
  );
}

S32 pack(const char* path) {
  mut char files[MAX_FILES][KILOBYTE] = {0};
  {
    mut char buf[KILOBYTE * 5000];
    mut U32 i;
    mut U16 newlines = 0;
    mut FILE* f;
    ASSERT(is_file(path));
    f = fopen(path, "r");
    ASSERT(f != nullptr);
    fgets(buf, KILOBYTE * 5000, f);
    fclose(f);
    for (i = 0; i < KILOBYTE * 5000; ++i) {
      if (buf[i] == '\n') ++newlines;
    }
    /* Re-using the iterator is so green... */
    for (i = 0; i < newlines; ++i) {
      
    }
  }
  printf("1st file: %s\n", files[0]);
  return 0;
}

S32 main(const S32 argc, const char** argv) {
  mut U8 i;
  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == (char)'-') {
      switch (argv[i][1]) {
        /* h for help */
        case (char)'h': fallthrough
        case (char)'H':
          print_help();
          break;
        default:
          printf("Unknown option -%c\nStop.\n", argv[i][1]);
          break;
      }
    } else {
      return pack(argv[i]);
    }
  }
  return 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

