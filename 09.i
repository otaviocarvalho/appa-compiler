loadI 0 => rsp 
jumpI -> Lmain
Lmain: 
load rsp => rarp 
loadI 2 => r0 
storeAI r0 => rarp, 2
loadI 3 => rsp 
load rsp => rlocal_vars_offset 
loadI 1 => r1 
addI rbss, 0 => r2
store r1 => r2
jumpI -> Lend
Lend: 
