use unicode_segmentation::*;

fn test_password(pass: &String, c: char, min: usize, max: usize) -> bool {
    let char_count = pass.chars().filter(|x| *x == c).count ();
    
    return char_count >= min && char_count <= max;
}

fn test_password2(pass: &String, c: char, p1: usize, p2: usize) -> bool {
    let idx1 = p1 - 1; // indices are not zero based
    let idx2 = p2 - 1;

    let c1 = pass.chars().nth(idx1).unwrap();
    let c2 = pass.chars().nth(idx2).unwrap();

    return (c1 == c || c2 == c) && (c1 != c2);
}

fn parse_line(line: String) -> (String, char, usize, usize) {
    let words: Vec<String> = line.unicode_words().map(|x| String::from(x)).collect();
    let min: usize = words[0].parse().unwrap();
    let max: usize = words[1].parse().unwrap();
    let c: char = words[2].parse().unwrap();
    let password: String = words[3].parse().unwrap();

    return (password, c, min, max);
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("file not found");

    let values = contents.split('\n').filter(|x| !x.is_empty()).map(|x| parse_line(x.to_string()));

    let count = values.filter(|(pass, c, min, max)| test_password2(pass, *c, *min, *max)).count();

    println!("{}", count);
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn identity() {
        assert!(true);
    }

    #[test]
    fn pass() {
        assert!(test_password(&"abc".to_string(), 'a', 1, 3));
    }

    #[test]
    fn fail() {
        assert!(test_password(&"xyz".to_string(), 'a', 1, 5) == false);
    }

    #[test]
    fn part2() {
        assert!(test_password2(&"abcde".to_string(), 'a', 1, 3) == true);
        assert!(test_password2(&"cdefg".to_string(), 'b', 1, 3) == false);
        assert!(test_password2(&"ccccccccc".to_string(), 'c', 2, 9) == false);
    }

}
