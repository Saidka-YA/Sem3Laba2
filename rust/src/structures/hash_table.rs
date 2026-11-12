#[derive(Clone, Copy)]
pub enum HashMethod {
    LinearProbing,
    DoubleHashing,
}

struct Cell {
    key: String,
    value: String,
    occupied: bool,
    deleted: bool,
}

impl Cell {
    fn empty() -> Self {
        Self {
            key: String::new(),
            value: String::new(),
            occupied: false,
            deleted: false,
        }
    }
}

pub struct HashTable {
    cells: Vec<Cell>,
    size: usize,
    used: usize,
    method: HashMethod,
}

fn hash(key: &str, multiplier: usize) -> usize {
    let mut result = 0usize;
    for byte in key.bytes() {
        result = result.wrapping_mul(multiplier).wrapping_add(byte as usize);
    }
    result
}

fn is_prime(number: usize) -> bool {
    if number < 2 {
        return false;
    }
    let mut divisor = 2;
    while divisor <= number / divisor {
        if number % divisor == 0 {
            return false;
        }
        divisor += 1;
    }
    true
}

impl HashTable {
    pub fn new(method: HashMethod) -> Self {
        let mut cells = Vec::new();
        for _ in 0..17 {
            cells.push(Cell::empty());
        }
        Self { cells, size: 0, used: 0, method }
    }

    fn probe(&self, key: &str, attempt: usize) -> usize {
        let capacity = self.cells.len();
        let start = hash(key, 31) % capacity;
        let step = match self.method {
            HashMethod::LinearProbing => 1,
            HashMethod::DoubleHashing => 1 + hash(key, 37) % (capacity - 1),
        };
        (start + attempt * step) % capacity
    }

    fn rehash(&mut self) {
        let mut capacity = self.cells.len() * 2 + 1;
        while !is_prime(capacity) {
            capacity += 2;
        }
        let mut bigger = HashTable::new(self.method);
        bigger.cells.clear();
        for _ in 0..capacity {
            bigger.cells.push(Cell::empty());
        }
        for cell in &self.cells {
            if cell.occupied {
                bigger.add(&cell.key, &cell.value);
            }
        }
        *self = bigger;
    }

    pub fn add(&mut self, key: &str, value: &str) {
        if (self.used + 1) * 10 > self.cells.len() * 7 {
            self.rehash();
        }
        let mut first_deleted = None;
        for attempt in 0..self.cells.len() {
            let index = self.probe(key, attempt);
            let cell = &self.cells[index];
            if cell.occupied {
                if cell.key == key {
                    self.cells[index].value = value.to_string();
                    return;
                }
            } else if cell.deleted {
                if first_deleted.is_none() {
                    first_deleted = Some(index);
                }
            } else {
                let target = first_deleted.unwrap_or(index);
                self.cells[target].key = key.to_string();
                self.cells[target].value = value.to_string();
                self.cells[target].occupied = true;
                self.cells[target].deleted = false;
                self.size += 1;
                if first_deleted.is_none() {
                    self.used += 1;
                }
                return;
            }
        }
        if let Some(index) = first_deleted {
            self.cells[index].key = key.to_string();
            self.cells[index].value = value.to_string();
            self.cells[index].occupied = true;
            self.cells[index].deleted = false;
            self.size += 1;
        }
    }

    pub fn get(&self, key: &str) -> Option<&str> {
        for attempt in 0..self.cells.len() {
            let cell = &self.cells[self.probe(key, attempt)];
            if !cell.occupied && !cell.deleted {
                return None;
            }
            if cell.occupied && cell.key == key {
                return Some(&cell.value);
            }
        }
        None
    }

    pub fn remove(&mut self, key: &str) -> bool {
        for attempt in 0..self.cells.len() {
            let index = self.probe(key, attempt);
            let cell = &mut self.cells[index];
            if !cell.occupied && !cell.deleted {
                return false;
            }
            if cell.occupied && cell.key == key {
                cell.key.clear();
                cell.value.clear();
                cell.occupied = false;
                cell.deleted = true;
                self.size -= 1;
                return true;
            }
        }
        false
    }
}

#[cfg(test)]
mod tests {
    use super::{HashMethod, HashTable};

    #[test]
    fn both_methods_keep_colliding_keys_after_deletion() {
        for method in [HashMethod::LinearProbing, HashMethod::DoubleHashing] {
            let mut table = HashTable::new(method);
            for index in 0..100 {
                table.add(&format!("key{}", index), &index.to_string());
            }
            for index in (0..100).step_by(2) {
                assert!(table.remove(&format!("key{}", index)));
            }
            for index in 0..100 {
                let expected = if index % 2 == 0 { None } else { Some(index.to_string()) };
                assert_eq!(table.get(&format!("key{}", index)), expected.as_deref());
            }
        }
    }
}
