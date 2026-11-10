

# It's a simple UEFI application that says "hello, world"

You won't break your computer... trust me bro. 👍🏿

Oh, and you need Debian

0. Requirements

the main.c file


1. Requirements(bis):

You will need gnu-efi, gcc, ld, objcopy, qemu, ovmf, mtools, and dosfstools


To install them:

```bash

sudo apt install gnu-efi gcc qemu-system-x86 ovmf  mtools dosfstools binutils

```



2. Compile

```bash

gcc -I/usr/include/efi \
    -I/usr/include/efi/x86_64 \
    -fpic -fshort-wchar -mno-red-zone \
    -DEFI_FUNCTION_WRAPPER \
    -c main.c -o main.o
```


3. Link the program

```bash

ld -nostdlib -znocombreloc \
   -T /usr/lib/elf_x86_64_efi.lds \
   -shared -Bsymbolic \
   /usr/lib/crt0-efi-x86_64.o main.o \
   -L/usr/lib -lefi -lgnuefi \
   -o main.so
```


4. Create the EFI file

```bash

objcopy \
  -j .text -j .sdata -j .data \
  -j .dynamic -j .dynsym -j .rel \
  -j .rela -j .reloc \
  --target=efi-app-x86_64 \
  main.so BOOTX64.EFI
```


5. Check for the OVMF files

On debian,

```bash
ls /usr/share/OVMF/
```
You should find OVMF_CODE.fd (The firmware uefi) and OVMF_VARS.fd (persistent variables)
Plot twist: depending on your Debian version, the files may be called OVMF_CODE_4M.fd instead of OVMF_CODE.fd. The 4M is the size of the virtual flash chip. Bigger number, same vibes. Whatever your ls shows, use that name in steps 9 and 10, or QEMU will complain and you’ll blame me.


6. Create a virtual USB


```bash

mkdir -p esp/EFI/BOOT
cp BOOTX64.EFI esp/EFI/BOOT/
```

7. Create a FAT (not to be confused with BIG) image


```bash

dd if=/dev/zero of=uefi.img bs=1M count=64
mkfs.vfat uefi.img

```



8. Copy your program in the image


```bash

mmd -i uefi.img ::/EFI
mmd -i uefi.img ::/EFI/BOOT
mcopy -i uefi.img esp/EFI/BOOT/BOOTX64.EFI ::/EFI/BOOT/
```


9. Copy the UEFI variables

in my case, it was,

```bash

cp /usr/share/OVMF/OVMF_VARS_4M.fd vars.fd
```



10. Launch QEMU !!!

```bash
qemu-system-x86_64 \
  -machine q35 \
  -m 512 \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
  -drive if=pflash,format=raw,file=vars.fd \
  -drive format=raw,file=uefi.img \
  -net none
```
