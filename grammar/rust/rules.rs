use frame;
use parser;
use symbol;

impl parser::ParseInput for (symbol::Any, frame::Any<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        match self
        {
            (symbol::Any::NotDelimeter(symbol), frame::Any::Text(frame)) => (symbol, frame).parse(ch),
            (symbol::Any::NotDelimeterMore(symbol), frame::Any::Text(frame)) => (symbol, frame).parse(ch),
            (symbol::Any::Value(symbol), frame::Any::Line(frame)) => (symbol, frame).parse(ch),
            (symbol::Any::Character(symbol), frame::Any::Line(frame)) => (symbol, frame).parse(ch),
            (symbol::Any::Key(symbol), frame::Any::Line(frame)) => (symbol, frame).parse(ch),
            (symbol::Any::Line(symbol), frame::Any::Line(frame)) => (symbol, frame).parse(ch),
            (symbol::Any::Line(symbol), frame::Any::Message(frame)) => (symbol, frame).parse(ch),
            _ => parser::Result::Mismatch
        }
    }
}

impl parser::ParseNoInput for (symbol::Any, frame::Any<'_>)
{
    fn parse(self) -> parser::Result
    {
        match self
        {
            (symbol::Any::Message(symbol), frame::Any::Message(frame)) => (symbol, frame).parse(),
            _ => parser::Result::Mismatch
        }
    }
}

impl parser::ParseInput for (symbol::NotDelimeter, frame::Text<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        let (symbol, frame) = self;
        if ch.is_alphanumeric() || ch == ' '
        {
            parser::Result::Finished { consume: true, frame: false }
        }
        else
        {
            parser::Result::Mismatch
        }
    }
}

impl parser::ParseInput for (symbol::NotDelimeterMore, frame::Text<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        let (symbol, frame) = self;
        if ch.is_alphanumeric() || ch == ' '
        {
            parser::Result::Consumed
        }
        else
        {
            parser::Result::Finished { consume: false, frame: true }
        }
    }
}

impl parser::ParseInput for (symbol::Value, frame::Line<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        parser::Result::ChangeFrame
    }
}

impl parser::ParseInput for (symbol::Character, frame::Line<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        let (symbol, frame) = self;
        match symbol.0
        {
            ':' => parser::Result::Finished { consume: true, frame: false },
            '\n' => parser::Result::Finished { consume: true, frame: true },
            _ => parser::Result::Mismatch
        }
    }
}

impl parser::ParseInput for (symbol::Key, frame::Line<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        parser::Result::ChangeFrame
    }
}

impl parser::ParseInput for (symbol::Line, frame::Line<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        parser::Result::AppendSymbols(vec![
            symbol::Any::Key(symbol::Key),
            symbol::Any::Character(symbol::Character(':')),
            symbol::Any::Value(symbol::Value),
            symbol::Any::Character(symbol::Character('\n'))
        ])
    }
}

impl parser::ParseInput for (symbol::Line, frame::Message<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        let (symbol, frame) = self;
        parser::Result::ChangeFrame
    }
}

impl parser::ParseInput for (symbol::Message, frame::Message<'_>)
{
    fn parse(self, ch: char) -> parser::Result
    {
        parser::Result::AppendSymbols(vec![
            symbol::Any::Line(symbol::Line),
            symbol::Any::Message(symbol::Message)
        ])
    }
}

impl parser::ParseNoInput for (symbol::Message, frame::Message<'_>)
{
    fn parse(self) -> parser::Result
    {
        parser::Result::Finished { consume: false, frame: false }
    }
}
