use std::env;
use std::fs;
use std::io::{self, Write};

use crate::algorithms::employee_counter::EmployeeCounter;
use crate::algorithms::genome::GenomeComparator;
use crate::algorithms::huffman::Huffman;
use crate::algorithms::lru::LruCache;
use crate::algorithms::postfix::PostfixConverter;
use crate::algorithms::stress::StressChecker;
use crate::structures::hash_table::HashMethod;
use crate::structures::set::Set;

enum Task {
    Postfix,
    Genom,
    Stress,
    Huffman,
    Manager,
    Lru,
}

fn get_task(name: &str) -> Option<Task> {
    match name {
        "POSTFIX" => Some(Task::Postfix),
        "GENOM" => Some(Task::Genom),
        "STRESS" => Some(Task::Stress),
        "HUFFMAN" => Some(Task::Huffman),
        "MANAGER" => Some(Task::Manager),
        "LRU" => Some(Task::Lru),
        _ => None,
    }
}

fn read_line() -> Result<String, String> {
    eprint!("-> ");
    io::stderr().flush().map_err(|error| error.to_string())?;
    let mut line = String::new();
    if io::stdin().read_line(&mut line).map_err(|error| error.to_string())? == 0 {
        return Err("Не удалось прочитать ввод.".to_string());
    }
    Ok(line.trim_end_matches(['\r', '\n']).to_string())
}

fn read_number(text: &str) -> Result<usize, String> {
    text.trim().parse::<usize>().map_err(|_| "Ожидалось неотрицательное число.".to_string())
}

fn run_postfix() -> Result<(), String> {
    let expression = read_line()?;
    let converter = PostfixConverter;
    let postfix = converter.convert(&expression);
    let value = converter.evaluate(&postfix)?;
    println!("-> {} = {}", postfix, value);
    Ok(())
}

fn run_genom() -> Result<(), String> {
    let first = read_line()?;
    let second = read_line()?;
    let comparator = GenomeComparator::new(first.trim(), second.trim());
    println!("{}", comparator.closeness());
    Ok(())
}

fn run_stress() -> Result<(), String> {
    let count = read_number(&read_line()?)?;
    let mut checker = StressChecker::new();
    for _ in 0..count {
        let word = read_line()?;
        checker.add_dictionary_word(word.trim());
    }
    let text = read_line()?;
    println!("{}", checker.count_errors(&text));
    Ok(())
}

fn run_huffman() -> Result<(), String> {
    let text = read_line()?;
    let huffman = Huffman::new(&text);
    println!("{}", huffman.encode());
    Ok(())
}

fn run_manager() -> Result<(), String> {
    let count = read_number(&read_line()?)?;
    let mut counter = EmployeeCounter::new(HashMethod::LinearProbing);
    for _ in 0..count {
        let line = read_line()?;
        let mut words = line.split_whitespace();
        let employee = words.next().ok_or("Ожидалась пара сотрудник менеджер.")?;
        let manager = words.next().ok_or("Ожидалась пара сотрудник менеджер.")?;
        counter.add_pair(employee, manager);
    }
    println!("{}", counter.counts_text());
    Ok(())
}

fn run_lru() -> Result<(), String> {
    let line = read_line()?;
    let mut numbers = line.split_whitespace();
    let capacity = read_number(numbers.next().ok_or("Ожидались ёмкость и число запросов.")?)?;
    let count = read_number(numbers.next().ok_or("Ожидались ёмкость и число запросов.")?)?;
    let mut cache = LruCache::new(capacity);
    for _ in 0..count {
        let line = read_line()?;
        let mut words = line.split_whitespace();
        let command = words.next().ok_or("Пустой запрос.")?;
        let key: i32 = words.next().ok_or("Нет ключа.")?
            .parse().map_err(|_| "Неверный ключ.")?;
        match command {
            "SET" => {
                let value: i32 = words.next().ok_or("Нет значения.")?
                    .parse().map_err(|_| "Неверное значение.")?;
                cache.set(key, value);
            }
            "GET" => println!("{}", cache.get(key)),
            _ => return Err("Неизвестный запрос к кэшу.".to_string()),
        }
    }
    Ok(())
}

fn skip_spaces(chars: &[char], position: &mut usize) {
    while *position < chars.len() && chars[*position].is_whitespace() {
        *position += 1;
    }
}

fn read_json_string(chars: &[char], position: &mut usize) -> Result<String, String> {
    if chars.get(*position) != Some(&'"') {
        return Err("Ожидалась строка в JSON-массиве.".to_string());
    }
    *position += 1;
    let mut result = String::new();
    while let Some(&symbol) = chars.get(*position) {
        *position += 1;
        if symbol == '"' {
            return Ok(result);
        }
        if symbol == '\\' {
            let escaped = *chars.get(*position).ok_or("Незакрытая строка в JSON.")?;
            *position += 1;
            match escaped {
                'n' => result.push('\n'),
                'r' => result.push('\r'),
                't' => result.push('\t'),
                '"' | '\\' | '/' => result.push(escaped),
                _ => return Err("Неверное экранирование в JSON.".to_string()),
            }
        } else {
            result.push(symbol);
        }
    }
    Err("Незакрытая строка в JSON.".to_string())
}

fn read_set(path: &str) -> Result<Set, String> {
    if !std::path::Path::new(path).exists() {
        fs::write(path, "[]\n").map_err(|error| error.to_string())?;
    }
    let json = fs::read_to_string(path).map_err(|error| error.to_string())?;
    let chars: Vec<char> = json.chars().collect();
    let mut position = 0;
    skip_spaces(&chars, &mut position);
    if chars.get(position) != Some(&'[') {
        return Err("Ожидался JSON-массив строк.".to_string());
    }
    position += 1;
    let mut set = Set::new();
    loop {
        skip_spaces(&chars, &mut position);
        if chars.get(position) == Some(&']') {
            position += 1;
            break;
        }
        let value = read_json_string(&chars, &mut position)?;
        set.add(&value);
        skip_spaces(&chars, &mut position);
        if chars.get(position) == Some(&',') {
            position += 1;
        } else if chars.get(position) != Some(&']') {
            return Err("Ошибка между элементами JSON-массива.".to_string());
        }
    }
    skip_spaces(&chars, &mut position);
    if position != chars.len() {
        return Err("Лишние данные после JSON-массива.".to_string());
    }
    Ok(set)
}

fn write_json_string(value: &str) -> String {
    let mut result = String::from("\"");
    for symbol in value.chars() {
        match symbol {
            '"' => result.push_str("\\\""),
            '\\' => result.push_str("\\\\"),
            '\n' => result.push_str("\\n"),
            '\r' => result.push_str("\\r"),
            '\t' => result.push_str("\\t"),
            _ => result.push(symbol),
        }
    }
    result.push('"');
    result
}

fn save_set(path: &str, set: &Set) -> Result<(), String> {
    let values: Vec<String> = set.values().iter().map(|value| write_json_string(value)).collect();
    let json = format!("[{}]\n", values.join(","));
    fs::write(path, json).map_err(|error| error.to_string())
}

fn run_file(args: &[String]) -> Result<(), String> {
    if args.len() < 5 || args.len() % 2 == 0 || args[1] != "--file" {
        return Err("Использование: LABA2 --file data.json --query \"SETADD apple\"".to_string());
    }
    let path = &args[2];
    if !path.ends_with(".json") {
        return Err("Нужен файл с расширением .json.".to_string());
    }
    for index in (3..args.len()).step_by(2) {
        if args[index] != "--query" {
            return Err("Ожидался параметр --query.".to_string());
        }
    }
    let mut set = read_set(path)?;
    for index in (4..args.len()).step_by(2) {
        let query = &args[index];
        let mut parts = query.splitn(2, char::is_whitespace);
        let command = parts.next().unwrap_or("");
        let value = parts.next().unwrap_or("").trim_start();
        if value.is_empty() {
            return Err("Не указан элемент множества.".to_string());
        }
        let changed = match command {
            "SETADD" => set.add(value),
            "SETDEL" => set.remove(value),
            "SET_AT" => { set.contains(value); false },
            _ => return Err("Неизвестный запрос к множеству.".to_string()),
        };
        if changed {
            save_set(path, &set)?;
        }
    }
    Ok(())
}

pub fn run() -> Result<(), String> {
    let args: Vec<String> = env::args().collect();
    if args.len() > 1 && args[1] == "--file" {
        return run_file(&args);
    }
    if args.len() != 2 {
        return Err("Использование: LABA2 POSTFIX|GENOM|STRESS|HUFFMAN|MANAGER|LRU".to_string());
    }
    match get_task(&args[1]) {
        Some(Task::Postfix) => run_postfix(),
        Some(Task::Genom) => run_genom(),
        Some(Task::Stress) => run_stress(),
        Some(Task::Huffman) => run_huffman(),
        Some(Task::Manager) => run_manager(),
        Some(Task::Lru) => run_lru(),
        None => Err("Неизвестная команда.".to_string()),
    }
}
