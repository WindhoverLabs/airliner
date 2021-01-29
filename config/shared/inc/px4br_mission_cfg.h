#ifndef PX4BR_MISSION_CFG_H
#define PX4BR_MISSION_CFG_H

/*
** Pragmas
*/

/*
** Local Defines
*/

/*
** Include Files
*/


/*
** Local Structure Declarations
*/

/*
 * Defines
 */
#define PX4BR_HEARTBEAT_SEND_PERIOD			1000
#define PX4BR_HEARTBEAT_TASK_NAME_TEMPLATE	"PX4BR_HB_"
#define PX4BR_HEARTBEAT_TASK_STACK_SIZE 	24576
#define PX4BR_HEARTBEAT_TASK_PRIORITY		120
#define PX4BR_HEARTBEAT_SEND_DELAY       	1000

#define PX4BR_LISTENER_TASK_NAME_TEMPLATE	"PX4BR_LST_"
#define PX4BR_LISTENER_TASK_STACK_SIZE 	24576
#define PX4BR_LISTENER_TASK_PRIORITY		120
#define PX4BR_LISTENER_SEND_DELAY       	1000

#define PX4BR_DATAOUT_TASK_NAME_TEMPLATE	"PX4BR_DO_"
#define PX4BR_DATAOUT_TASK_STACK_SIZE 		24576
#define PX4BR_DATAOUT_TASK_PRIORITY			120
#define PX4BR_DATAOUT_SEND_DELAY       		1000

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

#endif /* PX4BR_MISSION_CFG_H */

    
