mod data;
mod display;
mod frame;
mod parser;
mod rules;
mod symbol;

use parser::ParseNoInput;

fn print_stack(stack: &Vec<symbol::Any>)
{
    println!("stack\n{{");
    for symbol in stack.iter().rev()
    {
        println!("\t{}", symbol);
    }
    println!("}}");
}

fn main()
{
    let input = "text:hello world\nasdf:qwer\n";
    let mut input_it = input.chars();
    let mut message = data::Message { lines: Vec::new() };

    let mut stack: Vec<symbol::Any> = Vec::new();
    let mut backtrace: Vec<frame::Any> = Vec::new();

    stack.push(symbol::Any::Message(symbol::Message));
    backtrace.push(frame::Any::Message(frame::Message(&mut message)));

    let mut next_char = input_it.next();

    loop
    {
        if stack.is_empty()
        {
            break;
        }

        print_stack(&stack);

        let symbol = stack.pop().unwrap();
        let frame = backtrace.pop().unwrap();

        println!("next symbol: {}", symbol);

        match (symbol, frame).parse()
        {
            parser::Result::Mismatch => {},
            parser::Result::Consumed =>
            {
                next_char = input_it.next();
            },
            parser::Result::Finished{consume, frame} =>
            {
                if consume
                {
                    next_char = input_it.next();
                }
                if frame
                {
                    backtrace.pop();
                }
            },
            parser::Result::AppendSymbols(_) => {},
            parser::Result::ChangeFrame => {}
        }
    }
}
