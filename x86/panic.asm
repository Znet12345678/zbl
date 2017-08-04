[bits 32]
global _entry
extern panic
extern t_readvals
_entry:call t_readvals
call panic
