use crate::structures::array::Array;
use crate::structures::hash_table::{HashMethod, HashTable};

pub struct EmployeeCounter {
    managers: HashTable,
    counts: HashTable,
    employees: Array<String>,
    counted: bool,
}

impl EmployeeCounter {
    pub fn new(method: HashMethod) -> Self {
        Self {
            managers: HashTable::new(method),
            counts: HashTable::new(method),
            employees: Array::new(),
            counted: false,
        }
    }

    fn add_employee(&mut self, name: &str) {
        if self.counts.get(name).is_some() {
            return;
        }
        self.employees.push(name.to_string());
        self.counts.add(name, "0");
    }

    pub fn add_pair(&mut self, employee: &str, manager: &str) {
        self.add_employee(employee);
        self.add_employee(manager);
        self.managers.add(employee, manager);
        self.counted = false;
    }

    fn calculate(&mut self) {
        for index in 0..self.employees.len() {
            self.counts.add(self.employees.get(index).unwrap(), "0");
        }
        for index in 0..self.employees.len() {
            let mut current = self.employees.get(index).unwrap().clone();
            while let Some(manager) = self.managers.get(&current) {
                if manager == current {
                    break;
                }
                let manager = manager.to_string();
                let count: usize = self.counts.get(&manager).unwrap().parse().unwrap();
                self.counts.add(&manager, &(count + 1).to_string());
                current = manager;
            }
        }
        self.counted = true;
    }

    pub fn get_count(&mut self, name: &str) -> Option<usize> {
        if !self.counted {
            self.calculate();
        }
        self.counts.get(name)?.parse().ok()
    }

    pub fn counts_text(&mut self) -> String {
        if !self.counted {
            self.calculate();
        }
        let mut parts = Vec::new();
        for index in 0..self.employees.len() {
            let name = self.employees.get(index).unwrap().clone();
            parts.push(format!("{}-{}", name, self.get_count(&name).unwrap()));
        }
        parts.join(", ")
    }
}
