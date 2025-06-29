.text
.global check_collision_arm

check_collision_arm:
    push {r4-r11, lr}   @ Guarda registros para cálculos

    @ Parámetros:
    @ r0 = a.getX(), r1 = a.getY(), r2 = a.getWidth(), r3 = a.getHeight()
    @ Stack: b.getX(), b.getY(), b.getWidth(), b.getHeight()

    @ Carga parámetros de b desde el stack
    ldr r4, [sp, #36]   @ b.getX()
    ldr r5, [sp, #40]   @ b.getY()
    ldr r6, [sp, #44]   @ b.getWidth()
    ldr r7, [sp, #48]   @ b.getHeight()

    @ Calcula hitbox de A (centrada)
    @ ax1 = a.getX() - a.getWidth()/2
    mov r8, r2, asr #1  @ r8 = width/2 (división por 2)
    sub r9, r0, r8      @ r9 = ax1 (a.getX() - width/2)
    
    @ ay1 = a.getY() - a.getHeight()/2
    mov r10, r3, asr #1 @ r10 = height/2
    sub r11, r1, r10    @ r11 = ay1
    
    @ ax2 = ax1 + width - 1
    add r12, r9, r2     @ ax1 + width
    sub r12, r12, #1    @ ax2 = ax1 + width - 1
    
    @ ay2 = ay1 + height - 1
    add lr, r11, r3     @ ay1 + height
    sub lr, lr, #1      @ ay2 = ay1 + height - 1

    @ Calcula hitbox de B
    @ bx1 = b.getX() - b.getWidth()/2
    mov r8, r6, asr #1  @ width/2
    sub r0, r4, r8      @ r0 = bx1
    
    @ by1 = b.getY() - b.getHeight()/2
    mov r10, r7, asr #1 @ height/2
    sub r1, r5, r10     @ r1 = by1
    
    @ bx2 = bx1 + width - 1
    add r2, r0, r6      @ bx1 + width
    sub r2, r2, #1      @ bx2
    
    @ by2 = by1 + height - 1
    add r3, r1, r7      @ by1 + height
    sub r3, r3, #1      @ by2
    
    cmp r12, r0         @ ax2 < bx1?
    blt no_collision
    
    cmp r9, r2          @ ax1 > bx2?
    bgt no_collision
    
    cmp lr, r1          @ ay2 < by1?
    blt no_collision
    
    cmp r11, r3         @ ay1 > by2?
    bgt no_collision

    @ Hay colisión
    mov r0, #1
    pop {r4-r11, pc}

no_collision:
    mov r0, #0
    pop {r4-r11, pc}
    