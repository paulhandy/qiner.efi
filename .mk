TARGET = qubic.efi
#ARCH = $(shell uname -m | sed s,i[3456789]86,ia32,)

CC = clang

CFLAGS := -xc++ \
	      -target x86_64-unknown-windows \
	      -ffreestanding \
	      -fshort-wchar \
	      -mno-red-zone

LDFLAGS :=	\
	-target x86_64-unknown-windows \
	-nostdlib \
	-Wl,-entry:efi_main \
	-Wl,-subsystem:efi_application \
	-fuse-ld=lld-link

CFLAGS += $(EFIINCS)
LDFLAGS += $(EFILIB) $(LIB)


OUTDIR = objs
BINDIR = ./EFI
SRCDIR = src

SRCS = $(wildcard *.cpp $(foreach fd, $(SRCDIR), $(fd)*.c))
OBJS = $(addprefix $(OUTDIR)/, $(SRCS:cpp=o))

all: $(BINDIR)/$(TARGET)

qemu: all
	qemu-system-x86_64 -enable-kvm \
	-drive if=pflash,format=raw,readonly=on,file=./ovmf/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=./ovmf/OVMF_VARS.fd \
	-drive index=0,format=raw,file=fat:rw:$(BINDIR)
	#-nographic \

$(BINDIR)/$(TARGET): $(OBJS)
	@ mkdir -p $(@D)
	@ $(CC) $(LDFLAGS) -o $@ $^

$(OUTDIR)/%.o: %.cpp
	@ mkdir -p $(@D)
	@ $(CC) $(CFLAGS) -o $@ -c $<

#$(EFI_HDIR)/efi.h:
#	git submodule update --init --recursive

.PHONY: clean

clean:
	@rm -Path $(OUTDIR) $(BINDIR)
