


                         Lua Example 3


This state machine "recognizes" the palindromes (words that read the
same backwards as forwards). The words consist of the alphabet
{0, 1, c} where the letter 'c' may appear only once and marks the
words center.


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
      -> unacceptable
    $ nodejs checkstring.js 00
      -> unacceptable
    $ nodejs checkstring.js 1c
      -> unacceptable
    $ nodejs checkstring.js c0
      -> unacceptable
    $ nodejs checkstring.js abcba
      -> unacceptable
    $ nodejs checkstring.js 110010c010011
      -> acceptable
    $ nodejs checkstring.js 110010c110010
      -> unacceptable
