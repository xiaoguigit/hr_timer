KERN_DIR = /work/xilinx-v2015.4

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= hello.o
#obj-m	+= buttons.o
