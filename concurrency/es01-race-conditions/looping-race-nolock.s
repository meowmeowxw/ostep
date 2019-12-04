# assumes %bx has loop count in it

.main
.top	
# critical section
mov 2000, %ax	# mov ax, [2000]
add $1, %ax		# inc ax
mov %ax, 2000	# mov [2000], ax

# see if we're still looping
sub  $1, %bx	# sub bx, 1
test $0, %bx	# test bx, 0
jgt .top		# jg .top

halt			# halt until interrupt occurs
