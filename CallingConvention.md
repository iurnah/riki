Calling Convention (C Language calling convention)
===

1. Based heavily on the use of hardware-supported stack. ``push, pop, call, `` and ``ret`` instructions. 
2. Caller convention
------------------------
Before make a subrouting call:

	- Save the contents of certain registers are designed *caller-saved*. (i.e. EAX, ECX, 	EDX) the caller must push the values in these registers onto the stack, so they can be restore after the subroutine returns.
  	- Push parameters to on the stack as parameter passing (last parameter first)
	- Use the ``call`` instruction to call the subroutine, this instruction place the return address on top of the parameters on the stack
------------------------------
After the subrutine returns:

	- Remove the parameter from stack.
	- Restroe the contents of caller-saved registers(``EAX, ECX, EDX``)
-------------------------------
Example;

```
push [var] ; Push last parameter first
push 216   ; Push the second parameter
push eax   ; Push first parameter last

call _myFunc ; Call the function (assume C naming)

add esp, 12 ; cleans up the stack
```
3. Callee convention


![image] (http://www.cs.virginia.edu/~evans/cs216/guides/stack-convention.png "stack convention")