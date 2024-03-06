.global context_switch
.type context_switch, @function
context_switch:
    #a0 -> old_context
    #a1 -> new_context

    #save thread context
    sd ra, 0 * 8(a0)
    sd sp, 1 * 8(a0)

    #restore thread context
    ld ra, 0 * 8(a1)
    ld sp, 1 * 8(a1)

    ret