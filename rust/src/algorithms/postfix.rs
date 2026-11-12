use crate::structures::stack::Stack;

pub struct PostfixConverter;

fn is_operator(symbol: char) -> bool {
    matches!(symbol, '+' | '-' | '*' | '/')
}

fn priority(symbol: char) -> u8 {
    match symbol {
        '+' | '-' => 1,
        '*' | '/' => 2,
        _ => 0,
    }
}

impl PostfixConverter {
    pub fn convert(&self, expression: &str) -> String {
        let mut result = String::new();
        let mut operators = Stack::new();
        for symbol in expression.chars() {
            if !is_operator(symbol) && symbol != '(' && symbol != ')' {
                result.push(symbol);
            } else if symbol == '(' {
                operators.push(symbol);
            } else if symbol == ')' {
                while let Some(&top) = operators.peek() {
                    if top == '(' {
                        break;
                    }
                    result.push(operators.pop().unwrap());
                }
                operators.pop();
            } else {
                while let Some(&top) = operators.peek() {
                    if is_operator(top) && priority(top) >= priority(symbol) {
                        result.push(operators.pop().unwrap());
                    } else {
                        break;
                    }
                }
                operators.push(symbol);
            }
        }
        while let Some(symbol) = operators.pop() {
            result.push(symbol);
        }
        result
    }

    pub fn evaluate(&self, postfix: &str) -> Result<f64, String> {
        let mut numbers = Stack::new();
        for symbol in postfix.chars() {
            if let Some(digit) = symbol.to_digit(10) {
                numbers.push(digit as f64);
            } else if is_operator(symbol) {
                let right = numbers.pop().ok_or("Ошибка в выражении.")?;
                let left = numbers.pop().ok_or("Ошибка в выражении.")?;
                let value = match symbol {
                    '+' => left + right,
                    '-' => left - right,
                    '*' => left * right,
                    '/' => left / right,
                    _ => unreachable!(),
                };
                numbers.push(value);
            }
        }
        let result = numbers.pop().ok_or("Ошибка в выражении.")?;
        if numbers.pop().is_some() {
            return Err("Ошибка в выражении.".to_string());
        }
        Ok(result)
    }
}
