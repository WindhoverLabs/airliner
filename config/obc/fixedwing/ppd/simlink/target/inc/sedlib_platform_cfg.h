    
#ifndef SEDLIB_PLATFORM_CFG_H
#define SEDLIB_PLATFORM_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** sedlib Platform Configuration Parameter Definitions
*/

/** \brief Mission specific version number for SEDLIB application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined here and the other
**       parts are defined in "sedlib_version.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define SEDLIB_MISSION_REV                (0)

#define SEDLIB_PIPE_MAX_INSTANCES         (100)
#define SEDLIB_PIPE_NAME_MAX_LENGTH       (40)

#ifdef __cplusplus
}
#endif

#endif /* SEDLIB_PLATFORM_CFG_H */

/************************/
/*  End of File Comment */
/************************/
    
