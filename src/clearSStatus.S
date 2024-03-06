.global clear_sstatus
.type clear_sstatus, @function
clear_sstatus:
     csrr t0, sstatus

     # Clear bit 5 (SPIE) and bit 8 (SPP) in t0

     # Create a bitmask with bit 5 set (1<<5) and shift the bitmask to position 5
     li t1, 1
     slli t1, t1, 5

     # Create a bitmask with bit 8 set (1<<8) and shift the bitmask to position 8
     li t2, 1
     slli t2, t2, 8

     or t1, t1, t2

     not t1, t1          # Invert the combined bitmask to clear both bit 5 and bit 8
     and t0, t0, t1      # Clear bit 5 and bit 8 in t0

     csrw sstatus, t0

     csrw sepc, ra

    sret