
/************************************************************************
** Pragmas
*************************************************************************/

/************************************************************************
** Includes
*************************************************************************/
#include "cfe_tbl_filedef.h"
#include "{{cookiecutter.app_name|lower}}_tbldefs.h"

/************************************************************************
** Local Defines
*************************************************************************/

/************************************************************************
** Local Structure Definitions
*************************************************************************/

/**
** \brief The cFE {{cookiecutter.app_name}} config table definition.
**
** Content format: ObjName[64], TblName[38], Desc[32], TgtFileName[20], ObjSize
**    ObjName - variable name of config table, e.g., {{cookiecutter.app_name}}_ConfigDefTbl[]
**    TblName - app's table name, e.g., {{cookiecutter.app_name}}.CONFIG_TBL, where {{cookiecutter.app_name}} is the same app name
**              used in cfe_es_startup.scr, and {{cookiecutter.app_name}}_defConfigTbl is the same table
**              name passed in to CFE_TBL_Register()
**    Desc - description of table in string format
**    TgtFileName[20] - table file name, compiled as .tbl file extension
**    ObjSize - size of the entire table
**
*/
static CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "{{cookiecutter.app_name}}_ConfigTbl", "{{cookiecutter.app_name}}.CONFIG_TBL", "{{cookiecutter.app_name}} default config table",
    "{{cookiecutter.app_name|lower}}_config.tbl", (sizeof({{cookiecutter.app_name}}_ConfigTblEntry_t) * {{cookiecutter.app_name}}_CONFIG_TABLE_MAX_ENTRIES)
};

/************************************************************************
** External Global Variables
*************************************************************************/

/************************************************************************
** Global Variables
*************************************************************************/

/**
**  \brief Default {{cookiecutter.app_name}} config table data
*/
{{cookiecutter.app_name}}_ConfigTblEntry_t {{cookiecutter.app_name}}_ConfigTbl[{{cookiecutter.app_name}}_CONFIG_TABLE_MAX_ENTRIES] =
{
    /* Entry 1 */
    {
        .iParam = 123

        /* TODO:  Add default values for Config parameters here.
        **
        ** Examples: (See example of type declarations in {{cookiecutter.app_name|lower}}_tbldefs.h)
        **    .cParam   = 7,
        **    .cParams  = "Init Str Value",
        **    .ucParam  = 'A',
        **    .ucParams = "/dev/ttyUSB",
        **
        **    .sParam   = -16,
        **    .sParams  = {-3, -2, -1, 0, 1, 2, 3, 4},
        **    .usParam  = 16,
        **    .usParams = {1, 2, 3, 4, 5, 6, 7, 8},
        **
        **    .iParam   = -32,
        **    .iParams  = {-123, -234, 0, 123, 234},
        **    .uiParam  = -32,
        **    .uiParams = {123, 234, 345, 456, 678},
        **
        **    .fParam  = 32.1234,
        **    .fParams = {-12.34, 0.0, 12.34},
        **
        **    .dParam = 123.456789,
        **    .dParams = {-123.456789, 0.0, 123.456789}
        */
    }
};

/************************************************************************
** Local Variables
*************************************************************************/

/************************************************************************
** Local Function Prototypes
*************************************************************************/

/************************************************************************
** Function Definitions
*************************************************************************/

/************************/
/*  End of File Comment */
/************************/
    
