#[macro_use] extern crate itertools;


fn adjacent_cells(cells: &mut Vec<(i64, i64, i64)>) -> Vec<(i64, i64, i64)> {
    let mut result: Vec<(i64, i64, i64)> = cells.iter().map(|x| iproduct!(x.0-1..x.0+1, x.1-1..x.1+1, x.2-1..x.2+1).filter(|p| p != x).collect::<Vec<(i64, i64, i64)>>()).flatten().collect();
    result.sort();
    result.dedup();
    return result;
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let mut cells: Vec<(i64, i64, i64)> = Vec::new();

    

    
}
