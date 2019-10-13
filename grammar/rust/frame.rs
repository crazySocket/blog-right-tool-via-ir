use data;

pub struct Text<'a>(pub &'a data::Text);
pub struct Line<'a>(pub &'a data::Line);
pub struct Message<'a>(pub &'a data::Message);

pub enum Any<'a>
{
    Text(Text<'a>),
    Line(Line<'a>),
    Message(Message<'a>)
}
