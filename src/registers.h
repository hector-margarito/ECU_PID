#ifndef REGISTERS_H
#define REGISTERS_H

#define BASE_ADDR 0x40038000

#define PID_CR (BASE_ADDR + 0x00) // WR only
#define PID_CHER (BASE_ADDR + 0x04) // WR only
#define PID_CHDR (BASE_ADDR + 0x14) // WR only
#define PID_CHSR (BASE_ADDR + 0x18) // RD only
#define PID_CHGR1 (BASE_ADDR + 0x48) // RW
#define PID_CHGR2 (BASE_ADDR + 0x4C) // RW
#define PID_CHGR3 (BASE_ADDR + 0x50) // RW


#endif
