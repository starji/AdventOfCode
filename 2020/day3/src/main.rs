

fn check_slope(contents: &String, x: usize, y: usize) -> usize {
    let mut x_offset: usize = 0;
    let mut y_offset: usize = 0;
    let mut count: usize = 0;
    for l in contents.split_whitespace() {
        if y_offset % y == 0 {
            if '#' == l.chars().nth(x_offset).unwrap()
            {
                count += 1;
            }

            x_offset = (x_offset + x) % l.len();
        }
        y_offset = y_offset + 1;
    }

    return count;
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let counts = vec![check_slope(&contents, 1, 1),
                      check_slope(&contents, 3, 1),
                      check_slope(&contents, 5, 1),
                      check_slope(&contents, 7, 1),
                      check_slope(&contents, 1, 2)];

    println!("{:?}", counts);

    let mult_total = counts.iter().fold(1, |x,y| x * y);
    println!("{}", mult_total);
}
