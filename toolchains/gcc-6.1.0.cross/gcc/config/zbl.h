#ifndef __ZBL_H
#define __ZBL_H
#undef TARGET_ZBL
#define TARGET_ZBL 1
//#define LIB_SPEC "-lc"
#define STARTFILE_SPEC "crt0.o%s crti.o%s crtbegin.o%s"
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"
#undef  NO_IMPLICIT_EXTERN_C
#define NO_IMPLICIT_EXTERN_C 1
#undef TARGET_OS_CPP_BUILTINS
#define LIB_SPEC "-lc"
#define TARGET_OS_CPP_BUILTINS()      \
  do {                                \
    builtin_define ("__myos__");      \
    builtin_define ("__unix__");      \
    builtin_assert ("system=myos");   \
    builtin_assert ("system=unix");   \
    builtin_assert ("system=posix");   \
  } while(0);
#endif
