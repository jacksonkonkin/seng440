	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 5
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ; -- Begin function huffman_decoder_create
lCPI0_0:
	.quad	0                               ; 0x0
	.quad	1024                            ; 0x400
lCPI0_1:
	.quad	4096                            ; 0x1000
	.quad	0                               ; 0x0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_huffman_decoder_create
	.p2align	2
_huffman_decoder_create:                ; @huffman_decoder_create
	.cfi_startproc
; %bb.0:
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
	mov	x21, x0
	mov	w0, #40                         ; =0x28
	bl	_malloc
	mov	x19, x0
	cbz	x0, LBB0_8
; %bb.1:
	str	x21, [x19]
	mov	w0, #1024                       ; =0x400
	bl	_malloc
	mov	x20, x0
	str	x0, [x19, #8]
Lloh0:
	adrp	x8, lCPI0_0@PAGE
Lloh1:
	ldr	q0, [x8, lCPI0_0@PAGEOFF]
	str	q0, [x19, #16]
	cbz	x21, LBB0_7
; %bb.2:
	ldr	x22, [x21]
	cbz	x22, LBB0_6
; %bb.3:
	mov	w0, #64                         ; =0x40
	bl	_malloc
	mov	x21, x0
	cbz	x0, LBB0_7
; %bb.4:
Lloh2:
	adrp	x8, lCPI0_1@PAGE
Lloh3:
	ldr	q0, [x8, lCPI0_1@PAGEOFF]
	str	q0, [x21, #16]
	mov	w8, #3088                       ; =0xc10
	strh	w8, [x21, #32]
	mov	w0, #64                         ; =0x40
	mov	w1, #262144                     ; =0x40000
	bl	_aligned_alloc
	str	x0, [x21]
	cbz	x0, LBB0_9
; %bb.5:
	mov	x23, x0
	mov	w1, #262144                     ; =0x40000
	bl	_bzero
	mov	x0, x22
	mov	w1, #0                          ; =0x0
	mov	w2, #0                          ; =0x0
	mov	x3, x23
	mov	w4, #12                         ; =0xc
	bl	_build_lookup_table_recursive
	str	xzr, [x21, #8]
	str	x21, [x19, #32]
	cbnz	x20, LBB0_8
	b	LBB0_10
LBB0_6:
	mov	x21, #0                         ; =0x0
LBB0_7:
	str	x21, [x19, #32]
	cbz	x20, LBB0_10
LBB0_8:
	mov	x0, x19
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
	ret
LBB0_9:
	mov	x0, x21
	bl	_free
	mov	x21, #0                         ; =0x0
	str	x21, [x19, #32]
	cbnz	x20, LBB0_8
LBB0_10:
	cbz	x21, LBB0_16
; %bb.11:
	ldr	x0, [x21]
	cbz	x0, LBB0_13
; %bb.12:
	bl	_free
LBB0_13:
	ldr	x0, [x21, #8]
	cbz	x0, LBB0_15
; %bb.14:
	bl	_free
LBB0_15:
	mov	x0, x21
	bl	_free
LBB0_16:
	mov	x0, x19
	bl	_free
	mov	x19, #0                         ; =0x0
	mov	x0, x19
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
	ret
	.loh AdrpLdr	Lloh0, Lloh1
	.loh AdrpLdr	Lloh2, Lloh3
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ; -- Begin function create_vectorized_lookup_table
lCPI1_0:
	.quad	4096                            ; 0x1000
	.quad	0                               ; 0x0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_create_vectorized_lookup_table
	.p2align	2
_create_vectorized_lookup_table:        ; @create_vectorized_lookup_table
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB1_8
; %bb.1:
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
	ldr	x19, [x0]
	cbz	x19, LBB1_6
; %bb.2:
	mov	w0, #64                         ; =0x40
	bl	_malloc
	cbz	x0, LBB1_7
; %bb.3:
Lloh4:
	adrp	x8, lCPI1_0@PAGE
Lloh5:
	ldr	q0, [x8, lCPI1_0@PAGEOFF]
	str	q0, [x0, #16]
	mov	w8, #3088                       ; =0xc10
	strh	w8, [x0, #32]
	mov	x21, x0
	mov	w0, #64                         ; =0x40
	mov	w1, #262144                     ; =0x40000
	bl	_aligned_alloc
	str	x0, [x21]
	cbz	x0, LBB1_5
; %bb.4:
	mov	x20, x0
	mov	w1, #262144                     ; =0x40000
	bl	_bzero
	mov	x0, x19
	mov	w1, #0                          ; =0x0
	mov	w2, #0                          ; =0x0
	mov	x3, x20
	mov	w4, #12                         ; =0xc
	bl	_build_lookup_table_recursive
	mov	x0, x21
	str	xzr, [x21, #8]
	b	LBB1_7
LBB1_5:
	mov	x0, x21
	bl	_free
LBB1_6:
	mov	x0, #0                          ; =0x0
LBB1_7:
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp], #48             ; 16-byte Folded Reload
LBB1_8:
	ret
	.loh AdrpLdr	Lloh4, Lloh5
	.cfi_endproc
                                        ; -- End function
	.globl	_destroy_vectorized_lookup_table ; -- Begin function destroy_vectorized_lookup_table
	.p2align	2
_destroy_vectorized_lookup_table:       ; @destroy_vectorized_lookup_table
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB2_6
; %bb.1:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	ldr	x0, [x0]
	cbz	x0, LBB2_3
; %bb.2:
	bl	_free
LBB2_3:
	ldr	x0, [x19, #8]
	cbz	x0, LBB2_5
; %bb.4:
	bl	_free
LBB2_5:
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	b	_free
LBB2_6:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_huffman_decoder_destroy        ; -- Begin function huffman_decoder_destroy
	.p2align	2
_huffman_decoder_destroy:               ; @huffman_decoder_destroy
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB3_10
; %bb.1:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	ldr	x0, [x0, #8]
	cbz	x0, LBB3_3
; %bb.2:
	bl	_free
LBB3_3:
	ldr	x20, [x19, #32]
	cbz	x20, LBB3_9
; %bb.4:
	ldr	x0, [x20]
	cbz	x0, LBB3_6
; %bb.5:
	bl	_free
LBB3_6:
	ldr	x0, [x20, #8]
	cbz	x0, LBB3_8
; %bb.7:
	bl	_free
LBB3_8:
	mov	x0, x20
	bl	_free
LBB3_9:
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	b	_free
LBB3_10:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_huffman_decode_symbol          ; -- Begin function huffman_decode_symbol
	.p2align	2
_huffman_decode_symbol:                 ; @huffman_decode_symbol
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB4_10
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
	cbz	x2, LBB4_9
; %bb.2:
	mov	x20, x1
	cbz	x1, LBB4_9
; %bb.3:
	ldr	x21, [x8]
	cbz	x21, LBB4_9
; %bb.4:
	mov	w22, #8                         ; =0x8
	mov	w23, #16                        ; =0x10
LBB4_5:                                 ; =>This Inner Loop Header: Depth=1
	ldrb	w8, [x21, #1]
	tbnz	w8, #0, LBB4_11
; %bb.6:                                ;   in Loop: Header=BB4_5 Depth=1
	mov	x0, x20
	bl	_bit_stream_has_data
	cbz	w0, LBB4_12
; %bb.7:                                ;   in Loop: Header=BB4_5 Depth=1
	mov	x0, x20
	bl	_bit_stream_read_bit
	cmp	w0, #0
	csel	x8, x23, x22, ne
	ldr	x21, [x21, x8]
	cbnz	x21, LBB4_5
; %bb.8:
	mov	w0, #-1                         ; =0xffffffff
LBB4_9:
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
	ret
LBB4_10:
	mov	w0, #-1                         ; =0xffffffff
	ret
LBB4_11:
	mov	w0, #0                          ; =0x0
	ldrb	w8, [x21]
	strb	w8, [x19]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
	ret
LBB4_12:
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
	sub	sp, sp, #192
	stp	x28, x27, [sp, #96]             ; 16-byte Folded Spill
	stp	x26, x25, [sp, #112]            ; 16-byte Folded Spill
	stp	x24, x23, [sp, #128]            ; 16-byte Folded Spill
	stp	x22, x21, [sp, #144]            ; 16-byte Folded Spill
	stp	x20, x19, [sp, #160]            ; 16-byte Folded Spill
	stp	x29, x30, [sp, #176]            ; 16-byte Folded Spill
	add	x29, sp, #176
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
	mov	x21, x0
Lloh6:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh7:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh8:
	ldr	x8, [x8]
	str	x8, [sp, #88]
	mov	w0, #-1                         ; =0xffffffff
	cbz	x21, LBB5_45
; %bb.1:
	mov	x22, x1
	cbz	x1, LBB5_45
; %bb.2:
	cbz	x2, LBB5_45
; %bb.3:
	cbz	x3, LBB5_45
; %bb.4:
	stp	x2, x3, [sp]                    ; 16-byte Folded Spill
	str	xzr, [x21, #16]
	ldr	x19, [x21, #32]
	mov	x0, x22
	bl	_bit_stream_has_data
	cbz	x19, LBB5_31
; %bb.5:
	cbz	w0, LBB5_44
; %bb.6:
	add	x24, sp, #16
	add	x25, x24, #64
	mov	w26, #32                        ; =0x20
	mov	w27, #8                         ; =0x8
	add	x28, sp, #48
	b	LBB5_10
LBB5_7:                                 ;   in Loop: Header=BB5_10 Depth=1
	cmp	w19, #0
	b.le	LBB5_24
LBB5_8:                                 ;   in Loop: Header=BB5_10 Depth=1
	ldp	x8, x9, [x21, #8]
	mov	w23, w19
	add	x0, x8, x9
	add	x1, sp, #16
	mov	x2, x23
	bl	_memcpy
	ldr	x8, [x21, #16]
	add	x8, x8, x23
	str	x8, [x21, #16]
	tst	x8, #0x7f
	b.eq	LBB5_23
LBB5_9:                                 ;   in Loop: Header=BB5_10 Depth=1
	mov	x0, x22
	bl	_bit_stream_has_data
	tbz	w0, #0, LBB5_44
LBB5_10:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB5_14 Depth 2
                                        ;       Child Loop BB5_18 Depth 3
	ldp	x9, x8, [x21, #16]
	sub	x10, x8, #64
	cmp	x9, x10
	b.hs	LBB5_29
; %bb.11:                               ;   in Loop: Header=BB5_10 Depth=1
	ldr	x20, [x21, #32]
	cbz	x20, LBB5_44
LBB5_12:                                ;   in Loop: Header=BB5_10 Depth=1
	mov	w19, #0                         ; =0x0
	b	LBB5_14
LBB5_13:                                ;   in Loop: Header=BB5_14 Depth=2
	cmp	w19, #32
	b.ge	LBB5_8
LBB5_14:                                ;   Parent Loop BB5_10 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB5_18 Depth 3
	mov	x0, x22
	bl	_bit_stream_has_data
	cbz	w0, LBB5_7
; %bb.15:                               ;   in Loop: Header=BB5_14 Depth=2
	sub	w8, w26, w19
	cmp	w8, #8
	csel	w23, w8, w27, lo
	ldrb	w1, [x20, #33]
	add	x0, sp, #80
	mov	x2, x23
	bl	_memset
	add	x1, sp, #80
	add	x2, sp, #48
	mov	x0, x22
	mov	x3, x23
	bl	_bit_stream_read_bits_batch
	cmp	w0, #1
	b.lt	LBB5_7
; %bb.16:                               ;   in Loop: Header=BB5_14 Depth=2
	mov	x8, #0                          ; =0x0
	mov	w9, w0
	ldr	x10, [x20, #16]
	add	x11, x24, w19, uxtw
	b	LBB5_18
LBB5_17:                                ;   in Loop: Header=BB5_18 Depth=3
	add	x8, x8, #1
	cmp	x9, x8
	b.eq	LBB5_21
LBB5_18:                                ;   Parent Loop BB5_10 Depth=1
                                        ;     Parent Loop BB5_14 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	w12, [x28, x8, lsl #2]
	cmp	x10, x12
	b.ls	LBB5_17
; %bb.19:                               ;   in Loop: Header=BB5_18 Depth=3
	ldr	x13, [x20]
	add	x12, x13, x12, lsl #6
	ldrb	w13, [x12, #1]
	cbz	w13, LBB5_17
; %bb.20:                               ;   in Loop: Header=BB5_18 Depth=3
	ldrb	w12, [x12]
	strb	w12, [x11, x8]
	b	LBB5_17
LBB5_21:                                ;   in Loop: Header=BB5_14 Depth=2
	add	w19, w0, w19
	cmn	w19, #32
	b.ge	LBB5_13
; %bb.22:                               ;   in Loop: Header=BB5_14 Depth=2
	add	x8, x25, w19, sxtw
	prfm	pstl3keep, [x8]
	b	LBB5_13
LBB5_23:                                ;   in Loop: Header=BB5_10 Depth=1
	ldr	x9, [x21, #8]
	add	x8, x9, x8
	prfm	pstl3keep, [x8, #128]
	b	LBB5_9
LBB5_24:                                ;   in Loop: Header=BB5_10 Depth=1
	ldr	x19, [x21, #32]
	cbz	x19, LBB5_44
; %bb.25:                               ;   in Loop: Header=BB5_10 Depth=1
	ldr	x8, [x19]
	cbz	x8, LBB5_44
; %bb.26:                               ;   in Loop: Header=BB5_10 Depth=1
	ldrb	w1, [x19, #33]
	mov	x0, x22
	bl	_bit_stream_read_bits
	mov	w8, w0
	ldr	x9, [x19, #16]
	cmp	x9, x8
	b.ls	LBB5_44
; %bb.27:                               ;   in Loop: Header=BB5_10 Depth=1
	ldr	x9, [x19]
	add	x8, x9, x8, lsl #6
	ldrb	w9, [x8, #1]
	cbz	w9, LBB5_44
; %bb.28:                               ;   in Loop: Header=BB5_10 Depth=1
	ldrb	w8, [x8]
	ldp	x9, x10, [x21, #8]
	add	x11, x10, #1
	str	x11, [x21, #16]
	strb	w8, [x9, x10]
	b	LBB5_9
LBB5_29:                                ;   in Loop: Header=BB5_10 Depth=1
	lsl	x23, x8, #1
	ldr	x0, [x21, #8]
	mov	x1, x23
	bl	_realloc
	cbz	x0, LBB5_47
; %bb.30:                               ;   in Loop: Header=BB5_10 Depth=1
	str	x0, [x21, #8]
	str	x23, [x21, #24]
	ldr	x20, [x21, #32]
	cbnz	x20, LBB5_12
	b	LBB5_44
LBB5_31:
	cbz	w0, LBB5_44
; %bb.32:
	mov	w19, #8                         ; =0x8
	mov	w20, #16                        ; =0x10
LBB5_33:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB5_36 Depth 2
	ldp	x9, x8, [x21, #16]
	sub	x10, x8, #8
	cmp	x9, x10
	b.hs	LBB5_41
; %bb.34:                               ;   in Loop: Header=BB5_33 Depth=1
	ldr	x8, [x21]
	cbz	x8, LBB5_44
LBB5_35:                                ;   in Loop: Header=BB5_33 Depth=1
	ldr	x23, [x8]
	cbz	x23, LBB5_44
LBB5_36:                                ;   Parent Loop BB5_33 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldrb	w8, [x23, #1]
	tbnz	w8, #0, LBB5_39
; %bb.37:                               ;   in Loop: Header=BB5_36 Depth=2
	mov	x0, x22
	bl	_bit_stream_has_data
	cbz	w0, LBB5_44
; %bb.38:                               ;   in Loop: Header=BB5_36 Depth=2
	mov	x0, x22
	bl	_bit_stream_read_bit
	cmp	w0, #0
	csel	x8, x20, x19, ne
	ldr	x23, [x23, x8]
	cbnz	x23, LBB5_36
	b	LBB5_44
LBB5_39:                                ;   in Loop: Header=BB5_33 Depth=1
	ldrb	w8, [x23]
	ldp	x9, x10, [x21, #8]
	add	x11, x10, #1
	str	x11, [x21, #16]
	strb	w8, [x9, x10]
	ldr	x8, [x21, #16]
	tst	x8, #0x3f
	b.eq	LBB5_43
; %bb.40:                               ;   in Loop: Header=BB5_33 Depth=1
	mov	x0, x22
	bl	_bit_stream_has_data
	tbnz	w0, #0, LBB5_33
	b	LBB5_44
LBB5_41:                                ;   in Loop: Header=BB5_33 Depth=1
	lsl	x23, x8, #1
	ldr	x0, [x21, #8]
	mov	x1, x23
	bl	_realloc
	cbz	x0, LBB5_47
; %bb.42:                               ;   in Loop: Header=BB5_33 Depth=1
	str	x0, [x21, #8]
	str	x23, [x21, #24]
	ldr	x8, [x21]
	cbnz	x8, LBB5_35
	b	LBB5_44
LBB5_43:                                ;   in Loop: Header=BB5_33 Depth=1
	ldr	x9, [x21, #8]
	add	x8, x9, x8
	prfm	pstl3keep, [x8, #64]
	mov	x0, x22
	bl	_bit_stream_has_data
	tbnz	w0, #0, LBB5_33
LBB5_44:
	mov	w0, #0                          ; =0x0
	ldp	x8, x9, [x21, #8]
	ldr	x10, [sp]                       ; 8-byte Folded Reload
	str	x8, [x10]
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	str	x9, [x8]
LBB5_45:
	ldr	x8, [sp, #88]
Lloh9:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh10:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh11:
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	LBB5_48
LBB5_46:
	ldp	x29, x30, [sp, #176]            ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #160]            ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #144]            ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #128]            ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #112]            ; 16-byte Folded Reload
	ldp	x28, x27, [sp, #96]             ; 16-byte Folded Reload
	add	sp, sp, #192
	ret
LBB5_47:
	mov	w0, #-1                         ; =0xffffffff
	ldr	x8, [sp, #88]
Lloh12:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh13:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh14:
	ldr	x9, [x9]
	cmp	x9, x8
	b.eq	LBB5_46
LBB5_48:
	bl	___stack_chk_fail
	.loh AdrpLdrGotLdr	Lloh6, Lloh7, Lloh8
	.loh AdrpLdrGotLdr	Lloh9, Lloh10, Lloh11
	.loh AdrpLdrGotLdr	Lloh12, Lloh13, Lloh14
	.cfi_endproc
                                        ; -- End function
	.globl	_neon_decode_symbols_batch      ; -- Begin function neon_decode_symbols_batch
	.p2align	2
_neon_decode_symbols_batch:             ; @neon_decode_symbols_batch
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #128
	stp	x26, x25, [sp, #48]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #64]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #80]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #96]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #112]            ; 16-byte Folded Spill
	add	x29, sp, #112
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
Lloh15:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh16:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh17:
	ldr	x8, [x8]
	str	x8, [sp, #40]
	mov	w19, #-1                        ; =0xffffffff
	cbz	x0, LBB6_17
; %bb.1:
	mov	x22, x1
	cbz	x1, LBB6_17
; %bb.2:
	mov	x21, x2
	cbz	x2, LBB6_17
; %bb.3:
	mov	x20, x3
	cmp	w3, #1
	b.lt	LBB6_17
; %bb.4:
	mov	x23, x0
	mov	w19, #0                         ; =0x0
	mov	w25, #8                         ; =0x8
	mov	x26, sp
	b	LBB6_6
LBB6_5:                                 ;   in Loop: Header=BB6_6 Depth=1
	cmp	w19, w20
	b.ge	LBB6_17
LBB6_6:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB6_12 Depth 2
	mov	x0, x22
	bl	_bit_stream_has_data
	cbz	w0, LBB6_17
; %bb.7:                                ;   in Loop: Header=BB6_6 Depth=1
	sub	w8, w20, w19
	cmp	w8, #8
	csel	w24, w8, w25, lt
	cmp	w8, #1
	b.lt	LBB6_9
; %bb.8:                                ;   in Loop: Header=BB6_6 Depth=1
	cmp	w24, #1
	csinc	w2, w24, wzr, gt
	ldrb	w1, [x23, #33]
	add	x0, sp, #32
	bl	_memset
LBB6_9:                                 ;   in Loop: Header=BB6_6 Depth=1
	add	x1, sp, #32
	mov	x2, sp
	mov	x0, x22
	mov	x3, x24
	bl	_bit_stream_read_bits_batch
	cmp	w0, #1
	b.lt	LBB6_17
; %bb.10:                               ;   in Loop: Header=BB6_6 Depth=1
	mov	x8, #0                          ; =0x0
	mov	w9, w0
	add	x10, x21, w19, uxtw
	b	LBB6_12
LBB6_11:                                ;   in Loop: Header=BB6_12 Depth=2
	add	x8, x8, #1
	cmp	x9, x8
	b.eq	LBB6_15
LBB6_12:                                ;   Parent Loop BB6_6 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	w11, [x26, x8, lsl #2]
	ldr	x12, [x23, #16]
	cmp	x12, x11
	b.ls	LBB6_11
; %bb.13:                               ;   in Loop: Header=BB6_12 Depth=2
	ldr	x12, [x23]
	add	x11, x12, x11, lsl #6
	ldrb	w12, [x11, #1]
	cbz	w12, LBB6_11
; %bb.14:                               ;   in Loop: Header=BB6_12 Depth=2
	ldrb	w11, [x11]
	strb	w11, [x10, x8]
	b	LBB6_11
LBB6_15:                                ;   in Loop: Header=BB6_6 Depth=1
	add	w19, w0, w19
	add	w8, w19, #64
	cmp	w8, w20
	b.ge	LBB6_5
; %bb.16:                               ;   in Loop: Header=BB6_6 Depth=1
	add	x8, x21, w8, uxtw
	prfm	pstl3keep, [x8]
	b	LBB6_5
LBB6_17:
	ldr	x8, [sp, #40]
Lloh18:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh19:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh20:
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	LBB6_19
; %bb.18:
	mov	x0, x19
	ldp	x29, x30, [sp, #112]            ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #96]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #80]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #64]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #128
	ret
LBB6_19:
	bl	___stack_chk_fail
	.loh AdrpLdrGotLdr	Lloh15, Lloh16, Lloh17
	.loh AdrpLdrGotLdr	Lloh18, Lloh19, Lloh20
	.cfi_endproc
                                        ; -- End function
	.globl	_vectorized_decode_symbol       ; -- Begin function vectorized_decode_symbol
	.p2align	2
_vectorized_decode_symbol:              ; @vectorized_decode_symbol
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB7_8
; %bb.1:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	mov	w0, #-1                         ; =0xffffffff
	cbz	x2, LBB7_7
; %bb.2:
	cbz	x1, LBB7_7
; %bb.3:
	ldr	x8, [x19]
	cbz	x8, LBB7_7
; %bb.4:
	mov	x20, x2
	ldrb	w8, [x19, #33]
	mov	x0, x1
	mov	x1, x8
	bl	_bit_stream_read_bits
	mov	w8, w0
	ldr	x9, [x19, #16]
	cmp	x9, x8
	b.ls	LBB7_9
; %bb.5:
	ldr	x9, [x19]
	add	x8, x9, x8, lsl #6
	ldrb	w9, [x8, #1]
	cbz	w9, LBB7_9
; %bb.6:
	mov	w0, #0                          ; =0x0
	ldrb	w8, [x8]
	strb	w8, [x20]
LBB7_7:
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
LBB7_8:
	mov	w0, #-1                         ; =0xffffffff
	ret
LBB7_9:
	mov	w0, #-1                         ; =0xffffffff
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function build_lookup_table_recursive
_build_lookup_table_recursive:          ; @build_lookup_table_recursive
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB8_9
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
	mov	x19, x4
	mov	x20, x3
	mov	x21, x2
	mov	x23, x1
	mov	x22, x0
	ldrb	w8, [x0, #1]
	tbnz	w8, #0, LBB8_7
; %bb.2:
	mov	w24, #1                         ; =0x1
LBB8_3:                                 ; =>This Inner Loop Header: Depth=1
	ldr	x0, [x22, #8]
	cbz	x0, LBB8_5
; %bb.4:                                ;   in Loop: Header=BB8_3 Depth=1
	lsl	w1, w23, #1
	add	w8, w21, #1
	and	w2, w8, #0xff
	mov	x3, x20
	mov	x4, x19
	bl	_build_lookup_table_recursive
LBB8_5:                                 ;   in Loop: Header=BB8_3 Depth=1
	ldr	x22, [x22, #16]
	cbz	x22, LBB8_8
; %bb.6:                                ;   in Loop: Header=BB8_3 Depth=1
	orr	w23, w24, w23, lsl #1
	add	w21, w21, #1
	ldrb	w8, [x22, #1]
	tbz	w8, #0, LBB8_3
LBB8_7:
	cmp	w19, w21, uxtb
	b.hs	LBB8_10
LBB8_8:
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp], #64             ; 16-byte Folded Reload
LBB8_9:
	ret
LBB8_10:
	sub	w8, w19, w21, uxtb
	lsl	w9, w23, w8
	mov	w10, #1                         ; =0x1
	b	LBB8_12
LBB8_11:                                ;   in Loop: Header=BB8_12 Depth=1
	lsr	w11, w10, w8
	add	w10, w10, #1
	cbnz	w11, LBB8_8
LBB8_12:                                ; =>This Inner Loop Header: Depth=1
	sub	w11, w10, #1
	orr	w11, w11, w9
	lsr	w12, w11, w19
	cbnz	w12, LBB8_11
; %bb.13:                               ;   in Loop: Header=BB8_12 Depth=1
	ldrb	w12, [x22]
	add	x11, x20, x11, lsl #6
	strb	w12, [x11]
	strb	w21, [x11, #1]
	b	LBB8_11
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
