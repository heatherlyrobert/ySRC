/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"


/*
 *  source registers are meant to be kept flexible and macro-ready
 *  so automation can be through the roof.  i have added some sourcing
 *  information to *potentially* help in the future, but also assist
 *  testing.
 *
 *  users have a-z easily available and appendable
 *
 *  0-9 and �-� are useful for the system and macros to reduce collisions.
 *
 */



char        G_SREG_LIST [S_SREG_MAX];
tSREG       g_sreg;
tSREG       g_sregs  [S_SREG_MAX];
uchar       g_nsreg  =   0;
uchar       g_csreg   = '"';
uchar       g_wsreg   = '"';
uchar       g_psreg   = '"';
tSREG       g_save;

char    (*e_sreg)  (char a_reg, char *a_contents) = NULL;


#define     S_SREG_NONE     '-'
#define     S_SREG_AUTO     'a'
#define     S_SREG_USER     'u'
#define     S_SREG_MULTI    'M'
#define     S_SREG_IMPORT   'i'
#define     S_SREG_DIRECT   'd'
#define     S_SREG_FILE     'f'



/*====================------------------------------------====================*/
/*===----                         program level                        ----===*/
/*====================------------------------------------====================*/
static void  o___ELEMENTAL_______o () { return; }

char
ysrc_sreg__wipedata  (tSREG *a_dst)
{
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(contents)-----------------------*/
   a_dst->len    =  0;
   if (a_dst->data  != g_stub && a_dst->data  != NULL) {
      DEBUG_YSRC   yLOG_spoint  (a_dst->data);
      free (a_dst->data);
   } else {
      DEBUG_YSRC   yLOG_snote   ("data null");
   }
   a_dst->data   = g_stub;
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_sreg__wipeall      (char a_scope, tSREG *a_dst)
{
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(active)-------------------------*/
   a_dst->active = S_SREG_NOT;
   a_dst->source = S_SREG_NONE;
   /*---(source)-------------------------*/
   DEBUG_YSRC   yLOG_spoint  (__FUNCTION__);
   if (a_scope == YSTR_INIT) {
      DEBUG_YSRC   yLOG_snote   ("label initialized");
   } else if (a_dst->label != g_stub && a_dst->label != NULL) {
      DEBUG_YSRC   yLOG_spoint  (a_dst->label);
      free (a_dst->label);
   } else {
      DEBUG_YSRC   yLOG_snote   ("label null");
   }
   a_dst->label  = g_stub;
   a_dst->root   =  0;
   a_dst->beg    =  0;
   a_dst->end    =  0;
   /*---(contents)-----------------------*/
   a_dst->len    =  0;
   if (a_scope == YSTR_INIT) {
      DEBUG_YSRC   yLOG_snote   ("data initialized");
   } else if (a_dst->data  != g_stub && a_dst->data  != NULL) {
      DEBUG_YSRC   yLOG_spoint  (a_dst->data);
      free (a_dst->data);
   } else {
      DEBUG_YSRC   yLOG_snote   ("data null");
   }
   a_dst->data   = g_stub;
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         program level                        ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
ysrc_sreg_purge         (char a_scope)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        x_abbr      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_char    ("a_scope"   , a_scope);
   --rce;  switch (a_scope) {
   case YSTR_ILOWER  : case YSTR_INUMBER :
   case YSTR_IGREEK  : case YSTR_IFULL   :
   case YSTR_INIT    :
      break;
   default :
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear)--------------------------*/
   DEBUG_YSRC   yLOG_value   ("g_nsreg"   , g_nsreg);
   for (i = 0; i < g_nsreg; ++i) {
      x_abbr = G_SREG_LIST [i];
      if (a_scope == YSTR_ILOWER  && strchr (YSTR_LOWER , x_abbr) == NULL)  continue;
      if (a_scope == YSTR_INUMBER && strchr (YSTR_NUMBER, x_abbr) == NULL)  continue;
      if (a_scope == YSTR_IGREEK  && strchr (YSTR_GREEK , x_abbr) == NULL)  continue;
      DEBUG_YSRC   yLOG_char    ("x_abbr"    , x_abbr);
      ysrc_sreg__wipeall (a_scope, &g_sregs [i]);
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> clear all selections ---------------[ shoot  [gz.311.001.02]*/ /*-[00.0000.102.1]-*/ /*-[--.---.---.--]-*/
ysrc_sreg_init          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_prep  (SMOD_SREG)) {
      DEBUG_YSRC   yLOG_note    ("status is not ready for init");
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(macro abbrev list)--------------*/
   ystrlcpy (G_SREG_LIST, "�"        , S_SREG_MAX);
   ystrlcat (G_SREG_LIST, YSTR_NUMBER, S_SREG_MAX);
   ystrlcat (G_SREG_LIST, YSTR_LOWER , S_SREG_MAX);
   ystrlcat (G_SREG_LIST, YSTR_NUMBER, S_SREG_MAX);
   ystrlcat (G_SREG_LIST, YSTR_GREEK , S_SREG_MAX);
   ystrlcat (G_SREG_LIST, "�"        , S_SREG_MAX);
   DEBUG_YSRC   yLOG_info    ("SREG_LIST" , G_SREG_LIST);
   g_nsreg  = strlen (G_SREG_LIST);
   DEBUG_YSRC   yLOG_value   ("g_nsreg"   , g_nsreg);
   /*---(registers)----------------------*/
   ysrc_sreg_purge    (YSTR_IFULL);
   /*---(globals)------------------------*/
   ysrc_select_reset   (G_SREG_ZERO);
   g_wsreg = '"';
   /*---(commands)-----------------------*/
   DEBUG_YSRC   yLOG_note    ("add commands/status");
   /*> yVIKEYS_view_optionX (YVIEW_STATUS  , "sreg", ysrc_sreg_status   , "details of current source register");   <*/
   /*> yVIKEYS_cmds_addX    (YVIKEYS_M_CONFIG, "sreg", "", "a", ysrc_sreg_direct, "direct definition of source registers");   <*/
   /*---(update status)------------------*/
   yMODE_init_set   (SMOD_SREG, NULL, ysrc_sreg_smode);
   DEBUG_YSRC   yLOG_info    ("sreg"      , yMODE_actual (SMOD_SREG));
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void  o___UTILITY_________o () { return; }

char 
ysrc_sreg_index         (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(check)--------------------------*/
   DEBUG_YSRC   yLOG_schar   (a_abbr);
   --rce;  if (a_abbr == 0) {
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_snote   ("check");
   if (a_abbr == '"')  n  = 0;
   else                n  = ystrlchr (G_SREG_LIST, a_abbr, g_nsreg);
   DEBUG_YSRC   yLOG_sint    (n);
   --rce;  if (n  < 0) {
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, n);
      return n;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return n;
}

char
ysrc_sreg_setreg        (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_schar   (a_abbr);
   DEBUG_YSRC   yLOG_schar   (g_csreg);
   n  = ystrlchr (G_SREG_LIST, tolower (a_abbr), g_nsreg);
   DEBUG_YSRC   yLOG_sint    (n);
   --rce;  if (n < 0) {
      g_csreg = '"';
      DEBUG_YSRC   yLOG_schar   (g_csreg);
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(set)----------------------------*/
   g_csreg = a_abbr;
   DEBUG_YSRC   yLOG_schar   (g_csreg);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysrc_sreg_setwork       (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_schar   (a_abbr);
   DEBUG_YSRC   yLOG_schar   (g_wsreg);
   n  = ystrlchr (G_SREG_LIST, tolower (a_abbr), g_nsreg);
   DEBUG_YSRC   yLOG_sint    (n);
   --rce;  if (n < 0) {
      g_wsreg = '"';
      DEBUG_YSRC   yLOG_schar   (g_wsreg);
      DEBUG_YSRC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(set)----------------------------*/
   g_wsreg = a_abbr;
   DEBUG_YSRC   yLOG_schar   (g_wsreg);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        data movement                         ----===*/
/*====================------------------------------------====================*/
static void  o___DATA____________o () { return; }

char
ysrc_sreg_clear         (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_char    ("a_abbr"    , a_abbr);
   n = ysrc_sreg_index  (a_abbr);
   DEBUG_YSRC   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear)--------------------------*/
   ysrc_sreg__wipeall ('-', &g_sregs [n]);
   /*---(back-to-host)-------------------*/
   if (e_sreg != NULL) {
      DEBUG_YSRC   yLOG_note    ("save back to host");
      e_sreg (a_abbr, "");
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc_sreg_push          (uchar a_abbr, char *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   tSREG      *a_dst       = NULL;
   char        t           [LEN_RECD] = "";
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_complex ("args"      , "abbr %u, data %-10p", a_abbr, a_data);
   /*---(defense)------------------------*/
   --rce;  if (a_data  == NULL) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   n = ysrc_sreg_index  (tolower (a_abbr));
   DEBUG_YSRC   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   a_dst = &g_sregs [n];
   /*---(push data)----------------------*/
   if (a_abbr == tolower (a_abbr) || a_dst->data == g_stub)  {
      DEBUG_YSRC   yLOG_note    ("normal, replace mode");
      ysrc_sreg__wipeall  ('-', a_dst);
      a_dst->active = S_SREG_YES;
      a_dst->source = S_SREG_DIRECT;
      a_dst->data   = strdup (a_data);
   } else {
      DEBUG_YSRC   yLOG_note    ("append mode");
      ystrlcpy (t, a_dst->data, LEN_RECD);
      ysrc_sreg__wipedata (a_dst);
      ystrlcat (t, a_data     , LEN_RECD);
      a_dst->data   = strdup (t);
      a_dst->source = S_SREG_MULTI;
   }
   a_dst->len    = ystrllen (a_dst->data , LEN_RECD);
   /*---(back-to-host)-------------------*/
   if (e_sreg != NULL) {
      DEBUG_YSRC   yLOG_note    ("save back to host");
      e_sreg (a_abbr, a_dst->data);
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char ySRC_push      (uchar a_sreg, char *a_data)  { return ysrc_sreg_push (a_sreg, a_data); }

char
ysrc_sreg_pop           (uchar a_abbr, char *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_complex ("args"      , "abbr %u, data %-10p", a_abbr, a_data);
   /*---(prepare)------------------------*/
   if (a_data != NULL)  ystrlcpy (a_data, "", LEN_RECD);
   /*---(defense)------------------------*/
   n = ysrc_sreg_index  (a_abbr);
   DEBUG_YSRC   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_char    ("active"    , g_sregs [n].active);
   --rce;  if (g_sregs [n].active != S_SREG_YES) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(return)-------------------------*/
   if (a_data != NULL)  ystrlcpy (a_data, g_sregs [n].data, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc_sreg_save          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   tSREG      *a_dst       = NULL;
   char        s           [LEN_RECD] = "";
   char        t           [LEN_RECD] = "";
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*> DEBUG_YSRC   yLOG_complex ("args"      , "label %-10p, data %-10p", a_label, a_data);   <*/
   /*---(defense)------------------------*/
   n = ysrc_sreg_index  (tolower (g_csreg));
   DEBUG_YSRC   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   a_dst = &g_sregs [n];
   /*---(save)---------------------------*/
   if (g_csreg == tolower (g_csreg) || a_dst->data == g_stub) {
      DEBUG_YSRC   yLOG_note    ("normal, replace mode");
      ysrc_sreg__wipeall  ('-', a_dst);
      a_dst->active = S_SREG_YES;
      a_dst->source = S_SREG_USER;
      a_dst->label  = strdup (s_cur->label);
      ysrc_select_get (&(a_dst->beg), &(a_dst->end), &(a_dst->root), &(a_dst->len), t);
      a_dst->data   = strdup (t);
   } else {
      DEBUG_YSRC   yLOG_note    ("append mode");
      ystrlcpy (s, a_dst->data, LEN_RECD);
      ysrc_sreg__wipedata (a_dst);
      ysrc_select_get (NULL         , NULL         , NULL          , &(a_dst->len), t);
      ystrlcat (s, t , LEN_RECD );
      a_dst->source = S_SREG_MULTI;
      a_dst->data   = strdup (s);
      a_dst->len    = ystrllen (a_dst->data , LEN_RECD);
   }
   a_dst->len    = ystrllen (a_dst->data , LEN_RECD);
   DEBUG_YSRC   yLOG_value   ("len"       , a_dst->len);
   /*---(back-to-host)-------------------*/
   if (e_sreg != NULL) {
      DEBUG_YSRC   yLOG_note    ("save back to host");
      e_sreg (g_csreg, a_dst->data);
   }
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysrc_sreg_fetch         (short *a_len, char *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_complex ("args"      , "len %-10p, data %-10p", a_len, a_data);
   /*---(init)---------------------------*/
   if (a_len  != NULL)  *a_len = 0;
   if (a_data != NULL)  ystrlcpy (a_data, "", LEN_RECD);
   /*---(defense)------------------------*/
   n = ysrc_sreg_index  (g_csreg);
   DEBUG_YSRC   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_char    ("active"    , g_sregs [n].active);
   --rce;  if (g_sregs [n].active != S_SREG_YES) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(init)---------------------------*/
   if (a_len  != NULL)  *a_len = g_sregs [n].len;
   if (a_data != NULL)  ystrlcpy (a_data, g_sregs [n].data, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     status and information                   ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS__________o () { return; }

char
ysrc_sreg_info          (int a_index, char *a_entry)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_abbr      =  '-';
   /*---(defense)------------------------*/
   --rce;  if (a_entry == NULL)    return rce;
   --rce;  if (a_index < 0)        return rce;
   --rce;  if (a_index >= g_nsreg) return rce;
   /*---(get info)-----------------------*/
   x_abbr = G_SREG_LIST [a_index];
   rc = ysrc_sreg_line   (x_abbr, a_entry);
   --rce;  if (rc < 0)             return rce;
   /*---(complete)--------------------*/
   return rc;
}

char         /*-> tbd --------------------------------[ ------ [ge.C53.244.61]*/ /*-[02.0000.204.!]-*/ /*-[--.---.---.--]-*/
ysrc_sreg_status_sel    (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         x_len       = 0;
   /*---(defenses)--------------------*/
   --rce;  if (a_list  == NULL) return rce;
   /*---(write line)------------------*/
   x_len = g_sreg.end - g_sreg.beg + 1;
   /*> sprintf (a_list , "selection %c %3d %3d %3d[%-*.*s]",                          <* 
    *>       g_sreg.active, g_sreg.beg, g_sreg.end, g_sreg.len,                       <* 
    *>       x_len, x_len, s_cur->contents + g_sreg.beg);                             <*/
   /*---(complete)--------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         mode handling                        ----===*/
/*====================------------------------------------====================*/
static void  o___MODE____________o () { return; }

char
ysrc_sreg__export       (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   n = ysrc_sreg_index (a_abbr);
   --rce;  if (n <  0)  {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   rc = ystrlexport (0, g_sregs [n].data);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc_sreg__import       (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD];
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(read)---------------------------*/
   rc = ystrlimport  (0, x_recd, NULL);
   DEBUG_YSRC   yLOG_value   ("read"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_info    ("x_recd"    , x_recd);
   /*---(add)----------------------------*/
   rc = ysrc_sreg_push       (a_abbr, x_recd);
   DEBUG_YSRC   yLOG_value   ("push"      , rc);
   /*---(mark imported)------------------*/
   n = ysrc_sreg_index  (tolower (a_abbr));
   DEBUG_YSRC   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_sregs [n].source = S_SREG_IMPORT;
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc_sreg__copy         (char a_abbr, char a_dst)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        s           =   -1;
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   char        x_direct    [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_char    ("a_abbr"      , a_abbr);
   s = ysrc_sreg_index  (a_abbr);
   DEBUG_YSRC   yLOG_value   ("s"         , s);
   --rce;  if (s <  0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy)---------------------------*/
   ystrlcpy (x_recd, g_sregs [s].data, LEN_RECD);
   x_len = strlen (x_recd);
   DEBUG_YSRC   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <=  0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add)----------------------------*/
   DEBUG_YSRC   yLOG_char    ("a_dst"     , a_dst);
   rc = ysrc_sreg_push       (a_dst, x_recd);
   DEBUG_YSRC   yLOG_value   ("push"      , rc);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ysrc_sreg__save         (char a_abbr, char *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD];
   char       *p           = NULL;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(fix quotes)---------------------*/
   ystrlcpy (x_recd, a_string, LEN_RECD);
   x_len = strlen (x_recd);
   p = x_recd;
   if (x_recd [0] == G_KEY_DQUOTE && x_recd [x_len - 1] == G_KEY_DQUOTE) {
      DEBUG_YSRC   yLOG_note    ("quoted macro format");
      x_recd [--x_len] = G_KEY_NULL;
      p = x_recd + 1;
   }
   /*---(add)----------------------------*/
   rc = ysrc_sreg_push       (a_abbr, p);
   DEBUG_YSRC   yLOG_value   ("push"      , rc);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> enter a macro directly -------------[ ------ [ge.850.137.A4]*/ /*-[02.0000.00#.!]-*/ /*-[--.---.---.--]-*/
ysrc_sreg_direct        (char *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char        x_abbr      =  '-';
   char        x_div       =  '-';
   char       *x_valid     = "*aA0�";
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_operational (SMOD_SREG)) {
      DEBUG_YSRC   yLOG_note    ("can not execute until operational");
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_YSRC   yLOG_point   ("a_string"  , a_string);
   --rce;  if (a_string == NULL) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSRC   yLOG_info    ("a_string"  , a_string);
   x_len = strlen (a_string);
   DEBUG_YSRC   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 0) {
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_abbr  = a_string [0];
   DEBUG_YSRC   yLOG_char    ("x_abbr"      , x_abbr);
   x_div = a_string [1];
   DEBUG_YSRC   yLOG_char    ("x_div"     , x_div);
   /*---(check for purge)----------------*/
   --rce;  if (x_len == 1) {
      rc = ysrc_sreg_purge  (x_abbr);
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(check dividers)-----------------*/
   --rce;  if (strchr ("#-+>])=", x_div) == NULL)  {
      DEBUG_YSRC   yLOG_note    ("divider (x_div) not understood");
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for shorts)---------------*/
   --rce;  if (x_len == 2) {
      switch (x_div) {
      case '#' : rc = ysrc_sreg_clear       (x_abbr);        break;
      case '-' : rc = ysrc_sreg__export     (x_abbr);        break;
      case '+' : rc = ysrc_sreg__import     (x_abbr);        break;
      default  :
                 DEBUG_YSRC   yLOG_note    ("divider (x_div) not a 2-char version");
                 DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
                 return rce;
      }
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(copy)---------------------------*/
   --rce;  if (x_len == 3 && x_div == '>') {
      rc = ysrc_sreg__copy (x_abbr, a_string [2]);
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(move)---------------------------*/
   --rce;  if (x_len == 3 && x_div == ']') {
      rc = ysrc_sreg__copy  (x_abbr, a_string [2]);
      rc = ysrc_sreg_clear  (x_abbr);
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(swap)---------------------------*/
   --rce;  if (x_len == 3 && x_div == ')') {
      rc = ysrc_sreg__copy  (a_string [2], '�');
      rc = ysrc_sreg__copy  (x_abbr      , a_string [2]);
      rc = ysrc_sreg__copy  ('�'         , x_abbr);
      rc = ysrc_sreg_clear  ('�');
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(assignment)---------------------*/
   --rce;  if (x_div != '=') {
      DEBUG_YSRC   yLOG_note    ("action not understood");
      DEBUG_YSRC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ysrc_sreg__save (x_abbr, a_string + 2);
   /*---(complete)-----------------------*/
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return rc;
}

char         /*-> process keys for register actions --[ leaf   [ge.QG5.287.FB]*/ /*-[02.0000.102.!]-*/ /*-[--.---.---.--]-*/
ysrc_sreg_smode         (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          =  -1;
   int         x_buf       =  -1;
   int         n           =   0;
   int         i           =   0;
   char       *x_start     = NULL;
   char        x_label     [10]        = "";
   int         x_diff      =   0;
   char        x_prev      = '"';
   static char x_data      [LEN_RECD]  = "";
   static int  x_dlen      =   0;
   /*---(header)-------------------------*/
   DEBUG_YSRC   yLOG_enter   (__FUNCTION__);
   DEBUG_YSRC   yLOG_char    ("a_major"   , a_major);
   DEBUG_YSRC   yLOG_char    ("a_minor"   , a_minor);
   /*> myVIKEYS.info_win = '-';                                                       <*/
   /*---(defenses)-----------------------*/
   DEBUG_YSRC   yLOG_char    ("mode"      , yMODE_curr ());
   --rce;  if (yMODE_not (SMOD_SREG)) {
      DEBUG_YSRC   yLOG_note    ("not the correct mode");
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(always cancel repeats)----------*/
   yKEYS_repeat_reset ();
   /*---(escape)-------------------------*/
   if (a_minor == G_KEY_ESCAPE)  {
      DEBUG_YSRC   yLOG_note    ("escape and return to previous mode");
      ysrc_select_reset (G_SREG_CURR);
      yMODE_exit ();
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return  0;
   }
   /*---(select register name)-----------*/
   --rce;  if (a_major == '"') {
      DEBUG_YSRC   yLOG_char    ("g_csreg"   , g_csreg);
      DEBUG_YSRC   yLOG_char    ("g_psreg"   , g_psreg);
      n = ysrc_sreg_index (tolower (a_minor));
      if (a_minor == '"' || a_minor == '�') {
         DEBUG_YSRC   yLOG_note    ("reuse the last register again");
         g_csreg = g_psreg;
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return 0;
      } else if (n >= 0) {
         DEBUG_YSRC   yLOG_note    ("select a source register");
         if (n > 0)  g_psreg = a_minor;
         g_csreg = a_minor;
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return 0;
      } else if (a_minor == '_') {
         DEBUG_YSRC   yLOG_note    ("show register status line");
         g_wsreg = g_csreg;
         yVIHUB_yCMD_direct (":status sreg");
         yMODE_exit ();
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return  0;
      } else if (a_minor == '?') {
         DEBUG_YSRC   yLOG_note    ("show source register inventory");
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         /*> myVIKEYS.info_win = 'y';                                                 <*/
         return  a_major;
      }
      yMODE_exit ();
      DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(select register action)---------*/
   --rce;  if (a_major == G_KEY_SPACE) {
      switch (a_minor) {
      case  '=' :
         DEBUG_YSRC   yLOG_note    ("switch to direct save mode");
         x_data [0] = '\0';
         x_dlen     = 0;
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return  a_minor;
         break;
      case  '#' :
         DEBUG_YSRC   yLOG_note    ("wipe source register");
         n = ysrc_sreg_index  (g_csreg);
         if (n < 0)  return -1;
         ysrc_sreg__wipeall ('-', &g_sregs [n]);
         yMODE_exit ();
         break;
      case  'y' :
         DEBUG_YSRC   yLOG_note    ("yank/copy (y) selection source to register");
         ysrc_copy          ();
         ysrc_select_reset (G_SREG_ROOT);
         yMODE_exit   ();
         break;
      case  'Y'  :
         DEBUG_YSRC   yLOG_note    ("yank/cut (Y) selection source");
         ysrc_copy          ();
         ysrc_delete_select ();
         ysrc_select_reset (G_SREG_CURR);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  'x' :
         DEBUG_YSRC   yLOG_note    ("clear (x) selection source");
         /*> ysrc_copy          ();                                                   <*/
         ysrc_clear_select  ();
         ysrc_select_reset (G_SREG_CURR);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  'd' :
         DEBUG_YSRC   yLOG_note    ("delete (d) selection source");
         /*> ysrc_copy          ();                                                   <*/
         ysrc_delete_select ();
         ysrc_select_reset (G_SREG_CURR);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  'c' :
         DEBUG_YSRC   yLOG_note    ("change (c) selection source");
         /*> ysrc_copy          ();                                                   <*/
         ysrc_delete_select ();
         ysrc_select_reset  (G_SREG_CURR);
         yMODE_exit         ();
         yMODE_enter        (UMOD_INPUT);
         ysrc_sundo_chain   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  'r' :
         DEBUG_YSRC   yLOG_note    ("replace (r) source from register");
         ysrc_replace        ();
         ysrc_select_reset (G_SREG_CURR);
         yMODE_exit         ();
         UPDATE_AFTER_CHANGES;
         break;
      case  's' :
         DEBUG_YSRC   yLOG_note    ("substitute (s) selection source");
         ysrc_delete_select ();
         ysrc_sundo_chain   ();
         ysrc_paste         ('i');
         ysrc_select_reset (G_SREG_CURR);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  'p' :
         DEBUG_YSRC   yLOG_note    ("paste after (a) selection source");
         ysrc_paste        ('a');
         ysrc_select_reset (G_SREG_CURR);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  'P' :
         DEBUG_YSRC   yLOG_note    ("paste before (i) selection source");
         ysrc_paste        ('i');
         ysrc_select_reset (G_SREG_CURR);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  '-' :
         DEBUG_YSRC   yLOG_note    ("export (-) content to clipboard");
         ysrc_sreg__export (g_csreg);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  '+' :
         DEBUG_YSRC   yLOG_note    ("import (+) content from clipboard");
         ysrc_sreg__import (g_csreg);
         yMODE_exit   ();
         UPDATE_AFTER_CHANGES;
         break;
      case  '>' :
         DEBUG_YSRC   yLOG_note    ("begin copy multi-key");
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return a_minor;
      case  ']' :
         DEBUG_YSRC   yLOG_note    ("begin copy multi-key");
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return a_minor;
      case  ')' :
         DEBUG_YSRC   yLOG_note    ("begin copy multi-key");
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return a_minor;
      default :
         DEBUG_YSRC   yLOG_note    ("minor key not understood");
         yMODE_exit   ();
         break;
      }
   }
   else if (a_major == '>') {
      DEBUG_YSRC   yLOG_note    ("copy one register to another");
      rc = ysrc_sreg__copy (g_csreg, a_minor);
      yMODE_exit   ();
   }
   else if (a_major == ']') {
      DEBUG_YSRC   yLOG_note    ("move one register to another");
      rc = 0;
      rc = ysrc_sreg__copy (g_csreg, a_minor);
      rc = ysrc_sreg_clear  (g_csreg);
      yMODE_exit   ();
   }
   else if (a_major == ')') {
      DEBUG_YSRC   yLOG_note    ("swap content of two registers");
      rc = ysrc_sreg__copy (g_csreg, '�');
      if (rc >= 0)  rc = ysrc_sreg__copy (a_minor, g_csreg);
      if (rc >= 0)  rc = ysrc_sreg__copy ('�', a_minor);
      if (rc >= 0)  rc = ysrc_sreg_clear ('�');
      yMODE_exit   ();
   }
   else if (a_major == '=') {
      DEBUG_YSRC   yLOG_note    ("creating a new sreg entry");
      if (a_minor == G_KEY_RETURN) {
         DEBUG_YSRC   yLOG_note    ("save new sreg");
         ysrc_sreg_push (g_csreg, x_data);
         yMODE_exit   ();
      }
      else if (x_dlen >= LEN_RECD - 1) {
         DEBUG_YSRC   yLOG_note    ("entry too long, char rejected");
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return  rce;
      }
      else {
         DEBUG_YSRC   yLOG_note    ("add another character");
         x_data [x_dlen]   = a_minor;
         x_data [++x_dlen] = '\0';
         DEBUG_YSRC   yLOG_complex ("x_data"    "%3d�%s�", x_dlen, x_data);
         DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
         return a_major;
      }
   }
   DEBUG_YSRC   yLOG_exit    (__FUNCTION__);
   return 0;
}





