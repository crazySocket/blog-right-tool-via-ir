use symbol;

impl std::fmt::Display for symbol::Any
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        match self
        {
            symbol::Any::Message(symbol) => symbol.fmt(f),
            symbol::Any::Line(symbol) => symbol.fmt(f),
            symbol::Any::Key(symbol) => symbol.fmt(f),
            symbol::Any::Value(symbol) => symbol.fmt(f),
            symbol::Any::Character(symbol) => symbol.fmt(f),
            symbol::Any::NotDelimeter(symbol) => symbol.fmt(f),
            symbol::Any::NotDelimeterMore(symbol) => symbol.fmt(f)
        }
    }
}

impl std::fmt::Display for symbol::Message
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        write!(f, "symbol::Message")
    }
}

impl std::fmt::Display for symbol::Line
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        write!(f, "symbol::Line")
    }
}

impl std::fmt::Display for symbol::Key
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        write!(f, "symbol::Key")
    }
}

impl std::fmt::Display for symbol::Value
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        write!(f, "symbol::Value")
    }
}

impl std::fmt::Display for symbol::Character
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        write!(f, "Symbol::Character '{}'", self.0)
    }
}

impl std::fmt::Display for symbol::NotDelimeter
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        write!(f, "Symbol::NotDelimeter")
    }
}

impl std::fmt::Display for symbol::NotDelimeterMore
{
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result
    {
        write!(f, "Symbol::NotDelimeterMore")
    }
}
