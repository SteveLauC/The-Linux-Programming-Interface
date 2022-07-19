use std::io::{Error, Read, Write};

pub trait VectorInput {
    fn read_v(&mut self, v: &mut [&mut [u8]]) -> Result<usize, Error>;
}

pub trait VectorOutput {
    fn write_v(&mut self, v: &[&[u8]]) -> Result<usize, Error>;
}
impl<T> VectorInput for T
where
    T: Read,
{
    fn read_v(&mut self, v: &mut [&mut [u8]]) -> Result<usize, Error> {
        let mut num_read: usize = 0;
        for buf in v.iter_mut() {
            num_read += self.read(*buf)?;
        }

        Ok(num_read)
    }
}

impl<T: Write> VectorOutput for T {
    fn write_v(&mut self, v: &[&[u8]]) -> Result<usize, Error> {
        let mut num_written: usize = 0;
        for buf in v.iter() {
            num_written += self.write(*buf)?;
        }
        Ok(num_written)
    }
}
