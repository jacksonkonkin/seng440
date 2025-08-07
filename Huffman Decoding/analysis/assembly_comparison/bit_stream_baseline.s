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
	ldrb	w11, [x0, #40]
	cbz	w11, LBB2_2
LBB2_1:
	ldr	x9, [x0, #32]
	lsr	x8, x9, #63
	lsl	x9, x9, #1
	str	x9, [x0, #32]
	sub	w9, w11, #1
	strb	w9, [x0, #40]
	mov	x0, x8
	ret
LBB2_2:
	ldp	x12, x8, [x0, #8]
	cmp	x8, x12
	csel	x10, x8, x12, hi
	b.hs	LBB2_7
; %bb.3:
	ldr	x9, [x0]
	ldrb	w11, [x9, x8]
	ldr	x13, [x0, #32]
	orr	x13, x13, x11, lsl #56
	str	x13, [x0, #32]
	mov	w11, #8                         ; =0x8
	strb	w11, [x0, #40]
	add	x14, x8, #1
	str	x14, [x0, #16]
	cmp	x12, x14
	b.eq	LBB2_1
; %bb.4:
	ldrb	w11, [x9, x14]
	orr	x12, x13, x11, lsl #48
	str	x12, [x0, #32]
	mov	w11, #16                        ; =0x10
	strb	w11, [x0, #40]
	add	x13, x8, #2
	str	x13, [x0, #16]
	cmp	x13, x10
	b.eq	LBB2_1
; %bb.5:
	ldrb	w11, [x9, x13]
	orr	x12, x12, x11, lsl #40
	str	x12, [x0, #32]
	mov	w11, #24                        ; =0x18
	strb	w11, [x0, #40]
	add	x13, x8, #3
	str	x13, [x0, #16]
	cmp	x13, x10
	b.eq	LBB2_1
; %bb.6:
	ldrb	w9, [x9, x13]
	orr	x9, x12, x9, lsl #32
	str	x9, [x0, #32]
	add	x8, x8, #4
	str	x8, [x0, #16]
	mov	w11, #32                        ; =0x20
	b	LBB2_1
LBB2_7:
	mov	w8, #0                          ; =0x0
	mov	x0, x8
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_bit_stream_read_bits           ; -- Begin function bit_stream_read_bits
	.p2align	2
_bit_stream_read_bits:                  ; @bit_stream_read_bits
	.cfi_startproc
; %bb.0:
	sub	w8, w1, #1
	cmp	w8, #31
	b.hi	LBB3_12
; %bb.1:
	mov	w9, #0                          ; =0x0
	mov	w8, #0                          ; =0x0
	ldrb	w10, [x0, #40]
	b	LBB3_4
LBB3_2:                                 ;   in Loop: Header=BB3_4 Depth=1
	ldr	x11, [x0, #32]
LBB3_3:                                 ;   in Loop: Header=BB3_4 Depth=1
	lsl	x12, x11, #1
	str	x12, [x0, #32]
	sub	w10, w10, #1
	strb	w10, [x0, #40]
	lsr	x11, x11, #63
	orr	w8, w8, w11
	add	w9, w9, #1
	cmp	w9, w1
	b.hs	LBB3_11
LBB3_4:                                 ; =>This Inner Loop Header: Depth=1
	lsl	w8, w8, #1
	tst	w10, #0xff
	b.ne	LBB3_2
; %bb.5:                                ;   in Loop: Header=BB3_4 Depth=1
	ldp	x15, x12, [x0, #8]
	cmp	x12, x15
	csel	x14, x12, x15, hi
	b.hs	LBB3_10
; %bb.6:                                ;   in Loop: Header=BB3_4 Depth=1
	ldr	x13, [x0]
	ldrb	w10, [x13, x12]
	ldr	x11, [x0, #32]
	orr	x11, x11, x10, lsl #56
	str	x11, [x0, #32]
	mov	w10, #8                         ; =0x8
	strb	w10, [x0, #40]
	add	x16, x12, #1
	str	x16, [x0, #16]
	cmp	x15, x16
	b.eq	LBB3_3
; %bb.7:                                ;   in Loop: Header=BB3_4 Depth=1
	ldrb	w10, [x13, x16]
	orr	x11, x11, x10, lsl #48
	str	x11, [x0, #32]
	mov	w10, #16                        ; =0x10
	strb	w10, [x0, #40]
	add	x15, x12, #2
	str	x15, [x0, #16]
	cmp	x15, x14
	b.eq	LBB3_3
; %bb.8:                                ;   in Loop: Header=BB3_4 Depth=1
	ldrb	w10, [x13, x15]
	orr	x11, x11, x10, lsl #40
	str	x11, [x0, #32]
	mov	w10, #24                        ; =0x18
	strb	w10, [x0, #40]
	add	x15, x12, #3
	str	x15, [x0, #16]
	cmp	x15, x14
	b.eq	LBB3_3
; %bb.9:                                ;   in Loop: Header=BB3_4 Depth=1
	ldrb	w10, [x13, x15]
	orr	x11, x11, x10, lsl #32
	add	x10, x12, #4
	str	x10, [x0, #16]
	mov	w10, #32                        ; =0x20
	b	LBB3_3
LBB3_10:                                ;   in Loop: Header=BB3_4 Depth=1
	mov	w10, #0                         ; =0x0
	add	w9, w9, #1
	cmp	w9, w1
	b.lo	LBB3_4
LBB3_11:
	mov	x0, x8
	ret
LBB3_12:
	mov	w8, #0                          ; =0x0
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
