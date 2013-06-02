<h1>MemoryUtility</h1>

MemoryUtility is a simple library that aims to ease the process of editing processes at runtime. 

Code modification blocks are organized into PaySegments, which are composed of Payloads.  Payloads are composed of asm instructions in byte form and can be built from the string representation of the byte arrays.  This makes it possible to just copy and paste your binary modifications from ollydbg into a useable payload. 

The PayManager is in charge of injecting the PaySegments into the target exectuable. 

Modifying a running process with MemoryUtility is as easy as building your payload and calling Inject() on PayManager. 