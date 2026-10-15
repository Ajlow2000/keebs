#![no_main]
#![no_std]

use defmt_rtt as _;
use panic_probe as _;
use rmk::macros::rmk_peripheral;

#[rmk_peripheral(id = 0)]
mod keyboard {}
