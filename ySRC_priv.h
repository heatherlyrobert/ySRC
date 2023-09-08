/*============================----beg-of-source---============================*/
#ifndef ySRC_priv
#define ySRC_priv yes


/*===[[ HEADER BEG ]]=========================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_NAME      "ySRC"
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "us (user control)"
#define     P_SUBJECT   "efficent ascii-text content creation"
#define     P_PURPOSE   "provides efficient, flexible ascii-text content creation services"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "angelia-minyma (message)"
#define     P_PRONOUNCE ""
#define     P_TERSE     "embodiment of the gods communications"
#define     P_HERITAGE  "daimona, message personified, daughter of the god hermes"
#define     P_BRIEFLY   ""
#define     P_IMAGERY   "beautiful young woman with graceful wings and feathers in her hair"
#define     P_REASON    "this library is about editing source/messages"
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/ySRC.vi_keys_input_processing"
#define     P_BASENAME  "ySRC"
#define     P_FULLNAME  "/usr/local/lib64/libySRC"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 11.3.0"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"
/*········· ··········· ´·····························´········································*/
#define     P_DEPSTDC   "stdio,stdlib,string"
#define     P_DEPGRAPH  "···"
#define     P_DEPCORE   "yURG,yLOG,ySTR"
#define     P_DEPVIKEYS "yVIHUB,yMODE,yKEYS"
#define     P_DEPOTHER  "···"
#define     P_DEPSOLO   "···"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   ""
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "2.--, clean, improve, and expand"
#define     P_VERMINOR  "2.1-, back onto github and moving forward"
#define     P_VERNUM    "2.1p"
#define     P_VERTXT    "improvements found by vikeys_content.script demo"
/*········· ··········· ´·····························´········································*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*===[[ HEADER END ]]=========================================================*/



/*---(ansi-c standard)-------------------*/
#include    <stdio.h>             /* clibc  standard input/output             */
#include    <stdlib.h>            /* clibc  standard general purpose          */
#include    <string.h>            /* clibc  standard string handling          */
/*---(custom core)-----------------------*/
#include    <yURG.h>              /* heatherly urgent processing              */
#include    <yLOG.h>              /* heatherly program logging                */
#include    <ySTR.h>              /* heatherly string processing              */
/*---(custom vi-keys)--------------------*/
#include    <yVIHUB.h>
#include    <yMODE.h>             /* heatherly vikeys mode tracking          */
#include    <yKEYS.h>             /* heatherly vikeys key handling           */
/*---(done)------------------------------*/



extern char    (*s_saver) (char *a_contents);
extern char    (*e_sreg)  (char a_reg, char *a_contents);
extern uchar    *g_stub;
extern uchar     g_char;
extern uchar     g_goto;



#define     S_SREG_NOT '-'
#define     S_SREG_YES 'y'


typedef  struct cEDIT   tEDIT;
struct cEDIT {
   uchar       type;                      /* type of input                    */
   uchar       label       [LEN_LABEL];   /* source label                     */
   uchar       format      [LEN_LABEL];   /* formatting information           */
   uchar       original    [LEN_RECD];    /* pre-edit content                 */
   uchar       contents    [LEN_RECD];    /* working content                  */
   short       wide;                      /* full display space width         */
   short       apos;                      /* available space for display      */
   short       npos;                      /* length of edited content         */
   short       cpos;                      /* current character position       */
   short       bpos;                      /* beginning of visiable part       */
   short       epos;                      /* end of visiable part             */
   char        words       [LEN_RECD];    /* working space for wbe keys       */
};
extern tEDIT   s_src;
extern tEDIT   s_cmd;
extern tEDIT  *s_cur;

#define      EDIT_NORM        '-'
#define      EDIT_CMDS        'c'
#define      EDIT_SRCH        's'




#define        S_SREG_MAX      100
typedef struct  cSREG  tSREG;
struct cSREG {
   /*---(active)------------*/
   char        active;                      /* register in use                */
   char        source;                      /* user, file, import             */
   /*---(source)------------*/
   char       *label;                       /* original cell                  */
   short       beg;                         /* original starting point        */
   short       end;                         /* original ending point          */
   short       root;
   /*---(contents)----------*/
   short       len;                         /* length of text                 */
   char       *data;                        /* text                           */
   /*---(done)--------------*/
};
extern char   G_SREG_LIST     [S_SREG_MAX];
extern tSREG  g_sreg;
extern tSREG  g_sregs  [S_SREG_MAX];
extern uchar  g_nsreg;
extern uchar  g_csreg;
extern uchar  g_wsreg;
extern uchar  g_psreg;
extern tSREG  g_save;                       /* last selection                 */

#define   G_SREG_ZERO    'z'
#define   G_SREG_CURR    'c'
#define   G_SREG_BEG     'b'
#define   G_SREG_END     'e'
#define   G_SREG_ROOT    'r'



typedef    struct    cMY    tMY;
struct cMY {
   /*---(wander origin)-----*/
   uchar       w_contents  [LEN_RECD];
   ushort      w_uorig;
   ushort      w_xorig;
   ushort      w_yorig;
   ushort      w_zorig;
   /*---(wander screen beg)-*/
   ushort      w_xbeg;
   ushort      w_ybeg;
   ushort      w_zbeg;
   /*---(wander current)----*/
   ushort      w_univ;
   ushort      w_xpos;
   ushort      w_ypos;
   ushort      w_zpos;
   /*---(wander working)----*/
   uchar       w_pre       [LEN_RECD];
   uchar       w_suf       [LEN_RECD];
   uchar       w_new       [LEN_RECD];
   char        w_abs;
   /*---(holding)-----------*/
   uchar       yank        [LEN_RECD];
   int         ylen;
   int         ybeg;
   /*---(marks)-------------*/
   short       s_marks     [LEN_HUND];
   /*---(sets)--------------*/
   uchar       s_set       [256];
   uchar       s_tes       [256];
   uchar       s_text      [LEN_HUND];
   uchar       s_code      [LEN_HUND];
   uchar       s_once      [LEN_TERSE];
   uchar       s_act;
   /*---(done)--------------*/
};
extern tMY         mySRC;



/*===[[ ySRC_sundo.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#define     S_SUNDO_MAX      1000
typedef struct cSUNDO  tSUNDO;
struct cSUNDO {
   int         seq;
   char        major;
   char        minor;
   int         cpos;
   uchar       before;
   uchar       after;
};
extern tSUNDO g_sundos  [S_SUNDO_MAX];
extern int    g_nsundo;
extern int    g_csundo;
extern int    g_nseq;


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
#define     UPDATE_BEFORE_CHANGES   ysrc_before ()
#define     UPDATE_AFTER_CHANGES    ysrc_after  ()
/*---(program)--------------*/
char*       ySRC_version            (void);
char        ySRC_init               (void);
char        ySRC_wrap               (void);
/*---(ends)-----------------*/
char        ysrc_reset              (void);
char        ySRC_update             (char *a_label, char *a_format, char *a_contents);
char        ySRC_start              (char *a_prefix);
char        ysrc_before             (void);
char        ysrc_after              (void);
char        ysrc_accept             (void);
char        ysrc_reject             (void);
/*---(unittest)-------------*/
char        ysrc__unit_quiet        (void);
char        ysrc__unit_loud         (void);
char        ysrc__unit_end          (void);
char*       ySRC__unit              (char *a_question, int n);



/*===[[ ySRC_source.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(workers)--------------*/
char        ysrc_replace_one        (uchar a_key);
char        ysrc_delete_one         (void);
char        ysrc_backspace_one      (void);
char        ysrc_insert_one         (uchar a_key);
char        ysrc_insert_and_move    (uchar a_key);
char        ysrc_append_one         (uchar a_key);
/*---(selection)------------*/
char        ysrc_clear_select       (void);
char        ysrc_delete_select      (void);
char        ysrc_multi_pure         (uchar a_major, uchar a_minor);
char        ysrc_copy               (void);
char        ysrc_cut                (void);
char        ysrc_delete             (void);
char        ysrc_paste              (char a_dir);
char        ysrc_replace            (void);
char        ysrc_swap               (char *a_new);
/*---(accessors)------------*/
int         ysrc_cpos               (void);
int         ysrc_npos               (void);
char       *ysrc_contents           (void);



/*===[[ ySRC_sundo.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(program)--------------*/
char        ysrc_sundo_trim         (int a_start);
char        ysrc_sundo_reset        (void);
char        ysrc_sundo_init         (void);
/*---(grouping)-------------*/
char        ysrc_sundo_beg          (void);
char        ysrc_sundo_chain        (void);
char        ysrc_sundo_end          (void);
/*---(record)---------------*/
char        ysrc_sundo_add          (char a_major, char a_minor, short a_pos, char a_before, char a_after);
char        ysrc_sundo_single       (char a_minor, short a_pos, char a_before, char a_after);
/*---(undo)-----------------*/
char        ysrc_sundo__undo        (void);
char        ysrc_sundo_undo         (void);
/*---(redo)-----------------*/
char        ysrc_sundo__redo        (void);
char        ysrc_sundo_redo         (void);
/*---(reporting)------------*/
char        ysrc_sundo_status       (char a_size, short a_wide, char *a_list);
/*---(done)-----------------*/



/*===[[ ySRC_input.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc__input_wipe        (void);
char        ysrc_input_init         (void);
char        ysrc_input_prepper      (void);
char        ysrc__input_biggies     (uchar a_major, uchar a_minor);
char        ysrc__input_escaped     (uchar a_major, uchar a_minor);
char        ysrc__input_editing     (uchar a_major, uchar a_minor);
char        ysrc_input__add         (uchar a_minor);
char        ysrc_input_umode        (uchar a_major, uchar a_minor);
char        ysrc__input_unit        (char *a_text);



/*===[[ ySRC_sreg.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        ysrc_sreg__wipedata     (tSREG *a_dst);
char        ysrc_sreg__wipeall      (char a_scope, tSREG *a_dst);
/*---(program)--------------*/
char        ysrc_sreg_purge         (char a_scope);
char        ysrc_sreg_init          (void);
/*---(utility)--------------*/
char        ysrc_sreg_index         (uchar a_abbr);
char        ysrc_sreg_setreg        (uchar a_abbr);
char        ysrc_sreg_setwork       (uchar a_abbr);
char        ysrc_sreg_clear         (uchar a_abbr);
char        ysrc_sreg_push          (uchar a_abbr, char *a_data);
char        ysrc_sreg_pop           (uchar a_abbr, char *a_data);
char        ysrc_sreg_fetch         (short *a_len, char *a_data);
char        ysrc_sreg_save          (void);
/*---(status)---------------*/
char        ysrc_sreg_info          (int a_index, char *a_entry);
/*---(mode)-----------------*/
char        ysrc_sreg__export       (char a_abbr);
char        ysrc_sreg__import       (char a_abbr);
char        ysrc_sreg__copy         (char a_abbr, char a_dst);
char        ysrc_sreg__save         (char a_abbr, char *a_string);
char        ysrc_sreg_direct        (char *a_string);
char        ysrc_sreg_smode         (uchar a_major, uchar a_minor);
/*---(done)-----------------*/



/*===[[ ySRC_input.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_select_reset       (char a_type);
char        ysrc_select_update      (short a_pos);
short       ysrc_select_reverse     (void);
char        ysrc_select_exact       (short a_beg, short a_end, short a_root);
char        ysrc_select_curr        (short *a_beg, short *a_end, short *a_root);
char        ysrc_select_get         (short *a_beg, short *a_end, short *a_root, short *a_len, char *a_data);
char        ysrc_select_all         (void);
char        ysrc_select_getlive     (void);
char        ySRC_select_islive      (void);
char        ysrc_select_makelive    (void);



/*===[[ ySRC_input.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
uchar       ysrc_word__type         (char a_type, char a_curr);
char        ysrc_word__curr         (char a_type, char a_save, char a_mode);
char        ysrc_word__prev         (char a_type, char a_save, char a_mode);
char        ysrc_word__update       (short a_pos, char a_curr);
char        ysrc_word_update        (void);
char        ysrc_word_next          (uchar a_major, uchar a_minor, short *a_cur, short *a_len);
char        ysrc_word_end           (uchar a_major, uchar a_minor, short *a_cur, short *a_len);
char        ysrc_word_prev          (uchar a_major, uchar a_minor, short *a_cur, short *a_len);
char        ysrc_word_hmode         (uchar a_major, uchar a_minor);


/*===[[ ySRC_move.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(absolute)-------------*/
char        ysrc_move_simple        (uchar a_major, uchar a_minor);
char        ysrc_move_char_next     (uchar a_ch);
char        ysrc_move_char_prev     (uchar a_ch);
/*---(screen)---------------*/
char        ysrc_move_goto          (uchar a_major, uchar a_minor);
char        ysrc_move_scroll        (uchar a_major, uchar a_minor);
/*---(done)-----------------*/



/*===[[ ySRC_source.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_source_push        (char a_prev, int a_tail);
char        ysrc_source_pop         (void);
char        ysrc_source_smark_set   (uchar a_mark);
char        ysrc_source_smark_clear (uchar a_mark);
char        ysrc_source_smark_go    (uchar a_mark);
char        ysrc_source_smark_purge (void);
char        ysrc_source_set_purge   (void);
char        ysrc_source_set_set     (uchar a_major, uchar *a_text);
char        ysrc__source_biggies    (uchar a_major, uchar a_minor);
char        ysrc__source_macro      (uchar a_major, uchar a_minor);
char        ysrc__source_subs       (uchar a_major, uchar a_minor);
char        ysrc__source_findchar   (uchar a_major, uchar a_minor);
char        ysrc__source_editing    (uchar a_major, uchar a_minor);
char        ysrc__source_multikey   (uchar a_major, uchar a_minor);
char        ysrc_source_escaped     (uchar a_major, uchar *a_minor);
char        ySRC_mode               (uchar a_major, uchar a_minor);
char        ysrc__source_unit       (char *a_text);



/*===[[ ySRC_replace.c ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_replace_init       (void);
char        ysrc__replace_wipe      (void);
char        ysrc_replace_prepper    (void);
char        ysrc__replace_biggies   (uchar a_major, uchar a_minor);
char        ysrc__replace_escaped   (uchar a_major, uchar a_minor);
char        ysrc__replace_editing   (uchar a_major, uchar a_minor);
char        ysrc_replace_umode      (uchar a_major, uchar a_minor);



/*===[[ ySRC_wander.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc__wander_loc        (char *a_label, char *a_len);
char        ysrc_wander_prepper     (void);
char        ysrc__wander_return     (void);
char        ysrc__wander_done       (uchar a_key);
char        ysrc__wander_and        (uchar a_math);
char        ysrc__wander_biggies    (uchar a_major, uchar a_minor);
char        ysrc_wander_umode       (uchar a_major, uchar a_minor);



/*===[[ ySRC_rptg.c ]]========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysrc_select_line        (char *a_entry);
char        ysrc_sreg_line          (uchar a_abbr, char *a_entry);
char        ysrc_sreg_status        (char a_size, short a_wide, char *a_list);
char        ysrc_sreg_dump          (void *f);


#endif

