SDCC = "../compiler/sdcc/bin/sdcc"
IHX2SMS=../compiler/sdcc/bin/ihx2sms
FOLDER2C=../compiler/sdcc/bin/folder2c
BMP2TILE=../bmp2tile/bmp2tile.exe
EMULATOR=Fusion

all: main.sms

graphics.c:
	# for i in `ls assets/*.png`; do STRIPPED=`echo $i | sed 's/assets\///' | sed 's/\.png//'`; filename=`dirname $i`/includes/$(STRIPPED);	../bmp2tile/bmp2tile.exe $i -8x16 -savepalette $(filename).bin -savetiles $(filename).psgcompr -exit;	done;

	# -savetilemap assets/includes/blatilemap.stmcompr
	../bmp2tile/bmp2tile.exe assets/fritz.png -8x16 -savepalette assets/includes/blapal.bin -savetiles assets/includes/blatiles.psgcompr -exit
	../bmp2tile/bmp2tile.exe assets/title.png -savepalette assets/includes/titlepal.bin -savetiles assets/includes/titletiles.psgcompr -savetilemap assets/includes/titletilemap.stmcompr -exit
	$(FOLDER2C) assets/includes/ graphics

main.rel: graphics.c

%.rel:	%.c
	$(SDCC) -c -mz80 $< -o $@

main.ihx: main.rel graphics.rel
	$(SDCC) -o main.ihx -mz80 --data-loc 0xC000 --no-std-crt0 devkit/crt0_sms.rel main.rel devkit/SMSlib.lib graphics.rel

main.sms: main.ihx
	$(IHX2SMS) main.ihx main.sms
	$(EMULATOR) main.sms

clean:
	$(RM) *.sym *.rel *.noi *.map *.lst *.lk *.ihx graphics.* main.asm assets/includes/*
