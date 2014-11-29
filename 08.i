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
addI rlocal_vars_offset, 4 => r5
load r5 => r6 
loadI 2 => r7 
div r6, r7 => r8
addI rlocal_vars_offset, 0 => r9
store r8 => r9
jumpI -> Lend
Lend: 
