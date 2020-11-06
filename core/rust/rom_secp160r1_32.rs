/*
 * Copyright (c) 2012-2020 MIRACL UK Ltd.
 *
 * This file is part of MIRACL Core
 * (see https://github.com/miracl/core).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Fixed Data in ROM - Field and Curve parameters */

use crate::arch::Chunk;
use crate::secp160r1::big::NLEN;

// Base Bits= 29
pub const MODULUS:[Chunk;NLEN]=[0x1FFFFFFF,0x1FFFFFFB,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x7FFF];
pub const R2MODP:[Chunk;NLEN]=[0x10000000,0x0,0x4,0x8,0x0,0x0];
pub const ROI:[Chunk;NLEN]=[0x1FFFFFFE,0x1FFFFFFB,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x7FFF];
pub const MCONST:Chunk=0x1;

pub const CURVE_COF_I:isize = 1;
pub const CURVE_COF:[Chunk;NLEN]=[0x1,0x0,0x0,0x0,0x0,0x0];
pub const CURVE_B_I:isize = 0;
pub const CURVE_B:[Chunk;NLEN]=[0x565FA45,0xEA6A56E,0xB3E27E0,0x1AF516CB,0x1BEFC54B,0xE4B];
pub const CURVE_ORDER:[Chunk;NLEN]=[0xA752257,0x93D769E,0x7D323E,0x0,0x0,0x8000];
pub const CURVE_GX:[Chunk;NLEN]=[0x13CBFC82,0x61C5DC8,0x191A625A,0xAE6508C,0xB5688EF,0x254B];
pub const CURVE_GY:[Chunk;NLEN]=[0x1AC5FB32,0x11A89BB,0x17324481,0x1128FAB3,0x2855316,0x11D3];
pub const CURVE_HTPC:[Chunk;NLEN]=[0x25BC0DA,0xED7D79D,0x139A9CC1,0x14C20DF5,0xBD02CD5,0xF28];
