#ifndef MAILBOX_MAP_H
#define MAILBOX_MAP_H

/* Mailbox Registers */
/* Table 2-5 Mailbox v2.1 Product Guide */
#define MAILBOX_WRITE_REG                  (0x0)
#define MAILBOX_RESERVED_0_REG             (0x4)
#define MAILBOX_READ_REG                   (0x8)
#define MAILBOX_RESERVED_1_REG             (0xC)
#define MAILBOX_STATUS_REG                 (0x10)
#define MAILBOX_ERROR_REG                  (0x14)
#define MAILBOX_SEND_INT_REG               (0x18)
#define MAILBOX_RECV_INT_REG               (0x1C)
#define MAILBOX_INT_STATUS_REG             (0x20)
#define MAILBOX_INT_ENABLE_REG             (0x24)
#define MAILBOX_INT_PENDING_REG            (0x28)
#define MAILBOX_CONTROL_REG                (0x2C)
#define MAILBOX_RESERVED_2_REG             (0x30)
#define MAILBOX_RESERVED_3_REG             (0x34)
#define MAILBOX_RESERVED_4_REG             (0x38)
#define MAILBOX_RESERVED_5_REG             (0x3C)

/* Status bits */
/* Table 2-11 Mailbox v2.1 Product Guide */
#define MAILBOX_STATUS_FIFO_EMPTY_BIT       (0x1)
#define MAILBOX_STATUS_FIFO_FULL_BIT        (0x2)
#define MAILBOX_STATUS_FIFO_RTA_BIT         (0x4)
#define MAILBOX_STATUS_FIFO_STA_BIT         (0x8)

/* Error bits */
/* Table 2-13 Mailbox v2.1 Product Guide */
#define MAILBOX_ERROR_FULL_BIT              (0x1)
#define MAILBOX_ERROR_EMPTY_BIT             (0x2)

/* Interrupt bits */
/* Table 2-14 - 2-23 Mailbox v2.1 Product Guide */
#define MAILBOX_INT_STA_BIT                 (0x1)
#define MAILBOX_INT_RTA_BIT                 (0x2)
#define MAILBOX_INT_ERROR_BIT               (0x4)

/* Control bits */
/* Table 2-24 Mailbox v2.1 Product Guide */
#define MAILBOX_CONTROL_RESET_SEND_FIFO_BIT (0x1)
#define MAILBOX_CONTROL_RESET_RECV_FIFO_BIT (0x2)


#endif /* MAILBOX_MAP_H */
