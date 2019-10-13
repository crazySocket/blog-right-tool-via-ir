
pub struct Text(pub String);

pub struct Line
{
    pub key: Text,
    pub value: Text
}

pub struct Message
{
    pub lines: Vec<Line>
}
