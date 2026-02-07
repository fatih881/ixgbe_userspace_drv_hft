#ifndef IXGBE_H
#define IXGBE_H

#include "base.h"
#include "hw.h"

static u32 ixgbe_read_reg(const struct hw *hw, const u32 reg) {
    return *((volatile u32 *)(hw->hw_addr + reg));
}

static void ixgbe_write_reg(const struct hw *hw, const u32 reg, const u32 val) {
    *((volatile u32 *)(hw->hw_addr + reg)) = val;
}
#endif