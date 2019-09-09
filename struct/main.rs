
#[derive(Debug)]
struct Point
{
    x: f32,
    y: f32
}

impl Point
{
    fn north(self, d: f32) -> Point
    {
        Point { x: self.x, y: self.y + d}
    }
    fn south(self, d: f32) -> Point
    {
        self.north(-d)
    }
    fn east(self, d: f32) -> Point
    {
        Point { x: self.x + d, y: self.y }
    }
    fn west(self, d: f32) -> Point
    {
        self.east(-d)
    }
}

macro_rules! print_point
{
    ($expression:expr) =>
    {
        println!("{:?}", $expression);
    }
}

fn main()
{
    let point = Point { x: 0.0, y: 0.0 };
    print_point!(point.north(1.0).east(1.0));
}
