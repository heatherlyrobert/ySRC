/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"

char         /*-> replace sub-mode -------------------[ ------ [ge.RG6.25#.E5]*/ /*-[02.0000.112.E]-*/ /*-[--.---.---.--]-*/
ysrc_replace_umode      (int a_major, int a_minor)
{  /*---(design notes)-------------------*/
   /*
    *   very limited sub-mode of SOURCE mode.  allows a user to "type over"
    *   existing text including past the end of the line.  there are no
    *   special keys except ESCAPE to return to SOURCE mode.
    */
   /*---(locals)-----------+-----+-----+-*/
   /*> char        rce         =  -10;                                                           <* 
    *> char        rc          =    0;                                                           <* 
    *> static char x_saved     = '\0';                                                           <* 
    *> static char x_prev      =  '-';                                                           <* 
    *> static char x_mode      =  '-';                                                           <* 
    *> uchar       x_ch        =  '·';                                                           <* 
    *> /+---(header)-------------------------+/                                                  <* 
    *> DEBUG_USER   yLOG_enter   (__FUNCTION__);                                                 <* 
    *> DEBUG_USER   yLOG_char    ("a_major"   , a_major);                                        <* 
    *> DEBUG_USER   yLOG_char    ("a_minor"   , chrvisible (a_minor));                           <* 
    *> /+---(defenses)-----------------------+/                                                  <* 
    *> DEBUG_USER   yLOG_char    ("mode"      , yMODE_curr ());                                  <* 
    *> --rce;  if (yMODE_not (UMOD_REPLACE)) {                                                   <* 
    *>    DEBUG_USER   yLOG_note    ("not the correct mode");                                    <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                              <* 
    *>    return rce;                                                                            <* 
    *> }                                                                                         <* 
    *> /+---(prepare)------------------------+/                                                  <* 
    *> ysrc_before_change   ();                                                                  <* 
    *> /+---(check for mark)-----------------+/                                                  <* 
    *> if (a_major == 'm') {                                                                     <* 
    *>    DEBUG_USER   yLOG_note    ("mark replacement position and save existing");             <* 
    *>    x_mode  = a_minor;                                                                     <* 
    *>    x_saved = s_cur->contents [s_cur->cpos];                                               <* 
    *>    yvikeys_src_one_replace (G_CHAR_PLACE);                                                <* 
    *>    if (x_saved == NULL)  s_cur->contents [s_cur->cpos + 1] = NULL;                        <* 
    *>    SOURCE__done   ();                                                                     <* 
    *>    yvikeys_sundo_beg (__FUNCTION__);                                                      <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                              <* 
    *>    return 0;                                                                              <* 
    *> }                                                                                         <* 
    *> /+---(escaped chars)------------------+/                                                  <* 
    *> if (a_major != 'r' && a_minor == '\\' && x_prev != '\\') {                                <* 
    *>    DEBUG_USER   yLOG_note    ("found a leading backslash");                               <* 
    *>    x_prev = '\\';                                                                         <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                              <* 
    *>    return 0;                                                                              <* 
    *> }                                                                                         <* 
    *> if (x_prev == '\\' && a_minor == '_') {                                                   <* 
    *>    DEBUG_USER   yLOG_note    ("found a leading backslash/underscore");                    <* 
    *>    x_prev = '_';                                                                          <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                              <* 
    *>    return 0;                                                                              <* 
    *> }                                                                                         <* 
    *> /+---(mode changes)-------------------+/                                                  <* 
    *> else if (a_minor == G_KEY_ESCAPE || a_minor == G_KEY_RETURN) {                            <* 
    *>    DEBUG_USER   yLOG_note    ("escape/return, return to source mode");                    <* 
    *>    x_prev  = '-';                                                                         <* 
    *>    x_mode  = '-';                                                                         <* 
    *>    rc = yvikeys_src_one_replace (x_saved);                                                <* 
    *>    yMODE_exit ();                                                                         <* 
    *>    yvikeys_sundo_end (__FUNCTION__);                                                      <* 
    *>    DEBUG_USER   yLOG_value   ("mode"     , yMODE_curr ());                                <* 
    *>    if (a_minor == G_KEY_RETURN && strchr (MODES_ONELINE, yMODE_curr ()) != NULL) {        <* 
    *>       DEBUG_USER   yLOG_note    ("fast path back to map mode");                           <* 
    *>       ysrc_source_mode (G_KEY_SPACE, a_minor);                                                 <* 
    *>    }                                                                                      <* 
    *> }                                                                                         <* 
    *> /+---(handle repeat r)----------------+/                                                  <* 
    *> else if (a_major == 'r') {                                                                <* 
    *>    DEBUG_USER   yLOG_note    ("replace repeatedly");                                      <* 
    *>    x_mode  = '-';                                                                         <* 
    *>    if (a_minor >= 32 && a_minor < 127 && a_minor != '\\') {                               <* 
    *>       yvikeys_sundo_beg (__FUNCTION__);                                                   <* 
    *>       yvikeys_sundo_single ('r', s_cur->cpos, s_cur->contents [s_cur->cpos], a_minor);    <* 
    *>       rc = yvikeys_src_one_replace (a_minor);                                             <* 
    *>       yvikeys_sundo_end (__FUNCTION__);                                                   <* 
    *>       ++s_cur->cpos;                                                                      <* 
    *>       if (yKEYS_repeats () == 0)  yMODE_exit ();                                          <* 
    *>    } else {                                                                               <* 
    *>       rc = -1;                                                                            <* 
    *>       yMODE_exit ();                                                                      <* 
    *>    }                                                                                      <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                              <* 
    *>    return rc;                                                                             <* 
    *> }                                                                                         <* 
    *> /+---(handle little r)----------------+/                                                  <* 
    *> else if (x_mode == 'r') {                                                                 <* 
    *>    DEBUG_USER   yLOG_note    ("replace the marked character");                            <* 
    *>    x_mode  = '-';                                                                         <* 
    *>    if (x_prev == '\\') {                                                                  <* 
    *>       yvikeys_sundo_single ('r', s_cur->cpos, x_saved, chrslashed (a_minor));             <* 
    *>       rc = yvikeys_src_one_replace (chrslashed (a_minor));                                <* 
    *>       x_prev = '-';                                                                       <* 
    *>    } else if (x_prev == '_') {                                                            <* 
    *>       yvikeys_sundo_single ('r', s_cur->cpos, x_saved, chrslashed_more (a_minor));        <* 
    *>       rc = yvikeys_src_one_replace (chrslashed_more (a_minor));                           <* 
    *>       x_prev = '-';                                                                       <* 
    *>    } else if (a_minor >= 32 && a_minor < 127) {                                           <* 
    *>       if (a_minor == G_KEY_SPACE)  a_minor = G_CHAR_STORAGE;                              <* 
    *>       yvikeys_sundo_single ('r', s_cur->cpos, x_saved, a_minor);                          <* 
    *>       rc = yvikeys_src_one_replace (a_minor);                                             <* 
    *>    } else {                                                                               <* 
    *>       rc = yvikeys_src_one_replace (x_saved);                                             <* 
    *>    }                                                                                      <* 
    *>    yvikeys_sundo_end (__FUNCTION__);                                                      <* 
    *>    /+> if (yKEYS_repeats () == 0) {                                                 <*    <* 
    *>     *>    yMODE_exit ();                                                            <*    <* 
    *>     *> } else {                                                                    <*     <* 
    *>     *>    ++s_cur->cpos;                                                           <*     <* 
    *>     *> }                                                                           <+/    <* 
    *>    yMODE_exit ();                                                                         <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                              <* 
    *> }                                                                                         <* 
    *> /+---(handle big R)-------------------+/                                                  <* 
    *> else {                                                                                    <* 
    *>    /+---(check for backspace)------------+/                                               <* 
    *>    if (a_minor == G_KEY_DEL || a_minor == G_KEY_BS) {                                     <* 
    *>       DEBUG_USER   yLOG_note    ("handle a backspace/delete");                            <* 
    *>       if (s_cur->cpos > 0) {                                                              <* 
    *>          rc = yvikeys_src_one_replace (x_saved);                                          <* 
    *>          --s_cur->cpos;                                                                   <* 
    *>          x_saved = s_cur->contents [s_cur->cpos];                                         <* 
    *>          rc = yvikeys_src_one_replace (G_CHAR_PLACE);                                     <* 
    *>       }                                                                                   <* 
    *>    }                                                                                      <* 
    *>    /+---(handle new character)-----------+/                                               <* 
    *>    else {                                                                                 <* 
    *>       DEBUG_USER   yLOG_note    ("replace and move right");                               <* 
    *>       if (x_prev == '\\') {                                                               <* 
    *>          yvikeys_sundo_single ('R', s_cur->cpos, x_saved, chrslashed (a_minor));          <* 
    *>          rc = yvikeys_src_one_replace (chrslashed (a_minor));                             <* 
    *>          x_prev = '-';                                                                    <* 
    *>       } else if (x_prev == '_') {                                                         <* 
    *>          yvikeys_sundo_single ('R', s_cur->cpos, x_saved, chrslashed_more (a_minor));     <* 
    *>          rc = yvikeys_src_one_replace (chrslashed_more (a_minor));                        <* 
    *>          x_prev = '-';                                                                    <* 
    *>       } else if (a_minor >= 32 && a_minor < 127) {                                        <* 
    *>          if (a_minor == G_KEY_SPACE)  a_minor = G_CHAR_STORAGE;                           <* 
    *>          yvikeys_sundo_single ('R', s_cur->cpos, x_saved, a_minor);                       <* 
    *>          rc = yvikeys_src_one_replace (a_minor);                                          <* 
    *>       } else {                                                                            <* 
    *>          rc = yvikeys_src_one_replace (x_saved);                                          <* 
    *>       }                                                                                   <* 
    *>       ++s_cur->cpos;                                                                      <* 
    *>       x_saved = s_cur->contents [s_cur->cpos];                                            <* 
    *>       if (x_saved == NULL)  s_cur->contents [s_cur->cpos + 1] = NULL;                     <* 
    *>       rc = yvikeys_src_one_replace (G_CHAR_PLACE);                                        <* 
    *>    }                                                                                      <* 
    *> }                                                                                         <* 
    *> /+---(wrap up)------------------------+/                                                  <* 
    *> SOURCE__done   ();                                                                        <* 
    *> /+---(complete)-----------------------+/                                                  <* 
    *> DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                 <* 
    *> return rc;                                                                                <*/
}
