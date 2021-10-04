/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


tEDIT   s_src;
tEDIT   s_cmd;
tEDIT  *s_cur    = &s_src;



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
   UPDATE_AFTER_CHANGES;
   /*---(complete)-----------------------*/
   DEBUG_EDIT   yLOG_exit    (__FUNCTION__);
   return 0;
}

int   ysrc_cpos        (void) { return s_cur->cpos; }
int   ysrc_npos        (void) { return s_cur->npos; }
char *ysrc_contents    (void) { return s_cur->contents; }

char         /*-> process keys for cell edit mode ----[ leaf   [ge.#R6.23#.HG]*/ /*-[05.0000.102.#]-*/ /*-[--.---.---.--]-*/
ysrc_source_mode        (uchar a_major, uchar a_minor)
{
   /*---(design notes)-------------------*/
   /*
    *   this should imitate the vi-normal mode, but only horizontally.
    *   all formulas and contents should be relatively short or they
    *   should be broken up (formulas) or in a database (text).  so,
    *   forget wordwrap.
    */
   /*---(locals)-----------+-----+-----+-*/
   /*> char        rce         =  -10;                                                          <* 
    *> char        rc          =   -1;                                                          <* 
    *> int         i           =    0;                                                          <* 
    *> int         n           =    0;                                                          <* 
    *> static char x_char      =    0;                                                          <* 
    *> /+---(header)-------------------------+/                                                 <* 
    *> DEBUG_USER   yLOG_enter   (__FUNCTION__);                                                <* 
    *> DEBUG_USER   yLOG_char    ("a_major"   , a_major);                                       <* 
    *> DEBUG_USER   yLOG_char    ("a_minor"   , chrvisible (a_minor));                          <* 
    *> /+---(defenses)-----------------------+/                                                 <* 
    *> DEBUG_USER   yLOG_char    ("mode"      , yMODE_curr ());                                 <* 
    *> DEBUG_USER   yLOG_info    ("valid"     , MODES_ONELINE);                                 <* 
    *> --rce;  if (strchr (MODES_ONELINE, yMODE_curr ()) == NULL ) {                            <* 
    *>    DEBUG_USER   yLOG_note    ("not an acceptable mode");                                 <* 
    *>    yMODE_exit  ();                                                                       <* 
    *>    DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);                                        <* 
    *>    return rce;                                                                           <* 
    *> }                                                                                        <* 
    *> /+---(universal)----------------------+/                                                 <* 
    *> switch (a_minor) {                                                                       <* 
    *> case G_KEY_SPACE  :                                                                      <* 
    *>    DEBUG_USER   yLOG_note    ("space, nothing to do");                                   <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                             <* 
    *>    rc = 0;                                                                               <* 
    *>    break;                                                                                <* 
    *> case G_KEY_ESCAPE :                                                                      <* 
    *>    if (yvikeys_sreg_islive ()) {                                                         <* 
    *>       DEBUG_USER   yLOG_note    ("escape, means get out of selection");                  <* 
    *>       yvikeys_sreg_reset (s_cur->cpos);                                                  <* 
    *>    } else {                                                                              <* 
    *>       DEBUG_USER   yLOG_note    ("escape, means forget and return to previous mode");    <* 
    *>       SOURCE__reset  ();                                                                 <* 
    *>       SOURCE__done   ();                                                                 <* 
    *>       yMODE_exit      ();                                                                <* 
    *>    }                                                                                     <* 
    *>    rc = 0;                                                                               <* 
    *>    break;                                                                                <* 
    *> case G_KEY_RETURN :                                                                      <* 
    *>    DEBUG_USER   yLOG_note    ("enter, means save and return to previous mode");          <* 
    *>    rc = SOURCE__accept ();                                                               <* 
    *>    DEBUG_USER   yLOG_value   ("rc"        , rc);                                         <* 
    *>    SOURCE__done   ();                                                                    <* 
    *>    yMODE_exit      ();                                                                   <* 
    *>    if (rc > 0)   rc = 0;                                                                 <* 
    *>    break;                                                                                <* 
    *> case G_KEY_BTICK :                                                                       <* 
    *>    DEBUG_USER   yLOG_note    ("backtick, means re-wander or move label");                <* 
    *>    yvikeys_map_wander_prep ();                                                           <* 
    *>    rc = yMODE_enter (UMOD_WANDER);                                                       <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                             <* 
    *>    return rc;                                                                            <* 
    *> }                                                                                        <* 
    *> if (rc >= 0) {                                                                           <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                             <* 
    *>    return 0;                                                                             <* 
    *> }                                                                                        <* 
    *> /+---(single letter)------------------+/                                                 <* 
    *> if (a_major == G_KEY_SPACE) {                                                            <* 
    *>    /+---(repeats)---------------------+/                                                 <* 
    *>    if (strchr (g_repeat, a_minor) != 0) {                                                <* 
    *>       yMODE_enter  (PMOD_REPEAT);                                                        <* 
    *>       rc = a_minor;                                                                      <* 
    *>    }                                                                                     <* 
    *>    /+---(select related)--------------+/                                                 <* 
    *>    if (strchr ("yYpP", a_minor) != 0) {                                                  <* 
    *>       DEBUG_USER   yLOG_note    ("switch to a text register mode (yYpP)");               <* 
    *>       yvikeys_sreg_setreg ('"');                                                         <* 
    *>       yMODE_enter (SMOD_SREG);                                                           <* 
    *>       rc = yvikeys_sreg_smode (G_KEY_SPACE, a_minor);                                    <* 
    *>       DEBUG_USER   yLOG_exit    (__FUNCTION__);                                          <* 
    *>       return rc;                                                                         <* 
    *>    }                                                                                     <* 
    *>    if (yvikeys_sreg_islive () && strchr ("xXdD", a_minor) != 0) {                        <* 
    *>       DEBUG_USER   yLOG_note    ("switch to a text register mode (xXdD)");               <* 
    *>       yvikeys_sreg_setreg ('"');                                                         <* 
    *>       yMODE_enter (SMOD_SREG);                                                           <* 
    *>       rc = yvikeys_sreg_smode (G_KEY_SPACE, a_minor);                                    <* 
    *>       DEBUG_USER   yLOG_exit    (__FUNCTION__);                                          <* 
    *>       return rc;                                                                         <* 
    *>    }                                                                                     <* 
    *>    /+---(multikey prefixes)-----------+/                                                 <* 
    *>    if (strchr (g_multisrc, a_minor) != 0) {                                              <* 
    *>       DEBUG_USER   yLOG_note    ("beginning of multi-key command");                      <* 
    *>       DEBUG_USER   yLOG_exit    (__FUNCTION__);                                          <* 
    *>       return a_minor;                                                                    <* 
    *>    }                                                                                     <* 
    *>    /+---(mode changes)----------------+/                                                 <* 
    *>    switch (a_minor) {                                                                    <* 
    *>    case '@'      :                                                                       <* 
    *>       DEBUG_USER   yLOG_note    ("macro execution");                                     <* 
    *>       yMODE_enter  (SMOD_MACRO   );                                                      <* 
    *>       rc = a_minor;                                                                      <* 
    *>       break;                                                                             <* 
    *>    case 'q'      :                                                                       <* 
    *>       IF_MACRO_RECORDING {                                                               <* 
    *>          DEBUG_USER   yLOG_note    ("end macro recording");                              <* 
    *>          rc = yMACRO_rec_end ();                                                         <* 
    *>       } else {                                                                           <* 
    *>          DEBUG_USER   yLOG_note    ("begin macro recording");                            <* 
    *>          yMODE_enter  (SMOD_MACRO   );                                                   <* 
    *>          rc = a_minor;                                                                   <* 
    *>       }                                                                                  <* 
    *>       break;                                                                             <* 
    *>    case 'Q'      :                                                                       <* 
    *>       DEBUG_USER   yLOG_note    ("reset macro recording");                               <* 
    *>       rc = yMACRO_reset_all ();                                                          <* 
    *>       break;                                                                             <* 
    *>    case  'v' :                                                                           <* 
    *>       if (yvikeys_sreg_islive ())                                                        <* 
    *>          s_cur->cpos = yvikeys_sreg_reverse  ();                                         <* 
    *>       else                                                                               <* 
    *>          yvikeys_sreg_makelive ();                                                       <* 
    *>       rc     = 0;                                                                        <* 
    *>       break;                                                                             <* 
    *>    case  '#' :                                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("current char find");                                   <* 
    *>       x_char = s_cur->contents [s_cur->cpos];                                            <* 
    *>       rc = ysrc_move_char_next (x_char);                                                    <* 
    *>       break;                                                                             <* 
    *>    case  'n' :                                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("char find next");                                      <* 
    *>       rc = ysrc_move_char_next (x_char);                                                    <* 
    *>       break;                                                                             <* 
    *>    case  'N' :                                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("char find reverse");                                   <* 
    *>       rc = ysrc_move_char_prev (x_char);                                                 <* 
    *>       break;                                                                             <* 
    *>    case  '"' :                                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("switch to a text register mode (¶)");                  <* 
    *>       yMODE_enter (SMOD_SREG);                                                           <* 
    *>       rc = a_minor;                                                                      <* 
    *>       break;                                                                             <* 
    *>    case  'r' :                                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("enter replace mode");                                  <* 
    *>       yMODE_enter (UMOD_REPLACE);                                                        <* 
    *>       if (yKEYS_repeats () == 0) {                                                       <* 
    *>          SRC_REPL_umode ('m', a_minor);                                                  <* 
    *>          rc = 0;                                                                         <* 
    *>       } else {                                                                           <* 
    *>          rc = a_minor;                                                                   <* 
    *>       }                                                                                  <* 
    *>       /+> DEBUG_USER   yLOG_exit    (__FUNCTION__);                                <+/   <* 
    *>       break;                                                                             <* 
    *>    case  'R' :                                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("enter replace mode");                                  <* 
    *>       yMODE_enter (UMOD_REPLACE);                                                        <* 
    *>       SRC_REPL_umode ('m', a_minor);                                                     <* 
    *>       /+> DEBUG_USER   yLOG_exit    (__FUNCTION__);                                <+/   <* 
    *>       rc = 0;                                                                            <* 
    *>       break;                                                                             <* 
    *>    case  'i' : case  'a' : case  'I' : case  'A' :                                       <* 
    *>       DEBUG_USER   yLOG_note    ("enter input mode");                                    <* 
    *>       if (a_minor == 'A')  s_cur->cpos = s_cur->npos - 1;                                <* 
    *>       if (a_minor == 'I')  s_cur->cpos = 0;                                              <* 
    *>       yMODE_enter (UMOD_INPUT);                                                          <* 
    *>       SRC_INPT_umode ('m', tolower (a_minor));                                           <* 
    *>       /+> DEBUG_USER   yLOG_exit    (__FUNCTION__);                                <+/   <* 
    *>       rc = tolower (a_minor);                                                            <* 
    *>       break;                                                                             <* 
    *>    case  'D' :                                                                           <* 
    *>       rc = SOURCE_delete     ('d', 'l');                                                 <* 
    *>       SOURCE__done ();                                                                   <* 
    *>       break;                                                                             <* 
    *>    case  'X' :                                                                           <* 
    *>       rc = SOURCE_delete     ('x', 'l');                                                 <* 
    *>       SOURCE__done ();                                                                   <* 
    *>       break;                                                                             <* 
    *>    }                                                                                     <* 
    *>    /+---(check for backspace)------------+/                                              <* 
    *>    if (a_minor == G_KEY_BS) {                                                            <* 
    *>       DEBUG_USER   yLOG_note    ("handle a backspace");                                  <* 
    *>       rc = SOURCE_delete     ('d', 'h');                                                 <* 
    *>       SOURCE__done ();                                                                   <* 
    *>    }                                                                                     <* 
    *>    if (a_minor == G_KEY_DEL) {                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("handle a delete");                                     <* 
    *>       rc = SOURCE_delete     ('d', 'l');                                                 <* 
    *>       SOURCE__done ();                                                                   <* 
    *>    }                                                                                     <* 
    *>    /+---(basic movement)--------------+/                                                 <* 
    *>    if (strchr (g_hsimple, a_minor) != 0) {                                               <* 
    *>       rc = ysrc_move_simple (a_major, a_minor);                                           <* 
    *>    }                                                                                     <* 
    *>    /+---(word movement)---------------+/                                                 <* 
    *>    if (strchr (g_hword, a_minor) != 0) {                                                 <* 
    *>       rc = ysrc_word_hmode (a_major, a_minor);                                           <* 
    *>    }                                                                                     <* 
    *>    /+---(other stuff)-----------------+/                                                 <* 
    *>    switch (a_minor) {                                                                    <* 
    *>    case '|' :                                                                            <* 
    *>       s_cur->cpos = yKEYS_repeat_useall ();                                              <* 
    *>       rc     = 0;                                                                        <* 
    *>       SOURCE__done ();                                                                   <* 
    *>       break;                                                                             <* 
    *>    case 'u' :                                                                            <* 
    *>       yvikeys_sreg_reset (s_cur->cpos);                                                  <* 
    *>       rc = yvikeys_sundo_undo (&(s_cur->cpos));                                          <* 
    *>       SOURCE__done ();                                                                   <* 
    *>       break;                                                                             <* 
    *>    case 'U' :                                                                            <* 
    *>       yvikeys_sreg_reset (s_cur->cpos);                                                  <* 
    *>       rc = yvikeys_sundo_redo (&(s_cur->cpos));                                          <* 
    *>       SOURCE__done ();                                                                   <* 
    *>       break;                                                                             <* 
    *>    }                                                                                     <* 
    *> }                                                                                        <* 
    *> if (rc >= 0) {                                                                           <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                             <* 
    *>    return rc;                                                                            <* 
    *> }                                                                                        <* 
    *> /+---(delete-family)------------------+/                                                 <* 
    *> /+---(multi-key)----------------------+/                                                 <* 
    *> else {                                                                                   <* 
    *>    switch (a_major) {                                                                    <* 
    *>    case 'd' : case 'x' :                                                                 <* 
    *>       rc = SOURCE_delete     (a_major, a_minor);                                         <* 
    *>       SOURCE__done ();                                                                   <* 
    *>       break;                                                                             <* 
    *>    case 'g' :  rc = ysrc_move_goto    (a_major, a_minor);  break;                        <* 
    *>    case 'z' :  rc = ysrc_move_scroll  (a_major, a_minor);  break;                        <* 
    *>    case 'f' :  x_char = a_minor;                                                         <* 
    *>                rc = ysrc_move_char_next  (x_char);                                          <* 
    *>                break;                                                                    <* 
    *>    case 'F' :  x_char = a_minor;                                                         <* 
    *>                rc = ysrc_move_char_prev (x_char);                                        <* 
    *>                break;                                                                    <* 
    *>    case 'c' :  n = yKEYS_repeat_useall ();                                               <* 
    *>                for (i = 0; i <= n; ++i) {                                                <* 
    *>                   SOURCE_delete  ('d', a_minor);                                         <* 
    *>                   SOURCE__done   ();                                                     <* 
    *>                }                                                                         <* 
    *>                yMODE_enter (UMOD_INPUT);                                                 <* 
    *>                SRC_INPT_umode ('m', tolower (a_minor));                                  <* 
    *>                rc = tolower (a_minor);                                                   <* 
    *>                DEBUG_USER   yLOG_exit    (__FUNCTION__);                                 <* 
    *>                return rc;                                                                <* 
    *>                break;                                                                    <* 
    *>    default  :  rc = -1;                              break;                              <* 
    *>    }                                                                                     <* 
    *> }                                                                                        <* 
    *> /+---(complete)-----------------------+/                                                 <* 
    *> if (rc < 0) {                                                                            <* 
    *>    DEBUG_USER   yLOG_note    ("return code is negative");                                <* 
    *>    DEBUG_USER   yLOG_exitr   (__FUNCTION__, rc);                                         <* 
    *>    return rc;                                                                            <* 
    *> }                                                                                        <* 
    *> DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                <* 
    *> return 0;                                                                                <*/
}


