#include "clock.h"
#include "kernel.h"
#include "xosc.h"
#include "rp2040.h"




void init_clocks() {
        /*  Disable resus that may be enabled from previous software */
        write_reg(CLOCKS_CLK_SYS_RESUS_CTRL, 0);

        xosc_init();
        
        write_reg(CLOCKS_BASE+0x48, 0x880); // clk_peri enble
        /* source of clk_ref to xosc */
        write_reg_op(CLOCKS_REF_CTRL, 0x2, OP_SET);
        /* reset the source of clk_sys to clk_ref which references xosc */
        write_reg(CLOCKS_SYS_CTRL, 0x0);
}
