use crate::structures::array::Array;
use crate::structures::hash_table::{HashMethod, HashTable};

pub struct LruCache {
    capacity: usize,
    values: HashTable,
    order: Array<String>,
}

impl LruCache {
    pub fn new(capacity: usize) -> Self {
        Self {
            capacity,
            values: HashTable::new(HashMethod::LinearProbing),
            order: Array::new(),
        }
    }

    fn make_recent(&mut self, key: &str) {
        for index in 0..self.order.len() {
            if self.order.get(index).unwrap() == key {
                let value = self.order.remove(index);
                self.order.push(value);
                return;
            }
        }
    }

    pub fn set(&mut self, key: i32, value: i32) {
        if self.capacity == 0 {
            return;
        }
        let key = key.to_string();
        if self.values.get(&key).is_some() {
            self.values.add(&key, &value.to_string());
            self.make_recent(&key);
            return;
        }
        if self.order.len() == self.capacity {
            let oldest = self.order.remove(0);
            self.values.remove(&oldest);
        }
        self.values.add(&key, &value.to_string());
        self.order.push(key);
    }

    pub fn get(&mut self, key: i32) -> i32 {
        let key = key.to_string();
        let Some(value) = self.values.get(&key) else { return -1 };
        let result = value.parse().unwrap();
        self.make_recent(&key);
        result
    }
}
