Calling Convention (C Language)
===
## Caller rules

![image] (http://www.cs.virginia.edu/~evans/cs216/guides/stack-convention.png "stack convention")

Before make a subrouting call:

  - Save the contents of certain registers are designed *caller-saved*. (i.e. EAX, ECX, 	EDX) the caller must push the values in these registers onto the stack, so they can be restore after the subroutine returns.
  - Push parameters to on the stack as parameter passing (last parameter first)
  - Use the ``call`` instruction to call the subroutine, this instruction place the return address on top of the parameters on the stack

After the subrutine returns:

  - Remove the parameter from stack.
  - Restroe the contents of caller-saved registers(``EAX, ECX, EDX``)

### Example 1 (caller rules):

```
push [var] ; Push last parameter first
push 216   ; Push the second parameter
push eax   ; Push first parameter last

call _myFunc ; Call the function (assume C naming)

add esp, 12 ; cleans up the stack
```

## Callee rules

The definition of the subroutine should adhere to the following rules at the beginning of the subroutine:

  - Push old EPB onto the stack, copy the value of ESP into EBP using the following Insturcture

``` 
push ebp
mov ebp, esp
```

  Here the ebp is equal the the initial value of esp when the subroutine is starting executing. Remember the caller is not expecting the subroutine to change the value of the base pointer. EBP is used to obtain our point reference for accessing parameters and local variables.

  - Next allocate local variables.
  - Save the values of callee-saved registers that will be used by the function must be saved(push them on stack, i.e. ``EBX, EDI``, and ``ESI``).  
  
After these three actions are performed, the body of the subroutine will proceed. When the subroutine is returns, it must follow these steps:

  - Leave the return value in EAX
  - Restore the old value of ``EDI`` and ``ESI`` that were modified.
  - Deallocate local variables (``mov esp, ebp``) *why?*
  - Immediately before returning, restore the caller's base pointer value by popping EBP off the stack.
  - Return to the caller by executing a ``ret`` instruction. This instruction will will find and remove the appropriate return address from the stack.

--------------------------------------------
###Example 2 (callee rules):

```
.486
.MODEL FLAT
.CODE
PUBLIC _myFunc
_myFunc PROC
  ; Subroutine Prologue
  push ebp     ; Save the old base pointer value.
  mov ebp, esp ; Set the new base pointer value.
  sub esp, 4   ; Make room for one 4-byte local variable.
  push edi     ; Save the values of registers that the function
  push esi     ; will modify. This function uses EDI and ESI.
  ; (no need to save EBX, EBP, or ESP)

  ; Subroutine Body
  mov eax, [ebp+8]   ; Move value of parameter 1 into EAX
  mov esi, [ebp+12]  ; Move value of parameter 2 into ESI
  mov edi, [ebp+16]  ; Move value of parameter 3 into EDI

  mov [ebp-4], edi   ; Move EDI into the local variable
  add [ebp-4], esi   ; Add ESI into the local variable
  add eax, [ebp-4]   ; Add the contents of the local variable
                     ; into EAX (final result)

  ; Subroutine Epilogue 
  pop esi      ; Recover register values
  pop  edi
  mov esp, ebp ; Deallocate local variables
  pop ebp ; Restore the caller's base pointer value
  ret
_myFunc ENDP
END
```

##References:
[x86 Assembly Guide](http://www.cs.virginia.edu/~evans/cs216/guides/x86.html)

