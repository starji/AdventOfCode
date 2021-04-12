
fn part(side: char, range: (u32, u32)) -> (u32, u32) {
    let (min, max) = range;
    let mid = ((max - min) / 2) + min;
    match side {
        'F' | 'L' => (min, mid),
        'B' | 'R' => (mid + 1, max),
        _ => (min, max)
    }
}

fn seat_row_col(seat: &str) -> (u32, u32) {
    let row_str = &seat[..7];
    let col_str = &seat[7..];

    let mut rows = (0, 127);
    for r in row_str.chars() {
        rows = part(r, rows);
    }

    let mut cols = (0, 7);
    for c in col_str.chars() {
        cols = part(c, cols);
    }

    let (rvalue, _) = rows;
    let (cvalue, _) = cols;

    return (rvalue, cvalue);
}

fn seat_id(seat: (u32, u32)) -> u32 {
    let (row, col) = seat;
    return row * 8 + col;
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let mut seat_ids: Vec<u32> = contents.trim().lines().map(|x| seat_id(seat_row_col(x))).collect();

    seat_ids.sort();

    let mut check: u32 = *seat_ids.first().unwrap ();
    for id in &seat_ids {
        if check != *id
        {
            println!("Found mismatch, had {} expected {}", id, check);
            break;
        }
        else
        {
            check = check + 1;
        }
    }

//    println!("Max: {}", seat_ids.max().unwrap());
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn identity() {
        assert!(true);
    }

    #[test]
    fn seat_ids() {

        assert_eq!(357, seat_id(seat_row_col("FBFBBFFRLR")));
        assert_eq!(567, seat_id(seat_row_col("BFFFBBFRRR")));
        assert_eq!(119, seat_id(seat_row_col("FFFBBBFRRR")));
        assert_eq!(820, seat_id(seat_row_col("BBFFBBFRLL")));
        
    }
}
