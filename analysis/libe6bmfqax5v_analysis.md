# libe6bmfqax5v.so Analysis Report

## Binary Information
- File: libe6bmfqax5v.so
- Size: 866232 bytes
- Exported functions: 4
- Total strings: 1726

## Exported Functions

### JNI_OnLoad
- Address: 0x00000000000547b4
- Size: ~800 bytes
- Instructions: 200
```asm
bti	c
nop
adr	x0, bdb40 <pthread_key_create@plt+0x4010>
b	b98f0 <__cxa_finalize@plt>
bti	c
ret
bti	c
bti	c
mov	x16, x0
br	x16
ret
bti	c
mov	x1, x0
nop
nop
adr	x2, bdb40 <pthread_key_create@plt+0x4010>
b	b9900 <__cxa_atexit@plt>
paciasp
str	x30, [sp, #-64]!
stp	x24, x23, [sp, #16]
stp	x22, x21, [sp, #32]
stp	x20, x19, [sp, #48]
add	x8, x0, #0x10
mov	x19, x0
ldar	x22, [x8]
adrp	x23, da000 <pthread_key_create@plt+0x204d0>
ldr	w0, [x23, #3444]
bl	b9aa0 <pthread_getspecific@plt>
ldr	x21, [x0, #8]
mov	x20, x0
cmp	x21, x22
add	x8, x22, #0x11
mov	x0, x20
and	x8, x8, #0xfffffffffffffff0
sub	x24, x8, #0x2
lsl	x8, x24, #3
add	x1, x8, #0x10
bl	b9a90 <realloc@plt>
sub	x8, x24, x21
add	x9, x0, x21, lsl #3
mov	x20, x0
lsl	x2, x8, #3
mov	w1, wzr
add	x0, x9, #0x10
bl	b9ae0 <memset@plt>
adrp	x20, da000 <pthread_key_create@plt+0x204d0>
add	x20, x20, #0xd78
nop
mov	x0, x20
bl	b9b20 <pthread_once@plt>
... (150 more instructions)
```

### Mundo_Activate_SDK
- Address: 0x0000000000054814
- Size: ~800 bytes
- Instructions: 200
```asm
movk	x9, #0xe0a2, lsl #16
nop
adr	x2, bdb40 <pthread_key_create@plt+0x4010>
movk	x9, #0x461c, lsl #32
movk	x9, #0xdf76, lsl #48
ldr	x8, [x8, x9]
add	x16, x8, x22
mov	x1, x20
ldp	x20, x19, [sp, #32]
ldp	x22, x21, [sp, #16]
ldp	x29, x30, [sp], #48
autiasp
br	x16
paciasp
stp	x29, x30, [sp, #-48]!
str	x21, [sp, #16]
stp	x20, x19, [sp, #32]
mov	x29, sp
mov	x9, #0xcfb9                	// #53177
adrp	x20, d6000 <pthread_key_create@plt+0x1c4d0>
mov	x21, #0x6c1e                	// #27678
movk	x9, #0xcb88, lsl #16
ldr	x8, [x20, #1072]
movk	x21, #0x5b01, lsl #16
movk	x9, #0xab4, lsl #32
movk	x21, #0x5da6, lsl #32
nop
adr	x19, db760 <pthread_key_create@plt+0x21c30>
movk	x9, #0x4c19, lsl #48
movk	x21, #0xa3ab, lsl #48
ldr	x8, [x8, x9]
mov	x0, x19
add	x8, x8, x21
blr	x8
mov	x9, #0xcfb1                	// #53169
ldr	x8, [x20, #1072]
nop
movk	x9, #0xcb88, lsl #16
nop
adr	x2, bdb40 <pthread_key_create@plt+0x4010>
movk	x9, #0xab4, lsl #32
movk	x9, #0x4c19, lsl #48
ldr	x8, [x8, x9]
add	x16, x8, x21
mov	x1, x19
ldp	x20, x19, [sp, #32]
ldr	x21, [sp, #16]
ldp	x29, x30, [sp], #48
autiasp
br	x16
... (150 more instructions)
```

### __start___lcxx_override
- Address: 0x00000000000b9870
- Size: ~800 bytes
- Instructions: 200
```asm
mov	x1, x19
ldp	x30, x19, [sp], #16
ret
sub	sp, sp, #0x30
stp	x30, x19, [sp, #32]
adrp	x1, 40000 <JNI_OnLoad@@Base-0x147b4>
add	x1, x1, #0xc0e
bl	b9674 <Mundo_Activate_SDK@@Base+0x64e60>
ldrb	w8, [sp, #8]
ldr	x9, [sp, #24]
tst	w8, #0x1
csinc	x0, x9, x19, ne	// ne = any
bl	6de1c <Mundo_Activate_SDK@@Base+0x19608>
sub	sp, sp, #0x30
stp	x30, x19, [sp, #32]
adrp	x1, 40000 <JNI_OnLoad@@Base-0x147b4>
add	x1, x1, #0xb49
bl	b9674 <Mundo_Activate_SDK@@Base+0x64e60>
ldrb	w8, [sp, #8]
ldr	x9, [sp, #24]
tst	w8, #0x1
csinc	x0, x9, x19, ne	// ne = any
bl	b94ac <Mundo_Activate_SDK@@Base+0x64c98>
str	x30, [sp, #-16]!
mov	x1, x0
adrp	x0, 40000 <JNI_OnLoad@@Base-0x147b4>
add	x0, x0, #0xb59
bl	b9698 <Mundo_Activate_SDK@@Base+0x64e84>
mov	w9, #0x4240                	// #16960
movk	w9, #0xf, lsl #16
cmp	w1, w9
b.cs	b94f8 <Mundo_Activate_SDK@@Base+0x64ce4>  // b.hs, b.nlast
lsr	w8, w1, #4
cmp	w8, #0x270
b.hi	b952c <Mundo_Activate_SDK@@Base+0x64d18>  // b.pmore
cmp	w1, #0x63
b.hi	b9574 <Mundo_Activate_SDK@@Base+0x64d60>  // b.pmore
cmp	w1, #0x9
b.hi	b95d0 <Mundo_Activate_SDK@@Base+0x64dbc>  // b.pmore
orr	w8, w1, #0x30
strb	w8, [x0], #1
ret
mov	w8, #0xe100                	// #57600
movk	w8, #0x5f5, lsl #16
cmp	w1, w8
b.cs	b9550 <Mundo_Activate_SDK@@Base+0x64d3c>  // b.hs, b.nlast
mov	w8, #0x967f                	// #38527
movk	w8, #0x98, lsl #16
cmp	w1, w8
b.hi	b95b0 <Mundo_Activate_SDK@@Base+0x64d9c>  // b.pmore
... (150 more instructions)
```

### __stop___lcxx_override
- Address: 0x00000000000b98cc
- Size: ~0 bytes

## Interesting Strings

```
Android
13004108
__cxa_finalize
__cxa_atexit
JNI_OnLoad
Mundo_Activate_SDK
__system_property_get
atoi
strlen
getuid
getpid
dlopen
dlsym
dlclose
memcmp
syscall
dladdr
time
memchr
__memcpy_chk
__vsprintf_chk
__memset_chk
pthread_mutex_lock
pthread_mutex_unlock
free
malloc
posix_memalign
__start___lcxx_override
__stop___lcxx_override
__errno
close
__open_2
__read_chk
strtol
abort
vfprintf
vasprintf
android_set_abort_message
openlog
syslog
closelog
__sF
fwrite
fputc
strncmp
realloc
pthread_getspecific
memcpy
memmove
__stack_chk_fail
memset
getauxval
pthread_key_delete
pthread_setspecific
pthread_once
pthread_key_create
libdl.so
LIBC
libc.so
libm.so
libe6bmfqax5v.so
\!%C
\$%C
,#%C
<$%C
|!%C
\ %C
< %C
L#%C
.SZg
NQZg
NQZg
zQZg
NQZg
>RZg
jSZg
NQZg
RSZg
jQZg
&RZg
NQZg
NQZg
>QZg
VRZg
x1Qu_
)w1Qu_
w1Qu_
v1Qu_
9w1Qu_
w1Qu_
w1Qu_
v1Qu_
v1Qu_
v1Qu_
v1Qu_
x1Qu_
v1Qu_
v1Qu_
Q+1`
\R+1`
```
