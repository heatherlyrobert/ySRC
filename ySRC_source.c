/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


tEDIT   s_src;
tEDIT   s_cmd;
tEDIT  *s_cur    = &s_src;

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
    *>       rc = ysrc_multi_pure ('d', 'l');                                                 <* 
    *>       break;                                                                             <* 
    *>    case  'X' :                                                                           <* 
    *>       rc = ysrc_multi_pure ('x', 'l');                                                 <* 
    *>       break;                                                                             <* 
    *>    }                                                                                     <* 
    *>    /+---(check for backspace)------------+/                                              <* 
    *>    if (a_minor == G_KEY_BS) {                                                            <* 
    *>       DEBUG_USER   yLOG_note    ("handle a backspace");                                  <* 
    *>       rc = ysrc_multi_pure ('d', 'h');                                                 <* 
    *>    }                                                                                     <* 
    *>    if (a_minor == G_KEY_DEL) {                                                           <* 
    *>       DEBUG_USER   yLOG_note    ("handle a delete");                                     <* 
    *>       rc = ysrc_multi_pure ('d', 'l');                                                 <* 
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
    *>       rc = ysrc_multi_pure (a_major, a_minor);                                         <* 
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
    *>                   ysrc_multi_pure  ('d', a_minor);                                         <* 
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


