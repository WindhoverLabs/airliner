/************************************************************************
** Includes
*************************************************************************/
#include "cfe.h"
#include "hk_utils.h"
#include "hk_app.h"
#include "hk_msgids.h"
#include "hk_tbldefs.h"
#include "cfe_tbl_filedef.h"

hk_copy_table_entry_t      HK_CopyTable[HK_COPY_TABLE_ENTRIES] =
{
/*         inputMid        inputOffset     outputMid    outputOffset  numBytes*/

/*   0 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*   1 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*   2 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*   3 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*   4 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },

/*   5 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*   6 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*   7 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*   8 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },

/*   9 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  10 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  11 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  12 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  13 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  14 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  15 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },                                                                                                        
/*  16 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  17 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  18 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  19 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  20 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  21 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  22 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  23 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  24 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  25 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },                                                                                                        
/*  26 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  27 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  28 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  29 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  30 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  31 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  32 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },                                                                                                        
/*  33 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  34 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  35 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  36 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  37 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  38 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  39 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  40 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  41 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  42 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  43 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  44 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  45 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  46 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  47 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  48 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  49 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  50 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  51 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  52 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  53 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  54 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  55 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  56 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  57 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  58 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  59 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  60 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  61 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  62 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  63 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  64 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  65 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  66 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  67 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  68 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  69 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  70 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  71 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  72 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  73 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  74 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  75 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  76 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  77 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  78 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  79 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  80 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  81 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  82 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  83 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  84 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  85 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  86 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  87 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  88 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  89 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  90 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  91 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  92 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  93 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  94 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  95 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  96 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  97 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  98 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/*  99 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 100 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 101 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 102 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 103 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 104 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 105 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 106 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 107 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 108 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 109 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 110 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 111 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 112 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 113 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 114 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 115 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 116 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 117 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 118 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 119 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 120 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 121 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 122 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 123 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 124 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 125 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 126 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },
/* 127 */ { HK_UNDEFINED_ENTRY,    0,   HK_UNDEFINED_ENTRY,       0,   0, },

};

/*
** Table file header
*/
CFE_TBL_FILEDEF(HK_CopyTable, HK.CopyTable, HK Copy Tbl, hk_cpy_tbl.tbl)

/************************/
/*  End of File Comment */
/************************/
