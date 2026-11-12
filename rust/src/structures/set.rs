struct Node {
    value: String,
    next: Option<Box<Node>>,
}

pub struct Set {
    buckets: Vec<Option<Box<Node>>>,
    count: usize,
}

fn hash(value: &str) -> usize {
    let mut result = 0usize;
    for byte in value.bytes() {
        result = result.wrapping_mul(31).wrapping_add(byte as usize);
    }
    result
}

impl Set {
    pub fn new() -> Self {
        let mut buckets = Vec::new();
        for _ in 0..16 {
            buckets.push(None);
        }
        Self { buckets, count: 0 }
    }

    pub fn contains(&self, value: &str) -> bool {
        let index = hash(value) % self.buckets.len();
        let mut node = self.buckets[index].as_ref();
        while let Some(current) = node {
            if current.value == value {
                return true;
            }
            node = current.next.as_ref();
        }
        false
    }

    fn rehash(&mut self) {
        let mut bigger = Vec::new();
        for _ in 0..self.buckets.len() * 2 {
            bigger.push(None);
        }
        for bucket in &mut self.buckets {
            while let Some(mut node) = bucket.take() {
                *bucket = node.next.take();
                let index = hash(&node.value) % bigger.len();
                node.next = bigger[index].take();
                bigger[index] = Some(node);
            }
        }
        self.buckets = bigger;
    }

    pub fn add(&mut self, value: &str) -> bool {
        if self.contains(value) {
            return false;
        }
        if (self.count + 1) * 4 > self.buckets.len() * 3 {
            self.rehash();
        }
        let index = hash(value) % self.buckets.len();
        let node = Box::new(Node {
            value: value.to_string(),
            next: self.buckets[index].take(),
        });
        self.buckets[index] = Some(node);
        self.count += 1;
        true
    }

    pub fn remove(&mut self, value: &str) -> bool {
        let index = hash(value) % self.buckets.len();
        let mut link = &mut self.buckets[index];
        loop {
            match link {
                Some(node) if node.value == value => {
                    *link = node.next.take();
                    self.count -= 1;
                    return true;
                }
                Some(node) => link = &mut node.next,
                None => return false,
            }
        }
    }

    pub fn values(&self) -> Vec<String> {
        let mut result = Vec::new();
        for bucket in &self.buckets {
            let mut node = bucket.as_ref();
            while let Some(current) = node {
                result.push(current.value.clone());
                node = current.next.as_ref();
            }
        }
        result
    }
}
