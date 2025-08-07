	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ; -- Begin function huffman_decoder_create
lCPI0_0:
	.quad	0                               ; 0x0
	.quad	1024                            ; 0x400
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_huffman_decoder_create
	.p2align	2
_huffman_decoder_create:                ; @huffman_decoder_create
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
	mov	x19, x0
	mov	w0, #32                         ; =0x20
	bl	_malloc
	cbz	x0, LBB0_2
; %bb.1:
	str	x19, [x0]
	mov	x19, x0
	mov	w0, #1024                       ; =0x400
	bl	_malloc
	mov	x8, x0
	mov	x0, x19
	str	x8, [x19, #8]
Lloh0:
	adrp	x9, lCPI0_0@PAGE
Lloh1:
	ldr	q0, [x9, lCPI0_0@PAGEOFF]
	str	q0, [x19, #16]
	cbz	x8, LBB0_3
LBB0_2:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
LBB0_3:
	bl	_free
	mov	x0, #0                          ; =0x0
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.loh AdrpLdr	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_huffman_decoder_destroy        ; -- Begin function huffman_decoder_destroy
	.p2align	2
_huffman_decoder_destroy:               ; @huffman_decoder_destroy
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB1_4
; %bb.1:
	ldr	x8, [x0, #8]
	cbz	x8, LBB1_3
; %bb.2:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	mov	x0, x8
	bl	_free
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
LBB1_3:
	b	_free
LBB1_4:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_huffman_decode_symbol          ; -- Begin function huffman_decode_symbol
	.p2align	2
_huffman_decode_symbol:                 ; @huffman_decode_symbol
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB2_10
; %bb.1:
	stp	x24, x23, [sp, #-64]!           ; 16-byte Folded Spill
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	mov	x19, x2
	mov	x8, x0
	mov	w0, #-1                         ; =0xffffffff
	cbz	x2, LBB2_9
; %bb.2:
	mov	x20, x1
	cbz	x1, LBB2_9
; %bb.3:
	ldr	x21, [x8]
	cbz	x21, LBB2_9
; %bb.4:
	mov	w22, #8                         ; =0x8
	mov	w23, #16                        ; =0x10
LBB2_5:                                 ; =>This Inner Loop Header: Depth=1
	ldrb	w8, [x21, #1]
	tbnz	w8, #0, LBB2_11
; %bb.6:                                ;   in Loop: Header=BB2_5 Depth=1
	mov	x0, x20
	bl	_bit_stream_has_data
	cbz	w0, LBB2_12
; %bb.7:                                ;   in Loop: Header=BB2_5 Depth=1
	mov	x0, x20
	bl	_bit_stream_read_bit
	cmp	w0, #0
	csel	x8, x23, x22, ne
	ldr	x21, [x21, x8]
	cbnz	x21, LBB2_5
; %bb.8:
	mov	w0, #-1                         ; =0xffffffff
LBB2_9:
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
	ret
LBB2_10:
	mov	w0, #-1                         ; =0xffffffff
	ret
LBB2_11:
	mov	w0, #0                          ; =0x0
	ldrb	w8, [x21]
	strb	w8, [x19]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
	ret
LBB2_12:
	mov	w0, #-1                         ; =0xffffffff
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_huffman_decode                 ; -- Begin function huffman_decode
	.p2align	2
_huffman_decode:                        ; @huffman_decode
	.cfi_startproc
; %bb.0:
	stp	x26, x25, [sp, #-80]!           ; 16-byte Folded Spill
	stp	x24, x23, [sp, #16]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #48]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
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
	mov	x21, x0
	mov	w0, #-1                         ; =0xffffffff
	cbz	x21, LBB3_18
; %bb.1:
	mov	x22, x1
	cbz	x1, LBB3_18
; %bb.2:
	mov	x20, x2
	cbz	x2, LBB3_18
; %bb.3:
	mov	x19, x3
	cbz	x3, LBB3_18
; %bb.4:
	str	xzr, [x21, #16]
	mov	x0, x22
	bl	_bit_stream_has_data
	cbz	w0, LBB3_17
; %bb.5:
	mov	w24, #8                         ; =0x8
	mov	w25, #16                        ; =0x10
LBB3_6:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB3_9 Depth 2
	ldp	x9, x8, [x21, #16]
	sub	x10, x8, #8
	cmp	x9, x10
	b.hs	LBB3_14
; %bb.7:                                ;   in Loop: Header=BB3_6 Depth=1
	ldr	x8, [x21]
	cbz	x8, LBB3_17
LBB3_8:                                 ;   in Loop: Header=BB3_6 Depth=1
	ldr	x23, [x8]
	cbz	x23, LBB3_17
LBB3_9:                                 ;   Parent Loop BB3_6 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldrb	w8, [x23, #1]
	tbnz	w8, #0, LBB3_12
; %bb.10:                               ;   in Loop: Header=BB3_9 Depth=2
	mov	x0, x22
	bl	_bit_stream_has_data
	cbz	w0, LBB3_17
; %bb.11:                               ;   in Loop: Header=BB3_9 Depth=2
	mov	x0, x22
	bl	_bit_stream_read_bit
	cmp	w0, #0
	csel	x8, x25, x24, ne
	ldr	x23, [x23, x8]
	cbnz	x23, LBB3_9
	b	LBB3_17
LBB3_12:                                ;   in Loop: Header=BB3_6 Depth=1
	ldrb	w8, [x23]
	ldp	x9, x10, [x21, #8]
	add	x11, x10, #1
	str	x11, [x21, #16]
	strb	w8, [x9, x10]
	ldr	x8, [x21, #16]
	tst	x8, #0x3f
	b.eq	LBB3_16
; %bb.13:                               ;   in Loop: Header=BB3_6 Depth=1
	mov	x0, x22
	bl	_bit_stream_has_data
	tbnz	w0, #0, LBB3_6
	b	LBB3_17
LBB3_14:                                ;   in Loop: Header=BB3_6 Depth=1
	lsl	x23, x8, #1
	ldr	x0, [x21, #8]
	mov	x1, x23
	bl	_realloc
	cbz	x0, LBB3_19
; %bb.15:                               ;   in Loop: Header=BB3_6 Depth=1
	str	x0, [x21, #8]
	str	x23, [x21, #24]
	ldr	x8, [x21]
	cbnz	x8, LBB3_8
	b	LBB3_17
LBB3_16:                                ;   in Loop: Header=BB3_6 Depth=1
	ldr	x9, [x21, #8]
	add	x8, x9, x8
	prfm	pstl3keep, [x8, #64]
	mov	x0, x22
	bl	_bit_stream_has_data
	tbnz	w0, #0, LBB3_6
LBB3_17:
	mov	w0, #0                          ; =0x0
	ldp	x8, x9, [x21, #8]
	str	x8, [x20]
	str	x9, [x19]
LBB3_18:
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp], #80             ; 16-byte Folded Reload
	ret
LBB3_19:
	mov	w0, #-1                         ; =0xffffffff
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp], #80             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
