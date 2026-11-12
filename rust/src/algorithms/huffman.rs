use crate::structures::array::Array;

struct Node {
    symbol: u8,
    frequency: usize,
    left: Option<usize>,
    right: Option<usize>,
}

pub struct Huffman {
    nodes: Vec<Node>,
    root: Option<usize>,
    codes: Vec<String>,
    input: String,
}

impl Huffman {
    pub fn new(input: &str) -> Self {
        let mut result = Self {
            nodes: Vec::new(),
            root: None,
            codes: vec![String::new(); 256],
            input: input.to_string(),
        };
        result.build_tree();
        if let Some(root) = result.root {
            result.build_codes(root, String::new());
        }
        result
    }

    fn insert_sorted(&self, free_nodes: &mut Array<usize>, new_id: usize) {
        let new_frequency = self.nodes[new_id].frequency;
        let mut position = 0;
        while position < free_nodes.len() {
            let current_id = *free_nodes.get(position).unwrap();
            if new_frequency > self.nodes[current_id].frequency {
                break;
            }
            position += 1;
        }
        free_nodes.insert(position, new_id);
    }

    fn take_min(free_nodes: &mut Array<usize>) -> usize {
        free_nodes.remove(free_nodes.len() - 1)
    }

    fn build_tree(&mut self) {
        let mut frequencies = [0usize; 256];
        for byte in self.input.bytes() {
            frequencies[byte as usize] += 1;
        }
        let mut free_nodes = Array::new();
        for (symbol, frequency) in frequencies.iter().enumerate() {
            if *frequency == 0 {
                continue;
            }
            let id = self.nodes.len();
            self.nodes.push(Node {
                symbol: symbol as u8,
                frequency: *frequency,
                left: None,
                right: None,
            });
            self.insert_sorted(&mut free_nodes, id);
        }
        while free_nodes.len() > 1 {
            let left = Self::take_min(&mut free_nodes);
            let right = Self::take_min(&mut free_nodes);
            let id = self.nodes.len();
            self.nodes.push(Node {
                symbol: 0,
                frequency: self.nodes[left].frequency + self.nodes[right].frequency,
                left: Some(left),
                right: Some(right),
            });
            self.insert_sorted(&mut free_nodes, id);
        }
        if free_nodes.len() == 1 {
            self.root = Some(Self::take_min(&mut free_nodes));
        }
    }

    fn build_codes(&mut self, id: usize, path: String) {
        let node = &self.nodes[id];
        if node.left.is_none() && node.right.is_none() {
            self.codes[node.symbol as usize] = if path.is_empty() { "0".to_string() } else { path };
            return;
        }
        if let Some(left) = node.left {
            self.build_codes(left, format!("{}0", path));
        }
        if let Some(right) = self.nodes[id].right {
            self.build_codes(right, format!("{}1", path));
        }
    }

    pub fn encode(&self) -> String {
        let mut result = String::new();
        for byte in self.input.bytes() {
            result.push_str(&self.codes[byte as usize]);
        }
        result
    }

    pub fn decode(&self, bits: &str) -> String {
        let mut bytes = Vec::new();
        let Some(root) = self.root else { return String::new() };
        if self.nodes[root].left.is_none() {
            for bit in bits.bytes() {
                if bit == b'0' {
                    bytes.push(self.nodes[root].symbol);
                }
            }
            return String::from_utf8_lossy(&bytes).into_owned();
        }
        let mut current = root;
        for bit in bits.bytes() {
            current = if bit == b'0' {
                self.nodes[current].left.unwrap()
            } else {
                self.nodes[current].right.unwrap()
            };
            if self.nodes[current].left.is_none() {
                bytes.push(self.nodes[current].symbol);
                current = root;
            }
        }
        String::from_utf8_lossy(&bytes).into_owned()
    }
}

#[cfg(test)]
mod tests {
    use super::Huffman;

    #[test]
    fn round_trip_for_empty_single_and_mixed_text() {
        for text in ["", "aaaa", "abracadabra", "Привет"] {
            let huffman = Huffman::new(text);
            assert_eq!(huffman.decode(&huffman.encode()), text);
        }
    }
}
