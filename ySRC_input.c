/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


char
ysrc_add_one            (uchar a_mode, uchar a_minor)
{
   DEBUG_USER   yLOG_note    ("add and move remaining chars to the right");
   a_minor = chrvisible (a_minor);
   ysrc_sundo_single (a_mode, s_cur->cpos, G_CHAR_NULL, a_minor);
   ysrc_insert_one   (a_minor);
   ++s_cur->cpos;
   return 0;
}

char         /*-> process keys for input mode --------[ ------ [ge.TQ5.25#.F9]*/ /*-[03.0000.122.R]-*/ /*-[--.---.---.--]-*/
ysrc_input_umode           (uchar a_major, uchar a_minor)
{
   /*---(design notes)-------------------*/
   /*
    *   this should imitate a very basic vi-input mode by handling
    *   all characters, ignoring new line, and popping out with escape
    */
   /*> /+---(locals)-----------+-----+-----+-+/                                                                 <* 
    *> char        rce         =  -10;                                                                          <* 
    *> static char x_prev      =  '-';                                                                          <* 
    *> char        rc          =    0;                                                                          <* 
    *> static char x_mode      =  '-';                                                                          <* 
    *> char        x_prevmode  =  '-';                                                                          <* 
    *> char        x_history   [LEN_LABEL];                                                                     <* 
    *> static char x_quoting   =  '-';                                                                          <* 
    *> /+---(header)-------------------------+/                                                                 <* 
    *> DEBUG_USER   yLOG_enter   (__FUNCTION__);                                                                <* 
    *> DEBUG_USER   yLOG_char    ("a_major"   , a_major);                                                       <* 
    *> DEBUG_USER   yLOG_char    ("a_minor"   , chrvisible (a_minor));                                          <* 
    *> DEBUG_USER   yLOG_char    ("mode"      , x_quoting);                                                     <* 
    *> /+---(defenses)-----------------------+/                                                                 <* 
    *> DEBUG_USER   yLOG_char    ("mode"      , yMODE_curr ());                                                 <* 
    *> --rce;  if (yMODE_not (UMOD_INPUT)) {                                                                    <* 
    *>    DEBUG_USER   yLOG_note    ("not the correct mode");                                                   <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                             <* 
    *>    return rce;                                                                                           <* 
    *> }                                                                                                        <* 
    *> /+---(prepare)------------------------+/                                                                 <* 
    *> rc = -1;                                                                                                 <* 
    *> SOURCE__prep ();                                                                                         <* 
    *> /+---(check for initial mark)---------+/                                                                 <* 
    *> if (a_major == 'm') {                                                                                    <* 
    *>    DEBUG_USER   yLOG_note    ("mark replacement position and save existing");                            <* 
    *>    x_mode  = a_minor;                                                                                    <* 
    *>    if (a_minor == 'a')  yvikeys_src_one_append (G_CHAR_PLACE);                                           <* 
    *>    else                 yvikeys_src_one_insert (G_CHAR_PLACE);                                           <* 
    *>    SOURCE__done   ();                                                                                    <* 
    *>    yvikeys_sundo_beg (__FUNCTION__);                                                                     <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                             <* 
    *>    return 0;                                                                                             <* 
    *> }                                                                                                        <* 
    *> /+---(escaped chars)------------------+/                                                                 <* 
    *> if (x_prev != '\\' && a_minor == '\\') {                                                                 <* 
    *>    DEBUG_USER   yLOG_note    ("found a leading backslash");                                              <* 
    *>    x_prev = '\\';                                                                                        <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                             <* 
    *>    return 0;                                                                                             <* 
    *> }                                                                                                        <* 
    *> if (x_prev == '\\' && a_minor == '_') {                                                                  <* 
    *>    DEBUG_USER   yLOG_note    ("found a leading backslash/underscore");                                   <* 
    *>    x_prev = '_';                                                                                         <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                             <* 
    *>    return 0;                                                                                             <* 
    *> }                                                                                                        <* 
    *> if (x_prev == '\\') {                                                                                    <* 
    *>    DEBUG_USER   yLOG_note    ("converting backslash character");                                         <* 
    *>    yvikeys_sundo_single (x_mode, s_cur->cpos, G_CHAR_NULL, chrslashed (a_minor));                        <* 
    *>    rc = yvikeys_src_one_insert (chrslashed (a_minor));                                                   <* 
    *>    ++s_cur->cpos;                                                                                        <* 
    *>    x_prev = '-';                                                                                         <* 
    *> }                                                                                                        <* 
    *> else if (x_prev == '_') {                                                                                <* 
    *>    DEBUG_USER   yLOG_note    ("converting backslash/underscore");                                        <* 
    *>    yvikeys_sundo_single (x_mode, s_cur->cpos, G_CHAR_NULL, chrslashed_more (a_minor));                   <* 
    *>    rc = yvikeys_src_one_insert (chrslashed_more (a_minor));                                              <* 
    *>    ++s_cur->cpos;                                                                                        <* 
    *>    x_prev = '-';                                                                                         <* 
    *> }                                                                                                        <* 
    *> /+---(mode changes)-------------------+/                                                                 <* 
    *> else if (a_minor == G_KEY_ESCAPE || a_minor == G_KEY_RETURN) {                                           <* 
    *>    x_prev = '-';                                                                                         <* 
    *>    if (x_quoting == 'y' && yMODE_prev () != MODE_SOURCE) {                                               <* 
    *>       rc = ySRC_add_one (x_mode, a_minor);                                                               <* 
    *>    } else {                                                                                              <* 
    *>       DEBUG_USER   yLOG_note    ("escape/return, return to source mode");                                <* 
    *>       x_quoting = '-';                                                                                   <* 
    *>       x_mode    = '-';                                                                                   <* 
    *>       rc = yvikeys_src_one_delete ();                                                                    <* 
    *>       --s_cur->cpos;                                                                                     <* 
    *>       yvikeys_sundo_end (__FUNCTION__);                                                                  <* 
    *>       yMODE_exit ();                                                                                     <* 
    *>       DEBUG_USER   yLOG_value   ("mode"     , yMODE_curr ());                                            <* 
    *>       if (a_minor == G_KEY_RETURN && strchr (MODES_ONELINE, yMODE_curr ()) != NULL) {                    <* 
    *>          DEBUG_USER   yLOG_note    ("fast path back to map mode");                                       <* 
    *>          rc = SOURCE_mode (G_KEY_SPACE, a_minor);                                                        <* 
    *>       }                                                                                                  <* 
    *>    }                                                                                                     <* 
    *> }                                                                                                        <* 
    *> /+---(check for backspace)------------+/                                                                 <* 
    *> else if (a_minor == G_KEY_BS) {                                                                          <* 
    *>    if (x_quoting == 'y' && yMODE_prev () != MODE_SOURCE) {                                               <* 
    *>       rc = ysrc_add_one (x_mode, a_minor);                                                               <* 
    *>    } else {                                                                                              <* 
    *>       DEBUG_USER   yLOG_note    ("handle a backspace");                                                  <* 
    *>       if (s_cur->cpos > 0) {                                                                             <* 
    *>          yvikeys_sundo_add ('d', 'h', s_cur->cpos - 1, s_cur->contents [s_cur->cpos - 1], G_KEY_NULL);   <* 
    *>          rc = yvikeys_src_one_backspace ();                                                              <* 
    *>       }                                                                                                  <* 
    *>    }                                                                                                     <* 
    *> }                                                                                                        <* 
    *> else if (a_minor == G_KEY_DEL) {                                                                         <* 
    *>    if (x_quoting == 'y' && yMODE_prev () != MODE_SOURCE) {                                               <* 
    *>       rc = ysrc_add_one (x_mode, a_minor);                                                               <* 
    *>    } else {                                                                                              <* 
    *>       DEBUG_USER   yLOG_note    ("handle a delete");                                                     <* 
    *>       if (s_cur->cpos < s_cur->npos - 1) {                                                               <* 
    *>          ++s_cur->cpos;                                                                                  <* 
    *>          yvikeys_sundo_add ('d', 'l', s_cur->cpos, s_cur->contents [s_cur->cpos], G_KEY_NULL);           <* 
    *>          rc = yvikeys_src_one_delete ();                                                                 <* 
    *>          --s_cur->cpos;                                                                                  <* 
    *>       }                                                                                                  <* 
    *>    }                                                                                                     <* 
    *> }                                                                                                        <* 
    *> /+---(wander mode)--------------------+/                                                                 <* 
    *> else if (a_minor == G_KEY_BTICK) {                                                                       <* 
    *>    yvikeys_map_wander_prep ();                                                                           <* 
    *>    rc = yMODE_enter (UMOD_WANDER);                                                                       <* 
    *>    DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                             <* 
    *>    return rc;                                                                                            <* 
    *> }                                                                                                        <* 
    *> /+---(handle new character)-----------+/                                                                 <* 
    *> else if (a_minor == G_KEY_SPACE) {                                                                       <* 
    *>    rc = ysrc_add_one (x_mode, G_CHAR_STORAGE);                                                           <* 
    *> }                                                                                                        <* 
    *> else if ((a_minor > 32 && a_minor < 127) || (a_minor > 127 && a_minor < 256)) {                          <* 
    *>    rc = ysrc_add_one (x_mode, a_minor);                                                                  <* 
    *> }                                                                                                        <* 
    *> /+---(wrap up)------------------------+/                                                                 <* 
    *> SOURCE__done   ();                                                                                       <* 
    *> /+---(check for history)--------------+/                                                                 <* 
    *> x_prevmode = yMODE_prev ();                                                                              <* 
    *> sprintf (x_history, "%c%c%c¤"  , x_prevmode, x_prevmode, x_prevmode);                                    <* 
    *> if (strcmp (s_cur->contents, x_history) == 0) {                                                          <* 
    *>    rc = yvikeys_src_one_delete ();                                                                       <* 
    *>    --s_cur->cpos;                                                                                        <* 
    *>    yvikeys_sundo_end (__FUNCTION__);                                                                     <* 
    *>    SOURCE__done ();                                                                                      <* 
    *>    yMODE_exit ();                                                                                        <* 
    *>    /+> if (x_prevmode == MODE_SEARCH)  strlcpy (s_cur->contents, "/", LEN_RECD);   <*                    <* 
    *>     *> else                            strlcpy (s_cur->contents, ":", LEN_RECD);   <+/                   <* 
    *>    /+> SOURCE__done   ();                                                          <+/                   <* 
    *>    yvikeys_hist__bounds ();                                                                              <* 
    *>    yMODE_enter  (UMOD_HISTORY);                                                                          <* 
    *>    myVIKEYS.cursor = '-';                                                                                <* 
    *>    DEBUG_USER   yLOG_note    ("change to history mode");                                                 <* 
    *> }                                                                                                        <* 
    *> /+---(complete)-----------------------+/                                                                 <* 
    *> DEBUG_USER   yLOG_exit    (__FUNCTION__);                                                                <* 
    *> return rc;                                                                                               <*/
}
