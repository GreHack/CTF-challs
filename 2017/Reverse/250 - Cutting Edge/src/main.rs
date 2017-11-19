// Copyright 2017 Thomas Gerbet.  All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

use std::{thread, time};
use std::os::raw::c_char;
use std::ffi::CString;
use std::ffi::CStr;

const AUTOMATA: &'static [[u8; 2]] = &[
    [2, 1], [18, 3], [4, 2], [3, 25], [7, 20],
    [8, 3], [23, 5], [24, 6], [19, 9], [0, 10],
    [11, 13], [11, 12], [10, 13], [13, 14], [16, 26],
    [17, 16], [15, 6], [19, 6], [2, 4], [19, 3],
    [21, 0], [20, 7], [20, 11], [12, 22], [5, 10],
    [25, 26], [26, 25]
];

const OUTPUT: &'static [[char; 2]] = &[
    ['🔥', 'G'], ['H', '🍺'], ['{', '7'], ['🎉', '🎉'], ['f', '🦊'],
    ['u', '🐪'], ['🎃', 'u'], ['🍷', 'u'], ['🥃', '_'], ['🍾', 'w'],
    ['👽', 'e'], ['💀', '👹'], ['💥', '🙈'], ['🦄', 'b'], ['_', '🐃'],
    ['e', '🚒'], ['t', '🍸'], ['c', '🍹'], ['1', '🍻'], ['h', '}'],
    ['o', '🐶'], ['n', 't'], ['i', 's'], ['🐺', 'l'], ['r', 'o'],
    ['🐟', '🐆'], ['🐩', '🐦']
];

const SIZE: u8 = 20;

fn main() {
}

#[no_mangle]
pub fn get_data_js(c_key: *const c_char) -> *mut c_char {
    let key = string_safe_from_c(c_key);
    CString::new(get_value(key)).unwrap().into_raw()
}

fn string_safe_from_c(c_str: *const c_char) -> String {
    unsafe {
        CStr::from_ptr(c_str).to_string_lossy().into_owned()
    }
}

fn get_value(key: String) -> String {
    match key.parse::<u32>() {
        Ok(ki) => deobfuscate(ki, SIZE),
        Err(__e) => String::from("Expected an uint"),
    }
}

// Idea from https://antonioparata.blogspot.nl/2017/09/using-mealy-automata-for-string.html
fn deobfuscate(key: u32, size: u8) -> String {
    let mut secret = String::with_capacity(size as usize);
    let mut state = 0u8;
    for i in (0..(size-1)).rev() {
        let v = (key >> i) & 1;
        if v == 1 { // Let's waste time
            thread::sleep(time::Duration::from_millis(111));
        }
        secret.push(OUTPUT[state as usize][v as usize]);
        state = AUTOMATA[state as usize][v as usize];
    }
    secret
}
