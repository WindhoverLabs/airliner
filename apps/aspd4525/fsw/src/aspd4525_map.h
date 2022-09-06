#ifndef ASPD4525_MAP_H
#define ASPD4525_MAP_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#define ASPD4525_I2C_ADDRESS              (0x28)

#define ASPD4525_CONVERSION_INTERVAL_US   (100000)

#define ASPD4525_STATUS_MASK              (0xc0000000)  /*  2 bits top */
#define ASPD4525_DIFF_PRESSURE_MASK       (0x3fff0000)  /* 14 bits next*/
#define ASPD4525_TEMPERATURE_MASK         (0x0000FFE0)  /* 11 bits next*/

#define ASPD4525_STATUS_SHIFT             (30)          /*  2 bits top */
#define ASPD4525_DIFF_PRESSURE_SHIFT      (16)          /* 14 bits next*/
#define ASPD4525_TEMPERATURE_SHIFT        (5)           /* 11 bits next*/

/**
 * @brief This macro signifies that the data received from the ASPD-4525 is valid
 * 
 */
#define ASPD4525_MAPS_STATUS_SUCCESS                    (0)   /*0b00*/

/**
 * @brief This macro signifies that the data from ASPD-4525 is not ready to be read yet
 * 
 */
#define ASPD4525_MAPS_STATUS_BUSY                       (1)   /*0b01*/

/**
 * @brief This macro signifies that the data from ASPD-4525 is stale, and a new data should be read for accuracy
 * 
 */
#define ASPD4525_MAPS_STATUS_STALE                      (2)   /*0b10*/

/**
 * @brief This macro signifies that something is not right in the ASPD-4525 or its setup
 * 
 */
#define ASPD4525_MAPS_STATUS_ERROR                      (-1)   /*0b11*/


#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* ASPD4525_MAP_H */
