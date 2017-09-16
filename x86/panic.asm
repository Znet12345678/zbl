[bits 32]
global _entry
extern _error
extern t_readvals
_entry:call t_readvals
call _error

iret
