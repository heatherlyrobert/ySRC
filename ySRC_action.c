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
   /*---(defense)------------------------*/
   if (s_cur->cpos <  0)            return -1;
   if (s_cur->cpos >  s_cur->npos)  return -2;
   /*---(replace)------------------------*/
   if (a_key == G_CHAR_NULL)       a_key = '\0';
   s_cur->contents [s_cur->cpos] = a_key;
   /*---(handle tail replaces)-----------*/
   if (s_cur->cpos >= s_cur->npos) {
      s_cur->contents [s_cur->cpos + 1] = '\0';
      ++s_cur->npos;
   }
   /*---(complete)-----------------------*/
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
   if (s_cur->npos > 0 && s_cur->cpos > s_cur->npos)  return -2;
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



/*====================------------------------------------====================*/
/*===----                  selection source actions                    ----===*/
/*====================------------------------------------====================*/
static void  o___SELECT__________o () { return; }

char         /*-> process keys for register actions --[ ------ [gz.320.011.02]*/ /*-[01.0000.113.!]-*/ /*-[--.---.---.--]-*/
ysrc_clear_select       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   short       x_beg       =    0;
   short       x_end       =    0;
   /*---(defense)------------------------*/
   --rce;  if (s_cur->npos <= 0)  return rce;
   /*---(prepare)------------------------*/
   rc = ysrc_select_curr (&x_beg, &x_end, NULL);
   --rce;  if (rc < 0)  return rce;
   /*---(begin)--------------------------*/
   rc = ysrc_sundo_beg ();
   --rce;  if (rc < 0)  return rce;
   /*---(act)----------------------------*/
   for (i = x_beg; i <= x_end; ++i) {
      s_cur->cpos = i;
      rc = ysrc_sundo_add ('x', 'l', i, s_cur->contents [i], G_CHAR_STORAGE);
      if (rc < 0)  break;
      rc = ysrc_replace_one (G_CHAR_STORAGE);
      if (rc < 0)  break;
   }
   --rce;  if (rc < 0)  return rce;
   /*---(end)----------------------------*/
   ysrc_sundo_end ();
   --rce;  if (rc < 0)  return rce;
   /*---(complete)-----------------------*/
   return rc;
}

char         /*-> process keys for register action ---[ ------ [gz.430.031.02]*/ /*-[01.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_delete_select      (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_diff      =   0;
   int         i           =   0;
   short       x_beg       =   0;
   short       x_end       =   0;
   /*---(defense)------------------------*/
   --rce;  if (s_cur->npos <= 0)  return rce;
   /*---(prepare)------------------------*/
   rc = ysrc_select_curr (&x_beg, &x_end, NULL);
   --rce;  if (rc < 0)  return rce;
   /*---(short-path)---------------------*/
   rc = ysrc_sundo_beg ();
   --rce;  if (rc < 0)  return rce;
   /*---(act)----------------------------*/
   for (i = x_beg; i <= x_end; ++i) {
      s_cur->cpos = x_beg;
      rc = ysrc_sundo_add ('d', 'l', x_beg, s_cur->contents [x_beg], G_CHAR_STORAGE);
      if (rc < 0)  break;
      rc = ysrc_delete_one ();
      if (rc < 0)  break;
   }
   --rce;  if (rc < 0)  return rce;
   /*---(end)----------------------------*/
   rc = ysrc_sundo_end ();
   --rce;  if (rc < 0)  return rce;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> complex delete action --------------[ ------ [gz.430.031.02]*/ /*-[01.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_multi_pure         (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        i           =    0;
   short       x_len       =    0;
   char        x_pos       =    0;
   char        x_last      =  '-';
   char        x_left      =  '-';
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   UPDATE_BEFORE_CHANGES;
   /*---(defense)------------------------*/
   DEBUG_EDIT   yLOG_value   ("s_live"    , ysrc_select_islive ());
   --rce;  if (ysrc_select_islive ()) {
      DEBUG_EDIT   yLOG_note    ("function only handles non-selected text");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_EDIT   yLOG_char    ("a_major"   , chrvisible (a_major));
   --rce;  if (a_major == 0 || strchr ("dx", a_major) == NULL) {
      DEBUG_EDIT   yLOG_note    ("source only allows right and left");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_EDIT   yLOG_char    ("a_minor"   , chrvisible (a_minor));
   --rce;  if (a_minor == 0 || strchr ("HhLlWwBbEe0$", a_minor) == NULL) {
      DEBUG_EDIT   yLOG_note    ("source only allows right and left");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_EDIT   yLOG_value   ("npos"      , s_cur->npos);
   --rce;  if (s_cur->npos <= 0) {
      DEBUG_EDIT   yLOG_note    ("nothing text to delete");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_EDIT   yLOG_value   ("cpos"      , s_cur->cpos);
   --rce;  if (a_minor == 'h' && s_cur->cpos <= 0) {
      DEBUG_EDIT   yLOG_note    ("nothing left to delete");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_minor == 'l' && s_cur->cpos >= s_cur->npos - 1) {
      DEBUG_EDIT   yLOG_note    ("nothing right to delete");
      DEBUG_EDIT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   switch (a_minor) {
   case 'H' : case 'L' : x_len = 5;                               break;
   case 'h' : case 'l' : x_len = 1;                               break;
   case 'w' : case 'W' : ysrc_word_next (a_minor, NULL, &x_len);
                         if (x_len > 0)  --x_len;
                         break;
   case 'b' : case 'B' : ysrc_word_prev (a_minor, NULL, &x_len);  break;
   case 'e' : case 'E' : ysrc_word_end  (a_minor, NULL, &x_len);  break;
   case '0' : x_len = s_cur->cpos + 1; s_cur->cpos = 0;           break;
   case '$' : x_len = s_cur->npos - s_cur->cpos;                  break;
   }
   DEBUG_EDIT   yLOG_value   ("x_len"     , x_len);
   if (strchr ("HhBb0" , a_minor) != NULL)  x_left = 'y';
   if (strchr ("HhBb0$", a_minor) != NULL)  x_pos  = s_cur->cpos;
   else                                     x_pos  = s_cur->cpos + x_len;
   DEBUG_EDIT   yLOG_value   ("x_pos"     , x_pos);
   /*---(end)----------------------------*/
   rc = ysrc_sundo_beg ();
   --rce;  for (i = 0; i <  x_len; ++i) {
      if (a_major == 'd') {
         if (strchr ("<" , a_minor) != NULL)  --s_cur->cpos;
         if (s_cur->cpos < 0)                 { rc = rce; break; }
         if (s_cur->npos > 0 && s_cur->cpos >= s_cur->npos - 1)  { x_last = 'y'; }
         rc = ysrc_sundo_add (a_major, 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_KEY_NULL);
         if (rc < 0)  break;
         rc = ysrc_delete_one ();
         if (rc < 0)  break;
         if (x_left != 'y' && x_last == 'y')  { rc = rce;  break; }
         if (strchr ("HhBb", a_minor) != NULL && x_last != 'y')  --s_cur->cpos;
      } else {
         if (x_left == 'y' && s_cur->cpos < 0)                 { rc = rce; break; }
         if (x_left != 'y' && s_cur->npos > 0 && s_cur->cpos >= s_cur->npos - 1)  { x_last = 'y'; }
         rc = ysrc_sundo_add (a_major, 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_CHAR_STORAGE);
         if (rc < 0)  break;
         rc = ysrc_replace_one (G_CHAR_STORAGE);
         if (rc < 0)  break;
         if (x_last == 'y')  { rc = rce;  break; }
         if (strchr ("HhBb", a_minor) != NULL)  --s_cur->cpos;
         else                                   ++s_cur->cpos;
      }
   }
   ysrc_sundo_end ();
   /*---(wrapup)-------------------------*/
   UPDATE_AFTER_CHANGES;
   if (a_minor == '0')  s_cur->cpos = x_pos;
   if (strchr ("0$"  , a_minor) != NULL)  yKEYS_repeat_reset ();
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                  register source actions                     ----===*/
/*====================------------------------------------====================*/
static void  o___REGISTER________o () { return; }

char         /*-> process keys for register actions --[ ------ [ge.640.051.04]*/ /*-[00.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_copy              (void)
{
   yKEYS_repeat_reset ();
   ysrc_sreg_save     ();
   ysrc_select_reset  (G_SREG_BEG);
   return 0;
}

char
ysrc_cut               (void)
{
   yKEYS_repeat_reset ();
   ysrc_sreg_save     ();
   ysrc_delete_select ();
   ysrc_select_reset  (G_SREG_BEG);
   return 0;
}

char
ysrc_delete            (void)
{
   yKEYS_repeat_reset ();
   ysrc_sreg_save     ();
   ysrc_delete_select ();
   ysrc_select_reset  (G_SREG_BEG);
   return 0;
}

char         /*-> insert/append register contents ----[ ------ [gz.640.151.11]*/ /*-[01.0000.213.!]-*/ /*-[--.---.---.--]-*/
ysrc_paste              (char a_dir)
{
   /*---(locals)-----------+-----------+-*/
   short       x_dlen      =   0;
   char        x_data      [LEN_RECD];
   short       x_beg       =   0;
   short       x_end       =   0;
   int         i           =   0;
   /*---(defense)------------------------*/
   if (a_dir == 0 || strchr ("ai", a_dir) == NULL)   return 0;
   /*---(get register data)--------------*/
   ysrc_sreg_fetch  (&x_dlen, x_data);
   /*---(handle if selected)-------------*/
   if (ysrc_select_islive ()) {
      ysrc_select_curr (&x_beg, &x_end, NULL);
      if (a_dir == 'a') {
         s_cur->cpos = x_end;
      } else {
         s_cur->cpos = x_beg;
      }
   }
   /*---(set the start)------------------*/
   ysrc_sundo_beg ();
   /*---(walk the data)------------------*/
   for (i = 0; i < x_dlen; ++i) {
      if (a_dir == 'a') {
         ysrc_sundo_add (G_KEY_SPACE, 'a', s_cur->cpos, G_CHAR_NULL, x_data [i]);
         ysrc_append_one (x_data [i]);
      } else  {
         ysrc_sundo_add (G_KEY_SPACE, 'i', s_cur->cpos, G_CHAR_NULL, x_data [i]);
         ysrc_insert_one (x_data [i]);
         ++s_cur->cpos;
      }
   }
   /*---(set the end)--------------------*/
   ysrc_sundo_end ();
   /*---(complete)-----------------------*/
   return 0;
}

char         /*-> replace with register text ---------[ ------ [gz.740.061.21]*/ /*-[01.0000.013.!]-*/ /*-[--.---.---.--]-*/
ysrc_replace            (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =   0;
   short       x_dlen      =   0;
   char        x_data      [LEN_RECD];
   short       x_beg       =   0;
   short       x_end       =   0;
   int         x_len       =   0;
   int         i           =   0;
   char        x_ch        = G_CHAR_STORAGE;
   char        x_append    = '-';
   /*---(header)-------------------------*/
   DEBUG_EDIT   yLOG_enter   (__FUNCTION__);
   /*---(get register data)--------------*/
   ysrc_sreg_fetch  (&x_dlen, x_data);
   DEBUG_EDIT   yLOG_value   ("x_dlen"    , x_dlen);
   DEBUG_EDIT   yLOG_info    ("x_data"    , x_data);
   /*---(handle if selected)-------------*/
   DEBUG_EDIT   yLOG_value   ("islive"    , ysrc_select_islive ());
   if (ysrc_select_islive ()) {
      ysrc_select_curr (&x_beg, &x_end, NULL);
      x_len = x_end - x_beg + 1;
   } else {
      x_beg = s_cur->cpos;
      x_len = x_dlen;
      x_end = x_beg + x_len - 1;
   }
   DEBUG_EDIT   yLOG_value   ("x_beg"     , x_beg);
   DEBUG_EDIT   yLOG_value   ("x_end"     , x_end);
   DEBUG_EDIT   yLOG_value   ("x_len"     , x_len);
   /*---(set the start)------------------*/
   ysrc_sundo_beg ();
   /*---(walk the data)------------------*/
   for (i = 0; i < x_len; ++i) {
      /*---(get char)--------------------*/
      if (i < x_dlen)    x_ch = x_data [i];
      else               x_ch = G_CHAR_STORAGE;
      DEBUG_EDIT   yLOG_complex ("PASS"      , "%2d, %c, %c, %2d, %2d", i, chrvisible (s_cur->contents [s_cur->cpos]), x_ch, s_cur->cpos, s_cur->npos);
      /*---(add to source)---------------*/
      if (x_append == '-' && s_cur->cpos < s_cur->npos) {
         DEBUG_EDIT   yLOG_note    ("replace mode");
         ysrc_sundo_add   (G_KEY_SPACE, 'r', s_cur->cpos, s_cur->contents [s_cur->cpos], x_ch);
         rc = ysrc_replace_one (x_ch);
         ++s_cur->cpos;
      } else {
         DEBUG_EDIT   yLOG_note    ("append mode");
         x_append = 'y';
         s_cur->cpos = s_cur->npos - 1;
         DEBUG_EDIT   yLOG_complex ("new pass"  , "%2d, %c, %c, %2d, %2d", i, chrvisible (s_cur->contents [s_cur->cpos]), x_ch, s_cur->cpos, s_cur->npos);
         ysrc_sundo_add   (G_KEY_SPACE, 'a', s_cur->cpos, G_CHAR_NULL, x_ch);
         rc = ysrc_append_one  (x_ch);
      }
      DEBUG_EDIT   yLOG_complex ("after"     , "%3d, %2d, %2d", rc, s_cur->cpos, s_cur->npos);
      /*---(done)------------------------*/
   }
   /*---(set the end)--------------------*/
   ysrc_sundo_end ();
   ysrc_select_reset  (G_SREG_CURR);
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}


