let @a=""
g/^void/y A
g/^char/y A
g/^int/y A
g/^t_/y A
call feedkeys("\<CR>")
q
