// Массив с теми же основными действиями, что и MArray в C++.
pub struct Array<T> {
    data: Vec<T>,
}

impl<T> Array<T> {
    pub fn new() -> Self {
        Self { data: Vec::new() }
    }

    pub fn len(&self) -> usize {
        self.data.len()
    }

    pub fn push(&mut self, value: T) {
        self.data.push(value);
    }

    pub fn insert(&mut self, index: usize, value: T) {
        self.data.insert(index, value);
    }

    pub fn get(&self, index: usize) -> Option<&T> {
        self.data.get(index)
    }

    pub fn remove(&mut self, index: usize) -> T {
        self.data.remove(index)
    }

    pub fn sort_by<F>(&mut self, compare: F)
    where
        F: FnMut(&T, &T) -> std::cmp::Ordering,
    {
        self.data.sort_by(compare);
    }
}
