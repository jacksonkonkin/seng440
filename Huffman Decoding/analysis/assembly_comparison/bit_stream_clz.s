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
	ldrb	w9, [x0, #40]
	cbz	w9, LBB2_2
LBB2_1:
	ldr	x10, [x0, #32]
	lsr	x8, x10, #63
	lsl	x10, x10, #1
	str	x10, [x0, #32]
	sub	w9, w9, #1
	strb	w9, [x0, #40]
	mov	x0, x8
	ret
LBB2_2:
	ldp	x12, x8, [x0, #8]
	cmp	x8, x12
	csel	x11, x8, x12, hi
	subs	x9, x12, x8
	b.ls	LBB2_6
; %bb.3:
	ldr	x10, [x0]
	cmp	x9, #8
	b.lo	LBB2_7
; %bb.4:
	ldr	x9, [x10, x8]
	rev	x10, x9
	mov	w9, #64                         ; =0x40
	mov	w11, #8                         ; =0x8
LBB2_5:
	str	x10, [x0, #32]
	add	x8, x8, x11
	str	x8, [x0, #16]
	b	LBB2_1
LBB2_6:
	mov	w8, #0                          ; =0x0
	mov	x0, x8
	ret
LBB2_7:
	ldrb	w9, [x10, x8]
	ldr	x13, [x0, #32]
	orr	x13, x13, x9, lsl #56
	str	x13, [x0, #32]
	add	x14, x8, #1
	mov	w9, #8                          ; =0x8
	strb	w9, [x0, #40]
	str	x14, [x0, #16]
	cmp	x12, x14
	b.eq	LBB2_1
; %bb.8:
	ldrb	w9, [x10, x14]
	orr	x12, x13, x9, lsl #48
	str	x12, [x0, #32]
	add	x13, x8, #2
	mov	w9, #16                         ; =0x10
	strb	w9, [x0, #40]
	str	x13, [x0, #16]
	cmp	x13, x11
	b.eq	LBB2_1
; %bb.9:
	ldrb	w9, [x10, x13]
	orr	x12, x12, x9, lsl #40
	str	x12, [x0, #32]
	add	x13, x8, #3
	mov	w9, #24                         ; =0x18
	strb	w9, [x0, #40]
	str	x13, [x0, #16]
	cmp	x13, x11
	b.eq	LBB2_1
; %bb.10:
	ldrb	w9, [x10, x13]
	orr	x10, x12, x9, lsl #32
	mov	w9, #32                         ; =0x20
	mov	w11, #4                         ; =0x4
	b	LBB2_5
	.cfi_endproc
                                        ; -- End function
	.globl	_bit_stream_read_bits           ; -- Begin function bit_stream_read_bits
	.p2align	2
_bit_stream_read_bits:                  ; @bit_stream_read_bits
	.cfi_startproc
; %bb.0:
                                        ; kill: def $w1 killed $w1 def $x1
	sub	w8, w1, #33
	and	w8, w8, #0xff
	cmp	w8, #224
	b.hs	LBB3_2
; %bb.1:
	mov	w8, #0                          ; =0x0
	mov	x0, x8
	ret
LBB3_2:
	ldrb	w10, [x0, #40]
	subs	w9, w10, w1
	b.hs	LBB3_16
; %bb.3:
	mov	w9, #0                          ; =0x0
	mov	w8, #0                          ; =0x0
	b	LBB3_6
LBB3_4:                                 ;   in Loop: Header=BB3_6 Depth=1
	ldr	x11, [x0, #32]
LBB3_5:                                 ;   in Loop: Header=BB3_6 Depth=1
	lsl	x12, x11, #1
	str	x12, [x0, #32]
	sub	w10, w10, #1
	strb	w10, [x0, #40]
	lsr	x11, x11, #63
	orr	w8, w8, w11
	add	w9, w9, #1
	cmp	w1, w9, uxtb
	b.ls	LBB3_17
LBB3_6:                                 ; =>This Inner Loop Header: Depth=1
	lsl	w8, w8, #1
	tst	w10, #0xff
	b.ne	LBB3_4
; %bb.7:                                ;   in Loop: Header=BB3_6 Depth=1
	ldp	x15, x12, [x0, #8]
	cmp	x12, x15
	csel	x14, x12, x15, hi
	subs	x10, x15, x12
	b.ls	LBB3_11
; %bb.8:                                ;   in Loop: Header=BB3_6 Depth=1
	ldr	x13, [x0]
	cmp	x10, #8
	b.lo	LBB3_12
; %bb.9:                                ;   in Loop: Header=BB3_6 Depth=1
	ldr	x10, [x13, x12]
	rev	x11, x10
	mov	w10, #64                        ; =0x40
	mov	w13, #8                         ; =0x8
LBB3_10:                                ;   in Loop: Header=BB3_6 Depth=1
	add	x12, x13, x12
	str	x12, [x0, #16]
	b	LBB3_5
LBB3_11:                                ;   in Loop: Header=BB3_6 Depth=1
	mov	w10, #0                         ; =0x0
	add	w9, w9, #1
	cmp	w1, w9, uxtb
	b.hi	LBB3_6
	b	LBB3_17
LBB3_12:                                ;   in Loop: Header=BB3_6 Depth=1
	ldrb	w10, [x13, x12]
	ldr	x11, [x0, #32]
	orr	x11, x11, x10, lsl #56
	str	x11, [x0, #32]
	add	x16, x12, #1
	mov	w10, #8                         ; =0x8
	strb	w10, [x0, #40]
	str	x16, [x0, #16]
	cmp	x15, x16
	b.eq	LBB3_5
; %bb.13:                               ;   in Loop: Header=BB3_6 Depth=1
	ldrb	w10, [x13, x16]
	orr	x11, x11, x10, lsl #48
	str	x11, [x0, #32]
	add	x15, x12, #2
	mov	w10, #16                        ; =0x10
	strb	w10, [x0, #40]
	str	x15, [x0, #16]
	cmp	x15, x14
	b.eq	LBB3_5
; %bb.14:                               ;   in Loop: Header=BB3_6 Depth=1
	ldrb	w10, [x13, x15]
	orr	x11, x11, x10, lsl #40
	str	x11, [x0, #32]
	add	x15, x12, #3
	mov	w10, #24                        ; =0x18
	strb	w10, [x0, #40]
	str	x15, [x0, #16]
	cmp	x15, x14
	b.eq	LBB3_5
; %bb.15:                               ;   in Loop: Header=BB3_6 Depth=1
	ldrb	w10, [x13, x15]
	orr	x11, x11, x10, lsl #32
	mov	w10, #32                        ; =0x20
	mov	w13, #4                         ; =0x4
	b	LBB3_10
LBB3_16:
	mov	x8, #-1                         ; =0xffffffffffffffff
	lsl	x8, x8, x1
	mvn	x8, x8
	mov	w10, #64                        ; =0x40
	sub	w10, w10, w1
	lsl	x8, x8, x10
	ldr	x11, [x0, #32]
	and	x8, x11, x8
	lsr	x8, x8, x10
	lsl	x10, x11, x1
	str	x10, [x0, #32]
	strb	w9, [x0, #40]
LBB3_17:
	mov	x0, x8
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
