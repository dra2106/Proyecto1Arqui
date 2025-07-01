.section .text
.global check_collision_arm
.type check_collision_arm, %function

check_collision_arm:
    @ Función: bool check_collision_arm(int y1, int x1, int h1, int w1, int y2, int x2, int h2, int w2)
    @ Registros de entrada:
    @ r0 = y1 (a.getY())
    @ r1 = x1 (a.getX()) 
    @ r2 = h1 (a.getHeight())
    @ r3 = w1 (a.getWidth())
    @ Stack: y2, x2, h2, w2

    push    {r4-r11, lr}        @ Guardar registros

    @ Cargar parámetros desde el stack
    ldr     r4, [sp, #36]       @ r4 = y2 (b.getY())
    ldr     r5, [sp, #40]       @ r5 = x2 (b.getX())
    ldr     r6, [sp, #44]       @ r6 = h2 (b.getHeight())
    ldr     r7, [sp, #48]       @ r7 = w2 (b.getWidth())

    @ Calcular hitbox de A (centrada)
    @ ax1 = x1 - w1/2
    mov     r8, r3, asr #1      @ r8 = w1/2
    sub     r9, r1, r8          @ r9 = ax1

    @ ay1 = y1 - h1/2
    mov     r10, r2, asr #1     @ r10 = h1/2
    sub     r11, r0, r10        @ r11 = ay1

    @ ax2 = ax1 + w1 - 1
    add     r12, r9, r3         @ ax1 + w1
    sub     r12, r12, #1        @ r12 = ax2

    @ ay2 = ay1 + h1 - 1
    add     r0, r11, r2         @ ay1 + h1 (reutilizar r0)
    sub     r0, r0, #1          @ r0 = ay2

    @ Calcular hitbox de B (centrada)
    @ bx1 = x2 - w2/2
    mov     r8, r7, asr #1      @ r8 = w2/2
    sub     r1, r5, r8          @ r1 = bx1 (reutilizar r1)

    @ by1 = y2 - h2/2
    mov     r10, r6, asr #1     @ r10 = h2/2
    sub     r2, r4, r10         @ r2 = by1 (reutilizar r2)

    @ bx2 = bx1 + w2 - 1
    add     r3, r1, r7          @ bx1 + w2 (reutilizar r3)
    sub     r3, r3, #1          @ r3 = bx2

    @ by2 = by1 + h2 - 1
    add     r4, r2, r6          @ by1 + h2
    sub     r4, r4, #1          @ r4 = by2

    @ Verificar colisión
    @ if (ax2 < bx1) return false
    cmp     r12, r1             @ ax2 vs bx1
    blt     no_collision

    @ if (ax1 > bx2) return false
    cmp     r9, r3              @ ax1 vs bx2
    bgt     no_collision

    @ if (ay2 < by1) return false
    cmp     r0, r2              @ ay2 vs by1
    blt     no_collision

    @ if (ay1 > by2) return false
    cmp     r11, r4             @ ay1 vs by2
    bgt     no_collision

    @ Hay colisión
    mov     r0, #1
    pop     {r4-r11, pc}

no_collision:
    mov     r0, #0
    pop     {r4-r11, pc}

.size check_collision_arm, .-check_collision_arm
