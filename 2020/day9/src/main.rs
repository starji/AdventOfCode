#[macro_use] extern crate itertools;
use itertools::Itertools;

fn is_valid(preamble: &[u64], value: u64) -> bool {
    for (i, j) in iproduct!(preamble.iter(), preamble.iter()) {
        if i != j && i + j == value {
            return true;
        }
    }
    return false;
}

fn find_first_invalid(buffer: &[u64], preamble_count: usize) -> (usize, u64) {
    let mut index = 0;
    while buffer.get(index + preamble_count).is_some() {
        let check_value = buffer[index + preamble_count];
        if !is_valid (&buffer[index..index+preamble_count], check_value) {
            println!("found mismatch = {}", check_value);
            return (index + preamble_count, check_value);
        }
        index = index + 1;
    }
    return (0, 0);
}

fn find_contiguous_sum(buffer: &[u64], value: u64) -> (usize, &[u64]) {
    let mut length = 3;

    while length < buffer.len() {

        let mut index = 0;
        while buffer.get(index..index + length).is_some() {
            let check_slice = &buffer[index..index+length];
            if value == check_slice.iter().fold(0, |acc, x| acc + *x) {
                println!("Found match at {} length {}", index, length);
                return (index, check_slice);
            }
            index = index + 1;
        }

        length = length + 1;
    }
    return (0, buffer);
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let numbers: Vec<u64> = contents.trim().lines().map(|x| x.parse::<u64>().unwrap()).collect();

    let preamble_count = 25;

    let (index, value) = find_first_invalid(&numbers[..], preamble_count);
    println!("Found first invalid at {} value {}", index, value);

    let (found_index, found_slice) = find_contiguous_sum(&numbers[..index], value);
    println!("Found match for value {} at {} length {} : {:?}", value, found_index, found_slice.len(), found_slice);

    let min = found_slice.iter().min().unwrap();
    let max = found_slice.iter().max().unwrap();
    println!("Min={} Max={} Sum={}", min, max, min + max);

}
