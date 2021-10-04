/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



/*====================------------------------------------====================*/
/*===----                      working functions                       ----===*/
/*====================------------------------------------====================*/
static void  o___WORKERS_________o () { return; }

char
ysrc_replace_one        (uchar a_key)
{
   if (s_cur->cpos <  0)            return -1;
   if (s_cur->cpos >= s_cur->npos)  return -2;
   s_cur->contents [s_cur->cpos] = a_key;
   return 0;
}

char
ysrc_delete_one         (void)
{
   int         i           =   0;
   if (s_cur->cpos <  0)            return -1;
   if (s_cur->cpos >= s_cur->npos)  return -2;
   if (s_cur->cpos == s_cur->npos - 1) {
      s_cur->contents [s_cur->cpos] = '\0';
      --s_cur->cpos;
   } else {
      for (i = s_cur->cpos; i < s_cur->npos; ++i) {
         s_cur->contents [i] = s_cur->contents [i + 1];
      }
   }
   --s_cur->npos;
   return 0;
}

char
ysrc_backspace_one      (void)
{
   char        rc          =    0;
   if (s_cur->cpos <= 0)            return -1;
   if (s_cur->cpos >= s_cur->npos)  return -2;
   --s_cur->cpos;
   rc = ysrc_delete_one  ();
   return rc;
}

char
ysrc_insert_one         (uchar a_key)
{
   char        rc          =    0;
   int         i           =   0;
   if (s_cur->cpos <  0)            return -1;
   if (s_cur->npos > 0 && s_cur->cpos >= s_cur->npos)  return -2;
   for (i = s_cur->npos; i >= s_cur->cpos; --i) {
      s_cur->contents [i + 1] = s_cur->contents [i];
   }
   ++s_cur->npos;
   rc = ysrc_replace_one (a_key);
   return rc;
}

char
ysrc_insert_and_move    (uchar a_key)
{
   char        rc          =    0;
   rc = ysrc_insert_one (a_key);
   if (s_cur->npos > 0 && s_cur->cpos < s_cur->npos - 1)  ++s_cur->cpos;
   return rc;
}

char
ysrc_append_one         (uchar a_key)
{
   char        rc          =    0;
   if (s_cur->cpos < 0)            return -1;
   if (s_cur->npos > 0 && s_cur->cpos >= s_cur->npos)  return -2;
   if (s_cur->npos > 0)  ++s_cur->cpos;
   ++s_cur->npos; /* required for tail append */
   rc = ysrc_insert_one (a_key);
   if (rc == 0)  --s_cur->npos;
   return rc;
}



