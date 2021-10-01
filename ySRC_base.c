/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "ySRC.h"
#include    "ySRC_priv.h"



char
ySRC_init               (void)
{
   return 0;
}

char
ySRC_wrap               (void)
{
   return 0;
}


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char           unit_answer [LEN_FULL];

char       /*----: set up program urgents/debugging --------------------------*/
ysrc__unit_quiet        (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMODE_unit" };
   /*> yURG_logger   (x_narg, x_args);                                                <*/
   /*> yURG_urgs     (x_narg, x_args);                                                <*/
   yMODE_init (MODE_MAP);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysrc__unit_loud         (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMODE_unit" };
   yURG_logger   (x_narg, x_args);
   yURG_urgs     (x_narg, x_args);
   yURG_name  ("kitchen"      , YURG_ON);
   yURG_name  ("mode"         , YURG_ON);
   yURG_name  ("ystr"         , YURG_ON);
   DEBUG_MODE  yLOG_info     ("yMODE"     , yMODE_version   ());
   yMODE_init (MODE_MAP);
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ysrc__unit_end          (void)
{
   DEBUG_MODE  yLOG_enter   (__FUNCTION__);
   yMODE_wrap ();
   DEBUG_MODE  yLOG_exit    (__FUNCTION__);
   yLOGS_end    ();
   return 0;
}

char*        /*-> unit test accessor -----------------[ light  [us.420.111.11]*/ /*-[01.0000.00#.Z]-*/ /*-[--.---.---.--]-*/
ySRC__unit              (char *a_question, int n)
{
   /*---(locals)-----------+-----------+-*/
   char        r           [LEN_FULL]  = "";
   char        s           [LEN_FULL]  = "";
   char        t           [LEN_FULL]  = "";
   /*---(preprare)-----------------------*/
   strcpy  (unit_answer, "SRC unit         : question not understood");
   /*---(selection)----------------------*/

   /*---(complete)-----------------------*/
   return unit_answer;
}


