


                         JavaScript Example 2


This state machine "recognizes" the string 0*1*. Example 2 differs
from example 1 in that example 2 uses default transitions.


+ Building
----------

NOTE: Smc.jar must be built and installed.

Unix & Windows:
    $ make checkstring


+ Executing
-----------

Unix & Windows:

    $ nodejs checkstring.js <string>

Try several different strings, such as:

    $ nodejs checkstring.js ""
      -> acceptable
    $ nodejs checkstring.js 000
      -> acceptable
    $ nodejs checkstring.js 00011
      -> acceptable
    $ nodejs checkstring.js 111
      -> acceptable
    $ nodejs checkstring.js 000111100
      -> unacceptable
    $ nodejs checkstring.js 00011a1b10c0
      -> unacceptable

