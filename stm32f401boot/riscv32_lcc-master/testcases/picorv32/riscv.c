#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "riscv.h"
 
void raise_exception(uint32_t cause,uint32_t tval)
{
   
}

 
int load(rv32 *riscv,uint32_t address, uint32_t *result, int width){
uint32_t physical = address;
switch (width){
        case 4:
            //*result=r32m(physical);
            {
                uint8_t dato[4];
                uint32_t res=0;
                for (int i = 0; i < 4; ++i){
                     dato[i]=riscv->memory[physical+i];//read_vm(physical+i);
                } 
                *result= *(uint32_t *)dato;
            }
            break;
        case 2:
        {
            //*result=r16m(physical);

            uint8_t dato[2];
            uint16_t res=0;
            for (int i = 0; i < 2; ++i){
                 dato[i]=riscv->memory[physical+i];//read_vm(physical+i);
            } 
            *result= *(uint16_t *)dato;
        }
        break;
        case 1:
        {
            *result=riscv->memory[physical];
        }
        break;
        default:
            printf("error load\n");
            return 1;
    }
    return 0;
}
int store(rv32 *riscv,uint32_t address, uint32_t data, int width){
    uint32_t physical = address;
    if (address == UART_TX_ADDR) {
         printf("%c", data);
         return 0;
    }

    switch (width){
            case 4:
                {
                     uint8_t dato[4];
                     dato[0] = data & 0xff;
                     dato[1] = (data >> 8) & 0xff;
                     dato[2] = (data >> 16) & 0xff;
                     dato[3] = (data >> 24) & 0xff;
                     for (int i = 0; i < 4; ++i)
                     {
                        riscv->memory[physical + i]=dato[i];
                     }
                }
                break;
            case 2:
            {
                uint8_t dato[2];
                dato[0] = data & 0xff;
                dato[1] = (data >> 8) & 0xff;
                for (int i = 0; i < 2; ++i)
                {
                   riscv->memory[physical + i]=dato[i];
                }
            }
            break;
            case 1:
            {
                riscv->memory[physical]=data & 0xff;
            }
            break;
            default:
                printf("error load\n");
                return 1;
        }
        return 0;
}
uint32_t get_op(rv32 *core){
uint8_t dato[4];
//addr=(size_t)addr;//importante el size_t
dato[0] = core->memory[core->pc];
dato[1] = core->memory[core->pc+1];
dato[2] = core->memory[core->pc+2];
dato[3] = core->memory[core->pc+3];
return *(uint32_t *)dato;
}

/**************************************************/
int32_t div32(int32_t a, int32_t b)
{
    if (b == 0) {
        return -1;
    } else if (a == ((int32_t)1 << (XLEN - 1)) && b == -1) {
        return a;
    } else {
        return a / b;
    }
}

uint32_t divu32(uint32_t a, uint32_t b)
{
    if (b == 0) {
        return -1;
    } else {
        return a / b;
    }
}

int32_t rem32(int32_t a, int32_t b)
{
    if (b == 0) {
        return a;
    } else if (a == ((int32_t)1 << (XLEN - 1)) && b == -1) {
        return 0;
    } else {
        return a % b;
    }
}

uint32_t remu32(uint32_t a, uint32_t b)
{
    if (b == 0) {
        return a;
    } else {
        return a % b;
    }
}

 uint32_t mulh32(int32_t a, int32_t b)
{
    return ((int64_t)a * (int64_t)b) >> 32;
}

 uint32_t mulhsu32(int32_t a, uint32_t b)
{
    return ((int64_t)a * (int64_t)b) >> 32;
}

uint32_t mulhu32(uint32_t a, uint32_t b)
{
    return ((int64_t)a * (int64_t)b) >> 32;
}
 

void execute_instruction(rv32 *riscv)
{
 
    uint32_t opcode, rd, rs1, rs2, funct3,insn;
    int32_t imm=0, cond, err;
    uint32_t addr, val, val2,temp;
    
    
    insn= get_op(riscv);

    opcode = insn & 0x7f;
    rd = (insn >> 7) & 0x1f;
    rs1 = (insn >> 15) & 0x1f;
    rs2 = (insn >> 20) & 0x1f;
    if(riscv->debug_inst){
       printf("instruction: %08x , pc: %08x\n", insn,riscv->pc);
       
    }
    switch(opcode) {
    case 0x37: /* lui */
        if (rd != 0)
            riscv->reg[rd] = (int32_t)(insn & 0xfffff000);

        if(riscv->debug_inst){
        printf("lui\n");
        printf("rd: %i , rd val: %x\n", rd, riscv->reg[rd]);
        }
        
        riscv->pc += 4;
        break;
    case 0x17: /* auipc */
        if (rd != 0)
            riscv->reg[rd] = (int32_t)(riscv->pc + (int32_t)(insn & 0xfffff000));

        
        riscv->pc += 4;
        break;
    case 0x6f: /* jal */
        imm = ((insn >> (31 - 20)) & (1 << 20)) |
              ((insn >> (21 - 1)) & 0x7fe) |
              ((insn >> (20 - 11)) & (1 << 11)) |
              (insn & 0xff000);
        
        
         imm = (imm << 11) >> 11;

        if (rd != 0)
            riscv->reg[rd] = riscv->pc + 4;

        if(riscv->debug_inst){
           printf("jal: %x\n",riscv->pc + (uint32_t)imm);
        }
        riscv->pc =  (int32_t)(riscv->pc + imm);
         
        
        if(riscv->debug_inst){
           printf("jal: %x,%x\n",riscv->pc,imm);
        }
        break;
    case 0x67: /* jalr */
        imm = (int32_t)insn >> 20;
        val = riscv->pc + 4;
        riscv->pc = (int32_t)(riscv->reg[rs1] + imm) & ~1;
        
        //jump(((riscv->reg[rs1] + imm)),&riscv->pc);
        
        if (rd != 0)
            riscv->reg[rd] = val;
        break;
    case 0x63:
        funct3 = (insn >> 12) & 7;
        switch(funct3 >> 1) {
        case 0: /* beq/bne */
            cond = (riscv->reg[rs1] == riscv->reg[rs2]);
              if(riscv->debug_inst){
                 printf("compare: %i , a4: %i , a5: %i\n",cond,riscv->reg[rs1],riscv->reg[rs2]);
              }
            break;
        case 2: /* blt/bge */
            cond = ((int32_t)riscv->reg[rs1] < (int32_t)riscv->reg[rs2]);
            break;
        case 3: /* bltu/bgeu */
            cond = (riscv->reg[rs1] < riscv->reg[rs2]);
            break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        cond ^= (funct3 & 1);
        if (cond) {
            imm = ((insn >> (31 - 12)) & (1 << 12)) |
                  ((insn >> (25 - 5)) & 0x7e0) |
                  ((insn >> (8 - 1)) & 0x1e) |
                  ((insn << (11 - 7)) & (1 << 11));
            imm = (imm << 19) >> 19;
            riscv->pc = (int32_t)(riscv->pc + imm);
            
            break;
        }else riscv->pc += 4;
        
        break;
    case 0x03: /* load */
        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        addr = riscv->reg[rs1] + imm;

        switch(funct3) {
        case 0: /* lb */
        {
            uint32_t rval;
            
           /* if (target_read_u8(&rval, addr)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }*/
            if (load(riscv,addr,&rval,1)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            val = (int8_t)rval;
        }
        break;
        case 1: /* lh */
        {
            uint32_t rval;
            /*if (target_read_u16(&rval, addr)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }*/

            if (load(riscv,addr,&rval,2)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            val = (int16_t)rval;
        }
        break;
        case 2: /* lw */
        {
            
            uint32_t rval;
            /*if (target_read_u32(&rval, addr)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }*/

            if (load(riscv,addr,&rval,4)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            val = (int32_t)rval;
        }
        break;
        case 4: /* lbu */
        {
            
            uint32_t rval;

           /* if (target_read_u8(&rval, addr)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }*/
            if (load(riscv,addr,&rval,1)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            val = rval & 0xff;
        }
        break;
        case 5: /* lhu */
        {
            uint32_t rval;
           /* if (target_read_u16(&rval, addr)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }*/
            if (load(riscv,addr,&rval,2)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            val = rval & 0xffff;
        }
        break;
        case 6: /* lwu */
        {
            uint32_t rval;
           /* if (target_read_u16(&rval, addr)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }*/
            if (load(riscv,addr,&rval,4)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            val = rval;
        }
        break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
        if (rd != 0)
            riscv->reg[rd] = val;

       if(riscv->debug_inst){
         printf("load");
        printf("addr: %x\n",addr );
        printf("rd: %i , rd val: %x\n", rd, riscv->reg[rd]);
        printf("rs1: %i, %i\n",rs1,riscv->reg[rs1]);
        printf("rs2: %i, %i\n",rs2,riscv->reg[rs2]);
        printf("imm: %i\n",imm );
        printf("val: %x\n", val);
        }
        riscv->pc += 4;
        break;
    case 0x23: /* store */
        funct3 = (insn >> 12) & 7;
        imm = rd | ((insn >> (25 - 5)) & 0xfe0);
        imm = (imm << 20) >> 20;
        addr = riscv->reg[rs1] + imm;
        val = riscv->reg[rs2];
        //printf("addr: %x\n",addr);
        switch(funct3) {
        case 0: /* sb */
            if (store(riscv,addr, val,1)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            
            break;
        case 1: /* sh */
            if (store(riscv,addr, val,2)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            }
            break;
        case 2: /* sw */
            if (store(riscv,addr, val,4)) {
                raise_exception(riscv->pending_exception, riscv->pending_tval);
                return;
            } 
            
            break;
        default:
            raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
            return;
        }
       if(riscv->debug_inst){
            printf("store ");
	        printf("addr: %x\n",addr );
	        printf("rd: %i , rd val: %x\n", rd, riscv->reg[rd]);
	        printf("rs1: %i, %i\n",rs1,riscv->reg[rs1]);
	        printf("rs2: %i, %i\n",rs2,riscv->reg[rs2]);
	        printf("imm: %i\n",imm );
	        printf("val: %x\n", val);
        }
        riscv->pc += 4;
        //printf("pc: %08x\n",riscv->pc ); 
        break;
    case 0x13:
        funct3 = (insn >> 12) & 7;
        imm = (int32_t)insn >> 20;
        switch(funct3) {
        case 0: /* addi */
            val = (int32_t)(riscv->reg[rs1] + imm) ;//& 0x0000ffff;
              if(riscv->debug_inst){
                 printf("addi/li : val: %x , imm: %x , reg: %x\n",val,imm,riscv->reg[rd]);
                   
          }
            
            break;
        case 1: /* slli */
            if ((imm & ~(XLEN - 1)) != 0) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            val = (int32_t)(riscv->reg[rs1] << (imm & (XLEN - 1)));
            break;
        case 2: /* slti */
            val = (int32_t)riscv->reg[rs1] < (int32_t)imm;
            break;
        case 3: /* sltiu */
            val = riscv->reg[rs1] < (uint32_t)imm;
            break;
        case 4: /* xori */
            val = riscv->reg[rs1] ^ imm;
            break;
        case 5: /* srli/srai */
            if ((imm & ~((XLEN - 1) | 0x400)) != 0) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            if (imm & 0x400)
                val = (int32_t)riscv->reg[rs1] >> (imm & (XLEN - 1));
            else
                val = (int32_t)((uint32_t)riscv->reg[rs1] >> (imm & (XLEN - 1)));
            break;
        case 6: /* ori */
            val = riscv->reg[rs1] | imm;
            break;
        default:
        case 7: /* andi */
            val = riscv->reg[rs1] & imm;
            break;
        }
        if (rd != 0)
            riscv->reg[rd] = val;
        
        riscv->pc += 4;
        break;
    case 0x33:
        imm = insn >> 25;
        val = riscv->reg[rs1];
        val2 = riscv->reg[rs2];
        if (imm == 1) {
            funct3 = (insn >> 12) & 7;
            switch(funct3) {
            case 0: /* mul */
                val = (int32_t)((int32_t)val * (int32_t)val2);
                break;
            case 1: /* mulh */
                val = (int32_t)mulh32(val, val2);
                break;
            case 2:/* mulhsu */
                val = (int32_t)mulhsu32(val, val2);
                break;
            case 3:/* mulhu */
                val = (int32_t)mulhu32(val, val2);
                break;
            case 4:/* div */
                val = div32(val, val2);
                break;
            case 5:/* divu */
                val = (int32_t)divu32(val, val2);
                break;
            case 6:/* rem */
                val = rem32(val, val2);
                break;
            case 7:/* remu */
                val = (int32_t)remu32(val, val2);
                break;
            default:
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
        } else {
            if (imm & ~0x20) {
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
            funct3 = ((insn >> 12) & 7) | ((insn >> (30 - 3)) & (1 << 3));
            switch(funct3) {
            case 0: /* add */
               
                val = (int32_t)(val + val2);
                break;
            case 0 | 8: /* sub */
                val = (int32_t)(val - val2);
                break;
            case 1: /* sll */
                val = (int32_t)(val << (val2 & (XLEN - 1)));
                break;
            case 2: /* slt */
                val = (int32_t)val < (int32_t)val2;
                break;
            case 3: /* sltu */
                val = val < val2;
                break;
            case 4: /* xor */
                val = val ^ val2;
                break;
            case 5: /* srl */
                val = (int32_t)((uint32_t)val >> (val2 & (XLEN - 1)));
                break;
            case 5 | 8: /* sra */
                val = (int32_t)val >> (val2 & (XLEN - 1));
                break;
            case 6: /* or */
                val = val | val2;
                break;
            case 7: /* and */
                val = val & val2;
                break;
            default:
                raise_exception(CAUSE_ILLEGAL_INSTRUCTION, insn);
                return;
            }
        }
        if (rd != 0)
            riscv->reg[rd] = val;

        riscv->pc += 4;
        break;
        case 0x73:
        funct3 = (insn >> 12) & 7;
        imm = insn >> 20;
        if (funct3 & 4)
            val = rs1;
        else
            val = riscv->reg[rs1];
        funct3 &= 3;

        switch(funct3) {
              case 0:
                switch(riscv->reg[17]){
                       
                      case 10://exit
                        printf("EXIT RISCV32\n");
                        riscv->running=0;
                      break;
                      
                }
            break;
        }
        riscv->pc += 4;
        
       break;
    }
    /******************************************/
   
}


int a;
void init_soc(rv32 *core){
  printf("RISCV32IM VIRTUAL CPU\n"); 
  
  while(core->running){

    if(core->next){
       scanf("%i",&a);
    }
    execute_instruction(core);
  }
}

/********************************************************************/
void openrom(rv32 *core,char *str){
    FILE * file = NULL;
    unsigned long lsize = 0;
    core->memory=(char*)malloc(150000);
    core->next=0;
    core->debug_inst=0;
    core->running=1;
    core->pc=0;
    printf("\n\nopen--%s\n\n",str);

    file = fopen(str, "rb");
    if(file == NULL){
       printf("Could not open file!");
       exit(-1);
    }
	fseek(file, 0, SEEK_END);
    lsize = ftell(file);
    rewind(file);
	fread(core->memory, sizeof(char), lsize, file);
	fclose(file);
}
/********************************************************************/
void main(int argc, char *argv[])
{
	char *exec;
	char *config;
 
	if(argc>1){
	   exec=argv[1];
	   
	   openrom(&riscv,exec);
       ///openrom(&riscv2,exec);
	}else{
	   printf("fail\n");
	   exit(1);
	}

    while(1){
        execute_instruction(&riscv);
       // execute_instruction(&riscv2);
    }
	//init_soc(&riscv);//init cpu
}