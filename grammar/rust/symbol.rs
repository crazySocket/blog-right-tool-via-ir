
pub struct Message;
pub struct Line;
pub struct Key;
pub struct Value;
pub struct Character(pub char);
pub struct NotDelimeter;
pub struct NotDelimeterMore;

pub enum Any
{
    Message(Message),
    Line(Line),
    Key(Key),
    Value(Value),
    Character(Character),
    NotDelimeter(NotDelimeter),
    NotDelimeterMore(NotDelimeterMore)
}
