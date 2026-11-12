use crate::structures::array::Array;

pub struct StressChecker {
    dictionary: Array<String>,
    sorted: bool,
}

fn normalize(word: &str) -> String {
    word.to_ascii_lowercase()
}

fn count_stresses(word: &str) -> usize {
    word.bytes().filter(|byte| byte.is_ascii_uppercase()).count()
}

impl StressChecker {
    pub fn new() -> Self {
        Self { dictionary: Array::new(), sorted: true }
    }

    pub fn add_dictionary_word(&mut self, word: &str) {
        self.dictionary.push(word.to_string());
        self.sorted = false;
    }

    fn sort_dictionary(&mut self) {
        if !self.sorted {
            self.dictionary.sort_by(|left, right| {
                normalize(left).cmp(&normalize(right)).then(left.cmp(right))
            });
            self.sorted = true;
        }
    }

    fn is_error(&self, word: &str) -> bool {
        let normal = normalize(word);
        let mut left = 0;
        let mut right = self.dictionary.len();
        while left < right {
            let middle = left + (right - left) / 2;
            if normalize(self.dictionary.get(middle).unwrap()) < normal {
                left = middle + 1;
            } else {
                right = middle;
            }
        }
        let mut found = false;
        for index in left..self.dictionary.len() {
            let item = self.dictionary.get(index).unwrap();
            if normalize(item) != normal {
                break;
            }
            found = true;
            if item == word {
                return false;
            }
        }
        if found { true } else { count_stresses(word) != 1 }
    }

    pub fn count_errors(&mut self, text: &str) -> usize {
        self.sort_dictionary();
        text.split_whitespace().filter(|word| self.is_error(word)).count()
    }
}
