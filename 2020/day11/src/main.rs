use std::convert::TryInto;

fn bounds(index: usize, boundary: usize) -> (usize, usize) {
    let min = match index.checked_sub(1) {
        Some(x) => x,
        None => index
    };
    let mut max = index + 2;
    if max >= boundary {
        max = boundary;
    }
    (min, max)
}

fn count_adjacent_occupied(field: &Vec<Vec<char> >, row: usize, col: usize) -> usize {
    let row_bounds = bounds(row, field.len());
    let rows = &field[row_bounds.0..row_bounds.1];

    let mut count: usize = 0;
    for r in rows {
        let col_bounds = bounds(col, r.len());
        let cols = &r[col_bounds.0..col_bounds.1];

        count = count + cols.iter().filter(|x| **x == '#').count();
    }
    if field[row][col] == '#' {
        count = match count.checked_sub(1) {
            Some(c) => c,
            None => 0
        }
    }
    count
}

fn count_diagonal_occupied(field: &Vec<Vec<char> >, row: i32, col: i32, delta_r: i32, delta_c: i32) -> usize {
    let r = row + delta_r;
    let c = col + delta_c;

    if r >= 0 && r < field.len().try_into().unwrap() {
        let r_usize: usize = r.try_into().unwrap();
        if c >= 0 && c < field[r_usize].len().try_into().unwrap()
        {
            let c_usize: usize = c.try_into().unwrap();
            return match field[r_usize][c_usize] {
                '#' => 1,
                '.' => count_diagonal_occupied(field, r, c, delta_r, delta_c),
                _ => 0,
            }
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

fn count_all_diagonal_occupied(field: &Vec<Vec<char> >, row: i32, col: i32) -> usize {
    count_diagonal_occupied(field, row, col, -1, -1) +
        count_diagonal_occupied(field, row, col, -1, 0) +
        count_diagonal_occupied(field, row, col, -1, 1) +
        count_diagonal_occupied(field, row, col, 0, -1) +
        count_diagonal_occupied(field, row, col, 0, 1) +
        count_diagonal_occupied(field, row, col, 1, -1) +
        count_diagonal_occupied(field, row, col, 1, 0) +
        count_diagonal_occupied(field, row, col, 1, 1)
}

fn state_transition(field: &Vec<Vec<char> >) -> (bool, Vec<Vec<char> >) {
    let mut changed = false;
    let result = field.iter().enumerate().map(|(row, line)| {
        line.iter().enumerate().map(|(col, c)| match c {
            'L' => {
                match count_all_diagonal_occupied(field, row.try_into().unwrap(), col.try_into().unwrap()) {
                    0 => { changed = true; '#' },
                    _ => 'L'
                }
            }
            '#' => {
                match count_all_diagonal_occupied(field, row.try_into().unwrap(), col.try_into().unwrap()) {
                    5..=8 => { changed = true; 'L' },
                    _ => '#'
                }
            }
            _ => '.'
        }).collect()
    }).collect();

    (changed, result)
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let mut field: Vec<Vec<char> > = contents.trim().lines().map(|x| x.chars().collect()).collect();


    let mut changed = true;
    while changed {
        // gotta be a better way to do this
        let (changed_result, field_result) = state_transition(&field);
        changed = changed_result;
        field = field_result;

    }

    let count = field.iter().fold(0, |acc, r| acc + r.iter().filter(|c| *c == &'#').count());
    println!("occupied count = {}", count);


}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn check_diagonal() {
        let test_field: Vec<Vec<char>> = r".......#.
...#.....
.#.......
.........
..#L....#
....#....
.........
#........
...#.....".to_string().trim().lines().map(|x| x.chars().collect()).collect();

        for r in &test_field {
            println!("{:?}", r);
        }

        assert_eq!(8, count_all_diagonal_occupied(&test_field, 4, 3));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, -1, -1));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, -1, 0));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, -1, 1));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, 0, -1));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, 0, 1));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, 1, -1));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, 1, 0));
        assert_eq!(1, count_diagonal_occupied(&test_field, 4, 3, 1, 1));
    }
}
