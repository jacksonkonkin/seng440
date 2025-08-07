	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.globl	_bit_stream_create              ; -- Begin function bit_stream_create
	.p2align	2
_bit_stream_create:                     ; @bit_stream_create
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x1
	mov	x20, x0
	mov	w0, #48                         ; =0x30
	bl	_malloc
	cbz	x0, LBB0_2
; %bb.1:
	stp	x20, x19, [x0]
	str	xzr, [x0, #16]
	strb	wzr, [x0, #24]
	str	xzr, [x0, #32]
	strb	wzr, [x0, #40]
LBB0_2:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_bit_stream_destroy             ; -- Begin function bit_stream_destroy
	.p2align	2
_bit_stream_destroy:                    ; @bit_stream_destroy
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB1_2
; %bb.1:
	b	_free
LBB1_2:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_bit_stream_read_bit            ; -- Begin function bit_stream_read_bit
	.p2align	2
_bit_stream_read_bit:                   ; @bit_stream_read_bit
	.cfi_startproc
; %bb.0:
	ldrb	w12, [x0, #40]
	cbnz	w12, LBB2_9
; %bb.1:
	ldp	x11, x8, [x0, #8]
	cmp	x8, x11
	csel	x13, x8, x11, hi
	subs	x12, x11, x8
	b.ls	LBB2_4
; %bb.2:
	ldr	x10, [x0]
	cmp	x12, #8
	b.lo	LBB2_5
; %bb.3:
	ldr	x9, [x10, x8]
	rev	x9, x9
	str	x9, [x0, #32]
	add	x8, x8, #8
	mov	w12, #64                        ; =0x40
	b	LBB2_8
LBB2_4:
	mov	w8, #0                          ; =0x0
	mov	x0, x8
	ret
LBB2_5:
	ldr	x9, [x0, #32]
	cmp	x12, #4
	b.lo	LBB2_10
; %bb.6:
	mov	w11, #0                         ; =0x0
LBB2_7:
	ldr	w10, [x10, x8]
	rev	w10, w10
	mov	w12, #32                        ; =0x20
	sub	w12, w12, w11
	lsl	x10, x10, x12
	orr	x9, x10, x9
	str	x9, [x0, #32]
	orr	w12, w11, #0x20
	add	x8, x8, #4
LBB2_8:
	str	x8, [x0, #16]
LBB2_9:
	ldr	x9, [x0, #32]
	lsr	x8, x9, #63
	lsl	x9, x9, #1
	str	x9, [x0, #32]
	sub	w9, w12, #1
	strb	w9, [x0, #40]
	mov	x0, x8
	ret
LBB2_10:
	ldrb	w12, [x10, x8]
	orr	x9, x9, x12, lsl #56
	str	x9, [x0, #32]
	mov	w12, #8                         ; =0x8
	strb	w12, [x0, #40]
	add	x14, x8, #1
	str	x14, [x0, #16]
	subs	x15, x11, x14
	b.eq	LBB2_9
; %bb.11:
	cmp	x15, #3
	b.ls	LBB2_13
; %bb.12:
	mov	w11, #8                         ; =0x8
	mov	x8, x14
	b	LBB2_7
LBB2_13:
	ldrb	w12, [x10, x14]
	orr	x9, x9, x12, lsl #48
	str	x9, [x0, #32]
	mov	w12, #16                        ; =0x10
	strb	w12, [x0, #40]
	add	x14, x8, #2
	str	x14, [x0, #16]
	cmp	x14, x13
	b.eq	LBB2_9
; %bb.14:
	sub	x12, x11, x14
	cmp	x12, #3
	b.ls	LBB2_16
; %bb.15:
	mov	w11, #16                        ; =0x10
	mov	x8, x14
	b	LBB2_7
LBB2_16:
	ldrb	w12, [x10, x14]
	orr	x9, x9, x12, lsl #40
	str	x9, [x0, #32]
	mov	w12, #24                        ; =0x18
	strb	w12, [x0, #40]
	add	x14, x8, #3
	str	x14, [x0, #16]
	cmp	x14, x13
	b.eq	LBB2_9
; %bb.17:
	sub	x11, x11, x14
	cmp	x11, #3
	b.ls	LBB2_19
; %bb.18:
	mov	w11, #24                        ; =0x18
	mov	x8, x14
	b	LBB2_7
LBB2_19:
	ldrb	w10, [x10, x14]
	orr	x9, x9, x10, lsl #32
	str	x9, [x0, #32]
	add	x8, x8, #4
	mov	w12, #32                        ; =0x20
	b	LBB2_8
	.cfi_endproc
                                        ; -- End function
	.globl	_bit_stream_read_bits           ; -- Begin function bit_stream_read_bits
	.p2align	2
_bit_stream_read_bits:                  ; @bit_stream_read_bits
	.cfi_startproc
; %bb.0:
	stp	x22, x21, [sp, #-48]!           ; 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	sub	w8, w1, #33
	and	w8, w8, #0xff
	cmp	w8, #224
	b.hs	LBB3_3
; %bb.1:
	mov	w21, #0                         ; =0x0
LBB3_2:
	mov	x0, x21
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp], #48             ; 16-byte Folded Reload
	ret
LBB3_3:
	mov	x20, x1
	mov	x19, x0
	ldrb	w8, [x0, #40]
	subs	w8, w8, w1
	b.hs	LBB3_6
; %bb.4:
	mov	w22, #0                         ; =0x0
	mov	w21, #0                         ; =0x0
LBB3_5:                                 ; =>This Inner Loop Header: Depth=1
	mov	x0, x19
	bl	_bit_stream_read_bit
	orr	w21, w0, w21, lsl #1
	add	w22, w22, #1
	cmp	w20, w22, uxtb
	b.hi	LBB3_5
	b	LBB3_2
LBB3_6:
	mov	x9, #-1                         ; =0xffffffffffffffff
	lsl	x9, x9, x20
	mvn	x9, x9
	mov	w10, #64                        ; =0x40
	sub	w10, w10, w20
	lsl	x9, x9, x10
	ldr	x11, [x19, #32]
	and	x9, x11, x9
	lsr	x21, x9, x10
	lsl	x9, x11, x20
	str	x9, [x19, #32]
	strb	w8, [x19, #40]
	mov	x0, x21
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp], #48             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_bit_stream_has_data            ; -- Begin function bit_stream_has_data
	.p2align	2
_bit_stream_has_data:                   ; @bit_stream_has_data
	.cfi_startproc
; %bb.0:
	ldrb	w8, [x0, #40]
	cbz	w8, LBB4_2
; %bb.1:
	mov	w0, #1                          ; =0x1
	ret
LBB4_2:
	ldp	x9, x8, [x0, #8]
	cmp	x8, x9
	cset	w0, lo
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
