#ifndef __SETJMP_H
#define __SETJMP_H
struct __jmp_buf_tag
  {
    /* NOTE: The machine-dependent definitions of `__sigsetjmp'
       assume that a `jmp_buf' begins with a `__jmp_buf' and that
       `__mask_was_saved' follows it.  Do not move these members
       or add others before it.  */
    int __jmpbuf;         /* Calling environment.  */
    int __mask_was_saved;       /* Saved the signal mask?  */
    int __saved_mask;    /* Saved signal mask.  */
  };
typedef struct __jmp_buf_tag jmp_buf[1];

#endif
