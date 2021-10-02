/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


tEDIT   s_src;
tEDIT   s_cmd;
tEDIT  *s_cur    = &s_src;


/*====================------------------------------------====================*/
/*===----                      working functions                       ----===*/
/*====================------------------------------------====================*/
static void  o___WORKERS_________o () { return; }

char
ysrc_replace_one        (uchar a_key)
{
   s_cur->contents [s_cur->cpos] = a_key;
   return 0;
}

char
ysrc_delete_one         (void)
{
   int         i           =   0;
   for (i = s_cur->cpos; i < s_cur->npos; ++i) {
      s_cur->contents [i] = s_cur->contents [i + 1];
   }
   --s_cur->npos;
   return 0;
}

char
ysrc_backspace_one      (void)
{
   if (s_cur->cpos <= 0)  return -1;
   --s_cur->cpos;
   ysrc_delete_one  ();
   return 0;
}

char
ysrc_insert_one         (uchar a_key)
{
   int         i           =   0;
   for (i = s_cur->npos + 1; i >= s_cur->cpos; --i) {
      s_cur->contents [i + 1] = s_cur->contents [i];
   }
   ++s_cur->npos;
   ysrc_replace_one (a_key);
   return 0;
}

char
ysrc_append_one         (uchar a_key)
{
   if (s_cur->npos == 0) {
      ysrc_insert_one (a_key);
   } else {
      ++s_cur->cpos;
      ysrc_insert_one (a_key);
   }
   ++s_cur->npos;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      text reg actions                        ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char         /*-> process keys for register actions --[ ------ [gz.320.011.02]*/ /*-[01.0000.113.!]-*/ /*-[--.---.---.--]-*/
ysrc_clear              (char a_major, char a_minor)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =   0;
   int         x_beg       =   0;
   int         x_end       =   0;
   /*---(prepare)------------------------*/
   ysrc_select_curr (&x_beg, &x_end, NULL);
   /*---(begin)--------------------------*/
   ysrc_sundo_beg ();
   /*---(act)----------------------------*/
   for (i = x_beg; i <= x_end; ++i) {
      s_cur->cpos = i;
      ysrc_sundo_add ('x', 'l', i, s_cur->contents [i], G_CHAR_STORAGE);
      ysrc_replace_one (G_CHAR_STORAGE);
   }
   /*---(end)----------------------------*/
   ysrc_sundo_end ();
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> process keys for register action ---[ ------ [gz.430.031.02]*/ /*-[01.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_delete             (char a_major, char a_minor)
{
   /*---(locals)-----------+-----------+-*/
   int         x_diff      =   0;
   int         i           =   0;
   int         x_beg       =   0;
   int         x_end       =   0;
   /*---(prepare)------------------------*/
   ysrc_select_curr (&x_beg, &x_end, NULL);
   /*---(short-path)---------------------*/
   ysrc_sundo_beg ();
   /*---(short-path)---------------------*/
   if (ysrc_select_isdead () && a_minor == 'D') {
      if (s_cur->cpos <= 0)  return -1;
      --s_cur->cpos;
      ysrc_sundo_add (a_major, tolower (a_minor), i, s_cur->contents [i], G_KEY_NULL);
      ysrc_delete_one ();
      return 0;
   }
   else {
      /*---(set size)-----------------------*/
      x_diff  = x_end - x_beg + 1;
      /*---(delete)-------------------------*/
      s_cur->cpos = x_beg;
      for (i = 0; i < x_diff; ++i) {
         ysrc_sundo_add ('d', 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_KEY_NULL);
         ysrc_delete_one ();
      }
   }
   ysrc_sundo_end ();
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> process keys for register actions --[ ------ [ge.640.051.04]*/ /*-[00.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_copy              (void)
{
   /*---(locals)-----------+-----------+-*/
   char       *x_start     = NULL;
   int         x_len       =   0;
   char        x_data      [LEN_RECD];
   int         x_beg       =   0;
   int         x_end       =   0;
   /*---(prepare)------------------------*/
   ysrc_select_curr (&x_beg, &x_end, NULL);
   /*---(set size)-----------------------*/
   x_start = s_cur->contents + x_beg;
   x_len   = x_end - x_beg + 1;
   /*---(copy)---------------------------*/
   strlcpy (x_data, x_start, x_len + 1);
   ysrc_sreg_save (s_cur->label, x_data);
   ysrc_select_curr (NULL, NULL, &(s_cur->cpos));
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> replace with register text ---------[ ------ [gz.740.061.21]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
ysrc_replace            (void)
{
   /*---(locals)-----------+-----------+-*/
   int         x_dlen      =   0;
   char        x_data      [LEN_RECD];
   int         x_beg       =   0;
   int         x_end       =   0;
   int         x_len       =   0;
   int         i           =   0;
   char        x_ch        = G_CHAR_STORAGE;
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   /*---(get register data)--------------*/
   ysrc_sreg_fetch  (&x_dlen, x_data);
   DEBUG_EDIT   yLOG_value   ("x_dlen"    , x_dlen);
   DEBUG_EDIT   yLOG_info    ("x_data"    , x_data);
   ysrc_select_curr (&x_beg, &x_end, NULL);
   DEBUG_EDIT   yLOG_value   ("x_beg"     , x_beg);
   DEBUG_EDIT   yLOG_value   ("x_end"     , x_end);
   s_cur->cpos = x_beg;
   /*---(set the length)-----------------*/
   DEBUG_EDIT   yLOG_value   ("islive"    , ysrc_select_islive ());
   if (ysrc_select_islive ())   x_len = x_end - x_beg + 1;
   else                         x_len = x_dlen;
   DEBUG_EDIT   yLOG_value   ("x_len"     , x_len);
   /*---(set the start)------------------*/
   ysrc_sundo_beg ();
   for (i = 0; i < x_len; ++i) {
      /*---(get char)--------------------*/
      if (i < x_dlen)    x_ch = x_data [i];
      else               x_ch = G_CHAR_STORAGE;
      /*---(add to source)---------------*/
      if (s_cur->cpos < s_cur->npos) {
         ysrc_sundo_add   (G_KEY_SPACE, 'r', s_cur->cpos, s_cur->contents [s_cur->cpos], x_ch);
         ysrc_replace_one (x_ch);
         ++s_cur->cpos;
      } else {
         ysrc_sundo_add   (G_KEY_SPACE, 'r', s_cur->cpos, s_cur->contents [s_cur->cpos], x_ch);
         ysrc_append_one  (x_ch);
      }
      /*---(done)------------------------*/
   }
   ysrc_sundo_end ();
   --s_cur->cpos;
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> insert/append register contents ----[ ------ [gz.640.151.11]*/ /*-[01.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_paste              (char a_dir)
{
   /*---(locals)-----------+-----------+-*/
   int         x_dlen      =   0;
   char        x_data      [LEN_RECD];
   int         x_beg       =   0;
   int         x_end       =   0;
   int         i           =   0;
   /*---(get register data)--------------*/
   ysrc_sreg_fetch  (&x_dlen, x_data);
   ysrc_select_curr (&x_beg, &x_end, NULL);
   s_cur->cpos = x_beg;
   /*---(prepare)------------------------*/
   if (a_dir == 'a' && s_cur->npos > 0) {
      ++s_cur->cpos;
   }
   /*---(set the start)------------------*/
   ysrc_sundo_beg ();
   for (i = 0; i < x_dlen; ++i) {
      ysrc_sundo_add (G_KEY_SPACE, 'i', s_cur->cpos, G_CHAR_NULL, x_data [i]);
      ysrc_insert_one (x_data [i]);
      ++s_cur->cpos;
   }
   ysrc_sundo_end ();
   --s_cur->cpos;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> remove existing and add new --------[ ------ [gz.640.151.11]*/ /*-[01.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_swap_all           (char *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         x_len       =    0;
   char        x_old       [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   DEBUG_EDIT   yLOG_info    ("a_new"     , a_new);
   /*---(start undo logging)-------------*/
   ysrc_sundo_beg ();
   /*---(remove existing)----------------*/
   DEBUG_EDIT   yLOG_value   ("npos"      , s_cur->npos);
   x_len = s_cur->npos;
   s_cur->cpos = 0;
   for (i = 0; i < x_len; ++i) {
      DEBUG_EDIT   yLOG_complex ("delete"    , "%2d, %c", s_cur->cpos, s_cur->contents [s_cur->cpos]);
      ysrc_sundo_add ('d', 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_CHAR_NULL);
      ysrc_delete_one ();
   }
   /*---(add new)------------------------*/
   x_len  = strllen (a_new, LEN_RECD);
   for (i = 0; i < x_len; ++i) {
      DEBUG_EDIT   yLOG_complex ("append"    , "%2d, %c", i, a_new [i]);
      ysrc_append_one (a_new [i]);
      ysrc_sundo_add (G_KEY_SPACE, 'a', i, G_KEY_NULL, a_new [i]);
   }
   /*---(stop undo logging)--------------*/
   ysrc_sundo_end ();
   s_cur->cpos = 1;
   SOURCE__done ();
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}

int   ysrc_cpos        (void) { return s_cur->cpos; }
int   ysrc_npos        (void) { return s_cur->npos; }
char *ysrc_contents    (void) { return s_cur->contents; }


