	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
_int_tag:
	.word	2
_bool_tag:
	.word	3
_string_tag:
	.word	4
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const33:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.byte	0	
	.align	2
	.word	-1
str_const32:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const31:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"O"
	.byte	0	
	.align	2
	.word	-1
str_const30:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"N"
	.byte	0	
	.align	2
	.word	-1
str_const29:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"M"
	.byte	0	
	.align	2
	.word	-1
str_const28:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"C"
	.byte	0	
	.align	2
	.word	-1
str_const27:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"B"
	.byte	0	
	.align	2
	.word	-1
str_const26:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"A"
	.byte	0	
	.align	2
	.word	-1
str_const25:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const7
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const24:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const23:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const5
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const22:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const3
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const21:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const7
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const20:
	.word	4
	.word	8
	.word	String_dispTab
	.word	int_const8
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const19:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"\n"
	.byte	0	
	.align	2
	.word	-1
str_const18:
	.word	4
	.word	8
	.word	String_dispTab
	.word	int_const9
	.ascii	"Hello World\n"
	.byte	0	
	.align	2
	.word	-1
str_const17:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"Inh2"
	.byte	0	
	.align	2
	.word	-1
str_const16:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"b"
	.byte	0	
	.align	2
	.word	-1
str_const15:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"a"
	.byte	0	
	.align	2
	.word	-1
str_const14:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"Inh1"
	.byte	0	
	.align	2
	.word	-1
str_const13:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"Base"
	.byte	0	
	.align	2
	.word	-1
str_const12:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"int\n"
	.byte	0	
	.align	2
	.word	-1
str_const11:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const10
	.ascii	"string\n"
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"888\n"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"777\n"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"666\n"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"555\n"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"444\n"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"333\n"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"222\n"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const6
	.ascii	"111\n"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const11
	.ascii	"str_field"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const5
	.ascii	"222"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const10
	.ascii	"test.cl"
	.byte	0	
	.align	2
	.word	-1
int_const11:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	9
	.word	-1
int_const10:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	7
	.word	-1
int_const9:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	12
	.word	-1
int_const8:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	13
	.word	-1
int_const7:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	6
	.word	-1
int_const6:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	4
	.word	-1
int_const5:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	3
	.word	-1
int_const4:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	5
	.word	-1
int_const3:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	2
	.word	-1
int_const2:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
int_const1:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	1
	.word	-1
int_const0:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	00000
	.word	-1
bool_const0:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
bool_const1:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	1
class_nameTab:
	.word	str_const21
	.word	str_const22
	.word	str_const23
	.word	str_const24
	.word	str_const25
	.word	str_const26
	.word	str_const27
	.word	str_const28
	.word	str_const29
	.word	str_const30
	.word	str_const31
	.word	str_const13
	.word	str_const14
	.word	str_const17
	.word	str_const32
class_objTab:
	.word	Object_protObj
	.word	Object_init
	.word	IO_protObj
	.word	IO_init
	.word	Int_protObj
	.word	Int_init
	.word	Bool_protObj
	.word	Bool_init
	.word	String_protObj
	.word	String_init
	.word	A_protObj
	.word	A_init
	.word	B_protObj
	.word	B_init
	.word	C_protObj
	.word	C_init
	.word	M_protObj
	.word	M_init
	.word	N_protObj
	.word	N_init
	.word	O_protObj
	.word	O_init
	.word	Base_protObj
	.word	Base_init
	.word	Inh1_protObj
	.word	Inh1_init
	.word	Inh2_protObj
	.word	Inh2_init
	.word	Main_protObj
	.word	Main_init
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Main_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Main.let_test
	.word	Main.get
	.word	Main.main
Base_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Base.name
Inh2_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Inh2.name
Inh1_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Inh1.name
M_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	M.t
N_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	N.t
O_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	N.t
	.word	O.test
A_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.get_str
	.word	A.f
B_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.get_str
	.word	A.f
C_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.get_str
	.word	A.f
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.length
	.word	String.concat
	.word	String.substr
Bool_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Int_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
IO_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
	.word	-1
Object_protObj:
	.word	0
	.word	3
	.word	Object_dispTab
	.word	-1
Main_protObj:
	.word	14
	.word	11
	.word	Main_dispTab
	.word	0
	.word	str_const33
	.word	bool_const0
	.word	0
	.word	0
	.word	int_const2
	.word	0
	.word	0
	.word	-1
Base_protObj:
	.word	11
	.word	3
	.word	Base_dispTab
	.word	-1
Inh2_protObj:
	.word	13
	.word	3
	.word	Inh2_dispTab
	.word	-1
Inh1_protObj:
	.word	12
	.word	3
	.word	Inh1_dispTab
	.word	-1
M_protObj:
	.word	8
	.word	3
	.word	M_dispTab
	.word	-1
N_protObj:
	.word	9
	.word	3
	.word	N_dispTab
	.word	-1
O_protObj:
	.word	10
	.word	3
	.word	O_dispTab
	.word	-1
A_protObj:
	.word	5
	.word	7
	.word	A_dispTab
	.word	0
	.word	int_const2
	.word	str_const33
	.word	str_const33
	.word	-1
B_protObj:
	.word	6
	.word	7
	.word	B_dispTab
	.word	0
	.word	int_const2
	.word	str_const33
	.word	str_const33
	.word	-1
C_protObj:
	.word	7
	.word	7
	.word	C_dispTab
	.word	0
	.word	int_const2
	.word	str_const33
	.word	str_const33
	.word	-1
String_protObj:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const2
	.word	0
	.word	-1
Bool_protObj:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
Int_protObj:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
IO_protObj:
	.word	1
	.word	3
	.word	IO_dispTab
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
Object_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	la	$a0 IO_protObj
	jal	Object.copy
	jal	IO_init
	sw	$a0 12($s0)
	la	$a0 A_protObj
	jal	Object.copy
	jal	A_init
	sw	$a0 24($s0)
	lw	$a0 24($s0)
	sw	$a0 28($s0)
	la	$a0 int_const1
	sw	$a0 32($s0)
	la	$a0 O_protObj
	jal	Object.copy
	jal	O_init
	sw	$a0 36($s0)
	la	$a0 Inh2_protObj
	jal	Object.copy
	jal	Inh2_init
	sw	$a0 40($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Base_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Inh2_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Base_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Inh1_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Base_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
M_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
N_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	M_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
O_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	N_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
A_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	la	$a0 IO_protObj
	jal	Object.copy
	jal	IO_init
	sw	$a0 12($s0)
	move	$a0 $s0
	bne	$a0 $zero label0
	la	$a0 str_const0
	li	$t1 7
	jal	_dispatch_abort
label0:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	sw	$a0 20($s0)
	la	$a0 str_const1
	sw	$a0 24($s0)
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
B_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	A_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
C_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	B_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
String_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Bool_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Int_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
IO_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main.let_test:
	addiu	$sp $sp -16
	sw	$fp 16($sp)
	sw	$s0 12($sp)
	sw	$ra 8($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	sw	$s1 0($fp)
	move	$s1 $zero
	move	$a0 $s1
	bne	$a0 $zero label1
	la	$a0 str_const0
	li	$t1 84
	jal	_dispatch_abort
label1:
	lw	$t1 8($a0)
	lw	$t1 16($t1)
	jalr		$t1
	lw	$s1 0($fp)
	lw	$fp 16($sp)
	lw	$s0 12($sp)
	lw	$ra 8($sp)
	addiu	$sp $sp 16
	jr	$ra	
Main.get:
	addiu	$sp $sp -16
	sw	$fp 16($sp)
	sw	$s0 12($sp)
	sw	$ra 8($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	sw	$s1 0($fp)
	lw	$s1 16($fp)
	la	$t2 int_const1
	move	$t1 $s1
	la	$a0 bool_const1
	beq	$t1 $t2 label4
	la	$a1 bool_const0
	jal	equality_test
label4:
	lw	$t1 12($a0)
	beqz	$t1 label2
	la	$a0 Inh1_protObj
	jal	Object.copy
	jal	Inh1_init
	b	label3
label2:
	la	$a0 Inh2_protObj
	jal	Object.copy
	jal	Inh2_init
label3:
	lw	$s1 0($fp)
	lw	$fp 16($sp)
	lw	$s0 12($sp)
	lw	$ra 8($sp)
	addiu	$sp $sp 20
	jr	$ra	
Main.main:
	addiu	$sp $sp -16
	sw	$fp 16($sp)
	sw	$s0 12($sp)
	sw	$ra 8($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	sw	$s1 0($fp)
	la	$a0 int_const1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$a0 $s0
	bne	$a0 $zero label5
	la	$a0 str_const0
	li	$t1 90
	jal	_dispatch_abort
label5:
	lw	$t1 8($a0)
	lw	$t1 16($t1)
	jalr		$t1
	la	$a0 str_const18
	sw	$a0 16($s0)
	lw	$a0 16($s0)
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label6
	la	$a0 str_const0
	li	$t1 92
	jal	_dispatch_abort
label6:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	lw	$a0 24($s0)
	bne	$a0 $zero label7
	la	$a0 str_const0
	li	$t1 93
	jal	_dispatch_abort
label7:
	lw	$t1 8($a0)
	lw	$t1 16($t1)
	jalr		$t1
	la	$s1 int_const1
	la	$a0 int_const3
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s1)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
	move	$s1 $a0
	la	$a0 int_const3
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s1)
	mul	$t1 $t1 $t2
	sw	$t1 12($a0)
	move	$s1 $a0
	la	$a0 int_const4
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s1)
	sub	$t1 $t1 $t2
	sw	$t1 12($a0)
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	move	$a0 $s0
	bne	$a0 $zero label8
	la	$a0 str_const0
	li	$t1 94
	jal	_dispatch_abort
label8:
	lw	$t1 8($a0)
	lw	$t1 16($t1)
	jalr		$t1
	bne	$a0 $zero label9
	la	$a0 str_const0
	li	$t1 94
	jal	_dispatch_abort
label9:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label10
	la	$a0 str_const0
	li	$t1 94
	jal	_dispatch_abort
label10:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	la	$a0 str_const19
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label11
	la	$a0 str_const0
	li	$t1 95
	jal	_dispatch_abort
label11:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	la	$a0 int_const1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	la	$a0 int_const3
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	la	$a0 int_const5
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 36($s0)
	bne	$a0 $zero label12
	la	$a0 str_const0
	li	$t1 96
	jal	_dispatch_abort
label12:
	lw	$t1 8($a0)
	lw	$t1 16($t1)
	jalr		$t1
	lw	$a0 40($s0)
	bne	$a0 $zero label13
	la	$a0 str_const0
	li	$t1 97
	jal	_dispatch_abort
label13:
	la	$t1 Base_dispTab
	lw	$t1 12($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label14
	la	$a0 str_const0
	li	$t1 97
	jal	_dispatch_abort
label14:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	lw	$s1 0($fp)
	lw	$fp 16($sp)
	lw	$s0 12($sp)
	lw	$ra 8($sp)
	addiu	$sp $sp 16
	jr	$ra	
Base.name:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	la	$a0 str_const13
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Inh2.name:
	addiu	$sp $sp -16
	sw	$fp 16($sp)
	sw	$s0 12($sp)
	sw	$ra 8($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	sw	$s1 0($fp)
	la	$s1 int_const1
	la	$a0 int_const2
	lw	$t1 12($s1)
	lw	$t2 12($a0)
	la	$a0 bool_const1
	blt	$t1 $t2 label17
	la	$a0 bool_const0
label17:
	lw	$t1 12($a0)
	beqz	$t1 label15
	la	$a0 str_const15
	b	label16
label15:
	la	$a0 str_const16
label16:
	la	$a0 str_const17
	lw	$s1 0($fp)
	lw	$fp 16($sp)
	lw	$s0 12($sp)
	lw	$ra 8($sp)
	addiu	$sp $sp 16
	jr	$ra	
Inh1.name:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	la	$a0 str_const14
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
M.t:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	la	$a0 int_const1
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 20
	jr	$ra	
N.t:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	la	$a0 int_const1
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 20
	jr	$ra	
O.test:
	addiu	$sp $sp -16
	sw	$fp 16($sp)
	sw	$s0 12($sp)
	sw	$ra 8($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	sw	$s1 0($fp)
	lw	$s1 24($fp)
	lw	$a0 20($fp)
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s1)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
	move	$s1 $a0
	lw	$a0 16($fp)
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s1)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
	lw	$s1 0($fp)
	lw	$fp 16($sp)
	lw	$s0 12($sp)
	lw	$ra 8($sp)
	addiu	$sp $sp 28
	jr	$ra	
A.get_str:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	la	$a0 str_const2
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
A.f:
	addiu	$sp $sp -40
	sw	$fp 40($sp)
	sw	$s0 36($sp)
	sw	$ra 32($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	sw	$s1 24($fp)
	sw	$s2 20($fp)
	sw	$s3 16($fp)
	sw	$s4 12($fp)
	sw	$s5 8($fp)
	sw	$s6 4($fp)
	la	$a0 int_const0
	sw	$a0 16($s0)
	la	$s6 str_const3
	sw	$s6 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label18
	la	$a0 str_const0
	li	$t1 15
	jal	_dispatch_abort
label18:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	la	$s6 str_const4
	la	$s5 str_const5
	sw	$s6 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label19
	la	$a0 str_const0
	li	$t1 18
	jal	_dispatch_abort
label19:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	sw	$s5 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label20
	la	$a0 str_const0
	li	$t1 19
	jal	_dispatch_abort
label20:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	la	$s4 str_const6
	la	$s3 str_const7
	sw	$s3 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label21
	la	$a0 str_const0
	li	$t1 22
	jal	_dispatch_abort
label21:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	la	$s2 str_const8
	la	$s1 str_const9
	sw	$s2 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label22
	la	$a0 str_const0
	li	$t1 25
	jal	_dispatch_abort
label22:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	la	$a0 str_const10
	sw	$a0 0($fp)
	lw	$a0 0($fp)
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label23
	la	$a0 str_const0
	li	$t1 27
	jal	_dispatch_abort
label23:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	la	$a0 int_const1
	bne	$a0 $zero label25
	la	$a0 str_const0
	li	$t1 35
	jal	_case_abort2
label25:
	lw	$t2 0($a0)
	blt	$t2 4 label26
	bgt	$t2 4 label26
	move	$s6 $a0
	la	$a0 str_const11
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label27
	la	$a0 str_const0
	li	$t1 36
	jal	_dispatch_abort
label27:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	b	label24
label26:
	blt	$t2 2 label28
	bgt	$t2 2 label28
	move	$s6 $a0
	la	$a0 str_const12
	sw	$a0 0($sp)
	addiu	$sp $sp -4
	lw	$a0 12($s0)
	bne	$a0 $zero label29
	la	$a0 str_const0
	li	$t1 37
	jal	_dispatch_abort
label29:
	lw	$t1 8($a0)
	lw	$t1 12($t1)
	jalr		$t1
	b	label24
label28:
	jal	_case_abort
label24:
	la	$a0 int_const1
	lw	$s1 24($fp)
	lw	$s2 20($fp)
	lw	$s3 16($fp)
	lw	$s4 12($fp)
	lw	$s5 8($fp)
	lw	$s6 4($fp)
	lw	$fp 40($sp)
	lw	$s0 36($sp)
	lw	$ra 32($sp)
	addiu	$sp $sp 40
	jr	$ra	
