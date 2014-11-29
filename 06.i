loadI 0 => rsp 
jumpI -> Lmain
Lmain: 
load rsp => rarp 
loadI 1 => r0 
storeAI r0 => rarp, 2
loadI 3 => rsp 
load rsp => rlocal_vars_offset 
loadI 1 => r1 
addI rlocal_vars_offset, 0 => r2
store r1 => r2
loadI 2 => r3 
addI rlocal_vars_offset, 4 => r4
store r3 => r4
loadI 2 => r5 
loadI 2 => r6 
sub r5, r6 => r7
addI rlocal_vars_offset, 0 => r8
store r7 => r8
jumpI -> Lend
Lend: 
