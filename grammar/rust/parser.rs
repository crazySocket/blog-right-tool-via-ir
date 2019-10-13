
use symbol;

pub enum Result
{
    Mismatch,
    Consumed,
    Finished
    {
        consume: bool,
        frame: bool
    },
    AppendSymbols(Vec<symbol::Any>),
    ChangeFrame,
}

pub trait ParseInput
{
    fn parse(self, ch: char) -> Result;
}

pub trait ParseNoInput
{
    fn parse(self) -> Result;
}
