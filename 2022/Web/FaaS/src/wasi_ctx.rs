use std::time::Duration;

use wasi_common::{Poll, RngCore, WasiCtx, WasiSched};
use wasmtime_wasi::clocks_ctx;

struct LinearNG {
    state: u64,
}

impl RngCore for LinearNG {
    fn next_u32(&mut self) -> u32 {
        self.next_u64() as u32
    }

    fn next_u64(&mut self) -> u64 {
        self.state = self.state + 1;
        self.state
    }

    fn fill_bytes(&mut self, dest: &mut [u8]) {
        let mut left = dest;
        while left.len() >= 8 {
            let (l, r) = { left }.split_at_mut(8);
            left = r;
            let chunk: [u8; 8] = self.next_u64().to_le_bytes();
            l.copy_from_slice(&chunk);
        }
        let n = left.len();
        if n > 0 {
            let chunk: [u8; 8] = self.next_u64().to_le_bytes();
            left.copy_from_slice(&chunk[..n]);
        }
    }

    fn try_fill_bytes(&mut self, dest: &mut [u8]) -> Result<(), rand_core::Error> {
        self.fill_bytes(dest);
        Ok(())
    }
}

pub struct NullScheduler;

#[async_trait::async_trait]
impl WasiSched for NullScheduler {
    async fn poll_oneoff<'a>(&self, _poll: &mut Poll<'a>) -> Result<(), wasi_common::Error> {
        Ok(())
    }

    async fn sched_yield(&self) -> Result<(), wasi_common::Error> {
        Ok(())
    }

    async fn sleep(&self, _duration: Duration) -> Result<(), wasi_common::Error> {
        Ok(())
    }
}

pub fn build_wasi_ctx() -> WasiCtx {
    let random = Box::new(LinearNG { state: 0 });
    let scheduler = Box::new(NullScheduler {});
    let table = wasi_common::table::Table::new();

    WasiCtx::new(random, clocks_ctx(), scheduler, table)
}
