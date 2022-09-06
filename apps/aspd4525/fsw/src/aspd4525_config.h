#ifndef ASPD4525_CONFIG_H
#define ASPD4525_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/


/* Config Table Modes for Air Density Calculation. */
typedef enum {

/** \brief <tt> 'ASPD4525_CONFIG_AIRDENSITY_NO_ALTITUDE_MODE - ' </tt>
**  \mode <tt> 'ASPD4525_CONFIG_AIRDENSITY_NO_ALTITUDE_MODE - ' </tt>
**  
**  \par Type: Congiuration
**
**
*/
    ASPD4525_CONFIG_AIRDENSITY_NO_ALTITUDE_MODE = 0,

/** \brief <tt> 'ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_MODE - ' </tt>
**  \mode <tt> 'ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_MODE - ' </tt>
**  
**  \par Type: Congiuration
**
*/
    ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_MODE,

/** \brief <tt> 'ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_TEMPERATURE_MODE - ' </tt>
**  \mode <tt> 'ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_TEMPERATURE_MODE - ' </tt>
**  
**  \par Type: Congiuration
**
*/
    ASPD4525_CONFIG_AIRDENSITY_ALTITUDE_TEMPERATURE_MODE
} ASPD4525_CONFIG_AirDensity_Modes_t;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*ASPD4525_CONFIG_H*/