#ifndef __LOCALE_H
#define __LOCALE_H
struct lconv
{
  /* Numeric (non-monetary) information.  */

  char *decimal_point;          /* Decimal point character.  */
  char *thousands_sep;          /* Thousands separator.  */
  /* Each element is the number of digits in each group;
     elements with higher indices are farther left.
     An element with value CHAR_MAX means that no further grouping is done.
     An element with value 0 means that the previous element is used
     for all groups farther left.  */
  char *grouping;

  /* Monetary information.  */

  /* First three chars are a currency symbol from ISO 4217.
     Fourth char is the separator.  Fifth char is '\0'.  */
  char *int_curr_symbol;
  char *currency_symbol;        /* Local currency symbol.  */
  char *mon_decimal_point;      /* Decimal point character.  */
  char *mon_thousands_sep;      /* Thousands separator.  */
  char *mon_grouping;           /* Like `grouping' element (above).  */
  char *positive_sign;          /* Sign for positive values.  */
  char *negative_sign;          /* Sign for negative values.  */
  char int_frac_digits;         /* Int'l fractional digits.  */
  char frac_digits;             /* Local fractional digits.  */
  /* 1 if currency_symbol precedes a positive value, 0 if succeeds.  */
  char p_cs_precedes;
  /* 1 iff a space separates currency_symbol from a positive value.  */
  char p_sep_by_space;
  /* 1 if currency_symbol precedes a negative value, 0 if succeeds.  */
  char n_cs_precedes;
  /* 1 iff a space separates currency_symbol from a negative value.  */
  char n_sep_by_space;
  /* Positive and negative sign positions:
     0 Parentheses surround the quantity and currency_symbol.
     1 The sign string precedes the quantity and currency_symbol.
     2 The sign string follows the quantity and currency_symbol.
     3 The sign string immediately precedes the currency_symbol.
     4 The sign string immediately follows the currency_symbol.  */
  char p_sign_posn;
  char n_sign_posn;
  char __int_p_cs_precedes;
  char __int_p_sep_by_space;
  char __int_n_cs_precedes;
  char __int_n_sep_by_space;
  char __int_p_sign_posn;
  char __int_n_sign_posn;
};
#define __LC_CTYPE               0
#define __LC_NUMERIC             1
#define __LC_TIME                2
#define __LC_COLLATE             3
#define __LC_MONETARY            4
#define __LC_MESSAGES            5
#define __LC_ALL                 6
#define __LC_PAPER               7
#define __LC_NAME                8
#define __LC_ADDRESS             9
#define __LC_TELEPHONE          10
#define __LC_MEASUREMENT        11
#define __LC_IDENTIFICATION     12
#define LC_CTYPE          __LC_CTYPE
#define LC_NUMERIC        __LC_NUMERIC
#define LC_TIME           __LC_TIME
#define LC_COLLATE        __LC_COLLATE
#define LC_MONETARY       __LC_MONETARY
#define LC_MESSAGES       __LC_MESSAGES
#define LC_ALL            __LC_ALL
#define LC_PAPER          __LC_PAPER
#define LC_NAME           __LC_NAME
#define LC_ADDRESS        __LC_ADDRESS
#define LC_TELEPHONE      __LC_TELEPHONE
#define LC_MEASUREMENT    __LC_MEASUREMENT
#define LC_IDENTIFICATION __LC_IDENTIFICATION

#endif
