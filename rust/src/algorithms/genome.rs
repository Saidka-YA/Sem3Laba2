use crate::structures::set::Set;

pub struct GenomeComparator {
    first: String,
    second: String,
}

impl GenomeComparator {
    pub fn new(first: &str, second: &str) -> Self {
        Self { first: first.to_string(), second: second.to_string() }
    }

    pub fn closeness(&self) -> usize {
        let mut pairs = Set::new();
        for pair in self.second.as_bytes().windows(2) {
            pairs.add(std::str::from_utf8(pair).unwrap());
        }
        let mut result = 0;
        for pair in self.first.as_bytes().windows(2) {
            if pairs.contains(std::str::from_utf8(pair).unwrap()) {
                result += 1;
            }
        }
        result
    }
}
