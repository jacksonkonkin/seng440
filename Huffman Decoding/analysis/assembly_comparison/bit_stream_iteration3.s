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
	.globl	_bit_stream_read_bits_batch     ; -- Begin function bit_stream_read_bits_batch
	.p2align	2
_bit_stream_read_bits_batch:            ; @bit_stream_read_bits_batch
	.cfi_startproc
; %bb.0:
	stp	x28, x27, [sp, #-96]!           ; 16-byte Folded Spill
	stp	x26, x25, [sp, #16]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #32]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #80]             ; 16-byte Folded Spill
	add	x29, sp, #80
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	mov	w23, #-1                        ; =0xffffffff
	cbz	x0, LBB4_44
; %bb.1:
	mov	x21, x1
	cbz	x1, LBB4_44
; %bb.2:
	mov	x20, x2
	cbz	x2, LBB4_44
; %bb.3:
	mov	x19, x3
	sub	w8, w3, #9
	cmn	w8, #8
	b.lo	LBB4_44
; %bb.4:
	mov	x22, x0
	ldrb	w16, [x0, #40]
	cmp	w16, #31
	b.hi	LBB4_32
; %bb.5:
	mov	w9, #71                         ; =0x47
	mov	w10, #56                        ; =0x38
	mov	w11, #63                        ; =0x3f
	ldp	x12, x8, [x22, #8]
	mov	w13, #48                        ; =0x30
	mov	w14, #55                        ; =0x37
	mov	w15, #40                        ; =0x28
	mov	w17, #32                        ; =0x20
	b	LBB4_9
LBB4_6:                                 ;   in Loop: Header=BB4_9 Depth=1
	mov	x8, x1
	mov	x0, x5
LBB4_7:                                 ;   in Loop: Header=BB4_9 Depth=1
	and	w16, w0, #0xff
	mov	x1, x8
LBB4_8:                                 ;   in Loop: Header=BB4_9 Depth=1
	mov	x8, x1
	cmp	w16, #32
	b.hs	LBB4_31
LBB4_9:                                 ; =>This Inner Loop Header: Depth=1
	cbnz	w16, LBB4_11
; %bb.10:                               ;   in Loop: Header=BB4_9 Depth=1
	cmp	x12, x8
	b.ls	LBB4_31
LBB4_11:                                ;   in Loop: Header=BB4_9 Depth=1
	mov	w0, w16
	cmp	x8, x12
	csel	x2, x8, x12, hi
	subs	x1, x12, x8
	b.ls	LBB4_7
; %bb.12:                               ;   in Loop: Header=BB4_9 Depth=1
	sub	x3, x9, x0
	lsr	x3, x3, #3
	cmp	x1, x3
	csel	x1, x1, x3, lo
	ldr	x3, [x22]
	cmp	x1, #8
	b.hs	LBB4_25
; %bb.13:                               ;   in Loop: Header=BB4_9 Depth=1
	ldr	x4, [x22, #32]
	cmp	x1, #4
	b.hs	LBB4_29
; %bb.14:                               ;   in Loop: Header=BB4_9 Depth=1
	ldrb	w1, [x3, x8]
	sub	x3, x10, x0
	lsl	x1, x1, x3
	orr	x1, x1, x4
	str	x1, [x22, #32]
	add	x5, x0, #8
	strb	w5, [x22, #40]
	add	x1, x8, #1
	str	x1, [x22, #16]
	cmp	w16, #23
	b.hi	LBB4_24
; %bb.15:                               ;   in Loop: Header=BB4_9 Depth=1
	cmp	x1, x2
	b.eq	LBB4_6
; %bb.16:                               ;   in Loop: Header=BB4_9 Depth=1
	ldr	x3, [x22]
	sub	x4, x11, x0
	lsr	x4, x4, #3
	sub	x6, x12, x1
	cmp	x6, x4
	csel	x6, x6, x4, lo
	ldr	x4, [x22, #32]
	cmp	x6, #3
	b.hi	LBB4_28
; %bb.17:                               ;   in Loop: Header=BB4_9 Depth=1
	ldrb	w1, [x3, x1]
	sub	x3, x13, x0
	lsl	x1, x1, x3
	orr	x1, x1, x4
	str	x1, [x22, #32]
	add	x5, x0, #16
	strb	w5, [x22, #40]
	add	x1, x8, #2
	str	x1, [x22, #16]
	cmp	w16, #15
	b.hi	LBB4_24
; %bb.18:                               ;   in Loop: Header=BB4_9 Depth=1
	cmp	x1, x2
	b.eq	LBB4_6
; %bb.19:                               ;   in Loop: Header=BB4_9 Depth=1
	ldr	x3, [x22]
	sub	x4, x14, x0
	lsr	x4, x4, #3
	sub	x6, x12, x1
	cmp	x6, x4
	csel	x6, x6, x4, lo
	ldr	x4, [x22, #32]
	cmp	x6, #3
	b.hi	LBB4_28
; %bb.20:                               ;   in Loop: Header=BB4_9 Depth=1
	ldrb	w1, [x3, x1]
	sub	x3, x15, x0
	lsl	x1, x1, x3
	orr	x1, x1, x4
	str	x1, [x22, #32]
	add	x5, x0, #24
	strb	w5, [x22, #40]
	add	x1, x8, #3
	str	x1, [x22, #16]
	cmp	w16, #7
	b.hi	LBB4_24
; %bb.21:                               ;   in Loop: Header=BB4_9 Depth=1
	cmp	x1, x2
	b.eq	LBB4_6
; %bb.22:                               ;   in Loop: Header=BB4_9 Depth=1
	ldr	x3, [x22]
	sub	x2, x12, x1
	ldr	x4, [x22, #32]
	cmp	x2, #3
	b.hi	LBB4_28
; %bb.23:                               ;   in Loop: Header=BB4_9 Depth=1
	ldrb	w1, [x3, x1]
	sub	x0, x17, x0
	lsl	x0, x1, x0
	orr	x0, x0, x4
	str	x0, [x22, #32]
	orr	w16, w16, #0x20
	strb	w16, [x22, #40]
	add	x1, x8, #4
	str	x1, [x22, #16]
	b	LBB4_8
LBB4_24:                                ;   in Loop: Header=BB4_9 Depth=1
	and	w16, w5, #0xff
	b	LBB4_8
LBB4_25:
	cbz	w16, LBB4_27
; %bb.26:
	ldr	x4, [x22, #32]
	b	LBB4_29
LBB4_27:
	ldr	x9, [x3, x8]
	rev	x9, x9
	str	x9, [x22, #32]
	mov	w9, #64                         ; =0x40
	mov	w10, #8                         ; =0x8
	b	LBB4_30
LBB4_28:
	mov	x8, x1
	mov	x0, x5
LBB4_29:
	ldr	w9, [x3, x8]
	rev	w9, w9
	mov	w10, #32                        ; =0x20
	sub	w10, w10, w0
	lsl	x9, x9, x10
	orr	x9, x9, x4
	str	x9, [x22, #32]
	orr	w9, w0, #0x20
	mov	w10, #4                         ; =0x4
LBB4_30:
	strb	w9, [x22, #40]
	add	x8, x8, x10
	str	x8, [x22, #16]
LBB4_31:
	cmp	w19, #1
	b.lt	LBB4_43
LBB4_32:
	mov	x23, #0                         ; =0x0
	mov	w24, w19
	mov	x25, #-1                        ; =0xffffffffffffffff
	b	LBB4_35
LBB4_33:                                ;   in Loop: Header=BB4_35 Depth=1
	str	wzr, [x20, x23, lsl #2]
LBB4_34:                                ;   in Loop: Header=BB4_35 Depth=1
	add	x23, x23, #1
	cmp	x23, x24
	b.eq	LBB4_42
LBB4_35:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB4_39 Depth 2
	ldrb	w8, [x22, #40]
	cbz	w8, LBB4_44
; %bb.36:                               ;   in Loop: Header=BB4_35 Depth=1
	ldrb	w26, [x21, x23]
	sub	w9, w26, #33
	and	w9, w9, #0xff
	cmp	w9, #223
	b.ls	LBB4_33
; %bb.37:                               ;   in Loop: Header=BB4_35 Depth=1
	cmp	w8, w26
	b.hs	LBB4_41
; %bb.38:                               ;   in Loop: Header=BB4_35 Depth=1
	mov	w28, #0                         ; =0x0
	mov	w27, #0                         ; =0x0
LBB4_39:                                ;   Parent Loop BB4_35 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	mov	x0, x22
	bl	_bit_stream_read_bit
	orr	w27, w0, w27, lsl #1
	add	w28, w28, #1
	cmp	w26, w28, uxtb
	b.hi	LBB4_39
; %bb.40:                               ;   in Loop: Header=BB4_35 Depth=1
	str	w27, [x20, x23, lsl #2]
	b	LBB4_34
LBB4_41:                                ;   in Loop: Header=BB4_35 Depth=1
	lsl	x9, x25, x26
	mvn	x9, x9
	neg	x10, x26
	lsl	x9, x9, x10
	ldr	x11, [x22, #32]
	and	x9, x11, x9
	lsr	x9, x9, x10
	str	w9, [x20, x23, lsl #2]
	ldrb	w9, [x21, x23]
	lsl	x9, x11, x9
	str	x9, [x22, #32]
	ldrb	w9, [x21, x23]
	sub	w8, w8, w9
	strb	w8, [x22, #40]
	b	LBB4_34
LBB4_42:
	mov	x23, x19
	b	LBB4_44
LBB4_43:
	mov	w23, #0                         ; =0x0
LBB4_44:
	mov	x0, x23
	ldp	x29, x30, [sp, #80]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #64]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #16]             ; 16-byte Folded Reload
	ldp	x28, x27, [sp], #96             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_bit_stream_has_data            ; -- Begin function bit_stream_has_data
	.p2align	2
_bit_stream_has_data:                   ; @bit_stream_has_data
	.cfi_startproc
; %bb.0:
	ldrb	w8, [x0, #40]
	cbz	w8, LBB5_2
; %bb.1:
	mov	w0, #1                          ; =0x1
	ret
LBB5_2:
	ldp	x9, x8, [x0, #8]
	cmp	x8, x9
	cset	w0, lo
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
