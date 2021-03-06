/*
 * device driver for Conexant 2388x based TV cards
 * card-specific stuff.
 *
 * (c) 2003 Gerd Knorr <kraxel@bytesex.org> [SuSE Labs]
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#include "cx88.h"

/* ------------------------------------------------------------------ */
/* board config info                                                  */

struct cx88_board cx88_boards[] = {
	[CX88_BOARD_UNKNOWN] = {
		.name		= "UNKNOWN/GENERIC",
		.tuner_type     = UNSET,
		.input          = {{
			.type   = CX88_VMUX_COMPOSITE1,
			.vmux   = 0,
		},{
			.type   = CX88_VMUX_COMPOSITE2,
			.vmux   = 1,
		},{
			.type   = CX88_VMUX_COMPOSITE3,
			.vmux   = 2,
		},{
			.type   = CX88_VMUX_COMPOSITE4,
			.vmux   = 3,
		}},
	},
	[CX88_BOARD_HAUPPAUGE] = {
		.name		= "Hauppauge WinTV 34xxx models",
		.tuner_type     = UNSET,
		.input          = {{
			.type   = CX88_VMUX_TELEVISION,
			.vmux   = 0,
			//.gpio0  = 0xff03,
			.gpio0  = 0xff01,
		},{
			.type   = CX88_VMUX_DEBUG,
			.vmux   = 0,
			.gpio0  = 0xff00,
		},{
			.type   = CX88_VMUX_COMPOSITE1,
			.vmux   = 1,
			.gpio0  = 0xff02,
		}},
		.radio = {
			.type   = CX88_RADIO,
			.gpio0  = 0xff01,
		},
	},
	[CX88_BOARD_GDI] = {
		.name		= "GDI Black Gold",
		.tuner_type     = UNSET,
		.input          = {{
			.type   = CX88_VMUX_TELEVISION,
			.vmux   = 0,
		}},
	},
	[CX88_BOARD_PIXELVIEW] = {
		.name           = "PixelView",
		.tuner_type     = UNSET,
		.input          = {{
			.type   = CX88_VMUX_TELEVISION,
			.vmux   = 0,
		},{
			.type   = CX88_VMUX_COMPOSITE1,
			.vmux   = 1,
		},{
			.type   = CX88_VMUX_SVIDEO,
			.vmux   = 2,
		}},
	},
	[CX88_BOARD_ATI_WONDER_PRO] = {
		.name           = "ATI TV Wonder Pro",
		.tuner_type     = UNSET,
		.input          = {{
			.type   = CX88_VMUX_TELEVISION,
			.vmux   = 0,
		}},
	},
        [CX88_BOARD_WINFAST2000XP] = {
                .name           = "Leadtek Winfast 2000XP Expert",
                .tuner_type     = 38,
                .input          = {{
                        .type   = CX88_VMUX_TELEVISION,
                        .vmux   = 0,
                }},
                .radio = {
                        .type   = CX88_RADIO,
                },
        },
	[CX88_BOARD_AVERTV_303] = {
		.name           = "AverTV Studio 303 (M126)",
		.tuner_type     = TUNER_PHILIPS_PAL_DK,
		.input          = {{
			.type   = CX88_VMUX_TELEVISION,
			.vmux   = 0,
		}},
	},
	[CX88_BOARD_MSI_TVANYWHERE] = {
		.name           = "MSI TV-@nywhere Master",
		.tuner_type     = 33,
		.input          = {{
			.type   = CX88_VMUX_TELEVISION,
			.vmux   = 0,
		},{
                        .type   = CX88_VMUX_COMPOSITE1,
                        .vmux   = 1,
		},{
                        .type   = CX88_VMUX_SVIDEO,
                        .vmux   = 2,
                }},
                .radio = {
                        .type   = CX88_RADIO,
                },
	},
	[CX88_BOARD_WINFAST_DV2000] = {
                .name           = "Leadtek Winfast DV2000",
                .tuner_type     = 38,
                .input          = {{
                        .type   = CX88_VMUX_TELEVISION,
                        .vmux   = 0,
                }},
                .radio = {
                        .type   = CX88_RADIO,
                },
        },

};
const unsigned int cx88_bcount = ARRAY_SIZE(cx88_boards);

/* ------------------------------------------------------------------ */
/* PCI subsystem IDs                                                  */

struct cx88_subid cx88_subids[] = {
	{
		.subvendor = 0x0070,
		.subdevice = 0x3400,
		.card      = CX88_BOARD_HAUPPAUGE,
	},{
		.subvendor = 0x0070,
		.subdevice = 0x3401,
		.card      = CX88_BOARD_HAUPPAUGE,
	},{
		.subvendor = 0x14c7,
		.subdevice = 0x0106,
		.card      = CX88_BOARD_GDI,
	},{
		.subvendor = 0x14c7,
		.subdevice = 0x0107, /* with mpeg encoder */
		.card      = CX88_BOARD_GDI,
	},{
		.subvendor = PCI_VENDOR_ID_ATI,
		.subdevice = 0x00f8,
		.card      = CX88_BOARD_ATI_WONDER_PRO,
	},{
                .subvendor = 0x107d,
                .subdevice = 0x6611,
                .card      = CX88_BOARD_WINFAST2000XP,
	},{
		.subvendor = 0x107d,
                .subdevice = 0x6620,
                .card      = CX88_BOARD_WINFAST_DV2000,
        },{
		.subvendor = 0x1461,
		.subdevice = 0x000b,
		.card      = CX88_BOARD_AVERTV_303,
	},{
		.subvendor = 0x1462,
		.subdevice = 0x8606,
		.card      = CX88_BOARD_MSI_TVANYWHERE,
	}
};
const unsigned int cx88_idcount = ARRAY_SIZE(cx88_subids);

/* ----------------------------------------------------------------------- */
/* some hauppauge specific stuff                                           */

static struct {
        int  id;
        char *name;
} hauppauge_tuner[] __devinitdata = {
        { TUNER_ABSENT,        "" },
        { TUNER_ABSENT,        "External" },
        { TUNER_ABSENT,        "Unspecified" },
        { TUNER_PHILIPS_PAL,   "Philips FI1216" },
        { TUNER_PHILIPS_SECAM, "Philips FI1216MF" },
        { TUNER_PHILIPS_NTSC,  "Philips FI1236" },
        { TUNER_PHILIPS_PAL_I, "Philips FI1246" },
        { TUNER_PHILIPS_PAL_DK,"Philips FI1256" },
        { TUNER_PHILIPS_PAL,   "Philips FI1216 MK2" },
        { TUNER_PHILIPS_SECAM, "Philips FI1216MF MK2" },
        { TUNER_PHILIPS_NTSC,  "Philips FI1236 MK2" },
        { TUNER_PHILIPS_PAL_I, "Philips FI1246 MK2" },
        { TUNER_PHILIPS_PAL_DK,"Philips FI1256 MK2" },
        { TUNER_TEMIC_NTSC,    "Temic 4032FY5" },
        { TUNER_TEMIC_PAL,     "Temic 4002FH5" },
        { TUNER_TEMIC_PAL_I,   "Temic 4062FY5" },
        { TUNER_PHILIPS_PAL,   "Philips FR1216 MK2" },
        { TUNER_PHILIPS_SECAM, "Philips FR1216MF MK2" },
        { TUNER_PHILIPS_NTSC,  "Philips FR1236 MK2" },
        { TUNER_PHILIPS_PAL_I, "Philips FR1246 MK2" },
        { TUNER_PHILIPS_PAL_DK,"Philips FR1256 MK2" },
        { TUNER_PHILIPS_PAL,   "Philips FM1216" },
        { TUNER_PHILIPS_SECAM, "Philips FM1216MF" },
        { TUNER_PHILIPS_NTSC,  "Philips FM1236" },
        { TUNER_PHILIPS_PAL_I, "Philips FM1246" },
        { TUNER_PHILIPS_PAL_DK,"Philips FM1256" },
        { TUNER_TEMIC_4036FY5_NTSC, "Temic 4036FY5" },
        { TUNER_ABSENT,        "Samsung TCPN9082D" },
        { TUNER_ABSENT,        "Samsung TCPM9092P" },
        { TUNER_TEMIC_4006FH5_PAL, "Temic 4006FH5" },
        { TUNER_ABSENT,        "Samsung TCPN9085D" },
        { TUNER_ABSENT,        "Samsung TCPB9085P" },
        { TUNER_ABSENT,        "Samsung TCPL9091P" },
        { TUNER_TEMIC_4039FR5_NTSC, "Temic 4039FR5" },
        { TUNER_PHILIPS_FQ1216ME,   "Philips FQ1216 ME" },
        { TUNER_TEMIC_4066FY5_PAL_I, "Temic 4066FY5" },
        { TUNER_PHILIPS_NTSC,        "Philips TD1536" },
        { TUNER_PHILIPS_NTSC,        "Philips TD1536D" },
	{ TUNER_PHILIPS_NTSC,  "Philips FMR1236" }, /* mono radio */
        { TUNER_ABSENT,        "Philips FI1256MP" },
        { TUNER_ABSENT,        "Samsung TCPQ9091P" },
        { TUNER_TEMIC_4006FN5_MULTI_PAL, "Temic 4006FN5" },
        { TUNER_TEMIC_4009FR5_PAL, "Temic 4009FR5" },
        { TUNER_TEMIC_4046FM5,     "Temic 4046FM5" },
	{ TUNER_TEMIC_4009FN5_MULTI_PAL_FM, "Temic 4009FN5" },
	{ TUNER_ABSENT,        "Philips TD1536D_FH_44"},
	{ TUNER_LG_NTSC_FM,    "LG TPI8NSR01F"},
	{ TUNER_LG_PAL_FM,     "LG TPI8PSB01D"},
	{ TUNER_LG_PAL,        "LG TPI8PSB11D"},	
	{ TUNER_LG_PAL_I_FM,   "LG TAPC-I001D"},
	{ TUNER_LG_PAL_I,      "LG TAPC-I701D"}
};

static void __devinit hauppauge_eeprom(struct cx8800_dev *dev, u8 *eeprom_data)
{
	unsigned int blk2,tuner,radio,model;

	if (eeprom_data[0] != 0x84 || eeprom_data[2] != 0) {
		printk(KERN_WARNING "%s: Hauppauge eeprom: invalid\n",
		       dev->name);
		return;
	}

	/* Block 2 starts after len+3 bytes header */
	blk2 = eeprom_data[1] + 3;

	/* decode + use some config infos */
	model = eeprom_data[12] << 8 | eeprom_data[11];
	tuner = eeprom_data[9];
	radio = eeprom_data[blk2-1] & 0x01;
	
        if (tuner < ARRAY_SIZE(hauppauge_tuner))
                dev->tuner_type = hauppauge_tuner[tuner].id;
	if (radio)
		dev->has_radio = 1;
	
	printk(KERN_INFO "%s: hauppauge eeprom: model=%d, "
	       "tuner=%s (%d), radio=%s\n",
	       dev->name, model, hauppauge_tuner[tuner].name,
	       dev->tuner_type, radio ? "yes" : "no");
}

/* ----------------------------------------------------------------------- */
/* some GDI (was: Modular Technology) specific stuff                       */

static struct {
	int  id;
	int  fm;
	char *name;
} gdi_tuner[] = {
	[ 0x01 ] = { .id   = TUNER_ABSENT,
		     .name = "NTSC_M" },
	[ 0x02 ] = { .id   = TUNER_ABSENT,
		     .name = "PAL_B" },
	[ 0x03 ] = { .id   = TUNER_ABSENT,
		     .name = "BAL_I" },
	[ 0x04 ] = { .id   = TUNER_ABSENT,
		     .name = "PAL_D" },
	[ 0x05 ] = { .id   = TUNER_ABSENT,
		     .name = "SECAM" },

	[ 0x10 ] = { .id   = TUNER_ABSENT, .fm = 1, 
		     .name = "TEMIC_4049" },
	[ 0x11 ] = { .id   = TUNER_TEMIC_4136FY5,
		     .name = "TEMIC_4136" },
	[ 0x12 ] = { .id   = TUNER_ABSENT,
		     .name = "TEMIC_4146" },

	[ 0x20 ] = { .id   = TUNER_PHILIPS_FQ1216ME, .fm = 1,
		     .name = "PHILIPS_FQ1216_MK3" },
	[ 0x21 ] = { .id   = TUNER_ABSENT, .fm = 1,
		     .name = "PHILIPS_FQ1236_MK3" },
	[ 0x22 ] = { .id   = TUNER_ABSENT,
		     .name = "PHILIPS_FI1236_MK3" },
	[ 0x23 ] = { .id   = TUNER_ABSENT,
		     .name = "PHILIPS_FI1216_MK3" },
};

static void __devinit gdi_eeprom(struct cx8800_dev *dev, u8 *eeprom_data)
{
	char *name = (eeprom_data[0x0d] < ARRAY_SIZE(gdi_tuner))
		? gdi_tuner[eeprom_data[0x0d]].name : NULL;

	printk(KERN_INFO "%s: GDI: tuner=%s\n", dev->name,
	       name ? name : "unknown");
	if (NULL == name)
		return;
	dev->tuner_type = gdi_tuner[eeprom_data[0x0d]].id;
	dev->has_radio  = gdi_tuner[eeprom_data[0x0d]].fm;
}

/* ----------------------------------------------------------------------- */

static int
i2c_eeprom(struct i2c_client *c, unsigned char *eedata, int len)
{
	unsigned char buf;
	int err;

	c->addr = 0xa0 >> 1;
	buf = 0;
	if (1 != (err = i2c_master_send(c,&buf,1))) {
		printk(KERN_INFO "cx88: Huh, no eeprom present (err=%d)?\n",
		       err);
		return -1;
	}
	if (len != (err = i2c_master_recv(c,eedata,len))) {
		printk(KERN_WARNING "cx88: i2c eeprom read error (err=%d)\n",
		       err);
		return -1;
	}
#if 0
	for (i = 0; i < len; i++) {
		if (0 == (i % 16))
			printk(KERN_INFO "cx88 ee: %02x:",i);
		printk(" %02x",eedata[i]);
		if (15 == (i % 16))
			printk("\n");
	}
#endif
	return 0;
}

void __devinit cx88_card_setup(struct cx8800_dev *dev)
{
	static u8 eeprom[128];
		
	switch (dev->board) {
	case CX88_BOARD_HAUPPAUGE:
		if (0 == dev->i2c_rc)
			i2c_eeprom(&dev->i2c_client,eeprom,sizeof(eeprom));
		hauppauge_eeprom(dev,eeprom+8);
		break;
	case CX88_BOARD_GDI:
		if (0 == dev->i2c_rc)
			i2c_eeprom(&dev->i2c_client,eeprom,sizeof(eeprom));
		gdi_eeprom(dev,eeprom);
		break;
	}
}

/* ------------------------------------------------------------------ */

EXPORT_SYMBOL(cx88_boards);
EXPORT_SYMBOL(cx88_bcount);
EXPORT_SYMBOL(cx88_subids);
EXPORT_SYMBOL(cx88_idcount);
EXPORT_SYMBOL(cx88_card_setup);

/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
