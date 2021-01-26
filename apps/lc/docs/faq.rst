FAQ
================

TODO


Q) Watchpoints evaluate to LC_WATCH_TRUE or LC_WATCH_FALSE. When the the watchpoint is defined which should be the error condition?

Watchpoints evaluate to a boolean True or False. So if you setup a comparison < 100 any watchpoint value 0 to 99 will evaluate LC_WATCH_TRUE and 100+ will be LC_WATCH_FALSE.

You want to construct the watchpoint cases so they evaluate True when you are outside of the acceptable range for a telemetry point. That's why there are cumulative and consecutive true counts in the watchpoint results table, but no corresponding values for False.

For some examples, see the file lc_def_wdt.c

(Q) There is only a single comparison value for each watchpoint. How can I have multiple thresholds for a single telemetry point?

Create additional watchpoint definitions that reference the same telemetry point but have different comparison values. The watchpoints will be evaluated in the order they are listed in the WDT when the monitor point arrives.

Having multiple watchpoints allows different Actionpoints to trigger on each watchpoint state (or combination of states).

(Q) How do I calculate the watchpoint offset?

The offset is a zero based byte offset from the beginning of the message (including any headers) to the first byte of the watchpoint data. So for a cFE raw command using CCSDS, the offset has to account for the size of the cFE command header (CFE_SB_CMD_HDR_SIZE).

(Q) When do I need to use the bitmask in a watchpoint definition?

All watchpoints are sized to a 32 bit value when extracted from a message. The specified bitmask value is then applied (as a bitwise AND operation) before the comparison is made or the custom function is called.

When the watchpoint data is sized, data types smaller than 32 bits are properly sign or zero extended. For this reason, it is not necessary to define a mask for UWORD, WORD or UBYTE, BYTE data types to compare properly. Although it's not needed, a properly constructed bit mask will not cause a problem for these evaluations either.

When you really need to use a bitmask is when monitoring odd sized data (such as a 24 bit sensor reading) or testing data to see if certain bits have been set (or cleared).

When no bitmask is needed, be sure to use the constant LC_NO_BITMASK (or it's equivalent value 0xFFFFFFFF) in your WDT tables. Since masking is a bitwise AND, setting the bitmask to zero will have the effect of always clearing the watchpoint data prior to comparison.

(Q) Why do I need to specify the byte order (big or little endian) of the watchpoint data type?

One enhancement made to the CFS version of LC is that it will properly byte swap watchpoint data prior to masking and comparison if the byte order of the data is different than the order used by the processor running LC. This allows LC to monitor telemetry data that might be constructed by instrument or other subsystem processors that use a different byte order.

LC determines it's byte order using the STRUCT_HIGH_BIT_FIRST and STRUCT_LOW_BIT_FIRST compiler switches that are set in the cFE header common_types.h according to the platform designated in the cFE platform makefile.

(Q) When would a watchpoint evalute to #LC_WATCH_NOT_MEASURED?

#LC_WATCH_NOT_MEASURED is an initialization value for the Watchpoint Results Table. If a watchpoint has this WatchResult then the watchpoint is unused (the DataType in the WDT is set to LC_WATCH_NOT_USED) or a message that contains the watchpoint has not yet been recieved by LC and evaluated.

(Q) When would a watchpoint evalute to LC_WATCH_ERROR?

LC_WATCH_ERROR is a runtime error indicator for watchpoint processing. Such a result should be rare since most of the causes are invalid watchpoint parameters that should be caught during validation of the Watchpoint Definition Table.

One notable exception is for floating point watchpoints when the message data is detected to be a floating point NAN (Not-A-Number) that can't be relationally compared to any value.

In all cases, an error event detailing the cause of the problem will be issued when a WatchResult is set to LC_WATCH_ERROR.

(Q) What is a custom function and how do I create one?

A custom function is a way to do more complicated processing on a watchpoint when the standard comparison operators are not adequate.

When the OperatorID in a watchpoint definition is set to LC_OPER_CUSTOM. The function LC_CustomFunction is called instead of LC doing a comparison. LC_CustomFunction will then return LC_WATCH_TRUE or LC_WATCH_FALSE that will be used as the WatchResult.

Although LC_CustomFunction is the same entry point for all watchpoints with a designated custom function operator, a mission can modify the function body to switch on the watchpoint ID (or CustomFuncArgument specified in the watchpoint definition) and branch to as many different sub-functions as it needs for watchpoint specific processing.

The LC_CustomFunction stub is in the source file lc_custom.c

(Q) How is the WPResults array in the housekeeping packet (LC_HkPacket_t) interpreted?

The WPResults array is a byte array (aligned to the nearest longword boundary) that contains a packed subset of the current contents of the Watchpoint Results Table (see LC_WRTEntry_t).

It allocates 2 bits per watchpoint for the most recent watchpoint comparison result. The numerical 2 bit values are defined using the following constants:

LC_HKWR_FALSE, LC_HKWR_TRUE, LC_HKWR_ERROR, #LC_HKWR_NOT_MEASURED

Ordering (up to LC_MAX_WATCHPOINTS) is as follows :

Byte 0:(Rwp3, Rwp2, Rwp1, Rwp0), Byte 1:(Rwp7, Rwp6, Rwp5, Rwp4), etc...

The WPResults array is constructed every housekeeping cycle and is not effected by the reset counters (LC_RESET_CC) ground command. It will only get cleared if the Watchpoint Results Table is reset (via a new WDT table load or with a LC_RESET_WP_STATS_CC ground command)
Actionpoint Questions

(Q) Actionpoints evaluate to LC_ACTION_PASS or LC_ACTION_FAIL. How should the RPN expression get constructed so it will evaluate to the proper result?

Actionpoint RPN expressions are combinations of watchpoint states and logical operators that evaluate to a boolean True or False.

Just like a watchpoint evaluation of True indicates a parameter outside acceptable limits, an AP expression that evaluates True is considered to have Failed and will have it's ActionResult set to LC_ACTION_FAIL.

While the terminology may seem confusing, the key point is to construct both watchpoint and actionpoint expressions to define the error condition LC is looking for and NOT the normal condition of the spacecraft data stream.

For some examples, see the file lc_def_adt.c

(Q) When would a actionpoint evalute to #LC_ACTION_NOT_MEASURED?

#LC_ACTION_NOT_MEASURED is an initialization value for the Actionpoint Results Table. If an actionpoint has this ActionResult then one of three possible conditions are true:

1) The actionpoint is unused (the DefaultState in the ADT is set to LC_ACTION_NOT_USED).

2) An actionpoint sample request (LC_SAMPLE_AP_MID) targeting the AP has not yet been recieved by LC so the AP has not yet been evaluated.

3) One or more of the watchpoints that this AP depends on (as defined by the RPN expression) has a current WatchResult of #LC_WATCH_NOT_MEASURED so the AP can't be evaluated.

(Q) When would a actionpoint evalute to LC_ACTION_ERROR?

LC_ACTION_ERROR is a runtime error indicator for actionpoint processing. Such a result should be rare since most of the causes are invalid actionpoint parameters or improperly constructed RPN expressions that should be caught during validation of the Actionpoint Definition Table.

However, an exception is the case where one or more watchpoints that this AP depends on (as defined by the RPN expression) has a current WatchResult of LC_WATCH_ERROR. Since the AP can't be evaluated, this will cause the ActionResult to be set to LC_ACTION_ERROR.

In all cases, an error event detailing the cause of the problem will be issued when an ActionResult is set to LC_ACTION_ERROR.

(Q) How does the Actionpoint state LC_APSTATE_PERMOFF differ from LC_APSTATE_DISABLED?

The AP state LC_APSTATE_PERMOFF is intended to provide a way to disable an AP so it can not easily be turned back on by mistake. Such actionpoints may not be needed after a seperation sequence or only apply to certain mission phases.

While the two states are treated the same way during actionpoint processing (the AP isn't evaluated), there are a few differences.

An AP can't be set to LC_APSTATE_PERMOFF with the LC_SET_AP_STATE_CC command, it must be done with the LC_SET_AP_PERMOFF_CC command.

To set an AP to LC_APSTATE_PERMOFF with the LC_SET_AP_PERMOFF_CC command, the current AP state must be LC_APSTATE_DISABLED.

The LC_SET_AP_PERMOFF_CC command can only be issued for a single actionpoint, LC_ALL_ACTIONPOINTS is not valid as an argument for this command.

Once an AP is set to LC_APSTATE_PERMOFF, it can only be changed with a new ADT table load.

(Q) How is the APResults array in the housekeeping packet (LC_HkPacket_t) interpreted?

The APResults array is a byte array (aligned to the nearest longword boundary) that contains a packed subset of the current contents of the Actionpoint Results Table (see LC_ARTEntry_t).

It allocates 4 bits per actionpoint, with 2 bits representing the current state, and 2 bits for the most recent evaluation result.

The numerical 2 bit values for current state are defined using the following constants:

LC_HKAR_STATE_NOT_USED, LC_HKAR_STATE_ACTIVE, LC_HKAR_STATE_PASSIVE, LC_HKAR_STATE_DISABLED

An actionpoint whose current state is LC_APSTATE_PERMOFF will have it's state reported in the APResults as LC_HKAR_STATE_NOT_USED.

The numerical 2 bit values for evaluation results are defined using the following constants:

LC_HKAR_PASS, LC_HKAR_FAIL, LC_HKAR_ERROR, #LC_HKAR_NOT_MEASURED

Ordering (up to LC_MAX_ACTIONPOINTS) is as follows :

Byte 0:(Sap1, Rap1, Sap0, Rap0), Byte 1:(Sap3, Rap3, Sap2, Rap2), etc...

The APResults array is constructed every housekeeping cycle and is not effected by the reset counters (LC_RESET_CC) ground command. It will only get cleared if the Actionpoint Results Table is reset (via a new ADT table load or with a LC_RESET_AP_STATS_CC ground command)

(Q) Will an RTS get requested more than once if an AP stays in the LC_ACTION_FAIL state?

No. Assuming the current state of an actionpoint is LC_APSTATE_ACTIVE, then when the actionpoint fails enough times to trigger an RTS, the state is set to LC_APSTATE_PASSIVE.

In the passive state, the AP will continue to be sampled and statistics updated, but no RTS requests will be initiated.

(Q) Can we filter event messages per actionpoint?

Not in the current implementation. Transition event messages for actionpoints are of type CFE_EVS_DEBUG and enabling them will turn on events for all actionpoint transitions.

However, it is planned to add event message limits and event message counts to the ADT and ART tables that would allow this type of filtering in a future version of LC.
General Questions

(Q) When do results tables get cleared?

When either the WDT or ADT are updated, the corresponding results table (WRT or ART) is reset to initialization values. For each entry in the WRT, WatchResult is set to #LC_WATCH_NOT_MEASURED and all other values are zeroed. For each entry in the ART, ActionResult is set to #LC_ACTION_NOT_MEASURED, the CurrentState is set to the value of the actionpoint's DefaultState specified in the ADT, and all other values are zeroed.

These are also the values used (for the entries specified in the command) when the LC_RESET_AP_STATS_CC or LC_RESET_WP_STATS_CC ground command is received.

(Q) Where can I find some examples of watchpoint and actionpoint table definitions?

The files lc_def_wdt.c and lc_def_adt.c provide example source code files that can be used to build LC tables and contain some sample table entries in the comment blocks.

Since the CFS LC was based upon the LC implementation for LRO, LRO limit checker tables may also provide guidance though there are some minor differences in syntax between the two implementations.

(Q) When using the CDS, what is restored and when will defaults be used?

When the platform configuration parameter LC_SAVE_TO_CDS is defined, LC will attempt to use the critical data store (CDS) to save and restore data across application restarts. The data that LC will save are:

Watchpoint and Actionpoint Definition Tables Watchpoint and Actionpoint Results Tables LC Housekeeping Data

While the definition tables only need to be updated in the CDS when new ones are loaded, it is not practical from a performance standpoint to update the CDS every time the housekeeping data or results data changes. For this reason, results tables and housekeeping are only updated in the CDS if the application is shut down by cFE Executive Services and exits cleanly. When the application starts, it will check a "saved on exit" flag in the data restored from CDS to see if the data is good. If not, it will be reset to initialization values.

The detailed sequence for a CDS restore on application startup (with failure cases) is as follows:

1) Try to restore Watchpoint Definition Table from CDS If FAILS Load default Watchpoint Definition Table from filesystem Load default Actionpoint Definition Table from filesystem Clear Watchpoint Results Table to init values Clear Actionpoint Results Table to init values Clear Housekeeping variables to init values

2) If 1 succeeded, try to restore Actionpoint Definition Table from CDS If FAILS Load default Watchpoint Definition Table from filesystem Load default Actionpoint Definition Table from filesystem Clear Watchpoint Results Table to init values Clear Actionpoint Results Table to init values Clear Housekeeping variables to init values

3) If 1 and 2 succeeded, try to restore Watchpoint Results Table from CDS If FAILS Clear Watchpoint Results Table to init values Clear Actionpoint Results Table to init values Clear Housekeeping variables to init values

4) If 1 - 3 succeeded, try to restore Actionpoint Results Table from CDS If FAILS Clear Watchpoint Results Table to init values Clear Actionpoint Results Table to init values Clear Housekeeping variables to init values

5) If 1 - 4 succeeded, try to restore Application data (housekeeping variables) from CDS If FAILS Clear Watchpoint Results Table to init values Clear Actionpoint Results Table to init values Clear Housekeeping variables to init values

6) If 1 - 5 succeeded, check "save on exit" flag in application data restored in 5 IF not set to "SAVED" (meaning we updated on exit) Clear Watchpoint Results Table to init values Clear Actionpoint Results Table to init values Clear Housekeeping variables to init values 

Detailed FAQ documentation is available `here <../../doxy/apps/lc/cfslcfaqs.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/lc/index.html