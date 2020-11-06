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


package org.miracl.core.TWEEDLEDEE;

public class ROM
{

// Base  bits= 29
public static final int[] Modulus= {0x1,0x1657EA0,0x18A1B261,0x15424ED2,0x38,0x0,0x0,0x0,0x400000};
public static final int[] R2modp= {0x3FF4,0xE8E1080,0xA9C2909,0xC128269,0x89F8301,0x19607541,0x1F4C0E6F,0x3443009,0x22B644};
public static final int[] ROI= {0x69D57EC,0x5E991A4,0x213EFC7,0x10FBA370,0x72930A2,0x5B22D4C,0x102FC76F,0xA21B807,0x113EFC};
public static final int[] SQRTm3= {0x91F49FA,0x17E6AD31,0x19F5AE40,0x8F54DB0,0x3FFCD01,0xB5FF973,0x12C43D6C,0xE878126,0x2D8CDA};
public static final int MConst= 0x1FFFFFFF;

//*** rom curve parameters *****
public static final int CURVE_Cof_I= 1;
public static final int[] CURVE_Cof= {0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
public static final int CURVE_B_I= 5;
public static final int[] CURVE_B= {0x5,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
public static final int[] CURVE_Order= {0x1,0xA032710,0xFD66E68,0x15424ED8,0x38,0x0,0x0,0x0,0x400000};
public static final int[] CURVE_Gx= {0x0,0x1657EA0,0x18A1B261,0x15424ED2,0x38,0x0,0x0,0x0,0x400000};
public static final int[] CURVE_Gy= {0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
public static final int[] CURVE_HTPC= {0xD69C848,0x1C30E5ED,0xBBDA1CC,0x180EA8CE,0xE9704A,0x1172DE0D,0x1C820B1A,0x1BC54C92,0x3AC188};
}
